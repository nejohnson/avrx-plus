#ifndef AXPLUS_H
#define AXPLUS_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/* Kernel *************************************************************/

extern void * axpSetKernelStack(void *);

/* Tasks **************************************************************/

typedef struct axpPCB
{
    struct axpPCB   *next;
    uint8_t            flags;
#define axpPID_Idle         (1 << 4)
#define axpPID_Suspend      (1 << 5)
#define axpPID_Suspended    (1 << 6)

    uint8_t            priority;
    void              *pStack;
} axpPCB, *axpPID;
#define axpNOPID ((axpPID)0)
#define axpDEFAULT_PRIORITY (100)

/* Return address, 32 registers, Status Register */
#define _axpMINSTK           (35)

#define _axpSTKNAME(A) A ## Stk
#define _axpPIDNAME(A) A ## Pid
#define  axpPIDNAME(A) _axpPIDNAME(A)
#define _axpPCBNAME(A) A ## Pcb

/* Define a thread */
#define axpTHREADDEF(NAME, STK)                    \
   axpPCB  _axpPCBNAME(NAME);                      \
   axpPID  _axpPIDNAME(NAME) = &_axpPCBNAME(NAME); \
   uint8_t _axpSTKNAME(NAME)[STK+_axpMINSTK];

#define axpINITTHREAD(TNAME,ENTRY,ARG) \
   _axpPIDNAME(TNAME), \
   ENTRY, \
   &_axpSTKNAME(TNAME)[sizeof(_axpSTKNAME(TNAME))-1], \
   ARG

/* Interrupt handler */
#define axpSIGINT(vector) \
   ISR(vector, ISR_NAKED)

/* External definition of a task */
#define axpEXTERNTASK(TASKNAME)  \
   _axpTASKFUNC(TASKNAME);       \
   extern axpPIDNAME(TASKNAME);


extern axpPID  axpInitThread(axpPID,
                             void (*)(void *),
                             uint8_t *,
                             void *);
extern void    axpStartThread(axpPID,
                             void (*)(void *),
                             uint8_t *,
                             void *);
extern axpPID  axpSelf(void);
extern uint8_t axpGetPriority(axpPID);
extern void    axpSetPriority(axpPID, uint8_t);
extern void    axpResume(axpPID);
extern void    axpSuspend(axpPID);
extern void    axpYield(void);
extern void    axpTerminate(axpPID);
extern void    axpExitThread(void);
extern void    axpHalt(void);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpEnterKernel
 *      axpLeaveKernel
 *
 *  SYNOPSIS
 *      void axpEnterKernel(void)
 *      void axpLeaveKernel(void)
 *
 *  DESCRIPTION
 *      Switch to and from kernel context.  Must be first and last actions 
 *      inside interrupt handler.  E.g.,
 *
 *      axpSIGINT(TIMEY_WIMEY_vect)
 *      {
 *          axpEnterKernel();
 *              .
 *              .
 *              .
 *          axpLeaveKernel();
 *      }
 *
 *  RETURNS
 *      None
 *
 *****************************************************************************/
extern void axpEnterKernel(void);
extern void axpLeaveKernel(void);
#define     axpStartKernel()   axpLeaveKernel()

/*****************************************************************************/
/*****************************************************************************/
/***                                                                       ***/
/***                         S E M A P H O R E S                           ***/
/***                                                                       ***/
/*****************************************************************************/
/*****************************************************************************/

/*
 Semaphores are a simple linked list of waiting
 threads (PIDs).  The semaphore may have the following values:

 axpSEM_PEND      // Semaphore is reset waiting for a signal
 axpSEM_DONE      // Semaphore has been triggered.
 axpSEM_WAIT      // Something is waiting on the semaphore
                  // Any other value is the address of a PID
*/
#define axpSEM_PEND ((axpSEM)0)
#define axpSEM_DONE ((axpSEM)1)
#define axpSEM_WAIT ((axpSEM)2)

typedef axpPID axpSEM;     /* A semaphore is a pointer to a PID */

extern void axpSetSem(axpSEM *);
extern void axpIntSetSem(axpSEM *);
extern void axpWaitSem(axpSEM *);
extern axpSEM axpTestSem(axpSEM *);
#define axpIntTestSem(A) \
            axpTestSem(A)
extern void axpResetSem(axpSEM *);

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
    axpSEM semaphore;    
}
* pSystemObject, SystemObject;


/*****************************************************************************
 *
 *  FUNCTION
 *      axpSetObjectSemaphore
 *      axpIntSetObjectSemaphore
 *
 *  SYNOPSIS
 *      void axpSetObjectSemaphore(pSystemObject)
 *      void axpIntSetObjectSemaphore(pSystemObject) 
 *
 *  DESCRIPTION
 *      Sets the semaphore within a System Object.
 *      The Int version is safe to be called from within an interrupt handler.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axpSetObjectSemaphore(pSystemObject);
extern void axpIntSetObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpResetObjectSemaphore
 *
 *  SYNOPSIS
 *      void axpResetObjectSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Resets the semaphore within a System Object.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axpResetObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpTestObjectSemaphore
 *
 *  SYNOPSIS
 *      void axpTestObjectSemaphore(pSystemObject)
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
extern axpSEM axpTestObjectSemaphore(pSystemObject);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpWaitObjectSemaphore
 *
 *  SYNOPSIS
 *      void axpWaitObjectSemaphore(pSystemObject)
 *
 *  DESCRIPTION
 *      Waits on the semaphore within a System Object.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/
extern void axpWaitObjectSemaphore(pSystemObject);

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

#define axpMESSAGE(A) \
        MessageControlBlock A

typedef struct MessageQueue
{
    pMessageControlBlock message;    /* List of messages */
    axpPID pid;        /* List of processes */
}
* pMessageQueue, MessageQueue;

#define axpMESSAGEQ(A)\
        MessageQueue A

extern pMessageControlBlock axpRecvMessage(pMessageQueue);

extern pMessageControlBlock axpWaitMessage(pMessageQueue);

extern void axpSendMessage(pMessageQueue, pMessageControlBlock);

extern void axpIntSendMessage(pMessageQueue, pMessageControlBlock);

#define axpAckMessage(A) \
        axpSetObjectSemaphore((pSystemObject)(A))
        
#define axpWaitMessageAck(A) \
        axpWaitObjectSemaphore((pSystemObject)(A))

#define axpTestMessageAck(A) \
        axpTestObjectSemaphore((pSystemObject)(A))

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

#define axpTIMER(A) TimerControlBlock A

/*****************************************************************************
 *
 *  FUNCTION
 *      axpStartTimer
 *
 *  SYNOPSIS
 *      void axpStartTimer(pTimerControlBlock pTCB, uint16_t count)
 *
 *  DESCRIPTION
 *      Start a timer pTCB to run for count system ticks.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axpStartTimer(pTimerControlBlock, uint16_t);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpCancelTimer
 *
 *  SYNOPSIS
 *      pTimerControlBlock axpCancelTimer(pTimerControlBlock pTCB)
 *
 *  DESCRIPTION
 *      Cancels timer pTCB immediately.  Any waiting task is scehduled to run.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern pTimerControlBlock axpCancelTimer(pTimerControlBlock);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpDelay
 *
 *  SYNOPSIS
 *      void axpDelay(pTimerControlBlock pTCB, uint16_t count)
 *
 *  DESCRIPTION
 *      Utility function combining axpStartTimer() and axpWaitTimer().
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axpDelay(pTimerControlBlock, uint16_t);

/*****************************************************************************
 *
 *  FUNCTION
 *      axpWaitTimer
 *
 *  SYNOPSIS
 *      void axpWaitTimer(pTimerControlBlock pTCB)
 *
 *  DESCRIPTION
 *      Blocking wait for a timer to complete.
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

#define axpWaitTimer(A) \
        axpWaitObjectSemaphore((pSystemObject)(A))

/*****************************************************************************
 *
 *  FUNCTION
 *      axpTestTimer
 *
 *  SYNOPSIS
 *      void axpTestTimer(pTimerControlBlock pTCB)
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
        
#define axpTestTimer(A) \
        axpTestObjectSemaphore((pSystemObject)(A))

/*****************************************************************************
 *
 *  FUNCTION
 *      axpTimerHandler
 *
 *  SYNOPSIS
 *      void axpTimerHandler(void)
 *
 *  DESCRIPTION
 *      Kernel Function to be called by timer ISR.
 *      The simplest timer handler is:
 *
 *	axpSIGINT(TIMER0_OVF_vect)
 *	{
 *		axpEnterKernel();          // Switch to kernel stack/context
 *		TCNT0 = TCNT0_INIT;
 *		axpTimerHandler();         // Call Time queue manager
 *		axpLeaveKernel();          // Return to tasks
 *	}
 *
 *  RETURNS
 *      none
 *
 *****************************************************************************/

extern void axpTimerHandler(void);

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

extern void axpStartTimerMessage(pTimerMessageBlock, uint16_t, pMessageQueue);
extern pMessageControlBlock axpCancelTimerMessage(pTimerMessageBlock, pMessageQueue);



#endif /* AXPLUS_H */
