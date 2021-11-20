/*!
 * File:
 *  radio_comm.h
 *
 * Description:
 *  This file contains the RADIO communication layer.
 *
 * Silicon Laboratories Confidential
 * Copyright 2012 Silicon Laboratories, Inc.
 */

                /* ======================================= *
                 *              I N C L U D E              *
                 * ======================================= */

#include "bsp.h"
#include <stdlib.h>
#include <string.h>
#include "spibus/spibus.h"

spibus rf_spi[1];

void vSpi_init()
{
  memset(rf_spi, 0x0, sizeof(spibus));
  rf_spi->bits = 8;
  rf_spi->bus = 0;
  rf_spi->cs = 0;
  rf_spi->cs_gpio = -1;
  rf_spi->cs_internal = 1;
  rf_spi->internal_rotation = false;
  rf_spi->lsb = 0;
  rf_spi->mode = SPI_MODE_0;
  rf_spi->speed = 4000000;
  rf_spi->sleeplen = 0;

  if (spibus_init(rf_spi) < 0)
  {
    eprintf("Error initializing SPI");
    exit(0);
  }
}

                /* ======================================= *
                 *          D E F I N I T I O N S          *
                 * ======================================= */

                /* ======================================= *
                 *     G L O B A L   V A R I A B L E S     *
                 * ======================================= */

BIT ctsWentHigh = 0;


                /* ======================================= *
                 *      L O C A L   F U N C T I O N S      *
                 * ======================================= */

                /* ======================================= *
                 *     P U B L I C   F U N C T I O N S     *
                 * ======================================= */
/*!
 * Gets a command response from the radio chip
 *
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 *
 * @return CTS value
 */
U8 radio_comm_GetResp(U8 byteCount, U8* pData)
{
  SEGMENT_VARIABLE(ctsVal = 0u, U8, SEG_DATA);
  SEGMENT_VARIABLE(errCnt = RADIO_CTS_TIMEOUT, U16, SEG_DATA);
  static U8 inp[256], outp[256];
  memset(outp, 0xff, byteCount + 2);
  outp[0] = 0x44;
  while (errCnt != 0)      //wait until radio IC is ready with the data
  {
    spibus_xfer_full(rf_spi, inp, byteCount + 2, outp, byteCount + 2);
    ctsVal = inp[1];
    if (ctsVal == 0xff)
    {
      memcpy(pData, inp + 2, byteCount);
      break;
    }
    usleep(100);
    errCnt--;
  }
  if (errCnt == 0)
  {
    eprintf("FATAL: CTS took 1000 seconds");
    exit(0);
  }

  if (ctsVal == 0xFF)
  {
    ctsWentHigh = 1;
  }

  return ctsVal;
}

/*!
 * Sends a command to the radio chip
 *
 * @param byteCount     Number of bytes in the command to send to the radio device
 * @param pData         Pointer to the command to send.
 */
void radio_comm_SendCmd(U8 byteCount, U8* pData)
{
    while (!ctsWentHigh)
    {
        radio_comm_PollCTS();
    }
    spibus_xfer(rf_spi, pData, byteCount);
    ctsWentHigh = 0;
}

/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip.
 * @param pData         Pointer to where to put the data.
 */
void radio_comm_ReadData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    static U8 inp[256], outp[256];
    memset(inp, 0x0, byteCount + 1);
    memset(outp, 0xff, byteCount + 1);
    outp[0] = cmd;
    spibus_xfer_full(rf_spi, inp, byteCount + 1, outp, byteCount + 1);
    memcpy(pData, inp + 1, byteCount);
    ctsWentHigh = 0;
}


/*!
 * Gets a command response from the radio chip
 *
 * @param cmd           Command ID
 * @param pollCts       Set to poll CTS
 * @param byteCount     Number of bytes to get from the radio chip
 * @param pData         Pointer to where to put the data
 */
void radio_comm_WriteData(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    static U8 outp[256];
    outp[0] = cmd;
    memcpy(outp + 1, pData, byteCount);
    spibus_xfer(rf_spi, outp, byteCount + 1);
    ctsWentHigh = 0;
}

void radio_comm_WriteFifo(U8 cmd, BIT pollCts, U8 byteCount, U8* pData)
{
    if(pollCts)
    {
        while(!ctsWentHigh)
        {
            radio_comm_PollCTS();
        }
    }
    static U8 outp[256];
    outp[0] = cmd;
    outp[1] = byteCount;
    memcpy(outp + 2, pData, byteCount);
    spibus_xfer(rf_spi, outp, byteCount + 2);
    ctsWentHigh = 0;
}
/*!
 * Waits for CTS to be high
 *
 * @return CTS value
 */
U8 radio_comm_PollCTS(void)
{

    return radio_comm_GetResp(0, 0);
}

/**
 * Clears the CTS state variable.
 */
void radio_comm_ClearCTS()
{
  ctsWentHigh = 0;
}

/*!
 * Sends a command to the radio chip and gets a response
 *
 * @param cmdByteCount  Number of bytes in the command to send to the radio device
 * @param pCmdData      Pointer to the command data
 * @param respByteCount Number of bytes in the response to fetch
 * @param pRespData     Pointer to where to put the response data
 *
 * @return CTS value
 */
U8 radio_comm_SendCmdGetResp(U8 cmdByteCount, U8* pCmdData, U8 respByteCount, U8* pRespData)
{
    radio_comm_SendCmd(cmdByteCount, pCmdData);
    return radio_comm_GetResp(respByteCount, pRespData);
}

