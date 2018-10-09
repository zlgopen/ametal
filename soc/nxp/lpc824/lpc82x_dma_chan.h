/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X DMA 通道及配置参数定义 
 *
 * 该文件定义了DMA通道编号和DMA通道配置参数：
 * - 通道编号: DMA_CHAN_* 指明操作的DMA通道；
 * - 通道参数：DAM_CHAN_OPT_*  在 #am_lpc82x_dma_controller_get() 中作为第二个参数传入。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem, modified.
 * - 1.00 15-07-10  win, first implementation.
 * \endinternal
 */

#ifndef __LPC82X_DMA_CHAN_H
#define __LPC82X_DMA_CHAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup lpc82x_if_dma_chan
 * \copydoc lpc82x_dma_chan.h
 * @{
 */

/**
 * \name DMA 通道编号
 * \anchor grp_dma_chan
 * @{
 */

#define DMA_CHAN_0                    0           /**< \brief DMA通道0  */
#define DMA_CHAN_1                    1           /**< \brief DMA通道1  */
#define DMA_CHAN_2                    2           /**< \brief DMA通道2  */
#define DMA_CHAN_3                    3           /**< \brief DMA通道3  */
#define DMA_CHAN_4                    4           /**< \brief DMA通道4  */
#define DMA_CHAN_5                    5           /**< \brief DMA通道5  */
#define DMA_CHAN_6                    6           /**< \brief DMA通道6  */
#define DMA_CHAN_7                    7           /**< \brief DMA通道7  */
#define DMA_CHAN_8                    8           /**< \brief DMA通道8  */
#define DMA_CHAN_9                    9           /**< \brief DMA通道9  */
#define DMA_CHAN_10                   10          /**< \brief DMA通道10 */
#define DMA_CHAN_11                   11          /**< \brief DMA通道11 */
#define DMA_CHAN_12                   12          /**< \brief DMA通道12 */
#define DMA_CHAN_13                   13          /**< \brief DMA通道13 */
#define DMA_CHAN_14                   14          /**< \brief DMA通道14 */
#define DMA_CHAN_15                   15          /**< \brief DMA通道15 */
#define DMA_CHAN_16                   16          /**< \brief DMA通道16 */
#define DMA_CHAN_17                   17          /**< \brief DMA通道17 */
                                      
/** @} */

/**
 * \name 外设的DMA请求通道(与通道编号相同)
 * \anchor grp_dma_chan_peripheral
 *
 *  该参数与通道编号相同，每个外设请求对应一个DMA通道
 *
 * \note 如果DMA数据传输速度需要和对应的外设同步，应该使能外设请求; \n
 *       没有外设请求的的数据传输可以使用任何一个通道。
 *
 * @{
 */

#define DMA_CHAN_USART0_RX_REQ    DMA_CHAN_0     /**< \brief UART0 RX 请求   */
#define DMA_CHAN_USART0_TX_REQ    DMA_CHAN_1     /**< \brief UART0 TX 请求   */
#define DMA_CHAN_USART1_RX_REQ    DMA_CHAN_2     /**< \brief UART1 RX 请求   */
#define DMA_CHAN_USART1_TX_REQ    DMA_CHAN_3     /**< \brief UART1 TX 请求   */
#define DMA_CHAN_USART2_RX_REQ    DMA_CHAN_4     /**< \brief UART2 RX 请求   */
#define DMA_CHAN_USART2_TX_REQ    DMA_CHAN_5     /**< \brief UART2 TX 请求   */
#define DMA_CHAN_SPI0_RX_REQ      DMA_CHAN_6     /**< \brief SPI0 RX 请求    */
#define DMA_CHAN_SPI0_TX_REQ      DMA_CHAN_7     /**< \brief SPI0 TX 请求    */
#define DMA_CHAN_SPI1_RX_REQ      DMA_CHAN_8     /**< \brief SPI1 RX 请求    */
#define DMA_CHAN_SPI1_TX_REQ      DMA_CHAN_9     /**< \brief SPI1 TX 请求    */
#define DMA_CHAN_I2C0_SLV_REQ     DMA_CHAN_10    /**< \brief I2C0 从机请求   */
#define DMA_CHAN_I2C0_MST_REQ     DMA_CHAN_11    /**< \brief I2C0 主机请求   */
#define DMA_CHAN_I2C1_SLV_REQ     DMA_CHAN_12    /**< \brief I2C1 从机请求   */
#define DMA_CHAN_I2C1_MST_REQ     DMA_CHAN_13    /**< \brief I2C1 主机请求   */
#define DMA_CHAN_I2C2_SLV_REQ     DMA_CHAN_14    /**< \brief I2C2 从机请求   */
#define DMA_CHAN_I2C2_MST_REQ     DMA_CHAN_15    /**< \brief I2C2 主机请求   */
#define DMA_CHAN_I2C3_SLV_REQ     DMA_CHAN_16    /**< \brief I2C3 从机请求   */
#define DMA_CHAN_I2C3_MST_REQ     DMA_CHAN_17    /**< \brief I2C3 主机请求   */

/** @} */

/******************************************************************************/

/**
 * \name DMA 通道传输优先级(默认值：DMA_CHAN_OPT_PRIO_0)
 * \anchor grp_dma_chan_opt_prio
 *
 * \note DMA_CHAN_OPT_PRIO_0为最高优先级
 * @{
 */

#define DMA_CHAN_OPT_PRIO_0        AM_SBF(0, 16)    /**< \brief 传输优先级0  */
#define DMA_CHAN_OPT_PRIO_1        AM_SBF(1, 16)    /**< \brief 传输优先级1  */
#define DMA_CHAN_OPT_PRIO_2        AM_SBF(2, 16)    /**< \brief 传输优先级2  */
#define DMA_CHAN_OPT_PRIO_3        AM_SBF(3, 16)    /**< \brief 传输优先级3  */

/** @} */

/**
 * \name DMA外设请求使能(默认不使能)
 * \anchor grp_dma_chan_opt_periph_req_en
 * 
 * 每个外设请求对应一个DMA通道，使能通道外设请求后，传输速度和外设请求保持一致
 *
 * - 对于传输模式为内存到外设(产生通道请求的外设)或者外设传到内存，
 *   应该使能外设请求，如：使用通道0传输UART0_RX数据到内存，应使能外设请求；
 *   
 * - 对于和外设请求没有联系的数据传输，应关闭外设请求(默认)，
 *   使用最大的速度传输，如：内存到内存数据传输。
 *   
 * \note 不能产生请求的外设使用DMA传输数据时，也应关闭通道的外设请求 \n
 *       如: ADC采集数据到内存，应关闭外设请求，使得每次传输不受通道请求干扰
 *
 * @{
 */
 
/** \brief DMA外设请求使能 */
#define DMA_CHAN_OPT_PERIPH_REQ_EN  AM_BIT(0)

/** @} */

/**
 * \name DMA 硬件触发条件(默认: 不需要硬件触发即开始传输)
 * \anchor grp_dma_chan_opt_hwtrig_mode
 * 
 *   配置了硬件触发后，只有硬件触发源满足触发条件，才会触发DMA传输，
 *   通过硬件触发可以控制DMA传输的连续性
 *
 * - 边沿触发：DMA传输的大小由是否使用突发模式决定： \n
 *   如果不使用突发模式(默认)，一次传输完成全部数据； \n
 *   如果使用突发模式，则每次硬件触发传输的数据根据突发大小配置决定。
 *
 * - 电平触发：DMA传输的大小由是否使用突发模式决定： \n
 *   如果不使用突发模式(默认)，一次传输完成全部数据； \n
 *   如果使用突发模式，数据传输保持到电平无效，除非数据全部传输完毕。
 *
 * \note #am_lpc82x_dma_transfer() 默认不需要硬件触发，就可以进行DMA传输
 *
 * @{
 */
 
/** \brief 硬件下降沿触发 */
#define DMA_CHAN_OPT_HWTRIG_MODE_FALL    AM_BIT(1)

/** \brief 硬件上升沿触发 */
#define DMA_CHAN_OPT_HWTRIG_MODE_RISE    (AM_BIT(1) | AM_BIT(4))

/** \brief 硬件低电平触发 */
#define DMA_CHAN_OPT_HWTRIG_MODE_LOW     (AM_BIT(1) | AM_BIT(5))

/** \brief 硬件高电平触发 */
#define DMA_CHAN_OPT_HWTRIG_MODE_HIGH    (AM_BIT(1) | AM_BIT(4) | AM_BIT(5))

/** @} */

/**
 * \name DMA 通道的硬件触发源
 * \anchor grp_dma_chan_opt_hwtrig_src
 * 
 * \note 只有配置了硬件触发条件，该参数才有效
 *
 * @{
 */
 
/** \brief ADC0 序列A中断     */
#define DMA_CHAN_OPT_HWTRIG_SRC_ADC0_SEQA_IRQ AM_SBF(0, 19)

/** \brief ADC0 序列B中断     */
#define DMA_CHAN_OPT_HWTRIG_SRC_ADC0_SEQB_IRQ AM_SBF(1, 19) 

/** \brief SCT0 DMA0 请求     */
#define DMA_CHAN_OPT_HWTRIG_SRC_SCT0_DMA0     AM_SBF(2, 19) 

/** \brief SCT1 DMA1 请求     */
#define DMA_CHAN_OPT_HWTRIG_SRC_SCT0_DMA1     AM_SBF(3, 19) 

/** \brief ACMP 比较器输出    */
#define DMA_CHAN_OPT_HWTRIG_SRC_ACMP_O        AM_SBF(4, 19) 

/** \brief 引脚中断 0         */
#define DMA_CHAN_OPT_HWTRIG_SRC_PININT0       AM_SBF(5, 19) 

/** \brief 引脚中断 1         */
#define DMA_CHAN_OPT_HWTRIG_SRC_PININT1       AM_SBF(6, 19) 

/** \brief DMA 复用触发 0     */
#define DMA_CHAN_OPT_HWTRIG_SRC_DMA_MUX0      AM_SBF(7, 19) 

/** \brief DMA 复用触发 1     */
#define DMA_CHAN_OPT_HWTRIG_SRC_DMA_MUX1      AM_SBF(8, 19) 

/** @} */

/**
 * \name DMA 突发模式使能(默认: 不使用突发模式)
 * \anchor grp_dma_chan_opt_hwtrig_burst_en
 *
 * \note 只有配置了硬件触发条件，该参数才有效；
 *       使能突发模式后，硬件边沿触发可配置每次传输数据大小。
 *
 * @{
 */

/** \brief 突发模式使能   */ 
#define DMA_CHAN_OPT_HWTRIG_BURST_EN   AM_BIT(6)

/** @} */


/**
 * \name DMA 突发模式地址环绕(默认: 不使用地址环绕)
 * \anchor grp_dma_chan_opt_hwtrig_burst_wrap
 *
 * \note 只有使能了突发模式，该参数才有效
 *
 * @{
 */

/** \brief 突发传输时，源地址在突发大小范围内改变   */ 
#define DMA_CHAN_OPT_HWTRIG_BURST_SRC_WRAP   AM_BIT(14)

/** \brief 突发传输时，目的地址在突发大小范围内改变 */
#define DMA_CHAN_OPT_HWTRIG_BURST_DST_WRAP   AM_BIT(15)

/** @} */


/**
 * \name DMA 边沿触发条件突发传输大小（默认值：DMA_CHAN_OPT_HWTRIG_BURST_1）
 * \anchor grp_dma_chan_opt_hwtrig_burst_count
 *
 * \note 只在硬件边沿触发条件下，该配置才有效
 *
 * @{
 */
 
 /** \brief 突发传输大小为   1    */
#define DMA_CHAN_OPT_HWTRIG_BURST_1    AM_SBF(0, 8)

/** \brief 硬突发传输大小为 2    */
#define DMA_CHAN_OPT_HWTRIG_BURST_2    AM_SBF(1, 8)

/** \brief 硬突发传输大小为 4    */
#define DMA_CHAN_OPT_HWTRIG_BURST_4    AM_SBF(2, 8)

/** \brief 硬突发传输大小为 8    */
#define DMA_CHAN_OPT_HWTRIG_BURST_8    AM_SBF(3, 8)

/** \brief 硬突发传输大小为 16   */
#define DMA_CHAN_OPT_HWTRIG_BURST_16   AM_SBF(4, 8)

/** \brief 硬突发传输大小为 32   */
#define DMA_CHAN_OPT_HWTRIG_BURST_32   AM_SBF(5, 8)

/** \brief 硬突发传输大小为 64   */
#define DMA_CHAN_OPT_HWTRIG_BURST_64   AM_SBF(6, 8)

/** \brief 硬突发传输大小为 128  */
#define DMA_CHAN_OPT_HWTRIG_BURST_128  AM_SBF(7, 8)

/** \brief 硬突发传输大小为 256  */
#define DMA_CHAN_OPT_HWTRIG_BURST_256  AM_SBF(8, 8)

/** \brief 硬突发传输大小为 512  */
#define DMA_CHAN_OPT_HWTRIG_BURST_512  AM_SBF(9, 8)

/** \brief 硬突发传输大小为 1024 */
#define DMA_CHAN_OPT_HWTRIG_BURST_1024 AM_SBF(10, 8)

/** @} */

/**
 * \name DMA 生成触发源
 * \anchor grp_dma_chan_opt_trig_out
 * 
 * \note 生成的触发通过DMA MUX 作为其他通道的触发输入，如果其他通道先前被配置，
 *       将会被覆盖
 *
 * @{
 */
 
/** \brief DMA触发输出复用0 */
#define DMA_CHAN_OPT_TRIG_OUT_MUX0 AM_SBF(1, 23)

/** \brief DMA触发输出复用1 */
#define DMA_CHAN_OPT_TRIG_OUT_MUX1 AM_SBF(2, 23) 

/** @} */


/**
 * @} lpc82x_if_dma_chan
 */

#ifdef __cplusplus
}
#endif


#endif /* __LPC82X_DMA_CHAN_H */

/* end of file */
