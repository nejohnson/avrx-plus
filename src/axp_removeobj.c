/* Copyright (C) 2025 Neil Johnson */
#include "axpluskernel.h"

/*****************************************************************************
 *
 *  FUNCTION
 *      _avrxRemoveObject
 *
 *  SYNOPSIS
 *      pProcessID _avrxRemoveObject(pProcessID pQueue, pProcessID pObject)
 *
 *  DESCRIPTION
 *      Walks the Queue looking for the Object, then removes it.
 *
 *      Before:
 *
 *          pQueue              pObject          pNext
 *       +---------+          +---------+     +---------+
 *       |   next  +--/.../-->|   next  +---->|   next  +----->NOPID
 *       +---------+          +---------+     +---------+
 *       |   ...   |          |   ...   |     |   ...   |
 *       +---------+          +---------+     +---------+
 *
 *      After:
 *
 *          pQueue               pNext (returned)
 *       +---------+          +---------+
 *       |   next  +--/.../-->|   next  +----->NOPID
 *       +---------+          +---------+
 *       |   ...   |          |   ...   |
 *       +---------+          +---------+
 
 *         pObject
 *       +---------+
 *       |   next  +----->NOPID
 *       +---------+
 *       |   ...   |
 *       +---------+
 *
 *
 *  RETURNS
 *      Pointer to next object in the Queue, else NOPID if not found or there
 *      is no next object.
 *
 *****************************************************************************/

axpPID _axpRemoveObject(axpPID pQueue, axpPID pObject)
{
   while (pQueue->next)
   {
      if (pQueue->next == pObject)
      {
         _axpRemoveNextObject(pQueue);
         return pQueue->next;
      }
      pQueue = pQueue->next;
   }
   return axpNOPID;
}
