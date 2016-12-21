
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

  psg_chip_num = PSG_CHIPS_DEF;
  wschan_num = WS_CH_DEF;

  current_psg_chip = 0;

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
  for (u8 i = 0; i < 16; i++)
  {
    u32 v = amptab_[i] * vol;
    vtab[chip][chan][i][ear] = v >> 8;
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
      gen.amp = 255;
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
      gen.period = 0;
      gen.periodl = 1;
      gen.type = 0;
      gen.amp = 0;
      gen.phase = EF_STILL;
  }
}

/// Waveform renderers
void render_tone(TONE_GEN &gen)
{
  u16 n = 0;

  while (n < snum)
  {
    tone_buf[n++] = gen.bit;

    if (++gen.cnt >= gen.period)
    {
      gen.cnt = 0;
      gen.bit = !gen.bit;
    }
  }
}

void render_noise(NOISE_GEN &gen)
{
  u16 n = 0;

  while (n < snum)
  {
    noise_buf[n++] = gen.bit;

    if (++gen.cnt >= gen.period)
    {
      gen.cnt = 0;
      gen.seed = ((gen.seed << 1) | 1) ^ (((gen.seed >> 16) ^ (gen.seed >> 13)) & 1);
    }
  }
}

void render_envelope(ENV_GEN &gen)
{
  u16 n = 0;

  while (n < snum)
  {
    switch (gen.phase)
    {
      case EF_FALL:
        while (n < snum)
        {
          env_buf[n++] = gen.amp;

          if (++gen.cnt >= gen.period)
          {
            gen.cnt = 0;

            if (gen.amp > 0)
              gen.amp--;
            else
            {
              switch(gen.type)
              {
                // F,D
                case 0:
                case 1:
                case 2:
                case 3:
                case 9:
                  gen.phase = EF_STILL;
                break;

                // (F)
                case 8:
                  gen.amp = 255;
                break;

                // (F,R)
                case 10:
                case 14:
                  gen.phase = EF_RISE;
                break;

                // F,U
                case 11:
                  gen.amp = 255;
                  gen.phase = EF_STILL;
                break;
              }
              break;
            }
          }
        }
      break;

      case EF_RISE:
        while (n < snum)
        {
          env_buf[n++] = gen.amp;

          if (++gen.cnt >= gen.period)
          {
            gen.cnt = 0;

            if (gen.amp < 255)
              gen.amp++;
            else
            {
              switch(gen.type)
              {
                // R,D
                case 4:
                case 5:
                case 6:
                case 7:
                case 15:
                  gen.amp = 0;
                  gen.phase = EF_STILL;
                break;

                // (F,R)
                case 10:
                case 14:
                  gen.phase = EF_FALL;
                break;

                // (R)
                case 12:
                  gen.amp = 0;
                break;

                case 13:
                  gen.phase = EF_STILL;
                break;
              }
              break;
            }
          }
        }
      break;

      case EF_STILL:
        while (n < snum)
          env_buf[n++] = gen.amp;
      break;
    }
  }
}

/// Waveform mixer
void mix_channel(u8 chip, u8 chn, u8 opts)
{
  u8 amp = chan[chip][chn].amp;
  SAMP s;
  s.l = vtab[chip][chn][amp][0];
  s.r = vtab[chip][chn][amp][1];
  u32 *addr = &sndbuf[offs];
  u8 *taddr = tone_buf;
  u8 *naddr = noise_buf;

  switch (opts)
  {
    // DC
    case 0:
    case 4:
      for (u16 n = 0; n < snum; n++)
        *addr++ += s.w;
    break;

    // tone
    case 1:
    case 5:
      for (u16 n = 0; n < snum; n++)
        *addr++ += *taddr++ ? s.w : 0;
    break;

    // noise
    case 2:
    case 6:
      for (u16 n = 0; n < snum; n++)
        *addr++ += *naddr++ ? s.w : 0;
    break;

    // noise + tone
    case 3:
    case 7:
      for (u16 n = 0; n < snum; n++)
        *addr++ += (*taddr++ && *naddr++) ? s.w : 0;
    break;
/*
    // envelope + dc
    case 4:
      for (u16 n = 0; n < snum; n++)
      {
        s.l = vtab[chip][chn][env_buf[n] >> 4][0];
        s.r = vtab[chip][chn][env_buf[n] >> 4][1];
        sndbuf[offs + n] += s.w;
      }
    break;

    // envelope + tone
    case 5:
      for (u16 n = 0; n < snum; n++)
      {
        s.l = vtab[chip][chn][env_buf[n] >> 4][0];
        s.r = vtab[chip][chn][env_buf[n] >> 4][1];
        sndbuf[offs + n] += tone_buf[n] ? s.w : 0;
      }
    break;

    // envelope + noise
    case 6:
      for (u16 n = 0; n < snum; n++)
      {
        s.l = vtab[chip][chn][env_buf[n] >> 4][0];
        s.r = vtab[chip][chn][env_buf[n] >> 4][1];
        sndbuf[offs + n] += noise_buf[n] ? s.w : 0;
      }
    break;

    // envelope + noise + tone
    case 7:
      for (u16 n = 0; n < snum; n++)
      {
        s.l = vtab[chip][chn][env_buf[n] >> 4][0];
        s.r = vtab[chip][chn][env_buf[n] >> 4][1];
        sndbuf[offs + n] += (tone_buf[n] && noise_buf[n]) ? s.w : 0;
      }
    break;
    */
  }
}

void render_generators()
{
  for (int a = 0; a < psg_chip_num; a++)
  {
    render_noise(noise[a]);
    // render_envelope(env[a]);
    render_tone(tone[a][0]);
    mix_channel(a, 0, (chan[a][0].is_env << 2) | (chan[a][0].is_noise << 1) | chan[a][0].is_tone);
    render_tone(tone[a][1]);
    mix_channel(a, 1, (chan[a][1].is_env << 2) | (chan[a][1].is_noise << 1) | chan[a][1].is_tone);
    render_tone(tone[a][2]);
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
    // no event: render to the end of buffer
    else
    {
      snum = DAC_SAMPLES_COUNT - offs;
      render_generators();
    }
  }
};
