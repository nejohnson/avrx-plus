/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _implYield(void)
{
   axpPID pQ = (axpPID)&_axpKernelData;
   _axpBeginCritical();
   axpPID pSelf = _axpKernelData.running;
   _axpRemoveObject(pQ, pSelf);
   _axpEndCritical();
   _axpQueuePid(pSelf);
}

void axpYield(void)
{
   _axpProlog();
   _implYield();
   _axpEpilog();
}
