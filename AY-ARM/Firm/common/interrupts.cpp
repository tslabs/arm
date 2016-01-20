
// Interrupts
//
// AY-ARM project
// (c)2015 TS-Labs
//
// Ur rymdens svarta djup

namespace clk
{
  void hseFailureHandler()
  {
  }
}

namespace exception
{
  extern "C"
  {
    void SysTick()
    {
    }
  }
}

namespace interrupt
{
  // Console
  void UART_CONSOLE_IRQ()
  {
    console::processUART();
  }

  void AU_DMA_IRQ()
  {
    AU_DMA::clearTransferCompleteFlag();

    u32 *buf = AU_DMA::isMemory1TheCurrentTarget() ? dac_buf1 : dac_buf2;

    static u16 c = 0;

    for (int i = 0; i < (sizeof(dac_buf1) / 4); i++)
    {
      buf[i] = ((!(c & 0x200)) ^ (i < (c & 0x1FF))) ? 0x0000FFFF : 0xFFFF0000;
    }

    c++;
  }

  enum
  {
    AYCB_BDIR  = 1 << 0,
    AYCB_BC1   = 1 << 1,
    AYCB_BC2   = 1 << 4,
    AYCB_A8    = 1 << 6,
    AYCB_A9    = 1 << 7,
    AYCB_MASK  = AYCB_BDIR | AYCB_BC1 | AYCB_BC2 | AYCB_A8 | AYCB_A9,
    AYCB_WADDR = AYCB_BDIR | AYCB_BC1 | AYCB_BC2 | AYCB_A8,
    AYCB_WREG  = AYCB_BDIR | AYCB_BC2 | AYCB_A8,
    AYCB_RREG  = AYCB_BC1 | AYCB_BC2 | AYCB_A8
  };

  // out #FFFD/#BFFD
  void BDIR_IRQ()
  {
    u16 bus = GPIOB::getInput();
    switch (bus & AYCB_MASK)
    {
      // out #FFFD
      case AYCB_WADDR:
        snd::write_address(bus >> 8);
        GPIOB::setOutput(1, snd::read_reg());
      break;

      // out #BFFD
      case AYCB_WREG:
        snd::write_reg(bus >> 8);
        GPIOB::setOutput(1, snd::read_reg());
      break;
    }

    BDIR::clearPendingFlag();
    NVIC::clearPendingIrq<nvic::irqn::BDIR_IRQ>();
    BC1::clearPendingFlag();
    NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
  }

  // in #FFFD
  void BC1_IRQ_epi() __attribute__ ((noinline));
  void BC1_IRQ() __attribute__ ((naked));
  void BC1_IRQ()
  {
    // This code give databus deassertion within 50ns,
    // which is half-T-state of Z80 at 7MHz
    asm
    (
      "    ldr  r0, =0x40020400 \n"  // GPIOB perifery address
      "    movw r1, #0x5555 \n"      // ODR[15:8] as outputs
      "    strh r1, [r0, #2] \n"
      "    movs r1, #0 \n"           // ODR[15:8] as inputs
      "l1: ldr  r2, [r0, #16] \n"
      "    and  r2, r2, #0xd3 \n"
      "    cmp  r2, #0x52 \n"        // AYCB_BC1 | AYCB_BC2 | AYCB_A8
      "    beq  l1 \n"
      "    strh r1, [r0, #2] \n"
    );

    asm ("push {lr} \n");
    BC1_IRQ_epi();
    asm ("pop {pc} \n");
  }

  void BC1_IRQ_epi()
  {
    GPIOB::setOutput(1, snd::read_reg());

    BC1::clearPendingFlag();
    NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
  }
};

void initializeSystick()
{
  SYSTICK::configure<
    SYSTICK_PERIOD,
    stk::cr::clksource::EXTERNAL_CLOCK,
    stk::cr::tickint::TICKINT_ENABLED,
    stk::cr::enable::SYSTICK_ENABLED>();
}

void initializeInterrupts()
{
  SCB::setPriority<scb::irqn::SysTick, SYSTICK_PRIORITY>();
  NVIC::setPriority<nvic::irqn::UART_CONSOLE_IRQ, UART_CONSOLE_PRIORITY>();
  NVIC::setPriority<nvic::irqn::AU_DMA_IRQ, AU_DMA_PRIORITY>();
  NVIC::setPriority<nvic::irqn::BDIR_IRQ, BDIR_PRIORITY>();
  NVIC::setPriority<nvic::irqn::BC1_IRQ, BC1_PRIORITY>();
  initializeSystick();

  NVIC::enableIrq<nvic::irqn::UART_CONSOLE_IRQ>();
  NVIC::enableIrq<nvic::irqn::AU_DMA_IRQ>();
  NVIC::enableIrq<nvic::irqn::BDIR_IRQ>();
  NVIC::enableIrq<nvic::irqn::BC1_IRQ>();
}
