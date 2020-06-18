/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief HC32 DMA 通道编号定义
 *
 * \internal
 * \par Modification History
 *
 * - 1.00 17-08-23  lqy, first implementation
 *
 * \endinternal
 */

#ifndef __HC32_DMA_CHAN_H
#define __HC32_DMA_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup hc32_if_dma_chan
 * \copydoc hc32_dma_chan.h
 * @{
 */

/**
 * \name hc32 DMA 通道编号
 * @{
 */
#define DMA_CHAN_1                0               /**< \brief DMA通道1 */
#define DMA_CHAN_2                1               /**< \brief DMA通道2 */

/** @} */

/**
 * \name 外设的DMA请求通道
 * \anchor dma_request_src
 * @{
 */

#define HC32_DMA_SRC_TYPE_SOFT         0x00  /* 传输请求触发源为 软件 */

#ifdef HC32X3X

#define HC32_DMA_SRC_TYPE_SPI0_RX      0x20  /* 传输请求触发源为 SPI0_RX */
#define HC32_DMA_SRC_TYPE_SPI0_TX      0x21  /* 传输请求触发源为 SPI0_TX */
#define HC32_DMA_SRC_TYPE_SPI1_RX      0x22  /* 传输请求触发源为 SPI1_RX */
#define HC32_DMA_SRC_TYPE_SPI1_TX      0x23  /* 传输请求触发源为 SPI1_TX */
#define HC32_DMA_SRC_TYPE_ADC_JRQ0_RX  0x24  /* 传输请求触发源为 ADC_JRQ0_RX */
#define HC32_DMA_SRC_TYPE_ADC_SRQ0_RX  0x25  /* 传输请求触发源为 ADC_SRQ0_RX */
#define HC32_DMA_SRC_TYPE_LCD_TX       0x26  /* 传输请求触发源为 LCD_TX */

#define HC32_DMA_SRC_TYPE_UART0_RX     0x28  /* 传输请求触发源为 UART0_RX */
#define HC32_DMA_SRC_TYPE_UART0_TX     0x29  /* 传输请求触发源为 UART0_TX */
#define HC32_DMA_SRC_TYPE_UART1_RX     0x2A  /* 传输请求触发源为 UART1_RX */
#define HC32_DMA_SRC_TYPE_UART1_TX     0x2B  /* 传输请求触发源为 UART1_TX */
#define HC32_DMA_SRC_TYPE_LPUART0_RX   0x2C  /* 传输请求触发源为 LPUART0_RX */
#define HC32_DMA_SRC_TYPE_LPUART0_TX   0x2D  /* 传输请求触发源为 LPUART0_TX */
#define HC32_DMA_SRC_TYPE_LPUART1_RX   0x2E  /* 传输请求触发源为 LPUART1_RX */
#define HC32_DMA_SRC_TYPE_LPUART1_TX   0x2F  /* 传输请求触发源为 LPUART1_TX */

#define HC32_DMA_SRC_TYPE_TIM0_CHA     0x32  /* 传输请求触发源为 TIM0_CHA */
#define HC32_DMA_SRC_TYPE_TIM0_CHB     0x33  /* 传输请求触发源为 TIM0_CHB */
#define HC32_DMA_SRC_TYPE_TIM1_CHA     0x34  /* 传输请求触发源为 TIM1_CHA */
#define HC32_DMA_SRC_TYPE_TIM1_CHB     0x35  /* 传输请求触发源为 TIM1_CHB */
#define HC32_DMA_SRC_TYPE_TIM2_CHA     0x36  /* 传输请求触发源为 TIM2_CHA */
#define HC32_DMA_SRC_TYPE_TIM2_CHB     0x37  /* 传输请求触发源为 TIM2_CHB */
#define HC32_DMA_SRC_TYPE_TIM3_CHA     0x38  /* 传输请求触发源为 TIM3_CHA */
#define HC32_DMA_SRC_TYPE_TIM3_CHB     0x39  /* 传输请求触发源为 TIM3_CHB */
#define HC32_DMA_SRC_TYPE_TIM4_CHA     0x3A  /* 传输请求触发源为 TIM4_CHA */
#define HC32_DMA_SRC_TYPE_TIM4_CHB     0x3B  /* 传输请求触发源为 TIM4_CHB */
#define HC32_DMA_SRC_TYPE_TIM5_CHA     0x3C  /* 传输请求触发源为 TIM5_CHA */
#define HC32_DMA_SRC_TYPE_TIM5_CHB     0x3D  /* 传输请求触发源为 TIM5_CHB */
#define HC32_DMA_SRC_TYPE_TIM6_CHA     0x3E  /* 传输请求触发源为 TIM6_CHA */
#define HC32_DMA_SRC_TYPE_TIM6_CHB     0x3F  /* 传输请求触发源为 TIM6_CHB */

#else

#define HC32_DMA_SRC_TYPE_SPI0_RX      0x40  /* 传输请求触发源为 SPI0_RX */
#define HC32_DMA_SRC_TYPE_SPI0_TX      0x41  /* 传输请求触发源为 SPI0_TX */
#define HC32_DMA_SRC_TYPE_SPI1_RX      0x42  /* 传输请求触发源为 SPI1_RX */
#define HC32_DMA_SRC_TYPE_SPI1_TX      0x43  /* 传输请求触发源为 SPI1_TX */
#define HC32_DMA_SRC_TYPE_ADC_JRQ0_RX  0x44  /* 传输请求触发源为 ADC_JRQ0_RX */
#define HC32_DMA_SRC_TYPE_ADC_SRQ0_RX  0x45  /* 传输请求触发源为 ADC_SRQ0_RX */
#define HC32_DMA_SRC_TYPE_LCD_TX       0x46  /* 传输请求触发源为 LCD_TX */

#define HC32_DMA_SRC_TYPE_UART0_RX     0x48  /* 传输请求触发源为 UART0_RX */
#define HC32_DMA_SRC_TYPE_UART0_TX     0x49  /* 传输请求触发源为 UART0_TX */
#define HC32_DMA_SRC_TYPE_UART1_RX     0x4A  /* 传输请求触发源为 UART1_RX */
#define HC32_DMA_SRC_TYPE_UART1_TX     0x4B  /* 传输请求触发源为 UART1_TX */
#define HC32_DMA_SRC_TYPE_LPUART0_RX   0x4C  /* 传输请求触发源为 LPUART0_RX */
#define HC32_DMA_SRC_TYPE_LPUART0_TX   0x4D  /* 传输请求触发源为 LPUART0_TX */
#define HC32_DMA_SRC_TYPE_LPUART1_RX   0x4E  /* 传输请求触发源为 LPUART1_RX */
#define HC32_DMA_SRC_TYPE_LPUART1_TX   0x4F  /* 传输请求触发源为 LPUART1_TX */
#define HC32_DMA_SRC_TYPE_DAC0         0x50  /* 传输请求触发源为 DAC0 */

#define HC32_DMA_SRC_TYPE_TIM0_CHA     0x52  /* 传输请求触发源为 TIM0_CHA */
#define HC32_DMA_SRC_TYPE_TIM0_CHB     0x53  /* 传输请求触发源为 TIM0_CHB */
#define HC32_DMA_SRC_TYPE_TIM1_CHA     0x54  /* 传输请求触发源为 TIM1_CHA */
#define HC32_DMA_SRC_TYPE_TIM1_CHB     0x55  /* 传输请求触发源为 TIM1_CHB */
#define HC32_DMA_SRC_TYPE_TIM2_CHA     0x56  /* 传输请求触发源为 TIM2_CHA */
#define HC32_DMA_SRC_TYPE_TIM2_CHB     0x57  /* 传输请求触发源为 TIM2_CHB */
#define HC32_DMA_SRC_TYPE_TIM3_CHA     0x58  /* 传输请求触发源为 TIM3_CHA */
#define HC32_DMA_SRC_TYPE_TIM3_CHB     0x59  /* 传输请求触发源为 TIM3_CHB */
#define HC32_DMA_SRC_TYPE_TIM4_CHA     0x5A  /* 传输请求触发源为 TIM4_CHA */
#define HC32_DMA_SRC_TYPE_TIM4_CHB     0x5B  /* 传输请求触发源为 TIM4_CHB */
#define HC32_DMA_SRC_TYPE_TIM5_CHA     0x5C  /* 传输请求触发源为 TIM5_CHA */
#define HC32_DMA_SRC_TYPE_TIM5_CHB     0x5D  /* 传输请求触发源为 TIM5_CHB */
#define HC32_DMA_SRC_TYPE_TIM6_CHA     0x5E  /* 传输请求触发源为 TIM6_CHA */
#define HC32_DMA_SRC_TYPE_TIM6_CHB     0x5F  /* 传输请求触发源为 TIM6_CHB */
#define HC32_DMA_SRC_TYPE_UART4_RX     0x60  /* 传输请求触发源为 UART4_RX */
#define HC32_DMA_SRC_TYPE_UART4_TX     0x61  /* 传输请求触发源为 UART4_TX */
#define HC32_DMA_SRC_TYPE_UART5_RX     0x62  /* 传输请求触发源为 UART5_RX */
#define HC32_DMA_SRC_TYPE_UART5_TX     0x63  /* 传输请求触发源为 UART5_TX */

#endif  /* HC32X3X */

/** @} */

/**
 * @} hc32_if_dma_chan
 */

#ifdef __cplusplus
}
#endif

#endif /* __HC32_DMA_CHAN_H */

/* end of file */
