/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

uint8_t _implSetSem(axpSEM *pSem)
{
   /* Even though we're in interrupt space it is still possible for interrupts
    * to be enabled (good practice) so still need to create a critical region.
    */
   _axpCriticalEnter();

   /* Trivial case of semaphore is at PEND or DONE then mark it DONE
    * and leave. */
   if(*pSem <= axpSEM_DONE)
   {
      *pSem = axpSEM_DONE;
      _axpCriticalLeave();
      return 1;
   }

   /* Otherwise take the first process from the Semaphore and queue it
    * for execution.
    */
   axpPID pid = _axpRemoveNextObject((axpPID)pSem);
   _axpCriticalLeave();
   _axpQueuePid(pid);
   return 0;
}

void axpSetSem(axpSEM *pSem)
{
   /* If either no other task was queued OR
    * called from kernel context then just
    * return.
    */
   if (_implSetSem(pSem)
   ||  _axpKernelData.sysLevel != AXP_SYSLEVEL_USER)
      return;

   /* Otherwise switch into kernel context and force a reschedule */
   _axpProlog();
   _axpEpilog();
}

void axpIntSetSem(axpSEM *pSem)
{
   _implSetSem(pSem);
}
