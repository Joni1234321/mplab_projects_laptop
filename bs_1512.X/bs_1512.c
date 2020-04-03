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


// Vars
uint8_t  cur_cat;                   // Current Category
uint16_t cur_val;          // Current Value
uint16_t extended_rst_streak = 0;
uint8_t rst_flag = 0;
// Category
void change_category (uint8_t category);
uint16_t get_cat_val (uint8_t category);
void set_cat_val (uint8_t category, uint16_t val);
void cat_auto_save (void);

// EEPROM functions
void init_storage (void);
void rst_storage (void);

// Switch actions
void increment (void);                                                    // Increment current category
void decrement (void);                                                    // Decrement current category

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

uint16_t dab;
void main(void) {
    init();
    init_structs();
    DBG_PIN = 1;
    dab = 1331;
    update_seg_disp_47(p_disp, dab);
    
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
        if (f == 0) k++; if (k >= 7 && f == 0) { f = 1; cur_val = get_cat_val (cur_cat); change_category(cur_cat); }    // Load sequence (I HAVE NO IDEA WHY THIS IS NEEDED)
        else {
            update_switches();
            update_seg_disp_47(p_disp, dab);
            update_int_flags();        
        }
    }
    return;
}

uint8_t scene = 0;
// Update Switches
void update_switches (void) {
    // Set switches
    update_switch (p_btns[BTN_UP], UP, SWITCH_STREAK);
    update_switch (p_btns[BTN_DN], DN, SWITCH_STREAK);

    switch (scene) {
        case 0:
            dab = 331;
            DBG_PIN = 0;
            break;
        case 1:
            DBG_PIN = 1;
            break;
        case 2:
            DBG_PIN = 0;
            break;
        case 3:
            DBG_PIN = 1; 
            break;
        case 4:
            DBG_PIN = 0;
            break;
        case 5:
            dab = 220;
            break;
        case 6:
            DBG_PIN = 1;
            break;
        case 7:
            DBG_PIN = 0;
            break;
        case 8:
            DBG_PIN = 1;
            break;
        case 9:
            DBG_PIN = 0;
            break;
            
        case 10:
            dab = 21;
            break;
        case 11:
            DBG_PIN = 1;
            break;
        case 12:
            DBG_PIN = 0;
            break;
        case 13:
            DBG_PIN = 1;
            break;
        case 14:
            DBG_PIN = 0;
            break;
            
        default:
            scene = 0;

            break;
    }   
    
    if (get_switch_toggle(old_btns[BTN_UP], v_btns[BTN_UP])){
        scene ++;
    }
    
    if (get_switch_toggle(old_btns[BTN_DN], v_btns[BTN_DN])){
        scene --;
        
    }

    
    // Set old buttons
    for (uint8_t i = 0; i < SWITCHES; i++){
        old_btns[i] = v_btns[i];
    }
}

void update_int_flags () {
    // SEG DISPLAY
    // Sleep
    if (INT_FLAGS.seg_disp_sleep){
        //seg_disp_47_off (p_disp);
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
        cat_auto_save();
        INT_FLAGS.seg_disp_save = 0;
    }
    
        
}

// Get category value
uint16_t get_cat_val (uint8_t category){
    if (cur_cat > CATEGORIES - 1) return -1;  // Overflow error
    
    // Convert the 4 uint8_t to 1 uint16_t 
    uint16_t val = 0;
    uint16_t add_start = CAT_ADD_START + (CAT_SIZE * category);

    for (uint8_t i = 0; i < CAT_SIZE; i ++){
        __delay_ms(EEP_DLY);
        uint8_t d = eeprom_read_16 (EEP_ADD, add_start + i);
        val |= d << (8 * i);  // Add byte to uint16_t
    }    
    
    return val;
}
// Set category value
void set_cat_val (uint8_t category, uint16_t val){
    if (cur_cat > CATEGORIES - 1) return;  // Overflow error

    // Convert the 1 uint16_t to 4 uint8_t
    uint16_t add_start = CAT_ADD_START + (CAT_SIZE * category);
    for (uint8_t i = 0; i < CAT_SIZE; i ++){
        __delay_ms(EEP_DLY);
        eeprom_write_16 (EEP_ADD, add_start + i, val);
        val = val >> 8;     // 
    }
}

uint8_t last_save_cat = 0;
uint16_t last_save_val = 0;
void cat_auto_save (void) {
    if (last_save_cat == cur_cat && last_save_val == cur_val) return;    // SAVE ONLY IF NEW DATA 
    last_save_cat = cur_cat; last_save_val = cur_val;
    set_cat_val (cur_cat, cur_val); // Save the categories value
}

void increment (void) {
    cur_val++;
}
void decrement (void) {
    cur_val--;
}

void change_category (uint8_t category){
    set_cat_val (cur_cat, cur_val); // Save the categories value
    cur_cat = category;             // Set new current
    cur_val = get_cat_val(cur_cat); // Load new category value
}

// Sets all categories values to 0
void rst_storage (void){
    for (uint8_t i = 0; i < CATEGORIES; i++){
        set_cat_val(i, 0x12);    
    }
}


void init_structs (void) {
    // SWITCHES
    // Set array
    p_btns[BTN_UP] = &v_btns[BTN_UP];
    p_btns[BTN_DN] = &v_btns[BTN_DN];
    p_btns[BTN_CAT_1] = &v_btns[BTN_CAT_1];
    p_btns[BTN_CAT_2] = &v_btns[BTN_CAT_2];
    p_btns[BTN_RST] = &v_btns[BTN_RST];
    // Create switch 
    create_switch(p_btns[BTN_UP]);
    create_switch(p_btns[BTN_DN]);
    create_switch(p_btns[BTN_CAT_1]);
    create_switch(p_btns[BTN_CAT_2]);
    create_switch(p_btns[BTN_RST]);

    // DISPLAY
    p_disp = &v_disp;
    create_seg_disp_47 (p_disp);
}

// OBSULETE
// Presets storage to values
void init_storage (void){
    set_cat_val(0, 0x0034);    
    set_cat_val(1, 0x0048);    
}


