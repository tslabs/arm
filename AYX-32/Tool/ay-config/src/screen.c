
#define xy(x, y) cx = (x); cy = (y)
#define color(a) cc = (u8)(a)

// Screen functions
void border(u8 b) __naked
{
  b;  // to avoid warning
  __asm
    ld hl, #2
    add hl, sp
    ld a, (hl)
    out (254), a
    ret
  __endasm;
}

void drawc(u8 c) __naked
{
  c;  // to avoid warning
  __asm
    ld hl, #2
    add hl, sp
    ld l, (hl)
    ld h, #0
    ld bc, #_res_font6x8_bin
    add hl, hl
    add hl, hl
    add hl, hl
    add hl, bc  // HL = font addr

    ld a, (#_cy)
    and #0x18
    or #0x40
    ld d, a
    ld a, (#_cx)
    rrca
    rrca
    rrca
    and #0x1F
    ld e, a
    ld a, (#_cy)
    and #7
    rrca
    rrca
    rrca
    or e
    ld e, a    // DE = screen addr

    ld b, #8
    ld a, (#_cx)
    rrca
    rrca
    jr c, draw26
    rrca
    jr c, draw4

    ld c, #3
  draw0:
    ld a, (de)
    and c
    or (hl)
    ld (de), a
    inc hl
    inc d
    djnz draw0
    jr drawc_a0

  draw26:
    rrca
    jr c, draw6
    
  draw2:
    ld a, (hl)
    rrca
    rrca
    ld c, a
    ld a, (de)
    and #0xC0
    or c
    ld (de), a
    inc hl
    inc d
    djnz draw2

  drawc_a0:
    ld a, (#_cy)
    rrca
    rrca
    rrca
    and #3
    or #0x58
    ld d, a
    ld a, (#_cc)
    ld (de), a
    ret

  draw4:
    ld a, (hl)
    rlca
    rlca
    rlca
    rlca
    and #0x0F
    ld c, a
    ld a, (de)
    and #0xF0
    or c
    ld (de), a
    inc e

    ld a, (hl)
    rlca
    rlca
    rlca
    rlca
    and #0xC0
    ld c, a
    ld a, (de)
    and #0x3F
    or c
    ld (de), a

    dec e
    inc d
    inc hl
    djnz draw4
    jr drawc_a1

  draw6:
    ld a, (hl)
    rlca
    rlca
    and #0x03
    ld c, a
    ld a, (de)
    and #0xFC
    or c
    ld (de), a
    inc e

    ld a, (hl)
    rlca
    rlca
    and #0xF0
    ld c, a
    ld a, (de)
    and #0x0F
    or c
    ld (de), a

    dec e
    inc d
    inc hl
    djnz draw6

  drawc_a1:
    ld a, (#_cy)
    rrca
    rrca
    rrca
    and #3
    or #0x58
    ld d, a
    ld a, (#_cc)
    ld (de), a
    inc e
    ld (de), a
    ret
  __endasm;
}

void cls()
{
  memset((void*)0x4000, 0, 6144);
  memset((void*)0x5800, 0, 768);
  defx = 0;
  xy(0, 0);
}

void fade()
{
  memset((void*)0x5800, 1, 768);
}

void putc(u8 c)
{
  if((c == '\r') || (c == '\n'))
  {
    xy(defx, cy + 1);
    return;
  }

  drawc(c);
  xy(cx + 6, cy);
}

int putchar(int c)
{
  putc(c);
  return 0;
}

void frame(u8 xx, u8 yy, u8 sx, u8 sy, u8 cf)
{
  u8 i, j;
  xy(xx, yy);

  color(cf);

  putc(201);
  for (i = 0; i < sx; i++)
    putc(205);
  putc(187);
  xy(xx, cy + 1);

  for(j = 0; j < sy; j++)
  {
    putc(186);
    for (i = 0; i < sx; i++)
      putc(32);
    putc(186);
    xy(xx, cy + 1);
  }

  putc(200);
  for (i = 0; i < sx; i++)
    putc(205);
  putc(188);
  
  defx = xx + 8;
  xy(defx, yy + 1);
}

void msg(u8 *adr)
{
  u8 c;
  while(c = *adr++)
    putc(c);
}
