/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _implSuspend(axpPID pid)
{
   axpPID pQ = (axpPID)&_axpKernelData;
   pid->flags |= axpPID_Suspend;
   _axpBeginCritical();
   if(_axpRemoveObject(pQ, pid))
      pid->flags |= axpPID_Suspended;
}

void axpSuspend(axpPID pid)
{
   _axpProlog();
   _implSuspend(pid);
   _axpEpilog();
}
