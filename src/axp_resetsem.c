/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpResetSem(axpSEM *pSem)
{
   _axpBeginCritical();
   if (*pSem == axpSEM_DONE)
      *pSem = axpSEM_PEND;
   _axpEndCritical();
}
