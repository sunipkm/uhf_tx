/*! @file spi.h
 * @brief This file is the interface file for SPI0 and SPI1 routines.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef SPI_H
#define SPI_H

#include "bsp.h"

/*------------------------------------------------------------------------*/
/*                          Global typedefs & definitions                 */
/*------------------------------------------------------------------------*/

/*! SPI device select enum */

typedef enum
{
  eSpi_Nsel_RF
} eSpi_Nsel;



/*------------------------------------------------------------------------*/
/*                          External variables                            */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/*                           Function prototypes                          */
/*------------------------------------------------------------------------*/

void vSpi_init();

void vSpi_WriteDataSpi0(U8 biDataInLength, U8 *pabiDataIn);

void vSpi_ReadDataSpi0(U8 biDataOutLength, U8 *paboDataOut);
U8 bSpi_ReadWriteSpi0(U8 biDataIn);

void vSpi_WriteDataSpi1(U8 biDataInLength, U8 *pabiDataIn);
void vSpi_ReadDataSpi1(U8 biDataOutLength, U8 *paboDataOut);
U8 bSpi_ReadWriteSpi1(U8 biDataIn);

void vSpi_ClearNsel(eSpi_Nsel qiSelect);
void vSpi_SetNsel(eSpi_Nsel qiSelect);

#endif
