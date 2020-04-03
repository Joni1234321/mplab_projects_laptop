#ifndef CONF_C
#define CONF_C

#include <xc.h>
#include "conf.h"

// Init 
void init () {
    init_oscillator();
    init_ports();
    //init_interrupts();
    
    init_i2c();
}

// Select oscillator
void init_oscillator (void){
    
    // Internal oscillator
    OSCCONbits.SCS1 = 1;
    
    // Set frequency to 4 MHz
    OSCCONbits.IRCF = 0b1101;
}
// IPS = (4 MHz / 4 cycles/instruction) = 1,000,000 instructions pr second
// 1,000 kIPS = 1 MIPS
#define _XTAL_FREQ 4000000
#define IPS  1000000

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
    
    init_i2c (); 
}

// Activate interrupts
void init_interrupts (void){
    // Pin
    init_interrupt_pin();
    // Timer
    init_interrupt_timer();
    
    // Enable global interrupts
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
    
    // Increment based on internal instruction cycle (Fosc / 4)
    OPTION_REGbits.TMR0CS = 0;
    
    // Increment on low to high transition on T0CKl pin
    OPTION_REGbits.TMR0SE = 0;
    
    
    // Enable timer
    T0IE = 1;
    // Reset timer counter 
    TMR0 = 0;
    
}


// Activate i2c module
void init_i2c (void) {
    // Set RC3 and RC4 as input to enable I2C
    TRISC = 0b00011000;
    
    //init_i2c_master ();
    init_i2c_slave (0x30);
    
}

// Set as master
void init_i2c_master (void) {
    
    // <3:0> = 1000 : Set as master
    // <5>   = 1    : Enables I2C
    SSPCON1 = 0b00101000;

    // Default settings
    SSPCON2 = 0;
    
    // Set clock rate to 100 kHz
    // Calculated: Fosc / 4 * (SSPADD + 1) = 4 MHz / 4 * (9 + 1) = 100 kHz
    SSPADD = 9;   
    
    // Default settings
    SSPSTAT = 0;
    
}
// Set as slave
void init_i2c_slave (short address) {
    
    // <3:0> = 0110 : Set as slave with 7-bit
    // <4>   = 1    : Enable Clock
    // <5>   = 1    : Enables I2C
    SSPCON1 = 0b00110110;

    // <0>  = 1     : Enable clock stretching
    SSPCON2 = 0x01;
        
    // Set slave address
    SSPADD = 0x30;
    
    // Disable the slew rate control since 100 kHz is used
    SSPSTAT = 0x80;
    
    // Enable Global interrupts
    INTCONbits.GIE = 1;
    
    // Enable peripheral interrupts
    INTCONbits.PEIE = 1;
    
    // Clear flag 
    SSPIF = 0;
    // Enable Synchronous serial port interrupts
    SSPIE = 1;
}

#endif
