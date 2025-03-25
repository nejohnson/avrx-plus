/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axp_Resume(axpPID pid)
{
   _axpBeginCritical();
   uint8_t *pUserContext;
   AXPENTERKERNEL(pUserContext);
   _axpEndCritical();

   /* Clear the suspend flag */
   pid->flags &= ~AXP_PID_Suspend;

   /* If removed from the run queue, then remove the suspended
   * bit and put it on the queue.
   */
   if(pid->flags & AXP_PID_Suspended)
   {
      pid->flags &= ~AXP_PID_Suspended;
      _axpQueuePid(pid);
   }
   _axpEpilog();
}
