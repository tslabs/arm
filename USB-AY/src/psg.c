
void delay()
{
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
  __asm__ ("nop");
}

void ad5206_write(u8 a, u8 d)
{

  (d & 1) ? gpio_clear(GPIOC, GPIO13) : gpio_set(GPIOC, GPIO13);

  gpio_clear(AD_PORT, AD_CS);

  for (int i = 0; i < 3; i++)
  {
    (a & 4) ? gpio_set(AD_PORT, AD_SDI) : gpio_clear(AD_PORT, AD_SDI);
    gpio_set(AD_PORT, AD_CLK);
    gpio_clear(AD_PORT, AD_CLK);
    a <<= 1;
  }

  for (int i = 0; i < 8; i++)
  {
    (d & 128) ? gpio_set(AD_PORT, AD_SDI) : gpio_clear(AD_PORT, AD_SDI);
    gpio_set(AD_PORT, AD_CLK);
    gpio_clear(AD_PORT, AD_CLK);
    d <<= 1;
  }

  gpio_set(AD_PORT, AD_CS);
}

void psg_write(u8 addr, u8 data)
{
  gpio_clear(PSG_DATA_P, GPIO7 | GPIO6 | GPIO5 | GPIO4 | GPIO3 | GPIO2 | GPIO1 | GPIO0);
  gpio_set(PSG_DATA_P, addr);
  gpio_set(PSG_CTRL_P, PSG_ADDR);
  delay();
  gpio_clear(PSG_CTRL_P, PSG_ADDR);

  gpio_clear(PSG_DATA_P, GPIO7 | GPIO6 | GPIO5 | GPIO4 | GPIO3 | GPIO2 | GPIO1 | GPIO0);
  gpio_set(PSG_DATA_P, data);
  gpio_set(PSG_CTRL_P, PSG_WDAT);
  delay();
  gpio_clear(PSG_CTRL_P, PSG_WDAT);
}

void psg_res()
{
  gpio_clear(PSG_CTRL_P, PSG_RES);
  delay();
  delay();
  delay();
  gpio_set(PSG_CTRL_P, PSG_RES);
}
