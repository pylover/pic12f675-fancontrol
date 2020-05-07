
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
#include "configuration.h"


#define FAN GP5
#define RANGE   (RISK_TEMP - LOW_TEMP)
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


enum {
    FANOFF,
    FANPWM,
    FANFULL,
};

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
    T0IE = 0;
    T0IF = 0;
    FAN = 1;
    fanstatus = FANFULL;
}

void fanpwm() {
    T0IF = 0;
    T0IE = 1;
    fanstatus = FANPWM;
}

void fanoff() {
    T0IE = 0;
    FAN = 0;
    fanstatus = FANOFF;
}


void post() {
    unsigned short counter = 0;
    
    // Dancing
    counter = 2; 
    while (counter > 0) {
        counter--;
        
        fanfull();
        _delay(13000);
        fanoff();
        _delay(900000);
    }

    // PWM test: Raise 
    counter = 255;  // Seconds
    fanpwm(); 
    while (counter > 0) {
        duty = 255 - counter;
        counter--;
        _delay(40000);
    }

    // Full speed test
    counter = 9;  // Seconds
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

        if ((fanstatus == FANOFF) && (adcvalue >= HIGH_TEMP)) {
            fanpwm();
        }
        else if ((fanstatus == FANPWM) && (adcvalue >= RISK_TEMP)) {
            fanfull();
        }
        else if ((fanstatus == FANFULL) && (adcvalue < RISK_TEMP)) {
            fanpwm();
        }
        else if ((fanstatus != FANOFF) && adcvalue < LOW_TEMP) {
            fanoff();
        }
        
        if (fanstatus == FANPWM) {
            d = adcvalue - LOW_TEMP;
            d *= 0xff;
            d /= RANGE;
            duty = (unsigned short)MAX(d, MINDUTY);
        }
        
        GO_nDONE = 1;   // ADC enable
        _delay(100000);
    }
    return 0;
}

