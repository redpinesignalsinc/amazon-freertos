/*
 * Amazon FreeRTOS Secure Sockets for RS14100 EVK V1.0.0
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
 * @file aws_secure_sockets.c
 * @brief WiFi and Secure Socket interface implementation.
 */

/* Define _SECURE_SOCKETS_WRAPPER_NOT_REDEFINE to prevent secure sockets functions
 * from redefining in aws_secure_sockets_wrapper_metrics.h */
#define _SECURE_SOCKETS_WRAPPER_NOT_REDEFINE

/* Socket and WiFi interface includes. */
#include "aws_secure_sockets.h"
#include "rsi_socket.h"
#include "rsi_error.h"
#include "aws_tls.h"
#include "task.h"

#undef _SECURE_SOCKETS_WRAPPER_NOT_REDEFINE

/*Max no of sockets supported*/
#define securesocketsMAX_NUM_SOCKETS				5

/**
 * @brief Flag indicating that socket send operations are not permitted.
 *
 * If a WR shutdown in SOCKETS_Shutdown() is invoked, this flag is
 * set in the socket's xShutdownFlags member.
 */
#define securesocketsSOCKET_WRITE_CLOSED_FLAG    	( 1UL << 2 )

/**
 * @brief Flag indicating that socket receive operations are not permitted.
 *
 * If a RD shutdown in SOCKETS_Shutdown() is invoked, this flag is
 * set in the socket's xShutdownFlags member.
 */
#define securesocketsSOCKET_READ_CLOSED_FLAG	    ( 1UL << 1 )

/*Flags related to sockets connection*/
#define securesocketsSOCKET_CONNECTED_FLAG   		1
#define securesocketsSOCKET_HANDSHAKE_DONE_FLAG     1
#define TLS_HANDSHAKE_FAILED_FLAG        			2

/*Timeout to the rsi_select api*/
#define securesocketsONE_MILLISECOND   			(1)

/*Delay used between network select attempts */
#define securesocketsFIVE_MILLISECONDS    (pdMS_TO_TICKS( 5 ))

/* Internal context structure. */
typedef struct SSOCKETContext
{
	Socket_t xSocket;         	 	/* Socket ID Created*/
	char * pcDestination;      	 	/*Network name of TLS  server*/
	void * pvTLSContext;
	BaseType_t xRequireTLS;   	 	/*set TLS functionality required or not*/
	BaseType_t xShutdownFlags;   	/*Set the flags for socket shutdown*/
	uint32_t ulSendTimeout;      	/*Send timeout*/
	uint32_t ulRecvTimeout;      	/*Receive timeout*/
	char * pcServerCertificate;  	/*Points to the certificate used while handshaking*/
	uint32_t ServerCertificateLength;  /*Certificate length*/
	uint32_t State;             	/*Socket state i.e connected or disconnected*/
	uint32_t StateSecure;
} SSOCKETContext_t, * SSOCKETContextPtr_t;

/*-----------------------------------------------------------*/
/*
 * @brief Network send callback.
 */
static BaseType_t prvNetworkSend( void * pvContext,
		const unsigned char * pucData,
		size_t xDataLength )
{
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext;
	uint32_t Status = SOCKETS_ERROR_NONE;
	int32_t Written_bytes = 0;
	/* Do not send data on unconnected socket */
	if( !( pxContext->State & securesocketsSOCKET_CONNECTED_FLAG ) )
	{
		return SOCKETS_SOCKET_ERROR;
	}

	Status = rsi_send(pxContext->xSocket,pucData,xDataLength,0);

	return Status;
}

/*-----------------------------------------------------------*/
/*
 * @brief Network receive callback.
 */
static BaseType_t prvNetworkRecv( void * pvContext,
		unsigned char * pucReceiveBuffer,
		size_t xReceiveLength )
{
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext;
	uint32_t Status = SOCKETS_ERROR_NONE;
	rsi_fd_set read_fds;
	struct rsi_timeval timeout;
	TickType_t xTimeOnEntering = xTaskGetTickCount();

	memset(&read_fds,0,sizeof(rsi_fd_set));
	timeout.tv_sec = 0 ;
	timeout.tv_usec = securesocketsONE_MILLISECOND ;

	rsi_set_fd(pxContext->xSocket,&read_fds);
	/* Do not receive data on unconnected socket */
	if( !( pxContext->State & securesocketsSOCKET_CONNECTED_FLAG ) )
	{
		return SOCKETS_SOCKET_ERROR;
	}

	for(;;)
	{

		Status = rsi_select(pxContext->xSocket + 1, &read_fds,NULL,NULL,&timeout,NULL);

		if((Status > 0 ) && rsi_fd_isset(pxContext->xSocket,&read_fds))
		{
			Status = rsi_recv(pxContext->xSocket,pucReceiveBuffer,xReceiveLength,0);

			if(Status > 0)
			{
				break;
			}
		}
		else
		{
			if( (xTaskGetTickCount() - xTimeOnEntering) < pxContext->ulRecvTimeout)
			{
				vTaskDelay( securesocketsFIVE_MILLISECONDS );
			}
			else
			{
				Status = SOCKETS_ERROR_NONE;
				break;
			}
		}
	}

	return Status;
}
/*-----------------------------------------------------------*/

/**
 * @brief Creates a TCP socket.
 * @param[in] lDomain Must be set to SOCKETS_AF_INET. See @ref SocketDomains.
 * @param[in] lType Set to SOCKETS_SOCK_STREAM to create a TCP socket.
 * @param[in] lProtocol Set to SOCKETS_IPPROTO_TCP to create a TCP socket.
 * @return
 * If a socket is created successfully, then the socket handle is
 * returned
 * * @ref SOCKETS_INVALID_SOCKET is returned if an error occurred.
 */
Socket_t SOCKETS_Socket( int32_t lDomain,
		int32_t lType,
		int32_t lProtocol )
{
	int32_t     Socket;
	int32_t Status = SOCKETS_ERROR_NONE;
	SSOCKETContextPtr_t pxContext = NULL;

	configASSERT( lDomain == SOCKETS_AF_INET );
	configASSERT( ( lType == SOCKETS_SOCK_STREAM && lProtocol == SOCKETS_IPPROTO_TCP ) );

	/* Create a socket */
	Socket = rsi_socket(lDomain, lType, 0);

	if( Socket != SOCKETS_INVALID_SOCKET && Socket != RSI_FAIL)
	{
		/* Allocate the internal context structure. */
		pxContext = pvPortMalloc( sizeof( SSOCKETContext_t ) );
		if( pxContext == NULL )
		{
			Status=rsi_shutdown(Socket, 0);
			pxContext = SOCKETS_INVALID_SOCKET;
		}
		else
		{
			memset( pxContext,0,sizeof(SSOCKETContext_t) );
			pxContext->xSocket = Socket;
			/* Set default timeouts. */
			pxContext->ulRecvTimeout = socketsconfigDEFAULT_RECV_TIMEOUT;
			pxContext->ulSendTimeout = socketsconfigDEFAULT_SEND_TIMEOUT;
		}

	}
	else
	{
		pxContext = SOCKETS_INVALID_SOCKET;
	}

  return pxContext;
}
/*-----------------------------------------------------------*/

Socket_t SOCKETS_Accept( Socket_t xSocket,
		SocketsSockaddr_t * pxAddress,
		Socklen_t * pxAddressLength )
{
	/* FIX ME. */
	return SOCKETS_INVALID_SOCKET;
}
/*-----------------------------------------------------------*/
/**
 * @brief Connects the socket to the specified IP address and port.
 *
 * The socket must first have been successfully created by a call to SOCKETS_Socket().
 *
 * \note To create a secure socket, SOCKETS_SetSockOpt() should be called with the
 * SOCKETS_SO_REQUIRE_TLS option \a before SOCKETS_Connect() is called.
 *
 * If this function returns an error the socket is considered invalid.
 *
 * \warning SOCKETS_Connect() is not safe to be called on the same socket
 * from multiple threads simultaneously with SOCKETS_Connect(),
 * SOCKETS_SetSockOpt(), SOCKETS_Shutdown(), SOCKETS_Close().
 * @param[in] xSocket The handle of the socket to be connected.
 * @param[in] pxAddress A pointer to a SocketsSockaddr_t structure that contains the
 * the address to connect the socket to.
 * @param[in] xAddressLength Should be set to sizeof( @ref SocketsSockaddr_t ).
 *
 * @return
 * * @ref SOCKETS_ERROR_NONE if a connection is established.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t SOCKETS_Connect( Socket_t xSocket,
		SocketsSockaddr_t * pxAddress,
		Socklen_t xAddressLength )
{
	int32_t Status = SOCKETS_ERROR_NONE;
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;
	TLSParams_t TLSParams = { 0 };

	struct rsi_sockaddr_in TempAddress;

	memset(&TempAddress, 0, sizeof(TempAddress));

	if( (pxContext != SOCKETS_INVALID_SOCKET ) && ( pxAddress != NULL) )
	{
		TempAddress.sin_family = AF_INET;
		TempAddress.sin_port = SOCKETS_ntohs(pxAddress->usPort);
		TempAddress.sin_addr.s_addr = pxAddress->ulAddress;

		Status = rsi_connect( (int32_t ) pxContext->xSocket, (struct rsi_sockaddr *) &TempAddress, sizeof(TempAddress));

		if(Status != SOCKETS_ERROR_NONE)
		{
			Status = rsi_wlan_get_status();
			configPRINTF(("ERROR:%d Socket failed to connect.\r\n",Status));
			return SOCKETS_SOCKET_ERROR;
		}
		pxContext->State |= securesocketsSOCKET_CONNECTED_FLAG;

		if( (Status == SOCKETS_ERROR_NONE) && ( pxContext->xRequireTLS == pdTRUE))
		{
			TLSParams.ulSize = sizeof( TLSParams );
			TLSParams.pcDestination = pxContext->pcDestination;
			TLSParams.pcServerCertificate = pxContext->pcServerCertificate;
			TLSParams.ulServerCertificateLength = pxContext->ServerCertificateLength;
			TLSParams.pvCallerContext = pxContext;
			TLSParams.pxNetworkRecv = prvNetworkRecv;
			TLSParams.pxNetworkSend = prvNetworkSend;
			Status = TLS_Init( &pxContext->pvTLSContext, &TLSParams );

			if( Status == SOCKETS_ERROR_NONE )
				Status = TLS_Connect( pxContext->pvTLSContext );
			{
				if( Status == SOCKETS_ERROR_NONE )
				{
					pxContext->StateSecure |= securesocketsSOCKET_HANDSHAKE_DONE_FLAG;
					return SOCKETS_ERROR_NONE;
				}
				else
				{
					pxContext->StateSecure = TLS_HANDSHAKE_FAILED_FLAG;
					return SOCKETS_TLS_HANDSHAKE_ERROR;
				}
			}
		}
	}
	else
	{
		return SOCKETS_SOCKET_ERROR;
	}
}

/*-----------------------------------------------------------*/
/**
 * @brief Receive data from a TCP socket.
 * @param[in] xSocket The handle of the socket from which data is being received.
 * @param[out] pvBuffer The buffer into which the received data will be placed.
 * @param[in] xBufferLength The maximum number of bytes which can be received.
 * pvBuffer must be at least xBufferLength bytes long.
 * @param[in] ulFlags Not currently used. Should be set to 0.
 *
 * @return
 * * If the receive was successful then the number of bytes received (placed in the
 *   buffer pointed to by pvBuffer) is returned.
 * * If a timeout occurred before data could be received then 0 is returned (timeout
 *   is set using @ref SOCKETS_SO_RCVTIMEO).
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t SOCKETS_Recv( Socket_t xSocket,
		void * pvBuffer,
		size_t xBufferLength,
		uint32_t ulFlags )
{
	int32_t Status = SOCKETS_ERROR_NONE;
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

	if( ( xSocket != SOCKETS_INVALID_SOCKET ) && ( pvBuffer != NULL)  &&
			( ( securesocketsSOCKET_READ_CLOSED_FLAG & pxContext->xShutdownFlags ) == 0UL ) &&
			( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
	)
	{
		if( pxContext->xRequireTLS == pdTRUE)
		{
			/* Receive through TLS pipe */
			Status = TLS_Recv( pxContext->pvTLSContext, pvBuffer, xBufferLength );
		}
		else
		{
			/* Receive unencrypted. */
			Status = prvNetworkRecv( pxContext, pvBuffer, xBufferLength );
		}
		return Status;
	}
	else
	{
		return SOCKETS_SOCKET_ERROR;
	}
}

/*-----------------------------------------------------------*/
/**
 * @brief Transmit data to the remote socket.
 * @param[in] xSocket The handle of the sending socket.
 * @param[in] pvBuffer The buffer containing the data to be sent.
 * @param[in] xDataLength The length of the data to be sent.
 * @param[in] ulFlags Not currently used. Should be set to 0.
 *
 * @return
 * * On success, the number of bytes actually sent is returned.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t SOCKETS_Send( Socket_t xSocket,
		const void * pvBuffer,
		size_t xDataLength,
		uint32_t ulFlags )
{
	int32_t Status = SOCKETS_ERROR_NONE;
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

	if( (xSocket != SOCKETS_INVALID_SOCKET ) && ( pvBuffer != NULL ) )
	{
		if( pdTRUE == pxContext->xRequireTLS )
		{
			Status = TLS_Send(pxContext->pvTLSContext,pvBuffer,xDataLength);
		}
		else
		{
			Status = prvNetworkSend(pxContext,pvBuffer,xDataLength);
		}
	}
	else
	{
		Status = SOCKETS_EINVAL;
	}
	return Status;
}

/*-----------------------------------------------------------*/
/**
 * @brief Closes all or part of a full-duplex connection on the socket.
 * \warning SOCKETS_Shutdown() is not safe to be called on the same socket
 * from multiple threads simultaneously with SOCKETS_Connect(),
 * SOCKETS_SetSockOpt(), SOCKETS_Shutdown(), SOCKETS_Close().
 * @param[in] xSocket The handle of the socket to shutdown.
 * @param[in] ulHow SOCKETS_SHUT_RD, SOCKETS_SHUT_WR or SOCKETS_SHUT_RDWR.
 * @ref ShutdownFlags
 *
 * @return
 * * If the operation was successful, 0 is returned.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t SOCKETS_Shutdown( Socket_t xSocket,
		uint32_t ulHow )
{
	int32_t Status = SOCKETS_SOCKET_ERROR;
	SSOCKETContextPtr_t pxSecureSocket = ( SSOCKETContextPtr_t ) xSocket;

	if( pxSecureSocket != SOCKETS_INVALID_SOCKET )
	{
		switch( ulHow )
		{
			case SOCKETS_SHUT_RD:
				/*After this flag set send call on this socket should return error*/
				pxSecureSocket->xShutdownFlags |= securesocketsSOCKET_READ_CLOSED_FLAG;

				Status = SOCKETS_ERROR_NONE;
				break;

			case SOCKETS_SHUT_WR:
				/*After this flag set receive call on this socket should return error*/
				pxSecureSocket->xShutdownFlags |= securesocketsSOCKET_WRITE_CLOSED_FLAG;

				/* Return success to the user. */
				Status = SOCKETS_ERROR_NONE;
				break;

			case SOCKETS_SHUT_RDWR:
				pxSecureSocket->xShutdownFlags |= securesocketsSOCKET_READ_CLOSED_FLAG;
				pxSecureSocket->xShutdownFlags |= securesocketsSOCKET_WRITE_CLOSED_FLAG;

				/* Return success to the user. */
				Status = SOCKETS_ERROR_NONE;
				break;

			default:
				Status = SOCKETS_EINVAL;
                break;
		}
	}
	else
	{
		return SOCKETS_EINVAL;
	}

	return Status;
}

/*-----------------------------------------------------------*/
/**
 * @brief Closes the socket and frees the related resources.
 * @param[in] xSocket The handle of the socket to close.
 * @return
 * * On success, 0 is returned.
 * * If an error occurred, a negative value is returned. @ref SocketsErrors
 */
int32_t SOCKETS_Close( Socket_t xSocket )
{
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

	int32_t Status = SOCKETS_ERROR_NONE;

	if( ( pxContext != NULL ) && ( pxContext != SOCKETS_INVALID_SOCKET ) )
	{
		pxContext->State = 0;

		if( pxContext->pcDestination != NULL )
		{
			vPortFree( pxContext->pcDestination );
		}

		if( pxContext->pcServerCertificate != NULL )
		{
			vPortFree( pxContext->pcServerCertificate );
		}

		if( pxContext->xRequireTLS  == pdTRUE )
		{
			TLS_Cleanup( pxContext->pvTLSContext );
		}
		/*Check the provided socket is in given range*/

		if( pxContext->xSocket < securesocketsMAX_NUM_SOCKETS )
		{
			if((pxContext->StateSecure == TLS_HANDSHAKE_FAILED_FLAG) )
			{
				vPortFree( pxContext );

				Status = SOCKETS_ERROR_NONE;  							//!socket is already closed in case of Handshake failed
				return Status;
			}

			Status = rsi_shutdown(pxContext->xSocket, 0);  				//!Don't call if handshake failed

			if(Status < 0)
			{
				Status = SOCKETS_SOCKET_ERROR;
			}
			else
			{
				Status = SOCKETS_ERROR_NONE;
			}

			vPortFree( pxContext );

			return Status;
		}
	}
	else
	{
		return SOCKETS_EINVAL;
	}
}

/*-----------------------------------------------------------*/
int32_t SOCKETS_SetSockOpt( Socket_t xSocket,
		int32_t lLevel,
		int32_t lOptionName,
		const void * pvOptionValue,
		size_t xOptionLength )
{
	int32_t Status = SOCKETS_ERROR_NONE;
	SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;
	TickType_t xTimeout;

	uint32_t Timeout,max_tcp_retry = 10;

	if( ( pxContext != NULL ) && ( pxContext != SOCKETS_INVALID_SOCKET ) )
	{
		switch(lOptionName)
		{
		  case SOCKETS_SO_SERVER_NAME_INDICATION:
			  /* SNI socket option cannot be used on already connected socket */
			  if( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
			  {
			    Status = SOCKETS_SOCKET_ERROR;
				break;
			  }

			  /* Non-NULL destination string indicates that SNI extension should
			  * be used during TLS negotiation. */
			  if( NULL == ( pxContext->pcDestination =
						( char * ) pvPortMalloc( 1 + xOptionLength ) ) )
			  {
			    Status = SOCKETS_ENOMEM;
			  }
			  else
			  {
			     memcpy( pxContext->pcDestination, pvOptionValue, xOptionLength );
			     pxContext->pcDestination[ xOptionLength ] = '\0';
		      }
			  break;

		    case SOCKETS_SO_TRUSTED_SERVER_CERTIFICATE:
			    /* Secure socket option cannot be used on already connected socket */
			    if( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
			    {
			      Status = SOCKETS_SOCKET_ERROR;
			      break;
			    }

			   /* Non-NULL server certificate field indicates that the default trust
			   * list should not be used. */
			   if( NULL == ( pxContext->pcServerCertificate =
			  	     ( char * ) pvPortMalloc( xOptionLength ) ) )
			   {
			      Status = SOCKETS_ENOMEM;
			   }
			   else
			   {
			     memcpy( pxContext->pcServerCertificate, pvOptionValue, xOptionLength );
			     pxContext->ServerCertificateLength = xOptionLength;
			   }
			   break;

		    case SOCKETS_SO_REQUIRE_TLS:
 			      /* Secure socket option cannot be used on already connected socket */
			     if( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
			     {
			       Status = SOCKETS_SOCKET_ERROR;
				   break;
			     }

			    pxContext->xRequireTLS = pdTRUE;
			    break;

		    case SOCKETS_SO_NONBLOCK:

		       if( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
		       {
		         xTimeout = 0;
			     pxContext->ulSendTimeout = 1;
			     pxContext->ulRecvTimeout = 1;
		       }
		       else
		       {
		         Status = SOCKETS_SOCKET_ERROR;
		       }
		      break;

	        case SOCKETS_SO_RCVTIMEO:
			    xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t. */

			    if( xTimeout == 0U )
			    {
				  pxContext->ulRecvTimeout = portMAX_DELAY;
			    }
			    else
			    {
				  pxContext->ulRecvTimeout = xTimeout;
			    }

			//			Status=rsi_setsockopt(( int ) pxContext->xSocket, SOL_SOCKET, SO_RCVTIMEO, &xTimeout, sizeof(xTimeout));
			  break;

		   case SOCKETS_SO_SNDTIMEO:
			   /* Comply with Berkeley standard - a 0 timeout is wait forever. */
			   xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t. */

			   if( xTimeout == 0U )
			   {
				 pxContext->ulSendTimeout = portMAX_DELAY;
			   }
			   else
			   {
				 pxContext->ulSendTimeout = xTimeout;
			   }
			   break;

 		   case SOCKETS_SO_ALPN_PROTOCOLS:

			  /* Secure socket option cannot be used on connected socket */
			  if( pxContext->State & ( securesocketsSOCKET_CONNECTED_FLAG ) )
			  {
			    Status = SOCKETS_SOCKET_ERROR;
			    break;
			  }
			 /* NOT implemented ? */
			break;

		  default:

			Status = rsi_setsockopt(( int ) pxContext->xSocket, lLevel, lOptionName, &max_tcp_retry, sizeof(max_tcp_retry));
			break;

		}

	}
	else
	{
		Status = SOCKETS_SOCKET_ERROR;
	}

	return Status;
}
/*-----------------------------------------------------------*/
/**
 * @brief Resolve a host name using Domain Name Service.
 * @param[in] pcHostName The host name to resolve.
 * @return
 * * The IPv4 address of the specified host.
 * * If an error has occured, 0 is returned.
 */
/* @[declare_secure_sockets_gethostbyname] */
uint32_t SOCKETS_GetHostByName( const char * pcHostName )
{
	uint8_t ucIPAddr[ 4 ];
	uint32_t Ipaddr=0;
	if(strlen(pcHostName) <= securesocketsMAX_DNS_NAME_LENGTH)
	{
		WIFI_GetHostIP(pcHostName,ucIPAddr);

		Ipaddr = rsi_bytes4R_to_uint32(ucIPAddr);
	}
	else
	{
		return Ipaddr;
	}
}
/*-----------------------------------------------------------*/

BaseType_t SOCKETS_Init( void )
{
	/* Socket initialization not required*/
	return pdPASS;
}
/*-----------------------------------------------------------*/

uint32_t ulRand( void )
{
	/* FIX ME. */
	return 0;
}


/*-----------------------------------------------------------*/
