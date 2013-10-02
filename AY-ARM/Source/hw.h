
// Periferal library definitions
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//

#pragma once

// --- Macros ---

#define OFF	0
#define ON 	1

#define IO_Bit_Set(reg0, reg1, reg2, val) \
	reg0 -> reg1 = (reg0 -> reg1 & ~(reg0##_##reg1##_##reg2)) | ((val)*(reg0##_##reg1##_##reg2))
// #define IO_Bit_Read(reg0, reg1, reg2) reg0 -> reg1 & ~ reg0##_##reg1##_##reg2

#define GPIO_Bit_Set(gp, num, val) \
	GPIO##gp -> ODR = (GPIO##gp -> ODR & ~((uint32_t)1<<(num))) | ((uint32_t)(val)<<(num))
// #define GPIO_Bit_Read(gp, num) (GPIO##gp -> IDR & ~((uint32_t)1<<(num)))

#define IO_Fld_Set(reg0, num, reg1, reg2, val) \
	reg0##num -> reg1 = (reg0##num -> reg1 & ~(reg0##_##reg1##_##reg2)) | (val)

#define GPIO_MODER(gp, num, md) \
	GPIO##gp -> MODER = (GPIO##gp -> MODER & ~((uint32_t)3<<((num)<<1))) | ((uint32_t)(md)<<((num)<<1))

#define GPIO_OTYPER(gp, num, tp) \
	GPIO##gp -> OTYPER = (GPIO##gp -> OTYPER & ~((uint32_t)1<<(num))) | ((uint32_t)(tp)<<(num))

#define GPIO_OSPEEDR(gp, num, sp) \
	GPIO##gp -> OSPEEDR = (GPIO##gp -> OSPEEDR & ~((uint32_t)3<<((num)<<1))) | ((uint32_t)(sp)<<((num)<<1))

#define GPIO_PUPDR(gp, num, pp) \
	GPIO##gp -> PUPDR = (GPIO##gp -> PUPDR & ~((uint32_t)3<<((num)<<1))) | ((uint32_t)(pp)<<((num)<<1))

#define GPIO_AFR(gp, num, af) \
	GPIO##gp -> AFR[(((num)&8)>>3)] = (GPIO##gp -> AFR[(((num)&8)>>3)] & ~((uint32_t)0xF<<(((num)&7)<<2))) | ((uint32_t)(af)<<(((num)&7)<<2))

#define GPIO_Tune(gp, num, md, tp, sp, pp) \
	GPIO_MODER(gp, num, md); \
	GPIO_OTYPER(gp, num, tp); \
	GPIO_OSPEEDR(gp, num, sp); \
	GPIO_PUPDR(gp, num, pp)

// To Do: split *Tune to separate macros, one per function
#define GPIO_Tune8(gp, num, md, tp, sp, pp) \
	GPIO##gp -> MODER = (GPIO##gp -> MODER & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((md)<<14)|((md)<<12)|((md)<<10)|((md)<<8)|((md)<<6)|((md)<<4)|((md)<<2)|(md))<<((num)<<1)); \
	GPIO##gp -> OTYPER = (GPIO##gp -> OTYPER & ~((uint32_t)0xFF<<(num))) \
	| ((uint32_t)(((tp)<<7)|((tp)<<6)|((tp)<<5)|((tp)<<4)|((tp)<<3)|((tp)<<2)|((tp)<<1)|(tp))<<(num)); \
	GPIO##gp -> OSPEEDR = (GPIO##gp -> OSPEEDR & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((sp)<<14)|((sp)<<12)|((sp)<<10)|((sp)<<8)|((sp)<<6)|((sp)<<4)|((sp)<<2)|(sp))<<((num)<<1)); \
	GPIO##gp -> PUPDR = (GPIO##gp -> PUPDR & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((pp)<<14)|((pp)<<12)|((pp)<<10)|((pp)<<8)|((pp)<<6)|((pp)<<4)|((pp)<<2)|(pp))<<((num)<<1))

// --- Functions prototypes ---
void 	GPIO_Init_My(void);
void 	INT_Init_My(void);
