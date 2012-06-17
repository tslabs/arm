
// ZX interface definition file
//
// (c) 2012, TS-Labs inc.
// All rights fucked out
//
// `Twas brillig and the slithy toves


#include "types.h"

#ifndef __ZX_H
#define __ZX_H


// --- Commands -----
#define	CMD_BRK			(U8) 0x00


// --- Error codes -----
#define	CMD_ERR_OK		(U8) 0x00
#define	CMD_ERR_ILG		(U8) 0x01
#define	CMD_ERR_BRK		(U8) 0x02
#define	CMD_ERR_MOD		(U8) 0x03
#define	CMD_ERR_PAR		(U8) 0x04


// --- Structures definition -----
typedef union {
	struct {
		U8	busy:1;		// Command in progress
		U8	drq:1;		// Data Request. Byte should be sent to chip
		U8	drd:1;		// Data Ready. Byte should be read from chip
		U8	ndat:1;		// No data to transfer
		U8	brk:1;		// Command interrupted
		U8	cmp:1;		// Command completed
		U8	acc:1;		// Command accepted
		U8	err:1;		// Error occured
	} i;
	U8 b;
} Stat;


// --- Functions prototypes -----
void	WReg(U8);
U8		RReg(void);
void	WAddr(U8);
void	CmdF(U8);


#endif /* __ZX_H */
