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
 * \brief Kl26 DMA 通道编号定义
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-16  mkr, first implementation.
 * \endinternal
 */

#ifndef __KL26_DMAMUX_CHAN_H
#define __KL26_DMAMUX_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup kl26_if_dma_chan
 * \copydoc kl26_dmamux_chan.h
 * @{
 */


/**
 * \name KL26 DMA 通道编号
 * @{
 */
#define DMA_CHAN_0                0               /**< \brief DMA通道0  */
#define DMA_CHAN_1                1               /**< \brief DMA通道1  */
#define DMA_CHAN_2                2               /**< \brief DMA通道2  */
#define DMA_CHAN_3                3               /**< \brief DMA通道3  */

/** @} */


/**
 * \name 外设的DMA请求源序号
 * @{
 */

#define DMA_REQUEST_MUX0_DISABLE        0      /**< \brief 无外设 请求源        */

#define DMA_REQUEST_MUX0_UART0RX        2      /**< \brief UART0 RX 请求     */
#define DMA_REQUEST_MUX0_UART0TX        3      /**< \brief UART0 TX 请求     */
#define DMA_REQUEST_MUX0_UART1RX        4      /**< \brief UART1 RX 请求     */
#define DMA_REQUEST_MUX0_UART1TX        5      /**< \brief UART1 TX 请求     */
#define DMA_REQUEST_MUX0_UART2RX        6      /**< \brief UART2 RX 请求     */
#define DMA_REQUEST_MUX0_UART2TX        7      /**< \brief UART2 TX 请求     */

#define DMA_REQUEST_MUX0_I2S0RX         14     /**< \brief I2S0 RX 请求       */
#define DMA_REQUEST_MUX0_I2S0TX         15     /**< \brief I2S0 TX 请求       */

#define DMA_REQUEST_MUX0_SPI0RX         16     /**< \brief SPI0 RX 请求       */
#define DMA_REQUEST_MUX0_SPI0TX         17     /**< \brief SPI0 TX 请求       */
#define DMA_REQUEST_MUX0_SPI1RX         18     /**< \brief SPI1 RX 请求       */
#define DMA_REQUEST_MUX0_SPI1TX         19     /**< \brief SPI1 TX 请求       */

#define DMA_REQUEST_MUX0_I2C0           22     /**< \brief I2C0    请求       */
#define DMA_REQUEST_MUX0_I2C1           23     /**< \brief I2C1    请求       */

#define DMA_REQUEST_MUX0_TPM0_CHANNEL0  24     /**< \brief TPM0通道0 请求   */
#define DMA_REQUEST_MUX0_TPM0_CHANNEL1  25     /**< \brief TPM0通道1 请求   */
#define DMA_REQUEST_MUX0_TPM0_CHANNEL2  26     /**< \brief TPM0通道2 请求   */
#define DMA_REQUEST_MUX0_TPM0_CHANNEL3  27     /**< \brief TPM0通道3 请求   */
#define DMA_REQUEST_MUX0_TPM0_CHANNEL4  28     /**< \brief TPM0通道4 请求   */
#define DMA_REQUEST_MUX0_TPM0_CHANNEL5  29     /**< \brief TPM0通道5 请求   */
#define DMA_REQUEST_MUX0_TPM1_CHANNEL0  32     /**< \brief TPM1通道0 请求   */
#define DMA_REQUEST_MUX0_TPM1_CHANNEL1  33     /**< \brief TPM1通道1 请求   */
#define DMA_REQUEST_MUX0_TPM2_CHANNEL0  34     /**< \brief TPM2通道0 请求   */
#define DMA_REQUEST_MUX0_TPM2_CHANNEL1  35     /**< \brief TPM2通道1 请求   */

#define DMA_REQUEST_MUX0_ADC0           40     /**< \brief ADC0 请求                */

#define DMA_REQUEST_MUX0_CMP0           42     /**< \brief CMP0 请求                */

#define DMA_REQUEST_MUX0_DAC0           45     /**< \brief DAC0 请求                */

#define DMA_REQUEST_MUX0_PORTA          49     /**< \brief PORTA 请求              */
#define DMA_REQUEST_MUX0_PORTC          51     /**< \brief PORTC 请求              */
#define DMA_REQUEST_MUX0_PORTD          52     /**< \brief PORTD 请求              */

#define DMA_REQUEST_MUX0_TPM0_OVERFLOW  54     /**< \brief TPM0溢出  请求        */
#define DMA_REQUEST_MUX0_TPM1_OVERFLOW  55     /**< \brief TPM0溢出  请求        */
#define DMA_REQUEST_MUX0_TPM2_OVERFLOW  56     /**< \brief TPM0溢出  请求        */

#define DMA_REQUEST_MUX0_TSI            57     /**< \brief TSI 请求                     */

#define DMA_REQUEST_MUX0_ALWAYSON0      60     /**< \brief 线结点0 请求           */
#define DMA_REQUEST_MUX0_ALWAYSON1      61     /**< \brief 线结点1 请求           */
#define DMA_REQUEST_MUX0_ALWAYSON2      62     /**< \brief 线结点2 请求           */
#define DMA_REQUEST_MUX0_ALWAYSON3      63     /**< \brief 线结点3 请求           */

/** @} */


/**
 * @} amhw_if_kl26_dma_chan
 */

#ifdef __cplusplus
}
#endif


#endif /* __KL26_DMAMUX_CHAN_H */

/* end of file */
