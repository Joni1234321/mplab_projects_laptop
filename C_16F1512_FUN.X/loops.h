
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LOOPS_H
#define	LOOPS_H


// High priority
// Repeats every 256 instructions
void fixedUpdate (void);


// Low priority
// Repeats this loop when it is done 
void update (void);
#endif	/* LOOPS_H */