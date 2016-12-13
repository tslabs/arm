
#include "stdafx.h"

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

typedef struct
{
  void *start_addr;
  u8 cprstring[32];
  u8 bldstring[32];
  struct
  {
    u16 hw;
    u16 fw;
    u16 cf;
  } ver;
} BOOT_HDR;
#pragma pack()

u8 fw_buf[97 * 1024];

//-------------------------------------------------------------------------------------------------------------------

// CRC-CCITT
// Poly: 0x8408
// Initial: 0xFFFF
u16 update_crc16(u16 crc, u8 data)
{
  data ^= (u8)(crc);
  data ^= data << 4;

  return ((((u16)data << 8) | (u8)(crc >> 8)) ^ (u8)(data >> 4) ^ ((u16)data << 3));
}

u16 calc_crc16(u8 *ptr, int num)
{
  u16 crc = 0xFFFF;

  while (num--)
    crc = update_crc16(crc, *ptr++);

  return crc;
}

//-------------------------------------------------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
  if (argc != 2)
  {
    printf("Usage: fw_tool.exe <firmware>.bin\n");
    return 1;
  }

  struct _stat st;
  _wstat(argv[1], &st);
  u32 fsize = st.st_size;

  if (fsize < 0x8001)
  {
    printf("Input file size is wrong!");
    return 2;
  }

  FILE *f_in = _wfopen(argv[1], L"rb");
  if (!f_in)
  {
    printf("Error opening input file for read!");
    return 3;
  }
  fseek(f_in, 0x8000, SEEK_SET);  // hardcoded value for address 0x08008000
  fsize -= 0x8000;

  if (fsize > sizeof(fw_buf))
  {
    printf("Input file is too big!");
    return 4;
  }

  FW_HDR *hdr = (FW_HDR*)fw_buf;
  hdr->offs = (u16)(((fsize + 255) >> 8) * sizeof(u16) + sizeof(FW_HDR));
  fread(fw_buf + hdr->offs, 1, fsize, f_in);
  fclose(f_in);

  BOOT_HDR *bthdr = (BOOT_HDR*)(fw_buf + hdr->offs);
  hdr->hdr = 1;
  hdr->hw = bthdr->ver.hw;
  hdr->fw = bthdr->ver.fw;
  hdr->size = fsize;

  u32 sz = fsize;
  u8 *ptr = fw_buf + hdr->offs;
  u16 *cp = hdr->crc;

  while (sz)
  {
    u16 s = min(sz, 256);
    *cp++ = calc_crc16(ptr, s);
    ptr += s;
    sz -= s;
  }

  char fo[32];
  sprintf(fo, "ayx32_%d_%d.fw", hdr->hw, hdr->fw);
  FILE *f_out = fopen(fo, "wb");
  if (!f_out)
  {
    printf("Error opening output file for write!");
    return 5;
  }

  fwrite(fw_buf, 1, fsize + hdr->offs, f_out);
  fclose(f_out);

  return 0;
}
