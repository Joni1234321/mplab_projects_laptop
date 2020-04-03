#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

void __interrupt() interrupt_handler (void);
void on_interrupt_pin (void);
void on_interrupt_timer(void);

#endif	/* INTERRUPTS_H */

