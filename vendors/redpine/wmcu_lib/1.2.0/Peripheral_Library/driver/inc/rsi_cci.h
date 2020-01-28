#include "RS1xxxx.h"
#include "rsi_error.h"

#ifndef __RSI_CCI_H__
#define __RSI_CCI_H__

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************
 * *                      Macros
 * ******************************************************/

#define AMS_EN       *(volatile uint32_t *)(0x46008000+0x14)

#define CCI_AHB_SLAVE0_ADDRESS   0x80000000
#define CCI_AHB_SLAVE1_ADDRESS   0x60000000


#define CCI_LSB_ADDRESS_S0  0x00000000
#define CCI_MSB_ADDRESS_S0  0x0001FFFF

#define CCI_LSB_ADDRESS_S1  0x00000000
#define CCI_MSB_ADDRESS_S1  0x00000004 //0x0001FFFF FIXME: Out of RANGE INDEX COMPARISION


#define CCI_LSB_ADDRESS_S2  0x00000000
#define CCI_MSB_ADDRESS_S2  0x00000004 //0x0001FFFF FIXME: Out of RANGE INDEX COMPARISION

#define CCI_LSB_ADDRESS_S3  0x00000000
#define CCI_MSB_ADDRESS_S3  0x00000004 //0x0001FFFF FIXME: Out of RANGE INDEX COMPARISION


/******************************************************
 * *                    Constants
 * ******************************************************/

/******************************************************
 * *                   Enumerations
 * ******************************************************/



/******************************************************
 * *                 Type Definitions
 * ******************************************************/




/******************************************************
 * *                    Structures
 * ******************************************************/

/** 
 *\cond HIDDEN_SYMBOLS
 */
/*!< CCI configuration structure*/
typedef struct RSI_CCI_Init_s
{
  
 uint8_t slave_enable ;                  /*!< number of CCI Slaves to be supported
                                               1,2,3  */
 uint8_t early_bus_termination;          /*!< Support early bus termination
                                               0 - disable
                                               1 - enable */ 

 uint8_t address_width_config;           /*!< Address width configuration
                                                b'11 �> 40 bit width (32 bit address and 8 bit command)
																								b'10 �> 32 bit width (24 bit address and 8 bit command)
																								b'01 �> 24 bit width (16 bit address and 8 bit command)
																								b'00 �> 16 bit width (8 bit address and 8 bit command)
                                                */ 
 uint8_t translation_enable;            /*!< Enable/Disable translation
                                                0 -  Disable
                                                1 -  Enable */
 uint32_t translation_address ;         /*load translation address */
  
 uint8_t mode;                          /*!< Mode of the interface 
                                                0 - SDR mode

                                                1 - DDR mode  */
																						
																					
 uint8_t prog_calib;
 uint8_t interface_width;                /*!<Width of the interface 
                                               0 - quad mode
                                               1 - octa mode  
                                               2 - Word mode */
 uint8_t slave_priority;                 /*!< This bits will represents priority of the slaves
                                               1 : slave 0 has highest priority 
                                               4 : slave 1 has highest priority 
                                               8 : slave 2 has highest priority*/

 uint16_t slave_timeout;                  /*!< configurable time out value for response*/


 uint32_t slave_lsb_address[3];              /*!< Slave lower and higher address 
                                            range for the each slave by programming*/
																						
uint32_t slave_msb_address[3];              /*!< Slave lower and higher address 
                                            range for the each slave by programming*/
 uint8_t  cci_cntrl_en ;
}RSI_CCI_Init_t;

/**
 * \endcond
 */

/******************************************************
 * *                 Global Variables
 * ******************************************************/
/******************************************************
 * *               Function Declarations
 * ******************************************************/


/**
 * \ingroup   RSI_SPECIFIC_DRIVERS
 */

/**
 * \defgroup RSI_CCI_DRIVER RSI:RS1xxxx CCI 
 *  @{
 *
 */
void RSI_CCI_AmsEnable(void);

error_t RSI_CCI_AMS_Initialise(RSI_CCI_Init_t *p_cci_config);

uint32_t RSI_CCI_SetFifoThreshlod(volatile CCI_Type *pstcCCI,uint8_t val);   

uint32_t RSI_CCI_PrefetchEnable(volatile CCI_Type *pstcCCI);

uint32_t RSI_CCI_MessageInterruptEnable(volatile CCI_Type *pstcCCI);

void RSI_CCI_CalibMode(volatile CCI_Type *pstcCCI);

void RSI_CCI_LowPowerMode(volatile CCI_Type *pstcCCI);

uint32_t RSI_CCI_IntStat(volatile CCI_Type *pstcCCI);

uint32_t RSI_CCI_IntClear(volatile CCI_Type *pstcCCI,uint8_t interrupt);

uint32_t RSI_CCI_SlaveResetMode(volatile CCI_Type *pstcCCI);

#ifdef __cplusplus
}
#endif


#endif // __RSI_CCI_H__

/* @} end of RSI_CCI_DRIVERS */
