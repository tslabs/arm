
// Common functions
//
// AYX-32 project
// (c) TS-Labs
//
// Siamo fiori d’acciaio, il freddo della notte non ci spezzerà

void clear_bg_task()
{
  bg_task = no_task;
  is_bg_task = false;
}

void set_bg_task(TASK t)
{
  bg_task = t;
  is_bg_task = true;
}

void wait_ms(u32 ms)
{
  u32 t = time_ms;
  while ((time_ms - t) < ms);
}
