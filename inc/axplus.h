#ifndef AXPLUS_H
#define AXPLUS_H

#include <stdint.h>
#include <avr/pgmspace.h>

/* Kernel *************************************************************/

extern void * axp_SetKernelStack(void *);

/* Tasks **************************************************************/

typedef struct axpPCB
{
    struct axpPCB   *next;
    uint8_t            flags;
#define AXP_PID_Idle         (_BV(4))
#define AXP_PID_Suspend      (_BV(5))
#define AXP_PID_Suspended    (_BV(6))

    uint8_t            priority;
    void              *pStack;
} axpPCB, *axpPID;
#define AXP_NOPID ((axpPID)0)

/* Return address, 32 registers, Status Register */
#define _AXP_MINSTK           (35)

/* Decorate task functions with compiler attributes */
#define _AXP_TASKFUNC(A)                   \
   void A(void) __attribute__ ((noreturn)); \
   void A(void)

#define AXP_STKNAME(A) A ## Stk
#define AXP_PIDNAME(A) A ## Pid

/* Define a task */
#define AXP_TASKDEF(TASKNAME, STACKSZ)                  \
   uint8_t AXP_STKNAME(TASKNAME)[STACKSZ+_AXP_MINSTK]; \
   axpPID AXP_PIDNAME(TASKNAME);                       \
   _AXP_TASKFUNC(TASKNAME)

/* Interrupt handler */
#define AXP_SIGINT(vector) \
   ISR(vector, ISR_NAKED)

/* External definition of a task */
#define AXP_EXTERNTASK(TASKNAME)  \
   _AXP_TASKFUNC(TASKNAME);       \
   extern AXP_PIDNAME(TASKNAME);

extern axpPID axp_TaskInit(uint8_t *, void (*)(void), axpPID, uint8_t);
extern void axp_RunTask(uint8_t *, void (*)(void), axpPID, uint8_t);

extern axpPID axp_Self(void);
extern uint8_t axp_GetPriority(axpPID);
extern uint8_t axp_SetPriority(axpPID, uint8_t);
extern void axp_Resume(axpPID);
extern void axp_Suspend(axpPID);
extern void axp_Yield(void);
extern void axp_IntReschedule(void);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_Terminate
 *
 *  SYNOPSIS
 *      void axp_Terminate(pProcessID)
 *
 *  DESCRIPTION
 *      Force any task to terminate.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
 extern void axp_Terminate(axpPID);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_TaskExit
 *
 *  SYNOPSIS
 *      void axp_TaskExit(void)
 *
 *  DESCRIPTION
 *      Called by a task to terminate itself.  From this point on the task can
 *      no longer be scheduled and remains in a zombie state.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axp_TaskExit(void);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_Halt
 *
 *  SYNOPSIS
 *      void axp_Halt(void)
 *
 *  DESCRIPTION
 *      Halt the system, wait for reset
 *
 *  RETURNS
 *      Never returns, it's the very last thing you ever do....
 *
 *****************************************************************************/
extern void axp_Halt(void);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_EnterKernel
 *      axp_LeaveKernel
 *
 *  SYNOPSIS
 *      void axp_EnterKernel(void)
 *      void axp_LeaveKernel(void)
 *
 *  DESCRIPTION
 *      Switch to and from kernel context.  Must be first and last actions 
 *      inside interrupt handler.  E.g.,
 *
 *      AXP_SIGINT(TIMEY_WIMEY_vect)
 *      {
 *          axp_EnterKernel();
 *              .
 *              .
 *              .
 *          axp_LeaveKernel();
 *      }
 *
 *  RETURNS
 *      None
 *
 *****************************************************************************/
extern void axp_EnterKernel(void);
extern void axp_LeaveKernel(void);

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                         S E M A P H O R E S                           ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/

/*
 Mutex semaphores are a simple linked list of waiting
 processes.  The mutex may have the following values:

 SEM_PEND         // Semaphore is reset waiting for a signal
 SEM_DONE         // Semaphore has been triggered.
 SEM_WAIT         // Something is waiting on the semaphore
                  // Any other value is the address of a processID
*/
#define AXP_SEM_PEND ((Mutex)0)
#define AXP_SEM_DONE ((Mutex)1)
#define AXP_SEM_WAIT ((Mutex)2)

typedef axpPID Mutex, *pMutex;     /* A mutex is a pointer to a process */

#define AXP_MUTEX(A)\
        Mutex A

extern void axp_SetSemaphore(pMutex);
extern void axp_IntSetSemaphore(pMutex);
extern void axp_WaitSemaphore(pMutex);

extern Mutex axp_TestSemaphore(pMutex);
#define axp_IntTestSemaphore(A) \
            axp_TestSemaphore(A)

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_ResetSemaphore
 *
 *  SYNOPSIS
 *      void axp_ResetSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Resets the semaphore.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axp_ResetSemaphore(pMutex);

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                    S Y S T E M    O B J E C T S                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/

typedef struct SystemObject
{
    struct SystemObject *next;
    Mutex semaphore;    
}
* pSystemObject, SystemObject;


/*****************************************************************************
 *
 *  FUNCTION
 *      axp_SetObjectSemaphore
 *      axp_IntSetObjectSemaphore
 *
 *  SYNOPSIS
 *      void axp_SetObjectSemaphore(pSystemObject)
 *      void axp_IntSetObjectSemaphore(pSystemObject) 
 *
 *  DESCRIPTION
 *      Sets the semaphore within a System Object.
 *      The Int version is safe to be called from within an interrupt handler.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axp_SetObjectSemaphore(pSystemObject);
extern void axp_IntSetObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_ResetObjectSemaphore
 *
 *  SYNOPSIS
 *      void axp_ResetObjectSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Resets the semaphore within a System Object.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axp_ResetObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_TestObjectSemaphore
 *
 *  SYNOPSIS
 *      void axp_TestObjectSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Gets the state of the semaphore within a System Object.
 *
 *  RETURNS
 *      Semaphore state:
 *           SEM_PEND         // Semaphore is reset waiting for a signal
 *           SEM_DONE         // Semaphore has been triggered.
 *           SEM_WAIT         // Something is waiting on the semaphore
 *                            // Any other value is the address of a processID
 *
 *****************************************************************************/
extern Mutex axp_TestObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_WaitObjectSemaphore
 *
 *  SYNOPSIS
 *      void axp_WaitObjectSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Waits on the semaphore within a System Object.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axp_WaitObjectSemaphore(pSystemObject);

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                           M E S S A G E S                             ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/

/*
    Message Queues are really an extension of Semaphore
    queue.  They can be used for mutual exlusion (have a
    message be the baton) or as a general purpose message
    passing system.  This is needed for tasks that must block
    waiting for several things at once (timer, interrupt, etc)
*/
typedef struct MessageControlBlock
{
    SystemObject SObj;
}
* pMessageControlBlock, MessageControlBlock;

#define NOMESSAGE ((pMessageControlBlock)0)

#define AXP_MESSAGE(A) \
        MessageControlBlock A

typedef struct MessageQueue
{
    pMessageControlBlock message;    /* List of messages */
    axpPID pid;        /* List of processes */
}
* pMessageQueue, MessageQueue;

#define AXP_MESSAGEQ(A)\
        MessageQueue A

extern pMessageControlBlock axp_RecvMessage(pMessageQueue);

extern pMessageControlBlock axp_WaitMessage(pMessageQueue);

extern void axp_SendMessage(pMessageQueue, pMessageControlBlock);

extern void axp_IntSendMessage(pMessageQueue, pMessageControlBlock);

#define axp_AckMessage(A) \
        axp_SetObjectSemaphore((pSystemObject)(A))
        
#define axp_WaitMessageAck(A) \
        axp_WaitObjectSemaphore((pSystemObject)(A))

#define axp_TestMessageAck(A) \
        axp_TestObjectSemaphore((pSystemObject)(A))

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                              T I M E R S                              ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/

/*
    The timer queue manager is a service run in kernel mode and is tuned
    to minimize interrupt latency while queueing, tracking and dequeuing
    timers
*/
typedef struct TimerControlBlock
{
    struct SystemObject SObj;
    uint16_t count;
}
* pTimerControlBlock, TimerControlBlock;

#define NOTIMER ((pTimerControlBlock)0)

#define AXP_TIMER(A) TimerControlBlock A

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_StartTimer
 *
 *  SYNOPSIS
 *      void axp_StartTimer(pTimerControlBlock pTCB, uint16_t count)
 *
 *  DESCRIPTION
 *      Start a timer pTCB to run for count system ticks.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axp_StartTimer(pTimerControlBlock, uint16_t);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_CancelTimer
 *
 *  SYNOPSIS
 *      pTimerControlBlock axp_CancelTimer(pTimerControlBlock pTCB)
 *
 *  DESCRIPTION
 *      Cancels timer pTCB immediately.  Any waiting task is scehduled to run.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern pTimerControlBlock axp_CancelTimer(pTimerControlBlock);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_Delay
 *
 *  SYNOPSIS
 *      void axp_Delay(pTimerControlBlock pTCB, uint16_t count)
 *
 *  DESCRIPTION
 *      Utility function combining axp_StartTimer() and axp_WaitTimer().
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axp_Delay(pTimerControlBlock, uint16_t);

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_WaitTimer
 *
 *  SYNOPSIS
 *      void axp_WaitTimer(pTimerControlBlock pTCB)
 *
 *  DESCRIPTION
 *      Blocking wait for a timer to complete.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

#define axp_WaitTimer(A) \
        axp_WaitObjectSemaphore((pSystemObject)(A))

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_TestTimer
 *
 *  SYNOPSIS
 *      void axp_TestTimer(pTimerControlBlock pTCB)
 *
 *  DESCRIPTION
 *      Non-blocking check to see if a timer is running or not.
 *
 *  RETURNS
 *      Timer state:
 *           SEM_PEND         // Timer running but nothing waiting
 *           SEM_DONE         // Timer expired
 *           other            // Timer is running and something is waiting
 *
 *****************************************************************************/
        
#define axp_TestTimer(A) \
        axp_TestObjectSemaphore((pSystemObject)(A))

/*****************************************************************************
 *
 *  FUNCTION
 *      axp_TimerHandler
 *
 *  SYNOPSIS
 *      void axp_TimerHandler(void)
 *
 *  DESCRIPTION
 *      Kernel Function to be called by timer ISR.
 *      The simplest timer handler is:
 *
 *	AXP_SIGINT(TIMER0_OVF_vect)
 *	{
 *		axp_EnterKernel();          // Switch to kernel stack/context
 *		TCNT0 = TCNT0_INIT;
 *		axp_TimerHandler();         // Call Time queue manager
 *		axp_LeaveKernel();          // Return to tasks
 *	}
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axp_TimerHandler(void);

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                     T I M E R   M E S S A G E S                       ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/
/*
    A special version of timers that send messages rather than firing
    a semaphore.
*/
typedef struct TimerMessageBlock
{
    union
    {
        struct MessageControlBlock mcb;
        struct TimerControlBlock tcb;
    } u;
    struct MessageQueue *queue;
}
* pTimerMessageBlock, TimerMessageBlock;

// Special versions of timer queue elements that get sent
// to a message queue when expired.

extern void axp_StartTimerMessage(pTimerMessageBlock, uint16_t, pMessageQueue);
extern pMessageControlBlock axp_CancelTimerMessage(pTimerMessageBlock, pMessageQueue);



#endif /* AXPLUS_H */
