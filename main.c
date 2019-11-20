
// PIC12F675 Configuration Bit Settings

// 'C' source line config statements

// BEGIN CONFIG
#pragma config FOSC =  INTRCIO   // Oscillator Selection bits (RC oscillator: CLKOUT function on GP4/OSC2/CLKOUT pin, RC on GP5/OSC1/CLKIN)
#pragma config WDTE =  ON        // Watchdog Timer Enable bit (WDT enabled)
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
#define REF     5
#define INTERVAL    10

static unsigned short counter = 0;


float adc() {
    short adcvalue = (ADRESH << 8) + ADRESL;
    float v = ((float)adcvalue) * REF;
    return v / 1.024;
}


void delayus(int us) {
   while(us > 0) {
       _delay(10);
      us -= 10;
    }
}



int main() {
    TRISIO = 0b00000100;        // GP2: IN 
    OPTION_REG = 0b11111111;
    ANSEL = 0b01110100;         // OSC/16, GP2->AN2
    ADCON0 = 0b10001001;        // ADON, AN2, VDD
    CMCON = 0b00000111;
    //VRCON = 0b10101010;

    float temp;
    unsigned int duty;
    max7219_init();
    while(1) {
        if (counter % INTERVAL == 0) {
            GO_nDONE = 1;   // ADC enable
        }
        if (!GO_nDONE) {
            temp = adc();
            duty = (unsigned int)(temp * 1000);
            display((unsigned int)temp, 3);
            //display(duty, 0);
        }
        counter++;
        FAN = 0;
        delayus((1000000 - duty));
        FAN = 1;
        delayus(duty);
        CLRWDT();
    }
    return 0;
}
