
#pragma once

/// - Enums ---
enum
{
  SYSTICK_PERIOD        = 1000,   // us
  UART_BAUD_RATE        = 115200 * 72 / 80,
  UART_IRQ_PRIORITY     = 4,
  DMA_IRQ_PRIORITY  = 5,
  ADC_FREQ          = 1000,  // Hz
  BUF_SIZE          = 200,   // half-words
  GUARD             = ADC_FREQ / 50 / 2, // 50% of 50Hz AC period
  THRESH            = 50,    // for 30A IC
};

/// - Pins ---
typedef PA8 MCO;

typedef PC13 LED;

typedef PB12 HB;
typedef PB13 VB;

typedef PA9   TX;
typedef PA10  RX;
typedef USART1 UART;
#define UART_IRQ  USART1
#define UART_REGS USART1_REGS

typedef SPI1 SPI;
#define SPI_REGS ((spi::Registers*)(spi::SPI1))
typedef PA4 SS;
typedef PA5 SCK;
typedef PA6 MISO;
typedef PA7 MOSI;
typedef ft8xx::Functions<spi::SPI1, gpio::GPIOA, 4> FT812;

typedef DMA1_CHANNEL3 DMA;
#define DMA_IRQ DMA1_Channel3
#define DMA_CH CHANNEL_3


/// - Functions ---
