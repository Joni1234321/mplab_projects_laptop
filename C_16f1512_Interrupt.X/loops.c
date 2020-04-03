
#ifndef LOOPS_C
#define LOOPS_C

#include <xc.h>
#include "loops.h"
#include "fncts.h"

unsigned char counter;

// Occurs evey 256 instructions 
// t = 256 / 125 kIPS = 0.002048 s = 2 ms
void fixedUpdate (void){
    ++counter;
    // t = ( 256^2 / 125 kIPS ) / 2^6 = 0.008192 s = 8 ms)
    if (counter % 0x04 == 0){
        toggle_RA1 ();
    }
    // t = ( 256^2 / 125 kIPS ) / 2 = 0.262144 s = 250 ms
    if (counter % 0x7F == 0){
        toggle_RA2 ();
    }
    
    // t = 256^2 / 125 kIPS = 0.524288 s = 500 ms
    if (counter == 0xFF){
        toggle_RA3 ();
        counter = 0x00;
        
        btn_ch ();

    }

}

// Occurs every time the program has
void update (void) {

}


// Button check
void btn_ch (void){
    if (PORTAbits.RA4 == 0){
        btn_dn ();
    }
    else if (PORTAbits.RA4 == 1){
        btn_up ();
    }    
}
#endif