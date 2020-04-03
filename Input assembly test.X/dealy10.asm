; TODO INSERT CONFIG CODE HERE USING CONFIG BITS GENERATOR

; PIC10F200 Configuration Bit Settings

; Assembly source line config statements

    #include <p10F200.inc>        ; processor specific variable definitions

    GLOBAL delay10_R

; VARIABLE DEFINITIONS
    UDATA
dc1	res 1
dc2	res 1
dc3	res 1
    
 
; SUBROUTINES
    CODE
; Variable delays 10 ms to 2.55 s
   
; delay = w x 10 ms
delay10_R
    movwf   dc3		    ; delay = 1 + 
dly2			    
    movlw   .13		    ; repeat 13 times
    movwf   dc2		    ; -> 13 x (767 + 3) - 1 = 10009 cycles´= 10009 uS = 10 ms
    clrf    dc1		    ; inner loop: 256 loops x 3 cycles pr loop - 1 = 767 cycles
   
dly1			    ; inner loop: 256 loops x 4 cycles pr loop - 1
    decfsz  dc1, f
    goto    dly1
    decfsz  dc2, f	    ; end middle loop 
    goto    dly1
    decfsz  dc3, f	    ; end outer loop
    goto    dly2
    retlw   0
    
    END


