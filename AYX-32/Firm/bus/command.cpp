
// Commands execution
//
// AYX-32 project
// (c) TS-Labs
//
// Achos mae bob un eiliad yn geni ac yn lladd, a bob un enaid yn caru a chasau

void terminate(ERR err)
{
  status.b = STATUS_CLR;
  error = err;
  rd_ptr.nul();
  wd_ptr.nul();
  nx_task = no_task;
}

// Execute command
void command(u8 cmd)
{
  if (cmd == C_BREAK)
  {
    if (!bg_task)
      terminate(E_BREAK);
    else
      ;   // +++ set BREAK flag for BG task
  }
  else if (!status.busy)
  {
    status.b = STATUS_CLR;
    error = E_NONE;
    rd_ptr.nul();
    wd_ptr.nul();
    command_vec[cmd]();
  }
}

// Note: command should be a short non-blocking function
#ifndef BOOT
void cm_psg_init()
{
  // +++
}

void cm_ws_init()
{
  // +++
}

void cm_ws_update()
{
  snd::put_ws_event();
  snd::ws_ext_cmdlist.put_byte(snd::WSC_END_OF_LIST);
}

void cm_lock()
{
  status.busy = true;

  if (*(u32*)param == MAGIC_LCK)
    set_bg_task(bg_unlock);
  else
    set_bg_task(bg_lock);
}

void cm_save_cfg()
{
  if (*(u32*)param != MAGIC_CFG)
    terminate(E_PARMERR);
  else
  {
    status.busy = true;
    set_bg_task(bg_save_cfg);
  }
}
#endif

#ifdef BOOT
void cm_up_fw()
{
  u32 size = *(u32*)param;

  if (!size || (size > sizeof(fw_buf)))
    terminate(E_PARMERR);
  else
  {
    flasher::raw_size = size;
    status.drq = true;
    status.busy = true;
    wd_ptr.init(fw_buf, size);
    nx_task = t_up_fw;
  }
}

void cm_fl_fw()
{
  if (!flasher::is_fw_valid)
    terminate(E_SEQERR);
  else if (*(u32*)param != MAGIC_FFW)
    terminate(E_PARMERR);
  else
  {
    status.busy = true;
    set_bg_task(bg_fl_fw);
  }
}
#endif

void cm_reset()
{
  if (*(u32*)param != MAGIC_RES)
    terminate(E_PARMERR);
  else
  {
    SCB::generateReset(scb::airc::SYSRESETREQ);
    while (1);
  }
}

// Main background tasks
#ifndef BOOT
void bg_save_cfg()
{
  terminate(snd::save_cfg() ? E_DONE : E_EXECERR);
}

void bg_lock()
{
  interrupts::setIrqIdle();
  init_vectors();
  interrupts::setIrqWork();
  terminate(E_DONE);
}

void bg_unlock()
{
  interrupts::setIrqIdle();
  init_vectors_ext();
  interrupts::setIrqWork();
  terminate(E_DONE);
}
#endif

// Boot background tasks
#ifdef BOOT
void t_up_fw()
{
  status.drq = false;
  set_bg_task(bg_up_fw);
}

void bg_up_fw()
{
  terminate(flasher::check_fw() ? E_DONE : E_DATAERR);
}

void bg_fl_fw()
{
  terminate(flasher::flash_fw() ? E_DONE : E_EXECERR);
}
#endif
