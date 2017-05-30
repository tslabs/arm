
// AYX-32 bus
//
// AYX-32 project
// (c) TS-Labs
//
// ...

/// Functions
// Init vectors
void set_idle_vectors()
{
  for (int i = 0; i < countof(write_addr_vec); i++)
    write_addr_vec[i] = wa_empty;

  for (int i = 0; i < countof(write_reg_vec); i++)
    write_reg_vec[i] = wr_empty;

  for (int i = 0; i < countof(read_reg_vec); i++)
    read_reg_vec[i] = rr_empty_ff;

  for (int i = 0; i < countof(command_vec); i++)
    command_vec[i] = cm_empty;
}

void init_common_vectors()
{
  for (int i = 0; i < countof(t_write_addr_vec); i++)
    write_addr_vec[t_write_addr_vec[i].num] = t_write_addr_vec[i].addr;

  for (int i = 0; i < countof(t_write_reg_vec); i++)
    write_reg_vec[t_write_reg_vec[i].num] = t_write_reg_vec[i].addr;

  for (int i = 0; i < countof(t_read_reg_vec); i++)
    read_reg_vec[t_read_reg_vec[i].num] = t_read_reg_vec[i].addr;

  for (int i = 0; i < countof(t_command_vec); i++)
    command_vec[t_command_vec[i].num] = t_command_vec[i].addr;
}
  
void init_ext_vectors()
{
  for (int i = 0; i < countof(t_write_addr_vec_ext); i++)
    write_addr_vec[t_write_addr_vec_ext[i].num] = t_write_addr_vec_ext[i].addr;

  for (int i = 0; i < countof(t_write_reg_vec_ext); i++)
    write_reg_vec[t_write_reg_vec_ext[i].num] = t_write_reg_vec_ext[i].addr;

  for (int i = 0; i < countof(t_read_reg_vec_ext); i++)
    read_reg_vec[t_read_reg_vec_ext[i].num] = t_read_reg_vec_ext[i].addr;

  for (int i = 0; i < countof(t_command_vec_ext); i++)
    command_vec[t_command_vec_ext[i].num] = t_command_vec_ext[i].addr;
}

void init_vectors()
{
  set_idle_vectors();
  init_common_vectors();
}

void init_vectors_ext()
{
  set_idle_vectors();
  init_common_vectors();
  init_ext_vectors();
}
