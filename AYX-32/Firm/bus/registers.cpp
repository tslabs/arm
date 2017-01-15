
// Chip registers read/write
//
// AYX-32 project
// (c) TS-Labs
//
// Tiktai tau galbūt šypseną sukels keliom sekundėm

/// Variables
u8 regnum;        // currently selected chip register
STATUS status;    // status byte
ERR error;        // error code

TASK nx_task;               // next task in command

u8 param[8];    // command parameters
u8 resp[8];     // command response

#ifndef BOOT
u8 psg_readback[PSG_CHIPS_MAX][16]; // readback copy of PSG registers
u8 psg_readback_sel;                // selected PSG chip for readback
u8 readback[256];                   // readback copy of misc registers
#endif

PTR r_ptr;
PTR w_ptr;
PTR rd_ptr;
PTR wd_ptr;

u32 temp_32;
u16 temp_16;

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
}

// Write AY register address
void write_address(u8 addr)
{
  regnum = addr;
#ifndef BOOT
  write_addr_vec[addr]();
#else
  if ((addr >= R_DEV_SIG) && (addr <= R_CORE_FRQ))
    write_addr_vec[addr - R_DEV_SIG]();
#endif
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
    snd::put_event(R_PSG_SEL, ~regnum & 1);
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

// Empty
void wa_(){}

// Write AY Register
void write_reg(u8 val)
{
#ifndef BOOT
  write_reg_vec[regnum](val);
#else
  if ((regnum >= R_DEV_SIG) && (regnum <= R_CORE_FRQ))
    write_reg_vec[regnum - R_DEV_SIG](val);
#endif
}

#ifndef BOOT
// AY/YM registers
// 8 bit
void wr_psg8(u8 val)
{
  psg_readback[psg_readback_sel][regnum] = val;
  snd::put_event(regnum, val);
}

// 5 bit
void wr_psg5(u8 val)
{
  psg_readback[psg_readback_sel][regnum] = val & 0x1F;
  snd::put_event(regnum, val);
}

// 4 bit
void wr_psg4(u8 val)
{
  psg_readback[psg_readback_sel][regnum] = val & 0x0F;
  snd::put_event(regnum, val);
}

// readback copy + event
void wr_rbe(u8 val)
{
  readback[regnum] = val;
  snd::put_event(regnum, val);
}

// DAC
void wr_dac_data(u8 val)
{
  snd::dac_fifo.put_byte(val);
}
#endif

// Data
void wr_data(u8 val)
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

// Empty
void wr_(u8 val){}

// Read AY register
u8 read_reg()
{
#ifndef BOOT
  return read_reg_vec[regnum]();
#else
  if ((regnum >= R_DEV_SIG) && (regnum <= R_CORE_FRQ))
    return read_reg_vec[regnum - R_DEV_SIG]();
  else
    return 0xFF;
#endif
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

// Array
u8 rr_arr()
{
  return r_ptr.read();
}

// Empty
u8 rr_() { return 0xFF; }
