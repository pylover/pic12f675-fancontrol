
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

static int adcvalue = 0;
static unsigned char duty = 0;


void interrupt isr(void) {
    if (ADIF) {
        adcvalue = (ADRESH << 8) + ADRESL;
        ADIF = 0;
    } 
    if (T0IF) {
        if (FAN) {
            TMR0 = duty;
            FAN = 0;
        }
        else if (duty > 5) {
            TMR0 = 0xff - duty;
            FAN = 1;
        }
        T0IF = 0;
    }
}


int main() {
    TRISIO = 0b00000100;        // GP2: IN 
    OPTION_REG = 0b11010001;
    ANSEL = 0b00010100;         // GP2->AN2
    ADCON0 = 0b10001001;        // ADON, AN2, VDD
    CMCON = 0b00000111;
    GIE = 1;
    ADIE = 1;
    PEIE = 1;
    ADIF = 0;
    T0IE = 1;
    TMR0 = 0;
    T0IF = 0;
    
    max7219_init();
    GO_nDONE = 1;   // ADC enable
    while(1) {
        duty = adcvalue / 4; 
        display(duty, 3);
        GO_nDONE = 1;   // ADC enable
        _delay(1000000);
    }
    return 0;
}
