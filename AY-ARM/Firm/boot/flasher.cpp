
// Flasher
//
// AYX-32 project
// (c) TS-Labs
//
// Ջանը ըլի - տուն կըլի:

/// - Types ---
#pragma pack(1)
typedef struct
{
  u16 hdr;      // Header version always 1
  u16 hw;       // H/W version
  u16 fw;       // F/W version
  u16 offs;     // data offset
  u32 size;     // F/W size
  u16 crc[];    // CRC16 for chunks (one per 256, 768 bytes max)
} FW_HDR;
#pragma pack()

/// - Variables ---
u32 raw_size;
u32 size;
bool is_fw_valid;

/// - Functions ---
bool flash_fw() __attribute__ ((noinline, section(".ram_text")));
void wait_busy() __attribute__ ((noinline, section(".ram_text")));

bool initialize()
{
  is_fw_valid = false;
}

bool check_fw()
{
  is_fw_valid = false;

  FW_HDR *hdr = (FW_HDR*)fw_buf;
  u8 *ptr = fw_buf + hdr->offs;
  u32 sz = hdr->size;
  u16 *crc = hdr->crc;

  if (hdr->hdr != FWHDR_VER) goto exit;
  if (hdr->hw != HW_VER) goto exit;
  if (!hdr->size || (hdr->size > (96 * 1024))) goto exit;

  while (sz)
  {
    u32 chsz = min(sz, 256);
    if (crc::calc_crc16(ptr, chsz) != *crc++) goto exit;

    ptr += chsz;
    sz -= chsz;
  }

  is_fw_valid = true;

exit:
  return is_fw_valid;
}

bool flash_fw()
{
  bool rc = false;
  FW_HDR *hdr = (FW_HDR*)fw_buf;

  interrupts::setIrqIdle();

  // unlock Flash
  FLASH::unlock();

  // erase Flash
  FLASH::eraseSector(flash::cr::snb::SECTOR_2);   // erase 0x08008000, 16 kB
  wait_busy();
  FLASH::eraseSector(flash::cr::snb::SECTOR_3);   // erase 0x0800C000, 16 kB
  wait_busy();
  FLASH::eraseSector(flash::cr::snb::SECTOR_4);   // erase 0x08010000, 64 kB
  wait_busy();

  // write Flash
  u32 *ptr_buf = (u32*)(fw_buf + hdr->offs);
  u32 *ptr_fls = (u32*)MAIN_ADDR;
  s32 sz = hdr->size;

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

  // check Flash
  ptr_buf = (u32*)(fw_buf + hdr->offs);
  ptr_fls = (u32*)MAIN_ADDR;
  sz = hdr->size;

  while (sz > 0)
  {
    sz -= sizeof(u32);
    if (*ptr_buf++ != *ptr_fls++)
      goto exit;
  }

  rc = true;

exit:
  interrupts::setIrqWork();
  return rc;
}

void wait_busy()
{
  while (*(u32 volatile*)(bitband::peripheral<flash::ADDRESS + flash::sr::OFFSET, flash::sr::bsy::POSITION>()));
}
