
// Console
//
// AY-ARM project
// (c) TS-Labs
//
// Was hält uns wach wenn der Himmel brennt

enum
{
  CONSOLE_WIDTH = 80
};

/// - Vars ---
TASK menu;       // current recurring menu task
TASK menu_next;  // next menu task
int menu_st;
u8 *dump_addr;
int dump_num;

/// - Strings ---
const char *env_txt[] =
{
  "\\_",  // 0
  "/_",   // 1
  "\\-",  // 2
  "/-",   // 3
  "\\\\", // 4
  "//",   // 5
  "\\/",  // 6
  "/\\"   // 7
};

const u8 env_txt_idx[] = {0, 0, 0, 0, 1, 1, 1, 1, 4, 0, 6, 2, 5, 3, 7, 1};

/// - Protos ---
void m_enter();
void m_enter0();
void m_dump();
void m_dump1();
void m_main();
void m_main1();
void m_sysinf();
void m_psgstat();
void m_psgstat1();
void m_config();

/// - Auxilliary menu functions ---
void initialize()
{
  uart_input_mode = UARTIM_NUL;
  console_uart_in.init(console_uart_inbuf, sizeof(console_uart_inbuf));
  console_uart_out.init(console_uart_outbuf, sizeof(console_uart_outbuf));
  is_sending = false;
  menu = m_main;
}

#define print_header(a) print_hdr((a), ((CONSOLE_WIDTH - sizeof(a)) / 2))
void print_hdr(const char *str, u8 x)
{
  print(_RES _C_OFF _BGBLK _CLS _BCYA);
  set_xy(x, 2);
  print(str);
}

// 'Press Enter'
void print_enter(u8 y)
{
  set_xy(10, y);
  print(_WHT "'Enter' - return to main menu");
}

/// - Auxilliary menus ---
// Wait for 'Enter' key
void m_enter()
{
  kb_enable(-1);
  menu = m_enter0;
}

void m_enter0()
{
  if (kb_done)
  {
    if (uart_inbuf[0] == '\r')
      menu = menu_next;
    else
      kb_enable(-1);
  }
}

// HEX dump
void m_dump()
{
  print(_CR);
  print_num_zebra(0, 1, min(dump_num, 16), 21, 3);

  menu_st = 0;
  menu = m_dump1;
}

void m_dump1()
{
  if (dump_num--)
  {
    u8 i = menu_st++;

    if (!(i & 0x0F))
      print(_CR _F(11) "%s0x%X: ", (i & 0x10) ? _BLU : _BBLU, i);

    print((i & 1) ? (" " _BBLK) : (" " _WHT));
    print("%x", *dump_addr++);
  }
  else
    menu = menu_next;
}

/// - Menus ---
void m_main()
{
  print_header("AY-ARM console");
  print(
    _XY(20,5) _WHT "Select option:"
    _BGRN _XY(12, 7) _CTAB _TB
             "1. System info"
    _CR _TAB "2. PSG debug"
    _CR _TAB "3. Configuration"
    );

  menu_st = 0;

  kb_enable(-1);
  menu = m_main1;
}

void m_main1()
{
  if (kb_done)
  {
    switch(uart_inbuf[0])
    {
      case '1': menu = m_sysinf; break;
      case '2': menu = m_psgstat; break;
      case '3': menu = m_config; break;
    }

    kb_enable(-1);
  }
}

void m_sysinf()
{
  u32 ci = SCB_REGS->CPUID;

  print_header("System info");
  print_enter(30);
  print(_XY(12,5) _BWHT "CPUID: " _BGRN "%y", ci);
  print(_CR _F(13) _BWHT "Implementer: " _BYLW "%s", ((ci >> 24) == 0x41) ? "ARM" : "unknown");
  print(_CR _F(13) _BWHT "Revision: " _BYLW "r%dp%d", (ci >> 20) & 0xF, ci & 0xF);

  print(_CR _F(13) _BWHT "Architecture: " _BYLW);
  switch ((ci >> 16) & 0xF)
  {
    case 0xC:
      print("ARMv6-M");
    break;

    case 0xF:
      print("ARMv7-M");
    break;

    default:
      print("unknown");
  }

  print(_CR _F(13) _BWHT "Part No: " _BYLW);
  switch ((ci >> 8) & 0xFF)
  {
    case 0xC2:
    print("Cortex-M%h", (ci >> 4) & 0xF);
    break;

    case 0xC6:  // 0xC60
      print("Cortex-M0+");
    break;

    default:
      print("unknown");
  }

  print(_CR _F(11) _BWHT "CPU clock: " _BGRN "%dMHz", core_freq / 1000000);
  print(_CR _F(11) _BWHT "Flash size: " _BGRN "%dkB", DEVSIG_REGS_FSIZE);
  print(_CR _F(11) _BWHT "Device UID: " _BGRN "%H", DEVSIG_REGS_UID, 12);

  print(_CR _CR _F(11) _BWHT "Info: " _BGRN "%s", boot_hdrp->cprstring);
  print(_CR _F(11) _BWHT "Build: " _BGRN "%s", boot_hdrp->bldstring);
  print(_CR _F(11) _BWHT "Hardware version: " _BGRN "%d", boot_hdrp->ver.hw);
  print(_CR _F(11) _BWHT "Firmware version: " _BGRN "%d", boot_hdrp->ver.fw);
  print(_CR _F(11) _BWHT "Config version: " _BGRN "%d", boot_hdrp->ver.cf);

  u32 f = time_ms;
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
  print(_CR _CR _F(11) _BWHT "Uptime: " _BGRN "%dd %dh %dm %ds %dms", d, h, m, s, ms);

  menu_next = m_main;
  menu = m_enter;
}

void m_psgstat()
{
  print_header("PSG debug");
  print_enter(30);

  print(_XY(1,4));
  print_num_zebra(0, 1, 16, 22, 3);

  for (int i = 0; i < PSG_CHIPS_MAX; i++)
  {
    bool is_act = i < snd::psg_chip_num;
    set_xy((i * 16) + 11, 11); print(_BWHT "PSG %d: %s", i, is_act ? (_BGRN "Active") : (_BBLK "Idle"));

    if (is_act)
    {
      set_xy((i * 16) + 11, 12); print(_BWHT "Vol A:");
      set_xy((i * 16) + 11, 13); print(_BWHT "Vol B:");
      set_xy((i * 16) + 11, 14); print(_BWHT "Vol C:");
      set_xy((i * 16) + 11, 15); print(_BWHT "Mix A:");
      set_xy((i * 16) + 11, 16); print(_BWHT "Mix B:");
      set_xy((i * 16) + 11, 17); print(_BWHT "Mix B:");
      set_xy((i * 16) + 11, 18); print(_BWHT "Tone A:");
      set_xy((i * 16) + 11, 19); print(_BWHT "Tone B:");
      set_xy((i * 16) + 11, 20); print(_BWHT "Tone C:");
      set_xy((i * 16) + 11, 21); print(_BWHT "Noise:");
      set_xy((i * 16) + 11, 22); print(_BWHT "EnvT:");
      set_xy((i * 16) + 11, 23); print(_BWHT "EnvP:");
    }
  }

  kb_enable(-1);
  menu = m_psgstat1;
}

void print_vol(u8 c, u8 i)
{
  u8 a = bus::psg_readback[c][i + 8];
  if (a & 16)
    print(_BMGN "Env");
  else
    print(_BYLW "%d  ", a & 15);
}

void print_tone(u8 c, u8 i)
{
  u16 a = bus::psg_readback[c][i * 2] + (bus::psg_readback[c][(i * 2) + 1] << 8);
  print(_BYLW "%d   ", a & 4095);
}

void print_mix(u8 c, u8 i)
{
  u16 a = bus::psg_readback[c][7];
  bool t = a & (1 << i);
  bool n = a & (1 << (i + 3));
  print(_BYLW "%sT %sN", t ? (_BBLK) : (_BYLW), n ? (_BBLK) : (_BYLW));
}

void print_noise(u8 c)
{
  u16 a = bus::psg_readback[c][6];
  print(_BYLW "%d ", a & 31);
}

void print_envt(u8 c)
{
  u16 a = bus::psg_readback[c][13];
  print(_BYLW "%s ", env_txt[env_txt_idx[a & 15]]);
}

void print_envp(u8 c)
{
  u16 a = bus::psg_readback[c][11] + (bus::psg_readback[c][12] << 8);
  print(_BYLW "%d   ", a & 4095);
}

void m_psgstat1()
{
  print(_XY(1,4));
  for (int i = 0; i < PSG_CHIPS_MAX; i++)
  {
    bool is_act = i < snd::psg_chip_num;
    print(_CR _F(13) _BWHT "PSG %d: %s%H", i, is_act ? (_BGRN) : (_BBLK), bus::psg_readback[i], 16);
  }

  for (int i = 0; i < PSG_CHIPS_MAX; i++)
  {
    bool is_act = i < snd::psg_chip_num;
    if (is_act)
    {
      set_xy((i * 16) + 19, 12); print_vol(i, 0);
      set_xy((i * 16) + 19, 13); print_vol(i, 1);
      set_xy((i * 16) + 19, 14); print_vol(i, 2);
      set_xy((i * 16) + 19, 15); print_mix(i, 0);
      set_xy((i * 16) + 19, 16); print_mix(i, 1);
      set_xy((i * 16) + 19, 17); print_mix(i, 2);
      set_xy((i * 16) + 19, 18); print_tone(i, 0);
      set_xy((i * 16) + 19, 19); print_tone(i, 1);
      set_xy((i * 16) + 19, 20); print_tone(i, 2);
      set_xy((i * 16) + 19, 21); print_noise(i);
      set_xy((i * 16) + 19, 22); print_envt(i);
      set_xy((i * 16) + 19, 23); print_envp(i);
    }
  }

  if (kb_done)
  {
    switch(uart_inbuf[0])
    {
      case '\r': menu = m_main; break;
    }

    kb_enable(-1);
  }
}

void m_config()
{
  print_header("System info");
  print_enter(30);
  
  print(_XY(10,4) _BRED "Not ready yet...");
  
  menu_next = m_main;
  menu = m_enter;
}
