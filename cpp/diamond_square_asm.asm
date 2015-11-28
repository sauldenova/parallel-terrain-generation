; Diamond Square 

section .text

global calculateValues

; extern void calculateValues(
    ; float *results,           // rdi
    ; int16_t size,             // rsi
    ; float *values,            // rdx
    ; float *randoms,           // rcx
    ; float *factors);          // r8
; value + random * factor;
calculateValues:
    shr         rsi,    2
    _loop:
    movdqu      xmm0,   [rdx]
    movdqu      xmm1,   [rcx]
    movdqu      xmm2,   [r8]
    mulps       xmm1,   xmm2
    addps       xmm0,   xmm1
    movdqu      [rdi],  xmm0
    add         rdi,    16
    add         rdx,    16
    add         rcx,    16
    add         r8,     16
    dec         rsi
    jnz         _loop
    ret
