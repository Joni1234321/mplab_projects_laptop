#include <xc.h>
#include <stdint.h>
#include <math.h>

#include "global.h"
#include "config.h"
#include "init.h"

#include "interrupt.h"

#include "switches.h"
#include "i2c_eeprom.h"
#include "segmentdisplay47.h"





// EEPROM functions
void init_storage (void);
void rst_storage (void);

// Switch actions

void init_structs (void);

// SWITCHES
switch_t v_btns [SWITCHES];
switch_t *p_btns [SWITCHES];
switch_t old_btns [SWITCHES];

// 4 DIGIT 7 SEGMENT DISPLAY
seg_disp_47_t v_disp;
seg_disp_47_t *p_disp;

// Update functions
void update_int_flags (void);
void update_switches (void);

void main(void) {
    init();
    init_structs();
    
    // CHECK RESET BYTE if is true then reset all categories
    if (eeprom_read_16(EEP_ADD, RST_ADD) != 0x01){
        rst_storage();
        __delay_ms(EEP_DLY);
        eeprom_write_16(EEP_ADD, RST_ADD, 0x01);    // Set reset byte
    }
   
    // LOAD CATEGORY 0
    uint8_t k = 0; uint8_t f = 0;
    // Main loop
    for(;;) {   
        if (f == 0) k++; if (k >= 7 && f == 0) { f = 1;  }    // Load sequence (I HAVE NO IDEA WHY THIS IS NEEDED)
        else {
            // Real loop 
            update_switches();
            update_seg_disp_47(p_disp, 331);
            update_int_flags();        
        }
    }
    return;
}

// Update Switches
void update_switches (void) {
    // Set switches
    update_switch (p_btns[BTN_CONTINUE], CONTINUE, SWITCH_STREAK);

    // Set old buttons
    for (uint8_t i = 0; i < SWITCHES; i++){
        old_btns[i] = v_btns[i];
    }
}

void update_int_flags () {
    // SEG DISPLAY
    // Sleep
    if (INT_FLAGS.seg_disp_sleep){
        seg_disp_47_off (p_disp);
        INT_FLAGS.seg_disp_sleep = 0;
    }
    // Awake if already turned off
    if (INT_FLAGS.seg_disp_awake && v_disp.status == 0){
        seg_disp_47_on  (p_disp);
        INT_FLAGS.seg_disp_awake = 0;
    }
    // Visualize
    if (INT_FLAGS.seg_disp_vis){
        vis_next_digit (p_disp);
        INT_FLAGS.seg_disp_vis = 0;
    }
    // Save if display is active
    if (INT_FLAGS.seg_disp_save && v_disp.status == 1){
        INT_FLAGS.seg_disp_save = 0;
    }
    
        
}



void init_structs (void) {
    // SWITCHES
    // Set array
    p_btns[BTN_CONTINUE] = &v_btns[BTN_CONTINUE];

    // Create switch 
    create_switch(p_btns[BTN_CONTINUE]);


    // DISPLAY
    p_disp = &v_disp;
    create_seg_disp_47 (p_disp);
}




