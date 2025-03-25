/* Copyright (c) 2025 Neil Johnson */
#include "axplus.inc"
        _MODULE axp_setkernelstack
        _FUNCTION axp_SetKernelStack

/* void * axp_SetKernelStack(void *pStack) */
axp_SetKernelStack:
        pop     R31
        pop     R30
        subi    R24, 0
        sbci    R25, 0
        brne    sks1
        in      R24, _SFR_IO_ADDR(SPL)
        in      R25, _SFR_IO_ADDR(SPH)
sks1:
        sts     _axpKernelData+pStack+NextL, R24
        sts     _axpKernelData+pStack+NextH, R25
        ijmp

        _ENDFUNC axp_SetKernelStack
