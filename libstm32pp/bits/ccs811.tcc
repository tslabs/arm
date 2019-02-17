
/*******************************************************************************
 *
 * Copyright (C) 2019 TSL
 *
 * Licenced with GNU General Public License
 * <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

namespace ccs811
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
    u8 reg;

    // check ID
    if (!I2C::ReadReg(DEVICE_ADDR, REG_HW_ID, reg)) return false;
    if (reg != REG_HW_ID_VAL) return false;

    STATUS stat;
    MEAS_MODE mode = {};

    if (!ReadStatus(stat)) return false;
    if (!stat.fw_mode)
    {
      if (!RunApplication()) return false;
      if (!ReadStatus(stat)) return false;
      if (!stat.fw_mode) return false;
      
      mode.drive_mode = MODE_1;
      if (!SetMeasurementMode(mode)) return false;
    }

    return true;
  }

  template<typename I2C>
  bool Functions<I2C>::ReadVer(VERSION &ver)
  {
    if (!I2C::ReadReg(DEVICE_ADDR, REG_HW_VERSION, &ver.hw, 1)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_FW_BOOT_VERSION, &ver.fw_boot, 2)) return false;
    if (!I2C::ReadReg(DEVICE_ADDR, REG_FW_APP_VERSION, &ver.fw_app, 2)) return false;

    return true;
  }

  template<typename I2C>
  bool Functions<I2C>::ReadStatus(STATUS &status)
  {
    return I2C::ReadReg(DEVICE_ADDR, REG_STATUS, status.byte);
  }

  template<typename I2C>
  bool Functions<I2C>::ReadError(ERROR &error)
  {
    return I2C::ReadReg(DEVICE_ADDR, REG_ERROR_ID, error.byte);
  }

  template<typename I2C>
  bool Functions<I2C>::ReadData(RES_DATA &data)
  {
    STATUS stat;
    if (!ReadStatus(stat)) return false;
    if (!stat.data_ready) return false;

    bool rc = I2C::ReadReg(DEVICE_ADDR, REG_ALG_RESULT_DATA, data.bytes, sizeof(data.bytes));
    SWAP_BYTES(data.eco2);
    SWAP_BYTES(data.tvoc);
    return rc;
  }

  template<typename I2C>
  bool Functions<I2C>::RunApplication()
  {
    return I2C::WriteReg(DEVICE_ADDR, REG_APP_START, 0, 0);
  }

  template<typename I2C>
  bool Functions<I2C>::SoftwareReset()
  {
    return I2C::WriteReg(DEVICE_ADDR, REG_SW_RESET, RESET_SEQ, sizeof(RESET_SEQ));
  }

  template<typename I2C>
  bool Functions<I2C>::SetMeasurementMode(MEAS_MODE &mode)
  {
    return I2C::WriteReg(DEVICE_ADDR, REG_MEAS_MODE, mode.byte);
  }

  template<typename I2C>
  bool Functions<I2C>::SetEnvironment(s16_16 &humidity, s16_16 &temperature)
  {
    u8 env[4];
    // +++
    return I2C::WriteReg(DEVICE_ADDR, REG_ENV_DATA, env, sizeof(env));
  }

#undef SWAP_BYTES
}
