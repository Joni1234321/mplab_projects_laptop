#include <xc.h>
#include "handler_io.h"

// The buttons that are being pushed down
unsigned char btns_activated = 0x00;

// Call this every 1 ms
void check_buttons (void) {
    // Stop the process, if there are no events
    if (btn_en == 0){
        btn_rst ();
        return;
    }
    
    // Check what switch has been activated
    poll_switches();
}

// Polls switches every 1 ms
void poll_switches (void){
    // Process all the data from the IO ports
    btn_process (PORTB);
}

// Set if there is a switch at the port
unsigned char btn_port = 0x00;

// Set if the during the last loop the switch was pressed down
unsigned char btn_prev_state = 0x00;

// Pointer to all the timers 
unsigned char btn_streaks[] = {0,0,0,0,0,0,0,0};
// Set if the button is actively being pushed down
unsigned char btn_active;

// Init all the vars
void port_init (char port) {
    
    // Init port
    btn_port = port;
    
    
    // TODO: Make it calculate the number of switches by counting the 1's in the port byte or something
    // number of switches that are in the port
    unsigned char n_switches = 8;
    
    
}

// Do this every time poll happens
void btn_process (char data){
    
    // Current port is set as the first
    char c_port = 0x01;
    
    // Iterate through all of the 8 ports
    for(char i = 0; i <= 7; i++){
        
        // If the current port is a switch
        if ((c_port & btn_port) == c_port){
        
            // If switch port is HI
            if ((c_port & data) == c_port){
                btn_hi (c_port, i);
            }
            // If switch port is LO
            else {
                btn_lo (c_port, i);
            }
        }
        
        // Go to the next port
        c_port = c_port << 1;
    }
        
}

// Called when the btn_en has changed
void btn_rst (void) {
    // Reset vars to 0
    btn_en = 0x00;
    btn_prev_state = 0x00;
}

// Called whenever the port input is high
void btn_hi (char c_port, char i) {

    // Get the streak of the current button
    unsigned char *btn_streak = &(btn_streaks[i]);

    // if the button was HI last loop
    if ((btn_prev_state & c_port) == c_port){
        
        (*btn_streak)++;
        // Change button state when the button has been pressed for 10 ms
        if ((*btn_streak) == 10){
            // Turn the port off (0)
            // First invert
            char sLATA = !LATA;
            // Then turn the port on, by changing the ports value to (1)
            sLATA = c_port | sLATA;
            // Then reverse again, so that the current port is 0, while the rest are unchanged
            LATA = !sLATA;
        }
    }
    // if the button was LO last loop
    else {
        // Reverse state
        btn_prev_state = btn_prev_state ^ 0x01;
        // Reset streak
        (*btn_streak) = 0x00;
    }
}

// Called whenever the port input is low
void btn_lo (char c_port, char i){
    
   // Get the streak of the current button
    unsigned char *btn_streak = &(btn_streaks[i]);

    // if the button was HI last loop
    if ((btn_prev_state & c_port) == c_port){
        // Reverse state
        btn_prev_state = btn_prev_state ^ 0x01;
        // Reset streak
        (*btn_streak) = 0x00;
    }
    // if the button was LO last loop
    else {

        (*btn_streak) ++;
        // Change button state when the button has been pressed for 10 ms
        if ((*btn_streak) == 10){
            // Turn the port on (1)
            LATA = c_port | LATA;
        }
    }
}


// Check if there are more events
// If there is no more events, disable this loop
void check_btn_events () {
    
  /*  if (){
        // Disable btn loop
        btn_en = 0;
    }*/
    
}

