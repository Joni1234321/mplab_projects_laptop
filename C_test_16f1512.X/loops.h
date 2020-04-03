
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LOOPS_H
#define	LOOPS_H

#include <xc.h> // include processor files - each processor file is guarded. 

// High priority
// 256 Cycles
void fixedUpdate (void);



// Low priority
void update (void);

#endif LOOPS_H