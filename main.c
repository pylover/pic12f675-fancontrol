
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

#define DISPLAY

#include <xc.h>

#ifdef DISPLAY
#include "max7219.c"
#endif

/*
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

  
*/


// R2: 47000
// lambda r: ((5/(r+R2)) * R2) * 1024 / 5
#define FAN GP5
#define FT  936   // 86°   4.3K
#define HT  829   // 60°   11K
#define LT  548   // 25°   50K 
#define RANGE   (FT - LT)


static unsigned short adcvalue = 0;
static unsigned char duty = 0;
static int fanstatus = 0;


void interrupt isr(void) {
    if (ADIF) {
        adcvalue = (unsigned short)(ADRESH << 8);
        adcvalue += ADRESL;
        //adcvalue /= 10;
        ADIF = 0;
    } 
    
    if ((fanstatus == 1) && T0IF) {
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
    T0IE = 0;
    T0IF = 0;
    FAN = 1;
    fanstatus = 2;
}

void fanon() {
    T0IF = 0;
    T0IE = 1;
    fanstatus = 1;
}

void fanoff() {
    T0IE = 0;
    FAN = 0;
    fanstatus = 0;
}


int main() {
    TRISIO = 0b00000100;        // GP2: IN 
    OPTION_REG = 0b11010011;
    ANSEL = 0b00110100;         // GP2->AN2
    CMCON = 0b00000111;
    ADCON0 = 0b10001001;        // ADON, AN2, VDD
    VRCON = 0b00000000;

    GIE = 1;
    ADIE = 1;
    PEIE = 1;
    ADIF = 0;
    TMR0 = 0;
    fanoff();

#ifdef DISPLAY
    max7219_init();
#endif

    GO_nDONE = 1;   // ADC enable
    long d = 0; 
    while (1) {
        if ((fanstatus == 0) && (adcvalue >= HT)) {
            fanon();
        }
        else if ((fanstatus == 1) && (adcvalue >= FT)) {
            fanfull();
        }
        else if ((fanstatus == 2) && (adcvalue < FT)) {
            fanon();
        }
        else if ((fanstatus != 0) && adcvalue < LT) {
            fanoff();
        }
        
        // TODO: ENUM
        if (fanstatus == 1) {
            d = adcvalue - LT;
            d *= 0xff;
            d /= RANGE;
            duty = (unsigned short)d;
        }

#ifdef DISPLAY
        display(adcvalue, 7);
#endif

        GO_nDONE = 1;   // ADC enable
        _delay(100000);
    }
    return 0;
}
