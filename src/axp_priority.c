/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpSetPriority(axpPID pid, uint8_t priority)
{
   pid->priority = priority;
   if(pid == axpSelf())
   {
      axpYield();
   }
   else
   {
      axpSuspend(pid);
      axpResume(pid);
   }
}

uint8_t axpGetPriority(axpPID pid)
{
   return pid->priority;
}
