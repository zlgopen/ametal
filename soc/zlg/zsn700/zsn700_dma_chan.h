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
 * \brief ZSN700 DMA 通道编号定义
 *
 * \internal
 * \par Modification History
 *
 * - 1.00 20-03-18  YRZ, first implementation
 *
 * \endinternal
 */

#ifndef __ZSN700_DMA_CHAN_H
#define __ZSN700_DMA_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup zsn700_if_dma_chan
 * \copydoc zsn700_dma_chan.h
 * @{
 */

/**
 * \name zsn700 DMA 通道编号
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

#define ZSN700_DMA_SRC_TYPE_SOFT         0x00  /* 传输请求触发源为 软件 */
#define ZSN700_DMA_SRC_TYPE_SPI0_RX      0x40  /* 传输请求触发源为 SPI0_RX */
#define ZSN700_DMA_SRC_TYPE_SPI0_TX      0x41  /* 传输请求触发源为 SPI0_TX */
#define ZSN700_DMA_SRC_TYPE_SPI1_RX      0x42  /* 传输请求触发源为 SPI1_RX */
#define ZSN700_DMA_SRC_TYPE_SPI1_TX      0x43  /* 传输请求触发源为 SPI1_TX */
#define ZSN700_DMA_SRC_TYPE_ADC_JRQ0_RX  0x44  /* 传输请求触发源为 ADC_JRQ0_RX */
#define ZSN700_DMA_SRC_TYPE_ADC_SRQ0_RX  0x45  /* 传输请求触发源为 ADC_SRQ0_RX */
#define ZSN700_DMA_SRC_TYPE_LCD_TX       0x45  /* 传输请求触发源为 LCD_TX */

#define ZSN700_DMA_SRC_TYPE_UART0_RX     0x48  /* 传输请求触发源为 UART0_RX */
#define ZSN700_DMA_SRC_TYPE_UART0_TX     0x49  /* 传输请求触发源为 UART0_TX */
#define ZSN700_DMA_SRC_TYPE_UART1_RX     0x4A  /* 传输请求触发源为 UART1_RX */
#define ZSN700_DMA_SRC_TYPE_UART1_TX     0x4B  /* 传输请求触发源为 UART1_TX */
#define ZSN700_DMA_SRC_TYPE_LPUART0_RX   0x4C  /* 传输请求触发源为 LPUART0_RX */
#define ZSN700_DMA_SRC_TYPE_LPUART0_TX   0x4D  /* 传输请求触发源为 LPUART0_TX */
#define ZSN700_DMA_SRC_TYPE_LPUART1_RX   0x4E  /* 传输请求触发源为 LPUART1_RX */
#define ZSN700_DMA_SRC_TYPE_LPUART1_TX   0x4F  /* 传输请求触发源为 LPUART1_TX */
#define ZSN700_DMA_SRC_TYPE_DAC0         0x50  /* 传输请求触发源为 DAC0 */

#define ZSN700_DMA_SRC_TYPE_TIM0_CHA     0x52  /* 传输请求触发源为 TIM0_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM0_CHB     0x53  /* 传输请求触发源为 TIM0_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM1_CHA     0x54  /* 传输请求触发源为 TIM1_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM1_CHB     0x55  /* 传输请求触发源为 TIM1_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM2_CHA     0x56  /* 传输请求触发源为 TIM2_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM2_CHB     0x57  /* 传输请求触发源为 TIM2_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM3_CHA     0x58  /* 传输请求触发源为 TIM3_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM3_CHB     0x59  /* 传输请求触发源为 TIM3_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM4_CHA     0x5A  /* 传输请求触发源为 TIM4_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM4_CHB     0x5B  /* 传输请求触发源为 TIM4_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM5_CHA     0x5C  /* 传输请求触发源为 TIM5_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM5_CHB     0x5D  /* 传输请求触发源为 TIM5_CHB */
#define ZSN700_DMA_SRC_TYPE_TIM6_CHA     0x5E  /* 传输请求触发源为 TIM6_CHA */
#define ZSN700_DMA_SRC_TYPE_TIM6_CHB     0x5F  /* 传输请求触发源为 TIM6_CHB */
#define ZSN700_DMA_SRC_TYPE_UART4_RX     0x60  /* 传输请求触发源为 UART4_RX */
#define ZSN700_DMA_SRC_TYPE_UART4_TX     0x61  /* 传输请求触发源为 UART4_TX */
#define ZSN700_DMA_SRC_TYPE_UART5_RX     0x62  /* 传输请求触发源为 UART5_RX */
#define ZSN700_DMA_SRC_TYPE_UART5_TX     0x63  /* 传输请求触发源为 UART5_TX */

/** @} */

/**
 * @} zsn700_if_dma_chan
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZSN700_DMA_CHAN_H */

/* end of file */
