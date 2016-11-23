
#pragma language=extended

#include <stdio.h>
#include <string.h>
// #include <stdlib.h>
#include <intrz80.h>
#include "defs.h"
#include "funcs.c"
#include "msg.c"
#include "menu.c"

int main(void)
{
  border(1); 
  
  while(1)
  {
    msg_disp();
    key_disp();
  }
}
