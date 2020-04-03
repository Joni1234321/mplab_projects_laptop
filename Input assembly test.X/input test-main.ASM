; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR

; PIC10F200 Configuration Bit Settings

; Assembly source line config statements

    list      p=10F200
    #include <p10F200.inc>        ; processor specific variable definitions

    EXTERN  delay10_R
    
; CONFIGURATION
    __CONFIG   _MCLRE_ON & _CP_OFF & _WDT_OFF

; VARIABLE DEFINITION
    UDATA
sGPIO   res 1		    ; safer to declare shadow copy of GPIO
db_cnt  res 1                   ; debounce counter
dc1     res 1                   ; delay counter
; RC CALIBRATION
RCCAL	CODE   0xFF	    ; processor reset vector
	res 1		    ; holds internal RC cal value, as a movlw k
	
; RESET VECTOR
RESET	CODE    0x000	    ; effective reset vector 
	movwf   OSCCAL	    ; apply internal RC factory calibration 
	pagesel start
	goto	start

; SUB VECTOR
delay10
	pagesel	delay10_R
	goto	delay10_R		
	
; MAIN PROGRAM
MAIN	CODE
; init
start	
    clrf    GPIO	; start with GPIO clear
    clrf    sGPIO	; start with sGPIO clear

    movlw   b'1101'
    tris    GPIO
    
    
; MAIN LOOP
main_loop
        banksel db_cnt          ; select data bank for this section

        ; wait for button press, debounce by counting:
db_dn   movlw   .13             ; max count = 10ms/768us = 13
        movwf   db_cnt        
        clrf    dc1             
dn_dly  incfsz  dc1,f           ; delay 256x3 = 768 us.
        goto    dn_dly
        btfsc   GPIO,3          ; if button up (GP3 high),
        goto    db_dn           ;   restart count
        decfsz  db_cnt,f        ; else repeat until max count reached
        goto    dn_dly 

        ; toggle LED on GP1
        movf    sGPIO,w
        xorlw   b'0010'       ; toggle bit corresponding to GP1 (bit 1)
        movwf   sGPIO           ;   in shadow register
        movwf   GPIO            ; and write to GPIO

        ; wait for button release, debounce by counting:
db_up   movlw   .13             ; max count = 10ms/768us = 13
        movwf   db_cnt        
        clrf    dc1             
up_dly  incfsz  dc1,f           ; delay 256x3 = 768 us.
        goto    up_dly
        btfss   GPIO,3          ; if button down (GP3 low),
        goto    db_up           ;   restart count
        decfsz  db_cnt,f        ; else repeat until max count reached
        goto    up_dly

        ; repeat forever
        goto    main_loop            

        END



