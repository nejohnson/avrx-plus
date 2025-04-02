/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

axpPID _axpRemoveObject(axpPID pQueue, axpPID pObject)
{
   while (pQueue->next)
   {
      if (pQueue->next == pObject)
      {
         _axpRemoveNextObject(pQueue);
         return pObject;
      }
      pQueue = pQueue->next;
   }
   return axpNOPID;
}
