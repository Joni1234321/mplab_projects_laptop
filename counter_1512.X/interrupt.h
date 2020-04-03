#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__


typedef union {
    struct {
        unsigned seg_disp_sleep     : 1;
        unsigned seg_disp_awake     : 1;
        unsigned seg_disp_vis       : 1;
        unsigned seg_disp_save      : 1;
    };
} int_flag_t;
volatile int_flag_t INT_FLAGS;   // GLOBAL INT FLAGS

/*
 * Resets the timer for sleep
 * So that there will have to go 60 seconds until it sleeps 
 * 
 */
void seg_disp_awake (void);


#endif
