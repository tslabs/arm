
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
#define DEVSIG_REGS reinterpret_cast<devsig::Registers *>(devsig::ADDRESS)
