#ifndef CONF_H
#define	CONF_H

// Sets all ports to input and 
void reset_ports (void);

void init_oscillator (void);
void init_ports (void);


// Set i2c specifacations
void init_i2c (void);
void init_i2c_master (void);

// Init
void init (void);

#endif	/* CONF_H */

