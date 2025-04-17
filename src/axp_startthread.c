/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

void axpStartThread(axpPID   pid,
                    void   (*pTask)(void *),
                    uint8_t *pStack,
                    void    *pArg)
{
   axpResume(
      axpInitThread(pid, pTask, pStack, pArg)
   );
}
