/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

axpPID _axpRemoveNextObject(axpPID pCurr)
{
   axpPID pNext = pCurr->next;
    
   if (!pNext)
      return axpNOPID;

   pCurr->next = pNext->next;
   pNext->next = axpNOPID;
   return pNext;
}
