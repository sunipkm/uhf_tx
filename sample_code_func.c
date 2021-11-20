/*! @file sample_code_func.c
 * @brief This file contains functions to manage behavior of basic human module interfaces (push-buttons, switches, LEDs).
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#include "bsp.h"


/*------------------------------------------------------------------------*/
/*                          Global variables                              */
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/

/*!
 * This function is used to send fix payload length packet.
 *
 * @return  TRUE - Started a packet TX / FALSE - No packet sent.
 */
BIT vSampleCode_SendFixPacket(void)
{

  vRadio_StartTx(pRadioConfiguration->Radio_ChannelNumber, (U8 *) &pRadioConfiguration->Radio_CustomPayload[0u]);

  /* Packet sending initialized */
  return TRUE;
}
