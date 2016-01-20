
// Console
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Was hält uns wach wenn der Himmel brennt

namespace console
{
#include "terminal.cpp"
#include "hw.cpp"

/// - Menu tags ---
 typedef enum
  {
    M_ENTER, M_ENTER0,
    M_DUMP, M_DUMP1,
    M_MAIN, M_MAIN1,
    M_SYSINF
  } MENU;

  enum
  {
    CONSOLE_WIDTH = 80
  };

/// - Types ---

/// - Vars ---
  // common
  u8 menu;
  u8 menu_next;
  u8 menu_st;
  u8 *dump_addr;
  u8 dump_num;

/// - Protos ---

/// - Auxilliary menu functions ---
  void initialize()
  {
    uart_input_mode = UARTIM_NUL;
    menu = M_MAIN;
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
    menu = M_ENTER0;
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
    print(_CR _F(12) _BCYA "Data:");
    print_num_zebra(0, 1, min(dump_num, 16), 21, 3);

    menu_st = 0;
    menu = M_DUMP1;
  }

  void m_dump1()
  {
    if (dump_num--)
    {
      u8 i = menu_st++;

      if (!(i & 0x0F))
        print(_CR _F(18));

      print((i & 1) ? (" " _BBLK) : (" " _WHT));
      print_hex(*dump_addr++);
      i++;
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
               "0. System info"
      // _CR _TAB "1. "
      );

    menu_st = 0;

    kb_enable(-1);
    menu = M_MAIN1;
  }

  void m_main1()
  {
    if (kb_done)
    {
      switch(uart_inbuf[0])
      {
        case '0': menu = M_SYSINF; break;
      }

      kb_enable(-1);
    }
  }

  void m_sysinf()
  {
    print_header("System info");
    print_enter(30);

    u32 ci = SCB_REGS->CPUID;
    print(_XY(12,5) _BCYA "CPUID: " _BGRN "%Y", ci);
    print(_CR _F(13) _BCYA "Implementer: " _BYLW "%s", ((ci >> 24) == 0x41) ? "ARM" : "unknown");
    print(_CR _F(13) _BCYA "Revision: " _BYLW "r%dp%d", (ci >> 20) & 0xF, ci & 0xF);
    
    print(_CR _F(13) _BCYA "Architecture: " _BYLW);
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
    
    print(_CR _F(13) _BCYA "Part No: " _BYLW);
    switch ((ci >> 8) & 0xFF)
    {
      case 0xC2:
        print("Cortex-M");
        print_hex_digit((ci >> 4) & 0xF);
      break;

      case 0xC6:  // 0xC60
        print("Cortex-M0+");
      break;

      default:
        print("unknown");
    }

    print(_CR _F(11) _BCYA "CPU clock: " _BGRN "%dMHz", (u16)(clk::SYSTEM / 1000000));
    print(_CR _F(11) _BCYA "Flash size: " _BGRN "%dkB", DEVSIG_REGS_FSIZE);
    print(_CR _F(11) _BCYA "Device UID: " _BGRN "%H", DEVSIG_REGS_UID, 12);
    
    menu_next = M_MAIN;
    menu = M_ENTER;
  }

// - Menu disp -------------------
  void disp()
  {
    switch(menu)
    {
      __SW(M_ENTER,  m_enter())
      __SW(M_ENTER0, m_enter0())

      __SW(M_DUMP,  m_dump())
      __SW(M_DUMP1, m_dump1())

      __SW(M_MAIN,  m_main())
      __SW(M_MAIN1, m_main1())

      __SW(M_SYSINF, m_sysinf())
    }
  }

#include "interrupts.cpp"
};
