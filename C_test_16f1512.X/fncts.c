#ifndef FNCTS_C
#define FNCTS_C

#define _XTAL_FREQ 500000

#include "fncts.h"

// Toggles pin RA1
// With attribute time delay
void toggle_RA3_delay_50 (void) {
    toggle_RA3();
    __delay_ms(50);
}
void toggle_RA3_delay_100 (void) {
    toggle_RA3();
    __delay_ms(100);
}
void toggle_RA3_delay_200 (void) {
    toggle_RA3();
    __delay_ms(200);
}
void toggle_RA3_delay_500 (void) {
    toggle_RA3();
    __delay_ms(500);
}


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