
void set_mix(u8 mix)
{
  u8 i;

  for (i = 0; i < 4; i++)
  {
    wr_reg8(R_PSG_SEL, i);
    wr_reg8(R_PSG_VOL_AL, mix_sel_val[mix][0]);
    wr_reg8(R_PSG_VOL_AR, mix_sel_val[mix][1]);
    wr_reg8(R_PSG_VOL_BL, mix_sel_val[mix][2]);
    wr_reg8(R_PSG_VOL_BR, mix_sel_val[mix][3]);
    wr_reg8(R_PSG_VOL_CL, mix_sel_val[mix][4]);
    wr_reg8(R_PSG_VOL_CR, mix_sel_val[mix][5]);
  }
}

void play_wav(u8 *w, u16 s)
{
  u16 f;

  while (s)
  {
    f = rd_reg16(R_DAC_FREE);

    // xy(0, 3);
    // color(C_NORM); printf("Free: %d, Used: %d      \n", f, rd_reg16(R_DAC_USED));

    f = min(f, s);
    if (f > 0)
    {
      wr_addr(R_DAC_DATA);
      wr_arr(w, f);
      w += f;
      s -= f;
    }
  }
}

void wait(u16 d)
{
  volatile u16 a = d;
  while (a--);
}
