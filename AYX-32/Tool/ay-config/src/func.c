
void wait(u16 d)
{
  volatile u16 a = d;
  while (a--);
}
