/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _implWaitSem(axpSEM *pSem)
{
   axpPID pid = _axpRemoveNextObject((axpPID)(&(_axpKernelData.runQueue)));
   _axpAppendObject((axpPID)pSem, pid);
}

void axpWaitSem(axpSEM *pSem)
{
   _axpBeginCritical();
   if(*pSem == axpSEM_DONE)
   {
      *pSem = axpSEM_PEND;
      _axpEndCritical();
      return;
   }
   _axpProlog();
   _implWaitSem(pSem);
   _axpEpilog();
}
