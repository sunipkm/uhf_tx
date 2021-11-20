/*! @file main.c
 * @brief The main.c file of the Fixed packet length Custom TX demo
 * for Si446X devices.
 *
 * Contains the initialization of the MCU & the radio.
 * @n The main loop controls the program flow & the radio.
 *
 * @b CREATED
 * @n Silicon Laboratories Ltd.
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2012 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 *
 */

#include "bsp.h"


/*------------------------------------------------------------------------*/
/*                            Local Macros                                */
/*------------------------------------------------------------------------*/
#define PACKET_SEND_INTERVAL  250u

/*------------------------------------------------------------------------*/
/*                          Local variables                               */
/*------------------------------------------------------------------------*/
SEGMENT_VARIABLE(lPer_SecCntr, U16, SEG_DATA);  /**< Demo sec time holder */

/*------------------------------------------------------------------------*/
/*                      Local function prototypes                         */
/*------------------------------------------------------------------------*/
void vInitializeHW       (void);
void DemoApp_Pollhandler (void);

/*------------------------------------------------------------------------*/
/*                          Function implementations                      */
/*------------------------------------------------------------------------*/

/** \fn void main(void)
 * \brief The main function of the demo.
 *
 * \todo Create description
 */
void main(void)
{
  // Initialize the Hardware and Radio
  vInitializeHW();

  while (TRUE)
  {
    // Demo Application Poll-Handler function
    vRadio_StartTx(0, NULL);
    usleep(250);
  }
}

/**
 *  Demo Application Poll-Handler
 *
 *  @note This function must be called periodically.
 *
 */
void DemoApp_Pollhandler()
{
  static SEGMENT_VARIABLE(lPktSending, U8, SEG_XDATA) = 0u;

  if ((lPer_SecCntr >= PACKET_SEND_INTERVAL) && (0u == lPktSending))
  {
    if (TRUE == vSampleCode_SendFixPacket())
    {
      lPer_SecCntr = 0u;

      /* Set Packet Sending flag */
      lPktSending = 1u;
    }
  }
}

/**
 *  Calls the init functions for the used peripherals/modules
 *
 *  @note Should be called at the beginning of the main().
 *
 */
void vInitializeHW()
{
  // Initialize GPIO
  gpioSetMode(RF_NSEL, GPIO_OUT);
  gpioSetMode(RF_NSEL, GPIO_HIGH);
  gpioSetMode(RF_PWRDN, GPIO_OUT);
  gpioSetMode(RF_NIRQ, GPIO_IN);
  // Initialize SPI
  vSpi_init();
  // Initialize the Radio
  vRadio_Init();
}
