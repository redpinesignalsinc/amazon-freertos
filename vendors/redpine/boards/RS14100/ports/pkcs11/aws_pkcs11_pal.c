/*
 * Amazon FreeRTOS PKCS #11 PAL for RS14100 EVK Module V1.0.0
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_pkcs11_pal.c
 * @brief Device specific helpers for PKCS11 Interface.
 */

/* Amazon FreeRTOS Includes. */
#include "aws_pkcs11.h"
#include "FreeRTOS.h"

/* C runtime includes. */
#include <stdio.h>
#include <string.h>

#include "rsi_chip.h"
#include "rsi_board.h"
#include "rsi_rom_rng.h"
#include "pkcs11_flash.h"

enum eObjectHandles
{
	InvalidHandle = 0,
	DevicePrivateKey = 1,
	DevicePublicKey,
	DeviceCertificate,
	CodeSigningKey
};

#define pkcs11palFILE_NAME_CLIENT_CERTIFICATE         "FreeRTOS_P11_Certificate.dat"
#define pkcs11palFILE_NAME_KEY                        "FreeRTOS_P11_Key.dat"
#define pkcs11palFILE_CODE_SIGN_PUBLIC_KEY            "FreeRTOS_P11_CodeSignKey.dat"

#define MFLASH_FILE_BASEADDR   0x80A8000
#define MFLASH_FILE_SIZE       (10 * 4096)

/* Flash structure */
file_structure_t g_cert_files[] =
{
		{ .path = pkcs11palFILE_NAME_CLIENT_CERTIFICATE,
		.flash_addr = MFLASH_FILE_BASEADDR,
		.max_size = MFLASH_FILE_SIZE },
		{ .path = pkcs11palFILE_NAME_KEY,
		.flash_addr = MFLASH_FILE_BASEADDR + MFLASH_FILE_SIZE,
		.max_size = MFLASH_FILE_SIZE },
		{ .path = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY,
		.flash_addr = MFLASH_FILE_BASEADDR + ( 2 * MFLASH_FILE_SIZE ),
		.max_size = MFLASH_FILE_SIZE },
		{ 0 }
};

/**
 * @brief Writes a file to local storage.
 *
 * Port-specific file write for crytographic information.
 *
 * @param[in] pxLabel       Label of the object to be saved.
 * @param[in] pucData       Data buffer to be written to file
 * @param[in] ulDataSize    Size (in bytes) of data to be saved.
 *
 * @return The file handle of the object that was stored.
 */
CK_OBJECT_HANDLE PKCS11_PAL_SaveObject( CK_ATTRIBUTE_PTR pxLabel,
		uint8_t * pucData,
		uint32_t ulDataSize )
{
	CK_OBJECT_HANDLE xHandle = InvalidHandle;

	char * pcFileName = NULL;
	if( pxLabel->pValue != NULL )
	{
		/* Translate from the PKCS#11 label to local storage file name. */
		if( 0 == memcmp( pxLabel->pValue,
				&pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
				sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) ) )
		{
			pcFileName = pkcs11palFILE_NAME_CLIENT_CERTIFICATE;
			xHandle = DeviceCertificate;
		}
		else if( 0 == memcmp( pxLabel->pValue,
				&pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
				sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) ) )
		{
			pcFileName = pkcs11palFILE_NAME_KEY;
			xHandle = DevicePrivateKey;
		}
		else if( 0 == memcmp( pxLabel->pValue,
				&pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
				sizeof( pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) ) )
		{
			pcFileName = pkcs11palFILE_NAME_KEY;
			xHandle = DevicePublicKey;
		}
		else if( 0 == memcmp( pxLabel->pValue,
				&pkcs11configLABEL_CODE_VERIFICATION_KEY,
				sizeof( pkcs11configLABEL_CODE_VERIFICATION_KEY ) ) )
		{
			pcFileName = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY;
			xHandle = CodeSigningKey;
		}
		else
		{
			pcFileName = NULL;
			xHandle = InvalidHandle;
		}
	}

	flash_init(g_cert_files);
	if( xHandle != InvalidHandle )
	{
		if( pdFALSE == flash_save_file( pcFileName, pucData, ulDataSize ) )
		{
			xHandle = InvalidHandle;
		}
	}

	return xHandle;
}

/**
 * @brief Translates a PKCS #11 label into an object handle.
 *
 * Port-specific object handle retrieval.
 *
 *
 * @param[in] pLabel         Pointer to the label of the object
 *                           who's handle should be found.
 * @param[in] usLength       The length of the label, in bytes.
 *
 * @return The object handle if operation was successful.
 * Returns eInvalidHandle if unsuccessful.
 */
CK_OBJECT_HANDLE PKCS11_PAL_FindObject( uint8_t * pLabel,
		uint8_t usLength )
{
	CK_OBJECT_HANDLE xHandle = 0;
	char *pcFileName = NULL;
	if( 0 == memcmp( pLabel,
			&pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS,
			sizeof( pkcs11configLABEL_DEVICE_CERTIFICATE_FOR_TLS ) ) )
	{
		pcFileName = pkcs11palFILE_NAME_CLIENT_CERTIFICATE;
		xHandle = DeviceCertificate;
	}
	else if( 0 == memcmp( pLabel,
			&pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS,
			sizeof( pkcs11configLABEL_DEVICE_PRIVATE_KEY_FOR_TLS ) ) )
	{
		pcFileName = pkcs11palFILE_NAME_KEY;
		xHandle = DevicePrivateKey;
	}
	else if( 0 == memcmp( pLabel,
			&pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS,
			sizeof( pkcs11configLABEL_DEVICE_PUBLIC_KEY_FOR_TLS ) ) )
	{
		pcFileName = pkcs11palFILE_NAME_KEY;
		xHandle = DevicePublicKey;
	}
	else if( 0 == memcmp( pLabel,
			&pkcs11configLABEL_CODE_VERIFICATION_KEY,
			sizeof( pkcs11configLABEL_CODE_VERIFICATION_KEY ) ) )
	{
		pcFileName = pkcs11palFILE_CODE_SIGN_PUBLIC_KEY;
		xHandle = CodeSigningKey;
	}
	else
	{
		pcFileName = NULL;
		xHandle = InvalidHandle;
	}
	return xHandle;
}

/**
 * @brief Gets the value of an object in storage, by handle.
 *
 * Port-specific file access for cryptographic information.
 *
 * This call dynamically allocates the buffer which object value
 * data is copied into.  PKCS11_PAL_GetObjectValueCleanup()
 * should be called after each use to free the dynamically allocated
 * buffer.
 *
 * @sa PKCS11_PAL_GetObjectValueCleanup
 *
 * @param[in] pcFileName    The name of the file to be read.
 * @param[out] ppucData     Pointer to buffer for file data.
 * @param[out] pulDataSize  Size (in bytes) of data located in file.
 * @param[out] pIsPrivate   Boolean indicating if value is private (CK_TRUE)
 *                          or exportable (CK_FALSE)
 *
 * @return CKR_OK if operation was successful.  CKR_KEY_HANDLE_INVALID if
 * no such object handle was found, CKR_DEVICE_MEMORY if memory for
 * buffer could not be allocated, CKR_FUNCTION_FAILED for device driver
 * error.
 */
CK_RV PKCS11_PAL_GetObjectValue( CK_OBJECT_HANDLE xHandle,
		uint8_t ** ppucData,
		uint32_t * pulDataSize,
		CK_BBOOL * pIsPrivate )
{
	CK_RV xReturn = CKR_OK;
	char *pcFileName =NULL;
	if(xHandle == DeviceCertificate )
	{
		pcFileName= pkcs11palFILE_NAME_CLIENT_CERTIFICATE;
		*pIsPrivate=CK_FALSE;
	}
	else if(xHandle == DevicePrivateKey)
	{
		pcFileName= pkcs11palFILE_NAME_KEY;
		*pIsPrivate=CK_TRUE;

	}
	else if(xHandle == DevicePublicKey)
	{
		pcFileName= pkcs11palFILE_NAME_KEY;
		*pIsPrivate=CK_FALSE;
	}
	else if(xHandle == CodeSigningKey)
	{
		pcFileName= pkcs11palFILE_CODE_SIGN_PUBLIC_KEY;
		*pIsPrivate=CK_FALSE;
	}
	else
	{
		xReturn=CKR_KEY_HANDLE_INVALID;
	}

	flash_data_read(pcFileName,ppucData,pulDataSize);
	return xReturn;
}

/**
 * @brief Cleanup after PKCS11_GetObjectValue().
 *
 * @param[in] pucData       The buffer to free.
 *                          (*ppucData from PKCS11_PAL_GetObjectValue())
 * @param[in] ulDataSize    The length of the buffer to free.
 *                          (*pulDataSize from PKCS11_PAL_GetObjectValue())
 */
void PKCS11_PAL_GetObjectValueCleanup( uint8_t * pucData,
		uint32_t ulDataSize )
{
	vPortFree(pucData);
}

/*-----------------------------------------------------------*/

int mbedtls_hardware_poll( void * data,
		unsigned char * output,
		size_t len,
		size_t * olen )
{
	uint32_t rand_no;
	size_t length;

	length = len;
	/*	Enable the peripheral clocks for set1 register*/
	RSI_CLK_PeripheralClkEnable1(M4CLK,HWRNG_PCLK_ENABLE);
	/*Start the Random Number Generation*/
	RSI_RNG_Start(HWRNG,0);

	while (length > 0)
	{
		RSI_RNG_GetBytes(HWRNG,&rand_no,1);;

		if (length >= sizeof(uint32_t))
		{
			memcpy(output, &rand_no, sizeof(uint32_t));
			length -= sizeof(uint32_t);
			output += sizeof(uint32_t);
		}
		else
		{
			memcpy(output, &rand_no, length);
			output += length;
			len = 0U;
		}

	}

	*olen = len;
	return 0;
}
