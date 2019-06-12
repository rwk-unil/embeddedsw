/******************************************************************************
*
* Copyright (C) 2015 - 2016 Xilinx, Inc.  All rights reserved.
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
* @file xhdcp22_rx_sinit.c
* @addtogroup hdcp22_rx_v2_2
* @{
* @details
*
* This file contains static initialization method for Xilinx HDCP 2.2 Receiver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00  MH   10/30/15 First Release
*</pre>
*
*****************************************************************************/


/***************************** Include Files *********************************/

#include "xhdcp22_rx.h"
#include "xparameters.h"

/************************** Constant Definitions *****************************/
#ifndef XPAR_XHDCP22_RX_NUM_INSTANCES
#define XPAR_XHDCP22_RX_NUM_INSTANCES  0
#endif
/***************** Macros (Inline Functions) Definitions *********************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions *****************************/

/*****************************************************************************/
/**
*
* This function returns a reference to an XHdcp22_Rx_Config structure based
* on the core id, <i>DeviceId</i>. The return value will refer to an entry in
* the device configuration table defined in the xhdcp22_rx_g.c file.
*
* @param	DeviceId is the unique core ID of the HDCP RX core for the
*			lookup operation.
*
* @return	XHdcp22Rx_LookupConfig returns a reference to a config record
*			in the configuration table (in xhdmi_tx_g.c) corresponding
*			to <i>DeviceId</i>, or NULL if no match is found.
*
* @note		None.
*
******************************************************************************/
XHdcp22_Rx_Config *XHdcp22Rx_LookupConfig(u16 DeviceId)
{
	extern XHdcp22_Rx_Config XHdcp22_Rx_ConfigTable[XPAR_XHDCP22_RX_NUM_INSTANCES];
	XHdcp22_Rx_Config *CfgPtr = NULL;
	u32 Index;

	/* Checking for device id for which instance it is matching */
	for (Index = (u32)0x0; Index < (u32)(XPAR_XHDCP22_RX_NUM_INSTANCES); Index++) {
		/* Assigning address of config table if both device ids
		 * are matched
		 */
		if (XHdcp22_Rx_ConfigTable[Index].DeviceId == DeviceId) {
			CfgPtr = &XHdcp22_Rx_ConfigTable[Index];
			break;
		}
	}

	return CfgPtr;
}

/** @} */
