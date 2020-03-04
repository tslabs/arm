/*----------------------------------------------------------------------------
 *      RL-ARM - USB
 *----------------------------------------------------------------------------
 *      Name:    usbd_user_msc.c
 *      Purpose: Mass Storage Device Class User module
 *      Rev.:    V4.70
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2013 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <rl_usb.h>
#include <string.h>

U8 *bulk = (U8 *)0x20000000;
// U8 *bulk = (U8 *)0x00000000;
U8 BlockBuf[512];

void usbd_msc_init () {
  USBD_MSC_MemorySize = 128 * 1024;
  // USBD_MSC_MemorySize = 1024 * 1024;
  USBD_MSC_BlockSize  = 512;
  USBD_MSC_BlockGroup = 1;
  USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
  USBD_MSC_BlockBuf   = BlockBuf;
  USBD_MSC_MediaReady = __TRUE;
}

void usbd_msc_read_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
  if (USBD_MSC_MediaReady)
	{
		memcpy(buf, bulk + (block * USBD_MSC_BlockSize), num_of_blocks * USBD_MSC_BlockSize);
  }
}

void usbd_msc_write_sect (U32 block, U8 *buf, U32 num_of_blocks)
{
  if (USBD_MSC_MediaReady)
	{
    memcpy(bulk + (block * USBD_MSC_BlockSize), buf, num_of_blocks * USBD_MSC_BlockSize);
  }
}
