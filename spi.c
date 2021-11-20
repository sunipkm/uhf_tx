/*! @file spi.c
 * @brief This file contains functions for controlling SPI0 and SPI1 interfaces.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "bsp.h"
#include <string.h>
#include "spibus/spibus.h"

bool spi_initd = false;
spibus rf_spi[1];

void vSpi_init()
{
  memset(rf_spi, 0x0, sizeof(spibus));
  rf_spi->bits = 8;
  rf_spi->bus = 0;
  rf_spi->cs = 1;
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

/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/
/**< Current state of the NSEL of SPI0 */


/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/


/*!
 * This function is used to read/write one byte from/to SPI1.
 *
 * @param[in] biDataIn    Data to be sent.
 * @return  Read value of the SPI port after writing on it.
 */
U8 bSpi_ReadWriteSpi1(U8 biDataIn)
{  
  U8 bValue;
  spibus_xfer_full(rf_spi, &bValue, 1, &biDataIn, 1);
  return bValue;
}


/*!
 * This function is used to send data over SPI1 no response expected.
 *
 *  @param[in] biDataInLength  The length of the data.
 *  @param[in] *pabiDataIn     Pointer to the first element of the data.
 *
 *  @return None
 */
void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn)
{
  while (biDataInLength--) {
    bSpi_ReadWriteSpi1(*pabiDataIn++);
  }
}


/*!
 * This function is used to read data from SPI1.
 *
 *  \param[in] biDataOutLength  The length of the data.
 *  \param[out] *paboDataOut    Pointer to the first element of the response.
 *
 *  \return None
 */
void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut)
{
  // send command and get response from the radio IC
  while (biDataOutLength--) {
    *paboDataOut++ = bSpi_ReadWriteSpi1(0xFF);
  }
}


  /**
 *  Pull down nSEL of the selected device.
 *
 *  @param[in] qiSelect Device select.
 *
 *  @note Input: qiSelect <br> 0 - \b DUT <br> 1 - \b LCD <br>
 *
 ******************************************************************************/
void vSpi_ClearNsel(eSpi_Nsel qiSelect)
{
  switch (qiSelect)
  {
      case eSpi_Nsel_RF:
        gpioWrite(RF_NSEL, GPIO_LOW);
        break;
      default:
        break;
  }
}


/**
 *  Pull-up nSEL of the selected device.
 *
 *  @param[in] qiSelect Device select.
 *
 *  @note Input: qiSelect <br> 0 - \b DUT <br> 1 - \b LCD <br>
 *
 ******************************************************************************/
void vSpi_SetNsel(eSpi_Nsel qiSelect)
{
  switch (qiSelect)
  {
      case eSpi_Nsel_RF:
        gpioWrite(RF_NSEL, GPIO_HIGH);
        break;

      default:
        break;
  }
}