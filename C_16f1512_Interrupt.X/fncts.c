#ifndef FNCTS_C
#define FNCTS_C

#define _XTAL_FREQ 500000

#include <xc.h>
#include "fncts.h"

void toggle_RA1 (void) {
    LATAbits.LATA1 = !LATAbits.LATA1;    
}
void toggle_RA2 (void) {
    LATAbits.LATA2 = !LATAbits.LATA2;
}
void toggle_RA3 (void) {
    LATAbits.LATA3 = !LATAbits.LATA3;
}


char btn_state = 0;
char btn_streak = 0;

void btn_dn (void){

    // If last time it checked, the button was pressed down
    if (btn_state == 1){
        btn_streak ++;
        // Change button state when the button has been pressed for 10 ms
        if (btn_streak == 10){
            LATAbits.LATA1 = 1; 
        }
    }
    // if this is the first time the button is pressed down
    else {
        // Reverse state
        btn_state = btn_state ^ 0x01;
        // Reset streak
        btn_streak = 0;
    }
}

void btn_up (void) {
    
    // If last time it checked, the button wasn't pressed down
    if (btn_state == 0){
        btn_streak ++;
        // Change button state when the button has been pressed for 10 ms
        if (btn_streak == 10){
            LATAbits.LATA1 = 0; 
        }
    }
    else {
        // Reverse state
        btn_state = btn_state ^ 0x01;
        // Reset streak
        btn_streak = 0;
    }
}




#endif