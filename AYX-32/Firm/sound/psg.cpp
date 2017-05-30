// PSG synthesis
//
// AY-ARM project
// (c) TS-Labs
//
// ...

void init_psg()
{
  psg_bus_events.init(psg_bus_events_buf, sizeof(psg_bus_events_buf));

  is_bus_evt = false;

  // +++ PSGCCTRL

  // PSGBCTRL
  psg_chip_num = lim_chip[config.busctr.psgmul];
  selected_psg = 0;

  // PSGACTRL
  amptab_ptr[0] = (u16*)amp_tab_addr[config.ampctr.ampsel0];
  amptab_ptr[1] = (u16*)amp_tab_addr[config.ampctr.ampsel1];
  amptab_ptr[2] = (u16*)amp_tab_addr[config.ampctr.ampsel2];
  amptab_ptr[3] = (u16*)amp_tab_addr[config.ampctr.ampsel3];

  init_psg_generators();

  memset(xlf_buf, 0, sizeof(xlf_buf));
  xlf_acc[0] = xlf_acc[1] = 0;
  xlf_ptr = 0;
}

void init_psg_generators()
{
  for (int a = 0; a < PSG_CHIPS_MAX; a++)
  {
    for (int b = 0; b < 3; b++)
    {
      chan[a][b].amp = 0;
      chan[a][b].is_env = false;
      chan[a][b].is_tone = true;
      chan[a][b].is_noise = true;

      tone[a][b].bit = 0;
      tone[a][b].cnt = 0;
      tone[a][b].period = 0;
    }

    noise[a].cnt = 0;
    noise[a].period = 0;
    noise[a].seed = 1;

    init_envelope(env[a], 16);

    memset(psgvol[a], 0, sizeof(psgvol[0]));
    init_vtab(a, 0, 0, config.psgvol[a][0][0]);
    init_vtab(a, 0, 1, config.psgvol[a][0][1]);
    init_vtab(a, 1, 0, config.psgvol[a][1][0]);
    init_vtab(a, 1, 1, config.psgvol[a][1][1]);
    init_vtab(a, 2, 0, config.psgvol[a][2][0]);
    init_vtab(a, 2, 1, config.psgvol[a][2][1]);
  }
}

void init_vtab(u8 chip, u8 chan, u8 ear, u8 vol)
{
  for (u8 i = 0; i < SIZE_OF_AMPTAB; i++)
  {
    u32 v = amptab_ptr[chip][i] * vol;
    vtab[chip][chan][i].h[ear] = v >> 8;
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
  u16 p = max(1, gen.period);
  u16 c = min(gen.cnt, p);
  bool b = gen.bit;
  u8 *ptr = tone_buf;
  u32 n = snum;
  u32 m;

  if (p == 1)
  {
    while (n >= 8)
    {
      ptr[0] = b;
      ptr[1] = !b;
      ptr[2] = b;
      ptr[3] = !b;
      ptr[4] = b;
      ptr[5] = !b;
      ptr[6] = b;
      ptr[7] = !b;
      ptr += 8;
      n -= 8;
    }

    while (n--)
    {
      *ptr++ = b;
      b = !b;
    }

    c = 0;
  }

  else
  {
    m = min(n, p - c);

    while (n)
    {
      n -= m;
      c += m;

      while (m >= 8)
      {
        ptr[0] = b;
        ptr[1] = b;
        ptr[2] = b;
        ptr[3] = b;
        ptr[4] = b;
        ptr[5] = b;
        ptr[6] = b;
        ptr[7] = b;
        ptr += 8;
        m -= 8;
      }

      while (m--)
        *ptr++ = b;

      m = min(n, p);

      if (c >= p)
      {
        b = !b;
        c = 0;
      }
    }
  }

  gen.cnt = c;
  gen.bit = b;
}

void render_noise(NOISE_GEN &gen)
{
  u16 p = max(2, gen.period);
  u16 c = min(gen.cnt, p);
  u32 s = gen.seed;
  u8 b = s & 1;
  u8 *ptr = noise_buf;
  u32 n = snum;
  u32 m = min(n, p - c);

  if (p < 4)
  {
    while (n)
    {
      n -= m;
      c += m;

      while (m--)
        *ptr++ = b;

      m = min(n, p);

      if (c >= p)
      {
        s = (s >> 1) ^ (b ? 0x14000 : 0);
        b = s & 1;
        c = 0;
      }
    }
  }

  else
  {
    while (n)
    {
      n -= m;
      c += m;

      while (m >= 4)
      {
        ptr[0] = b;
        ptr[1] = b;
        ptr[2] = b;
        ptr[3] = b;
        ptr += 4;
        m -= 4;
      }

      while (m--)
        *ptr++ = b;

      m = min(n, p);

      if (c >= p)
      {
        s = (s >> 1) ^ (b ? 0x14000 : 0);
        b = s & 1;
        c = 0;
      }
    }
  }

  gen.cnt = c;
  gen.seed = s;
}

void render_envelope(ENV_GEN &gen)
{
  u16 p = max(1, gen.period);
  u16 c = min(gen.cnt, p);
  ENV_PHASE ph = gen.phase;
  u8 a = gen.amp;
  u8 *ptr = env_buf;
  u32 n = snum;
  u32 m;

  while (n)
  {
    switch (ph)
    {
      case EF_FALL:
        m = min(n, p - c);

        while (n)
        {
          n -= m;
          c += m;

          while (m >= 8)
          {
            ptr[0] = a;
            ptr[1] = a;
            ptr[2] = a;
            ptr[3] = a;
            ptr[4] = a;
            ptr[5] = a;
            ptr[6] = a;
            ptr[7] = a;
            ptr += 8;
            m -= 8;
          }

          while (m--)
            *ptr++ = a;

          m = min(n, p);

          if (c >= p)
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
        m = min(n, p - c);

        while (n)
        {
          n -= m;
          c += m;

          while (m >= 8)
          {
            ptr[0] = a;
            ptr[1] = a;
            ptr[2] = a;
            ptr[3] = a;
            ptr[4] = a;
            ptr[5] = a;
            ptr[6] = a;
            ptr[7] = a;
            ptr += 8;
            m -= 8;
          }

          while (m--)
            *ptr++ = a;

          m = min(n, p);

          if (c >= p)
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
        while (n >= 8)
        {
          ptr[0] = a;
          ptr[1] = a;
          ptr[2] = a;
          ptr[3] = a;
          ptr[4] = a;
          ptr[5] = a;
          ptr[6] = a;
          ptr[7] = a;
          ptr += 8;
          n -= 8;
        }

        while (n)
        {
          *ptr++ = a;
          n--;
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
void mix_channel(int chip, int chn, int ofs, int opts)
{
  u32 *ptr = &sndbuf[ofs];
  u32 s = vtab[chip][chn][chan[chip][chn].amp].w;
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

void render_psg_generators(int a)
{
  render_noise(noise[a]);
  render_envelope(env[a]);
  render_tone(tone[a][0]);
  mix_channel(a, 0, offs[a], (chan[a][0].is_env << 2) | (chan[a][0].is_noise << 1) | chan[a][0].is_tone);
  render_tone(tone[a][1]);
  mix_channel(a, 1, offs[a], (chan[a][1].is_env << 2) | (chan[a][1].is_noise << 1) | chan[a][1].is_tone);
  render_tone(tone[a][2]);
  mix_channel(a, 2, offs[a], (chan[a][2].is_env << 2) | (chan[a][2].is_noise << 1) | chan[a][2].is_tone);

  offs[a] += snum;
}

u16 xlf_sz = 300;

void apply_dc()
{
  u32 *ptr = sndbuf;

  for (int i = 0; i < DAC_SAMPLES_COUNT; i++)
  {
    SAMP s, f;
    f.w = xlf_buf[xlf_ptr];
    s.w = *ptr;
    xlf_buf[xlf_ptr] = s.w;
    xlf_acc[0] = xlf_acc[0] - f.h[0] + s.h[0];
    xlf_acc[1] = xlf_acc[1] - f.h[1] + s.h[1];

    // s.h[0] = xlf_acc[0] >> 10;
    // s.h[1] = xlf_acc[1] >> 10;
    s.h[0] = s.h[0] - (xlf_acc[0] / xlf_sz) + 0x8000;
    s.h[1] = s.h[1] - (xlf_acc[1] / xlf_sz) + 0x8000;
    *ptr++ = s.w;

    if (++xlf_ptr >= xlf_sz) xlf_ptr = 0;
  }
}

void render_psg()
{
  u16 time = buf_time - DAC_SAMPLES_COUNT;

  for (int i = 0; i < PSG_CHIPS_MAX; i++)
    offs[i] = 0;

  while (1)
  {
    // get new event
    if (!is_bus_evt)
    {
      if (psg_bus_events.used() >= sizeof(bus_evt))
      {
        psg_bus_events.get((u8*)&bus_evt, sizeof(bus_evt));
        is_bus_evt = true;
      }
      else
        break;
    }

    // render until next event and process event
    int bp = bus_evt.time - time;
    if (bp < 0) bp += 65536;

    // event is in scope of current buffer
    if (bp < DAC_SAMPLES_COUNT)
    {
      snum = bp - offs[selected_psg];
      render_psg_generators(selected_psg);
      process_bus_event();
    }

    // event is in scope of next buffer
    else if (bp < (DAC_SAMPLES_COUNT * 2))
      break;

    // event dropped out, kill it
    else
      is_bus_evt = false;
  }

  // render to the end of the buffer
  for (int i = 0; i < psg_chip_num; i++)
  {
    if (snum = DAC_SAMPLES_COUNT - offs[i])
      render_psg_generators(i);
  }

  // apply_dc();
}
