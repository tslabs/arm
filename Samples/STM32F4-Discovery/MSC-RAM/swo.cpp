
namespace SWO
{
  void init(void)
  {
    /* Enable SWO output */
    DBGMCU->CR = 0x00000020;
  }

  int fputc(int ch)
  {
    /* Check parameters */
    if 
    ((ITM->TCR & ITM_TCR_ITMENA_Msk) &&      /* ITM enabled */
     (ITM->TER & (1UL << 0)))					/* ITM Port #0 enabled */
    {
      while (ITM->PORT[0].u32 == 0);			/* Wait for available */
      ITM->PORT[0].u8 = (uint8_t) ch;			/* Send character */

      /* Return written character */
      return ch;
    }

    /* Return EOF to stop writing */
    return -1;
  }
}
