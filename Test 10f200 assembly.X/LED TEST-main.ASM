; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR

; PIC10F200 Configuration Bit Settings

; Assembly source line config statements

    list      p=10F200
    #include <p10F200.inc>        ; processor specific variable definitions

    EXTERN  delay10_R

    
; CONFIGURATION
    __CONFIG   _MCLRE_ON & _CP_OFF & _WDT_OFF

    uDATA
sGPIO   res 1
    
; RC CALIBRATION
RCCAL	CODE   0xFF	    ; processor reset vector
	res 1		    ; holds internal RC cal value, as a movlw k
	
; RESET VECTOR
RESET	CODE    0x000	    ; effective reset vector 
	movwf   OSCCAL	    ; apply internal RC factory calibration 
	pagesel start
	goto	start
	
; SUBROUTINE VECTORS
delay10 
	pagesel delay10_R
	goto	delay10_R

; MAIN PROGRAM
MAIN	CODE
	
; init
start	
    movlw   b'1101'
    tris    GPIO
    
    clrf    sGPIO	    ; start with shadow GPIO zero

; MAIN LOOP
    
main_loop
    ; toggle LED on GP1
    movf    sGPIO, w
    xorlw   b'0010'	    ; toggle bit
    movwf   sGPIO
    movwf   GPIO
    ; delay .5 s
    movlw   .50		    ; delay 50 x 10 ms = 500 ms
    pagesel delay10
    call    delay10	    ; delay 500 ms
    ; turn off LED
    clrf    GPIO
    movlw   .30
    call    delay10
    
    pagesel main_loop
    goto    main_loop
    
end