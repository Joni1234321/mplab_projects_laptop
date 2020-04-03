#ifndef CONF_C
#define CONF_C

#include <xc.h>
#include "conf.h"
#include "handler_io.h"

// Init 
void init () {
    init_oscillerator();
    init_ports();
    init_interrupts();
    

}

// Select oscillerator
void init_oscillerator (void){
    
    // Internal oscillerator
    OSCCONbits.SCS1 = 1;
    
    // Set frequency to 1 MHz
    OSCCONbits.IRCF = 0b1011;
}
// IPS = (500 kHz / 4 cycles/instruction) = 125,000 instructions pr second
// 125 kIPS = 0.125 MIPS
#define FREQ 1000000
#define IPS   250000

// Resets the registers that contain the port information
void reset_ports (void){
    // 0 is output  WRITE
    // 1 is input   READ
    // Set all ports to output
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;

    // Set outputs to 0
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;  
    
    // Disable all analog ports
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
}

// Activate ports
void init_ports (void){
    // Reset all ports
    reset_ports ();
    

    // RB0 RB1 RB2 RB3 as input
    TRISB = 0b00000111;
    
    // IO port
    port_init(TRISB);
}

// Activate interrupts
void init_interrupts (void){
    // Pin
    init_interrupt_pin();
    // Timer
    init_interrupt_timer();
    
    // Enable interrupts
    INTCONbits.GIE = 1;         
}

// Interrupt pin activate
void init_interrupt_pin (void){
    
    // Weak pull ups
    OPTION_REGbits.nWPUEN = 0;
    // Interrupt on the falling edge
    OPTION_REGbits.INTEDG = 0;  
    
    // Reset flag
    INTCONbits.INTF = 0;        
    // Enable the interrupt 
    INTCONbits.INTE = 1;
}

// Interrupt timer activate
void init_interrupt_timer (void){
    // Prescaler disabled
    OPTION_REGbits.PSA = 1;
    // Prescaler 1 : 2 
    OPTION_REGbits.PS = 0b000;
    
    // I don't know what this does, but for it to work this has to be declared to work
    OPTION_REGbits.TMR0CS = 0;
    
    // Increment on low to high transition on T0CKl pin
    OPTION_REGbits.TMR0SE = 0;
    
    
    // Enable timer
    T0IE = 1;
    // Reset timer counter 
    TMR0 = 0;
    
}

#endif