
// To do:
// - check caps at Shenzhou-1 for crystal

#include <stdlib.h>
#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/stm32/timer.h>
#include "usb-ay.h"

#include "usb.c"
#include "psg.c"

void init_hw()
{
  // rcc_clock_setup_in_hsi_out_48mhz();
  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  // enable clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_AFIO);

  // MCO
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO8);

  // Clock configuration register2 (RCC_CFGR2)
  rcc_set_prediv2(15);  // Div: value=div: 0=1, ..., 15=16
  rcc_set_pll2_multiplication_factor(12); // Mul: value=mul: 6=8, ..., 12=14, 14=16, 15=20
  rcc_set_mco(RCC_CFGR_MCO_PLL2);
  rcc_osc_on(RCC_PLL2);

  // Timer for PSG clock
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_reset_pulse(RST_TIM2);

  timer_set_mode(PSG_TIM, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(PSG_TIM, 0);
  timer_disable_preload(PSG_TIM);
  timer_continuous_mode(PSG_TIM);
  timer_set_period(PSG_TIM, 1);  // ARR: 1=div(2), 3=div(4)
  timer_set_oc_mode(PSG_TIM, TIM_OC3, TIM_OCM_TOGGLE);
  timer_enable_oc_output(PSG_TIM, TIM_OC3);
  timer_slave_set_trigger(PSG_TIM, TIM_SMCR_TS_ETRF);
  timer_slave_set_mode(PSG_TIM, TIM_SMCR_SMS_ECM1);
  timer_enable_counter(PSG_TIM);

  gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON, AFIO_MAPR_TIM2_REMAP_FULL_REMAP);
  gpio_set_mode(PSG_CLK_P, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, PSG_CLK);

  // init AD5206 interface
  gpio_set(AD_PORT, AD_CS);
  gpio_clear(AD_PORT, AD_CLK | AD_SDI);
  gpio_set_mode(AD_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, AD_CS | AD_CLK | AD_SDI);

  // init AY interface
  gpio_set_mode(PSG_DPORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO7 | GPIO6 | GPIO5 | GPIO4 | GPIO3 | GPIO2 | GPIO1 | GPIO0);

  gpio_clear(PSG_PORT, PSG_BC1 | PSG_BDIR | PSG_RES);
  gpio_set_mode(PSG_PORT, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, PSG_BC1 | PSG_BDIR | PSG_RES);
  gpio_set(PSG_PORT, PSG_RES);

  // USB
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
  gpio_set(GPIOA, GPIO9);

  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
  gpio_clear(GPIOA, GPIO12);
  for (unsigned int i = 0; i < 8000000; i++) __asm__ ("nop");
}

void usb_rx(char *buf, int *len)
{
  typedef enum
  {
    OP_WR_NONE = 0,
    OP_AD_WR,
    OP_PSG_WR,
    OP_PSG_RES
  } OP;

/*
  int a = 4;
  static int d = 0;
  OP op = OP_WR_NONE;
  static char ib[256];
  static int ibp = 0;
  int reg = 0, dat = 0;

  switch(buf[0])
  {
    case '-':
      d--;
      op = OP_AD_WR;
    break;

    case '+':
      d++;
      op = OP_AD_WR;
    break;

    case '*':
      op = OP_PSG_RES;
    break;

    case '\r':
      op = OP_PSG_WR;
      ib[ibp] = 0;
      ibp = 0;
      sscanf(ib, "%d,%d", &reg, &dat);
    break;

    default:
      ib[ibp++] = buf[0];
    break;
  }

  switch (op)
  {
    case OP_AD_WR:
      ad5206_write(a, d);
      *len = sprintf(buf, "%d\r\n", (u8)d);
    break;

    case OP_PSG_WR:
      reg = (u8)reg;
      dat = (u8)dat;
      psg_write(reg, dat);
      *len = sprintf(buf, "PSG %d,%d\r\n", reg, dat);
    break;

    case OP_PSG_RES:
      psg_res();
    break;
  }
*/
  // *len = sprintf(buf, "%d\r\n", *len);
}

int main()
{
  init_hw();

  usbd_device *usbd_dev;
  usb_rx_cb = usb_rx;
  usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config, usb_strings, 3, usbd_control_buffer, sizeof(usbd_control_buffer));
  usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);

  while (1)
    usbd_poll(usbd_dev);
}
