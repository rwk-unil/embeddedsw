/******************************************************************************
*
* Copyright (C) 2010 - 2015 Xilinx, Inc.  All rights reserved.
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
/****************************************************************************/
/**
 *
 * @file xusbps_sinit.c
* @addtogroup usbps_v2_4
* @{
 *
 * The implementation of the XUsbPs driver's static initialization
 * functionality.
 *
 * <pre>
 * MODIFICATION HISTORY:
 *
 * Ver   Who  Date     Changes
 * ----- ---- -------- -----------------------------------------------
 * 1.00a wgr  10/10/10 First release
 * </pre>
 *
 *****************************************************************************/

/***************************** Include Files ********************************/

#include "xstatus.h"
#include "xusbps.h"
#include "xparameters.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/

extern XUsbPs_Config XUsbPs_ConfigTable[];

/************************** Function Prototypes *****************************/

/****************************************************************************/
/**
*
* Looks up the controller configuration based on the unique controller ID. A
* table contains the configuration info for each controller in the system.
*
* @param	DeviceID is the ID of the controller to look up the
*		configuration for.
*
* @return
*		A pointer to the configuration found or NULL if the specified
*		controller ID was not found.
*
******************************************************************************/
XUsbPs_Config *XUsbPs_LookupConfig(u16 DeviceID)
{
	XUsbPs_Config *CfgPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XUSBPS_NUM_INSTANCES; Index++) {
		if (XUsbPs_ConfigTable[Index].DeviceID == DeviceID) {
			CfgPtr = &XUsbPs_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}
/** @} */
