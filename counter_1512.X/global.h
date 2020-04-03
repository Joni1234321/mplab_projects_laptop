#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <xc.h>

// Osc
#define _XTAL_FREQ 8000000
#define IPS  2000000 // IPS = (1 MHz / 4 cycles/instruction) = 1,000,000 instructions pr second

// Interrupts
#define INT_FLAGS INT_FLAGS

// Switches
#define SWITCH_STREAK 5   

#define SWITCHES  5
// INDEX TABLE 
#define BTN_UP    0
#define BTN_DN    1
#define BTN_CAT_1 2
#define BTN_CAT_2 3
#define BTN_RST   4

// input
#define UP      PORTBbits.RB4
#define DN      PORTBbits.RB5
#define CAT_1   PORTBbits.RB6
#define CAT_2   PORTBbits.RB7

#define RST     PORTCbits.RC5

// output
#define A       LATAbits.LATA0
#define B       LATAbits.LATA1
#define C       LATAbits.LATA2
#define D       LATAbits.LATA3
#define E       LATAbits.LATA4
#define F       LATAbits.LATA5
#define G       LATAbits.LATA6
#define DEC     LATAbits.LATA7
#define D1      LATBbits.LATB0
#define D2      LATBbits.LATB1
#define D3      LATBbits.LATB2
#define D4      LATBbits.LATB3

#define DBG_PIN LATCbits.LATC7

// EEPROM
#define EEP_ADD         0xA0        // EEPPROM ADDRESS
#define EEP_DLY         5           // Delay in ms
#define CAT_SIZE        2           // Byte size
#define CAT_ADD_START   0x0450
#define RST_ADD         0x0001      // Reset byte address

// VARS
#define CATEGORIES 2

#endif