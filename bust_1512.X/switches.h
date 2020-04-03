#ifndef __SWITCHES_H__
#define __SWITCHES_H__

typedef union {
    struct { 
        unsigned prev   :1;     // Previous state
        unsigned state  :1;     // High = 1, Low = 0
        unsigned streak :5;     // Streak

        unsigned status :1;     // Closed = 1, Open = 0
    };
} switch_t;



/*
 * Switch constructor
 * 
 * Because this is xc8, no malloc is available
 *  
 */
void create_switch (switch_t *s_t);

/* 
 * Updates the switches variables
 * <b>Parameters</b>
 * Switch_t the switch
 * uint8_t the btns values 
 */
void update_switch (switch_t *s_t, uint8_t sbtn, uint32_t streak);


/*
 * Returns the switch status
 *  
 */
uint8_t get_switch_status (switch_t s_t);
uint8_t get_switch_toggle (switch_t old_s_t, switch_t new_s_t);    

#endif