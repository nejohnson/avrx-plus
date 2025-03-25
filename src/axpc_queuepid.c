/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

int8_t _axpQueuePid(axpPID pid)
{
    if (pid->flags & (AXP_PID_Idle | AXP_PID_Suspend))
    {
        /* If the PID is idle or about to suspend then just mark it as
        * suspended and return -1.
        */
        pid->flags |= AXP_PID_Suspended;
        return -1;
    }
    else
    {
        /* Walk the RunQueue in descending priority to find a place to insert
        * this PID into the queue.
        */
        axpPID pCurr, pNext = (axpPID)(&(_axpKernelData.runQueue));
        int8_t counter = -1;

        uint8_t sreg = SREG;
        cli();

        do {
            counter++;
            pCurr = pNext;
            pNext = pCurr->next;
        } while ((pNext != AXP_NOPID) && pNext->priority > pid->priority);

        /* Place found, insert in list, then restore interrutps and return
        * the depth in the RunQueue.
        */
        pCurr->next = pid;
        pid->next   = pNext;
        SREG        = sreg;
        return counter;
    }
}
