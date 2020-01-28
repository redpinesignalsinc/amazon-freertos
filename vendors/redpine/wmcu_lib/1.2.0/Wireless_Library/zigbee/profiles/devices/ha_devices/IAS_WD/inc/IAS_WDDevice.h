/******************************************************************************
* ThermostatDevice.h
******************************************************************************
* Program Description:
* This file contains the Thermostat Device
******************************************************************************/

#ifndef _IAS_WDDEVICE_H_
#define _IAS_WDDEVICE_H_

#include "ZCL_Foundation.h"

/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/
extern uint8_t allocatedbuffer[24];
extern uint8_t freebuffer[24][2];

uint8_t g_Application_Identify_Timer_One_c;
/*-----------------------------------------------------------------------------
* Memory Declarations
*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
* Function Prototypes
*----------------------------------------------------------------------------*/

void Handle_ZCL_Message( uint8_t buffer_index, APSDE_Data_Indication_t *p_buffer );


#endif /* _THERMOSTAT_H_*/

/*-----------------------------------------------------------------------------
* End Of File
*----------------------------------------------------------------------------*/
