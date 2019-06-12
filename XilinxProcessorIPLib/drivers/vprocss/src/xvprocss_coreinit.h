/******************************************************************************
 *
 * Copyright (C) 2015 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMANGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 *
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xvprocss_coreinit.h
* @addtogroup vprocss_v2_4
* @{
* @details
*
* This header file contains the video processing engine sub-cores
* initialization routines and helper functions.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  rco  07/21/15   Initial Release

* </pre>
*
******************************************************************************/
#ifndef XVPROCSS_COREINIT_H__  /* prevent circular inclusions */
#define XVPROCSS_COREINIT_H__  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

#include "xvprocss.h"
/************************** Constant Definitions *****************************/

/************************** Function Prototypes ******************************/
int XVprocSs_SubcoreInitResetAxis(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitResetAximm(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitRouter(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitCsc(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitHScaler(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitVScaler(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitHCrsmplr(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitVCrsmpleIn(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitVCrsmpleOut(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitLetterbox(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitVdma(XVprocSs *XVprocSsPtr);
int XVprocSs_SubcoreInitDeinterlacer(XVprocSs *XVprocSsPtr);

#ifdef __cplusplus
}
#endif

#endif
/** @} */
