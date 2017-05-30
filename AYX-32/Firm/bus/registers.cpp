
// Chip registers read/write
//
// AYX-32 project
// (c) TS-Labs
//
// Tiktai tau galbūt šypseną sukels keliom sekundėm

/// Functions
void initialize()
{
  regnum = 0;
#ifndef BOOT
  psg_readback_sel = 0;
#endif
  status.b = STATUS_CLR;
  r_ptr.nul();
  w_ptr.nul();
  rd_ptr.nul();
  wd_ptr.nul();
  nx_task = no_task;
  clear_bg_task();
#ifndef BOOT
  init_vectors();
#else
  init_vectors_ext();
#endif
  
#ifndef BOOT
  readback[R_PSG_CCTRL] = snd::config.clkctr.b;
  readback[R_PSG_BCTRL] = snd::config.busctr.b;
  readback[R_PSG_ACTRL] = snd::config.ampctr.b;

  for (int i = 0; i < PSG_CHIPS_MAX; i++)
  {
    psg_readback[i][R_PSG_VOL_AL] = snd::config.psgvol[i][0][0];
    psg_readback[i][R_PSG_VOL_AR] = snd::config.psgvol[i][0][1];
    psg_readback[i][R_PSG_VOL_BL] = snd::config.psgvol[i][1][0];
    psg_readback[i][R_PSG_VOL_BR] = snd::config.psgvol[i][1][1];
    psg_readback[i][R_PSG_VOL_CL] = snd::config.psgvol[i][2][0];
    psg_readback[i][R_PSG_VOL_CR] = snd::config.psgvol[i][2][1];
  }
#endif
}

// Write AY register address
void write_address(u8 addr)
{
  regnum = addr;
  write_addr_vec[addr]();
}

#ifndef BOOT
// Select AY chip using bit0 (0 - chip1, 1 - chip0) - NedoPC Turbo-AY scheme
void wa_ts()
{
  snd::PSGBCTRL t;
  t.b = readback[R_PSG_BCTRL];
  if (t.psgmul == snd::PMM_TS)
  {
    psg_readback_sel = ~regnum & 1;
    snd::put_bus_evt(R_PSG_SEL, ~regnum & 1);
  }
}

// Custom amplitude table
void wa_amp_tab()
{
  r_ptr.init((u8*)snd::config.amptab, sizeof(snd::config.amptab));
  w_ptr.init((u8*)snd::config.amptab, sizeof(snd::config.amptab));
}

// DAC
void wa_dac_free()
{
  temp_16 = (u16)snd::dac_fifo.free();
  r_ptr.init((u8*)&temp_16, sizeof(temp_16));
  w_ptr.nul();
}

void wa_dac_used()
{
  temp_16 = (u16)snd::dac_fifo.used();
  r_ptr.init((u8*)&temp_16, sizeof(temp_16));
  w_ptr.nul();
}
#endif

// Device signature
void wa_dev_sig()
{
  temp_16 = DEV_SIG;
  r_ptr.init((u8*)&temp_16, sizeof(temp_16));
  w_ptr.nul();
}

// Firmware Version String
void wa_ver()
{
  r_ptr.init((u8*)&boot_hdrp->ver, sizeof(boot_hdrp->ver));
  w_ptr.nul();
}

// Chip Copyright String
void wa_cpr()
{
  r_ptr.init((u8*)boot_hdrp->cprstring, sizeof(boot_hdrp->cprstring));
  w_ptr.nul();
}

// Build String
void wa_bld()
{
  r_ptr.init((u8*)boot_hdrp->bldstring, sizeof(boot_hdrp->bldstring));
  w_ptr.nul();
}

// Core Frequency
void wa_frq()
{
  r_ptr.init((u8*)&core_freq, sizeof(core_freq));
  w_ptr.nul();
}

// Uptime
void wa_upt()
{
  temp_32 = time_ms;
  r_ptr.init((u8*)&temp_32, sizeof(temp_32));
  w_ptr.nul();
}

// Parameters
void wa_param()
{
  r_ptr.init((u8*)resp, sizeof(resp));
  w_ptr.init((u8*)param, sizeof(param));
}

// Data
void wa_data()
{
  r_ptr.init(rd_ptr.addr, rd_ptr.max);
  w_ptr.init(wd_ptr.addr, wd_ptr.max);
}

// Write AY Register
void write_reg(u8 val)
{
  write_reg_vec[regnum](val);
}

#ifndef BOOT
// readback copy
void wr_rb(u8 val)
{
  readback[regnum] = val;
}

// readback copy + event
void wr_rbe(u8 val)
{
  snd::put_bus_evt(regnum, val);
  readback[regnum] = val;
}

// 8 bit
void wr_psg8(u8 val)
{
  psg_readback[psg_readback_sel][regnum] = val;
}

// 8 bit + event
void wr_psg8e(u8 val)
{
  snd::put_bus_evt(regnum, val);
  psg_readback[psg_readback_sel][regnum] = val;
}

// 5 bit + event
void wr_psg5e(u8 val)
{
  snd::put_bus_evt(regnum, val);
  psg_readback[psg_readback_sel][regnum] = val & 0x1F;
}

// 4 bit + event
void wr_psg4e(u8 val)
{
  snd::put_bus_evt(regnum, val);
  psg_readback[psg_readback_sel][regnum] = val & 0x0F;
}

// DAC
void wr_dac_data(u8 val)
{
  snd::dac_fifo.put_byte(val);
}

// Channel A Volume Left
void wr_volal(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][0][0] = val;
}

// Channel A Volume Right
void wr_volar(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][0][1] = val;
}

// Channel B Volume Left
void wr_volbl(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][1][0] = val;
}

// Channel B Volume Right
void wr_volbr(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][1][1] = val;
}

// Channel C Volume Left
void wr_volcl(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][2][0] = val;
}

// Channel C Volume Right
void wr_volcr(u8 val)
{
  wr_psg8e(val);
  snd::config.psgvol[psg_readback_sel][2][1] = val;
}

// PSG select
void wr_psgsel(u8 val)
{
  if (val >= PSG_CHIPS_MAX) val = PSG_CHIPS_MAX - 1;

  wr_rbe(val);
  psg_readback_sel = val;
}

// WS command
void wr_wsc(u8 val)
{
  snd::ws_ext_cmdlist.put_byte(regnum);
  snd::ws_ext_cmdlist.put_byte(val);
}

// WS command + readback
void wr_wscrb(u8 val)
{
  readback[regnum] = val;
  wr_wsc(val);
}
#endif

// Data
void wr_arr_t(u8 val)
{
  w_ptr.write_t(val, nx_task);
}

// Array
void wr_arr(u8 val)
{
  w_ptr.write(val);
}

// Command
void wr_cmd(u8 val)
{
  command(val);
}

// Read AY register
u8 read_reg()
{
  return read_reg_vec[regnum]();
}

#ifndef BOOT
// AY/YM registers
u8 rr_psg()
{
  return psg_readback[psg_readback_sel][regnum];
}

// readback
u8 rr_rb()
{
  return readback[regnum];
}
#endif

// Status
u8 rr_status()
{
  return status.b;
}

// Error
u8 rr_error()
{
  return error;
}

// CPU load
u8 rr_cpuld()
{
#ifndef BOOT
  return cpu_load;
#else
  return 0xFF;
#endif
}

// Array
u8 rr_arr()
{
  return r_ptr.read();
}
