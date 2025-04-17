/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 1
 *
 * Two tasks that print out '1' and '2' round robin
 * for equal share of CPU, and a third task whose
 * priority reduces from highest to just below the
 * other two at runtime.
 *
 * APIs tested:
 *   axpSetKernelStack
 *   axpInitThread
 *   axpResume
 *   axpLeaveKernel
 *   axpYield
 *   axpSelf
 *   axpGetPriority
 *   axpSetPriority
 */

#include "axplus.h"
#include "framework.h"

#define STK 10

axpTHREADDEF(task1, STK)
axpTHREADDEF(task2, STK)
axpTHREADDEF(taskX, STK)

void task(void *pArg)
{
   char c = (char)((int)pArg);
   while(1)
   {
      debug_putchar(c);
      if (c == 'X')
         axpSetPriority(axpSelf(), 
	    axpGetPriority(axpSelf())+1);
      axpYield();
   }
}

int main(void)
{
   axpSetKernelStack(0);
   
   axpStartThread(axpINITTHREAD(task1, task, (void*)'1'));
   axpStartThread(axpINITTHREAD(task2, task, (void*)'2'));
   axpStartThread(axpINITTHREAD(taskX, task, (void*)'X'));
   axpSetPriority(axpPIDNAME(taskX), 0);

   axpStartKernel();
   while(1);
}
