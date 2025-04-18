/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 5 - Interrupt and semaphore
 *
 * Bring in the asynchronous timer interrupt, in a simple
 * handler that just sets a semaphore. The corresponding
 * consumer task waits for the semaphore in a loop.
 *
 * APIs tested:
 *   axpSEM
 *   axpIntSetSem
 *   axpWaitSem
 */

#include "axplus.h"
#include "framework.h"

#define STK 50

#define NULL ((void *)0)

axpTHREADDEF(task, STK)

axpSEM sem = axpSEM_DONE;

axpSIGINT(TIMER0_OVF_vect)
{
   axpEnterKernel();
   TCNT0 = TCNT0_INIT;
   axpIntSetSem(&sem);
   debug_putchar('.');
   axpLeaveKernel();
}

void task(void *pArg)
{
   while(1)
   {
      axpWaitSem(&sem);
      debug_putchar('t');
   }
}

int main(void)
{
   axpSetKernelStack(0);

   /* Initialise Timer */
   MCUCR = _BV(SE);
   TCNT0 = TCNT0_INIT;
   TCCR0 = TMC8_CK256;

   /* Enable timer overflow interrupt */
   TIMSK = _BV(TOIE0);

   axpStartThread(axpINITTHREAD(task, task, NULL));

   axpResetSem(&sem);
   axpStartKernel();
   while(1);
}
