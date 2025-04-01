/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _implTerminate(axpPID pid)
{
   pid->flags = axpPID_Idle;
   axpPID pQ = (axpPID)&_axpKernelData;
   _axpBeginCritical();
   _axpRemoveObject(pQ, pid);   
}

void axpTerminate(axpPID pid)
{
   _axpProlog();
   _implTerminate(pid);
   _axpEpilog();
}
