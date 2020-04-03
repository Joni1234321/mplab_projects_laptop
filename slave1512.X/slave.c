
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
#define _XTAL_FREQ 2000000


// Function prototypes
void PicWrite (char slaveAdd, char data);
void i2c_interrupt (void);
char PicRead (void);

volatile char readData; // The data read (global)

void __interrupt () interrupt_handler (){

    if  (SSPIF){
        i2c_interrupt();   
        PIR1bits.SSPIF = 0;     // Clear SSPIF bit

    }
}

void main (void){
    int x;
    
    // 2 MHz Clock
    OSCCONbits.IRCF = 0b1100;   
    
    // Output pins
    ANSELA = 0;
    TRISA = 0;
    LATA = 0;
    
    ANSELB = 0;
    TRISB = 0;
    LATBbits.LATB0 = 1;
    
    ANSELC = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
            
    // I2C
    // ADdress
    SSPADD = 0x30;
    // OR 0x00 What works idc
    SSPSTAT = 0x80;
    SSPCON1 = 0x36;
    SSPCON2 = 0x00;
    
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
    

    while (1);
}


void i2c_interrupt (void){
    LATBbits.LATB1 ^= 1;
    readData = PicRead ();
    LATA = readData;
}

char PicRead (void){
    char tempData;
    char add;

    
    PIR1bits.SSPIF = 0;     // Clear SSPIF bit

    // Address received
    add = SSPBUF;
    CKP = 1;
    
    LATBbits.LATB3 = 1;
    // Wait for data
    while (!SSPIF);
    SSPIF = 0;
    LATBbits.LATB4 = 1;
    
    // Data received 
    while (!SSPSTATbits.BF);// Wait for the receive to complete 
    tempData = SSPBUF;      // Save the data byte
    LATBbits.LATB5 = 1;
  
    CKP = 1;
    
    return tempData;
}