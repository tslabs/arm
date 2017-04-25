
// Event function vectors
//
// AYX-32 project
// (c) TS-Labs
//
// ..

/// Function vector arrays
// Event
const T_E_VEC t_event_vec[] =
{
  0x00, ev_ay0,
  0x01, ev_ay1,
  0x02, ev_ay2,
  0x03, ev_ay3,
  0x04, ev_ay4,
  0x05, ev_ay5,
  0x06, ev_ay6,
  0x07, ev_ay7,
  0x08, ev_ay8,
  0x09, ev_ay9,
  0x0A, ev_ay10,
  0x0B, ev_ay11,
  0x0C, ev_ay12,
  0x0D, ev_ay13,
  0x10, ev_volal,
  0x11, ev_volar,
  0x12, ev_volbl,
  0x13, ev_volbr,
  0x14, ev_volcl,
  0x15, ev_volcr,
  0xD0, ev_psgsel,
  0xD1, ev_cctrl,
  0xD2, ev_bctrl,
  0xD3, ev_actrl,
};

// Init vectors
void init_vectors()
{
  // set default values
  for (int i = 0; i < countof(event_vec); i++)
    event_vec[i] = ev_empty;
  
  // set function addresses
  for (int i = 0; i < countof(t_event_vec); i++)
    event_vec[t_event_vec[i].num] = t_event_vec[i].addr;
}
