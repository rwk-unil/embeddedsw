/******************************************************************************
*
* Copyright (C) 2019 Xilinx, Inc.  All rights reserved.
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
*******************************************************************************/
/******************************************************************************/
/**
* @file xdmapcie_intr.c
*
* This file implements interrupt functions for the XDmaPcie IP
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ---------------------------------------------------
* 1.0	tk	01/30/2019	First release
* </pre>
*
******************************************************************************/

/****************************** Include Files ********************************/
#include "xdmapcie.h"

/*************************** Constant Definitions ****************************/

/***************************** Type Definitions ******************************/

/****************** Macros (Inline Functions) Definitions ********************/

/*************************** Variable Definitions ****************************/

/*************************** Function Prototypes *****************************/

/*****************************************************************************/
/**
* Enable the Global Interrupt.
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
*
* @return 	None
*
* @note 	This bit is in the Bridge Status and Control Register.
*
******************************************************************************/
void XDmaPcie_EnableGlobalInterrupt(XDmaPcie *InstancePtr)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	Data = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
							XDMAPCIE_BSC_OFFSET);

	Data |= (XDMAPCIE_BSC_GI_MASK >> XDMAPCIE_BSC_GI_SHIFT);
	XDmaPcie_WriteReg(InstancePtr->Config.BaseAddress,
						XDMAPCIE_BSC_OFFSET, Data);

}

/*****************************************************************************/
/**
* Disable the Global Interrupt.
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
*
* @return 	None
*
* @note 	This bit is in the Bridge Status and Control Register.
*
******************************************************************************/
void XDmaPcie_DisableGlobalInterrupt(XDmaPcie *InstancePtr)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	Data = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
							XDMAPCIE_BSC_OFFSET);

	Data &= ~(XDMAPCIE_BSC_GI_MASK >> XDMAPCIE_BSC_GI_SHIFT);

	XDmaPcie_WriteReg(InstancePtr->Config.BaseAddress,
						XDMAPCIE_BSC_OFFSET, Data);

}

/*****************************************************************************/
/**
* Enable the IP interrupt bits passed into "EnableMask".
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
* @param 	EnableMask is the bit pattern for interrupts wanted to be
*		enabled.
*
* @return 	None
*
* @note 	If an interrupt is already enabled before calling this
*		function, it will stay enabled regardless of the value of
*		"EnableMask" passed from the caller.
*
******************************************************************************/
void XDmaPcie_EnableInterrupts(XDmaPcie *InstancePtr, u32 EnableMask)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	Data = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
						XDMAPCIE_IM_OFFSET);

	XDmaPcie_WriteReg(InstancePtr->Config.BaseAddress,
				XDMAPCIE_IM_OFFSET, (Data | EnableMask));

}

/*****************************************************************************/
/**
* Disable the IP interrupt bits passed into "DisableMask".
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
* @param 	DisableMask is the bit pattern for interrupts wanted to be
*		disabled.
*
* @return 	None
*
* @note 	If an interrupt is already disabled before calling this
*		function, it will stay disabled regardless of the value of
*		"DisableMask" passed from the caller.
*
******************************************************************************/
void XDmaPcie_DisableInterrupts(XDmaPcie *InstancePtr, u32 DisableMask)
{
	u32 Data = 0;

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	Data = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
							XDMAPCIE_IM_OFFSET);

	XDmaPcie_WriteReg(InstancePtr->Config.BaseAddress,
				XDMAPCIE_IM_OFFSET, (Data & (~DisableMask)));

}

/*****************************************************************************/
/**
* Get the currently enabled interrupt bits of the IP and pass them back
* to the caller into "EnabledMask".
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
* @param 	EnabledMaskPtr is a pointer to a variable where the driver will
*        	pass back the enabled interrupt bits after reading them from IP.
*
* @return 	None.
*
* @note 	None.
*
******************************************************************************/
void XDmaPcie_GetEnabledInterrupts(XDmaPcie *InstancePtr, u32 *EnabledMaskPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(EnabledMaskPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);


	*EnabledMaskPtr = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
							XDMAPCIE_IM_OFFSET);

}

/*****************************************************************************/
/**
* Get the currently pending interrupt bits of the IP and pass them back
* to the caller into "PendingMask".
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
* @param 	PendingMaskPtr is a pointer to a variable where the driver will
* 		pass back the pending interrupt bits after reading them from IP.
*
* @return 	None.
*
* @note 	None.
*
******************************************************************************/
void XDmaPcie_GetPendingInterrupts(XDmaPcie *InstancePtr, u32 *PendingMaskPtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(PendingMaskPtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);


	*PendingMaskPtr = XDmaPcie_ReadReg(InstancePtr->Config.BaseAddress,
							XDMAPCIE_ID_OFFSET);

}

/*****************************************************************************/
/**
* Clear the currently pending interrupt bits of the IP passed from
* the caller into "ClearMask".
*
* @param 	InstancePtr is the XDmaPcie instance to operate on.
* @param 	ClearMask is the bit pattern for pending interrupts wanted to
* 		be cleared.
*
* @return 	None.
*
* @note 	None.
*
******************************************************************************/
void XDmaPcie_ClearPendingInterrupts(XDmaPcie *InstancePtr, u32 ClearMask)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);


	XDmaPcie_WriteReg((InstancePtr->Config.BaseAddress), XDMAPCIE_ID_OFFSET,
			XDmaPcie_ReadReg((InstancePtr->Config.BaseAddress),
				XDMAPCIE_ID_OFFSET) & (ClearMask));
}
