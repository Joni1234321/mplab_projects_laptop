#ifndef HANDLER_IO_H
#define	HANDLER_IO_H

// Buttons enabled 
unsigned char btn_en = 1;

// Enables check for button state
void check_buttons (void); 

// Checks switches and sends their corresponding data out
void poll_switches (void);

void port_init (char port);
// Process the data from the buttons ports
void btn_process (char data);

// Called when the button port is HI
void btn_hi (char c_port, char i);

// Called when the button port is LO
// Index of the current port
void btn_lo (char c_port, char i);



// Reset all the vars
void btn_rst (void);

#endif	/* INTERRUPTS_H */

