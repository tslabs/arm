
/*******************************************************************************
 *
 *                      Device electronic signature
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

#include "../defs.hpp"
#include "../../memorymap/devsig.hpp"

// Low-level access to the registers
#define DEVSIG_REGS_UID (u8*)(devsig::UID)
#define DEVSIG_REGS_FSIZE (*(u16*)(devsig::FSIZE))
