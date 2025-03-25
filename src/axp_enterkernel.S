/* Copyright (C) 2025 Neil Johnson */
#include "axplus.inc"

	_MODULE   axp_exterkernel
	_FUNCTION axp_EnterKernel

axp_EnterKernel:
	push	R29
	push	R28
	push	R27
	push	R26
	push	R25

	ldi	Yh, hi8(_axpKernelData)
	ldi	Yl, lo8(_axpKernelData)

	in	R25, _SFR_IO_ADDR(SREG)		; Save flags

	ldd	Xl, Y+sysLevel
	subi	Xl, lo8(-1)		; Carry set if results 0
	std	Y+sysLevel, Xl		; if already in kernel, then save context

	brcs	SaveContext

	ldd	Xl, Y+running+NextL	; Check if current task = IDLE
	ldd	Xh, Y+running+NextH
	or	Xh, Xl

	brne	SaveContext		; Carry cleared if results 0
		;
		; When interrupting IDLE, just reset the stack pointer to PRIOR the interrupt.
		; in Epilog, if still IDLE, control will return to the start of the IDLE loop.
		; I.E. we do not need the return address to IDLE, just the return address to the
		; caller of Prolog() (the interrupt handler).
		;
		; The code, below, is obscure, but cycle efficient.  The net effect is to reset
		; the stack to AvrXStack before returning to the interrupt handler.
		;
	in	Yl, _SFR_IO_ADDR(SPL)	;Idle task: do not bother saving registers, etc.
	in	Yh, _SFR_IO_ADDR(SPH)
	ldd	Zh, Y+6         	; Get return address
	ldd	Zl, Y+7
	adiw	Yl, 9			; Adjust pointer
	out	_SFR_IO_ADDR(SPL), Yl  	; This is cycle efficient, but obscure.
	out	_SFR_IO_ADDR(SPH), Yh
	ijmp				; ~37 cycles for IDLE task.

SaveContext:
	push	R24
        push    R23
        push    R22
        push    R21
        push    R20
        push    R19
        push    R18
        push    R17
        push    R16
        push    R15
        push    R14
        push    R13
        push    R12
        push    R11
        push    R10
        push    R9
        push    R8
        push    R7
        push    R6
        push    R5
        push    R4
        push    R3
        push    R2
        push    R1
        push    R0
        push    R25		; SREG

	in	Yl, _SFR_IO_ADDR(SPL)
	in	Yh, _SFR_IO_ADDR(SPH)         ; Grab frame pointer

	ldd	R25, Y+_R25	; Need to restore this register (used for SREG)
	ldd	tmp1, Y+_RetLo
	ldd	tmp2, Y+_RetHi
        std     Y+_R30, Zl
        std     Y+_R31, Zh      ; Swap return address with Z

	brcs	AlreadyInKernel ; Flags, still good, from above.

	ldi	Zh, hi8(_axpKernelData)
	ldi	Zl, lo8(_axpKernelData)
	ldd	Yh, Z+running+NextH    ; Point to running task
	ldd	Yl, Z+running+NextL
	in	Xh, _SFR_IO_ADDR(SPH)
	in	Xl, _SFR_IO_ADDR(SPL)
	std	Y+PidSP+NextH, Xh
	std	Y+PidSP+NextL, Xl

        ldd	tmp0, Z+pStack+NextL
	out	_SFR_IO_ADDR(SPL), tmp0
	ldd	tmp0, Z+pStack+NextH
	out	_SFR_IO_ADDR(SPH), tmp0       ; Swap to kernel stack
	mov	Yl, Xl
	mov	Yh, Xh		; restore frame pointer

AlreadyInKernel:                ; (85/102)
	clr     R1              ; R1 = __Zero__ for Avr-gcc
        mov     Zl, tmp1        ;
        mov     Zh, tmp2
	ijmp			; Return to caller (89/106)

        _ENDFUNC axp_EnterKernel
