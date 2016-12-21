
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
// Note: split into 64 case parts to allow GCC implement TBB/TBH branching
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

#ifndef BOOT
  command_vec[cmd]();
#else
  if ((cmd >= 0xE0) && (cmd <= 0xEF))
    command_vec[cmd - 0xE0]();
  else
    error = E_CMDERR;
#endif
  }
}

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

#ifndef BOOT
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

void cm_()
{
  error = E_CMDERR;
}

// Main background tasks
#ifndef BOOT
void bg_save_cfg()
{
  terminate(snd::save_cfg() ? E_DONE : E_EXECERR);
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
