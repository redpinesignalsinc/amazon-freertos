 /**
 * @file       sdmem_fatfs_exp.c
 * @version    1.0
 * @date       10th AUG 2018
 *
 * Copyright(C) Redpine Signals 2018
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains example code of all end to end functionality
 *
 * @section Description
 * This file contains example code to demonstrate the file system functionality.
 *
 *
 */

/* Includes -------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "rsi_board.h"
#include "rsi_chip.h"

/* Private typedef ------------------------------------------------------------------------------------------------*/

/* Private define -------------------------------------------------------------------------------------------------*/

/* Private macro --------------------------------------------------------------------------------------------------*/
#define BUFFER_SIZE (4096)
#define SOC_OPER_FREQUENCY_SIM    50000000 /*220Mhz*/

/* Private variables ----------------------------------------------------------------------------------------------*/
static FATFS fileSystem; /* File system object */
static FIL fileObject;   /* File object */

static uint8_t Write_Buffer[BUFFER_SIZE];
static uint8_t Read_Buffer [BUFFER_SIZE];

/* Private functions ----------------------------------------------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void)
{
	int i;
	FRESULT error;
	UINT bytesWritten;
	UINT bytesRead;
	const TCHAR driverNumberBuffer[3U] = {DEV_SD + '0', ':', '/'};

	SystemCoreClockUpdate();

	/*Configure Source clock and  module clock for SDMEM */
	RSI_CLK_SetSocPllFreq(M4CLK,50000000,40000000);
	RSI_CLK_M4SocClkConfig(M4CLK,M4_SOCPLLCLK,0);
	RSI_CLK_SdMemClkConfig(M4CLK ,1,SDMEM_SOCPLLCLK,1);

	DEBUGINIT();

	SysTick_Config(32000000 / 1000);

	for(i=0;i<BUFFER_SIZE;i++)
	{
		Write_Buffer[i]=i+1;
	}
  printf("This is a file system example \n");
	printf("Please insert a memory card into sd slot\n");

	if (f_mount(&fileSystem, driverNumberBuffer, 0U))
	{
		printf("Mount failed.\n");
		while(1);
	}

#if (_FS_RPATH >= 2U)
	error = f_chdrive((char const *)&driverNumberBuffer[0U]);
	if (error)
	{
		printf("Change drive failed\n");
		return -1;
	}
#endif
	printf("Create folder in card\n");
	error = f_mkdir(_T("/folder_1"));
	if (error)
	{
		if (error == FR_EXIST)
		{
			printf("Folder already exists\n");
		}
		else
		{
			printf("Make folder failed\n");
			return -1;
		}
	}
	printf("Create a file in created folder \n");
	error = f_open(&fileObject, _T("/folder_1/file_1.dat"), (FA_WRITE | FA_READ | FA_CREATE_ALWAYS));
	if (error)
	{
		if (error == FR_EXIST)
		{
			printf("This file already exists \n");
		}
		else
		{
			printf("Open file failed \n");
			return -1;
		}
	}
	printf("Write data to above created file\n");
	error = f_write(&fileObject, Write_Buffer, sizeof(Write_Buffer), &bytesWritten);
	if ((error) || (bytesWritten != sizeof(Write_Buffer)))
	{
		printf("Write file failed\n");
	}
	/* Move the file pointer */
	if (f_lseek(&fileObject, 0U))
	{
		printf("Set file pointer position failed \n");
	}
	printf("Read data from above created file\n");
	memset(Read_Buffer, 0, sizeof(Read_Buffer));
	error = f_read(&fileObject, Read_Buffer, sizeof(Read_Buffer), &bytesRead);
	if ((error) || (bytesRead != sizeof(Read_Buffer)))
	{
		printf("Read data from file failed\n");
	}
	printf("Compare both read and write data in sd card\n");
	if (memcmp(Write_Buffer, Read_Buffer, sizeof(Write_Buffer)))
	{
		printf("data comparision failed\n");
	}
	printf("data comparision done successfully\n");
	if(f_close(&fileObject))
	{
		printf("Close file failed\n");
		return -1;
	}
	printf("File closed\n");

	while (1);
}
