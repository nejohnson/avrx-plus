#ifndef AXPLUSKERNEL_H
#define AXPLUSKERNEL_H

#include <stdint.h>
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
#define AXP_SYSLEVEL_USER	 ( 0xFF )

extern struct _axpKernelData     _axpKernelData;
extern pTimerControlBlock        _axpTimerQueue;
extern uint8_t                   _axpTimerQLevel;

#define _axpBeginCritical()      asm volatile ("cli\n")
#define _axpEndCritical()        asm volatile ("sei\n")

#define _axpCriticalEnter()      uint8_t _axpSREG = SREG;\
                                 _axpBeginCritical()
#define _axpCriticalLeave()      SREG = _axpSREG

#define _axpCriticalScopeEnter() { _axpCriticalEnter()
#define _axpCriticalScopeLeave() _axpCriticalLeave() }

extern int8_t   _axpQueuePid(axpPID);
extern void     _axpAppendObject(axpPID, axpPID);
extern axpPID   _axpRemoveObject(axpPID, axpPID);
extern axpPID   _axpRemoveNextObject(axpPID);
extern void     _axpEpilog(void);

#define _axpProlog()                             \
   _axpBeginCritical();                          \
   asm volatile ("rcall axpEnterKernel" : : : ); \
   _axpEndCritical();

#endif /* AXPLUSKERNEL_H */
