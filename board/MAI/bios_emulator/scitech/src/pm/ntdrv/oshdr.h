/****************************************************************************
*
*                   SciTech OS Portability Manager Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:     ANSI C
* Environment:  32-bit Windows NT drivers
*
* Description:  Include file to include all OS specific header files.
*
****************************************************************************/

#ifndef __NTDRV_OSHDR_H
#define __NTDRV_OSHDR_H

/*--------------------------- Macros and Typedefs -------------------------*/

/*---------------------------- Global variables ---------------------------*/

/*--------------------------- Function Prototypes -------------------------*/

/* Internal unicode string handling functions */

UNICODE_STRING *    _PM_CStringToUnicodeString(const char *cstr);
void                _PM_FreeUnicodeString(UNICODE_STRING *uniStr);

#endif  /* __NTDRV_OSHDR_H */
