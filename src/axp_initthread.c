/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

axpPID axp_InitThread(uint8_t *pStack,
                      void   (*pTask)(void),
                      axpPID   pid,
                      uint8_t  priority)
{
   *pStack-- = ((uint16_t)pTask) & 0xFF;
   *pStack-- = (((uint16_t)pTask)>>8) & 0xFF;

   for (uint8_t i=2; i<_AXP_MINSTK; i++)
      *pStack-- = 0;

   pid->pStack   = (void *)pStack;
   pid->priority = priority;
   pid->flags    = AXP_PID_Suspend
                 | AXP_PID_Suspended;
   pid->next     = 0;

   return pid;
}
