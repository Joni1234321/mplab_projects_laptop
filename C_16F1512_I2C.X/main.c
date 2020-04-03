/*
 * File:   main.c
 * Author: Jonas
 *
 * Created on August 4, 2019, 2:28 PM
 */


#include <xc.h>
#include "conf.h"

#define _XTAL_FREQ 4000000

// RB0 PIN 
void on_interrupt_pin (void) {
    return;
}

void on_interrupt_timer (void){
    return;    
}

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
        PORTA = SSPBUF;  
        // Stop stretching
        CKP = 1;        
    }
}

void __interrupt() interruptHandler(void) {
    // Pin interrupts
    if (INTCONbits.INTE && INTCONbits.INTF){
        on_interrupt_pin ();
        INTCONbits.INTF  = 0;  
    }
    
    // Timer
    if (T0IE && T0IF){
        on_interrupt_timer();
        T0IF = 0;
    }
    
    // I2C 
    if (SSPIE && SSPIF){
        on_interrupt_i2c ();
        SSPIF = 0;
    }
}

void i2cWait(){
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
  return;
}

void i2cSend(unsigned int data){
    i2cWait();
    SEN = 1;           //start condition
    i2cWait();
    SSPBUF = 0x30;     //7-bit address of slave device plus 0 for write
    i2cWait(); 
    SSPBUF = data;        //data to be sent
    i2cWait();
    PEN = 1;           //stop condition
    __delay_ms(100);
}

void update_master (){
        
    // value that is going to be incrementet
    unsigned int v = 0;
    unsigned int i;
    while (1) {
        __delay_ms(100);
        v++;
        i2cSend(v);
        PORTA = v;
    }
}
void update_slave (){
    while (1);
}

void main(void) {
    init();
    PORTCbits.RC7 = 1;
    //update_master ();
    update_slave ();

}
