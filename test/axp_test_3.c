/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 3 - Suspend/Resume
 *
 * Two tasks, a master task that prints out 'M's and
 * alternately suspends and resumes a slave task, and
 * a slave task that prints out 'S's.
 *
 * APIs tested:
 *   axpSuspend
 *   axpResume
 */

#include "axplus.h"
#include "framework.h"

#define STK 10

#define NULL ((void *)0)

axpPCB taskMPCB;
axpPID taskMPID;
uint8_t taskMStack[STK+_axpMINSTK];

axpPCB taskSPCB;
axpPID taskSPID;
uint8_t taskSStack[STK+_axpMINSTK];

#define COUNT   10
void taskM(void *pArg)
{
   uint8_t n = COUNT;
   uint8_t running = 1;
   while(1)
   {
      debug_putchar('M');
      n--;
      if(n == 0) {
         debug_putchar('.');
         n = COUNT;
	 if(running) {
	    debug_putchar('s');
            axpSuspend(taskSPID);
	 }
         else
	 {
	    debug_putchar('r');
            axpResume(taskSPID);
	 }
         running = !running;
      }
      axpYield();
   }
}

void taskS(void *pArg)
{
   while(1)
   {
      debug_putchar('S');
      axpYield();
   }
}

int main(void)
{
   axpSetKernelStack(0);

   taskMPID = axpInitThread(&taskMPCB,
      taskM,
      &taskMStack[sizeof(taskMStack)-1],
      NULL);
   axpResume(taskMPID);

   taskSPID = axpInitThread(&taskSPCB,
      taskS,
      &taskSStack[sizeof(taskSStack)-1],
      NULL);
   axpResume(taskSPID);

   axpStartKernel();
   while(1);
}
