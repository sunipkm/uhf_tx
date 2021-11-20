/*! @file bsp.h
 * @brief This file contains application specific definitions and includes.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef BSP_H
#define BSP_H

/*------------------------------------------------------------------------*/
/*            Application specific global definitions                     */
/*------------------------------------------------------------------------*/
/*! Platform definition */
/* Note: Plaform is defined in Silabs IDE project file as
 * a command line flag for the compiler. */
//#define SILABS_PLATFORM_WMB930
/*! Enable logging on UART */

/*! Extended driver support 
 * Known issues: Some of the example projects 
 * might not build with some extended drivers 
 * due to data memory overflow */
#define RADIO_DRIVER_EXTENDED_SUPPORT

#undef  RADIO_DRIVER_FULL_SUPPORT

/*------------------------------------------------------------------------*/
/*            Application specific includes                               */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define eprintf(str, ...) \
{ \
    fprintf(stderr, "%s, %d: " str "\n", __func__, __LINE__, ##__VA_ARGS__); \
    fflush(stderr); \
}

#include "platform_defs.h"

#include <stdbool.h>

#define FALSE false
#define TRUE true

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef bool BIT;

#define SEGMENT_VARIABLE(name,type,invalid) \
type name

#define SEGMENT_VARIABLE_SEGMENT_POINTER(name, type, invalid, invalid2) \
type *name

#define BITS(bitArray, bitPos)  BIT bitArray ## bitPos
#define WRITE_TO_BIT_ARRAY(bitArray, byte)  bitArray ## 0 = byte & 0x01; \
                                            bitArray ## 1 = byte & 0x02; \
                                            bitArray ## 2 = byte & 0x04; \
                                            bitArray ## 3 = byte & 0x08; \
                                            bitArray ## 4 = byte & 0x10; \
                                            bitArray ## 5 = byte & 0x20; \
                                            bitArray ## 6 = byte & 0x40; \
                                            bitArray ## 7 = byte & 0x80;

#define READ_FROM_BIT_ARRAY(bitArray, byte) byte =  (bitArray ## 0) | \
                                                   ((bitArray ## 1) << 1) | \
                                                   ((bitArray ## 2) << 2) | \
                                                   ((bitArray ## 3) << 3) | \
                                                   ((bitArray ## 4) << 4) | \
                                                   ((bitArray ## 5) << 5) | \
                                                   ((bitArray ## 6) << 6) | \
                                                   ((bitArray ## 7) << 7);

#include "radio_config.h"
#include "radio.h"
#include "sample_code_func.h"

#include "radio_hal.h"
#include "radio_comm.h"

#include "Si446x/si446x_api_lib.h"
#include "Si446x/si446x_defs.h"
#include "Si446x/si446x_nirq.h"


#include "gpiodev/gpiodev.h"
#define RF_PWRDN 13
#define RF_NSEL 15
#define RF_NIRQ 11


#endif //BSP_H
