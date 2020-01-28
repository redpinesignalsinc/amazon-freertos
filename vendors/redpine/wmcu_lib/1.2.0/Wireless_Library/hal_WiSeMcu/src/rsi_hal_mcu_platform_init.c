/**
 * @file       rsi_hal_mcu_platform_init.c
 * @version    0.1
 * @date       11 OCT 2018
 *
 * Copyright(C) Redpine Signals 2018
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief HAL Board Init: Functions related to platform initialization
 *
 * @section Description
 * This file contains the list of functions for configuring the microcontroller clock.
 * Following are list of API's which need to be defined in this file.
 *
 */


/**
 * Includes
 */

#include "rsi_board.h"


/*==============================================*/
/**
 * @fn           void rsi_hal_board_init()
 * @brief        This function Initializes the platform
 * @param[in]    none 
 * @param[out]   none
 * @return       none
 * @section description
 * This function initializes the platform
 *
 */

void rsi_hal_board_init()
{
  SystemCoreClockUpdate();
#ifdef DEBUG_UART
    DEBUGINIT();
#endif
}







