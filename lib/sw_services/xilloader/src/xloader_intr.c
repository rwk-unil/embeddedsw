/******************************************************************************
* Copyright (c) 2019 - 2021 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xloader_intr.c
* @addtogroup xloader_apis XilLoader Versal APIs
* @{
* @cond xloader_internal
* This file contains the code related to the interrupt handling.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00  kc   03/25/2019 Initial release
* 1.01  kc   04/09/2019 Added support for PCIe secondary boot mode and
*						partial PDI load
*       kc   09/13/2019 SBI reset is removed for SMAP boot mode to ensure smap
*						bus width value remains unchanged
* 1.02  kc   02/17/2020 Added APIs to add services to task queues in interrupt
*						context
*       kc   06/03/2020 Moved PLM GIC interrupt enablement to GIC handlers
*       bsv  04/09/2020 Code clean up of Xilloader
*       bsv  08/12/2020 Remove misleading comments
*       td   08/19/2020 Fixed MISRA C violations Rule 10.3
*       bm   10/14/2020 Code clean up
*       ana  10/19/2020 Added doxygen comments
* 1.03  bsv  07/19/2021 Disable UART prints when invalid header is encountered
*                       in slave boot modes
*       bsv  08/02/2021 Updated function return type as part of code clean up
*       bsv  09/05/2021 Disable prints in slave boot modes in case of error
* 1.06  am   11/24/2021 Fixed doxygen warnings
*
* </pre>
*
* @note
* @endcond
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xplmi_hw.h"
#include "xloader.h"
#include "xplmi_proc.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define XLOADER_SBI_DELAY_IN_MICROSEC		(5000U) /**< Flag indicates SBI
                                                         * delay in micro second
							 */
#define XLOADER_LOG_LEVEL_MASK		(0xF0U) /**< Flag indicates Log level
                                                 * mask */

/************************** Function Prototypes ******************************/
/**
 * @{
 * @cond xloader_internal
 */
static int XLoader_SbiLoadPdi(void *Data);

/************************** Variable Definitions *****************************/

/*****************************************************************************/

/*****************************************************************************/
/**
 * @brief	This function initializes the loader instance and registers loader
 * commands with PLM.
 *
 * @return	Returns XST_SUCCESS
 *
 *****************************************************************************/
int XLoader_IntrInit(void)
{
	int Status = XST_FAILURE;

	/*
	 * Register the SBI RDY interrupt to enable the PDI loading from
	 * SBI interface.
	 */
	Status = XPlmi_GicRegisterHandler(XPLMI_PMC_GIC_IRQ_GICP4, XPLMI_GICP4_SRC8,
		XLoader_SbiLoadPdi, (void *)0U);

	return Status;
}

/**
 * @}
 * @endcond
 */
/*****************************************************************************/
/**
 * @brief	This function is the interrupt handler for SBI data ready.
 * In this handler, PDI is loadeed through SBI interface.
 * SBI interface setting for JTAG/SMAP/AXI/HSDP should be set before
 * this handler.
 *
 * @param	Data Not used as of now, present as a part of general interrupt
 *             handler definition.
 *
 * @return	XST_SUCCESS on success and error code on failure
 *
 *****************************************************************************/
static int XLoader_SbiLoadPdi(void *Data)
{
	int Status = XST_FAILURE;
	PdiSrc_t PdiSrc;
	u64 PdiAddr;
	u32 RegVal;
	XilPdi* PdiPtr = &SubsystemPdiIns;
	(void)Data;

	XPlmi_Printf(DEBUG_DETAILED, "%s \n\r", __func__);

	/*
	 * Disable the SBI RDY interrupt so that PDI load does not
	 * interrupt itself
	 */
	XPlmi_GicIntrDisable(XPLMI_PMC_GIC_IRQ_GICP4, XPLMI_GICP4_SRC8);

	/* Store the command fields in resume data */
	RegVal = XPlmi_In32(SLAVE_BOOT_SBI_CTRL) &
			SLAVE_BOOT_SBI_CTRL_INTERFACE_MASK;
	if (RegVal == 0U) {
		PdiSrc = XLOADER_PDI_SRC_SMAP;
	}
	else {
		PdiSrc = XLOADER_PDI_SRC_SBI;
	}

	PdiAddr = 0U;
	PdiPtr->PdiType = XLOADER_PDI_TYPE_PARTIAL;
	PdiPtr->IpiMask = 0U;
	if (PdiPtr->ValidHeader == (u8)FALSE) {
		DebugLog->LogLevel &= XLOADER_LOG_LEVEL_MASK;
	}
	XPlmi_Printf(DEBUG_GENERAL, "SBI PDI Load: Started\n\r");
	Status = XLoader_LoadPdi(PdiPtr, PdiSrc, PdiAddr);
	if (Status != XST_SUCCESS) {
		goto END;
	}
	XPlmi_Printf(DEBUG_GENERAL, "SBI PDI Load: Done\n\r");

END:
	if (Status != XST_SUCCESS) {
		/* Update the error code */
		XPlmi_ErrMgr(Status);
		PdiPtr->ValidHeader = (u8)FALSE;
		DebugLog->LogLevel |= (DebugLog->LogLevel >> XPLMI_LOG_LEVEL_SHIFT);
		usleep(XLOADER_SBI_DELAY_IN_MICROSEC);
	}
	XLoader_ClearIntrSbiDataRdy();
	return XST_SUCCESS;
}

/**
 * @{
 * @cond xloader_internal
 */
/*****************************************************************************/
/**
 * @brief	This function clears the previous SBI data ready
 * and enables IRQ for next interrupt.
 *
 * @return	None
 *
 *****************************************************************************/
void XLoader_ClearIntrSbiDataRdy(void)
{
	/* Clear the SBI interrupt */
	XPlmi_UtilRMW(SLAVE_BOOT_SBI_IRQ_STATUS,
		SLAVE_BOOT_SBI_IRQ_STATUS_DATA_RDY_MASK,
		SLAVE_BOOT_SBI_IRQ_STATUS_DATA_RDY_MASK);
	XPlmi_UtilRMW(SLAVE_BOOT_SBI_IRQ_ENABLE,
		SLAVE_BOOT_SBI_IRQ_ENABLE_DATA_RDY_MASK,
		SLAVE_BOOT_SBI_IRQ_ENABLE_DATA_RDY_MASK);

	/* Clear and Enable GIC interrupt */
	XPlmi_GicIntrClearStatus(XPLMI_PMC_GIC_IRQ_GICP4, XPLMI_GICP4_SRC8);
	XPlmi_GicIntrEnable(XPLMI_PMC_GIC_IRQ_GICP4, XPLMI_GICP4_SRC8);
}

/**
 * @}
 * @endcond
 */

/** @} */