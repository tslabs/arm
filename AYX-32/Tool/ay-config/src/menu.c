
void menu_disp()
{
  switch (menu)
  {
    case M_MAIN: msg_main(); break;
    case M_INFO: msg_info(); break;
    case M_SET: msg_set(); break;
    case M_SET_B: msg_set_b(); break;
    case M_SET_C: msg_set_c(); break;
    case M_MIX: msg_mix(); break;
    case M_MIX_1: msg_mix_1(); break;
    case M_MIX_2: msg_mix_2(); break;
    case M_MIX_3: msg_mix_3(); break;
    case M_AMP: msg_amp(); break;
    case M_AMP_1: msg_amp_1(); break;
    case M_AMP_2: msg_amp_2(); break;
    case M_AMP_3: msg_amp_3(); break;
    case M_AMP_4: msg_amp_4(); break;
    case M_CUST_AMP: msg_cust_amp(); break;
    case M_DAC: msg_dac(); break;
    case M_RES: msg_res(); break;
    case M_BOOT: msg_boot(); break;
    case M_SAVE: msg_save(); break;
    case M_SAVE1: msg_save1(); break;
    case M_FUPD: msg_fupd(); break;
    case M_FUPD1: msg_fupd1(); break;
  }
}

bool key_disp()
{
  bool rc = false;
  u8 key = getkey();

  if (req_unpress)
  {
    if (key == K_NONE)
      req_unpress = false;
  }
  else switch (menu)
  {
    case M_MAIN:
      switch(key)
      {
        case K_1: menu = M_INFO; rc = true; break;
        case K_2: menu = M_SET; rc = true; break;
        case K_3: menu = M_DAC; rc = true; break;
        case K_U: menu = M_FUPD; rc = true; break;
        case K_R: menu = M_RES; rc = true; break;
        case K_B: menu = M_BOOT; rc = true; break;
      }
    break;

    case M_SET:
      switch(key)
      {
        case K_1: menu = M_SET_C; rc = true; break;
        case K_2: menu = M_SET_B; rc = true; break;
        case K_3: menu = M_MIX; rc = true; break;
        case K_4: menu = M_AMP; rc = true; break;
        case K_5: menu = M_CUST_AMP; rc = true; break;
        case K_EN: menu = M_MAIN; rc = true; break;
        case K_S: menu = M_SAVE; rc = true; break;
      }
    break;

    case M_SET_C:
    case M_SET_B:
      menu = M_SET; rc = true; break;
    break;
    
    case M_MIX:
      switch(key)
      {
        case K_1: menu = M_MIX_1; rc = true; break;
        case K_2: menu = M_MIX_2; rc = true; break;
        case K_3: menu = M_MIX_3; rc = true; break;
        case K_EN: menu = M_SET; rc = true; break;
      }
    break;
    
    case M_AMP:
      switch(key)
      {
        case K_1: menu = M_AMP_1; rc = true; break;
        case K_2: menu = M_AMP_2; rc = true; break;
        case K_3: menu = M_AMP_3; rc = true; break;
        case K_4: menu = M_AMP_4; rc = true; break;
        case K_EN: menu = M_SET; rc = true; break;
      }
    break;

    case M_MIX_1:
    case M_MIX_2:
    case M_MIX_3:
      menu = M_MIX; rc = true;
    break;
      
    case M_AMP_1:
    case M_AMP_2:
    case M_AMP_3:
    case M_AMP_4:
      menu = M_AMP; rc = true;
    break;
    
    case M_CUST_AMP:
      switch(key)
      {
        case K_EN: menu = M_SET; rc = true; break;
      }
    break;

    case M_SAVE:
      switch (key)
      {
        case K_Y: menu = M_SAVE1; rc = true; break;
        case K_N: menu = M_SET; rc = true; break;
      }
    break;

    case M_FUPD:
      switch (key)
      {
        case K_Y: menu = M_FUPD1; rc = true; break;
        case K_N: menu = M_MAIN; rc = true; break;
      }
    break;

    default:
      if (key != K_NONE)
        { menu = M_MAIN; rc = true; break; }
  }

  return rc;
}