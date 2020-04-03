#ifndef LOOPS_C
#define LOOPS_C

#include <xc.h>
#include "loops.h"
#include "handler_io.h"

unsigned char counter;

/*
 * How to calc loop time
 * 
 * t = ( timer0.vals * prescale * counter.vals / Instructions Pr Second ) / ( counter.vals / iterations )
 * EXAMPLE:
 * timer0.vals (8-bit)          = 256
 * prescale = 1 : 2             = 2
 * counter.vals (8-bit)         = 256 
 * Instructions Pr Second       = 125,000 
 * Iterations                   = 2
 * // Every x iteration of fixedUpdate  
 * EXAMPLE:
 * // if (counter % 0x08 == 0) 
 * // That is the equal to 8 iterations
 * 
 * t = ( 256 * 2 * 256 / 125,000 ) / ( 256 / 2 ) = 0.008192 seconds = 8 ms
 * t = 8 ms
 */

// Occurs evey 256 instructions 
// High priority
// t = 256 / 250 kIPS = 0.001024 s = 1 ms
void fixedUpdate (void){
    ++counter;
    // t = 1 ms
    check_buttons();
    
    // t = 4 ms
    if (counter % 0x04 == 0){
        
    }
    
    // t = 130 ms
    if (counter % 0x7F == 0){
        //LATAbits.LATA1 = !LATAbits.LATA1;
    }
    
    // t = 260 ms
    if (counter == 0xFF){
        counter = 0x00;
        //LATAbits.LATA2 = !LATAbits.LATA2;
    }

}

// Occurs every time the program has
void update (void) {
   
}

#endif