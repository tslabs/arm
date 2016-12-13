
// Key functions
u8 getkey() __naked
{
  __asm
    ld bc, #0xFEFE
    ld l, #0
  k1:  
    in a, (c)
    ld h, #5
  k2:
    rra
    ret nc
    inc l
    dec h
    jr nz, k2
    rlc b
    jr c, k1
    ret
  __endasm;
}
