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

axpPCB taskHaltPCB;
axpPID taskHaltPID;
uint8_t taskHaltStack[STK+_axpMINSTK];

axpPCB taskTermPCB;
axpPID taskTermPID;
uint8_t taskTermStack[STK+_axpMINSTK];

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
   axpThreadExit();
   while(1);
}

int main(void)
{
   axpSetKernelStack(0);

   taskHaltPID = axpInitThread(&taskHaltPCB,
      taskHalt,
      &taskHaltStack[sizeof(taskHaltStack)-1],
      NULL);
   axpResume(taskHaltPID);

   taskTermPID = axpInitThread(&taskTermPCB,
      taskTerm,
      &taskTermStack[sizeof(taskTermStack)-1],
      NULL);
   axpResume(taskTermPID);

   axpStartKernel();
   while(1);
}
