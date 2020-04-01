
// PIC12F675 Configuration Bit Settings

// 'C' source line config statements

// BEGIN CONFIG
#pragma config FOSC =  INTRCIO   // Oscillator Selection bits (RC oscillator: CLKOUT function on GP4/OSC2/CLKOUT pin, RC on GP5/OSC1/CLKIN)
#pragma config WDTE =  OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
// END CONFIG

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


//#define DUAL_SENSOR
#define DELAYED

#define MIN_DUTY            140
#define SAMPLE_INTERVAL     300000

#ifdef DELAYED
#define LIMIT_UPCOUNTER       10
#define LIMIT_DOWNCOUNTER    -30 
#endif

/*
k = lambda x: x * 1000
R2 = k(100)
f = lambda r: ((5/(r+R2)) * R2) * 1024 / 5
*/
#define MAX_TEMP        806   // 55°   27K    3.93 V
#define MIN_TEMP        578   // 30°   77K    2.82 V


#define FAN GP5
#define RANGE   (MAX_TEMP - MIN_TEMP)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


enum {
    FANOFF,
    FANPWM,
    FANFULL,
};

#ifdef DELAYED
static short downcounter = 0;
#endif

#ifdef DUAL_SENSOR
static unsigned short adcvalue_gp4 = 0;
#endif

static unsigned short adcvalue_gp2 = 0;
static unsigned char duty = 0;
static int fanstatus = FANOFF;


void setadcvalue(unsigned short v) {
#ifdef DUAL_SENSOR
    if (CHS0) {
        adcvalue_gp4 = v;
    }
    else {
        adcvalue_gp2 = v;
    }
#else
    adcvalue_gp2 = v;
#endif
}

void interrupt isr(void) {
    unsigned short adcvalue;
    if (ADIF) {
        adcvalue = (unsigned short)(ADRESH << 8);
        adcvalue += ADRESL;
        setadcvalue(adcvalue);
#ifdef DUAL_SENSOR
        CHS0 = !CHS0;
#endif
        ADIF = 0;
    } 
    
    // PWM Generation
    if ((fanstatus == FANPWM) && T0IF) {
        if (FAN) {
            TMR0 = duty;
            FAN = 0;
        }
        else if (duty > 10) {
            TMR0 = (unsigned char)(0xff - duty);
            FAN = 1;
        }
        T0IF = 0;
    }
}

void fanfull() {
    downcounter = 0;
    T0IE = 0;
    T0IF = 0;
    FAN = 1;
    fanstatus = FANFULL;
}

void fanpwm() {
#ifdef DELAYED
    if (++downcounter < LIMIT_UPCOUNTER) {
        return;
    }
    downcounter = 0;
#endif

    T0IF = 0;
    T0IE = 1;
    fanstatus = FANPWM;
}

void fanoff() {

#ifdef DELAYED
    if (--downcounter > LIMIT_DOWNCOUNTER) {
        return;
    }
    downcounter = 0;
#endif

    T0IE = 0;
    FAN = 0;
    fanstatus = FANOFF;
}


void post() {
    unsigned short counter = 0;
    
    // PWM test: Raise 
    counter = 255;  // Seconds
    fanpwm(); 
    while (counter > 0) {
        duty = 255 - counter;
        counter--;
        _delay(20000);
    }

    // Full speed test
    counter = 3;  // Seconds
    fanfull(); 
    while (counter > 0) {
        counter--;
        _delay(1000000);
    }
    fanoff();

}


int main() {

#ifdef DUAL_SENSOR
    TRISIO = 0b00010100;        // GP2: IN, GP4: IN
    ANSEL = 0b00111100;         // GP2->AN2, GP4->AN3
#else
    TRISIO = 0b00000100;        // GP2: IN
    ANSEL = 0b00110100;         // GP2->AN2
#endif

    OPTION_REG = 0b11010011;
    CMCON = 0b00000111;
    ADCON0 = 0b10001001;        // ADON, AN2, VDD
    VRCON = 0b00000000;

    GIE = 1;
    ADIE = 1;
    PEIE = 1;
    ADIF = 0;
    TMR0 = 0;
    
    // Power on self test
    post();
    GO_nDONE = 1;   // ADC enable
    long d = 0; 
    unsigned short adcvalue;
    while (1) {

#ifdef DUAL_SENSOR
        adcvalue = MAX(adcvalue_gp2, adcvalue_gp4);
#else
        adcvalue = adcvalue_gp2;
#endif

        if (adcvalue >= MAX_TEMP) {
            fanfull();
        }
        else if (adcvalue >= MIN_TEMP) {
            fanpwm();
        }
        else if (adcvalue < MIN_TEMP) {
            fanoff();
        }
        
        if (fanstatus == FANPWM) {
            d = adcvalue - MIN_TEMP;
            d *= 0xff;
            d /= RANGE;
            d = MAX(d, MIN_DUTY);
            duty = (unsigned short)d;
        }
        
        GO_nDONE = 1;   // ADC enable
        _delay(SAMPLE_INTERVAL);
    }
    return 0;
}




/*
50K Thermistor table

 °C       Ohm     | °C        Ohm    |  °C        Ohm    
-39.44    1956240 | 02.78     151235 |  45.00      20651
-38.33    1812199 | 03.89     142605 |  46.11      19716
-37.22    1679700 | 05.00     134519 |  47.22      18829
-36.11    1557748 | 06.11     126941 |  48.33      17987
-35.00    1445439 | 07.22     119834 |  49.44      17187
-33.89    1341952 | 08.33     113168 |  50.56      16421
-32.78    1246540 | 09.44     106912 |  51.67      15699
-31.67    1158525 | 10.50     100988 |  52.78      15013
-30.56    1077290 | 11.60      95475 |  53.89      14360
-29.44    1001621 | 12.70      90296 |  55.00      13740
-28.33     932353 | 13.80      85428 |  56.11      13150
-27.22     868317 | 15.00      80852 |  57.22      12588
-26.11     809086 | 16.10      76547 |  58.33      12053
-25.00     754271 | 17.20      72497 |  59.44      11544
-23.89     703517 | 18.30      68685 |  60.56      11055
-22.78     656499 | 19.40      65095 |  61.67      10593
-21.67     612919 | 20.50      61685 |  62.78      10154
-20.56     572506 | 21.60      58500 |  63.89       9734
-19.44     534686 | 22.70      55499 |  65.00       9335
-18.33     499905 | 23.80      52669 |  66.11       8954
-17.22     467604 | 25.00      50000 |  67.22       8590
-16.11     437592 | 26.10      47481 |  68.33       8243
-15.00     409692 | 27.20      45104 |  69.44       7912
-13.89     383745 | 28.30      42859 |  70.56       7593
-12.78     359601 | 29.40      40739 |  71.67       7292
-11.67     337126 | 30.50      38718 |  72.78       7004
-10.56     316194 | 31.60      36826 |  73.89       6729
-09.44     296522 | 32.70      35037 |  75.00       6466
-08.33     278353 | 33.80      33345 |  76.11       6215
-07.22     261408 | 35.00      31745 |  77.22       5975
-06.11     245599 | 36.10      30230 |  78.33       5745
-05.00     230842 | 37.20      28796 |  79.44       5526
-03.89     217062 | 38.30      27438 |  80.56       5314
-02.78     204189 | 39.40      26152 |  81.67       5113
-01.67     192156 | 40.50      24923 |  82.78       4921
-00.56     180906 | 41.60      23768 |  83.89       4737
 00.56     170291 | 42.70      22674 |  85.00       4561
 01.67     160449 | 43.80      21635 |  86.11       4392

100K Thermistor Table
 °C       Ohm     | °C        Ohm    |  °C        Ohm    

-39.44    3916295 | 2.78      302466 | 45.00      41303
-38.33    3627711 | 3.89      285206 | 46.11      39434
-37.22    3362274 | 5.00      269035 | 47.22      37660
-36.11    3117987 | 6.11      253877 | 48.33      35976
-35.00    2893035 | 7.22      239664 | 49.44      34376
-33.89    2685770 | 8.33      226331 | 50.56      32843
-32.78    2494694 | 9.44      213819 | 51.67      31399
-31.67    2318444 | 10.56     201971 | 52.78      30027
-30.56    2155781 | 11.67     190946 | 53.89      28722
-29.44    2004274 | 12.78     180588 | 55.00      27481
-28.33    1865595 | 13.89     170853 | 56.11      26300
-27.22    1737397 | 15.00     161700 | 57.22      25177
-26.11    1618827 | 16.11     153092 | 58.33      24107
-25.00    1509102 | 17.22     144992 | 59.44      23089
-23.89    1407512 | 18.33     137367 | 60.56      22111
-22.78    1313405 | 19.44     130189 | 61.67      21188
-21.67    1226184 | 20.56     123368 | 62.78      20308
-20.56    1145306 | 21.67     117000 | 63.89      19469
-19.44    1069620 | 22.78     110998 | 65.00      18670
-18.33    1000019 | 23.89     105338 | 66.11      17907
-17.22     935383 | 25.00     100000 | 67.22      17180
-16.11     875329 | 26.11      94963 | 68.33      16486
-15.00     819505 | 27.22      90208 | 69.44      15824
-13.89     767589 | 28.33      85719 | 70.56      15187
-12.78     719284 | 29.44      81479 | 71.67      14584
-11.67     674319 | 30.56      77438 | 72.78      14008
-10.56     632442 | 31.67      73654 | 73.89      13458
-9.44      593086 | 32.78      70076 | 75.00      12932
-8.33      556739 | 33.89      66692 | 76.11      12430
-7.22      522842 | 35.00      63491 | 77.22      11949
-6.11      491217 | 36.11      60461 | 78.33      11490
-5.00      461699 | 37.22      57594 | 79.44      11051
-3.89      434134 | 38.33      54878 | 80.56      10627
-2.78      408383 | 39.44      52306 | 81.67      10225
-1.67      384316 | 40.56      49847 | 82.78       9841
-0.56      361813 | 41.67      47538 | 83.89       9473
 0.56      340581 | 42.78      45349 | 85.00       9121
 1.67      320895 | 43.89      43273 | 86.11       8783
                    
*/


