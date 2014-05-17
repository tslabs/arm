/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    USBD_Demo.c
 *      Purpose: USB Device Demonstration
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <stm32f4xx.h>                  /* STM32F4xx Definitions              */

int main (void) {

  usbd_init();                          /* USB Initialization                 */
  usbd_connect(__TRUE);                 /* USB Connect                        */

  while (1);                            /* Loop forever                       */
}
