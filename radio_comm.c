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

  while (errCnt != 0)      //wait until radio IC is ready with the data
  {
    U8 in[2], out[2];
    out[0] = 0x44; // read cmd buffer
    out[1] = 0xff;
    memset(in, 0x0, sizeof(in));
    spibus_xfer_full(rf_spi, in, sizeof(in), out, sizeof(out));
    ctsVal = in[1];
    if (ctsVal == 0xff)
    {
      if (byteCount)
      {
        U8 *out2 = NULL;
        out2 = malloc(byteCount);
        if (out2 == NULL)
        {
          eprintf("(FATAL) Malloc failed");
          exit(0);
        }
        memset(out2, 0xff, byteCount);
        spibus_xfer_full(rf_spi, pData, byteCount, out2, byteCount);
        free(out2);
        break;
      }
      usleep(100);
      errCnt--;
    }
  }

  if (errCnt == 0)
  {
    eprintf("FATAL: CTS took 1000 seconds");
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
    U8 *in = NULL, *out = NULL;
    in = malloc(byteCount + 1);
    out = malloc(byteCount + 1);
    memset(in, 0x0, byteCount + 1);
    memset(out, 0xff, byteCount + 1);
    out[0] = cmd;
    spibus_xfer_full(rf_spi, in, byteCount + 1, out, byteCount + 1);
    free(out);
    memcpy(pData, in + 1, byteCount);
    free(in);
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
    U8 *out = NULL;
    out = malloc(byteCount + 1);
    out[0] = cmd;
    memcpy(out + 1, pData, byteCount);
    spibus_xfer(rf_spi, out, byteCount + 1);
    free(out);
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
    U8 *out = NULL;
    out = malloc(byteCount + 2);
    out[0] = cmd;
    out[1] = byteCount;
    memcpy(out + 2, pData, byteCount);
    spibus_xfer(rf_spi, out, byteCount + 2);
    free(out);
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

