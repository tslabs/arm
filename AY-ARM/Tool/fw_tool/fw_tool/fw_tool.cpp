
#include "stdafx.h"

#pragma pack(1)
typedef struct
{
  u8 hdr;       // Header version always 1
  u8 hw;        // H/W version
  u16 ver;      // F/W version
  u16 sver;     //
  u16 offs;     // data offset
  u32 size;     // F/W size
  u16 crc[];    // CRC16 for chunks (one per 256, 768 bytes max)
} FW_HDR;
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
  if (argc != 4)
  {
    printf("Usage: fw_tool.exe <firmware>.bin <f/w version> <f/w sub-version>\n");
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

  FW_HDR *hdr = (FW_HDR*)fw_buf;
  hdr->hdr = 1;
  hdr->hw = 1;
  hdr->ver = _wtoi(argv[2]);
  hdr->sver = _wtoi(argv[3]);
  hdr->offs = ((fsize + 255) >> 8) * sizeof(u16) + sizeof(FW_HDR);
  hdr->size = fsize;

  if (fsize > (sizeof(fw_buf) - hdr->offs))
  {
    printf("Input file is too big!");
    return 4;
  }

  fread(fw_buf + hdr->offs, 1, fsize, f_in);
  fclose(f_in);

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
  sprintf(fo, "ayx32_%d_%d.fw", hdr->ver, hdr->sver);
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
