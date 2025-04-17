/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 4 - Semaphores
 *
 * APIs tested:
 *   axpSEM
 *   axpSetSem
 *   axpWaitSem
 *   axpTestSem
 */

#include "axplus.h"
#include "framework.h"

#define STK 50

#define NULL ((void *)0)

axpTHREADDEF(task1, STK)
axpTHREADDEF(task2, STK)

axpSEM sem = axpSEM_DONE;

void task1(void *pArg)
{
   uint8_t n = 0;
   while(1)
   {
      debug_putchar('u');
      n++;
      if (!(n % 8)) {
         axpSetSem(&sem);
      }
      axpYield();
   }
}

void task2(void *pArg)
{
   while(1)
   {
      axpWaitSem(&sem);
      axpSEM s = axpTestSem(&sem);
      debug_putchar('W');
      if (s==axpSEM_WAIT)
         debug_putchar('w');
      else if (s==axpSEM_DONE)
         debug_putchar('d');
      else
         debug_putchar('p');
   }
}

int main(void)
{
   axpSetKernelStack(0);

   axpStartThread(axpINITTHREAD(task1, task1, NULL));
   axpStartThread(axpINITTHREAD(task2, task2, NULL));

   axpResetSem(&sem);
   axpStartKernel();
   while(1);
}
