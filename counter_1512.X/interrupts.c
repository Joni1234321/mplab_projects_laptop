#include <xc.h>
#include <stdint.h>

#include "global.h"
#include "interrupt.h"

uint16_t seg_disp_sleep = 0;
uint16_t seg_disp_save  = 0;
void on_interrupt_timer_0 (void){
    // SEGMENT DISPLAY SLEEP
    // SECONDS DELAY = cycles / ( (8 MHz / 4 Cycles) / (256 tmr * 32 prescaler) )
    // 244,140625 cycles pr SECOND
    // 60 sec delay * 244,140625 cy/s = 14648 cycles 
    seg_disp_sleep++;
    if (seg_disp_sleep >= 14648){
        seg_disp_sleep = 0;
        INT_FLAGS.seg_disp_sleep = 1;
    }
    // SEGMENT DISPLAY VISUALISER
    INT_FLAGS.seg_disp_vis = 1; 
    // SEGMENT DISPLAY AUTOSAVE
    seg_disp_save++;
    if (seg_disp_save >= 1464){
        seg_disp_save = 0;
        INT_FLAGS.seg_disp_save = 1;
    }
    
    
    // OTHERS
    
}

void __interrupt() interruptHandler(void)
{ 
    // Timer
    if (T0IE && T0IF){
        on_interrupt_timer_0();
        T0IF = 0;
    }
}


void seg_disp_awake (void) {
    seg_disp_sleep = 0;
    INT_FLAGS.seg_disp_sleep = 0;
    INT_FLAGS.seg_disp_awake = 1;
}

