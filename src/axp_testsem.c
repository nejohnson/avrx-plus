/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

axpSEM axpTestSem(axpSEM *pSem)
{
   axpSEM s;

   _axpCriticalEnter();

   s = *pSem;

   /* If DONE, return DONE but reset to PEND. */
   if (s == axpSEM_DONE)
      *pSem = axpSEM_PEND;
   else if (s != axpSEM_PEND)
      s = axpSEM_WAIT;

   _axpCriticalLeave();
   return s;
}
