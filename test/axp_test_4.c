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

axpPCB task1PCB;
axpPID task1PID;
uint8_t task1Stack[STK+_axpMINSTK];

axpPCB task2PCB;
axpPID task2PID;
uint8_t task2Stack[STK+_axpMINSTK];

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

   task1PID = axpInitThread(&task1PCB,
      task1,
      &task1Stack[sizeof(task1Stack)-1],
      NULL);
   axpResume(task1PID);

   task2PID = axpInitThread(&task2PCB,
      task2,
      &task2Stack[sizeof(task2Stack)-1],
      NULL);
   axpResume(task2PID);

   axpResetSem(&sem);
   axpStartKernel();
   while(1);
}
