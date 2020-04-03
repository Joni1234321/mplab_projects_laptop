#ifndef __INIT_H__
#define	__INIT_H__

// PORTS
void res_ports (void);
void set_ports (void);

// INTERRUPT 
void init_interrupt_timer (void);


// SUB
void init_oscillerator (void);
void init_ports (void);
void init_i2c (void);
void init_interrupt (void);

// MAIN
void init (void);


#endif	/* INIT_H */

