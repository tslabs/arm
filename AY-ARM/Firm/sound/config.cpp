
// Configuration
//
// AYX-32 project
// (c) TS-Labs
//
// Proč svět je jako kolotoč? A jenom řeknou: "čert ví proč?"

bool save_cfg() __attribute__ ((noinline, section(".ram_text")));
void wait_busy() __attribute__ ((noinline, section(".ram_text")));

bool load_config()
{
  u8 *ptr = (u8*)CONF_ADDR;
  CONFIG *cfg_addr = 0;

  while ((u32)ptr < (CONF_ADDR + CONF_SIZE))
  {
    if (((CONFIG*)ptr)->ver == 0xFFFF)
      break;

    cfg_addr = (CONFIG*)ptr;
    ptr += CONF_PAD_SIZE;
  }

  bool is_valid;
  if (!cfg_addr)
    is_valid = false;
  else if (cfg_addr->ver != CF_VER)
    is_valid = false;
  else
    is_valid = true;

  // load valid config
  if (is_valid)
    memcpy(&config, cfg_addr, sizeof(config));
  // load defaults
  else
  {
    config.ver = CF_VER;
    config.clkctr.b = 0;
    config.busctr.b = 0;
    config.ampctr.b = 0;
    memcpy(config.amptab, amptab_ym, sizeof(config.amptab));
  }
  
  bus::readback[bus::R_PSG_CCTRL] = config.clkctr.b;
  bus::readback[bus::R_PSG_BCTRL] = config.busctr.b;
  bus::readback[bus::R_PSG_ACTRL] = config.ampctr.b;
}

bool save_cfg()
{
  u8 *ptr = (u8*)CONF_ADDR;
  u32 *addr = 0;
  bool rc = false;

  while ((u32)ptr < (CONF_ADDR + CONF_SIZE))
  {
    if (((CONFIG*)ptr)->ver == 0xFFFF)
    {
      addr = (u32*)ptr;
      break;
    }

    ptr += CONF_PAD_SIZE;
  }

  // unlock Flash
  interrupts::setIrqIdle();
  FLASH::unlock();

  // clear config area
  if (!addr)
  {
    FLASH::eraseSector(flash::cr::snb::SECTOR_1);   // erase 0x08004000, 16 kB
    wait_busy();
    addr = (u32*)CONF_ADDR;
  }

  // flash config
  u32 *ptr_buf = (u32*)&config;
  u32 *ptr_fls = addr;
  s32 sz = sizeof(config);

  FLASH::startProgram(flash::cr::psize::PROGRAM_X32);

  while (sz > 0)
  {
    *ptr_fls++ = *ptr_buf++;
    wait_busy();
    sz -= sizeof(u32);
  }

  // lock Flash and reset Caches
  FLASH::lock();
  FLASH::resetInstructionCache();
  FLASH::resetDataCache();
  interrupts::setIrqWork();

  // check Flash
  ptr_buf = (u32*)&config;
  ptr_fls = addr;
  sz = sizeof(config);

  while (sz > 0)
  {
    sz -= sizeof(u32);
    if (*ptr_buf++ != *ptr_fls++)
      goto exit;
  }

  rc = true;

exit:
  return rc;
}

void wait_busy()
{
  while (*(u32 volatile*)(bitband::peripheral<flash::ADDRESS + flash::sr::OFFSET, flash::sr::bsy::POSITION>()));
}
