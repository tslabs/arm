
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "../res/font6x8.h"
const
#include "../res/fw.h"
#include "screen.c"
#include "ayx32.c"
#include "kbd.c"
#include "func.c"
#include "msg.c"
#include "menu.c"

void t_none() {}

void crt_init()
{
  menu = M_MAIN;
}

void main()
{
  crt_init();

  border(1);

  while (1)
  {
    req_unpress = true;
    task = t_none;
    menu_disp();

    while (1)
    {
      task();
      if (key_disp())
        break;
    }
  }
}
