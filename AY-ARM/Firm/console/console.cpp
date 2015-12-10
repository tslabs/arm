
namespace console
{
#include "console.h"
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

  void print_hdr_ansi()
  {
    print(_RES _C_OFF _BBLACK _CLS _BR_ON _CYAN);
  }

  // 'Press Enter'
  void print_enter(u8 y)
  {
    set_xy(10, y);
    print(_BR_OFF _WHITE "'Enter' - return to main menu");
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
    print(_CR _F(12) _BR_ON _CYAN "Data:");
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

      print((i & 1) ? (" " _BR_ON _BLACK) : (" "  _BR_OFF _WHITE));
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
      _XY(20,5) _BR_OFF _WHITE "Select option:"
      _BR_ON _GREEN _XY(12, 7) _CTAB _TB
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
    print_enter(25);

    print(_XY(12,5) _BR_ON _CYAN "CPUID: " _GREEN);
    u32 ci = SCB_REGS->CPUID;
    print_hex(ci);
    print(_CR _F(13) _CYAN "Implementer: " _YELLOW);
    print(((ci >> 24) == 0x41) ? "ARM" : "unknown");
    print(_CR _F(13) _CYAN "Revision: " _YELLOW "r");
    print_dec((ci >> 20) & 0xF);
    print_char('p');
    print_dec(ci & 0xF);
    print(_CR _F(13) _CYAN "Architecture: " _YELLOW);
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
    print(_CR _F(13) _CYAN "Part No: " _YELLOW);
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

    print(_CR _F(11) _CYAN "UID: " _GREEN);
    print_hex_str(DEVSIG_REGS_UID, 12);

    print(_CR _F(11) _CYAN "Flash size: " _GREEN);
    print_dec(DEVSIG_REGS_FSIZE);
    print("kB");


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
