/**
 * @file       rsi_time_period.c
 * @version    0.1
 * @date       20 jan 2016
 *
 * Copyright(C) Redpine Signals 2016
 * All rights reserved by Redpine Signals.
 *
 * @section License
 * This program should be used on your own responsibility.
 * Redpine Signals assumes no responsibility for any losses
 * incurred by customers or third parties arising from the use of this file.
 *
 * @brief This files contains
 *
 * @section Description
 *
 */


/**
 * Includes
 */

#include "rsi_chip.h"

/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 * \defgroup RSI_TIME_PERIOD  RSI:RS1xxxx TIME PERIOD 
 *  @{
 *
 */

/**
 * @brief    This API is calibrate the RC
 *@param[in] pstcTimePeriod  is pointer to the timperiod calibration registration instance
 *@param[in] u32TimePeriodRefClk  is timeperiod of reference clk with each bit corresponding to 10ps granularity
 *@param[in] u32XtalSettle is no of 32khz clocks for xtal 40mhz clk to settle
 *@param[in] u16RcClkCnt is 2^no_of_rc_clocks = no of rc clocks used in calibration
 *@param[in] bPeriodicCalibEn is to enable the periodic calibration
 *           1 : periodic calibration mode enable
 *           0 : periodic calibration mode disable
 *@param[in] u8PeriodicCalibRate is periodic calibration rate
 *           0 : Every 30 secs
 *           1 : every 15 secs,
 *           2 : every 10 secs,
 *           3 : every 5  secs
 *@param[in] bTemperatureCalibEn is to enable the temperature based calibration
 *           1 : Temperature based calibration enable
 *           0 : Temperature based calibration disable
 *@param[in] u8TemperatureVal is maximum temperature change after which rc calibration must be trigger
 *           default 5 : 5 Degree C.
 *@param[in] u8AverageFactor is average factor
 *@return   returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_TIMEPERIOD_RCCalibration
(
		TimePeriodCalib_t *pstcTimePeriod ,
		uint32_t           u32TimePeriodRefClk     ,
		uint32_t           u32XtalSettle           ,
		uint16_t           u16RcClkCnt             ,
		boolean_t          bPeriodicCalibEn        ,
		uint8_t            u8PeriodicCalibRate     ,
		boolean_t          bTemperatureCalibEn     ,
		uint8_t            u8TemperatureVal        ,
		uint8_t            u8AverageFactor
)
{
	/*Parameter validation*/
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS;
	}
	/*Refernce clock time period*/
	pstcTimePeriod->MCU_CAL_REF_CLK_TIEMPERIOD_REG_b.TIMEPERIOD_REF_CLK = u32TimePeriodRefClk ;
	/*Clock settling time configuration */
	pstcTimePeriod->MCU_CAL_REF_CLK_SETTLE_REG_b.XTAL_SETTLE            = u32XtalSettle;
	/*RC Calib MUX select*/
	pstcTimePeriod->MCU_CAL_START_REG_b.RC_XTAL_MUX_SEL                 = 0 ;
	/*Enable the RC calib*/
	pstcTimePeriod->MCU_CAL_START_REG_b.START_CALIB_RC                  = 1 ;
	/*average factor */
	pstcTimePeriod->MCU_CAL_START_REG_b.ALPHA_RC                        = u8AverageFactor;
	/*Number of clock required for calibration */
	pstcTimePeriod->MCU_CAL_START_REG_b.NO_OF_RC_CLKS                   = u16RcClkCnt;

	/*Periodic calibration is selected */
	if(bPeriodicCalibEn)
	{
		/*Periodic calibration enable*/
		pstcTimePeriod->MCU_CAL_START_REG_b.PERIODIC_RC_CALIB_EN =1;
		/*Update periodic rate at which calibration has to happen */
		pstcTimePeriod->MCU_CAL_START_REG_b.RC_TRIGGER_TIME_SEL = u8PeriodicCalibRate;
	}

	/*Periodic calibration is selected */
	if(bTemperatureCalibEn)
	{
		/*Enable tempearature based calibrarion */
		pstcTimePeriod->MCU_CAL_TEMP_PROG_REG_b.PERIODIC_TEMP_CALIB_EN = 1;
		/*Update at which temperature calibration has to happen*/
		pstcTimePeriod->MCU_CAL_TEMP_PROG_REG_b.MAX_TEMP_CHANGE = u8TemperatureVal;
	}

	return RSI_OK;
}

/**
 * @brief    This API is used to read calibrated timeperiod of RC
 * @param[in] pstcTimePeriod is pointer to the timperiod calibration registration instance
 * @return   Returns the time period on success
 *
 * */
uint32_t RSI_TIMEPERIOD_RCCalibTimePeriodRead(TimePeriodCalib_t *pstcTimePeriod)
{
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}
	/*Wait for valid bit set */
	while(pstcTimePeriod->MCU_CAL_REF_CLK_SETTLE_REG_b.VALID_RC_TIMEPERIOD != 1);
	/*Return the time period value */
	return (pstcTimePeriod->MCU_CAL_RC_TIMEPERIOD_READ_b.TIMEPERIOD_RC);
}

/**
 * @brief     This API is used to read calibrated timeperiod of RO
 * @param[in]  pstcTimePeriod is pointer to the timperiod calibration registration instance
 * @return    Returns the time period on success
 */
uint32_t RSI_TIMEPERIOD_ROCalibTimePeriodRead(TimePeriodCalib_t *pstcTimePeriod)
{
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}
	/*Wait for valid bit set */
	while(pstcTimePeriod->MCU_CAL_REF_CLK_SETTLE_REG_b.VALID_RO_TIMEPERIOD != 1);
	/*Return the time period value */
	return (pstcTimePeriod->MCU_CAL_RO_TIMEPERIOD_READ_b.TIMEPERIOD_RO);
}

/**
 * @brief     This API is calibrate the External 32KHz oscilator
 *@param[in]  pstcTimePeriod is pointer to the timperiod calibration registration instance
 *@param[in]  u32TimePeriodRefClk is timeperiod of reference clk with each bit corresponding to 10ps granularity
 *@param[in]  u32XtalSettle is no of 32khz clocks for xtal 40mhz clk to settle
 *@param[in]  u16RcClkCnt is 2^no_of_rc_clocks = no of rc clocks used in calibration
 *@param[in]  bPeriodicCalibEn is to enable the periodic calibration
 *           1 : periodic calibration mode enable
 *           0 : periodic calibration mode disable
 *@param[in]  u8PeriodicCalibRate is periodic calibration rate
 *           0 : Every 30secs
 *           1 : every 15 secs,
 *           2 : every 10 secs,
 *           3 : every 5 secs
 *@param[in] bTemperatureCalibEn is to enable the temperature based calibration
 *           1 : Temperature based calibration enable
 *           0 : Temperature based calibration disable
 *@param[in]  u8TemperatureVal is maximum temperature change after which rc calibration must be trigger
 *           default 5 : 5 Degree C.
 *@param[in]  u8AverageFactor is average factor
 *@return    returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_TIMEPERIOD_XTAL32KHzCalibration
(
		TimePeriodCalib_t *pstcTimePeriod,
		uint32_t           u32TimePeriodRefClk     ,
		uint32_t           u32XtalSettle           ,
		uint16_t           u16RcClkCnt             ,
		boolean_t          bPeriodicCalibEn        ,
		uint8_t            u8PeriodicCalibRate     ,
		boolean_t          bTemperatureCalibEn     ,
		uint8_t            u8TemperatureVal        ,
		uint8_t            u8AverageFactor
)

{
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}
	pstcTimePeriod->MCU_CAL_REF_CLK_TIEMPERIOD_REG_b.TIMEPERIOD_REF_CLK = u32TimePeriodRefClk;
	pstcTimePeriod->MCU_CAL_START_REG_b.ALPHA_RC = u8AverageFactor ;
	pstcTimePeriod->MCU_CAL_START_REG_b.NO_OF_RC_CLKS = u16RcClkCnt ;
	pstcTimePeriod->MCU_CAL_START_REG_b.RC_XTAL_MUX_SEL =1;
	pstcTimePeriod->MCU_CAL_START_REG_b.START_CALIB_RC = 1;

	/*Periodic calibration is selected */
	if(bPeriodicCalibEn)
	{
		/*Periodic calibration enable*/
		pstcTimePeriod->MCU_CAL_START_REG_b.PERIODIC_RC_CALIB_EN =1;
		/*Update periodic rate at which calibration has to happen */
		pstcTimePeriod->MCU_CAL_START_REG_b.RC_TRIGGER_TIME_SEL = u8PeriodicCalibRate;
	}
	/*Periodic calibration is selected */
	if(bTemperatureCalibEn)
	{
		/*Enable tempearature based calibrarion */
		pstcTimePeriod->MCU_CAL_TEMP_PROG_REG_b.PERIODIC_TEMP_CALIB_EN = 1;
		/*Update at which temperature calibration has to happen*/
		pstcTimePeriod->MCU_CAL_TEMP_PROG_REG_b.MAX_TEMP_CHANGE = u8TemperatureVal;
	}

	return RSI_OK;
}

/**
 * @brief     This API is calibrate the RO
 *@param[in]  pstcTimePeriod is pointer to the timperiod calibration registration instance
 *@param[in]  u8RefClkSrc is clock source selection
 *            0 - RC clock calibration happens
 *            1 - XTAL 32khz clock timeperiod calibration occurs with reference clock as 40mhz xtal
 *            This should not be changed in the middle of process. Must be changed only once.
 *@param[in]  u32XtalSettle is no of clocks of RO for the RC clk to settle when enabled
 *@param[in]  u16RoClkCnt is 2^no_of_ro_clks no of clocks of ro clock counts for no of rc clocks in that time to measure timeperiod
 *@param[in]  bPeriodicCalibEn is to enable the periodic calibration
 *           1 : periodic calibration mode enable
 *           0 : periodic calibration mode disable
 *@param[in]  u8PeriodicCalibRate is periodic calibration duration
 *           3 : 8 times in a second
 *           2 : 4 times in a second
 *           1 : 2 times in a second
 *           0 : 1 time in a second
 *@param[in]  u8AverageFactor is average factor
 *
 *@return    returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_TIMEPERIOD_ROCalibration
(
		TimePeriodCalib_t *pstcTimePeriod ,
		uint8_t            u8RefClkSrc             ,
		uint32_t           u32XtalSettle           ,
		uint16_t           u16RoClkCnt             ,
		boolean_t          bPeriodicCalibEn        ,
		uint8_t            u8PeriodicCalibRate     ,
		uint8_t            u8AverageFactor

)
{
	uint32_t u32ValidTimePeriodRc = 0 ;

	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}

	/*Check RC calibration is done or not */
	u32ValidTimePeriodRc = RSI_TIMEPERIOD_RCCalibTimePeriodRead(pstcTimePeriod);

	if(!(u32ValidTimePeriodRc))
	{
		/*If RC calibration is not done before return error*/
		/*NOTE : Before calibrating RO atleast RC should be calibrated once */
		return ERROR_TIME_PERIOD_RC_CALIB_NOT_DONE;
	}

	/*Xtal 32Khz clock is used for calibration */
	if(u8RefClkSrc)
	{
		pstcTimePeriod->MCU_CAL_START_REG_b.RC_XTAL_MUX_SEL = 1;
	}
	/*RC 32Khz clock is used for calibration */
	else
	{
		pstcTimePeriod->MCU_CAL_START_REG_b.RC_XTAL_MUX_SEL = 0;
	}
	pstcTimePeriod->MCU_CAL_START_REG_b.NO_OF_RO_CLKS =u16RoClkCnt ;
	pstcTimePeriod->MCU_CAL_START_REG_b.START_CALIB_RO = 1;
	pstcTimePeriod->MCU_CAL_START_REG_b.ALPHA_RO = u8AverageFactor ;
	pstcTimePeriod->MCU_CAL_START_REG_b.RC_SETTLE_TIME = u32XtalSettle ;
	/*Periodic calibration is selected */
	if(bPeriodicCalibEn)
	{
		/*Periodic calibration enable*/
		pstcTimePeriod->MCU_CAL_START_REG_b.PERIODIC_RO_CALIB_EN =1;
		/*Update periodic rate at which calibration has to happen */
		pstcTimePeriod->MCU_CAL_START_REG_b.RO_TRIGGER_TIME_SEL = u8PeriodicCalibRate;
	}
	else
	{
		/*Periodic calibration enable*/
		pstcTimePeriod->MCU_CAL_START_REG_b.PERIODIC_RO_CALIB_EN =0;
		/*Update periodic rate at which calibration has to happen */
		//FIXME :  this can be default value if periodic mode is not used
		//pstcTimePeriod->MCU_CAL_START_REG_b.RO_TRIGGER_TIME_SEL = 0x00;
	}
	return RSI_OK;
}


/**
 * @brief     This API is select the RTC clock
 * @param[in] pstcTimePeriod is pointer to the timperiod calibration registration instance
 * @param[in] u32TimePeriod is RTC time period in pico seconds as 32Khz as reference
 * @return    returns 0 on success
 *			 Error code on failure
 */
error_t RSI_TIMEPERIOD_TimerClkSel
(
		TimePeriodCalib_t *pstcTimePeriod,
		uint32_t  u32TimePeriod
)
{
	/*Check for the NULL parameter*/
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}

	/*Update the timer period */
	pstcTimePeriod->MCU_CAL_TIMER_CLOCK_PERIOD_b.RTC_TIMER_CLK_PERIOD     = u32TimePeriod ;

	/*Indicated SOC programmed rtc_timer clock period is applied at KHz clock domain*/
	while(!pstcTimePeriod->MCU_CAL_TIMER_CLOCK_PERIOD_b.SPI_RTC_TIMER_CLK_PERIOD_APPLIED_b);

	return RSI_OK;
}


/**
 * @brief     This API is select the low power trigger
 * @param[in]  pstcTimePeriod is pointer to the timperiod calibration registration instance
 * @param[in]  bEn 1: seperate counter runs based 2^15 clocks of 32KHz clock = 1sec
 *               0: rtc runs and triggers are generated based on rtc
 * @return    returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_TIMEPERIOD_LowPwrTrigSelEn(TimePeriodCalib_t *pstcTimePeriod , boolean_t bEn)
{
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}
	pstcTimePeriod->MCU_CAL_START_REG_b.LOW_POWER_TRIGGER_SEL = bEn;
	return RSI_OK;
}

/**
 *@brief     This API is select the vbat trigger selection
 *@param[in]  pstcTimePeriod is pointer to the timperiod calibration registration instance
 *@param[in]  u8Time i
 *           0 : Every minute
 *           1 : Every 30secs
 *           2 : every 15 secs,
 *           3 : every 10 secs,
 *           4 : every 5 secs
 *           5 : every second
 *@return   returns 0 on success
 *			 Error code on failure
 * */
error_t RSI_TIMEPERIOD_VbatTrigSel(TimePeriodCalib_t *pstcTimePeriod , uint8_t u8Time)
{
	if(pstcTimePeriod == NULL)
	{
		return ERROR_TIME_PERIOD_PARAMETERS ;
	}
	pstcTimePeriod->MCU_CAL_START_REG_b.VBATT_TRIGGER_TIME_SEL = u8Time;
	return RSI_OK;
}

/*
 * @}
 */
/*End of file not truncated */
