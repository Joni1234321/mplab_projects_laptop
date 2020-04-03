;*******************************************************************************
;                                                                              *
;    Filename: test_main.asm		                                       *
;    Date: 29/03/2019							       *
;    File Version: 0.1							       *
;    Author: Dartfish							       *
;    Company: Dartfish inc                                                     *
;    Description: Turn on led every 6 second                                   *
;                                                                              *
;*******************************************************************************
    
; INCLUDE FILES
    
    list	p=10F200
    #include <p10f200.inc>
    
    
    
; CONFIG FILES

    __CONFIG   _MCLRE_ON & _CP_OFF & _WDT_OFF



; VARIABLES


; RC CALIBRATION

RCCAL	CODE   0xFF	    ; processor reset vector
	res 1		    ; holds internal RC cal value, as a movlw k
	
; RESET VECTOR
RESET	CODE    0x000	    ; effective reset vector 
	movwf   OSCCAL	    ; apply internal RC factory calibration 


	
	
; MAIN PROGRAM

start	
    movlw   b'1000'	    ; set GP0 and GP1 to output
    tris    GPIO
    
    
main_loop
    
    ; TODO: if button is pressed, then power on the LED

    movlw    b'0111'
    movwf   GPIO
    
    movlw    b'0100'
    movwf   GPIO
    
    
    ; LOOP FOREVER
    goto main_loop
    
    
END