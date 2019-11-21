
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
#include "max7219.c"

#define FAN GP5
#define FT  90 
#define HT  60 
#define LT  55
#define RANGE   (FT - LT)

static unsigned short adcvalue = 0;
static unsigned char duty = 0;
static int fanstatus = 0;


void interrupt isr(void) {
    if (ADIF) {
        adcvalue = (unsigned short)(ADRESH << 8);
        adcvalue += ADRESL;
        adcvalue /= 10;
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
    ADCON0 = 0b10001001;        // ADON, AN2, VDD
    CMCON = 0b00000111;
    GIE = 1;
    ADIE = 1;
    PEIE = 1;
    ADIF = 0;
    TMR0 = 0;
    fanoff();
    max7219_init();
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
        display(adcvalue * 10, 1);
        GO_nDONE = 1;   // ADC enable
        _delay(500000);
    }
    return 0;
}
