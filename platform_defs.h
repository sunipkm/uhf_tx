/*! @file platform_defs.h
 * @brief This file contains platform specific definitions.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef PLATFORM_DEFS_H_
#define PLATFORM_DEFS_H_

#define SILABS_PLATFORM_WMB
#define SILABS_PLATFORM_WMB930
/*---------------------------------------------------------------------*/
/*            Platform specific global definitions                     */
/*---------------------------------------------------------------------*/

  #define SILABS_PLATFORM_WMB
  #define SILABS_RADIO_SI446X
  #undef  SILABS_RADIO_SI4455
  #define SILABS_MCU_F930
  #define SILABS_LCD_DOG_GLCD

  #define SILABS_PLATFORM_COMPONENT_LED     4
  #define SILABS_PLATFORM_COMPONENT_PB      4
//  #define SILABS_PLATFORM_COMPONENT_SWITCH  0
  #define SILABS_PLATFORM_COMPONENT_BUZZER  1

#endif /* PLATFORM_DEFS_H_ */
