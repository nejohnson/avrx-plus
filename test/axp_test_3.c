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

axpTHREADDEF(taskM, STK)
axpTHREADDEF(taskS, STK)

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
            axpSuspend(axpPIDNAME(taskS));
	 }
         else
	 {
	    debug_putchar('r');
            axpResume(axpPIDNAME(taskS));
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

   axpStartThread(axpINITTHREAD(taskM, taskM, NULL));
   axpStartThread(axpINITTHREAD(taskS, taskS, NULL));

   axpStartKernel();
   while(1);
}
