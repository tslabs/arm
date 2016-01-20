
int pow(int b, int p)
{
  int r = 1;
  while (p--)
    r *= b;
  return r;
}