.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _hundredMicroSecond, _oneMilliSecond, _ws_send_pixel_asm
    
    

;5.6, 12.8 cycles
_write_0:
    inc LATA
    repeat #3
    nop
    bclr LATA, #0
    repeat #5
    nop
    return
    
    
;11.2, 9.6 cycles
_write_1:
    inc LATA	    
    repeat #8	    ;1
    nop		    ;1+8
    bclr LATA, #0	    ;1
    repeat #2	    ;1
    nop		    ;3
    return	    

    
_hundredMicroSecond:
    repeat #1591	; 100,000 / 62.5 = 1600. Then 9 for the repeat, call/return, basic nop
    nop
    return

_oneMilliSecond:
    repeat #15991	; 1,000,000 / 62.5 = 16000. Then 9 for the repeat, call/return, basic nop
    nop
    return
    
    
;G, R, B come into w0, w1, w2
    
_ws_send_pixel_asm:
    mov     W0, W3 ;w3 is the value we're analyzing
    mov     #8, W0 ;w0 becomes the loop counter

g_loop:
    btst    W3, #7 
    bra     NZ, g_one

;5.6, 12.8 cycles
g_zero:
    ;write a 0
    inc     LATA    ;1
    repeat  #3	    ;1
    nop		    ;1+3
    bclr    LATA, #0	;1
    repeat  #4	    ;1
    nop		    ;1+4
    bra     g_next  ;2+4

;11.2, 9.6 cycles
g_one:
    ;write a 1
    inc     LATA
    repeat  #8
    nop
    bclr    LATA, #0
    repeat  #3
    nop

g_next:
    sl      W3, W3 ;shift to look at the next bit
    dec     W0, W0
    bra     NZ, g_loop


    mov     W1, W3;now R
    mov     #8, W0;w0 = loop counter

r_loop:
    btst    W3, #7
    bra     NZ, r_one

r_zero:
    inc     LATA
    repeat  #3
    nop
    bclr    LATA, #0
    repeat  #4
    nop
    bra     r_next

r_one:
    inc     LATA
    repeat  #8
    nop
    bclr    LATA, #0
    repeat  #3
    nop

r_next:
    sl      W3, W3
    dec     W0, W0
    bra     NZ, r_loop


    mov     W2, W3; now B
    mov     #8, W0;w0 = loop counter

b_loop:
    btst    W3, #7
    bra     NZ, b_one

b_zero:
    inc     LATA
    repeat  #3
    nop
    bclr    LATA, #0
    repeat  #4
    nop
    bra     b_next

b_one:
    inc     LATA
    repeat  #8
    nop
    bclr    LATA, #0
    repeat  #3
    nop

b_next:
    sl      W3, W3
    dec     W0, W0
    bra     NZ, b_loop

    return
