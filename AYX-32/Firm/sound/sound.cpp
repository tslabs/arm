
// Sound top-level
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
  init_psg();
  init_ws();
  init_vectors();

  buf_time = 0;
  req_snd_buf = false;
  bus::readback[bus::R_PUSH_UP] = DAC_PUSHUP_DEF;
  memset(dac_buf, 0, sizeof(dac_buf));
}

// render sound buffer, called after each audio DMA end interrupt
void render_snd_buffer()
{
  sndbuf = dac_buf[curr_buf];

  // +++ use DMA instead
  memset(sndbuf, bus::readback[bus::R_PUSH_UP], sizeof(dac_buf[0]));
  // memset(sndbuf, 0, sizeof(dac_buf[0]));

  render_psg();
  // render_ws();
};
