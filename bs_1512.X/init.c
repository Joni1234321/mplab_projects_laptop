#include <xc.h>
#include "init.h"

// Init 
void init (void) {
    init_oscillerator();
    init_ports();
    init_i2c();
    init_interrupt();
}

void init_oscillerator (void) {
    OSCCONbits.SCS1 = 1;         // Internal oscillerator
    OSCCONbits.IRCF = 0b1110;    // 8 MHz
}
void init_ports (void){
    res_ports ();             // Reset ports
    set_ports ();             // Set ports
}


void res_ports (void) {
    // OUTPUT
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x00;

    // LOW
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;  
    
    // NO ANALOG
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;
}

// SET ALL INPUT PORTS HERE
void set_ports (void) {
    // The switches
    TRISBbits.TRISB4 = 1;   // UP
    TRISBbits.TRISB5 = 1;   // DN
    TRISBbits.TRISB6 = 1;   // CAT_1
    TRISBbits.TRISB7 = 1;   // CAT_2    
    TRISCbits.TRISC5 = 1;   // RST
}

void init_i2c (void) {
    // Set ports 
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
    // I2C
    // DATASHEET: ((SSPADD + 1) * 4)/Fosc = Baud Rate
    // To get the frequency of that: f = Fosc / ((SSPADD + 1) * 4)
    // f = 100 khz, baud = 0.00001 seconds, Fosc = 8 MHz ...  ((Fosc / Baud rate) / 4) - 1 = ((8 MHz * 0.00001 s) /  4) - 1 = 19
    // 100 kHz I2C clock speed
    SSPADD = 19; 

    // Set as master <3:0> 1000
    // Enable SSPEN = 1
    SSPCON1 = 0x28;
}


void init_interrupt (void){
    
    init_interrupt_timer ();
    
    // Enable interrupts
    INTCONbits.GIE = 1;   
}

// Interrupt timer activate
void init_interrupt_timer (void){
    // Prescaler enabled
    OPTION_REGbits.PSA = 0;
    // Prescaler 1 : 256 
    OPTION_REGbits.PS = 0b100;
    
    // Internal clock cycle
    OPTION_REGbits.TMR0CS = 0;
    
    // Increment on low to high transition on T0CKl pin
    OPTION_REGbits.TMR0SE = 0;
    
    
    // Enable timer
    T0IE = 1;
    // Reset timer counter 
    TMR0 = 0;
    
}
