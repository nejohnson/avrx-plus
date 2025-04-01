/* Copyright (c) 2025 Neil Johnson */
#include "axplus.inc"
        _MODULE axpsetkernelstack
        _FUNCTION axpSetKernelStack

/* void * axpSetKernelStack(void *pStack) */
axpSetKernelStack:
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

        _ENDFUNC axpSetKernelStack
