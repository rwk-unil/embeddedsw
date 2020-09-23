/******************************************************************************
* Copyright (c) 2020 Xilinx, Inc.  All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xsecure_ecdsa.c
*
* This file contains the implementation of the interface functions for ECDSA
* driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.0   rpo 03/31/20  Initial release
* 4.2   rpo 03/31/20  Updated file version to sync with library version
* 4.3   har 08/24/20  Added APIs to generate and verify ECDSA public key and
*                     signature
*                     Added support for ECDSA P521 curve
*
* </pre>
*
* @note
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xsecure_error.h"
#include "xsecure_ecdsa.h"
#include "xsecure_ecdsa_rsa_hw.h"
#include "xsecure_utils.h"
#include "xil_util.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/
static EcdsaCrvInfo* XSecure_EcdsaGetCrvData(XSecure_EcdsaCrvTyp CrvTyp);

/************************** Variable Definitions *****************************/

/************************** Function Definitions *****************************/
/*****************************************************************************/
/**
 * @brief	This function generates Public Key for a given curve type.
 *
 * @param	CrvTyp is the type of ECDSA curve
 * @param	D is the pointer to the static private key
 * @param	Key is the pointer to the public key
 *
 * @return
 *			- Returns XST_SUCCESS on success.
 *			- Returns error code on failure
 *
 *****************************************************************************/
int XSecure_EcdsaGenerateKey(XSecure_EcdsaCrvTyp CrvTyp, const u8* D,
	XSecure_EcdsaKey *Key)
{
	int Status = XSECURE_ECDSA_NON_SUPPORTED_CRV;
	EcdsaCrvInfo *Crv = NULL;

	if ((CrvTyp != XSECURE_ECDSA_NIST_P384) && (CrvTyp != XSECURE_ECDSA_NIST_P521)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if ((D == NULL) || (Key == NULL)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	XSecure_ReleaseReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);

	Crv = XSecure_EcdsaGetCrvData(CrvTyp);
	if(Crv != NULL) {
		Status = Ecdsa_GeneratePublicKey(Crv, D, (EcdsaKey *)Key);
		if (Status != XST_SUCCESS) {
			Status = XSECURE_ECDSA_GEN_KEY_ERR;
		}
	}

END:
	XSecure_SetReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function generates signature for a given hash and curve type.
 *
 * @param	CrvTyp is the type of ECDSA curve
 * @param	Hash is the pointer to the hash for which sign has to be generated
 * @param	HashLen is the length of the hash in bytes
 * @param	D is the pointer to the static private key
 * @param	K is an ephemeral private key
 * @param	Sign is the pointer to the signature
 *
 * @return
 *			- Returns XST_SUCCESS on success.
 *			- Returns error code on failure
 *
 * @note
 * K, the ephemeral private key, shall be an unpredictable (cryptographically
 * secure) random number unique for each signature.
 * Note that reuse or external predictability of this number generally breaks
 * the security of ECDSA.
 *****************************************************************************/
int XSecure_EcdsaGenerateSign(XSecure_EcdsaCrvTyp CrvTyp, const u8* Hash,
	const u32 HashLen, const u8* D, const u8* K, XSecure_EcdsaSign *Sign)
{
	int Status = XSECURE_ECDSA_NON_SUPPORTED_CRV;
	int GenStatus = XST_FAILURE;
	EcdsaCrvInfo *Crv = NULL;
	u8 PaddedHash[XSECURE_ECDSA_P521_SIZE_IN_BYTES] = {0U};

	if ((CrvTyp != XSECURE_ECDSA_NIST_P384) && (CrvTyp != XSECURE_ECDSA_NIST_P521)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if ((D == NULL) || (K == NULL) || (Hash == NULL) || (Sign == NULL)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if (HashLen > XSECURE_ECDSA_P521_SIZE_IN_BYTES) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	XSecure_ReleaseReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);

	Crv = XSecure_EcdsaGetCrvData(CrvTyp);
	if(Crv != NULL) {
		Xil_MemCpy(PaddedHash, Hash, HashLen);
		GenStatus = Ecdsa_GenerateSign(Crv, PaddedHash, Crv->Bits, D,
			K, (EcdsaSign *)Sign);
		if ((GenStatus == ECDSA_GEN_SIGN_BAD_R) || (GenStatus == ECDSA_GEN_SIGN_BAD_S)) {
			Status = XSECURE_ECDSA_GEN_SIGN_BAD_RAND_NUM;
		}
		else if (GenStatus == ECDSA_GEN_SIGN_INCORRECT_HASH_LEN) {
			Status = XSECURE_ECDSA_GEN_SIGN_INCORRECT_HASH_LEN;
		}
		else if (GenStatus != ECDSA_SUCCESS) {
			Status = XST_FAILURE;
		}
		else {
			Status = XST_SUCCESS;
		}
	}

END:
	XSecure_SetReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function validates the public key for a given curve type.
 *
 * @param	CrvTyp is the type of ECDSA curve
 * @param	Key is the pointer to the public key
 *
 * @return
 *			- Returns XST_SUCCESS on success.
 *			- Returns error code on failure
 *
 *****************************************************************************/
int XSecure_EcdsaValidateKey(XSecure_EcdsaCrvTyp CrvType, XSecure_EcdsaKey *Key)
{
	int Status = XSECURE_ECDSA_NON_SUPPORTED_CRV;
	int ValidateStatus = XST_FAILURE;
	EcdsaCrvInfo *Crv = NULL;

	if ((CrvType != XSECURE_ECDSA_NIST_P384) && (CrvType != XSECURE_ECDSA_NIST_P521)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if (Key == NULL) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	XSecure_ReleaseReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);

	Crv = XSecure_EcdsaGetCrvData(CrvType);
	if(Crv != NULL) {
		ValidateStatus = Ecdsa_ValidateKey(Crv, (EcdsaKey *)Key);
		if (ValidateStatus == ECDSA_KEY_ZERO) {
			Status = XSECURE_ECDSA_KEY_ZERO;
		}
		else if (ValidateStatus == ECDSA_KEY_WRONG_ORDER) {
			Status = XSECURE_ECDSA_KEY_WRONG_ORDER;
		}
		else if (ValidateStatus == ECDSA_KEY_NOT_ON_CRV) {
			Status = XSECURE_ECDSA_KEY_NOT_ON_CRV;
		}
		else if (ValidateStatus != ECDSA_SUCCESS) {
			Status = XST_FAILURE;
		}
		else {
			Status = XST_SUCCESS;
		}
	}

END:
	XSecure_SetReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function verifies the signature for a given hash, key and
 *			curve type
 *
 * @param	CrvTyp is the type of ECDSA curve
 * @param	Hash is the pointer to the hash for which sign has to be generated
 * @param	HashLen is the length of the hash in bytes
 * @param	Key is the pointer to the public key
 * @param	Sign is the pointer to the signature
 *
 * @return
 *			- Returns XST_SUCCESS on success.
 *			- Returns error code on failure
 *
 *****************************************************************************/
int XSecure_EcdsaVerifySign(XSecure_EcdsaCrvTyp CrvType, const u8 *Hash,
	const u32 HashLen, XSecure_EcdsaKey *Key, XSecure_EcdsaSign *Sign)
{
	int Status = XSECURE_ECDSA_NON_SUPPORTED_CRV;
	volatile int VerifyStatus = XST_FAILURE;
	EcdsaCrvInfo *Crv = NULL;
	u8 PaddedHash[XSECURE_ECDSA_P521_SIZE_IN_BYTES] = {0U};

	if ((CrvType != XSECURE_ECDSA_NIST_P384) && (CrvType != XSECURE_ECDSA_NIST_P521)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if ((Hash == NULL) || (Key == NULL) || (Sign == NULL)) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	if (HashLen > XSECURE_ECDSA_P521_SIZE_IN_BYTES) {
		Status = XSECURE_ECDSA_INVALID_PARAM;
		goto END;
	}

	XSecure_ReleaseReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);

	Crv = XSecure_EcdsaGetCrvData(CrvType);
	if(Crv != NULL) {
		Xil_MemCpy(PaddedHash, Hash, HashLen);
		XSECURE_TEMPORAL_CHECK(SIG_ERR, VerifyStatus, Ecdsa_VerifySign,
			Crv, PaddedHash, Crv->Bits, (EcdsaKey *)Key, (EcdsaSign *)Sign);

SIG_ERR:
		if (ECDSA_BAD_SIGN == VerifyStatus) {
			Status = XSECURE_ECDSA_BAD_SIGN;
		}
		else if (ECDSA_VER_SIGN_INCORRECT_HASH_LEN == VerifyStatus) {
			Status = XSECURE_ECDSA_VER_SIGN_INCORRECT_HASH_LEN;
		}
		else if (ECDSA_SUCCESS != VerifyStatus) {
			Status = XST_FAILURE;
		}
		else {
			Status = XST_SUCCESS;
		}
	}

END:
	XSecure_SetReset(XSECURE_ECDSA_RSA_BASEADDR,
		XSECURE_ECDSA_RSA_RESET_OFFSET);
	return Status;
}

/*****************************************************************************/
/**
 * @brief	This function performs known answer test(KAT) on ECDSA core.
 *
 * @return
 *		- Returns XST_SUCCESS on success.
 *		- Returns error code on failure
 *
 *****************************************************************************/
u32 XSecure_EcdsaKat(void)
{
	volatile u32 Status = XSECURE_ECC_KAT_FAILED_ERROR;
	const u32 QxCord[XSECURE_ECC_P384_DATA_SIZE_WORDS] = {
		0x88371BE6U, 0xFD2D8761U, 0x30DA0A10U, 0xEA9DBD2EU,
		0x30FB204AU, 0x1361EFBAU, 0xF9FDF2CEU, 0x48405353U,
		0xDE06D343U, 0x335DFF33U, 0xCBF43FDFU, 0x6C037A0U
	};

	const u32 QyCord[XSECURE_ECC_P384_DATA_SIZE_WORDS] = {
		0xEA662A43U, 0xD380E26EU, 0x57AA933CU, 0x4DD77035U,
		0x5891AD86U, 0x7AB634EDU, 0x3E46D080U, 0xD97F2544U,
		0xBF70B8A4U, 0x9204B98FU, 0x940E3467U, 0x360D38F3U
	};

	const u32 SignR[XSECURE_ECC_P384_DATA_SIZE_WORDS] = {
		0x52D853B5U, 0x41531533U, 0x2D1B4AA6U, 0x6EAF0088U,
		0x4E88153DU, 0x9F0AB1AAU, 0x12A416D8U, 0x7A50E599U,
		0xB7CA0FA0U, 0x330C7507U, 0x3495767EU, 0x5886078DU
	};

	const u32 SignS[XSECURE_ECC_P384_DATA_SIZE_WORDS] = {
		0x7A36E1AAU, 0x329682AEU, 0xE17F691BU, 0xF3869DA0U,
		0xE32BDE69U, 0x6F78CDC4U, 0x89C8FF9FU, 0x449A3523U,
		0x82CC2114U, 0xFD14B06BU, 0xBF1BF8CCU, 0x2CC10023U
	};

	const u32 HashVal[XSECURE_ECC_P384_DATA_SIZE_WORDS] = {
		0x925FA874U, 0x331B36FBU, 0x13173C62U, 0x57633F17U,
		0x110BA0CDU, 0x9E3B9A7DU, 0x46DE70D2U, 0xB30870DBU,
		0xF3CA965DU, 0xADAA0A68U, 0x9573A993U, 0x1128C8B0U
	};

	XSecure_EcdsaKey Key;
	XSecure_EcdsaSign ExpectedSign;

	Key.Qx = (u8*)QxCord;
	Key.Qy = (u8*)QyCord;
	ExpectedSign.SignR = (u8*)SignR;
	ExpectedSign.SignS = (u8*)SignS;

	Status = XSecure_EcdsaValidateKey(XSECURE_ECDSA_NIST_P384, &Key);
	if(Status != XST_SUCCESS) {
		Status = XSECURE_ECC_KAT_KEY_NOTVALID_ERROR;
		goto END;
	}

	Status = XSECURE_ECC_KAT_FAILED_ERROR;

	Status = XSecure_EcdsaVerifySign(XSECURE_ECDSA_NIST_P384, (u8*)HashVal, XSECURE_SHA3_LEN_BYTES,
		&Key, &ExpectedSign);
	if(Status != XST_SUCCESS) {
		Status = XSECURE_ECC_KAT_FAILED_ERROR;
		goto END;
	}

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief
 * This function gets the curve related information
 *
 * @param	CrvTyp is the type of the ECDSA curve
 *
 * @return
 *			- Crv which contains the curve information
 *
 *****************************************************************************/
static EcdsaCrvInfo* XSecure_EcdsaGetCrvData(XSecure_EcdsaCrvTyp CrvTyp)
{
	u32 Index;
	EcdsaCrvInfo *Crv = NULL;
	u32 TotalCurves = XSecure_EcdsaCrvsGetCount();

	for(Index = 0U; Index < TotalCurves; Index++) {
		if (XSecure_EcdsaCrvsDb[Index].CrvType == (EcdsaCrvTyp)CrvTyp) {
			Crv = &XSecure_EcdsaCrvsDb[Index];
			break;
		}
	}

	return Crv;
}
