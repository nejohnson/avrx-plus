#ifndef __AXPLUSINC
#define __AXPLUSINC

#include <avr/io.h> 



/* C to ASM */

/* Incoming Function Parameters */
#  define p1l     R24
#  define p1h     R25
#  define p2l     R22
#  define p2h     R23
#  define p3l     R20
#  define p3h     R21
#  define r1l     R24
#  define r1h     R25

#  define tmp0    R18
#  define tmp1    R19
#  define tmp2    R20
#  define tmp3    R21
#  define tmp4    R26   /* X */
#  define tmp5    R27   /* X */

.macro _MODULE filename
		.file "\filename"
		.text
.endm

.macro _PUBLIC name
		.global \name
.endm

.macro _FUNCTION fname
		.section .text.axp.\fname,"ax",@progbits
		.global \fname
		.type \fname, @function
.endm

.macro _ENDFUNC fname
		.size \fname, .-\fname
.endm




#ifndef BV
#define BV(A) (1<<A)
#endif

/*
 Frame Pointer Offsets for various registers after the return from
 a call to _Prolog

 Used mainly to restore registers used internally (rather than saving
 and restoring) and to stuff return values across _Epilog.

*/
#define _RetLo  33      /* Return address on stack big endian */
#define _R31    33      /* Registers saved in order (little endian) */
#define _RetHi  32
#define _R30    32
#define _R29    31
#define _R28    30
#define _R27    29
#define _R26    28
#define _R25    27
#define _R24    26
#define _R23    25
#define _R22    24
#define _R0     2
#define _SREG   1       /* SP points below last saved item */

#  define _p1l     26
#  define _p1h     27
#  define _p2l     24
#  define _p2h     25
#  define _r1l     26
#  define _r1h     27


/* ******** AVRX-Plus kernel data structure offsets
   These are offsets into a slab of memory called "axpKernelData"
   defined in axp_tasking.s

   NB: CODE depends upon RunQueue being the FIRST item!
*/

#define runQueue 0	/* Head of the run queue */
#define running 2	/* Current running task */
#define pStack 4	/* User defined stack location */
#define sysLevel 6	/* re-entry counter into kernel context */

#	define axpKernelDataSz 7

/* ******** TCB (Task Control Block) offsets */

#define TaskSP          0       /* Stack pointer */
#define TaskMain        2       /* Entry point of process */
#define TaskPid         4       /* Pointer to Process ID */
#define TaskFlags       6       /* Priority */

#define TaskSz  7

/* All objects have same link list structure
   Change this to convert to big-endian for direct C access
*/

#define NextL 0		/* Big-endian for C compatibility */
#define NextH 1

/******** PID (Process ID) block offsets */

#define PidNext         0       /* Next item on list (semaphore, run) */
#define PidState        2       /* Upper Nibble: Task flags, Lower Nibble :Priority */
#define PidPriority     3
#define PidSP           4       /* Context Pointer */
#define PidSz           6

/* ******* PID (Process ID) BLOCK BIT DEFINITIONS ******* */

/* PID.State  */

#define IdleBit         4       /* Dead Task, don't schedule, resume or step */
#define SuspendBit      5       /* Mark task for suspension (may be blocked elsewhere) */
#define SuspendedBit    6       /* Mark task suspended (it was removed from the run queue) */
#define SingleStep      7       /* Enable single step debug support */

/*+ --------------------------------------------------
SEMAPHORE BIT DEFINITIONS

 General Semaphore Flags.  When a task is waiting on the
 semaphore, the semaphore is set to the TASK ID.  Otherwise
 _PEND means the semaphore is set and _DONE means it has
 been triggered.  A task will only block if it tries to
 grab a _PEND semaphore.  Semaphores are initialized in the
 _PEND state and are left in the _PEND state after each
 complete operation.  Processes are allowed to queue on
 a semaphore.  This effectively make the semaphore a Mutex
 when it has been "set" prior to the first call.  When a process
 done with the mutex, it releases it by "setting" it again.
-*/
#define _PEND           0
#define _DONE           1

#define MtxSz           2

/*+ --------------------------------------------------

 Events

 Events replace PID's in semaphores.  The kernel recognizes
 semaphore values (contents) < 0x60 as events, indexes into
 a table and calls a routine.  Macros and segments are used
 to build the table.

 */
#define TIMERMESSAGE_EV 2
#define PERIODICTIMER_EV 3      /* Not implemented */
#define _LASTEV 0x5F

/* Timers */

#define TcbNext         0       /* Pointer in linked list */
#define TcbSemaphore    2       /* Associated semaphore */
#define TcbCount        4       /* Timer ticks till expired */
#define TcbQueue        6
#define TcbSz           6       /* Primitive Timer */
#define TmbSz           8       /* Timer Message */

/* Message Queue */

#define MsqSz           4       /* Head of message queue */

#define MsqMessage      0       /* Head of list of messages */
#define MsqPid          2       /* Head of list of waiting processes */

#define QcbSz           4       /* Queue Block Size (No data) */

#define QcbNext         0
#define QcbSemaphore    2       /* Return Receipt Semaphore */
#define QcbData         4       /* pointer to data/or data */

/*+ -------------------------------------------------- 
 Handy Macros
*/


.macro EndInterrupt
        reti
.endm

.macro BeginCritical
        cli
.endm

.macro EndCritical
        sei
.endm

#define EndCriticalReturn EndInterrupt

/*
 Use this macro rather than a call to _Prolog, see
 version notes in AvrX.asm
*/

.macro AXP_Prolog
        BeginCritical
        rcall   axp_EnterKernel
        EndCritical
.endm

/*
 These register definitions are just handy aliases for the
 various index and word math registers
 */
#define		Wl 		R24
#define		Wh 		R25
#define		Xl 		R26
#define		Xh 		R27
#define		Yl 		R28
#define		Yh 		R29
#define		Zl 		R30
#define		Zh 		R31

#endif  /* __AXPLUSINC */
