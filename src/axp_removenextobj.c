/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

/*****************************************************************************
 *
 *  FUNCTION
 *      _avrxRemoveNextObject
 *
 *  SYNOPSIS
 *      pProcessID _avrxRemoveNextObject(pProcessID pCurr)
 *
 *  DESCRIPTION
 *      Removes the next Object following pCurr object if it exists
 *
 *      Before:
 *
 *          pCurr           pNext            etc
 *       +---------+     +---------+     +---------+
 *       |   next  +---->|   next  +---->|   next  +----->NOPID
 *       +---------+     +---------+     +---------+
 *       |   ...   |     |   ...   |     |   ...   |
 *       +---------+     +---------+     +---------+
 *
 *      After:
 *
 *          pCurr            etc
 *       +---------+     +---------+
 *       |   next  +---->|   next  +----->NOPID
 *       +---------+     +---------+
 *       |   ...   |     |   ...   |
 *       +---------+     +---------+
 
 *          pNext (returned)
 *       +---------+
 *       |   next  +----->NOPID
 *       +---------+
 *       |   ...   |
 *       +---------+
 *
 *  RETURNS
 *      Pointer to the next Object in the queue, or NOPID if there is no next
 *      item in the queue.
 *
 *****************************************************************************/

axpPID _axpRemoveNextObject(axpPID pCurr)
{
   axpPID pNext = pCurr->next;
    
   if (!pNext)
      return axpNOPID;

   pCurr->next = pNext->next;
   pNext->next = axpNOPID;
   return pNext;
}
