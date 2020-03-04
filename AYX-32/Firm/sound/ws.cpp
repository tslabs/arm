// WS synthesis
//
// AY-ARM project
// (c) TS-Labs
//
// ...

void init_ws()
{
  ws_int_cmdlist.init(ws_int_cmdlist_buf, sizeof(ws_int_cmdlist_buf));
  ws_ext_cmdlist.init(ws_ext_cmdlist_buf, sizeof(ws_ext_cmdlist_buf));
  ws_events.init(ws_events_buf, sizeof(ws_events_buf));
  dac_fifo.init(dac_fifo_buf, sizeof(dac_fifo_buf));

  is_ws_evt = false;

  ws_chan_num = WS_CHAN_MAX;
  selected_ws = 0;
  selected_samp = 0;

  init_ws_channels();
}

void init_ws_channels()
{
  for (int i = 0; i < WS_CHAN_MAX; i++)
    wschan[i].is_enabled = false;
}

void put_ws_event()
{
  WS_EVT evt;
  evt.time = EVT_TIM::getCounter();
  ws_events.put((u8*)&evt, sizeof(evt));
}

void process_ws_cmdlist(FIFO &cl)
{
  u8 cmd;
  u32 cnt;
  u8 last_cmd = WSC_END_OF_LIST;
  u32 acc;

  while ((cmd = cl.get_byte()) != WSC_END_OF_LIST)
  {
    u8 val = cl.get_byte();

    if (cmd == last_cmd)
      cnt++;
    else
    {
      last_cmd = cmd;
      cnt = 0;
      acc = 0;
    }

    acc += val << (cnt * 8);

    switch (cmd)
    {
      case bus::R_WS_SEL:
        selected_ws = val;
      break;

      case bus::R_WS_CTRL:
      {
        WS_CTRL c;
        c.b = val;

        wschan[selected_ws].is_enabled = c.enabled;

        if (c.restart)
        {
          wschan[selected_ws].addr = wssamp[selected_samp].start_addr;
          // wschan[selected_ws].end_addr = +++
          // wschan[selected_ws].step_i = +++
          // wschan[selected_ws].step_f = +++
          wschan[selected_ws].is_fwd = true;
          wschan[selected_ws].vol[0] = wssamp[selected_samp].vol[0];
          wschan[selected_ws].vol[1] = wssamp[selected_samp].vol[1];
        }
      }
      break;

      case bus::R_WS_VOL_L:
        wschan[selected_ws].vol[0] = val;
      break;

      case bus::R_WS_VOL_R:
        wschan[selected_ws].vol[1] = val;
      break;

      case bus::R_WS_SAMP:
        selected_samp = val;
      break;

      case bus::R_WS_NOTE:
        if (cnt == 2)
        {
          // +++ calculate and set note (acc, 8.16)
        }
      break;

      case bus::R_WS_NOTE_S:
        if (cnt == 1)
        {
          // +++ calculate and set note (acc, 4.12)
        }
      break;
    }
  }
}

void render_ws_channel(u8 a)
{
  if (wschan[a].is_enabled)
  {
    u32 *p = &ws_buf[offs[a]];
    u32 n = snum;

    switch(wschan[a].fmt)
    {
      case M_U8:
        while (n--)
        {
        }
      break;

      case M_S8:
      case S_U8:
      case S_S8:
      case M_U16:
      case M_S16:
      case S_U16:
      case S_S16:
      break;
    }
  }

  offs[a] += snum;
}

void render_dac()
{
  if (!(((DACCTRL*)&bus::readback[bus::R_DAC_CTRL])->en))
    return;

  u8 vl = bus::readback[bus::R_DAC_VOL_L];
  u8 vr = bus::readback[bus::R_DAC_VOL_R];
  u32 *p = ws_buf;
  u16 used = dac_fifo.used();
  u32 n;

  switch (((DACCTRL*)&bus::readback[bus::R_DAC_CTRL])->fmt)
  {
    case M_U8:
      n = min(WS_SAMPLES_COUNT, used);

      while (n--)
      {
        u8 d = dac_fifo.get_byte();
        u16 l = d * vl;
        u16 r = d * vr;
        *p++ = l + (r << 16);
      }
    break;

    case M_S8:
    case S_U8:
    case S_S8:
    case M_U16:
    case M_S16:
    case S_U16:
    case S_S16:
    break;
  }
}

void mix_ws()
{
  u32 *sp = ws_buf;
  u32 *dp = sndbuf;
  u32 n = WS_SAMPLES_COUNT;

  while (n--)
  {
    u32 s = *sp++;
    *dp++ += s;
    *dp++ += s;
    *dp++ += s;
    *dp++ += s;
    *dp++ += s;
  }
}

// Note: to process internal player cmdlist add check for regular timed event (TICK)
void render_ws()
{
  u16 time = buf_time - DAC_SAMPLES_COUNT;

  for (int i = 0; i < ws_chan_num; i++)
    offs[i] = 0;

  // +++ use DMA instead
  memset(ws_buf, 0, sizeof(ws_buf));

  while (1)
  {
    // get new event
    if (!is_ws_evt)
    {
      if (ws_events.used() >= sizeof(ws_evt))
      {
        ws_events.get((u8*)&ws_evt, sizeof(ws_evt));
        is_ws_evt = true;
      }
      else
        break;
    }

    // render until next event and process event
    int bp = ws_evt.time - time;
    if (bp < 0) bp += 65536;
    bp /= DAC_WS_RATIO;

    // event is in scope of current buffer
    if (bp < WS_SAMPLES_COUNT)
    {
      snum = bp - offs[selected_ws];
      render_ws_channel(selected_ws);
      process_ws_cmdlist(ws_ext_cmdlist);
    }

    // event is in scope of next buffer
    else if (bp < (WS_SAMPLES_COUNT * 2))
      break;

    // event dropped out, kill it
    else
      is_bus_evt = false;
  }

  // render to the end of the buffer
  for (int i = 0; i < ws_chan_num; i++)
  {
    if (snum = WS_SAMPLES_COUNT - offs[i])
      render_ws_channel(i);
  }

  render_dac();
  mix_ws();
}
