/*
 * Amazon FreeRTOS Wi-Fi for RS14100 EVK V1.0.0
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
 * @file aws_wifi.c
 * @brief Wi-Fi Interface.
 */

/* Socket and Wi-Fi interface includes. */
#include "FreeRTOS.h"
#include "aws_wifi.h"

/* Wi-Fi configuration includes. */
#include "aws_wifi_config.h"

/**
 * Include files
 * * */
//! include file to refer data types
#include "rsi_data_types.h"

//! COMMON include file to refer wlan APIs
#include "rsi_common_apis.h"

//! WLAN include file to refer wlan APIs
#include "rsi_wlan_apis.h"

//! socket include file to refer socket APIs
#include "rsi_socket.h"

#include "rsi_bootup_config.h"

//! Error include files
#include "rsi_error.h"

#include "rsi_wlan.h"

#include "rsi_wlan_apis.h"
#include "rsi_driver.h"
#include "rsi_wlan_non_rom.h"
#include "rsi_error.h"
#include "rsi_board.h"
#include "USART.h"
#include "RTE_Device.h"

/*Configuration parameters for AP*/
//! Access point SSID to be created
#define SSID              	 "REDPINE_AP"

//! Channel number
#define CHANNEL_NO           11

//! Encryption type
#define ENCRYPTION_TYPE   	 RSI_CCMP

//! Beacon interval
#define  BEACON_INTERVAL     100

//! DTIM interval
#define DTIM_INTERVAL        4

//! IP address of the module
//! E.g: 0x650AA8C0 == 192.168.10.101
#define DEVICE_IP           0x010AA8C0

//! IP address of Gateway
//! E.g: 0x010AA8C0 == 192.168.10.1
#define GATEWAY            0x010AA8C0

//! IP address of netmask
//! E.g: 0x00FFFFFF == 255.255.255.0
#define NETMASK           0x00FFFFFF


/*-----------------------------------------------------------*/
/*Configuration parameters for ping*/
//! IP address of the remote device
//! E.g: 0x650AA8C0 == 192.168.10.101
#define REMOTE_IP         0x812BA8C0
//! ping size
#define PING_SIZE         100
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length);
uint8_t ping_rsp_received;

/* Buffer to hold the SSID name */
char pcSSID[64];
/* Buffer to keep ap response */
uint8_t ipconfig_rsp[18] ={0} ;

/*Structure to keep the Wifi Status*/
typedef struct wifi{
	volatile uint32_t initialized;     /* Wifi Initialization */
	volatile uint32_t  uninitialized;  /* Wifi UnInitialization*/
	volatile uint32_t connected;       /* Wifi Connection */
	volatile uint32_t disconnected;    /* Wifi Disonnection */
	volatile uint8_t oper_mode;        /* Wifi Oper mode*/
	volatile uint8_t scanning_done;    /* Wifi Scanning Status*/
}wifi_status;

wifi_status wlan_status;

/*Get the WiFi Mode*/
uint32_t rsi_GetOperMode()
{
	return rsi_driver_cb->wlan_cb->opermode;
}

/*-----------------------------------------------------------*/
/**
 * @brief Initializes the Wi-Fi module.
 *
 * This function turns on Wi-Fi module,initializes the drivers and must be called
 * before calling any other Wi-Fi API
 *
 * @return @ref eWiFiSuccess if Wi-Fi module was successfully turned on, failure code otherwise.
 */
WIFIReturnCode_t WIFI_On( void )
{
	int32_t Status = RSI_SUCCESS;

	//! WC initialization
	if(!wlan_status.initialized)
	{
		wlan_status.oper_mode = RSI_WLAN_CLIENT_MODE;
		Status = rsi_wireless_init(wlan_status.oper_mode, 0);
		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}
		wlan_status.initialized = 1;
	}

	return eWiFiSuccess;
}

/*-----------------------------------------------------------*/
/**
 * @brief Turns off Wi-Fi.
 *
 * This function turns off the Wi-Fi module. The Wi-Fi peripheral should be put in a
 * low power or off state in this routine.
 *
 * @return @ref eWiFiSuccess if Wi-Fi module was successfully turned off, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Off( void )
{
	int32_t Status = RSI_SUCCESS;
	if(wlan_status.initialized)
	{
		Status = rsi_wireless_deinit();
		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}
		wlan_status.initialized = 0;
	}

	return eWiFiSuccess;
}

/*-----------------------------------------------------------*/
/**
 * @brief Connects to the Wi-Fi Access Point (AP) specified in the input.
 *
 * The Wi-Fi should stay connected when the same Access Point it is currently connected to
 * is specified. Otherwise, the Wi-Fi should disconnect and connect to the new Access Point
 * specified. If the new Access Point specifed has invalid parameters, then the Wi-Fi should be
 * disconnected.
 *
 * @param[in] pxNetworkParams Configuration to join AP.
 *
 * @return @ref eWiFiSuccess if connection is successful, failure code otherwise.
*/
WIFIReturnCode_t WIFI_ConnectAP( const WIFINetworkParams_t * const pxNetworkParams )
{
	int32_t Status = RSI_SUCCESS,password;

	configASSERT( pxNetworkParams != NULL );
	configASSERT( pxNetworkParams->pcSSID != NULL );
	/* Stay Connected when the same Access Point it is currently connected to is specified */
	if(wlan_status.connected && (memcmp(pcSSID, pxNetworkParams->pcSSID, pxNetworkParams->ucSSIDLength)))
	{
		WIFI_Disconnect();
	}

	if(!wlan_status.scanning_done)
	{
		/* Scan for Access points */
		Status = rsi_wlan_scan((int8_t *)pxNetworkParams->pcSSID, (uint8_t)pxNetworkParams->cChannel, NULL, 0);

		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}
		wlan_status.scanning_done = 1;

	}
	if(!wlan_status.connected)
	{
		/* Connect to an Access point */
		Status = rsi_wlan_connect((int8_t *)pxNetworkParams->pcSSID,pxNetworkParams->xSecurity,pxNetworkParams->pcPassword);

		/* configure IP */
		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}
		Status = rsi_config_ipaddress(RSI_IP_VERSION_4, RSI_DHCP, 0, 0, 0, ipconfig_rsp, sizeof(ipconfig_rsp), 0);
		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}

		/* copy the credintials */
		memcpy(pcSSID, pxNetworkParams->pcSSID, pxNetworkParams->ucSSIDLength);

		wlan_status.connected = 1;
	}

	return eWiFiSuccess;
}

/*-----------------------------------------------------------*/
/**
 * @brief Disconnects from the currently connected Access Point.
 *
 * @return @ref eWiFiSuccess if disconnection was successful or if the device is already
 * disconnected, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Disconnect( void )
{
	int32_t     Status       = RSI_SUCCESS;
	if(wlan_status.connected)
	{
		Status = rsi_wlan_disconnect();
		if(Status != RSI_SUCCESS)
		{
			return eWiFiFailure;
		}
		wlan_status.connected = 0;

		wlan_status.scanning_done = 0;
	}

	return eWiFiSuccess;
}

/*-----------------------------------------------------------*/
/**
 * @brief Resets the Wi-Fi Module.
 *
 * @return @ref eWiFiSuccess if Wi-Fi module was successfully reset, failure code otherwise.
 */
WIFIReturnCode_t WIFI_Reset( void )
{
	WIFIReturnCode_t Status = eWiFiSuccess;

	Status = WIFI_Off();

	if( Status != eWiFiSuccess )
	{
		return Status;
	}

	Status = WIFI_On();

	if( Status != eWiFiSuccess )
	{

		return Status;
	}

	return Status;
}

/*-----------------------------------------------------------*/
/**
 * @brief Perform a Wi-Fi network Scan.
 *
 * @param[in] pxBuffer - Buffer for scan results.
 * @param[in] ucNumNetworks - Number of networks to retrieve in scan result.
 *
 * @return @ref eWiFiSuccess if the Wi-Fi network scan was successful, failure code otherwise.
 *
 * @note The input buffer will have the results of the scan.
 *
*/
WIFIReturnCode_t WIFI_Scan( WIFIScanResult_t * pxBuffer,
		uint8_t ucNumNetworks )
{
	int32_t Status = RSI_SUCCESS,i = 0;
	rsi_rsp_scan_t result;

	if( ( pxBuffer == NULL ) || ( ucNumNetworks == 0 ) )
	{
		return eWiFiFailure;
	}
	//! Scan for Access points
	Status = rsi_wlan_scan(0, 0, &result, (uint32_t)sizeof(result)) ;
	if(Status != RSI_SUCCESS)
	{
		return eWiFiFailure;
	}

	/* Fill scan result in buffer*/
	do
	{
		memcpy( ( uint8_t * ) pxBuffer[ i ].cSSID, result.scan_info[i].ssid, sizeof(result.scan_info[i].ssid) );

		memcpy( pxBuffer[ i ].ucBSSID, result.scan_info[i].bssid, wificonfigMAX_BSSID_LEN );
		pxBuffer[ i ].xSecurity = result.scan_info[i].security_mode;
		pxBuffer[ i ].cRSSI = result.scan_info[i].rssi_val;
		pxBuffer[ i ].cChannel = ( int8_t ) result.scan_info[i].rf_channel;
		pxBuffer[ i ].ucHidden = 0;
	}while(++i < ucNumNetworks);

	return eWiFiSuccess ;
}

/*-----------------------------------------------------------*/
/**
 * @brief Sets the Wi-Fi mode.
 *
 * @param[in] xDeviceMode - Mode of the device Station / Access Point /P2P.
 * @return @ref eWiFiSuccess if Wi-Fi mode was successfully retrieved, failure code otherwise.
 */

WIFIReturnCode_t WIFI_SetMode( WIFIDeviceMode_t xDeviceMode )
{
	int32_t Status = RSI_SUCCESS;

	switch( xDeviceMode )
	{
		case eWiFiModeStation:
			wlan_status.oper_mode = RSI_WLAN_CLIENT_MODE;
			Status = eWiFiSuccess;
			break;

		case eWiFiModeAP:
			wlan_status.oper_mode = RSI_WLAN_ACCESS_POINT_MODE ;
			Status = eWiFiSuccess;
			break;

		case eWiFiModeP2P:
			Status = eWiFiNotSupported;
			break;

		default:
			/* Undefined mode */
			Status = eWiFiNotSupported;
			break;
	}

	return Status;
}
/*-----------------------------------------------------------*/

/**
 * @brief Gets the Wi-Fi mode.
 *
 * param[in] pxDeviceMode - return mode Station / Access Point /P2P
 * @return eWiFiSuccess if everything succeeds.
 */
WIFIReturnCode_t WIFI_GetMode( WIFIDeviceMode_t * pxDeviceMode )
{
	configASSERT( pxDeviceMode != NULL );
	/*get the WiFi mode*/
	*pxDeviceMode = rsi_GetOperMode();
	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkAdd( const WIFINetworkProfile_t * const pxNetworkProfile,
		uint16_t * pusIndex )
{
	/*Check input parameters*/
	if( ( pxNetworkProfile == NULL ) || ( pusIndex == NULL ) )
	{
		return eWiFiFailure;
	}

	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkGet( WIFINetworkProfile_t * pxNetworkProfile,
		uint16_t usIndex )
{
	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_NetworkDelete( uint16_t usIndex )
{
	/* FIX ME. */
	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

/**
 * @brief Ping an Ip address In the Network.
 *
 * @param[in] IP Address to ping.
 * @param[in] No of times to ping.
 * @param[in] Interval in milliseconds for ping operation.
 * @return eWiFiSuccess if everything succeeds.
 */
WIFIReturnCode_t WIFI_Ping( uint8_t * pucIPAddr,

		uint16_t usCount,
		uint32_t ulIntervalMS )
{
	int32_t Status = RSI_SUCCESS;
	int32_t packet_count = 0;
	uint32_t remote_ip_addr = REMOTE_IP;
	uint16_t size = PING_SIZE;
	if( ( NULL == pucIPAddr ) || ( 0 == usCount ) )
	{
		return eWiFiFailure;
	}

	Status = rsi_wlan_ping_async( 0, (uint8_t *)&remote_ip_addr, size, rsi_ping_response_handler);
	if(Status != RSI_SUCCESS)
	{
		return eWiFiFailure;
	}
	/* FIX ME. */
	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

/**
 * @brief Retrieves the Wi-Fi interface's IP address.
 *
 * @param[in] IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds.
 */
WIFIReturnCode_t WIFI_GetIP( uint8_t * pucIPAddr )
{
	configASSERT( pucIPAddr != NULL );
	/*copy the ip address*/
	memcpy( pucIPAddr, (ipconfig_rsp+6), 4 );

	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

/**
 * @brief Retrieves the Wi-Fi interface's MAC address.
 *
 * @param[in] MAC Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds.
 */
WIFIReturnCode_t WIFI_GetMAC( uint8_t * pucMac )
{
	/* FIX ME. */
	configASSERT( pucMac != NULL );

	rsi_wlan_get( RSI_MAC_ADDRESS, pucMac, 6 );

	return eWiFiSuccess;
}

/*-----------------------------------------------------------*/
/**
 * @brief Retrieves host IP address from URL using DNS
 *
 * @param[in] pxHost - Host URL.
 * @param[in] pxIPAddr - IP Address buffer.
 *
 * @return eWiFiSuccess if everything succeeds, failure code otherwise.
 */
WIFIReturnCode_t WIFI_GetHostIP( char * pcHost,
		uint8_t * pucIPAddr )
{
	int32_t Status = RSI_SUCCESS;
	rsi_rsp_dns_query_t dns_query_rsp;

	configASSERT( pcHost != NULL );
	configASSERT( pucIPAddr != NULL );

	do{

		Status = rsi_dns_req( RSI_IP_VERSION_4, (uint8_t *)pcHost, NULL, NULL, &dns_query_rsp, sizeof(dns_query_rsp));

		if( Status != RSI_SUCCESS )
			break;
		else
		{
			pucIPAddr[0] = dns_query_rsp.ip_address[0].ipv4_address[0];
			pucIPAddr[1] = dns_query_rsp.ip_address[0].ipv4_address[1];
			pucIPAddr[2] = dns_query_rsp.ip_address[0].ipv4_address[2];
			pucIPAddr[3] = dns_query_rsp.ip_address[0].ipv4_address[3];
			break;
		}

	}while(1);

	if( Status == RSI_SUCCESS )
	{
		return eWiFiSuccess;
	}
	else
	{
		return Status;
	}
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StartAP( void )
{
	/* FIX ME. */
	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_StopAP( void )
{
	/* FIX ME. */
	return eWiFiNotSupported;
}

/*-----------------------------------------------------------*/

/**
 * @brief Configure SoftAP.
 *
 * @param[in] pxNetworkParams - Network parameters to configure AP.
 *
 * @return @ref eWiFiSuccess if SoftAP was successfully configured, failure code otherwise.
 */
WIFIReturnCode_t WIFI_ConfigureAP( const WIFINetworkParams_t * const pxNetworkParams )
{
	WIFIReturnCode_t Status = eWiFiFailure;

	uint32_t ip_addr = DEVICE_IP;
	uint32_t network_mask = NETMASK;
	uint32_t gateway = GATEWAY;

	/*Check for WiFi Initialization*/
	if(wlan_status.initialized)
	{
		WIFI_Off();
		return Status;
	}

	if(( pxNetworkParams == NULL ) || ( pxNetworkParams->pcSSID == NULL ) || ( pxNetworkParams->pcPassword == NULL))
		return eWiFiFailure;

	/* Set Wifi Mode*/
	Status = WIFI_SetMode( eWiFiModeAP );

	if( Status != RSI_SUCCESS )
	{
		return Status;
	}

	Status = rsi_wireless_init(wlan_status.oper_mode, 0);

	if( Status != RSI_SUCCESS )
	{
	  return Status;
	}

	//! Configure IP
	Status = rsi_config_ipaddress( RSI_IP_VERSION_4, RSI_STATIC,(uint8_t *)&ip_addr,(uint8_t *)&network_mask, (uint8_t *)&gateway, NULL, 0, 0);
	if(Status != RSI_SUCCESS)
	{
		return Status;
	}

	//! Start Access point
	Status = rsi_wlan_ap_start((int8_t *)pxNetworkParams->pcSSID, pxNetworkParams->cChannel, pxNetworkParams->xSecurity, ENCRYPTION_TYPE, (uint8_t *)pxNetworkParams->pcPassword, BEACON_INTERVAL, DTIM_INTERVAL );

	if(Status != RSI_SUCCESS)
	{
		return Status;
	}

	return eWiFiSuccess;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_SetPMMode( WIFIPMMode_t xPMModeType,
		const void * pvOptionValue )
{
	/* FIX ME. */
	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

WIFIReturnCode_t WIFI_GetPMMode( WIFIPMMode_t * pxPMModeType,
		void * pvOptionValue )
{
	/* FIX ME. */
	return eWiFiNotSupported;
}
/*-----------------------------------------------------------*/

BaseType_t WIFI_IsConnected(void)
{
	if(wlan_status.connected)
		return pdTRUE;
	else
		return pdFALSE;
}

WIFIReturnCode_t WIFI_RegisterNetworkStateChangeEventCallback( IotNetworkStateChangeEventCallback_t Callback )
{
	return eWiFiNotSupported;
}

//! ping response notify call back handler
void rsi_ping_response_handler(uint16_t status, const uint8_t *buffer, const uint16_t length)
{
	ping_rsp_received = 1;
}

