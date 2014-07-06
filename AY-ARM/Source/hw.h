
// Periferal library definitions
//
// (c) 2014, TS-Labs inc.
// All rights fucked out
//

#pragma once

// --- Macros ---

#define OFF	0
#define ON 	1

// to do: merge with REG_SET_FLD
#define REG_SET_BIT(group, reg, field, val) \
    group -> reg = (group -> reg & ~(group##_##reg##_##field)) | ((val)*(group##_##reg##_##field))

#define REG_SET_FLD(group, reg, field, val) \
    group -> reg = (group -> reg & ~(group##_##reg##_##field)) | ((val)*(group##_##reg##_##field##_0))

#define REGn_SET_BIT(group, num, reg, field, val) \
    group##num -> reg = (group##num -> reg & ~(group##_##reg##_##field)) | ((val)*(group##_##reg##_##field))

#define REGn_SET_FLD(group, num, reg, field, val) \
    group##num -> reg = (group##num -> reg & ~(group##_##reg##_##field)) | ((val)*(group##_##reg##_##field##_0))

// #define REG_Fld_Set(group, num, reg, field, val) \
	// group##num -> reg = (group##num -> reg & ~(group##_##reg##_##field)) | (val)

// #define GPIO_Bit_Set(gp, num, val) \
	// GPIO##gp -> ODR = (GPIO##gp -> ODR & ~((uint32_t)1<<(num))) | ((uint32_t)(val)<<(num))

// #define GPIO_Bit_Read(gp, num) (GPIO##gp -> IDR & ~((uint32_t)1<<(num)))
// #define IO_Bit_Read(group, reg, field) group -> reg & ~ group##_##reg##_##field


//--- GPIO tune ---

// gp - GPIO group:
//   A..K

// num - GPIO pin number:
//   0..15

// md - GPIO mode:
//   GPIO_Mode_IN
//   GPIO_Mode_OUT
//   GPIO_Mode_AF
//   GPIO_Mode_AN

// tp - GPIO type:
//   GPIO_OType_PP
//   GPIO_OType_OD

// sp - GPIO speed:
//   GPIO_Low_Speed
//   GPIO_Medium_Speed
//   GPIO_Fast_Speed
//   GPIO_High_Speed

// pp - GPIO push-pull mode:
//   GPIO_PuPd_NOPULL
//   GPIO_PuPd_UP
//   GPIO_PuPd_DOWN

#define GPIO_Tune(gp, num, md, tp, sp, pp) \
	GPIO_SET(gp, MODER, num, md); \
	GPIO_SET(gp, OTYPER, num, tp); \
	GPIO_SET(gp, OSPEEDR, num, sp); \
	GPIO_SET(gp, PUPDR, num, pp)

#define GPIO_SET(gp, reg, num, md) \
    REGn_SET_FLD(GPIO, gp, reg, reg##num, md)

#define GPIO_AFR(gp, num, af) \
	GPIO##gp -> AFR[(((num)&8)>>3)] = (GPIO##gp -> AFR[(((num)&8)>>3)] & ~((uint32_t)0xF<<(((num)&7)<<2))) | ((uint32_t)(af)<<(((num)&7)<<2))

/*

#define GPIO_Tune8(gp, num, md, tp, sp, pp) \
	GPIO##gp -> MODER = (GPIO##gp -> MODER & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((md)<<14)|((md)<<12)|((md)<<10)|((md)<<8)|((md)<<6)|((md)<<4)|((md)<<2)|(md))<<((num)<<1)); \
	GPIO##gp -> OTYPER = (GPIO##gp -> OTYPER & ~((uint32_t)0xFF<<(num))) \
	| ((uint32_t)(((tp)<<7)|((tp)<<6)|((tp)<<5)|((tp)<<4)|((tp)<<3)|((tp)<<2)|((tp)<<1)|(tp))<<(num)); \
	GPIO##gp -> OSPEEDR = (GPIO##gp -> OSPEEDR & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((sp)<<14)|((sp)<<12)|((sp)<<10)|((sp)<<8)|((sp)<<6)|((sp)<<4)|((sp)<<2)|(sp))<<((num)<<1)); \
	GPIO##gp -> PUPDR = (GPIO##gp -> PUPDR & ~((uint32_t)0xFFFF<<((num)<<1))) \
	| ((uint32_t)(((pp)<<14)|((pp)<<12)|((pp)<<10)|((pp)<<8)|((pp)<<6)|((pp)<<4)|((pp)<<2)|(pp))<<((num)<<1))
*/

// --- Functions prototypes ---
void 	GPIO_Init(void);
