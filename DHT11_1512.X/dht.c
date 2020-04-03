/*
 * File:   dht.c
 * Author: Jonas
 *
 * Created on August 7, 2019, 5:34 PM
 */


#include <xc.h>
#define _XTAL_FREQ 2000000

#define DHT_DATA TRISCbits.TRISC0

// Global vars
volatile char temperature;
volatile char humidity;

// Prototype
char DhtData (void);


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
    // The data bit
    TRISCbits.TRISC0 = 0;
    
    while (1){
        LATBbits.LATB0 ^= 1;
        char r = DhtData ();
        if (r == 1) { LATBbits.LATB1 ^= 1; }
        else { LATBbits.LATB2 ^= 1; }
        PORTA = humidity;
        __delay_ms(1000);
    }

}

#define TIMEOUT 10000
char DhtData (void){
    
//    // Bytes received
//    char t_h;   // Temperature high
//    char t_l;   // Temperature low
//    char h_h;   // Humidity high
//    char h_l;   // Humidity low
//    
//    char parity;// The parity byte
//    
    char i = 0;
    char bits[5];
    for (i = 0; i < 5; i ++){
        bits [i] = 0;
    }
    char mask = 0x80;
    char idx = 0;
    
    // Request sample
    TRISCbits.TRISC0 = 0;   // Set to Output
    LATCbits.LATC0 = 0;     // Send Low
   
    __delay_ms (30);        // Wait at least 18 ms
    
    LATCbits.LATC0 = 1;     // Send High
    
    TRISCbits.TRISC0 = 1;   // Set to Input 
    
    
    // Get Acknowledge or timeout
    int loop = TIMEOUT;
    while(!PORTCbits.RC0){      // Wait for it to be held high
        //if (--loop == 0) return 0;
    }

    loop = TIMEOUT;
    while(PORTCbits.RC0){      // Wait for it to be held low
        //if (--loop == 0) return 0;
    }
    
   
    // Read data 40 bits
    for (char i = 40; i != 0; i--){
        // While it is high
        while (PORTCbits.RC0);
        // Wait for high
        loop = TIMEOUT;
        while (!PORTCbits.RC0) {
            //if (--loop == 0) return 0;
        }
        __delay_us(35);
//        loop = TIMEOUT;
//        while (PORTCbits.RC0) {
//            if (--loop == 0) return;
//        }
        
        // If bit is 1
        if (PORTCbits.RC0) {
            // Create the byte
            bits[idx] |=  mask;
        }
        // Go to next bit position
        mask >>= 1;
        // If it has been 8 bits, then 
        if (mask == 0) {
            mask = 0x80;
            idx++;
        }
        
    }
    
    // Stop bit 
    TRISCbits.TRISC0 = 1;  // High
    LATCbits.LATC0 = 1; // Output
    
    temperature = bits [0];
    humidity = bits [2];
    return 1;
    
//    char b; // byte
//    // Read current bit value
//    for (i = 0; i < 8; i++){
//        while(!PORTCbits.RC0);
//        __delay_us(35);
//        // Read bit
//        b = b << 1;
//
//        if(PORTCbits.RC0){      // If bit is 1 
//            b |= 0x01;          // Set bit  
//        }
//
//        while(PORTCbits.RC0);
//    }
//    // Check for parity
//    // Sum of all bytes = parity
//    char sum = t_h + t_l + h_h + h_l;
//    
//    
}