/**
 * @file IxOsPrintf.h
 *
 * @brief this file contains the API of the @ref IxOsServices component
 * 
 * @par
 * IXP400 SW Release version 2.0
 * 
 * -- Copyright Notice --
 * 
 * @par
 * Copyright 2001-2005, Intel Corporation.
 * All rights reserved.
 * 
 * @par
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * @par
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * 
 * @par
 * -- End of Copyright Notice --
 */

#include "IxTypes.h"

#ifndef IxOsPrintf_H

#ifndef __doxygen_hide
#define IxOsPrintf_H
#endif /* __doxygen_hide */

#ifdef __wince

#ifndef IX_USE_SERCONSOLE

static int
ixLogMsg(
    char *pFormat, 
    ...
    )
{    
#ifndef IN_KERNEL
    static WCHAR    pOutputString[256]; 
	static char     pNarrowStr[256];
    int             returnCnt = 0; 
    va_list ap;
    
    pOutputString[0] = 0; 
	pNarrowStr[0] = 0;	
    
    va_start(ap, pFormat);

	returnCnt = _vsnprintf(pNarrowStr, 256, pFormat, ap);

    MultiByteToWideChar(
            CP_ACP, 
            MB_PRECOMPOSED, 
            pNarrowStr, 
            -1, 
            pOutputString, 
            256
            ); 

    OutputDebugString(pOutputString);

    return returnCnt; 
#else
    return 0; 
#endif
}
#define printf ixLogMsg

#endif /* IX_USE_SERCONSOLE */

#endif /* __wince */

/**
 * @} IxOsPrintf
 */

#endif /* IxOsPrintf_H */
