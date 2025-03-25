#ifndef AXPLUSKERNEL_H
#define AXPLUSKERNEL_H

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "axplus.h"

struct _axpKernelData
{
    axpPID           runQueue;
    axpPID           running;
    void             *pStack;
    /* sysLevel:
     *  0xFF  == User Mode
     *  else  == Kernel Mode
     */
    uint8_t           sysLevel;    
};
#define AXP_SYSLEVEL_USER	( 0xFF )

extern struct _axpKernelData _axpKernelData;

/*****************************************************************************/
/**                            MACROS                                       **/
/*****************************************************************************/

#define _axpBeginCritical() 		asm volatile ("cli\n")
#define _axpEndCritical()   		asm volatile ("sei\n")

#define _axpCriticalEnter()       	uint8_t _axpSREG = SREG; _axpBeginCritical()
#define _axpCriticalLeave()       	SREG = _axpSREG

#define _axpCriticalScopeEnter()       { _axpCriticalEnter()
#define _axpCriticalScopeLeave()       _axpCriticalLeave() }

/*****************************************************************************/
/**                         GLOBAL DATA                                     **/
/*****************************************************************************/

extern struct _axpKernelData   _axpKernelData;
extern pTimerControlBlock      _axpTimerQueue;
extern uint8_t                 _axpTimerQLevel;

/*****************************************************************************/
/**                         CORE FUNCTIONS                                  **/
/*****************************************************************************/

extern int8_t 		_axpQueuePid(axpPID);
extern void 		_axpAppendObject(axpPID, axpPID);
extern axpPID 		_axpRemoveObject(axpPID, axpPID);
extern axpPID 		_axpRemoveNextObject(axpPID);

/*****************************************************************************
 *
 *  FUNCTION
 *      _axpEpilog
 *
 *  SYNOPSIS
 *      void _axpEpilog(void)
 *
 *  DESCRIPTION
 *      Low level kernel function that restores previous context.
 *      In C functions it MUST be the last line of code so that it is jumped
 *      to rather than called.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void _axpEpilog(void);

/*****************************************************************************
 *
 *  FUNCTION
 *      AXPENTERKERNEL
 *
 *  SYNOPSIS
 *      void AXPENTERKERNEL(pUserFrame)
 *
 *  DESCRIPTION
 *      Macros that makes a special call to the axp_EnterKernel function
 *      which returns a pointer to the User Context in the Z register.
 *
 *      For example:
 *
 *              cli();
 *              uint8_t *pUserContext;
 *              axp_EnterKernel(pUserContext);
 *              sei();
 *
 *      If the pUserContext is not used then this generates a single call
 *      instruction without saving registers.
 *
 *  RETURNS
 *      Address of the current User Context.
 *
 *****************************************************************************/

#define AXPENTERKERNEL(pUserFrame) \
	__asm__ __volatile__ ("rcall axp_EnterKernel" : "=z" (pUserFrame): : "cc" );

#endif /* AXPLUSKERNEL_H */

