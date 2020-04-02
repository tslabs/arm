
u64 pow(int b, int p)
{
  u64 r = 1;
  while (p--)
    r *= b;
  return r;
}