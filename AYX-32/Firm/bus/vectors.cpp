
// Command function vectors
//
// AYX-32 project
// (c) TS-Labs
//
// I wtedy wreszcie sam też włączę się do akcji

/// Function vector arrays
// Write Address
const T_A_VEC t_write_addr_vec[] =
{
#ifndef BOOT
  // Turbo-Sound
  0xF0, wa_ts,
  0xF1, wa_ts,
  0xF2, wa_ts,
  0xF3, wa_ts,
  0xF4, wa_ts,
  0xF5, wa_ts,
  0xF6, wa_ts,
  0xF7, wa_ts,
  0xF8, wa_ts,
  0xF9, wa_ts,
  0xFA, wa_ts,
  0xFB, wa_ts,
  0xFC, wa_ts,
  0xFD, wa_ts,
  0xFE, wa_ts,
  0xFF, wa_ts,
#endif

  // System
  R_DEV_SIG,     wa_dev_sig,
  R_PARAM,       wa_param,
};

const T_A_VEC t_write_addr_vec_ext[] =
{
#ifndef BOOT
  // DAC
  R_DAC_FREE,    wa_dac_free,
  R_DAC_USED,    wa_dac_used,

  // Device Control
  R_PSG_AMP_TAB, wa_amp_tab,
#endif

#ifdef BOOT
  R_DEV_SIG,     wa_dev_sig,
#endif
  R_DATA,        wa_data,
  R_UPTIME,      wa_upt,
  R_VER,         wa_ver,
  R_CPR_STR,     wa_cpr,
  R_BLD_STR,     wa_bld,
  R_CORE_FRQ,    wa_frq,
};

// Write register
const T_W_VEC t_write_reg_vec[] =
{
#ifndef BOOT
  // PSG generic
  R_PSG_TP_AL,   wr_psg8e,
  R_PSG_TP_AH,   wr_psg4e,
  R_PSG_TP_BL,   wr_psg8e,
  R_PSG_TP_BH,   wr_psg4e,
  R_PSG_TP_CL,   wr_psg8e,
  R_PSG_TP_CH,   wr_psg4e,
  R_PSG_NP,      wr_psg5e,
  R_PSG_MX,      wr_psg8e,
  R_PSG_V_A,     wr_psg5e,
  R_PSG_V_B,     wr_psg5e,
  R_PSG_V_C,     wr_psg5e,
  R_PSG_EPL,     wr_psg8e,
  R_PSG_EPH,     wr_psg8e,
  R_PSG_EC,      wr_psg4e,
  R_PSG_IOA,     wr_psg8,
  R_PSG_IOB,     wr_psg8,
#endif

  // System
  R_CMD,         wr_cmd,  // used to unlock
  R_PARAM,       wr_arr,
};

const T_W_VEC t_write_reg_vec_ext[] =
{
#ifndef BOOT
  // PSG extended
  R_PSG_VOL_AL,  wr_volal,
  R_PSG_VOL_AR,  wr_volar,
  R_PSG_VOL_BL,  wr_volbl,
  R_PSG_VOL_BR,  wr_volbr,
  R_PSG_VOL_CL,  wr_volcl,
  R_PSG_VOL_CR,  wr_volcr,
  R_PSG_TDC_A,   wr_psg8e,
  R_PSG_TDC_B,   wr_psg8e,
  R_PSG_TDC_C,   wr_psg8e,

  // DAC
  R_DAC_CTRL,    wr_rb,
  R_DAC_VOL_L,   wr_rb,
  R_DAC_VOL_R,   wr_rb,
  R_DAC_DATA,    wr_dac_data,

  // WS
  R_WS_CTRL,     wr_wsc,
  R_WS_VOL_L,    wr_wsc,
  R_WS_VOL_R,    wr_wsc,
  R_WS_SAMP,     wr_wsc,
  R_WS_NOTE,     wr_wsc,
  R_WS_NOTE_S,   wr_wsc,

  // Device Control
  R_PSG_SEL,     wr_psgsel,
  R_PSG_CCTRL,   wr_rbe,
  R_PSG_BCTRL,   wr_rbe,
  R_PSG_ACTRL,   wr_rbe,
  R_M_VOL_L,     wr_rbe,
  R_M_VOL_R,     wr_rbe,
  R_PSG_AMP_TAB, wr_arr,
  R_PUSH_UP,     wr_rb,
  R_WS_NUM,      wr_rb,
  R_WS_SEL,      wr_wscrb,
#endif

  // System
  R_DATA,        wr_arr_t,
};

// Read register
const T_R_VEC t_read_reg_vec[] =
{
#ifndef BOOT
  // PSG generic
  R_PSG_TP_AL,   rr_psg,
  R_PSG_TP_AH,   rr_psg,
  R_PSG_TP_BL,   rr_psg,
  R_PSG_TP_BH,   rr_psg,
  R_PSG_TP_CL,   rr_psg,
  R_PSG_TP_CH,   rr_psg,
  R_PSG_NP,      rr_psg,
  R_PSG_MX,      rr_psg,
  R_PSG_V_A,     rr_psg,
  R_PSG_V_B,     rr_psg,
  R_PSG_V_C,     rr_psg,
  R_PSG_EPL,     rr_psg,
  R_PSG_EPH,     rr_psg,
  R_PSG_EC,      rr_psg,
  R_PSG_IOA,     rr_psg,
  R_PSG_IOB,     rr_psg,
#endif

  // System
  R_DEV_SIG,     rr_arr,
};

const T_R_VEC t_read_reg_vec_ext[] =
{
#ifndef BOOT
  // PSG extended
  R_PSG_VOL_AL,  rr_psg,
  R_PSG_VOL_AR,  rr_psg,
  R_PSG_VOL_BL,  rr_psg,
  R_PSG_VOL_BR,  rr_psg,
  R_PSG_VOL_CL,  rr_psg,
  R_PSG_VOL_CR,  rr_psg,
  R_PSG_TDC_A,   rr_psg,
  R_PSG_TDC_B,   rr_psg,
  R_PSG_TDC_C,   rr_psg,

  // DAC
  R_DAC_CTRL,    rr_rb,
  R_DAC_VOL_L,   rr_rb,
  R_DAC_VOL_R,   rr_rb,
  R_DAC_SMPR,    rr_arr,
  R_DAC_FREE,    rr_arr,
  R_DAC_USED,    rr_arr,

  // Device Control
  R_PSG_SEL,     rr_rb,
  R_PSG_CCTRL,   rr_rb,
  R_PSG_BCTRL,   rr_rb,
  R_PSG_ACTRL,   rr_rb,
  R_M_VOL_L,     rr_rb,
  R_M_VOL_R,     rr_rb,
  R_PSG_AMP_TAB, rr_arr,
  R_PUSH_UP,     rr_rb,
  R_WS_NUM,      rr_rb,
  R_WS_SEL,      rr_rb,
#endif

  // System
#ifdef BOOT
  R_DEV_SIG,     rr_arr,
#endif
  R_STATUS,      rr_status,
  R_ERROR,       rr_error,
  R_RESP,        rr_arr,
  R_DATA,        rr_arr,
  R_CPU_LOAD,    rr_cpuld,
  R_UPTIME,      rr_arr,
  R_VER,         rr_arr,
  R_CPR_STR,     rr_arr,
  R_BLD_STR,     rr_arr,
  R_CORE_FRQ,    rr_arr,
};

// Command
const T_C_VEC t_command_vec[] =
{
#ifndef BOOT
  C_LOCK,      cm_lock,
#endif
};

const T_C_VEC t_command_vec_ext[] =
{
#ifndef BOOT
  C_PSG_INIT,  cm_psg_init,
  C_WS_INIT,   cm_ws_init,
  C_WS_UPDATE, cm_ws_update,
  C_SAVE_CFG,  cm_save_cfg,
#endif

#ifdef BOOT
  C_UPLD_FW,   cm_up_fw,
  C_FLASH_FW,  cm_fl_fw,
#endif

  C_RESET,     cm_reset,
};
