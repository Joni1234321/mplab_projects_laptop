#ifndef __SEGMENTDISPLAY47_H__
#define __SEGMENTDISPLAY47_H__

// 4 digit 7 segmentdisplay
// LATA contains the ports 

void vis_counter (uint32_t cur_val);
void set_digit (uint8_t digit_val, uint8_t digit);

typedef union {
    struct {
        unsigned digit      : 2;     // Current digit
        // SEPERATED IN TWO BECAUSE XC8 BIT FIELD CANNOT HANDLE MORE THAN 8
        unsigned value_lo   : 8;     // Value of display [MAX VALUE IS 0x3FFF = 16.383]
        unsigned value_hi   : 6; 
        
        unsigned status     : 1;     // On and off
    };
} seg_disp_47_t;
void create_seg_disp_47 (seg_disp_47_t *s_t);
void update_seg_disp_47 (seg_disp_47_t *s_t, uint16_t cur_val);

void seg_disp_47_on (seg_disp_47_t *s_t);
void seg_disp_47_off(seg_disp_47_t *s_t);

void seg_disp_47_set_val (seg_disp_47_t *s_t, uint16_t value);
void vis_next_digit(seg_disp_47_t *s_t);


#endif
