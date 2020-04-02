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
#include <stm32f4xx.h>
#include "swo.cpp"

int main (void) {

  // USB Initialization
  USBD_HighSpeed = 0;
  usbd_class_init();
  USBD_RTX_TaskInit();
  USBD_Init();

  // USB Connect
  USBD_Connect(__TRUE);

  while (1);
}
