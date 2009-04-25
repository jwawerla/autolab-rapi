/****************************************************************************
*
*   Copyright (c) 2006 Dave Hylands     <dhylands@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*   See README and COPYING for more details.
*
****************************************************************************/
/**
*
*   @file   i2c-io-api.c
*
*   @brief  This file immplements the i2c-io operations on the gumstix.
*
****************************************************************************/

// ---- Include Files -------------------------------------------------------

#include <string.h>
#include <errno.h>

#include "i2c-api.h"
#include "i2c-io.h"
#include "i2c-io-api.h"
#include "Log.h"

// ---- Public Variables ----------------------------------------------------

// ---- Private Constants and Types -----------------------------------------

#define TRUE    1
#define FALSE   0

// ---- Private Variables ---------------------------------------------------

// ---- Private Function Prototypes -----------------------------------------

// ---- Functions -----------------------------------------------------------

//***************************************************************************
/**
*   Checks the version of the bootloader to see if we're compatible.
*/

int I2C_IO_CheckVersion( const I2C_IO_Info_t *info )
{
    // If the range defined by I2C_IO_API_MIN_VERSION to I2C_IO_API_VERSION
    // overlaps with the info from the i2c-io on the robostix, then we can talk.

    if ( info->minVersion > I2C_IO_API_VERSION )
    {
        LogError( "BootLoader device is too new; Host is (%d-%d) Device is (%d-%d)\n",
                  I2C_IO_API_MIN_VERSION, I2C_IO_API_VERSION,
                  info->minVersion, info->version );

        return 0;
    }

    if ( I2C_IO_API_MIN_VERSION > info->version )
    {
        LogError( "BootLoader device is too old; Host is (%d-%d) Device is (%d-%d)\n",
                  I2C_IO_API_MIN_VERSION, I2C_IO_API_VERSION,
                  info->minVersion, info->version );

        return 0;
    }

    return 1;

} // I2C_IO_CheckVersion

//***************************************************************************
/**
*   Retrieves information about the i2c-io program running on the robostix
*/

int I2C_IO_GetInfo( int i2cDev, I2C_IO_Info_t *info )
{
    LogDebug( "----- I2C_IO_GetInfo -----\n" );

    if ( I2cReadBlock( i2cDev, I2C_IO_GET_INFO, info, sizeof( *info ), NULL ) != 0 )
    {
        LogError( "I2cReadBlock failed\n" );
        return 0;
    }

    return 1;

} // I2C_IO_GetInfo

//***************************************************************************
/**
*   Retrieves the value of a pin.
*/

int I2C_IO_GetGPIO( int i2cDev, uint8_t portNum, uint8_t *pinVal )
{
    I2C_IO_Get_GPIO_t   getReq;
    uint8_t             bytesRead = 0;

    getReq.portNum = portNum;

    if ( I2cProcessBlock( i2cDev, I2C_IO_GET_GPIO, &getReq, sizeof( getReq ),
                          pinVal, sizeof( *pinVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_GetGPIO: I2cProcessBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_GetGPIO

//***************************************************************************
/**
*   Sets the value of a pin.
*/

int I2C_IO_SetGPIO( int i2cDev, uint8_t portNum, uint8_t pinMask, uint8_t pinVal )
{
    I2C_IO_Set_GPIO_t   setReq;

    setReq.portNum = portNum;
    setReq.pinMask = pinMask;
    setReq.pinVal  = pinVal;

    if ( I2cWriteBlock( i2cDev, I2C_IO_SET_GPIO, &setReq, sizeof( setReq )) != 0 )
    {
        LogError( "I2C_IO_SetGPIO: I2cWriteBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_SetGPIO

//***************************************************************************
/**
*   Gets the direction of a pin.
*/

int I2C_IO_GetGPIODir( int i2cDev, uint8_t portNum, uint8_t *pinVal )
{
    I2C_IO_Get_GPIO_t   getReq;
    uint8_t             bytesRead = 0;

    getReq.portNum = portNum;

    if ( I2cProcessBlock( i2cDev, I2C_IO_GET_GPIO_DIR, &getReq, sizeof( getReq ),
                          pinVal, sizeof( *pinVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_GetGPIODir: I2cProcessBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_GetGPIODir

//***************************************************************************
/**
*   Sets the direction of a pin.
*/

int I2C_IO_SetGPIODir( int i2cDev, uint8_t portNum, uint8_t pinMask, uint8_t pinVal )
{
    I2C_IO_Set_GPIO_t   setReq;

    setReq.portNum = portNum;
    setReq.pinMask = pinMask;
    setReq.pinVal  = pinVal;

    if ( I2cWriteBlock( i2cDev, I2C_IO_SET_GPIO_DIR, &setReq, sizeof( setReq )) != 0 )
    {
        LogError( "I2C_IO_SetGPIODir: I2cWriteBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_SetGPIODir

//***************************************************************************
/**
*   Retrieves the adc value associated wth 'mux'.
*/

int I2C_IO_GetADC( int i2cDev, uint8_t mux, uint16_t *adcVal )
{
    I2C_IO_Get_ADC_t    adcReq;
    uint8_t             bytesRead = 0;

    adcReq.mux = mux;

    if ( I2cProcessBlock( i2cDev, I2C_IO_GET_ADC, &adcReq, sizeof( adcReq ),
                          adcVal, sizeof( *adcVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_GetADC: I2cProcessBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_GetADC


//***************************************************************************
/**
*   Retrieves the lpf adc value associated wth 'mux'.
*/

int I2C_IO_GetLpfADC( int i2cDev, uint8_t mux, float *adcVal )
{
    I2C_IO_Get_ADC_t    adcReq;
    uint8_t             bytesRead = 0;

    adcReq.mux = mux;

    if ( I2cProcessBlock( i2cDev, I2C_IO_GET_LPF_ADC, &adcReq, sizeof( adcReq ),
                          adcVal, sizeof( *adcVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_GetLpfADC: I2cProcessBlock failed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }
    return TRUE;

} // I2C_IO_GetADC

//***************************************************************************
/**
*   Reads an 8 bit register.
*/

int I2C_IO_ReadReg8( int i2cDev, uint8_t reg, uint8_t *regVal )
{
    I2C_IO_ReadReg8_t   readReg;
    uint8_t             bytesRead;

    readReg.reg = reg;

    if ( I2cProcessBlock( i2cDev, I2C_IO_READ_REG_8, &readReg, sizeof( readReg ),
                          regVal, sizeof( *regVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_ReadReg8: I2cProcessBlockFailed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }

    return TRUE;

} // I2C_IO_ReadReg8

//***************************************************************************
/**
*   Reads a 16 bit register.
*/

int I2C_IO_ReadReg16( int i2cDev, uint8_t reg, uint16_t *regVal )
{
    I2C_IO_ReadReg16_t  readReg;
    uint8_t             bytesRead;

    readReg.reg = reg;

    if ( I2cProcessBlock( i2cDev, I2C_IO_READ_REG_16, &readReg, sizeof( readReg ),
                          regVal, sizeof( *regVal ), &bytesRead ) != 0 )
    {
        LogError( "I2C_IO_ReadReg16: I2cProcessBlockFailed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }

    return TRUE;

} // I2C_IO_ReadReg16

//***************************************************************************
/**
*   Write an 8 bit register.
*/

int I2C_IO_WriteReg8( int i2cDev, uint8_t reg, uint8_t regVal )
{
    I2C_IO_WriteReg8_t   writeReg;

    writeReg.reg = reg;
    writeReg.val = regVal;

    if ( I2cWriteBlock( i2cDev, I2C_IO_WRITE_REG_8, &writeReg, sizeof( writeReg )) != 0 )
    {
        LogError( "I2C_IO_WriteReg8: I2cWriteBlockFailed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }

    return TRUE;

} // I2C_IO_WriteReg8

//***************************************************************************
/**
*   Write a 16 bit register.
*/

int I2C_IO_WriteReg16( int i2cDev, uint8_t reg, uint16_t regVal )
{
    I2C_IO_WriteReg16_t   writeReg;

    writeReg.reg = reg;
    writeReg.val = regVal;

    if ( I2cWriteBlock( i2cDev, I2C_IO_WRITE_REG_16, &writeReg, sizeof( writeReg )) != 0 )
    {
        LogError( "I2C_IO_WriteReg16: I2cWriteBlockFailed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }

    return TRUE;

} // I2C_IO_WriteReg16

//***************************************************************************
/**
*   Write time constant for low pass filter
*/

int I2C_IO_SetTauLpf( int i2cDev, float tau)
{
    I2C_IO_SetTau_t   writeData;

    writeData.tau = tau;

    if ( I2cWriteBlock( i2cDev, I2C_IO_SET_LPF_TAU, &writeData, sizeof( writeData )) != 0 )
    {
        LogError( "I2C_IO_SetTauLpf: I2cWriteBlockFailed: %s (%d)\n", strerror( errno ), errno );
        return FALSE;
    }

    return TRUE;

} // I2C_IO_SetTauLpf

