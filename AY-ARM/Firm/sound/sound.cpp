
// Sound chip
//
// AY-ARM project
// (c) TS-Labs
//
// `Twas brillig and the slithy toves did gyre and gimble in the wabe

#include "sound.hpp"

// runtime initialization
void initialize()
{
  load_config();

  // +++ PSGCCTRL
  
  // PSGBCTRL
  psg_chip_num = lim_chip[config.busctr.psgmul];
  selected_psg_chip = 0;

  // PSGACTRL
  amptab_ptr[0] = (u16*)amp_tab_addr[config.ampctr.ampsel0];
  amptab_ptr[1] = (u16*)amp_tab_addr[config.ampctr.ampsel1];
  amptab_ptr[2] = (u16*)amp_tab_addr[config.ampctr.ampsel2];
  amptab_ptr[3] = (u16*)amp_tab_addr[config.ampctr.ampsel3];

  wschan_num = WS_CH_DEF;

  buf_time = 0;
  is_evt = false;
  req_snd_buf = false;
  init_generators();
  memset(dac_buf, 0, sizeof(dac_buf));
  ay_bus_events.init(ay_bus_events_buf, sizeof(ay_bus_events_buf));
  dac_fifo.init(dac_fifo_buf, sizeof(dac_fifo_buf));
}

void init_generators()
{
  for (int a = 0; a < psg_chip_num; a++)
  {
    for (int b = 0; b < 3; b++)
    {
      chan[a][b].amp = 0;
      chan[a][b].is_env = false;
      chan[a][b].is_tone = true;
      chan[a][b].is_noise = true;

      tone[a][b].bit = 0;
      tone[a][b].cnt = 0;
      tone[a][b].period = 1;
    }

    noise[a].cnt = 0;
    noise[a].period = 1;
    noise[a].seed = 0;

    init_envelope(env[a], 16);

    init_vtab(a, 0, 0, 64);
    init_vtab(a, 0, 1, 0);
    init_vtab(a, 1, 0, 32);
    init_vtab(a, 1, 1, 32);
    init_vtab(a, 2, 0, 0);
    init_vtab(a, 2, 1, 64);
  }
}

void init_vtab(u8 chip, u8 chan, u8 ear, u8 vol)
{
  u16 *ptr = amptab_ptr[chip];
  
  switch (ear)
  {
    case 0:
      for (u8 i = 0; i < SIZE_OF_AMPTAB; i++)
      {
        u32 v = ptr[i] * vol;
        vtab[chip][chan][i].l = v >> 8;
      }
    break;
    
    default:
      for (u8 i = 0; i < SIZE_OF_AMPTAB; i++)
      {
        u32 v = ptr[i] * vol;
        vtab[chip][chan][i].r = v >> 8;
      }
    break;
  }
}

void init_envelope(ENV_GEN &gen, u8 type)
{
  gen.type = type;
  gen.cnt = 0;

  switch (type)
  {
    case 0:
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
    case 11:
      gen.amp = 31;
      gen.phase = EF_FALL;
    break;

    case 4:
    case 5:
    case 6:
    case 7:
    case 12:
    case 13:
    case 14:
    case 15:
      gen.amp = 0;
      gen.phase = EF_RISE;
    break;

    default:
      gen.period = 1;
      gen.type = 0;
      gen.amp = 0;
      gen.phase = EF_STILL;
  }
}

/// Waveform renderers
void render_tone(TONE_GEN &gen)
{
  u16 p = gen.period;
  u16 c = gen.cnt;
  bool b = gen.bit;
  u8 *ptr = tone_buf;
  u32 n = snum;

  switch (p)
  {
    case 0:
    case 1:
      while (n--)
      {
        *ptr++ = b;
        b = !b;
      }

      gen.cnt = 0;
      gen.bit = b;
    break;

    default:
      while (n--)
      {
        *ptr++ = b;

        if (++c >= p)
        {
          c = 0;
          b = !b;
        }
      }

      gen.cnt = c;
      gen.bit = b;
    break;
  }
}

void render_noise(NOISE_GEN &gen)
{
  static u8 parity = 0;

  u16 p = gen.period;
  u16 c = gen.cnt;
  u32 s = gen.seed;
  u8 b = gen.bit;
  u16 *ptr = (u16*)noise_buf;
  u32 n = (snum + 1) / 2;

  // odd pair
  if (parity & 1)
  {
    while (n--)
    {
      u8 a = b;

      if (++c >= p)
      {
        c = 0;
        s = ((s << 1) | 1) ^ (((s >> 16) ^ (s >> 13)) & 1);
        b = (s & (1 << 16)) != 0;
      }

      *ptr++ = (b << 8) | a;
    }
  }
  // even pair
  else
  {
    while (n--)
    {
      *ptr++ = (b << 8) | b;

      if (++c >= p)
      {
        c = 0;
        s = ((s << 1) | 1) ^ (((s >> 16) ^ (s >> 13)) & 1);
        b = (s & (1 << 16)) != 0;
      }
    }
  }

  gen.cnt = c;
  gen.seed = s;
  parity += snum;
}

void render_envelope(ENV_GEN &gen)
{
  u16 p = gen.period;
  u16 c = gen.cnt;
  ENV_PHASE ph = gen.phase;
  u8 a = gen.amp;
  u8 *ptr = env_buf;
  u32 n = snum;

  while (n)
  {
    switch (ph)
    {
      case EF_FALL:
        while (n)
        {
          n--;
          *ptr++ = a;

          if (++c >= p)
          {
            c = 0;

            if (a > 0)
              a--;
            else
            {
              switch (gen.type)
              {
                // F,D
                case 0:
                case 1:
                case 2:
                case 3:
                case 9:
                  ph = EF_STILL;
                break;

                // (F)
                case 8:
                  a = 31;
                break;

                // (F,R)
                case 10:
                case 14:
                  ph = EF_RISE;
                break;

                // F,U
                case 11:
                  a = 31;
                  ph = EF_STILL;
                break;
              }
              goto reiterate;
            }
          }
        }
      break;

      case EF_RISE:
        while (n)
        {
          n--;
          *ptr++ = a;

          if (++c >= p)
          {
            c = 0;

            if (a < 31)
              a++;
            else
            {
              switch (gen.type)
              {
                // R,D
                case 4:
                case 5:
                case 6:
                case 7:
                case 15:
                  a = 0;
                  ph = EF_STILL;
                break;

                // (F,R)
                case 10:
                case 14:
                  ph = EF_FALL;
                break;

                // (R)
                case 12:
                  a = 0;
                break;

                case 13:
                  ph = EF_STILL;
                break;
              }
              goto reiterate;
            }
          }
        }
      break;

      default:
        while (n)
        {
          n--;
          *ptr++ = a;
        }
      break;
    }
  reiterate: ;
  }

  gen.cnt = c;
  gen.phase = ph;
  gen.amp = a;
}

/// Waveform mixer
void mix_channel(u8 chip, u8 chn, u8 opts)
{
  u32 s = vtab[chip][chn][chan[chip][chn].amp].w;
  u32 *ptr = &sndbuf[offs];
  u8 *taddr = tone_buf;
  u8 *naddr = noise_buf;
  u8 *eaddr = env_buf;
  SAMP *v = vtab[chip][chn];
  u32 n = snum;

  switch (opts)
  {
    // DC
    case 0:
      while (n--)
      {
        *ptr += s;
        ptr++;
      }
    break;

    // tone
    case 1:
      while (n--) 
      {
        if (*taddr++) *ptr += s;
        ptr++;
      }
    break;

    // noise
    case 2:
      while (n--)
      {
        if (*naddr++) *ptr += s;
        ptr++;
      }
    break;

    // noise + tone
    case 3:
      while (n--)
      {
        if (*taddr++ && *naddr++) *ptr += s;
        ptr++;
      }
    break;

    // DC + env
    case 4:
      while (n--)
      {
        *ptr += v[*eaddr++].w;
        ptr++;
      }
    break;

    // tone + env
    case 5:
      while (n--)
      {
        if (*taddr++) *ptr += v[*eaddr].w;
        ptr++;
        eaddr++;
      }
    break;

    // noise + env
    case 6:
      while (n--)
      {
        if (*naddr++) *ptr += v[*eaddr].w;
        ptr++;
        eaddr++;
      }
    break;

    // noise + tone + env
    case 7:
      while (n--)
      {
        if (*taddr++ && *naddr++) *ptr += v[*eaddr].w;
        ptr++;
        eaddr++;
      }
    break;
  }
}

void render_generators()
{
  for (int a = 0; a < psg_chip_num; a++)
  {
    if (chan[a][0].is_noise || chan[a][1].is_noise || chan[a][2].is_noise)
      render_noise(noise[a]);
    else
      render_noise(noise[a]); // +++ 'mute' generator
      
    if (chan[a][0].is_env || chan[a][1].is_env || chan[a][2].is_env)
      render_envelope(env[a]);
    else
      render_envelope(env[a]); // +++ 'mute' generator
      
    if (chan[a][0].is_tone)
      render_tone(tone[a][0]);
    else
      render_tone(tone[a][0]); // +++ 'mute' generator
      
    mix_channel(a, 0, (chan[a][0].is_env << 2) | (chan[a][0].is_noise << 1) | chan[a][0].is_tone);
    
    if (chan[a][1].is_tone)
      render_tone(tone[a][1]);
    else
      render_tone(tone[a][1]); // +++ 'mute' generator
    
    mix_channel(a, 1, (chan[a][1].is_env << 2) | (chan[a][1].is_noise << 1) | chan[a][1].is_tone);
    
    if (chan[a][2].is_tone)
      render_tone(tone[a][2]);
    else
      render_tone(tone[a][2]); // +++ 'mute' generator
    
    mix_channel(a, 2, (chan[a][2].is_env << 2) | (chan[a][2].is_noise << 1) | chan[a][2].is_tone);
  }

  offs += snum;
}

// render sound buffer, called after each audio DMA end interrupt
void render_snd_buffer()
{
  u16 time = buf_time - DAC_SAMPLES_COUNT;
  sndbuf = dac_buf[curr_buf];
  offs = 0;

  memset(sndbuf, 0, sizeof(dac_buf[0]));  // +++ use DMA instead

  while (offs < DAC_SAMPLES_COUNT)
  {
    // get new event
    if (!is_evt)
    {
      if (ay_bus_events.used() >= sizeof(evt))
      {
        ay_bus_events.get((u8*)&evt, sizeof(evt));
        is_evt = true;
      }
    }

    // render until next event and process event
    if (is_evt)
    {
      int i = evt.time - time;

      // event is from a skipped buffer: consume it without rendering
      if (i <= 0)
        process_event();

      // event is in scope of current buffer: render until event and then consume it
      else if (i < DAC_SAMPLES_COUNT)
      {
        snum = i - offs;
        render_generators();
        process_event();
      }

      // event falls out of current buffer: render to the end of buffer
      else
      {
        snum = DAC_SAMPLES_COUNT - offs;
        render_generators();
      }
    }
    // no event: render to the end of the buffer
    else
    {
      snum = DAC_SAMPLES_COUNT - offs;
      render_generators();
    }
  }
};
