/*
 	avrx_waitmessage.c - Wait for a message from task context

	Copyright (c)2024    Neil Johnson (neil@njohnson.co.uk)

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the
	Free Software Foundation, Inc., 59 Temple Place - Suite 330,
	Boston, MA  02111-1307, USA.

	http://www.gnu.org/copyleft/lgpl.html
*/

#include "avrx.h"
#include "avrxcore.h"

pMessageControlBlock AvrXWaitMessage(pMessageQueue pMQ)
{
    pMessageControlBlock pMCB;
		
    do {
        _avrxBeginCritical();
        pMCB = (pMessageControlBlock)_avrxRemoveNextObject((pProcessID)pMQ);
	_avrxEndCritical();

        if(pMCB)
            break;

        AvrXWaitObjectSemaphore((pSystemObject)pMQ);
    } while(1);

    AvrXResetObjectSemaphore((pSystemObject)pMQ);
    return pMCB;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
