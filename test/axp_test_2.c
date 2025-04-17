/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 2
 *
 * Two tasks, one that prints out 100 'T's and then
 * terminates, the second prints out 200 'H's before
 * halting the system. The two tasks round robin
 * for equal share of CPU until the end.
 *
 * APIs tested:
 *   axpHalt
 *   axpThreadExit
 *   axpTerminate
 */

#include "axplus.h"
#include "framework.h"

#define STK 10

#define NULL ((void *)0)

axpTHREADDEF(taskHalt, STK)
axpTHREADDEF(taskTerm, STK)

void taskHalt(void *pArg)
{
   for(uint8_t i=0; i<200; i++)
   {
      debug_putchar('H');
      axpYield();
   }
   debug_putchar('!');
   axpHalt();
   while(1);
}

void taskTerm(void *pArg)
{
   for(uint8_t i=0; i<100; i++)
   {
      debug_putchar('T');
      axpYield();
   }
   debug_putchar('X');
   axpExitThread();
   while(1);
}

int main(void)
{
   axpSetKernelStack(0);

   axpStartThread(axpINITTHREAD(taskHalt, taskHalt, NULL));
   axpStartThread(axpINITTHREAD(taskTerm, taskTerm, NULL));

   axpStartKernel();
   while(1);
}
