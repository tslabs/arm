
#pragma once

namespace ft8xx
{
  u32 *ccmdb;
  u16 ccmdp;

  // Initialize
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::initialize(const MODE &mode)
  {
    sendCommand(cmd::PWRDOWN);
    sendCommand(cmd::ACTIVE);
    sendCommand(cmd::SLEEP);
    sendCommand(cmd::CLKEXT);
    sendCommand(cmd::CLKSEL, mode.f_mul | 0xC0);
    sendCommand(cmd::ACTIVE);
    while(readReg(reg::ID) != id::CHIP);

    writeReg(reg::HSYNC0 , mode.h_fporch);
    writeReg(reg::HSYNC1 , mode.h_fporch + mode.h_sync);
    writeReg(reg::HOFFSET, mode.h_fporch + mode.h_sync + mode.h_bporch);
    writeReg(reg::HCYCLE , mode.h_fporch + mode.h_sync + mode.h_bporch + mode.h_visible);
    writeReg(reg::HSIZE  , mode.h_visible);
    writeReg(reg::VSYNC0 , mode.v_fporch - 1);
    writeReg(reg::VSYNC1 , mode.v_fporch + mode.v_sync - 1);
    writeReg(reg::VOFFSET, mode.v_fporch + mode.v_sync + mode.v_bporch - 1);
    writeReg(reg::VCYCLE , mode.v_fporch + mode.v_sync + mode.v_bporch + mode.v_visible);
    writeReg(reg::VSIZE  , mode.v_visible);

    writeReg(reg::PCLK, mode.f_div);
    writeReg(reg::PCLK_POL, 0);
    writeReg(reg::CSPREAD, 0);
    writeReg(reg::INT_MASK, interr::SWAP);
    writeReg(reg::INT_EN, 1);
  }

  // Read 32 bit register
  template <spi::Address S, gpio::Address P, u8 N>
  u32 Functions<S, P, N>::readReg(u32 addr)
  {
    u32 val;
    readMem((u8*)&val, addr, sizeof(val));
    return val;
  }

// Write 32 bit register
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::writeReg(u32 addr, u32 val)
  {
    writeMem((u8*)&val, addr, sizeof(val));
  }

  // Read memory
  // Format: 00aaaaaa aaaaaaaa aaaaaaaa 00000000 <byte0> ... <byteN>
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::readMem(u8* buf, u32 addr, u32 num)
  {
    SS::setLow();

    sendByte((addr >> 16) & 0x3F);
    sendByte((addr >> 8) & 0xFF);
    sendByte(addr & 0xFF);
    sendByte(0);

    while (num--)
      *buf++ = sendByte(0);

    SS::setHigh();
  }

  // Write memory
  // Format: 10aaaaaa aaaaaaaa aaaaaaaa <byte0> ... <byteN>
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::writeMem(const u8* buf, u32 addr, u32 num)
  {
    SS::setLow();

    sendByte(((addr >> 16) & 0x3F) | 0x80);
    sendByte((addr >> 8) & 0xFF);
    sendByte(addr & 0xFF);

    while (num--)
      sendByte(*buf++);

    SS::setHigh();
  }

  // Send command
  // Format: 01cccccc 00000000 00000000
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::sendCommand(u8 cmd)
  {
    sendCommand(cmd, 0);
  }

  // Send command
  // Format: 01cccccc pppppppp 00000000
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::sendCommand(u8 cmd, u8 p1)
  {
    SS::setLow();
    sendByte(cmd);
    sendByte(p1);
    sendByte(0);
    SS::setHigh();
  }

  // Send byte via SPI
  template <spi::Address S, gpio::Address P, u8 N>
  u8 Functions<S, P, N>::sendByte(u8 byte)
  {
    while (!SPI::canSendData());
    SPI::sendByte(byte);
    while (!SPI::hasReceivedData());
    return SPI::getByte();
  }

  /// Co-processor functions
  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::cpWait()
  {
    while (readReg(reg::CMDB_SPACE) != 0xFFC);
  }

  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::cpReset()
  {
    writeReg(reg::CPURESET, 1);
    writeReg(reg::CMD_READ, 0);
    writeReg(reg::CMD_WRITE, 0);
    writeReg(reg::CPURESET, 0);
  }

  template <spi::Address S, gpio::Address P, u8 N>
  void Functions<S, P, N>::ccmdWrite()
  {
    writeMem((u8*)ccmdb, reg::CMDB_WRITE, ccmdp << 2);
  }

  template <spi::Address S, gpio::Address P, u8 N>
  bool Functions<S, P, N>::loadCfifo(void *h, u16 s)
  {
    u8 *m = h;
    s = (s + 3) & 0xFFFFFFFC;

    while (s)
    {
      u16 sp = readReg(reg::CMDB_SPACE);
      u16 z = min(sp, s);

      if (sp & 3)
      {
        cpReset();
        return false;
      }

      writeMem(m, reg::CMDB_WRITE, z);

      m += z, s -= z;
    }

    return true;
  }

  void ccmdStart(void *addr)
  {
    ccmdb = (u32*)addr;
    ccmdp = 0;
  }

  void cc(u32 a)
  {
    ccmdb[ccmdp++] = a;
  }

  void cstr(const char *s)
  {
    u8 *p = (u8*)&ccmdb[ccmdp];
    u16 c = 0;

    while ((p[c++] = *s++));

    ccmdp += (c + 3) >> 2;
  }

  /// Co-processor commands
  void SetBitmap(u32 source, u16 fmt, u16 w, u16 h)
  {
    cc(ccmd::SETBITMAP);
    cc(source);
    cc((((u32)w << 16) | (fmt & 0xffff)));
    cc(h);
  }

  void SetScratch(u32 handle)
  {
    cc(ccmd::SETSCRATCH);
    cc(handle);
  }

  void Text(s16 x, s16 y, s16 font, u16 options, const char *s)
  {
    cc(ccmd::TEXT);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)options << 16) | (font & 0xffff)));
    cstr(s);
  }

  void Number(s16 x, s16 y, s16 font, u16 options, s32 n)
  {
    cc(ccmd::NUMBER);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)options << 16) | (font & 0xffff)));
    cc(n);
  }

  void LoadIdentity()
  {
    cc(ccmd::LOADIDENTITY);
  }

  void Toggle(s16 x, s16 y, s16 w, s16 font, u16 options, u16 state, const char *s)
  {
    cc(ccmd::TOGGLE);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)font << 16) | (w & 0xffff)));
    cc((((u32)state << 16) | options));
    cstr(s);
  }

  void Gauge(s16 x, s16 y, s16 r, u16 options, u16 major, u16 minor, u16 val, u16 range)
  {
    cc(ccmd::GAUGE);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)options << 16) | (r & 0xffff)));
    cc((((u32)minor << 16) | (major & 0xffff)));
    cc((((u32)range << 16) | (val & 0xffff)));
  }

  void RegRead(u32 ptr, u32 result)
  {
    cc(ccmd::REGREAD);
    cc(ptr);
    cc(result);
  }

  void VideoStart()
  {
    cc(ccmd::VIDEOSTART);
  }

  void GetProps(u32 ptr, u32 w, u32 h)
  {
    cc(ccmd::GETPROPS);
    cc(ptr);
    cc(w);
    cc(h);
  }

  void Memcpy(u32 dest, u32 src, u32 num)
  {
    cc(ccmd::MEMCPY);
    cc(dest);
    cc(src);
    cc(num);
  }

  void Spinner(s16 x, s16 y, u16 style, u16 scale)
  {
    cc(ccmd::SPINNER);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)scale << 16) | (style & 0xffff)));
  }

  void BgColor(u32 c)
  {
    cc(ccmd::BGCOLOR);
    cc(c);
  }

  void Swap()
  {
    cc(ccmd::SWAP);
  }

  void Inflate(u32 ptr)
  {
    cc(ccmd::INFLATE);
    cc(ptr);
  }

  void Translate(s32 tx, s32 ty)
  {
    cc(ccmd::TRANSLATE);
    cc(tx);
    cc(ty);
  }

  void Stop()
  {
    cc(ccmd::STOP);
  }

  void SetBase(u32 base)
  {
    cc(ccmd::SETBASE);
    cc(base);
  }

  void Slider(s16 x, s16 y, s16 w, s16 h, u16 options, u16 val, u16 range)
  {
    cc(ccmd::SLIDER);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc((((u32)val << 16) | (options & 0xffff)));
    cc(range);
  }

  void VideoFrame(u32 dst, u32 ptr)
  {
    cc(ccmd::VIDEOFRAME);
    cc(dst);
    cc(ptr);
  }

  void TouchTransform(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 tx0, s32 ty0, s32 tx1, s32 ty1, s32 tx2, s32 ty2, u16 result)
  {
    cc(ccmd::TOUCH_TRANSFORM);
    cc(x0);
    cc(y0);
    cc(x1);
    cc(y1);
    cc(x2);
    cc(y2);
    cc(tx0);
    cc(ty0);
    cc(tx1);
    cc(ty1);
    cc(tx2);
    cc(ty2);
    cc(result);
  }

  void Interrupt(u32 ms)
  {
    cc(ccmd::INTERRUPT);
    cc(ms);
  }

  void FgColor(u32 c)
  {
    cc(ccmd::FGCOLOR);
    cc(c);
  }

  void Rotate(s32 a)
  {
    cc(ccmd::ROTATE);
    cc(a);
  }

  void Button(s16 x, s16 y, s16 w, s16 h, s16 font, u16 options, const char *s)
  {
    cc(ccmd::BUTTON);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc((((u32)options << 16) | (font & 0xffff)));
    cstr(s);
  }

  void MemWrite(u32 ptr, u32 num)
  {
    cc(ccmd::MEMWRITE);
    cc(ptr);
    cc(num);
  }

  void Scrollbar(s16 x, s16 y, s16 w, s16 h, u16 options, u16 val, u16 size, u16 range)
  {
    cc(ccmd::SCROLLBAR);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc((((u32)val << 16) | (options & 0xffff)));
    cc((((u32)range << 16) | (size & 0xffff)));
  }

  void GetMatrix(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
  {
    cc(ccmd::GETMATRIX);
    cc(a);
    cc(b);
    cc(c);
    cc(d);
    cc(e);
    cc(f);
  }

  void Sketch(s16 x, s16 y, u16 w, u16 h, u32 ptr, u16 format)
  {
    cc(ccmd::SKETCH);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc(ptr);
    cc(format);
  }

  void RomFont(u32 font, u32 romslot)
  {
    cc(ccmd::ROMFONT);
    cc(font);
    cc(romslot);
  }

  void PlayVideo(u32 options)
  {
    cc(ccmd::PLAYVIDEO);
    cc(options);
  }

  void MemSet(u32 ptr, u32 value, u32 num)
  {
    cc(ccmd::MEMSET);
    cc(ptr);
    cc(value);
    cc(num);
  }

  void GradColor(u32 c)
  {
    cc(ccmd::GRADCOLOR);
    cc(c);
  }

  void Sync()
  {
    cc(ccmd::SYNC);
  }

  void BitmapTransform(s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 tx0, s32 ty0, s32 tx1, s32 ty1, s32 tx2, s32 ty2, u16 result)
  {
    cc(ccmd::BITMAP_TRANSFORM);
    cc(x0);
    cc(y0);
    cc(x1);
    cc(y1);
    cc(x2);
    cc(y2);
    cc(tx0);
    cc(ty0);
    cc(tx1);
    cc(ty1);
    cc(tx2);
    cc(ty2);
    cc(result);
  }

  void Calibrate(u32 result)
  {
    cc(ccmd::CALIBRATE);
    cc(result);
  }

  void SetFont(u32 font, u32 ptr)
  {
    cc(ccmd::SETFONT);
    cc(font);
    cc(ptr);
  }

  void Logo()
  {
    cc(ccmd::LOGO);
  }

  void Append(u32 ptr, u32 num)
  {
    cc(ccmd::APPEND);
    cc(ptr);
    cc(num);
  }

  void MemZero(u32 ptr, u32 num)
  {
    cc(ccmd::MEMZERO);
    cc(ptr);
    cc(num);
  }

  void Scale(s32 sx, s32 sy)
  {
    cc(ccmd::SCALE);
    cc(sx);
    cc(sy);
  }

  void Clock(s16 x, s16 y, s16 r, u16 options, u16 h, u16 m, u16 s, u16 ms)
  {
    cc(ccmd::CLOCK);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)options << 16) | (r & 0xffff)));
    cc((((u32)m << 16) | (h & 0xffff)));
    cc((((u32)ms << 16) | (s & 0xffff)));
  }

  void Gradient(s16 x0, s16 y0, u32 rgb0, s16 x1, s16 y1, u32 rgb1)
  {
    cc(ccmd::GRADIENT);
    cc((((u32)y0 << 16) | (x0 & 0xffff)));
    cc(rgb0);
    cc((((u32)y1 << 16) | (x1 & 0xffff)));
    cc(rgb1);
  }

  void SetMatrix()
  {
    cc(ccmd::SETMATRIX);
  }

  void Track(s16 x, s16 y, s16 w, s16 h, s16 tag)
  {
    cc(ccmd::TRACK);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc(tag);
  }

  void Int_RAMShared(u32 ptr)
  {
    cc(ccmd::INT_RAMSHARED);
    cc(ptr);
  }

  void Int_SWLoadImage(u32 ptr, u32 options)
  {
    cc(ccmd::INT_SWLOADIMAGE);
    cc(ptr);
    cc(options);
  }

  void GetPtr(u32 result)
  {
    cc(ccmd::GETPTR);
    cc(result);
  }

  void Progress(s16 x, s16 y, s16 w, s16 h, u16 options, u16 val, u16 range)
  {
    cc(ccmd::PROGRESS);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc((((u32)val << 16) | (options & 0xffff)));
    cc(range);
  }

  void ColdStart()
  {
    cc(ccmd::COLDSTART);
  }

  void MediaFifo(u32 ptr, u32 size)
  {
    cc(ccmd::MEDIAFIFO);
    cc(ptr);
    cc(size);
  }

  void Keys(s16 x, s16 y, s16 w, s16 h, s16 font, u16 options, const char *s)
  {
    cc(ccmd::KEYS);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
    cc((((u32)options << 16) | (font & 0xffff)));
    cstr(s);
  }

  void Dial(s16 x, s16 y, s16 r, u16 options, u16 val)
  {
    cc(ccmd::DIAL);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)options << 16) | (r & 0xffff)));
    cc(val);
  }

  void Snapshot2(u32 fmt, u32 ptr, s16 x, s16 y, s16 w, s16 h)
  {
    cc(ccmd::SNAPSHOT2);
    cc(fmt);
    cc(ptr);
    cc((((u32)y << 16) | (x & 0xffff)));
    cc((((u32)h << 16) | (w & 0xffff)));
  }

  void LoadImage(u32 ptr, u32 options)
  {
    cc(ccmd::LOADIMAGE);
    cc(ptr);
    cc(options);
  }

  void SetFont2(u32 font, u32 ptr, u32 firstchar)
  {
    cc(ccmd::SETFONT2);
    cc(font);
    cc(ptr);
    cc(firstchar);
  }

  void SetRotate(u32 r)
  {
    cc(ccmd::SETROTATE);
    cc(r);
  }

  void Dlstart()
  {
    cc(ccmd::DLSTART);
  }

  void Snapshot(u32 ptr)
  {
    cc(ccmd::SNAPSHOT);
    cc(ptr);
  }

  void ScreenSaver()
  {
    cc(ccmd::SCREENSAVER);
  }

  void MemCrc(u32 ptr, u32 num, u32 result)
  {
    cc(ccmd::MEMCRC);
    cc(ptr);
    cc(num);
    cc(result);
  }

  /// Display list functions
  void AlphaFunc(u8 func, u8 ref)
  {
    cc((9UL << 24) | ((func & 7L) << 8) | ((ref & 255L) << 0));
  }

  void Begin(u8 prim)
  {
    cc((31UL << 24) | prim);
  }

  void BitmapHandle(u8 handle)
  {
    cc((5UL << 24) | handle);
  }

  void BitmapLayout(u8 format, u16 linestride, u16 height)
  {
    cc((0x28UL << 24) | ((linestride >> 8) & 12L) | ((height >> 9) & 3L));
    cc((7UL << 24) | ((format & 31L) << 19) | ((linestride & 1023L) << 9) | ((height & 511L) << 0));
  }

  void BitmapSize(u8 filter, u8 wrapx, u8 wrapy, u16 width, u16 height)
  {
    u8 fxy = (filter << 2) | (wrapx << 1) | (wrapy);
    cc((0x29UL << 24) | ((width >> 7) & 12L) | ((height>> 9) & 3L));
    cc((8UL << 24) | ((u32)fxy << 18) | ((width & 511L) << 9) | ((height & 511L) << 0));
  }

  void BitmapSource(u32 addr)
  {
    cc((1UL << 24) | ((addr & 0x3FFFFFL) << 0));
  }

  void BitmapTransformA(s32 a)
  {
    cc((21UL << 24) | ((a & 131071L) << 0));
  }

  void BitmapTransformB(s32 b)
  {
    cc((22UL << 24) | ((b & 131071L) << 0));
  }

  void BitmapTransformC(s32 c)
  {
    cc((23UL << 24) | ((c & 16777215L) << 0));
  }

  void BitmapTransformD(s32 d)
  {
    cc((24UL << 24) | ((d & 131071L) << 0));
  }

  void BitmapTransformE(s32 e)
  {
    cc((25UL << 24) | ((e & 131071L) << 0));
  }

  void BitmapTransformF(s32 f)
  {
    cc((26UL << 24) | ((f & 16777215L) << 0));
  }

  void BlendFunc(u8 src, u8 dst)
  {
    cc((11UL << 24) | ((src & 7L) << 3) | ((dst & 7L) << 0));
  }

  void Call(u16 dest)
  {
    cc((29UL << 24) | ((dest & 2047L) << 0));
  }

  void Cell(u8 cell)
  {
    cc((6UL << 24) | ((cell & 127L) << 0));
  }

  void ClearColorA(u8 alpha)
  {
    cc((15UL << 24) | ((alpha & 255L) << 0));
  }

  void ClearColorRGB(u8 red, u8 green, u8 blue)
  {
    cc((2UL << 24) | ((red & 255L) << 16) | ((green & 255L) << 8) | ((blue & 255L) << 0));
  }

  void ClearColorRGB32(u32 rgb)
  {
    cc((2UL << 24) | (rgb & 0xffffffL));
  }

  void Clear(u8 c, u8 s, u8 t)
  {
    u8 m = (c << 2) | (s << 1) | t;
    cc((38UL << 24) | m);
  }

  void ClearAll()
  {
    cc((38UL << 24) | 7);
  }

  void ClearStencil(u8 s)
  {
    cc((17UL << 24) | ((s & 255L) << 0));
  }

  void ClearTag(u8 s)
  {
    cc((18UL << 24) | ((s & 255L) << 0));
  }

  void ColorA(u8 alpha)
  {
    cc((16UL << 24) | ((alpha & 255L) << 0));
  }

  void ColorMask(u8 r, u8 g, u8 b, u8 a)
  {
    cc((32UL << 24) | ((r & 1L) << 3) | ((g & 1L) << 2) | ((b & 1L) << 1) | ((a & 1L) << 0));
  }

  void ColorRGB(u8 red, u8 green, u8 blue)
  {
    cc((4UL << 24) | ((red & 255L) << 16) | ((green & 255L) << 8) | ((blue & 255L) << 0));
  }

  void ColorRGB32(u32 rgb)
  {
    cc((4UL << 24) | (rgb & 0xffffffL));
  }

  void Display(void)
  {
    cc(0UL << 24);
  }

  void End(void)
  {
    cc(33UL << 24);
  }

  void Jump(u16 dest)
  {
    cc((30UL << 24) | ((dest & 2047L) << 0));
  }

  void LineWidth(u16 width)
  {
    cc((14UL << 24) | ((width & 4095L) << 0));
  }

  void Macro(u8 m)
  {
    cc((37UL << 24) | ((m & 1L) << 0));
  }

  void PaletteSource(u32 addr)
  {
    cc((0x2AUL << 24) | ((addr & 0x3FFFFFL) << 0));
  }

  void PointSize(u16 size)
  {
    cc((13UL << 24) | ((size & 8191L) << 0));
  }

  void RestoreContext(void)
  {
    cc(35UL << 24);
  }

  void Return(void)
  {
    cc(36UL << 24);
  }

  void SaveContext(void)
  {
    cc(34UL << 24);
  }

  void ScissorSize(u16 width, u16 height)
  {
    cc((28UL << 24) | ((width & 1023L) << 10) | ((height & 1023L) << 0));
  }

  void ScissorXY(u16 x, u16 y)
  {
    cc((27UL << 24) | ((x & 511L) << 9) | ((y & 511L) << 0));
  }

  void StencilFunc(u8 func, u8 ref, u8 mask)
  {
    cc((10UL << 24) | ((func & 7L) << 16) | ((ref & 255L) << 8) | ((mask & 255L) << 0));
  }

  void StencilMask(u8 mask)
  {
    cc((19UL << 24) | ((mask & 255L) << 0));
  }

  void StencilOp(u8 sfail, u8 spass)
  {
    cc((12UL << 24) | ((sfail & 7L) << 3) | ((spass & 7L) << 0));
  }

  void TagMask(u8 mask)
  {
    cc((20UL << 24) | ((mask & 1L) << 0));
  }

  void Tag(u8 s)
  {
    cc((3UL << 24) | s);
  }

  void Vertex2f(s16 x, s16 y)
  {
    cc((1UL << 30) | ((x & 32767L) << 15) | ((y & 32767L) << 0));
  }

  void Vertex2ii(u16 x, u16 y, u8 handle, u8 cell)
  {
    cc((2UL << 30) | ((x & 511L) << 21) | ((y & 511L) << 12) | ((handle & 31L) << 7) | ((cell & 127L) << 0));
  }

  void VertexFormat(u8 f)
  {
    cc((0x27UL << 24) | (f & 7L));
  }

  void VertexTranslateX(s32 v)
  {
    cc((0x2BUL << 24) | ((u32)v & 0x1FFFF));
  }

  void VertexTranslateY(s32 v)
  {
    cc((0x2CUL << 24) | ((u32)v & 0x1FFFF));
  }

  /// Math functions
  // >>> [s16(65535*math.sin(math.pi * 2 * i / 1024)) for i in range(257)]
  const u16 sintab[257] =
  {
  0,     402,   804,   1206,  1608,  2010,  2412,  2813,  3215,  3617,  4018,  4419,  4821,  5221,  5622,  6023,
  6423,  6823,  7223,  7622,  8022,  8421,  8819,  9218,  9615,  10013, 10410, 10807, 11203, 11599, 11995, 12390,
  12785, 13179, 13573, 13966, 14358, 14750, 15142, 15533, 15923, 16313, 16702, 17091, 17479, 17866, 18252, 18638,
  19023, 19408, 19791, 20174, 20557, 20938, 21319, 21699, 22078, 22456, 22833, 23210, 23585, 23960, 24334, 24707,
  25079, 25450, 25820, 26189, 26557, 26924, 27290, 27655, 28019, 28382, 28744, 29105, 29465, 29823, 30181, 30537,
  30892, 31247, 31599, 31951, 32302, 32651, 32999, 33346, 33691, 34035, 34378, 34720, 35061, 35400, 35737, 36074,
  36409, 36742, 37075, 37406, 37735, 38063, 38390, 38715, 39039, 39361, 39682, 40001, 40319, 40635, 40950, 41263,
  41574, 41885, 42193, 42500, 42805, 43109, 43411, 43711, 44010, 44307, 44603, 44896, 45189, 45479, 45768, 46055,
  46340, 46623, 46905, 47185, 47463, 47739, 48014, 48287, 48558, 48827, 49094, 49360, 49623, 49885, 50145, 50403,
  50659, 50913, 51165, 51415, 51664, 51910, 52155, 52397, 52638, 52876, 53113, 53347, 53580, 53810, 54039, 54265,
  54490, 54712, 54933, 55151, 55367, 55581, 55793, 56003, 56211, 56416, 56620, 56821, 57021, 57218, 57413, 57606,
  57796, 57985, 58171, 58355, 58537, 58717, 58894, 59069, 59242, 59413, 59582, 59748, 59912, 60074, 60234, 60391,
  60546, 60699, 60849, 60997, 61143, 61287, 61428, 61567, 61704, 61838, 61970, 62100, 62227, 62352, 62474, 62595,
  62713, 62828, 62941, 63052, 63161, 63267, 63370, 63472, 63570, 63667, 63761, 63853, 63942, 64029, 64114, 64196,
  64275, 64353, 64427, 64500, 64570, 64637, 64702, 64765, 64825, 64883, 64938, 64991, 65042, 65090, 65135, 65178,
  65219, 65257, 65293, 65326, 65357, 65385, 65411, 65435, 65456, 65474, 65490, 65504, 65515, 65523, 65530, 65533,
  65535
  };

  s16 rsin(s16 r, u16 th)
  {
    s16 th4;
    u16 s;
    s16 p;

    th >>= 6; // angle 0-123
    th4 = th & 511;
    if (th4 & 256) th4 = 512 - th4; // 256->256 257->255, etc
    s = sintab[th4];
    p = ((u32)s * r) >> 16;
    if (th & 512) p = -p;
    return p;
  }

  s16 rcos(s16 r, u16 th)
  {
    return rsin(r, th + 0x4000);
  }
}  // namespace ft8xx
