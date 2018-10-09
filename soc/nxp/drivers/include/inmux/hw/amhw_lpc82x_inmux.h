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
 * \brief 输入复用 (INMUX) 硬件操作接口
 *
 * 1. 配置DMA通道的硬件触发源;
 * 2. 配置DMA通道触发输出复用;
 * 3. 配置SCT输入触发源。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-30  mem, modified.
 * - 1.00 15-07-09  zxl, first implementation.
 * \endinternal
 */
 
#ifndef __AMHW_LPC82X_INMUX_H
#define __AMHW_LPC82X_INMUX_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

 /** 
 * \addtogroup amhw_lpc82x_if_inmux
 * \copydoc amhw_lpc82x_inmux.h
 * @{
 */


/**
 * \brief  输入复用寄存器块结构体
 */
typedef struct amhw_lpc82x_inmux {
    __IO uint32_t dma_inmux[18];      /**< \brief DMA 触发源选择寄存器        */
    __I  uint32_t reserved[4078];     /**< \brief 保留                        */  
    __IO uint32_t dma_mux[2];         /**< \brief DMA 输出复用寄存器          */
    __I  uint32_t reserved2[6];       /**< \brief 保留                        */ 
    __IO uint32_t sct0_inmux[4];      /**< \brief SCT 输入选择寄存器          */ 
} amhw_lpc82x_inmux_t;


/** \brief DMA 通道触发源 */
typedef enum amhw_lpc82x_inmux_dma_trig_src {
    AMHW_LPC82X_INMUX_DMA_TRIG_ADC0_SEQA_IRQ = 0, /**< \brief ADC0 序列A中断  */
    AMHW_LPC82X_INMUX_DMA_TRIG_ADC0_SEQB_IRQ,     /**< \brief ADC0 序列B中断  */
    AMHW_LPC82X_INMUX_DMA_TRIG_SCT0_DMA0,         /**< \brief SCT0 DMA0 请求  */
    AMHW_LPC82X_INMUX_DMA_TRIG_SCT0_DMA1,         /**< \brief SCT1 DMA1 请求  */
    AMHW_LPC82X_INMUX_DMA_TRIG_ACMP_O,            /**< \brief ACMP 比较器输出 */
    AMHW_LPC82X_INMUX_DMA_TRIG_PININT0,           /**< \brief 引脚中断 0      */
    AMHW_LPC82X_INMUX_DMA_TRIG_PININT1,           /**< \brief 引脚中断 1      */
    AMHW_LPC82X_INMUX_DMA_TRIG_DMA_MUX0,          /**< \brief DMA 复用触发 0  */
    AMHW_LPC82X_INMUX_DMA_TRIG_DMA_MUX1,          /**< \brief DMA 复用触发 1  */
} amhw_lpc82x_inmux_dma_trig_src_t;

/** \brief SCT 输入触发源 */
typedef enum amhw_lpc82x_inmux_sct_trig_src {
    AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN0 = 0,    /**< \brief SCT_PIN0.          */
    AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN1,        /**< \brief SCT_PIN1.          */
    AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN2,        /**< \brief SCT_PIN2.          */
    AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN3,        /**< \brief SCT_PIN3.          */
    AMHW_LPC82X_INMUX_SCTTRIG_ADC_THCMP_IRQ,   /**< \brief ADC_THCMP_IRQ      */
    AMHW_LPC82X_INMUX_SCTTRIG_ACMP_O,          /**< \brief ACMP_O             */
    AMHW_LPC82X_INMUX_SCTTRIG_ARM_TXEV,        /**< \brief ARM_TXEV           */
    AMHW_LPC82X_INMUX_DEBUG_HALTED,            /**< \brief DEBUG_HALTED       */
} amhw_lpc82x_inmux_sct_trig_src_t;

/**
 * \brief 配置指定DMA通道触发源
 *
 * \param[in] p_hw_inmux : 指向输入复用寄存器块的指针
 * \param[in] chan       : DMA通道，值为 DMA_CHAN_* (#DMA_CHAN_0)
 * \param[in] trig       : 触发源
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_inmux_dma_trig_set (amhw_lpc82x_inmux_t            *p_hw_inmux,
                                     uint8_t                         chan,
                                    amhw_lpc82x_inmux_dma_trig_src_t trig)
{
    p_hw_inmux->dma_inmux[chan] = trig;
}

/**
 * \brief 配置DMA复用输出连接的DAM通道
 *
 * \param[in] p_hw_inmux : 指向输入复用寄存器块的指针
 * \param[in] dma_mux    : inmux通道0或者1，对应值为0或者1 
 * \param[in] chan       : DMA通道，值为 DMA_CHAN_*(#DMA_CHAN_0)
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_inmux_dma_mux_set (amhw_lpc82x_inmux_t *p_hw_inmux,
                                    uint8_t              dma_mux,
                                    uint8_t              chan)
{
    p_hw_inmux->dma_mux[dma_mux] = chan;
}

/**
 * \brief 配置SCT输入触发源
 *
 * \param[in] p_hw_inmux : 指向输入复用寄存器块的指针
 * \param[in] chan       : SCT通道
 * \param[in] trig       : 触发源
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_inmux_sct_trig_set (amhw_lpc82x_inmux_t            *p_hw_inmux,
                                     uint8_t                         chan,
                                    amhw_lpc82x_inmux_sct_trig_src_t trig)
{
    p_hw_inmux->sct0_inmux[chan] = trig;
}

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_INMUX_H */

/* end of file */
