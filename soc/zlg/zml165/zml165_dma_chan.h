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
 * \brief ZML165 DMA 通道编号定义
 *
 * \internal
 * \par Modification History
 * \endinternal
 */

#ifndef __ZML165_DMA_CHAN_H
#define __ZML165_DMA_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup zml165_if_dma_chan
 * \copydoc zml165_dma_chan.h
 * @{
 */

/**
 * \name ZML165 DMA 通道编号
 * @{
 */
#define DMA_CHAN_1                0               /**< \brief DMA通道1 */
#define DMA_CHAN_2                1               /**< \brief DMA通道2 */
#define DMA_CHAN_3                2               /**< \brief DMA通道3 */
#define DMA_CHAN_4                3               /**< \brief DMA通道4 */
#define DMA_CHAN_5                4               /**< \brief DMA通道5 */

/** @} */

/**
 * \name 外设的DMA请求通道
 * \anchor dma_request_src
 * @{
 */

#define DMA_CHAN_ADC           DMA_CHAN_1     /**< \brief ADC      请求 */

#define DMA_CHAN_UART1_TX      DMA_CHAN_2     /**< \brief UART1 TX  请求 */
#define DMA_CHAN_UART1_RX      DMA_CHAN_3     /**< \brief UART1 RX  请求 */
#define DMA_CHAN_UART2_TX      DMA_CHAN_4     /**< \brief UART2 TX  请求 */
#define DMA_CHAN_UART2_RX      DMA_CHAN_5     /**< \brief UART2 RX  请求 */

#define DMA_CHAN_SPI1_RX       DMA_CHAN_2     /**< \brief SPI1 RX   请求 */
#define DMA_CHAN_SPI1_TX       DMA_CHAN_3     /**< \brief SPI1 TX   请求 */
#define DMA_CHAN_SPI2_RX       DMA_CHAN_4     /**< \brief SPI2 RX   请求 */
#define DMA_CHAN_SPI2_TX       DMA_CHAN_5     /**< \brief SPI2 TX   请求 */

#define DMA_CHAN_I2C_TX        DMA_CHAN_2     /**< \brief I2C TX   请求 */
#define DMA_CHAN_I2C_RX        DMA_CHAN_3     /**< \brief I2C RX   请求 */

#define DMA_CHAN_TIM1_CH1      DMA_CHAN_2     /**< \brief TIM1 CH1 请求 */
#define DMA_CHAN_TIM1_CH2      DMA_CHAN_3     /**< \brief TIM1 CH2 请求 */
#define DMA_CHAN_TIM1_TX4      DMA_CHAN_4     /**< \brief TIM1 TX4 请求 */
#define DMA_CHAN_TIM1_TRIG     DMA_CHAN_4     /**< \brief TIM1 TRIG请求 */
#define DMA_CHAN_TIM1_COM      DMA_CHAN_4     /**< \brief TIM1 COM 请求 */
#define DMA_CHAN_TIM1_CH3      DMA_CHAN_5     /**< \brief TIM1 CH3 请求 */
#define DMA_CHAN_TIM1_UP       DMA_CHAN_5     /**< \brief TIM1 UP  请求 */

#define DMA_CHAN_TIM2_CH3      DMA_CHAN_1     /**< \brief TIM2 CH3 请求 */
#define DMA_CHAN_TIM2_UP       DMA_CHAN_2     /**< \brief TIM2 UP  请求 */
#define DMA_CHAN_TIM2_CH2      DMA_CHAN_3     /**< \brief TIM2 CH2 请求 */
#define DMA_CHAN_TIM2_CH4      DMA_CHAN_4     /**< \brief TIM2 CH4 请求 */
#define DMA_CHAN_TIM2_CH1      DMA_CHAN_5     /**< \brief TIM2 CH1 请求 */

#define DMA_CHAN_TIM3_CH3      DMA_CHAN_2     /**< \brief TIM3 CH3 请求 */
#define DMA_CHAN_TIM3_CH4      DMA_CHAN_3     /**< \brief TIM3 CH4 请求 */
#define DMA_CHAN_TIM3_UP       DMA_CHAN_3     /**< \brief TIM3 UP  请求 */
#define DMA_CHAN_TIM3_CH1      DMA_CHAN_4     /**< \brief TIM3 CH1 请求 */
#define DMA_CHAN_TIM3_TRIG     DMA_CHAN_4     /**< \brief TIM3 TRIG请求 */

#define DMA_CHAN_TIM16_UP      DMA_CHAN_3     /**< \brief TIM16 UP 请求 */
#define DMA_CHAN_TIM16_CH1     DMA_CHAN_3     /**< \brief TIM16 CH1请求 */

#define DMA_CHAN_TIM17_UP      DMA_CHAN_1     /**< \brief TIM17 UP 请求 */
#define DMA_CHAN_TIM17_CH1     DMA_CHAN_1     /**< \brief TIM17 CH1请求 */

#define DMA_CHAN_AES_IN        DMA_CHAN_1     /**< \brief AES IN   请求 */
#define DMA_CHAN_AES_OUT       DMA_CHAN_2     /**< \brief AES OUT  请求 */

/** @} */

/**
 * @} zml165_if_dma_chan
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZML165_DMA_CHAN_H */

/* end of file */
