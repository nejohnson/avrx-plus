/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _implResume(axpPID pid)
{
   /* Clear the suspend flag */
   pid->flags &= ~axpPID_Suspend;

   /* If removed from the run queue, then remove the suspended
   * bit and put it on the queue.
   */
   if(pid->flags & axpPID_Suspended)
   {
      pid->flags &= ~axpPID_Suspended;
      _axpQueuePid(pid);
   }
}

void axpResume(axpPID pid)
{
   _axpProlog();
   _implResume(pid);
   _axpEpilog();
}
