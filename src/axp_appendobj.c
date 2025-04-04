/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void _axpAppendObject(axpPID pQueue, axpPID pObject)
{
   /* Walk the queue until we get to the end. */
   while(pQueue->next)
      pQueue = pQueue->next;

   /* Then append as the new last item */
   pQueue->next  = pObject;
   pObject->next = axpNOPID;
}
