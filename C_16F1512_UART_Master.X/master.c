#include <xc.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000


// Functions Declarations
void UART_TX_Init(void);
void UART_Write(uint8_t);



// Main Routine
void main(void) 
{
    OSCCONbits.IRCF = 0b1101;

    //--[ Peripherals & IO Configurations ]--
    UART_TX_Init(); // Initialize The UART in Master Mode @ 9600bps
    uint8_t Data = 0; // The Data Byte

    TRISA = 0x00; // Output Port (4-Pins)
    PORTA = 0x00; // Initially OFF
    
    ANSELC = 0;
    
    //---------------------------
    while(1)
    {
        Data++;

        PORTA = Data; // Display The Current Data Value @ PORTD
        UART_Write (Data);
        __delay_ms(100);
    } 
  
    return;
}

 
void UART_TX_Init(void)
{
    BRGH = 1;   // Set For High-Speed Baud Rate
    SPBRG = 25; // Set The Baud Rate To Be 9600 bps
    //--[ Enable The Ascynchronous Serial Port ]--
    SYNC = 0;
    SPEN = 1;
    //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
    TRISC6 = 1;  // As stated in the datasheet
    TRISC7 = 1;  // As stated in the datasheet
    TXEN = 1; // Enable UART Transmission
}
 
void UART_Write(uint8_t data)
{
    while(!TRMT);
    TXREG = data;
}