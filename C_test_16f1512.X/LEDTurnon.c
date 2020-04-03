/*
 * File:   LEDTurnon.c
 * Author: Jonas
 *
 * Created on June 2, 2019, 3:53 PM
 */

// PIC16F1512 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (VCAP pin function disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
// Define the frequency to correctly use the __delay_ms function
#define _XTAL_FREQ 500000

#include "fncts.h"
#include "loops.h"



void init (void) {
    // oscillator
    OSCCONbits.SCS1 = 1;
    // Set frequency to 500 kHz
    OSCCONbits.IRCF = 0b0111;
    
    // Set all ports to output
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;

    // Set outputs to 0
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;  
    
    // Disable all analog ports
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
    
    // RA1, RA2 and RA3 is output
    TRISA = 0b11110001;
    
    
    // INTERRUPT //
    INTCONbits.INTF = 0;        //reset the external interrupt flag
    OPTION_REGbits.INTEDG = 0;  //interrupt on the falling edge
    INTCONbits.INTE = 1;        //enable the external interrupt

    T0IE = 1;
    TMR0 = 0;
    OPTION_REG = 0B00001000;
    
    INTCONbits.GIE = 1;         //set the Global Interrupt Enable
}




unsigned char counter;
// Occurs every time the program has
void update (void) {

}

// Occurs evey 256 cycles
// High priority
void fixedUpdate (void){
    ++counter;
    if (counter == 0xFF){
        LATAbits.LATA3 = !LATAbits.LATA3;
        T0IF = 0;
        counter = 0x00;
        if (PORTAbits.RA4 == 0){
            btn_dn ();
        }
        else if (PORTAbits.RA4 == 1){
            btn_up ();
        }            
    }    
}

void __interrupt() interruptHandler(void) {
    // If flagged and enabled
    if (INTCONbits.INTE && INTCONbits.INTF){
        // Toggle LED
        toggle_RA2();
        
        // Reset
        INTCONbits.INTF  = 0;
    }
    
    // Timer
    if (T0IE && T0IF){
        fixedUpdate ();
        T0IF = 0;
    }
}

void main(void) {
    init();
    while(1) {
        update();
    }
    return;
}

