 /**
 * @file       rsi_sdmem_disk.c
 * @version    1.0
 * @date       10th AUG 2018
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This file contains functions related to sdmem disk for FATFS
 *
 */
#include <stdio.h>
#include <string.h>
#include "rsi_sdmem.h"
#include "rsi_sdmem_disk.h"


SMIH_CARD_CONFIG_T      SdmemConfgstruct ;
void sdmem_callback_handler(uint32_t event)
{
	switch(event)
	{
	case COMMAND_COMPLETE   :
		break;
	case TRANSFER_COMPLETE  :
		break;
	case CARD_INSERTION     :
		break;
	case CARD_REMOVAL       :
		break;
	case CARD_INTERRUPT     :
		break;
	}
}

DRESULT sdmem_disk_write(uint8_t physicalDrive, const uint8_t *buffer, uint32_t sector, uint8_t count)
{

	/*multi block write*/
	if(RSI_OK != RSI_SDMEM_BlocksWrite(&SdmemConfgstruct, buffer ,sector,count))
	{
		return RES_ERROR;
	}
	return RES_OK;

}

DRESULT sdmem_disk_read(uint8_t physicalDrive, uint8_t *buffer, uint32_t sector, uint8_t count)
{

	/*multi block read*/
	if(RSI_OK != RSI_SDMEM_BlocksRead(&SdmemConfgstruct, buffer ,sector,count))
	{
		return RES_ERROR;
	}
	return RES_OK;
}

DRESULT sdmem_disk_ioctl(uint8_t physicalDrive, uint8_t command, void *buffer)
{
	DRESULT result = RES_OK;

	switch (command)
	{
	case GET_SECTOR_COUNT:
		if (buffer)
		{
			*(uint32_t *)buffer =7747584;//SdiohConfg.numberOfBlocks;
		}
		else
		{
			result = RES_PARERR;
		}
		break;
	case GET_SECTOR_SIZE:
		if (buffer)
		{
			*(uint32_t *)buffer =SdmemConfgstruct.byteBlockSize;
		}
		else
		{
			result = RES_PARERR;
		}
		break;
	case GET_BLOCK_SIZE:
		if (buffer)
		{
			*(uint32_t *)buffer = 127;
		}
		else
		{
			result = RES_PARERR;
		}
		break;
	case CTRL_SYNC:
		result = RES_OK;
		break;
	default:
		result = RES_PARERR;
		break;
	}

	return result;
}

DSTATUS sdmem_disk_status(uint8_t physicalDrive)
{
	if (physicalDrive != DEV_SD)
	{
		return STA_NOINIT;
	}
	return 0;
}

DSTATUS sdmem_disk_initialize(uint8_t physicalDrive)
{
	// Clear structure
	memset(&SdmemConfgstruct, 0, sizeof(SdmemConfgstruct));

	SdmemConfgstruct.highSpeedEnable    = 0;
	SdmemConfgstruct.uhsModeSelect      = UHS_NONE;      //uhs mode will work only in 4bit mode
	SdmemConfgstruct.admaMode           = 0;
	SdmemConfgstruct.busWidthMode       = SMIH_BUS_WIDTH4;
	SdmemConfgstruct.clock              = 25000000;
	SdmemConfgstruct.voltage            = 0; //set this bit in case of 1.8v

	/*initialize sd card*/
	if(RSI_OK != RSI_SDMEM_Enumeration(&SdmemConfgstruct,sdmem_callback_handler))
	{
		return RES_ERROR;
	}

	/*change clock after enumeratuion*/
	smih_clock_config(&SdmemConfgstruct,25000000);

	return RES_OK;
}
