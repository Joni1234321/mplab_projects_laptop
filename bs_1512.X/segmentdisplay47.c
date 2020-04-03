#include <xc.h>
#include <stdint.h>

#include "segmentdisplay47.h"

/* PRE  STRUCT */
// Visual counter
void vis_counter (uint32_t cur_val){
    //DATA IS ON PORT A
    uint32_t value = cur_val;
    for (uint8_t digit = 0; digit < 4; digit++){
        uint32_t digit_seperator = value % 10;
        value /= 10;    // Next digit
        
        set_digit (digit_seperator, (digit % 4) + 1);
    }
}
void set_digit (uint8_t digit_val, uint8_t digit){
    
    // If digit is overflow 
    if (digit > 4) return;
    
    // Change the first 4 bits of LATB register
    uint8_t sLATB = (0xF0 & LATB);
    // 1000 for the first digit  and 0100 for the second
    LATB = sLATB| (0x08 >> (digit - 1));

    // Set current digit
    switch (digit_val)
    {
        case 0:
            LATA = 0xC0;
            break;
        case 1:
            LATA = 0xF9;
            break;
        case 2:
            LATA = 0xA4;
            break;
        case 3:
            LATA = 0xB0;
            break;
        case 4:
            LATA = 0x99;
            break;
        case 5:
            LATA = 0x92;
            break;
        case 6:
            LATA = 0x82;
            break;
        case 7:
            LATA = 0xF8;
            break;
        case 8:
            LATA = 0x80;
            break;
        case 9:
            LATA = 0x90;
            break;
            
        default:
            LATA = 0xEF;
            break;
    }
    
}



/* POST STRUCT */
uint16_t seg_disp_47_get_val (seg_disp_47_t *s_t);
void vis_next_digit(seg_disp_47_t * s_t){
    // If the display is off just return
    if (!s_t->status){
        return;
    }
    
    uint16_t val = seg_disp_47_get_val(s_t);
    uint16_t digit_divider = 1;
    for (uint8_t p = 0; p < s_t->digit; p++) {
        digit_divider *= 10;
    }
    // GET THE CURRENT DIGITS VALUE
    uint8_t digit_val = ( val / digit_divider ) % 10;
    
    // SET DIGIT
    // Change the first 4 bits of LATB register
    uint8_t sLATB = (0xF0 & LATB);
    // 1000 for the first digit  and 0100 for the second
    LATB = sLATB| (0x08 >> (s_t->digit));

    // SET VALUE
    switch (digit_val)
    {
        case 0:
            LATA = 0xC0;
            break;
        case 1:
            LATA = 0xF9;
            break;
        case 2:
            LATA = 0xA4;
            break;
        case 3:
            LATA = 0xB0;
            break;
        case 4:
            LATA = 0x99;
            break;
        case 5:
            LATA = 0x92;
            break;
        case 6:
            LATA = 0x82;
            break;
        case 7:
            LATA = 0xF8;
            break;
        case 8:
            LATA = 0x80;
            break;
        case 9:
            LATA = 0x90;
            break;
            
        default:
            LATA = 0xEF;
            break;
    }
    
    s_t->digit++;
}

void create_seg_disp_47 (seg_disp_47_t *s_t){
    s_t->digit = 0;
    s_t->value_lo = 0;
    s_t->value_hi = 0;
    s_t->status = 1;
}
void update_seg_disp_47 (seg_disp_47_t *s_t, uint16_t cur_val){
    seg_disp_47_set_val (s_t, cur_val);
}


uint16_t seg_disp_47_get_val (seg_disp_47_t *s_t){
    uint16_t val = 0;
    val |= s_t->value_lo;
    val |= s_t->value_hi << 8;
    return val;
}
void seg_disp_47_set_val (seg_disp_47_t *s_t, uint16_t value){
    s_t->value_lo = value;
    s_t->value_hi = value >> 8;
}



void seg_disp_47_on (seg_disp_47_t *s_t){s_t->status = 1; }
void seg_disp_47_off(seg_disp_47_t *s_t){s_t->status = 0; LATA = 0xFF;}