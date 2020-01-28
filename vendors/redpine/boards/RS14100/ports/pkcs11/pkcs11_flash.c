/*
 * Amazon FreeRTOS V1.0.0
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
/* Includes ------------------------------------------------------------------*/
#include "rsi_qspi.h"
#include "rsi_egpio.h"
#include "rsi_qspi_proto.h"
#include "rsi_chip.h"
#include "rsi_board.h"
#include "pkcs11_flash.h"
#include "stddef.h"
#include "string.h"
#include "FreeRTOS.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
//#define    DMA_READ
#define      IO_READ

#define    PadSelectionEnable 15
#define    QSPI_MODE					2

/* Private macro -------------------------------------------------------------*/
#define    SINGLE_MODE_ENABLE  1
#define    ADDRESS             0x808F000
#define    SIZE                256
#define    CHNL0               0


/* This function fills QSPI configuration structures */
void GetQspiConfig(spi_config_t *spi_config)
{
	memset(spi_config, 0, sizeof(spi_config_t));

	spi_config->spi_config_1.inst_mode         = SINGLE_MODE;
	spi_config->spi_config_1.addr_mode         = SINGLE_MODE;
	spi_config->spi_config_1.data_mode         = SINGLE_MODE;
	spi_config->spi_config_1.dummy_mode        = SINGLE_MODE;
	spi_config->spi_config_1.extra_byte_mode   = SINGLE_MODE;
	spi_config->spi_config_1.read_cmd          = READ;
	spi_config->spi_config_1.no_of_dummy_bytes = 0;
	spi_config->spi_config_1.prefetch_en       = DIS_PREFETCH;
	spi_config->spi_config_1.dummy_W_or_R      = DUMMY_READS;
	spi_config->spi_config_1.extra_byte_en     = 0;
	spi_config->spi_config_1.d3d2_data         = 3;
	spi_config->spi_config_1.continuous        = CONTINUOUS;
	spi_config->spi_config_1.flash_type        = MX_QUAD_FLASH;


	spi_config->spi_config_2.auto_mode         = EN_AUTO_MODE;
	spi_config->spi_config_2.cs_no             = CHIP_ZERO;
	spi_config->spi_config_2.neg_edge_sampling = NEG_EDGE_SAMPLING;
	spi_config->spi_config_2.qspi_clk_en       = QSPI_FULL_TIME_CLK;
	spi_config->spi_config_2.protection        = DNT_REM_WR_PROT;
	spi_config->spi_config_2.dma_mode          = NO_DMA;
	spi_config->spi_config_2.swap_en           = SWAP;
	spi_config->spi_config_2.full_duplex       = IGNORE_FULL_DUPLEX;
	spi_config->spi_config_2.wrap_len_in_bytes = NO_WRAP;
	spi_config->spi_config_2.addr_width_valid  = 0;
	spi_config->spi_config_2.addr_width        = _24BIT_ADDR;
	spi_config->spi_config_2.pinset_valid      = 0;
	spi_config->spi_config_2.flash_pinset      = GPIO_58_TO_63;
	spi_config->spi_config_2.dummy_cycles_for_controller = 0;

	spi_config->spi_config_3.wr_cmd                  = 0x2;
	spi_config->spi_config_3.wr_inst_mode            = SINGLE_MODE;
	spi_config->spi_config_3.wr_addr_mode            = SINGLE_MODE;
	spi_config->spi_config_3.wr_data_mode            = SINGLE_MODE;
	spi_config->spi_config_3.xip_mode                = 0;
	spi_config->spi_config_3._16bit_cmd_valid        = 0;
	spi_config->spi_config_3._16bit_rd_cmd_msb       = 0;
	spi_config->spi_config_3.ddr_mode_en             = 0;
	spi_config->spi_config_3.dummys_4_jump           = 1;

	spi_config->spi_config_4._16bit_wr_cmd_msb       = 0;
	spi_config->spi_config_4.qspi_ddr_clk_en         = 0;
	spi_config->spi_config_4.qspi_manual_ddr_phasse  = 0;
	spi_config->spi_config_4.ddr_data_mode           = 0;
	spi_config->spi_config_4.ddr_addr_mode           = 0;
	spi_config->spi_config_4.ddr_inst_mode           = 0;
	spi_config->spi_config_4.ddr_dummy_mode          = 0;
	spi_config->spi_config_4.ddr_extra_byte          = 0;
	spi_config->spi_config_4.dual_flash_mode         = 0;
	spi_config->spi_config_4.secondary_csn           = 1;
	spi_config->spi_config_4.polarity_mode           = 0;
	spi_config->spi_config_4.valid_prot_bits         = 4;
	spi_config->spi_config_4.no_of_ms_dummy_bytes    = 0;
	spi_config->spi_config_4.ddr_dll_en              = 0;
	spi_config->spi_config_4.continue_fetch_en       = 0;

	spi_config->spi_config_5.block_erase_cmd         = BLOCK_ERASE;
	spi_config->spi_config_5.busy_bit_pos            = 0;
	spi_config->spi_config_5.d7_d4_data              = 0xf;
	spi_config->spi_config_5.dummy_bytes_for_rdsr    = 0x0;
	spi_config->spi_config_5.reset_type              = 0x0;

	spi_config->spi_config_6.chip_erase_cmd          = CHIP_ERASE;
	spi_config->spi_config_6.sector_erase_cmd        = SECTOR_ERASE;

	spi_config->spi_config_7.status_reg_write_cmd    = 0x1;
	spi_config->spi_config_7.status_reg_read_cmd     = 0x5;
}

/*Init the QSPI configurations structure */

static file_structure_t *files_available=NULL;
volatile uint32_t flash_initialized=1;
file_size_t file_size;;
/**
 * @brief  flash Initialition.
 * @param  None
 * @retval None
 */
void flash_init(file_structure_t *files)
{
	if(flash_initialized)
	{
		files_available=files;

		flash_initialized=0;
	}
}


/*Read the data from flash*/
uint32_t flash_save_file(uint8_t *file,uint8_t *data,uint32_t datasize)
{
	uint32_t status=-1;
	file_structure_t *temp_file = files_available;
	volatile uint32_t i=0;
	spi_config_t spi_configs_write;
	uint8_t *data_write =NULL;
	data_write = (uint8_t *)pvPortMalloc(datasize);

	memcpy(data_write,data,datasize);
	GetQspiConfig(&spi_configs_write);

	if(temp_file == NULL)
	{
		return -1;
	}

	for(;(temp_file->flash_addr != 0)&& (temp_file->max_size != 0);temp_file++)
	{
		status = strncmp(file,temp_file->path,64);
		if(status == 0)
		{
			if(strncmp(file,"FreeRTOS_P11_Certificate.dat",64) == 0)
			{
				file_size.cert_size = datasize;
			}
			else if(strncmp(file,"FreeRTOS_P11_Key.dat",64) == 0)
			{
				file_size.key_size = datasize;
			}
			else
			{
				file_size.codesignkey_size = datasize;
			}
			break;
		}
	}

	if(status != 0)
		return 0;

	if(datasize > temp_file->max_size)
		return 0;
	/*Disable the interrupt*/
	__disable_irq();

	/*Erase the flash location before writing the data*/
	RSI_QSPI_SpiErase((qspi_reg_t *)QSPI_BASE,&spi_configs_write, SECTOR_ERASE, temp_file->flash_addr, 1, 0);
	/* writes the data to required address using qspi */
	status = RSI_QSPI_SpiWrite((qspi_reg_t *)QSPI_BASE,  &spi_configs_write,0x12ED ,
			temp_file->flash_addr, (uint8_t *) data_write,
			datasize, 256, _1BYTE, 0,0,0,0,0,0 );

	__enable_irq();
	/*Free the allocated memory of write buffer*/
	vPortFree(data_write);

	return 1;
}


/*Read the data from flash memory*/
uint32_t flash_data_read(uint8_t *file,uint8_t **data,uint32_t *datasize)
{
	file_structure_t *temp_file = files_available;
	int32_t status = -1;
	uint8_t *data_read =NULL;
	spi_config_t spi_configs_read;
	GetQspiConfig(&spi_configs_read);

	if(temp_file == NULL)
	{
		return pdFALSE;
	}

	for(;(temp_file->flash_addr != 0)&& (temp_file->max_size != 0);temp_file++)
	{
		status = strncmp(file,temp_file->path,64);
		if(status == 0)
		{
			if(strncmp(file,"FreeRTOS_P11_Certificate.dat",64) == 0)
			{
				*datasize = file_size.cert_size;
			}
			else if(strncmp(file,"FreeRTOS_P11_Key.dat",64) == 0)
			{
				*datasize = file_size.key_size;
			}
			else
			{
				*datasize = file_size.codesignkey_size;
			}

			break;
		}
	}

	if(status != 0)
		return pdFALSE;
	/*Allocate the memory to read buffer*/
	data_read = pvPortMalloc(*datasize);
	/*Disable the IRQ*/
	__disable_irq();
	/*Read the data from memory*/
	memcpy(data_read,temp_file->flash_addr, (*datasize));
	/*Enable IRQ*/
	__enable_irq();
	/*Point the read buffer to the actual buffer*/
	*data = (uint8_t *)data_read;

	return pdTRUE;
}

