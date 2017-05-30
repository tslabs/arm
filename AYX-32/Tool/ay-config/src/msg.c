
void msg_main()
{
  unlock_chip();

  cls();
  xy(41,1); color(C_HEAD); printf("AYX-32 Configuration Utility");
  defx = 30; xy(defx, 5);
  color(C_BUTN); printf("1. "); color(C_NORM); printf("Device info\n\n");
  color(C_BUTN); printf("2. "); color(C_NORM); printf("Settings\n\n");
  // color(C_BUTN); printf("3. "); color(C_NORM); printf("Test DAC\n\n\");
  printf("\n\n");
  color(C_BUTN); printf("U. "); color(C_NORM); printf("Upload Firmware\n\n");
  color(C_BUTN); printf("R. "); color(C_NORM); printf("Restart device\n\n");
  color(C_BUTN); printf("B. "); color(C_NORM); printf("Restart device in Boot");
}

/// -- Common --
void msg_mode()
{
  color(C_NORM); printf("Mode: ");
  color(C_DATA); printf("%s\n\n", (rd_reg8(R_STATUS) & S_BOOT) ? "Boot" : "Work");
}

void msg_res()
{
  reset_chip();
  unlock_chip();
  fade();
  frame (56, 8, 22, 5, C_FRAM);
  xy(80, 10); color(C_OK); printf("Device restarted");
  xy(96, 12); msg_mode();
}

void msg_boot()
{
  fade();
  frame (32, 6, 30, 3, C_FRAM);
  xy(72, 8); color(C_QUST); printf("Short 'BOOT' jumper");

  while (1)
  {
    reset_chip();
    unlock_chip();
    if (rd_reg8(R_STATUS) & S_BOOT) break;
  }

  fade();
  frame (48, 12, 24, 1, C_FRAM);
  xy(76, 13); color(C_INFO); printf("Device is in Boot");
}

void msg_save()
{
  fade();
  frame (32, 6, 30, 6, C_FRAM);
  xy(60, 8); color(C_QUST); printf("You are about to write");
  xy(70, 9); printf("settings into Flash");
  xy(98, 11); color(C_NORM); printf("Sure? (Y/N)");
}

void msg_save1()
{
  frame (48, 15, 24, 1, C_FRAM);
  xy(74, 16); color(C_INFO); printf("Saving ... ");

  wr_reg32(R_PARAM, MAGIC_CFG);
  wr_reg8(R_CMD, C_SAVE_CFG);
  wait_busy();

  if (rd_reg8(R_ERROR) != E_DONE)
  {
    color(C_ERR); printf("error 0x%02X", rd_reg8(R_ERROR));
    return;
  }
  else
  {
    color(C_OK); printf("ok\n");
  }
}

void msg_fupd()
{
  fade();
  frame (32, 4, 30, 6, C_FRAM);
  xy(60, 6); color(C_QUST); printf("You are about to flash");
  xy(60, 7); color (C_CRIT); printf("FIRMWARE ");
  color (C_QUST); printf("into the chip");
  xy(98, 9); color(C_NORM); printf("Sure? (Y/N)");
}

void msg_fupd1()
{
  FW_HDR *fw = (FW_HDR*)fw_bin;
  u32 fw_size = fw->size + fw->offs;

  fade();
  frame (32, 13, 30, 4, C_FRAM);

  // detect chip
  color(C_NORM);
  printf("Detect: ");

  if (detect_chip())
  {
    color(C_OK); printf("found\n");
  }
  else
  {
    color(C_ERR); printf("not found\n");
    return;
  }

  // check for Boot mode
  color(C_NORM); printf("Check mode: ");

  if (!(rd_reg8(R_STATUS) & S_BOOT))
  {
    color(C_ERR); printf("restart in Boot\n");
    return;
  }
  else
  {
    color(C_OK); printf("boot\n");
  }

  // send F/W Upload command
  color(C_NORM); printf("Send f/w (%ub): ", fw_size);
  wr_reg8(R_CMD, C_BREAK);
  wr_reg32(R_PARAM, fw_size);
  wr_reg8(R_CMD, C_UP_FW);

  if (!(rd_reg8(R_STATUS) & S_DRQ))
  {
    color(C_ERR); printf("DRQ err\n");
    return;
  }

  // send F/W data
  wr_addr(R_DATA);
  wr_arr(fw_bin, fw_size);
  wait_busy();

  if (rd_reg8(R_ERROR) != E_DONE)
  {
    color(C_ERR); printf("error 0x%02X\n", rd_reg8(R_ERROR));
    return;
  }
  else
  {
    color(C_OK); printf("ok\n");
  }

  // send Flash F/W command
  color(C_NORM); printf("Flash f/w: ");
  wr_reg32(R_PARAM, MAGIC_FFW);
  wr_reg8(R_CMD, C_FLASH_FW);
  wait_busy();

  if (rd_reg8(R_ERROR) != E_DONE)
  {
    color(C_ERR); printf("error 0x%02X", rd_reg8(R_ERROR));
    return;
  }
  else
  {
    color(C_OK); printf("ok\n");
  }
}

/// -- Info --
void msg_info1()
{
  u32 f = rd_reg32(R_UPTIME);
  u16 ms;
  u8 s, m, h, d;
  ms = f % 1000;
  f /= 1000;
  s = f % 60;
  f /= 60;
  m = f % 60;
  f /= 60;
  h = f % 24;
  d = f / 24;

  xy(0, 21); color(C_NORM); printf("Uptime: ");
  color(C_DATA); printf("%ud %uh %um %us %ums     ", d, h, m, s, ms);
}

void msg_info()
{
  u32 f;
  u16 f1;
  u8 s[32];
  DEV_VER ver;

  cls();
  xy(90,1); color(C_HEAD); printf("Device Info");

  f1 = rd_reg16(R_DEV_SIG);
  xy(0, 4); color(C_NORM); printf("Device signature: ");
  color(C_DATA); printf("%P\n\n", f1);

  if (f1 != M_DEVSIG)
  {
    color(C_ERR); printf("ERROR: chip not found!");
    return;
  }

  f = rd_reg32(R_CORE_FRQ) / 1000;
  color(C_NORM); printf("Core frequency: ");
  color(C_DATA); printf("%d.%03dMHz\n\n", f / 1000, f % 1000);

  msg_mode();

  rd_reg_arr(R_VER, (u8*)&ver, sizeof(ver));
  color(C_NORM); printf("Hardware version: ");
  color(C_DATA); printf("%u\n", ver.hw);
  color(C_NORM); printf("Firmware version: ");
  color(C_DATA); printf("%u\n", ver.fw);
  color(C_NORM); printf("Config version: ");
  color(C_DATA); printf("%u\n\n", ver.cf);

  rd_reg_str(R_CPR_STR, s, sizeof(s));
  color(C_NORM); printf("Info:  ");
  color(C_DATA); printf("%s\n", s);

  rd_reg_str(R_BLD_STR, s, sizeof(s));
  color(C_NORM); printf("Build: ");
  color(C_DATA); printf("%s", s);

  task = msg_info1;
}

/// -- Settings --
void msg_set()
{
  atb_sel.b = rd_reg8(R_PSG_ACTRL);
  bus_sel.b = rd_reg8(R_PSG_BCTRL);
  clk_sel.b = rd_reg8(R_PSG_CCTRL);

  cls();
  xy(70,1); color(C_HEAD); printf("PSG and WS Settings");
  defx = 16; xy(defx, 4);

  color(C_BUTN); printf("1. "); color(C_NORM); printf("Clock: "); color(C_SEL); printf("%s\n\n", clk_sel_txt[clk_sel.clksel]);
  color(C_BUTN); printf("2. "); color(C_NORM); printf("Multiple PSG mode: "); color(C_SEL); printf("%s\n\n", bus_sel_txt[bus_sel.psgmul]);
  color(C_BUTN); printf("3. "); color(C_NORM); printf("Set Channel Mixer\n\n");
  color(C_BUTN); printf("4. "); color(C_NORM); printf("Set Amplitude Table\n\n");
  color(C_BUTN); printf("5. "); color(C_NORM); printf("Custom Amplitude Table\n\n");

  xy(defx, 20);
  color(C_BUTN); printf("S. "); color(C_NORM); printf("Save settings\n\n");
  color(C_BUTN); printf("Enter. "); color(C_NORM); printf("Exit to main menu");
}

void msg_mix()
{
  u8 i;

  cls();
  xy(90,1); color(C_HEAD); printf("Mixer Settings");
  defx = 16; xy(defx, 4);

  color(C_BUTN); printf("1. "); color(C_NORM); printf("Full stereo\n\n");
  color(C_BUTN); printf("2. "); color(C_NORM); printf("Half stereo\n\n");
  color(C_BUTN); printf("3. "); color(C_NORM); printf("Mono\n\n\n");
  color(C_NORM); printf("Current settings:");

  for (i = 0; i < 4; i++)
  {
    u8 x = i * 54 + 28;
    xy(x, 13); color(C_NORM); printf("PSG %d:", i);
    color(C_SEL);
    wr_reg8(R_PSG_SEL, i);
    xy(x, 14); printf("%d/%d  ", rd_reg8(R_PSG_VOL_AL), rd_reg8(R_PSG_VOL_AR));
    xy(x, 15); printf("%d/%d  ", rd_reg8(R_PSG_VOL_BL), rd_reg8(R_PSG_VOL_BR));
    xy(x, 16); printf("%d/%d  ", rd_reg8(R_PSG_VOL_CL), rd_reg8(R_PSG_VOL_CR));
  }

  xy(defx, 22);
  color(C_BUTN); printf("Enter. "); color(C_NORM); printf("Back");
}

void msg_amp()
{
  u8 i;
  atb_sel.b = rd_reg8(R_PSG_ACTRL);

  cls();
  xy(90,1); color(C_HEAD); printf("AmpTab Settings");
  defx = 16; xy(defx, 4);

  color(C_BUTN); printf("1. "); color(C_NORM); printf("%s\n\n", atab_sel_txt[0]);
  color(C_BUTN); printf("2. "); color(C_NORM); printf("%s\n\n", atab_sel_txt[1]);
  color(C_BUTN); printf("3. "); color(C_NORM); printf("%s\n\n", atab_sel_txt[2]);
  color(C_BUTN); printf("4. "); color(C_NORM); printf("%s\n\n\n", atab_sel_txt[3]);
  color(C_NORM); printf("Current settings:");

  for (i = 0; i < 4; i++)
  {
    xy(defx, i + 15); color(C_NORM); printf("PSG %d: ", i);
    color(C_SEL); printf("%s", atab_sel_txt[(atb_sel.b >> (i * 2)) & 3]);
  }

  xy(defx, 22);
  color(C_BUTN); printf("Enter. "); color(C_NORM); printf("Back");
}

void msg_cust_amp()
{
  u8 i, j, k;

  cls();
  xy(58, 1); color(C_HEAD); printf("Custom amplitude table");

  rd_reg_arr(R_PSG_AMP_TAB, (u8*)c_amp, sizeof(c_amp));

  for (j = 0; j < 16; j++)
    for (k = 0; k < 2; k++)
    {
      i = k * 16 + j;
      xy((k * 100) + 40, j + 4);
      color(C_NORM);
      printf("%2u: ", i);
      color(C_DATA);
      printf("%P", c_amp[i++]);
    }

  xy(16, 22);
  color(C_BUTN); printf("Enter. "); color(C_NORM); printf("Back");
}

void msg_set_b()
{
  bus_sel.psgmul++;
  if ((bus_sel.psgmul == 5) || (bus_sel.psgmul == 6)) bus_sel.psgmul = 7;
  wr_reg8(R_PSG_BCTRL, bus_sel.b);
}

void msg_set_c()
{
  clk_sel.clksel++;
  wr_reg8(R_PSG_CCTRL, clk_sel.b);
}

void msg_mix_1()
{
  set_mix(0);
}

void msg_mix_2()
{
  set_mix(1);
}

void msg_mix_3()
{
  set_mix(2);
}

void msg_amp_1()
{
  atb_sel.ampsel0 = atb_sel.ampsel1 = atb_sel.ampsel2 = atb_sel.ampsel3 = 0;
  wr_reg8(R_PSG_ACTRL, atb_sel.b);
}

void msg_amp_2()
{
  atb_sel.ampsel0 = atb_sel.ampsel1 = atb_sel.ampsel2 = atb_sel.ampsel3 = 1;
  wr_reg8(R_PSG_ACTRL, atb_sel.b);
}

void msg_amp_3()
{
  atb_sel.ampsel0 = atb_sel.ampsel1 = atb_sel.ampsel2 = atb_sel.ampsel3 = 2;
  wr_reg8(R_PSG_ACTRL, atb_sel.b);
}

void msg_amp_4()
{
  atb_sel.ampsel0 = atb_sel.ampsel1 = atb_sel.ampsel2 = atb_sel.ampsel3 = 3;
  wr_reg8(R_PSG_ACTRL, atb_sel.b);
}

/// -- DAC --
void msg_dac1()
{
  static u8 v = 0;
  static bool d = false;
  u8 n = 10;

  wr_reg8(R_DAC_VOL_L, 128 - v);
  wr_reg8(R_DAC_VOL_R, v);

  while (n--)
    play_wav((u8*)wav, sizeof(wav));

  if (!d)
  {
    v++;
    if (v == 128) d = !d;
  }
  else
  {
    v--;
    if (v == 0) d = !d;
  }
}

void msg_dac()
{

  cls();
  xy(104, 1); color(C_HEAD); printf("DAC test");

  task = msg_dac1;
}
