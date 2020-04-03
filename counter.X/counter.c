/*
 * File:   counter.c
 * Author: Jonas
 *
 * Created on September 20, 2019, 12:12 AM
 */


#include <xc.h>


// The category
unsigned char category;
unsigned char curr_val;
int category_value (void);

// Switch actions
void change_category (unsigned short category);
void increment_category (void);
void decrement_category (void);
void set_digit(unsigned short digit_val);           // Set digit value

// Passive functions
void poll_switches (void);
void vis_counter(void);

void main(void) {
    for(;;) {  
        poll_switches();
        vis_counter();
    }


    return;
}

// Poll Switches
void poll_switches (void) {
    
}
// Visual counter
void vis_counter (void){
    //DATA IS ON PORT A
    //https://www.microcontroller-project.com/uploads/2/2/1/5/22159166/common-anode-7-segment-arduino-hex-code-for-numbers_orig.jpg
    
    
    set_digit (digit_val);
        
    
    
}

// Changes category
void change_category(unsigned short category){
    
}

int category_value (void) {
    //I2C to EEPROM
}


void set_digit (unsigned short digit_val){
    
    // Set current digit
    switch (digit_val)
    {
        case 0:
            LATA = 0x3F;
            break;
        case 1:
            LATA = 0x06;
            break;
        case 2:
            LATA = 0x5B;
            break;
        case 3:
            LATA = 0x4F;
            break;
        case 4:
            LATA = 0x66;
            break;
        case 5:
            LATA = 0x6D;
            break;
        case 6:
            LATA = 0x7D;
            break;
        case 7:
            LATA = 0x07;
            break;
        case 8:
            LATA = 0x7F;
            break;
        case 9:
            LATA = 0x6F;
            break;
            
        default:
            LATA = 0x00;
            break;
    }
}