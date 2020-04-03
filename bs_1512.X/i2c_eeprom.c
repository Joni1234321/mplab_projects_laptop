#include <xc.h>
#include <stdint.h>

#include "i2c_eeprom.h"

void eeprom_page_write (uint8_t slaveAdd, uint16_t add, uint8_t *eeData, uint32_t eeDataSize){
    uint8_t addHigh = add >> 8;
    uint8_t addLow  = add;
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
    
    // PAGE WRITE
    for (uint32_t i = 0; i < eeDataSize; i++) {
        SSPBUF = eeData[i];    // Send the data byte
        while (!SSPIF);     // Wait for Acknowledge
        PIR1bits.SSPIF = 0; // Clear SSP interrupt flag
    }
    
    SSPCON2bits.PEN = 1;    // Initiate stop condition
    while (PEN);            // Wait for Stop condition to complete
}
uint8_t ee_seq_4[4];
uint8_t* eeprom_sequential_4_read (uint8_t slaveAdd, uint16_t add){
    uint8_t addHigh = add >> 8;
    uint8_t addLow  = add;
    
    SSPCON2bits.SEN = 1;    // Start
    while (SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return 0;           // Exit (No Acknowledge)
    }
    
    SSPBUF = addHigh;       // Send EEPROM Address High
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return 0;      // Exit (No Acknowledge)
    }

    SSPBUF = addLow;        // Send EEPROM Address Low
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return 0;      // Exit (No Acknowledge)
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
        return 0;      // Exit (No Acknowledge)
    }
        
    // Sequential read
    for (uint32_t i = 0; i < 4; i++) {
        SSPCON2bits.RCEN = 1;   // Receive the Data byte from slave
        while (!SSPSTATbits.BF);// Wait for the receive to complete 
        ee_seq_4[i] = SSPBUF;      // Save the data byte
    }
    
    SSPCON2bits.ACKDT = 1;  // Prepare to send NACK (because we dont want to read more data)
    SSPCON2bits.ACKEN = 1;  // Initiate NACK to eeprom
    while (ACKEN);          // Wait for Nack to Complete
    
    SSPCON2bits.PEN = 1;    // Initiate stop condition
    while(PEN);             // Wait for stop condition to complete
    return ee_seq_4;        // Return the data byte value
}


void eeprom_write_16 (uint8_t slaveAdd, uint16_t add, uint8_t eeData){
    uint8_t addHigh = add >> 8;
    uint8_t addLow  = add;
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
uint8_t eeprom_read_16 (uint8_t slaveAdd, uint16_t add){
    uint8_t addHigh = add >> 8;
    uint8_t addLow  = add;
    uint8_t tempData;
    
    SSPCON2bits.SEN = 1;    // Start
    while (SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFB);      // Exit (No Acknowledge)
    }
    
    SSPBUF = addHigh;       // Send EEPROM Address High
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFE);      // Exit (No Acknowledge)
    }

    SSPBUF = addLow;        // Send EEPROM Address Low
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFD);      // Exit (No Acknowledge)
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
        return (0xFC);      // Exit (No Acknowledge)
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

// EEPROM
void eeprom_write (uint8_t slaveAdd, uint8_t addHigh, uint8_t addLow, uint8_t eeData) {
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
uint8_t eeprom_read (uint8_t slaveAdd, uint8_t addHigh, uint8_t addLow){
    uint8_t tempData;
    
    SSPCON2bits.SEN = 1;    // Start
    while (SEN);            // Wait for start condition to complete 
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    SSPBUF = slaveAdd;      // Send Slave Address and R/W bit
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFB);      // Exit (No Acknowledge)
    }
    
    SSPBUF = addHigh;       // Send EEPROM Address High
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFE);      // Exit (No Acknowledge)
    }

    SSPBUF = addLow;        // Send EEPROM Address Low
    while (!SSPIF);         // Wait for Acknowledge
    PIR1bits.SSPIF = 0;     // Clear SSP interrupt flag
    if (SSPCON2bits.ACKSTAT) {
        SSPCON2bits.PEN = 1;// Initiate stop condition
        while (PEN);        // Wait for Stop condition to complete
        return (0xFD);      // Exit (No Acknowledge)
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
        return (0xFC);      // Exit (No Acknowledge)
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
