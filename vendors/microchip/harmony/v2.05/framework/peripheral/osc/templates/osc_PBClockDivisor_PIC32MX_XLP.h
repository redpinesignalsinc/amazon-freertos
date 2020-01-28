/*******************************************************************************
  OSC Peripheral Library Template Implementation

  File Name:
    osc_PBClockDivisor_PIC32MX_XLP.h

  Summary:
    OSC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : PBClockDivisor
    and its Variant : PIC32MX_XLP
    For following APIs :
        PLIB_OSC_ExistsPBClockDivisor
        PLIB_OSC_PBClockDivisorGet
        PLIB_OSC_PBClockDivisorSet

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/

//DOM-IGNORE-END

#ifndef _OSC_PBCLOCKDIVISOR_PIC32MX_XLP_H
#define _OSC_PBCLOCKDIVISOR_PIC32MX_XLP_H

// *****************************************************************************
/* Oscillator Peripheral Bus Clock Divisor Value Type

  Summary:
    Type of the oscillator PB Clock divisor value.

  Description:
    This macro defines the type of the oscillator PB Clock divisor value.

  Remarks:
    None
*/

#define OSC_PB_CLOCK_DIV_TYPE						SFR_TYPE


// *****************************************************************************
/* Oscillator Peripheral Bus count

  Summary:
    The number of Peripheral buses in available in the device.

  Description:
    This macro gives the number of Peripheral buses in available in the device.

  Remarks:
    None
*/

#define PERIPHERAL_BUSES_MAX						8


//******************************************************************************
/* Function :  OSC_ExistsPBClockDivisor_PIC32MX_XLP

  Summary:
    Implements PIC32MX_XLP variant of PLIB_OSC_ExistsPBClockDivisor

  Description:
    This template implements the PIC32MX_XLP variant of the PLIB_OSC_ExistsPBClockDivisor function.
*/

#define PLIB_OSC_ExistsPBClockDivisor PLIB_OSC_ExistsPBClockDivisor
PLIB_TEMPLATE bool OSC_ExistsPBClockDivisor_PIC32MX_XLP( OSC_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  OSC_PBClockDivisorGet_PIC32MX_XLP

  Summary:
    Implements PIC32MX_XLP variant of PLIB_OSC_PBClockDivisorGet

  Description:
    This template implements the PIC32MX_XLP variant of the PLIB_OSC_PBClockDivisorGet function.
*/

PLIB_TEMPLATE OSC_PB_CLOCK_DIV_TYPE OSC_PBClockDivisorGet_PIC32MX_XLP( OSC_MODULE_ID index , OSC_PERIPHERAL_BUS peripheralBusNumber )
{
    return (PB1DIVbits.PBDIV + 1u );
}


//******************************************************************************
/* Function :  OSC_PBClockDivisorSet_PIC32MX_XLP

  Summary:
    Implements PIC32MX_XLP variant of PLIB_OSC_PBClockDivisorSet

  Description:
    This template implements the PIC32MX_XLP variant of the PLIB_OSC_PBClockDivisorSet function.
*/

PLIB_TEMPLATE void OSC_PBClockDivisorSet_PIC32MX_XLP( OSC_MODULE_ID index , OSC_PERIPHERAL_BUS peripheralBusNumber , OSC_PB_CLOCK_DIV_TYPE peripheralBusClkDiv )
{
    PB1DIVbits.PBDIV = peripheralBusClkDiv - 1u;
}


#endif /*_OSC_PBCLOCKDIVISOR_PIC32MX_XLP_H*/

/******************************************************************************
 End of File
*/

