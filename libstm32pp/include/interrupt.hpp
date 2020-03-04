/*******************************************************************************
 *
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of libstm32pp.
 *
 * libstm32pp is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * libstm32pp is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libstm32pp. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#pragma once

#include <device_select.hpp>

namespace interrupt {
  extern "C" {
    void WWDG() __attribute__ ((interrupt));
    void PVD() __attribute__ ((interrupt));
#if defined VALUE_LINE || \
    defined STM32F2XX || \
    defined STM32F4XX
    void TAMP_STAMP() __attribute__ ((interrupt));
    void RTC_WKUP() __attribute__ ((interrupt));
#else
    void TAMPER() __attribute__ ((interrupt));
    void RTC() __attribute__ ((interrupt));
#endif
    void FLASH() __attribute__ ((interrupt));
    void RCC() __attribute__ ((interrupt));
    void EXTI0() __attribute__ ((interrupt));
    void EXTI1() __attribute__ ((interrupt));
    void EXTI2() __attribute__ ((interrupt));
    void EXTI3() __attribute__ ((interrupt));
    void EXTI4() __attribute__ ((interrupt));
#ifdef STM32F1XX
    void DMA1_Channel1() __attribute__ ((interrupt));
    void DMA1_Channel2() __attribute__ ((interrupt));
    void DMA1_Channel3() __attribute__ ((interrupt));
    void DMA1_Channel4() __attribute__ ((interrupt));
    void DMA1_Channel5() __attribute__ ((interrupt));
    void DMA1_Channel6() __attribute__ ((interrupt));
    void DMA1_Channel7() __attribute__ ((interrupt));
#else
    void DMA1_Stream0() __attribute__ ((interrupt));
    void DMA1_Stream1() __attribute__ ((interrupt));
    void DMA1_Stream2() __attribute__ ((interrupt));
    void DMA1_Stream3() __attribute__ ((interrupt));
    void DMA1_Stream4() __attribute__ ((interrupt));
    void DMA1_Stream5() __attribute__ ((interrupt));
    void DMA1_Stream6() __attribute__ ((interrupt));
#endif
#ifdef VALUE_LINE
    void ADC1() __attribute__ ((interrupt));
#elif defined STM32F2XX || \
      defined STM32F4XX
    void ADC() __attribute__ ((interrupt));
#else
    void ADC1_2() __attribute__ ((interrupt));
#endif
#ifdef VALUE_LINE

#elif defined CONNECTIVITY_LINE || \
      defined STM32F2XX || \
      defined STM32F4XX
    void CAN1_TX() __attribute__ ((interrupt));
    void CAN1_RX0() __attribute__ ((interrupt));
#else
    void USB_HP_CAN1_TX() __attribute__ ((interrupt));
    void USB_LP_CAN1_RX0() __attribute__ ((interrupt));
#endif
#ifndef VALUE_LINE
    void CAN1_RX1() __attribute__ ((interrupt));
    void CAN1_SCE() __attribute__ ((interrupt));
#endif
    void EXTI9_5() __attribute__ ((interrupt));
#ifdef VALUE_LINE
    void TIM1_BRK_TIM15() __attribute__ ((interrupt));
    void TIM1_UP_TIM6() __attribute__ ((interrupt));
    void TIM1_TRG_COM_TIM17() __attribute__ ((interrupt));
#elif defined XL_DENSITY || \
      defined STM32F2XX || \
      defined STM32F4XX
    void TIM1_BRK_TIM9() __attribute__ ((interrupt));
    void TIM1_UP_TIM10() __attribute__ ((interrupt));
    void TIM1_TRG_COM_TIM11() __attribute__ ((interrupt));
#else
    void TIM1_BRK() __attribute__ ((interrupt));
    void TIM1_UP() __attribute__ ((interrupt));
    void TIM1_TRG_COM() __attribute__ ((interrupt));
#endif
    void TIM1_CC() __attribute__ ((interrupt));
    void TIM2() __attribute__ ((interrupt));
    void TIM3() __attribute__ ((interrupt));
    void TIM4() __attribute__ ((interrupt));
    void I2C1_EV() __attribute__ ((interrupt));
    void I2C1_ER() __attribute__ ((interrupt));
    void I2C2_EV() __attribute__ ((interrupt));
    void I2C2_ER() __attribute__ ((interrupt));
    void SPI1() __attribute__ ((interrupt));
    void SPI2() __attribute__ ((interrupt));
    void USART1() __attribute__ ((interrupt));
    void USART2() __attribute__ ((interrupt));
    void USART3() __attribute__ ((interrupt));
    void EXTI15_10() __attribute__ ((interrupt));
    void RTCAlarm() __attribute__ ((interrupt));
#ifdef VALUE_LINE
    void CEC() __attribute__ ((interrupt));
#elif defined CONNECTIVITY_LINE || \
      defined STM32F2XX || \
      defined STM32F4XX
    void OTG_FS_WKUP() __attribute__ ((interrupt));
#else
    void USB_WakeUp() __attribute__ ((interrupt));
#endif
#ifdef VALUE_LINE
    void TIM12() __attribute__ ((interrupt));
    void TIM13() __attribute__ ((interrupt));
    void TIM14() __attribute__ ((interrupt));
#elif defined CONNECTIVITY_LINE

#elif defined XL_DENSITY || \
      defined STM32F2XX || \
      defined STM32F4XX
    void TIM8_BRK_TIM12() __attribute__ ((interrupt));
    void TIM8_UP_TIM13() __attribute__ ((interrupt));
    void TIM8_TRG_COM_TIM14() __attribute__ ((interrupt));
#else
    void TIM8_BRK() __attribute__ ((interrupt));
    void TIM8_UP() __attribute__ ((interrupt));
    void TIM8_TRG_COM() __attribute__ ((interrupt));
#endif
#if not defined VALUE_LINE && \
    not defined CONNECTIVITY_LINE
    void TIM8_CC() __attribute__ ((interrupt));
#endif
#if defined VALUE_LINE || \
    defined CONNECTIVITY_LINE

#elif defined STM32F2XX || \
      defined STM32F4XX
    void ADC3() __attribute__ ((interrupt));
#else
    void DMA1_Stream7() __attribute__ ((interrupt));
#endif
#ifndef CONNECTIVITY_LINE
    void FSMC() __attribute__ ((interrupt));
#endif
#if not defined VALUE_LINE && \
    not defined CONNECTIVITY_LINE
    void SDIO() __attribute__ ((interrupt));
#endif
    void TIM5() __attribute__ ((interrupt));
    void SPI3() __attribute__ ((interrupt));
    void UART4() __attribute__ ((interrupt));
    void UART5() __attribute__ ((interrupt));
#if defined VALUE_LINE || \
    defined STM32F2XX || \
    defined STM32F4XX
    void TIM6_DAC() __attribute__ ((interrupt));
#else
    void TIM6() __attribute__ ((interrupt));
#endif
    void TIM7() __attribute__ ((interrupt));
#if defined STM32F2XX || \
    defined STM32F4XX
    void DMA2_Stream0() __attribute__ ((interrupt));
    void DMA2_Stream1() __attribute__ ((interrupt));
    void DMA2_Stream2() __attribute__ ((interrupt));
#else
    void DMA2_Channel1() __attribute__ ((interrupt));
    void DMA2_Channel2() __attribute__ ((interrupt));
    void DMA2_Channel3() __attribute__ ((interrupt));
#endif
#if defined STM32F2XX || \
    defined STM32F4XX
    void DMA2_Stream3() __attribute__ ((interrupt));
#elif defined CONNECTIVITY_LINE
    void DMA2_Channel4() __attribute__ ((interrupt));
#else
    void DMA2_Channel4_5() __attribute__ ((interrupt));
#endif
#if defined STM32F2XX || \
    defined STM32F4XX
    void DMA2_Stream4() __attribute__ ((interrupt));
#elif defined VALUE_LINE || \
      defined CONNECTIVITY_LINE
    void DMA2_Channel5() __attribute__ ((interrupt));
#endif
#if defined CONNECTIVITY_LINE || \
    defined STM32F2XX || \
    defined STM32F4XX
    void ETH() __attribute__ ((interrupt));
    void ETH_WKUP() __attribute__ ((interrupt));
    void CAN2_TX() __attribute__ ((interrupt));
    void CAN2_RX0() __attribute__ ((interrupt));
    void CAN2_RX1() __attribute__ ((interrupt));
    void CAN2_SCE() __attribute__ ((interrupt));
    void OTG_FS() __attribute__ ((interrupt));
#endif
#if defined STM32F2XX || \
    defined STM32F4XX
    void DMA2_Stream5() __attribute__ ((interrupt));
    void DMA2_Stream6() __attribute__ ((interrupt));
    void DMA2_Stream7() __attribute__ ((interrupt));
    void USART6() __attribute__ ((interrupt));
    void I2C3_EV() __attribute__ ((interrupt));
    void I2C3_ER() __attribute__ ((interrupt));
    void OTG_HS_EP1_OUT() __attribute__ ((interrupt));
    void OTG_HS_EP1_IN() __attribute__ ((interrupt));
    void OTG_HS_WKUP() __attribute__ ((interrupt));
    void OTG_HS() __attribute__ ((interrupt));
    void DCMI() __attribute__ ((interrupt));
    void CRYP() __attribute__ ((interrupt));
    void HASH_RNG() __attribute__ ((interrupt));
#endif
#ifdef STM32F4XX
    void FPU() __attribute__ ((interrupt));
#endif
  }
}  // namespace interrupt
