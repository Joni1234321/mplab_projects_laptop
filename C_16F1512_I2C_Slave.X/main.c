/*
 * File:   main.c
 * Author: Jonas
 *
 * Created on August 4, 2019, 2:28 PM
 */


#include <xc.h>
#include "conf.h"

#define _XTAL_FREQ 4000000

short d;
void on_interrupt_i2c (void) {
    // Clock stretch
    CKP = 0;            
    // If its Data and Read
    if(!D_nA && !R_nW){ 
        // Read data first
        d = SSPBUF;     
        // Wait for buffer to empty
        while(!BF);     
        // Content of SSPBUF
        LATA = SSPBUF;  
        // Stop stretching
        CKP = 1;        
    }
    if(!D_nA && R_nW){ 
        // Read data first
        d = SSPBUF;     
        // Wait for buffer to empty
        while(!BF);     
        // Content of SSPBUF
        LATA = SSPBUF;  
        // Stop stretching
        CKP = 1;        
    }
}

void __interrupt() interruptHandler(void) {

    // I2C 
    if (SSPIE && SSPIF){
        LATCbits.LATC7 ^= 1;

        on_interrupt_i2c ();
        SSPIF = 0;
    }
}

void main(void) {
    init();
    LATCbits.LATC6 = 1;
    while (1);

}
