/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

int8_t _axpRemoveObject(axpPID pQueue, axpPID pObject)
{
   while (pQueue->next)
   {
      if (pQueue->next == pObject)
      {
         _axpRemoveNextObject(pQueue);
         return -1;
      }
      pQueue = pQueue->next;
   }
   return 0;
}
