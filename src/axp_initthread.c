/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

axpPID axpInitThread(axpPID   pid,
                     void   (*pTask)(void *),
                     uint8_t *pStack,
                     void    *pArg)
{
   *pStack-- = ((uint16_t)pTask) & 0xFF;
   *pStack-- = (((uint16_t)pTask)>>8) & 0xFF;

   for (uint8_t i=2; i<_axpMINSTK; i++)
      *pStack-- = 0;

   if (pArg) {
      pStack[26]    = ((uint16_t)pArg) & 0xFF;
      pStack[27]    = (((uint16_t)pArg)>>8) & 0xFF;
   }

   pid->pStack   = (void *)pStack;
   pid->priority = axpDEFAULT_PRIORITY;
   pid->flags    = axpPID_Suspend
                 | axpPID_Suspended;
   pid->next     = 0;

   return pid;
}
