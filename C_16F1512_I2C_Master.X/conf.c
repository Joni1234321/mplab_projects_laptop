#ifndef CONF_C
#define CONF_C

#include <xc.h>
#include "conf.h"

// Init 
void init () {
    init_oscillator();
    init_ports();
    
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
}


// Activate i2c module
void init_i2c (void) {
    // Set RC3 and RC4 as input to enable I2C
    TRISC = 0b00011000;
    
    init_i2c_master ();
}

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

#endif
