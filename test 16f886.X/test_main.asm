
; PIC16F886 Configuration Bit Settings

; Assembly source line config statements

#include "p16f886.inc"

; CONFIG1
; __config 0xECD7
 __CONFIG _CONFIG1, _FOSC_EXTRC_CLKOUT & _WDTE_OFF & _PWRTE_OFF & _MCLRE_ON & _CP_OFF & _CPD_OFF & _BOREN_OFF & _IESO_ON & _FCMEN_ON & _LVP_OFF
; CONFIG2
; __config 0xFFFF
 __CONFIG _CONFIG2, _BOR4V_BOR40V & _WRT_OFF

 
; VARS
    UDATA_SHR 
sPORTA	    res	    1
sPORTB	    res	    1
Del_Count   res	    1
resulthi    res	    1
resultlo    res	    1
   
    UDATA
dc1	res 1
dc2	res 1
dc3	res 1


    
; RESET
RES_VECT  CODE    0x0000            ; processor reset vector
    PAGESEL START
    GOTO    START                   ; go to beginning of program

    
; SUBROUTINE VECTORS
WAIT_10: GOTO WAIT
WAIT_08: GOTO DELAY_W
WAIT_DN: GOTO W_BTN_D   
WAIT_UP: GOTO W_BTN_U    
LED01_T: GOTO LED1_TOGGLE
LED01_H: GOTO LED1_HIGH
LED01_L: GOTO LED1_LOW
CLOCK_INIT: GOTO INIT_CLOCK
PORT_INIT: GOTO INIT_PORT
ADC_INIT: GOTO INIT_ADC
ADC_START: GOTO START_ADC


; MAIN PROGRAM 
MAIN_PROG CODE                      ; let linker place main program

START:				    ; INITIALISATION
    call CLOCK_INIT
    call PORT_INIT
    call ADC_INIT
    CALL ADC_START	    ; Start the ADC and wait for the completion interrupt.
    
    ; Reset data
    clrf sPORTA
    clrf sPORTB
    





MYLOOP:
    ; Wait for the button pressed
    PAGESEL WAIT_DN
    CALL    WAIT_DN
    
    ; Toggle LED
    PAGESEL LED01_T
    CALL    LED01_T
    
    ; Wait 10 ms
    PAGESEL WAIT_10
    MOVLW   .1
    CALL    WAIT_10
    
    ; Wait for button release
    PAGESEL WAIT_UP
    CALL    WAIT_UP
      
    ; Wait 10 ms
    PAGESEL WAIT_10
    MOVLW   .1
    CALL    WAIT_10
    
    PAGESEL MYLOOP
    GOTO MYLOOP

IDLELOOP:
    NOP
    NOP
    NOP
    NOP
    GOTO IDLELOOP


; ************************  SUBROUTINES *******************************
SUBS CODE 
WAIT:
    pagesel dly1
    banksel dc3
    movwf   dc3		    ; delay = 1 + 
dly2:			    
    movlw   .13		    ; repeat 13 times
    movwf   dc2		    ; -> 13 x (767 + 3) - 1 = 10009 cycles´= 10009 uS = 10 ms
    clrf    dc1		    ; inner loop: 256 loops x 3 cycles pr loop - 1 = 767 cycles
   
dly1:			    ; inner loop: 256 loops x 4 cycles pr loop - 1
    decfsz  dc1, f
    goto    dly1
    decfsz  dc2, f	    ; end middle loop 
    goto    dly1
    decfsz  dc3, f	    ; end outer loop
    goto    dly2
    retlw   0


; Button 0 down
W_BTN_D:
    BANKSEL PORTA
    PAGESEL W_BTN_D
    
    btfsc   PORTA, 0
    goto    W_BTN_D
    
    retlw 0


; Button 0 up
W_BTN_U:
    BANKSEL PORTA
    PAGESEL W_BTN_D
    
    btfss   PORTA, 0
    goto    W_BTN_U
    
    retlw 0 
    

; LED 1 Toggle
LED1_TOGGLE:
    BANKSEL PORTB
    MOVF    sPORTB, w
    XORLW   0x08	    ; 0000 1000 
    MOVWF   sPORTB
    MOVWF   PORTB
        
    retlw 0

; LED 1 HIGH 
LED1_HIGH:
    BANKSEL PORTB
    BSF	    PORTB, 3  
    
    retlw 0
    
; LED 1 LOW
LED1_LOW:
    BANKSEL PORTB
    BCF	    PORTB, 3
    
    retlw 0
    
; INIT CLOCK FEQUENCY
INIT_CLOCK:
    BANKSEL OSCCON
    movlw   b'01100000'	    ; -110----    4 MHz
    movwf   OSCCON	    ; At 4 MHz it's one microsecond per line of code.
    retlw 0
       
INIT_PORT:
    
    BANKSEL TRISA
    MOVLW   b'00000011'	    ; Six Outputs. RA0/AN0 will be an analogue input. RA5 is always an input.
    MOVWF   TRISA	    ; Set PORTA data directions.
    MOVLW   b'00000000'	    ; Eight output lines for the eight LEDs
    MOVWF   TRISB	    ; Set PORTB data directions.
    MOVLW   b'00000000'	    ; Eight output lines for the eight LEDs
    MOVWF   TRISC	    ; Set PORTB data directions.

    BANKSEL PORTA    
    MOVLW   0x00	    ; Clear PORTA and PORTB and PORTC
    MOVWF   PORTA
    MOVWF   PORTB
    MOVWF   PORTC
    retlw 0
    
INIT_ADC:
    BANKSEL ANSEL
    MOVLW   b'00000010'	    ; AN1 will be an analogue input. This is same pin as RA1.
    MOVWF   ANSEL	    ; Set Analogue Select Register
    
    BANKSEL ADCON1
    MOVLW   b'10000000'	    ; Set up ADCON1
;             ||||3210------- Not used
;             ||54----------- Use power supply lines as voltage references
;             |6------------- Don't divide AtoD clock source by 2 - ADCS2
;             7-------------- Left justify the result bits. Use ADRESH and discard the two bits in ADRESL
    MOVWF   ADCON1	    ; Save settings to A to D Control Register 1
    
    BANKSEL ADCON0
    MOVLW   b'01001001'	    ; Set up ADCON0    0xC1
;             |||||||0-------   1 Turn on the ADC - ADON
;             ||||||1--------     not used
;             |||||2---------   0 set this to one to start a conversion
;             ||543---------- 000 selects channel 0, RA0/AN0
;             76-------------  01 divides 4MHz clock by 8   0.25us * 8  =  2 us  OK because the safe range is 1.6 to 6.4 us
    MOVWF   ADCON0	    ; Save settings to A to D Control Register 0
    retlw 0

START_ADC:
    BANKSEL PIE1
    BSF	    PIE1,   ADIE    ; Set A to D Interrupt Enable in the peripheral interrupt register

    BANKSEL PIR1
    BCF	    PIR1,   ADIF    ; Clear A to D Interruot flag in Peripheral interrupt register
    BSF	    INTCON, PEIE    ; Turn on Peripheral Interrupts    
    BSF	    INTCON, GIE	    ; Turn on Global Interrupts

    MOVLW   0x01	    ; Delay d'1' * 4 + 2 + 2  =~  8 microseconds
    CALL    WAIT_08	    ; Should be enough for the sample and hold to work
    
    BANKSEL ADCON0
    BSF	    ADCON0, GO	    ; Set the ADC GO bit - Start the AD Converter
    btfsc   ADCON0, GO
    GOTO    $-1
    
    banksel ADRESH
    movf    ADRESH, w
    movwf   resulthi
    banksel ADRESL
    movf    ADRESL, w
    movwf   resultlo
    
    
    movwf    PORTC
       
    retlw 0 
    
DELAY_W:
    MOVWF   Del_Count
D_LOOP:
    NOP			; 1us
    DECFSZ Del_Count, 1	; Subtract 1, result goes into Del_Count
    GOTO D_LOOP		; 2 us
    RETURN

    END
    
    END
    
    