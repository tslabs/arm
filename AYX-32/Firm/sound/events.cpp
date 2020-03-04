
// Event processing
//
// AYX-32 project
// (c) TS-Labs
//
// বসে আছি একা কাঁচা রোদ বিকেলে উদাস

// Store PSG BUS event
void put_bus_evt(u8 reg, u8 val)
{
  BUS_EVT evt;
  evt.time = EVT_TIM::getCounter();
  evt.reg = reg;
  evt.val = val;
  psg_bus_events.put_word_nocheck(evt.w);
  // psg_bus_events.put((u8*)&evt, sizeof(evt));
}

// Process event
void process_bus_event()
{
  // console::print_debug("%d: %x<-%x\n", bus_evt.time, bus_evt.reg, bus_evt.val);
  is_bus_evt = false;
  event_vec[bus_evt.reg]();
}

// Empty event
void ev_empty() {};

// PSG register 0 - channel A tone period LSB
void ev_ay0()
{
  tone[selected_psg][0].periodl = bus_evt.val;
}

// PSG register 1 - channel A tone period MSB
void ev_ay1()
{
  tone[selected_psg][0].periodh = bus_evt.val & 0x0F;
}

// PSG register 2 - channel B tone period LSB
void ev_ay2()
{
  tone[selected_psg][1].periodl = bus_evt.val;
}

// PSG register 3 - channel B tone period MSB
void ev_ay3()
{
  tone[selected_psg][1].periodh = bus_evt.val & 0x0F;
}

// PSG register 4 - channel C tone period LSB
void ev_ay4()
{
  tone[selected_psg][2].periodl = bus_evt.val;
}

// PSG register 5 - channel C tone period MSB
void ev_ay5()
{
  tone[selected_psg][2].periodh = bus_evt.val & 0x0F;
}

// PSG register 6 - noise period
void ev_ay6()
{
  noise[selected_psg].period = (bus_evt.val & 0x1F) << 1;
}

// PSG register 7 - channel mixer
void ev_ay7()
{
  chan[selected_psg][0].is_tone  = (bus_evt.val &  1) == 0;
  chan[selected_psg][1].is_tone  = (bus_evt.val &  2) == 0;
  chan[selected_psg][2].is_tone  = (bus_evt.val &  4) == 0;
  chan[selected_psg][0].is_noise = (bus_evt.val &  8) == 0;
  chan[selected_psg][1].is_noise = (bus_evt.val & 16) == 0;
  chan[selected_psg][2].is_noise = (bus_evt.val & 32) == 0;
}

// PSG register 8 - channel A volume
void ev_ay8()
{
  if (bus_evt.val & 16)
    chan[selected_psg][0].is_env = true;
  else
  {
    chan[selected_psg][0].is_env = false;
    chan[selected_psg][0].amp = (bus_evt.val & 15) << 1;
  }
}

// PSG register 9 - channel B volume
void ev_ay9()
{
  if (bus_evt.val & 16)
    chan[selected_psg][1].is_env = true;
  else
  {
    chan[selected_psg][1].is_env = false;
    chan[selected_psg][1].amp = (bus_evt.val & 15) << 1;
  }
}

// PSG register 10 - channel C volume
void ev_ay10()
{
  if (bus_evt.val & 16)
    chan[selected_psg][2].is_env = true;
  else
  {
    chan[selected_psg][2].is_env = false;
    chan[selected_psg][2].amp = (bus_evt.val & 15) << 1;
  }
}

// PSG register 11 - envelope generator period LSB
void ev_ay11()
{
  env[selected_psg].periodl = bus_evt.val;
}

// PSG register 12 - envelope generator period MSB
void ev_ay12()
{
  env[selected_psg].periodh = bus_evt.val;
}

// PSG register 13 - envelope type
void ev_ay13()
{
  init_envelope(env[selected_psg], bus_evt.val & 15);
}

// Channel A Volume Left
void ev_volal()
{
  init_vtab(selected_psg, 0, 0, bus_evt.val);
}

// Channel A Volume Right
void ev_volar()
{
  init_vtab(selected_psg, 0, 1, bus_evt.val);
}

// Channel B Volume Left
void ev_volbl()
{
  init_vtab(selected_psg, 1, 0, bus_evt.val);
}

// Channel B Volume Right
void ev_volbr()
{
  init_vtab(selected_psg, 1, 1, bus_evt.val);
}

// Channel C Volume Left
void ev_volcl()
{
  init_vtab(selected_psg, 2, 0, bus_evt.val);
}

// Channel C Volume Right
void ev_volcr()
{
  init_vtab(selected_psg, 2, 1, bus_evt.val);
}

// PSG Chip Select
void ev_psgsel()
{
  selected_psg = (bus_evt.val < PSG_CHIPS_MAX) ? bus_evt.val : (PSG_CHIPS_MAX - 1);
}

// Clock config
void ev_cctrl()
{
  config.clkctr.b = bus_evt.val;
  // +++
}

// Bus config
void ev_bctrl()
{
  config.busctr.b = bus_evt.val;
  psg_chip_num = lim_chip[config.busctr.psgmul];
  selected_psg = 0;
}

// Amplitude table config
void ev_actrl()
{
  config.ampctr.b = bus_evt.val;
  // +++
}
