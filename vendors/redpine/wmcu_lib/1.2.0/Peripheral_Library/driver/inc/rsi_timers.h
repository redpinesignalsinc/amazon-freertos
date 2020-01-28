 /**
 * @file       rsi_timers.h
 * @version    1.0
 * @date       1 Aug 2017
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains functions prototypes releated to TIMERS peripheral
 * 
 * @section Description
 * This file contains the list of function prototypes for the TIMERS and low level function definations 
 * Following are list of API's which need to be defined in this file.
 *
 */


#ifndef __RSI_TIMERS_H__
#define __RSI_TIMERS_H__

/******************************************************************************/
/*                               INCLUDE FILES                                */
/******************************************************************************/
#include "RS1xxxx.h"
#include "rsi_error.h"


#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                               LOCAL OR GLOBAL DEFINES                      */
/******************************************************************************/
 
#define   RSI_TIMERS_API_VERSION 	       RSI_DRIVER_VERSION_MAJOR_MINOR(2,00)   /*!< API version 0.1   */
#define   RSI_TIMERS_DRV_VERSION         RSI_DRIVER_VERSION_MAJOR_MINOR(00,01)  /*!< driver version 0.1*/

#define   MICRO_SEC_MODE              	 1
#define   _256_MICRO_SEC_MODE         	 2
#define   COUNTER_DOWN_MODE		           0

/****** TIMERS Events *****/
#define   RSI_TIMERS_EVENT_TIMER0_TIMEOUT  (1UL << 0)   /*!< Timer0 timeout interrupt */  
#define   RSI_TIMERS_EVENT_TIMER1_TIMEOUT  (1UL << 1)   /*!< Timer1 timeout interrupt */ 
#define   RSI_TIMERS_EVENT_TIMER2_TIMEOUT  (1UL << 2)   /*!< Timer2 timeout interrupt */ 
#define   RSI_TIMERS_EVENT_TIMER3_TIMEOUT  (1UL << 3)   /*!< Timer3 timeout interrupt */ 
	
#define   TIMER_MODE       0x18

/* Example defines */
#define    TIMER_0      0
#define    TIMER_1      1
#define    TIMER_2      2
#define    TIMER_3      3

#define    ULP_TIMER_RF_REF_CLK     				0
#define    ULP_TIMER_ULP_32KHZ_RO_CLK				1
#define    ULP_TIMER_ULP_32KHZ_RC_CLK				2
#define    ULP_TIMER_ULP_32KHZ_XTAL_CLK			3
#define    ULP_TIMER_ULP_32MHZ_RC_CLK				4
#define    ULP_TIMER_ULP_20MHZ_RO_CLK				5
#define    ULP_TIMER_SOC_CLK								6

#define    ULP_TIMER_CLK_DIV_FACT     0

#define    PERIODIC_TIMER				1
#define    ONESHOT_TIMER				0

typedef  TIMERS_Type  RSI_TIMERS_T;

/**************************************************************************************
 * \b STRUCTURES
 *************************************************************************************/

/**
 \brief  TIMERS Driver Capabilities.
*/
typedef struct  
{	
  uint32_t timerCount 	 : 4;                 /*!< Number of Timers          */ 
  uint32_t microSecMode  : 1;                 /*!< supports Micro second mode */ 
} RSI_TIMERS_CAPABILITIES_T;

/**
 * @fn          error_t RSI_TIMERS_TimerStart(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
 * @brief		    This API is used to start the timer 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code
 */
STATIC INLINE error_t  RSI_TIMERS_TimerStart(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_START = ENABLE; 
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;
}

/**
 * @fn          error_t RSI_TIMERS_TimerStop(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
 * @brief		    This API is used to stop the timer 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_TimerStop(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_STOP = ENABLE; 
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;                              
}

/**
 * @fn          error_t RSI_TIMERS_InterruptEnable(RSI_TIMERS_T *pTIMER , uint8_t timerNum)
 * @brief		    This API is used to enable the timer interrupt 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_InterruptEnable(RSI_TIMERS_T *pTIMER , uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER -> MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_INTR_ENABLE = ENABLE;		
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;															   															
}

/**
 * @fn          error_t RSI_TIMERS_InterruptDisable(RSI_TIMERS_T *pTIMER , uint8_t timerNum)
 * @brief		    This API is used to disable the timer interrupt 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_InterruptDisable(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER -> MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_INTR_ENABLE = DISABLE;			
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;															     
}

/**
 * @fn          error_t RSI_TIMERS_InterruptClear(RSI_TIMERS_T *pTIMER , uint8_t timerNum)
 * @brief		    This API is used to clear the timer interrupt 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_InterruptClear(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER -> MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_INTR_CLR = ENABLE;				
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;													     
}

/**
 * @fn          error_t RSI_TIMERS_SetMatch( RSI_TIMERS_T *pTIMER, uint8_t timerNum, uint32_t match)
 * @brief		    This API is used to disable the timer interrupt 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)
 * @param[in]   match      : delay time
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_SetMatch( RSI_TIMERS_T *pTIMER, uint8_t timerNum, uint32_t match)
{
	if(timerNum <= TIMER_3)
	{
		pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_MATCH = match;				
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;		
}

/**
 * @fn          error_t RSI_TIMERS_InterruptStatus(RSI_TIMERS_T *pTIMER , uint8_t timerNum)
 * @brief		    This API is used to get the timer interrupt status 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer interrupt status if valid timer else 0. 
 */
STATIC INLINE uint8_t RSI_TIMERS_InterruptStatus(RSI_TIMERS_T *pTIMER,uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		return (pTIMER ->MCUULP_TMR_INTR_STAT & (1 << timerNum));				
  }
	else
	{
		return 0;
	}
}

/**
 * @fn          error_t RSI_TIMERS_SetTimerType( RSI_TIMERS_T *pTIMER, uint8_t timerType, 
																               uint8_t timerNum)
 * @brief		    This API is used to set the timer type 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerType  : timer type
 *	                          - \ref MICRO_SEC_MODE
 *                            - \ref _256_MICRO_SEC_MODE
 *                            - \ref COUNTER_DOWN_MODE
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_SetTimerType( RSI_TIMERS_T *pTIMER, uint8_t timerType, 
																               uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		if((timerType == MICRO_SEC_MODE)||
			 (timerType == _256_MICRO_SEC_MODE)||
			 (timerType == COUNTER_DOWN_MODE))
		{
			pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_TYPE = timerType;
	  }
		else
		{
			return ERROR_INVAL_TIMERTYPE;
		}
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;	
}

/**
 * @fn          error_t RSI_TIMERS_SetTimerMode(RSI_TIMERS_T *pTIMER, boolean_t mode,uint8_t timerNum)
 * @brief		    This API is used to set the timer mode 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   mode       : in which mode timer run
 *                               - \ref PERIODIC_TIMER
 *                               - \ref ONESHOT_TIMER
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the timer error code 
 */
STATIC INLINE error_t RSI_TIMERS_SetTimerMode(RSI_TIMERS_T *pTIMER, boolean_t mode,uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		if((mode == PERIODIC_TIMER)||
			 (mode == ONESHOT_TIMER))		
		{
			pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_MODE = mode;
		}
		else
		{
			return ERROR_INVAL_TIMER_MODE;
		}
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
	return RSI_OK;		
}

/**
 * @fn          uint32_t RSI_TIMERS_GetTimerType(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
 * @brief		    This API is used to get the type of timer 
 * @param[in]   pTIMER     : Pointer to the TIMERS instance register area
 * @param[in]   timerNum   : Timer number(0 to 3)                          
 * @return 		  return the type of timer if valid timer else error code 
 */
STATIC INLINE uint32_t RSI_TIMERS_GetTimerType(RSI_TIMERS_T *pTIMER, uint8_t timerNum)
{
	if(timerNum <= TIMER_3)
	{
		return(pTIMER ->MATCH_CTRL[timerNum].MCUULP_TMR_CNTRL_b.TMR_TYPE);					
  }
	else
	{
		return ERROR_INVAL_TIMER_NUM;
	}
} 

/*************************************************************************************
*								              TIMERS FUNCTION PROTOTYPES											  
*************************************************************************************/
 
RSI_DRIVER_VERSION RSI_TIMERS_GetVersion(void);

RSI_TIMERS_CAPABILITIES_T RSI_TIMERS_GetCapabilities(void);

error_t timers_microsec_timer_config(RSI_TIMERS_T *pTIMER, uint8_t timerNum,
																				uint16_t integer,uint8_t fractional,
																				uint8_t mode);
uint32_t timers_read_timer(RSI_TIMERS_T *pTIMER, uint8_t timerNum,boolean_t countDir);

#ifdef __cplusplus
}
#endif	

#endif /* __RSI_TIMERS_H__*/
