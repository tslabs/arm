
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace hdc1080
{
# define SWAP_BYTES(a) \
  { \
    u8 *b = (u8*)&a; \
    u8 c = b[1]; \
    b[1] = b[0]; \
    b[0] = c; \
  }

  template<typename I2C>
  bool Functions<I2C>::Initialize()
  {
    u8 data[2];

    // check ID
    if (!I2C::ReadReg(DEVICE_ADDR, REG_MF_ID, data, 2)) return false;
    if (*(u16*)data != REG_MF_ID_VAL) return false;

    if (!I2C::ReadReg(DEVICE_ADDR, REG_DEV_ID, data, 2)) return false;
    if (*(u16*)data != REG_DEV_ID_VAL) return false;

    // read calibration
    // for (u8 i = 0; i <= (REG_CALIB_21 - REG_CALIB_00); i++)
      // if (!I2C::ReadReg(DEVICE_ADDR, REG_CALIB_00 + i, c[i ^ 1])) return false;

    return true;
  }

  template<typename I2C>
  bool Functions<I2C>::ReadId(void *data)
  {
    u8 *_data = (u8*)data;

    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID0, (u8*)&_data[0], 2)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID1, (u8*)&_data[2], 2)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_SER_ID2, (u8*)&_data[4], 2)) return false;
    SWAP_BYTES(_data[0]);
    SWAP_BYTES(_data[2]);
    SWAP_BYTES(_data[4]);

    return true;
  }

#undef SWAP_BYTES
}
