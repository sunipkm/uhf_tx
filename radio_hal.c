/*!
 * File:
 *  radio_hal.c
 *
 * Description:
 *  This file contains RADIO HAL.
 *
 * Silicon Laboratories Confidential
 * Copyright 2011 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */

#include "bsp.h"


                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */

void radio_hal_AssertShutdown(void)
{
  gpioWrite(RF_PWRDN, GPIO_HIGH);
}

void radio_hal_DeassertShutdown(void)
{
  gpioWrite(RF_PWRDN, GPIO_LOW);
}

void radio_hal_ClearNsel(void)
{
    gpioWrite(RF_NSEL, GPIO_LOW);
}

void radio_hal_SetNsel(void)
{
    gpioWrite(RF_NSEL, GPIO_HIGH);
}

BIT radio_hal_NirqLevel(void)
{
    return gpioRead(RF_NIRQ);
}

void radio_hal_SpiWriteByte(U8 byteToWrite)
{
  bSpi_ReadWriteSpi1(byteToWrite);
}

U8 radio_hal_SpiReadByte(void)
{
  return bSpi_ReadWriteSpi1(0xFF);
}

void radio_hal_SpiWriteData(U8 byteCount, U8* pData)
{
  vSpi_WriteDataSpi1(byteCount, pData);
}

void radio_hal_SpiReadData(U8 byteCount, U8* pData)
{
  vSpi_ReadDataSpi1(byteCount, pData);
}

#ifdef RADIO_DRIVER_EXTENDED_SUPPORT
BIT radio_hal_Gpio0Level(void)
{
  BIT retVal = FALSE;
  retVal = FALSE;


  return retVal;
}

BIT radio_hal_Gpio1Level(void)
{
  BIT retVal = FALSE;
  return retVal;
}

BIT radio_hal_Gpio2Level(void)
{
  BIT retVal = FALSE;
  return retVal;
}

BIT radio_hal_Gpio3Level(void)
{
  BIT retVal = FALSE;
  return retVal;
}

#endif
