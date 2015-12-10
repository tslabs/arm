
#pragma once

#define __SW(a, b) case a: b; break;

#define print_header(a) { \
  print_hdr_ansi(); \
  set_xy((CONSOLE_WIDTH - sizeof(a)) / 2, 2); \
  print(a); }
