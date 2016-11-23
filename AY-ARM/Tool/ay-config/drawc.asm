		module drawc
		public drawc
		RSEG RCODE:CODE

extern	cx
extern	cy
extern	cc
extern	font

; E = symbol code, D = 0
drawc:
		ex de, hl
		ld bc, font
		add hl, hl
		add hl, hl
		add hl, hl
		add hl, bc	; HL = font addr

		ld a, (cy)
		and 0x18
		or 0x40
		ld d, a
		ld a, (cx)
		rrca
		rrca
		rrca
		and 0x1F
		ld e, a
		ld a, (cy)
		and 7
		rrca
		rrca
		rrca
		or e
		ld e, a		; DE = screen addr

		ld b, 8
		ld a, (cx)
		rrca
		rrca
		jr c, draw26
		rrca
		jr c, draw4

		ld c, 3
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
		and 0xC0
		or c
		ld (de), a
		inc hl
		inc d
		djnz draw2

drawc_a0:
		ld a, (cy)
		rrca
		rrca
		rrca
		and 3
		or 0x58
		ld d, a
		ld a, (cc)
		ld (de), a
		ret

draw4:
		ld a, (hl)
		rlca
		rlca
		rlca
		rlca
		and 0x0F
		ld c, a
		ld a, (de)
		and 0xF0
		or c
		ld (de), a
		inc e

		ld a, (hl)
		rlca
		rlca
		rlca
		rlca
		and 0xC0
		ld c, a
		ld a, (de)
		and 0x3F
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
		and 0x03
		ld c, a
		ld a, (de)
		and 0xFC
		or c
		ld (de), a
		inc e

		ld a, (hl)
		rlca
		rlca
		and 0xF0
		ld c, a
		ld a, (de)
		and 0x0F
		or c
		ld (de), a

		dec e
		inc d
		inc hl
		djnz draw6

drawc_a1:
		ld a, (cy)
		rrca
		rrca
		rrca
		and 3
		or 0x58
		ld d, a
		ld a, (cc)
		ld (de), a
		inc e
		ld (de), a
		ret

		endmod
		end
