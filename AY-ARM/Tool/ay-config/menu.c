
void msg_disp()
{
  switch(menu)
  {
    case M_MAIN:
      { msg_main(); break; }
    case M_INFO:
      { msg_info(); break; }
    case M_AY:
      { msg_ay(); break; }
    case M_SSG:
      { msg_ssg(); break; }
    case M_SAVE:
      { msg_save(); break; }
    case M_SAVE1:
      { msg_save1(); break; }
    case M_FUPD:
      { msg_fupd(); break; }
    case M_FUPD1:
      { msg_fupd1(); break; }
  }
}

void key_disp()
{
  u8 key;

  while(1)
  {
    key = getkey();
    switch(menu)
    {
      case M_MAIN:
      {
        switch(key)
        {
          case K_0: { menu = M_SAVE; return; }
          case K_1: { menu = M_INFO; return; }
          case K_2: { menu = M_AY; return; }
          case K_3: { menu = M_SSG; return; }
          case K_U: { menu = M_FUPD; return; }
        }
        break;
      }

      case M_INFO:
        { menu = M_MAIN; return; }

      case M_AY:
      {
        menu = M_MAIN; return;
      }

      case M_SSG:
      {
        menu = M_MAIN; return;
      }

      case M_SAVE:
      {
        switch(key)
        {
          case K_Y: { menu = M_SAVE1; return; }
          case K_N: { menu = M_MAIN; return; }
        }
        break;
      }

      case M_SAVE1:
        { menu = M_MAIN; return; }

      case M_FUPD:
      {
        switch(key)
        {
          case K_Y: { menu = M_FUPD1; return; }
          case K_N: { menu = M_MAIN; return; }
        }
        break;
      }

      case M_FUPD1:
        { menu = M_MAIN; return; }
    }
  }
}