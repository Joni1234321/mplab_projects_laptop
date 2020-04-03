#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000


// DECLARATIONS
void UART_RX_Init(void);
uint8_t UART_Read(void);
// Globals
uint8_t UART_Buffer = 0;


void main(void)
{
    //--[ Peripherals & IO Configurations ]--
    TRISA = 0x00; // Output Port (4-Pins)
    PORTA = 0x00; // Initially OFF
    TRISB = 0;
    PORTB = 0;
    RB0 = 1;
    RB2 = 0;
    ANSELC = 0;

    //---------------------------
 
    
    UART_RX_Init(); // Initialize The UART in Master Mode @ 9600bps

    while(1) 
    {

      // Stay Idle, Everything is handled in the ISR !
    }
    return;
}
//--------------------------------
// Functions Definitions
 
void UART_RX_Init()
{
    BRGH = 1; // Set For High-Speed Baud Rate
    SPBRG = 25; // Set The Baud Rate To Be 9600 bps
    // Enable The Ascynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    // Set The RX-TX Pins to be in UART mode (not io)
    TRISC6 = 1; // As stated in the datasheet
    TRISC7 = 1; // As stated in the datasheet
    //--[ Enable UART Receiving Interrupts ]--
    RCIE = 1; // UART Receving Interrupt Enable Bit
    PEIE = 1; // Peripherals Interrupt Enable Bit
    GIE = 1; // Global Interrupt Enable Bit
    //------------------
    CREN = 1; // Enable Data Continous Reception
}
 
void __interrupt() ISR (void)
{    
    RB2 ^= 1;

    if (RCIF == 1)
    {
        
        UART_Buffer = RCREG; // Read The Received Data Buffer
        PORTA = UART_Buffer; // Display The Received Data On LEDs
        RCIF = 0; // Clear The Flag
    }
}