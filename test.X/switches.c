#include <xc.h>
#include <stdint.h>

#include "global.h"
#include "switches.h"



void create_switch (switch_t *s_t){
   
    // VARS
    s_t->state = 1;
    s_t->prev = s_t->state;

    s_t->streak = 0;    // Reset
    s_t->status = 0;    // Open
} 


void update_switch (switch_t *s_t, uint8_t sbtn, uint32_t streak){
    if (sbtn == s_t->prev && sbtn != s_t->state){    // If current input is the same as the last and not already set
        s_t->streak ++;
        if (s_t->streak >= streak){               // Wait 10 loops
            s_t->state  = sbtn;              // Set btn state to the current
            s_t->status = !sbtn;             // SET STATUS
        }
    }
    else {  // if current input is difefrent from the last or already set
        s_t->prev = sbtn;
        s_t->streak = 0;
    }   
}

void update_switch_streak (switch_t *s_t, uint8_t sbtn, uint16_t streak){
    if (sbtn == s_t->prev && sbtn != s_t->state){    // If current input is the same as the last and not already set
        s_t->streak ++;
        if (s_t->streak >= streak){               // Wait 10 loops
            s_t->state  = sbtn;              // Set btn state to the current
            s_t->status = !sbtn;             // SET STATUS
        }
    }
    else {  // if current input is difefrent from the last or already set
        s_t->prev = sbtn;
        s_t->streak = 0;
    }       
}

uint8_t get_switch_status (switch_t s_t){
    return s_t.status;
}
uint8_t get_switch_toggle (switch_t old_s_t, switch_t new_s_t){
    uint8_t val = 0;
    if (old_s_t.status != new_s_t.status && new_s_t.status == 1) val = 1;
    return val;
}