
// Event processing
//
// AYX-32 project
// (c) TS-Labs
//
// বসে আছি একা কাঁচা রোদ বিকেলে উদাস

// store AY BUS event
void put_event(u8 reg, u8 val)
{
  BUS_EVT evt;
  evt.time = EVT_TIM::getCounter();
  evt.reg = reg;
  evt.val = val;
  ay_bus_events.put((u8*)&evt, sizeof(evt));
}

// process event and change state according to it
void process_event()
{
  // console::print("Event %x %x\n", evt.reg, evt.val);
  is_evt = false;

  switch (evt.reg)  // 00-3F
  {
    // AY register 0 - channel A tone period LSB
    case bus::R_PSG_TP_AL:
      tone[current_psg_chip][0].periodl = evt.val;
      if (!tone[current_psg_chip][0].period)
        tone[current_psg_chip][0].period++;
    break;

    // AY register 1 - channel A tone period MSB
    case bus::R_PSG_TP_AH:
      tone[current_psg_chip][0].periodh = evt.val;
      if (!tone[current_psg_chip][0].period)
        tone[current_psg_chip][0].period++;
    break;

    // AY register 2 - channel B tone period LSB
    case bus::R_PSG_TP_BL:
      tone[current_psg_chip][1].periodl = evt.val;
      if (!tone[current_psg_chip][1].period)
        tone[current_psg_chip][1].period++;
    break;

    // AY register 3 - channel B tone period MSB
    case bus::R_PSG_TP_BH:
      tone[current_psg_chip][1].periodh = evt.val;
      if (!tone[current_psg_chip][1].period)
        tone[current_psg_chip][1].period++;
    break;

    // AY register 4 - channel C tone period LSB
    case bus::R_PSG_TP_CL:
      tone[current_psg_chip][2].periodl = evt.val;
      if (!tone[current_psg_chip][2].period)
        tone[current_psg_chip][2].period++;
    break;

    // AY register 5 - channel C tone period MSB
    case bus::R_PSG_TP_CH:
      tone[current_psg_chip][2].periodh = evt.val;
      if (!tone[current_psg_chip][2].period)
        tone[current_psg_chip][2].period++;
    break;

    // AY register 6 - noise period
    case bus::R_PSG_NP:
      noise[current_psg_chip].period = evt.val & 31;
      if (!noise[current_psg_chip].period)
        noise[current_psg_chip].period++;
    break;

    // AY register 7 - channel mixer
    case bus::R_PSG_MX:
      chan[current_psg_chip][0].is_tone  = (evt.val &  1) ? false : true;
      chan[current_psg_chip][1].is_tone  = (evt.val &  2) ? false : true;
      chan[current_psg_chip][2].is_tone  = (evt.val &  4) ? false : true;
      chan[current_psg_chip][0].is_noise = (evt.val &  8) ? false : true;
      chan[current_psg_chip][1].is_noise = (evt.val & 16) ? false : true;
      chan[current_psg_chip][2].is_noise = (evt.val & 32) ? false : true;
    break;

    // AY register 8 - channel A volume
    case bus::R_PSG_V_A:
      if (evt.val & 16)
        chan[current_psg_chip][0].is_env = true;
      else
      {
        chan[current_psg_chip][0].is_env = false;
        chan[current_psg_chip][0].amp = evt.val & 15;
      }
    break;

    // AY register 9 - channel B volume
    case bus::R_PSG_V_B:
      if (evt.val & 16)
        chan[current_psg_chip][1].is_env = true;
      else
      {
        chan[current_psg_chip][0].is_env = false;
        chan[current_psg_chip][1].amp = evt.val & 15;
      }
    break;

    // AY register 10 - channel C volume
    case bus::R_PSG_V_C:
      if (evt.val & 16)
        chan[current_psg_chip][2].is_env = true;
      else
      {
        chan[current_psg_chip][2].is_env = false;
        chan[current_psg_chip][2].amp = evt.val & 15;
      }
    break;

    // AY register 11 - envelope generator period LSB
    case bus::R_PSG_EPL:
      env[current_psg_chip].periodl = evt.val;
      if (!env[current_psg_chip].period)
        env[current_psg_chip].periodl = 1;
    break;

    // AY register 12 - envelope generator period MSB
    case bus::R_PSG_EPH:
      env[current_psg_chip].periodh = evt.val;
      if (!env[current_psg_chip].period)
        env[current_psg_chip].periodl = 1;
    break;

    // AY register 13 - envelope type
    case bus::R_PSG_EC:
      init_envelope(env[current_psg_chip], evt.val & 15);
    break;

    // Channel A Volume Left
    case bus::R_PSG_VOL_AL:
      init_vtab(current_psg_chip, 0, 0, evt.val);
    break;

    // Channel A Volume Right
    case bus::R_PSG_VOL_AR:
      init_vtab(current_psg_chip, 0, 1, evt.val);
    break;

    // Channel B Volume Left
    case bus::R_PSG_VOL_BL:
      init_vtab(current_psg_chip, 1, 0, evt.val);
    break;

    // Channel B Volume Right
    case bus::R_PSG_VOL_BR:
      init_vtab(current_psg_chip, 1, 1, evt.val);
    break;

    // Channel C Volume Left
    case bus::R_PSG_VOL_CL:
      init_vtab(current_psg_chip, 2, 0, evt.val);
    break;

    // Channel C Volume Right
    case bus::R_PSG_VOL_CR:
      init_vtab(current_psg_chip, 2, 1, evt.val);
    break;

    // AY Chip Select
    case bus::R_PSG_SEL:
      current_psg_chip = (evt.val < psg_chip_num) ? evt.val : (psg_chip_num - 1);
    break;
    
    // Clock config
    case bus::R_PSG_CCTRL:
      config.clkctr.b = evt.val;
      // +++
    break;

    // Bus config
    case bus::R_PSG_BCTRL:
      config.busctr.b = evt.val;
      psg_chip_num = lim_chip[config.busctr.psgmul];
      current_psg_chip = 0;
    break;
    
    // Amplitude table config
    case bus::R_PSG_ACTRL:
      config.ampctr.b = evt.val;
      // +++
    break;
  }

  switch (evt.reg)  // 40-7F
  {
  }
}
