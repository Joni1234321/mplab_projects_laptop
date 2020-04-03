
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
void EepromWrite (char slaveAdd, char addHigh, char addLow, char eeData);
char EepromRead (char slaveAdd, char addHigh, char addLow);

void PicWrite (char slaveAdd, char data);


volatile char readData; // The data read (global)


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
    // 100 kHz I2C clock speed
    SSPADD = 4; 

    // Set as master <3:0> 1000
    // Enable SSPEN = 1
    SSPCON1 = 0x28;
    
    EepromWrite (0xA0, 0x12, 0x34, 0xAA);
    while(1){
    for (x = 1; x < 165; x++);
    readData = EepromRead (0xA0, 0x12, 0x34);
    LATA = readData;}
    /*PicWrite (0x30, 0x55);

    short v = 0;
    int i;
    while (1){
        v++;
        PicWrite (0x30, v);
        // 500 k
        for (x = 1; x < 10; x++) {for (i = 1; i < 1000;  i++);}
    }*/
    
}

void EepromWrite (char slaveAdd, char addHigh, char addLow, char eeData) {
    SSPCON2bits.SEN = 1;    // Start
    while (SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    
    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return;             // Exit (No Acknowledge)
    }
    
    SSPBUF = addHigh;       // Send EEPROM Address High
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return;             // Exit (No Acknowledge)
    }
        
    SSPBUF = addLow;        // Send EEPROM Address Low
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return;             // Exit (No Acknowledge)
    }
    
    SSPBUF = eeData;        // Send the data byte
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    
    SSPCON2bits.PEN = 1;// Initiate stop condition
    while (PEN);        // Wait for Stop condition to complete
    
}

char EepromRead (char slaveAdd, char addHigh, char addLow){
    char tempData;
    
    SSPCON2bits.SEN = 1;    // Start
    while (SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag

    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFF);             // Exit (No Acknowledge)
    }
    
    SSPBUF = addHigh;       // Send EEPROM Address High
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFF);      // Exit (No Acknowledge)
    }
        
    SSPBUF = addLow;        // Send EEPROM Address Low
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFF);      // Exit (No Acknowledge)
    }

    SSPCON2bits.RSEN = 1;   // Init restart condition
    while (RSEN);           // Wait for restart condition to complete
    PIR1bits.SSPIF = 0;     // Clear SSP Interrupt flag
    
    SSPBUF = (slaveAdd+1);  // Send the slave address and R/W bit = 1
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFF);      // Exit (No Acknowledge)
    }
    
    SSPCON2bits.RCEN = 1;   // Receive the Data byte from slave
    while (!SSPSTATbits.BF);// Wait for the receive to complete 
    tempData = SSPBUF;      // Save the data byte
    
    SSPCON2bits.ACKDT = 1;  // Prepare to send NACK (because we dont want to read more data)
    SSPCON2bits.ACKEN = 1;  // Initiate NACK to eeprom
    while (ACKEN);          // Wait for Nack to Complete
    
    SSPCON2bits.PEN = 1;    // Initiate stop condition
    while(PEN);             // Wait for stop condition to complete
    return tempData;        // Return the data byte value
    
}

void PicWrite (char slaveAdd, char data) {
    char tempData;

    SSPCON2bits.SEN = 1;    // Start
    while (SSPCON2bits.SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag

    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return;             // Exit (No Acknowledge)
    }

    SSPBUF = data;          // Send the data byte
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    
    SSPCON2bits.PEN = 1;// Initiate stop condition
    while (PEN);        // Wait for Stop condition to complete
}
