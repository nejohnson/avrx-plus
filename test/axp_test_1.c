/*
 * -- AvrX-Plus -- Test Suite --
 *
 * Test 1
 *
 * Two tasks that print out '1' and '2' round robin
 * for equal share of CPU.
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

axpPCB task1PCB;
axpPID task1PID;
uint8_t task1Stack[STK+_axpMINSTK];

axpPCB task2PCB;
axpPID task2PID;
uint8_t task2Stack[STK+_axpMINSTK];

axpPCB taskXPCB;
axpPID taskXPID;
uint8_t taskXStack[STK+_axpMINSTK];

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
   task1PID = axpInitThread(&task1PCB,
      task,
      &task1Stack[sizeof(task1Stack)-1],
      (void*)'1');
   axpResume(task1PID);

   task2PID = axpInitThread(&task2PCB,
      task,
      &task2Stack[sizeof(task2Stack)-1],
      (void*)'2');
   axpResume(task2PID);

   taskXPID = axpInitThread(&taskXPCB,
      task,
      &taskXStack[sizeof(taskXStack)-1],
      (void*)'X');
   axpSetPriority(taskXPID, 0);
   axpResume(taskXPID);

   axpStartKernel();
   while(1);
}
