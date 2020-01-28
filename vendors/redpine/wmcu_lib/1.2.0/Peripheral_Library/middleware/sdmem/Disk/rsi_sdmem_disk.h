/**
 * @file       rsi_sdmem_disk.h
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
 * @brief This files contains functions prototypes releated to sdmem_fatfs disk related peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the sdmem_disk   
 *
 */


/* Includes */

#ifndef __RSI_SDMEM_DISK_H__
#define __RSI_SDMEM_DISK_H__

#include <stdint.h>
#include "diskio.h"
 

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @fn         DSTATUS sdmem_disk_initialize(uint8_t Drive);
 * @brief      This function is used to initialize the sdmem driver
 * @param[in]  Drive    : drive number.
 * @retval     STA_NOINIT Failed.
 * @retval     RES_OK Success.
 */
DSTATUS sdmem_disk_initialize(uint8_t Drive);

/**
 * @fn         DSTATUS sdmem_disk_status(uint8_t Drive);
 * @brief      This API is used to check the sdmem disk status
 * @param[in]  Drive  :drive number.
 * @retval     STA_NOINIT if Failed.
 * @retval     RES_OK  if Success.
 */
DSTATUS sdmem_disk_status(uint8_t Drive);

/**
 * @fn         DRESULT sdmem_disk_read(uint8_t Drive, uint8_t *buffer, uint32_t sector, uint8_t count);
 * @brief      This API is used to read the blocks in sd memory.
 * @param[in]  Drive    : drive number.
 * @param[in]  buffer   : The data buffer pointer to store read data from sd memory.
 * @param[in]  sector   : The start sector number to be read.
 * @param[in]  count    : No of sectors to read
 * @retval     RES_PARERR if Failed.
 * @retval     RES_OK     if Success.
 */
DRESULT sdmem_disk_read(uint8_t Drive, uint8_t *buffer, uint32_t sector, uint8_t count);

/**
 * @fn         DRESULT sdmem_disk_read(uint8_t Drive, uint8_t *buffer, uint32_t sector, uint8_t count);
 * @brief      This API is used to write the blocks of data in sd memory.
 * @param[in]  Drive  :drive number.
 * @param[out] buffer :The data buffer pointer to store write data to the sd memory.
 * @param[in]  sector :The start sector number to be written.
 * @param[in]  count  : No of sectores to write.
 * @retval     RES_PARERR Failed.
 * @retval     RES_OK Success.
 */
DRESULT sdmem_disk_write(uint8_t Drive, const uint8_t *buffer, uint32_t sector, uint8_t count);

/**
 * @fn     DRESULT sdmem_disk_ioctl(uint8_t Drive, uint8_t command, void *buffer);
 * @brief  This API is used to do Disk IO operation.
 * @param  Drive   : drive number.
 * @param  command : The command to be set.
 * @param  buffer  : The buffer pointer to store command result.
 * @retval RES_PARERR Failed.
 * @retval RES_OK Success.
 */
DRESULT sdmem_disk_ioctl(uint8_t Drive, uint8_t command, void *buffer);

/* @} */
#if defined(__cplusplus)
}
#endif

#endif /* __RSI_SDMEM_DISK_H__ */
