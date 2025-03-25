/* Copyright (C) 2025 Neil Johnson */
#include "axplus.inc"

        _MODULE   axp_leavekernel
        _FUNCTION axp_LeaveKernel

axp_LeaveKernel:
        pop     R30
        pop     R30

	_PUBLIC _axpEpilog
_axpEpilog:
	ldi	Zl, lo8(_axpKernelData)
	ldi	Zh, hi8(_axpKernelData)
        BeginCritical
        ldd     R16, Z+sysLevel    ; Interrupts off..
        dec     R16
        std	Z+sysLevel, R16
        brge    SkipTaskSwap

        ldd     Yh, Z+runQueue+NextH
        ldd     Yl, Z+runQueue+NextL
        std     Z+running+NextH, Yh
        std     Z+running+NextL, Yl   ; Update current running task
        adiw    Yl, 0
        breq    _IdleTask

        ldd     Xh, Y+PidSP+NextH
        ldd     Xl, Y+PidSP+NextL
        out     _SFR_IO_ADDR(SPL), Xl
        out     _SFR_IO_ADDR(SPH), Xh         ; 20 cycles
SkipTaskSwap:                   ; 20/6
        pop     R0
        out     _SFR_IO_ADDR(SREG), R0
        pop     R0
        pop     R1
        pop     R2
        pop     R3
        pop     R4
        pop     R5
        pop     R6
        pop     R7
        pop     R8
        pop     R9
        pop     R10
        pop     R11
        pop     R12
        pop     R13
        pop     R14
        pop     R15
        pop     R16
        pop     R17
        pop     R18
        pop     R19
        pop     R20
        pop     R21
        pop     R22
        pop     R23
        pop     R24
        pop     R25
        pop     R26
        pop     R27
        pop     R28
        pop     R29
        pop     R30
        pop     R31
        EndCriticalReturn       ; 97/83 cycles with interrupts off

; Jump here if there are no entries in the _RunQueue.  Never return.  Epilog will
; take care of that.  NB - this code has *NO* context.  Do not put anything in
; here that uses registers as you will get hosed every time an interrupt occurs.

_IdleTask:
; Any interrupt will exit the Idle task
        sei   					; Enable interrupts
        sleep                   ; Power Down..
        rjmp    _IdleTask

        _ENDFUNC axp_LeaveKernel

