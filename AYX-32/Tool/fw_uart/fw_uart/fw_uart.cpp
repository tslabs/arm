
// F/W UART Flasher
//
// AYX-32 project
// (c) TS-Labs
//
// ..

#include "stdafx.h"
#include <commctrl.h>

#pragma pack(1)
#pragma pack()

enum UART_ERR
{
  UE_OK    = 0x00,
  UE_SYNC  = 0x01,
  UE_DATA  = 0x02,
  UE_FLASH = 0x03,
  UE_UNK   = 0xFF,
};

enum
{
  MAGIC_FFW = 0x7841AA55, // 'Flash Firmware' magic
};

_TCHAR* cport;
DWORD dwRead, dwWrite;
HANDLE hPort;

u8 fw_buf[97 * 1024];

u8 uart_in_buf[32];
u8 uart_out_buf[32];

typedef struct
{
  u32 magic;
  u32 size;
} HDR;

//-------------------------------------------------------------------------------------------------------------------

bool rd_uart(u8 *ptr, u32 size, int tout)
{
  while (tout--)
  {
    if (!ReadFile(hPort, ptr, size, &dwRead, NULL))
    {
      printf("UART error!\n");
      return false;
    }
    else
    {
      ptr += dwRead;
      size -= dwRead;
    }

    if (!size)
      break;
  }

  return tout >= 0;
}

bool wr_uart(u8 *ptr, u32 size)
{
  while (size)
  {
    if (!WriteFile(hPort, ptr, size, &dwWrite, NULL))
    {
      printf("UART error!\n");
      return false;
    }
    else
    {
      ptr += dwWrite;
      size -= dwWrite;
    }
  }

  return true;
}

UART_ERR check_err()
{
  if ((uart_in_buf[1] == 0x55) && (uart_in_buf[2] == 0xAA) && (uart_in_buf[3] == 0x78))
    return (UART_ERR)uart_in_buf[0];
  else
    return UE_UNK;
}

bool wait_response(int tout)
{
  if (!rd_uart(uart_in_buf, 4, tout))
  {
    printf("timeout\n");
    return false;
  }
  else if (check_err() == UE_OK)
  {
    printf("OK\n");
    return true;
  }
  else
  {
    printf("error %d\n", uart_in_buf[0]);
    return false;
  }
}

//-------------------------------------------------------------------------------------------------------------------

int _tmain(int argc, _TCHAR* argv[])
{
  if (argc != 3)
  {
    printf("Usage: fw_uart.exe <firmware>.fw com<#>\n");
    return -1;
  }

  hPort = CreateFile (argv[2], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hPort == INVALID_HANDLE_VALUE)
  {
    wprintf(L"Can't open %s\n", argv[2]);
    goto err;
  }

  COMMTIMEOUTS times;
  times.ReadIntervalTimeout = MAXDWORD;
  times.ReadTotalTimeoutMultiplier = 0;
  times.ReadTotalTimeoutConstant = 1;
  times.WriteTotalTimeoutMultiplier = 0;
  times.WriteTotalTimeoutConstant = 1;
  SetCommTimeouts(hPort, &times);

  DCB PortDCB;
  PortDCB.DCBlength = sizeof(DCB);
  PortDCB.fBinary = TRUE;            // Binary mode; no EOF check
  PortDCB.fParity = FALSE;           // No parity checking
  PortDCB.fDsrSensitivity = FALSE;   // DSR sensitivity
  PortDCB.fErrorChar = FALSE;        // Disable error replacement
  PortDCB.fOutxDsrFlow = FALSE;      // No DSR output flow control
  PortDCB.fAbortOnError = FALSE;     // Do not abort reads/writes on error
  PortDCB.fNull = FALSE;             // Disable null stripping
  PortDCB.fTXContinueOnXoff = FALSE; // XOFF continues Tx
  PortDCB.BaudRate = 115200;
  PortDCB.ByteSize = 8;
  PortDCB.Parity = NOPARITY;
  PortDCB.StopBits = ONESTOPBIT;
  PortDCB.fOutxCtsFlow = FALSE;               // No CTS output flow control
  PortDCB.fDtrControl = DTR_CONTROL_DISABLE;  // DTR flow control type
  PortDCB.fOutX = FALSE;                      // No XON/XOFF out flow control
  PortDCB.fInX = FALSE;                       // No XON/XOFF in flow control
  PortDCB.fRtsControl = RTS_CONTROL_DISABLE;  // RTS flow control
  SetCommState (hPort, &PortDCB);

  struct _stat st;
  _wstat(argv[1], &st);
  u32 fsize = st.st_size;

  if (!fsize || (fsize < 0) || fsize > sizeof(fw_buf))
  {
    printf("Input file error!");
    goto err;
  }

  FILE *f_in = _wfopen(argv[1], L"rb");
  if (!f_in)
  {
    printf("Error opening input file for read!");
    goto err;
  }

  fread(fw_buf, 1, fsize, f_in);
  fclose(f_in);

  // initial sync
  printf("Syncing: ");

  uart_out_buf[0] = 'Z';
  int retries = 32;
  while (retries--)
  {
    if (!wr_uart(uart_out_buf, 1))
      goto err;

    if (rd_uart(uart_in_buf, 4, 10))
    {
      if (check_err() == UE_OK)
        break;
      else
      {
        printf("error %d\n", uart_in_buf[0]);
        goto err;
      }
    }
  }

  if (retries < 0)
  {
    printf("timeout\n");
    goto err;
  }
  else
    printf("OK\n");

  // send header
  printf("Sending request: ");

  HDR *hdr = (HDR*)uart_out_buf;
  hdr->magic = MAGIC_FFW;
  hdr->size = fsize;

  if (!wr_uart(uart_out_buf, sizeof(HDR)))
    goto err;
  if (!wait_response(100))
    goto err;

  // send data
  printf("Sending data: ");

  if (!wr_uart(fw_buf, fsize))
    goto err;
  if (!wait_response(5000))
    goto err;

  // flash
  printf("Flashing: ");

  if (!wait_response(10000))
    goto err;

  return 0;
  
err:
  return 1;
}
