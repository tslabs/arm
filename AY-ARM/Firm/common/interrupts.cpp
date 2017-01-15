
// Interrupts
//
// AYX-32 project
// (c) TS-Labs
//
// Ur rymdens svarta djup

/// - Variables ---
__attribute__ ((used, section(".ram_irq_vect")))
VECT_TABLE irq_vect_ram;

/// - Functions ---
void initializeIrqTab()
{
  TASK *p = (TASK*)&irq_vect_ram;
  for (u32 i = 0; i < (sizeof(VECT_TABLE) / sizeof(TASK)); i++)
    p[i] = ISR_Bad;
}

void setIrqTab()
{
  irq_vect_ram.SysTick = SysTick;
  irq_vect_ram.EXTI0 = BDIR;
  irq_vect_ram.EXTI1 = BC1;
  irq_vect_ram.EXTI2 = RST;
  irq_vect_ram.USART1 = UART_CONSOLE;
#ifndef BOOT
  irq_vect_ram.DMA1_Stream1 = AU_DMA;
#endif
}

void setIrqWork()
{
  irq_vect_ram.EXTI0 = BDIR;
  irq_vect_ram.EXTI1 = BC1;
  NVIC::enableIrq<nvic::irqn::UART_CONSOLE_IRQ>();
}

void setIrqIdle()
{
  irq_vect_ram.EXTI0 = BDIR_ram;
  irq_vect_ram.EXTI1 = BC1_ram;
  NVIC::disableIrq<nvic::irqn::UART_CONSOLE_IRQ>();
}

void initialize()
{
  initializeIrqTab();
  setIrqTab();

  SCB::setPriority<scb::irqn::SysTick, SYSTICK_PRIORITY>();
  NVIC::setPriority<nvic::irqn::BDIR_IRQ, BDIR_PRIORITY>();
  NVIC::setPriority<nvic::irqn::BC1_IRQ, BC1_PRIORITY>();
  NVIC::setPriority<nvic::irqn::RST_IRQ, RST_PRIORITY>();
  NVIC::setPriority<nvic::irqn::UART_CONSOLE_IRQ, UART_CONSOLE_PRIORITY>();
#ifndef BOOT
  NVIC::setPriority<nvic::irqn::AU_DMA_IRQ, AU_DMA_PRIORITY>();
#endif

  SYSTICK::configure<
    SYSTICK_PERIOD,
    stk::cr::clksource::EXTERNAL_CLOCK,
    stk::cr::tickint::TICKINT_ENABLED,
    stk::cr::enable::SYSTICK_ENABLED>();

  NVIC::enableIrq<nvic::irqn::UART_CONSOLE_IRQ>();
#ifndef BOOT
  NVIC::enableIrq<nvic::irqn::AU_DMA_IRQ>();
#endif
  NVIC::enableIrq<nvic::irqn::RST_IRQ>();
  NVIC::enableIrq<nvic::irqn::BDIR_IRQ>();
  NVIC::enableIrq<nvic::irqn::BC1_IRQ>();
}

// out #xFFD
void BDIR()
{
  // TEST::setHigh();
  u16 bus = GPIOB::getInput();
  switch (bus & AYCB_MASK)
  {
    // out #FFFD
    case AYCB_WADDR:
      bus::write_address(bus >> 8);
      GPIOB::setOutput(1, bus::read_reg());
    break;

    // out #BFFD
    case AYCB_WREG:
      bus::write_reg(bus >> 8);
      GPIOB::setOutput(1, bus::read_reg());
    break;
  }

  BDIR::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BDIR_IRQ>();
  BC1::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
  // TEST::setLow();
}

void BDIR_ram()
{
  switch (GPIOB::getInput() & AYCB_MASK)
  {
    // out #FFFD
    case AYCB_WADDR:
      bus::regnum = bus::R_STATUS;
      GPIOB::setOutput(1, bus::status.b);
    break;
  }

  BDIR::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BDIR_IRQ>();
  BC1::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
}

// in #FFFD
void BC1()
{
  // Databus deasserted within 70ns, which is half of Z80 T-state at 7MHz
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
  BC1_epi();
  asm ("pop {pc} \n");
}

void BC1_ram()
{
  // Databus deasserted within 70ns, which is half of Z80 T-state at 7MHz
  asm
  (
    "    ldr  r0, =0x40020400 \n"  // GPIOB perifery address
    "    movw r1, #0x5555 \n"      // ODR[15:8] as outputs
    "    strh r1, [r0, #2] \n"
    "    movs r1, #0 \n"           // ODR[15:8] as inputs
    "l2: ldr  r2, [r0, #16] \n"
    "    and  r2, r2, #0xd3 \n"
    "    cmp  r2, #0x52 \n"        // AYCB_BC1 | AYCB_BC2 | AYCB_A8
    "    beq  l2 \n"
    "    strh r1, [r0, #2] \n"
  );

  asm ("push {lr} \n");
  BC1_epi_ram();
  asm ("pop {pc} \n");
}

void BC1_epi()
{
  GPIOB::setOutput(1, bus::read_reg());
  BC1::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
}

void BC1_epi_ram()
{
  GPIOB::setOutput(1, bus::status.b);
  BC1::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::BC1_IRQ>();
}

void RST()
{
  // anti-chatter protection
  u32 delay = 12;  // about 900ns (13clk/cycle)
  while (delay--)
    if (AY_RST::isHigh())
      goto exit;

  SCB::generateReset(scb::airc::SYSRESETREQ);
  while (1);

exit:
  RST::clearPendingFlag();
  NVIC::clearPendingIrq<nvic::irqn::RST_IRQ>();
}

void UART_CONSOLE()
{
#ifdef BOOT
  uart::processUART();
#else
  console::processUART();
#endif
}

#ifndef BOOT
void AU_DMA()
{
  AU_DMA::clearTransferCompleteFlag();
  snd::buf_time += DAC_SAMPLES_COUNT;
  snd::curr_buf = AU_DMA::isMemory0TheCurrentTarget();
  req_snd_buf = true;
  // NVIC::triggerIrq<nvic::irqn::PVD>(); // set SndBuf renderer here
}
#endif

void SysTick()
{
  time_ms++;
}

void ISR_Bad()
{
  while (1);
}

void ISR_Default()
{
}
