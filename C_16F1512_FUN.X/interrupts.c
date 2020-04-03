#ifndef INTERRUPTS_C
#define INTERRUPTS_C

#include <xc.h>
#include "interrupts.h"
#include "loops.h"
#include "handler_io.h"

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
}

// RB0 PIN 
void on_interrupt_pin (void) {
    // Enable check for buttons
    btn_en = 1;
}

void on_interrupt_timer (void){
    fixedUpdate ();    
}


#endif