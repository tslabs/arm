
// AYX-32 bus
//
// AYX-32 project
// (c) TS-Labs
//
// ...

/// Functiond
// Init vectors
void init_vectors()
{
  // set default values
  for (int i = 0; i < countof(write_addr_vec); i++)
    write_addr_vec[i] = wa_empty;

  for (int i = 0; i < countof(write_reg_vec); i++)
    write_reg_vec[i] = wr_empty;

  for (int i = 0; i < countof(read_reg_vec); i++)
    read_reg_vec[i] = rr_empty;

  for (int i = 0; i < countof(command_vec); i++)
    command_vec[i] = cm_empty;

  // set function addresses
  for (int i = 0; i < countof(t_write_addr_vec); i++)
    write_addr_vec[t_write_addr_vec[i].num] = t_write_addr_vec[i].addr;

  for (int i = 0; i < countof(t_write_reg_vec); i++)
    write_reg_vec[t_write_reg_vec[i].num] = t_write_reg_vec[i].addr;

  for (int i = 0; i < countof(t_read_reg_vec); i++)
    read_reg_vec[t_read_reg_vec[i].num] = t_read_reg_vec[i].addr;

  for (int i = 0; i < countof(t_command_vec); i++)
    command_vec[t_command_vec[i].num] = t_command_vec[i].addr;
}
