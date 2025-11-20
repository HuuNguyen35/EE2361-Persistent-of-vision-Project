.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; define main().  However, we will need a .global statement to make available ASM
; functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _hundredMicroSecond, _oneMilliSecond
    
    

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
    repeat #2
    nop
    return

    
_hundredMicroSecond:
    repeat #1591	; 100,000 / 62.5 = 1600. Then 9 for the repeat, call/return, basic nop
    nop
    return

_oneMilliSecond:
    repeat #15991	; 1,000,000 / 62.5 = 16000. Then 9 for the repeat, call/return, basic nop
    nop
    return
    
    