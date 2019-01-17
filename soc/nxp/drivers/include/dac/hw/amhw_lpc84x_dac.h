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
 * \brief 数模转换（DAC）操作接口
 * 1. 片上可编程输出产生器(电压输出从 1mv 到  3300mv)
 * 2. 在正常停机模式的静态操作
 * 3. 支持 DMA
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-26  mkr, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC_DAC_H
#define __AMHW_LPC_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup amhw_fsl_if_dac
 * \copydoc amhw_fsl_dac.h
 * @{
 */

/**
 * \brief dac 寄存器块结构体
 */
typedef struct amhw_lpc_dac {
    __IO uint32_t cr;            /**< \brief DAC控制寄存器0     */
    __IO uint32_t ctrl;          /**< \brief DAC控制寄存器 1     */
    __IO uint32_t cntval;        /**< \brief DAC控制寄存器 2     */
} amhw_lpc_dac_t;


/**
 * \brief DAC 通道编号
 */
#define AMHW_LPC_DAC0_CHAN_0          0      /**< \brief DAC 通道 0 */

/**
 * \brief DAC 缓冲区大小
 */
#define AMHW_LPC_DAC_DATL_COUNT       2      /**< \brief DAC 缓存区大小 */


/**
 * \name DAC数据寄存器设置位宏定义
 * @{
 */

/**
 * \brief DAC 参考电压选择索引
 */
typedef enum amhw_lpc_dac_bias_sel {
    AMHW_LPC84X_DAC_UPDATA_1MHZ = 0,                    /**< \brief  DACREF_1 作为参考电压 */
	AMHW_LPC84X_DAC_UPDATA_400KHZ,                    /**< \brief  DACREF_2 作为参考电压 */
} amhw_lpc_dac_vref_sel_t;


/** \brief DAC使能                                                        */

#define AMHW_LPC84x_DAC_DBLBUF_EN       1
#define AMHW_LPC84x_DAC_DBLBUF_DISEN    0

#define AMHW_LPC84x_DAC_CNT_EN          1
#define AMHW_LPC84x_DAC_CNT_DISEN       0

#define AMHW_LPC84x_DAC_DMA_EN          1
#define AMHW_LPC84x_DAC_DMA_DISEN       0
/** @} */

/**
 * \brief 配置DAC输出值函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] value    : 输出电压为(VREFP) * value/1024
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_cr_set (amhw_lpc_dac_t *p_hw_dac, uint32_t value)
{

    uint32_t reg = p_hw_dac->cr;

    AM_BIT_CLR_MASK(reg, AM_SBF(0x3ff, 6));
    AM_BIT_SET_MASK(reg, AM_SBF(value, 6));

    p_hw_dac->cr = reg;
}


/**
 * \brief 配置DAC计数值函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] value    : 计数值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_cntval_set (amhw_lpc_dac_t *p_hw_dac, uint32_t value)
{
    p_hw_dac->cntval = value - 1;
}


/**
 * \brief bias功能选择
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] bias      : AMHW_LPC84X_DAC_UPDATA_1MHZ 或者 AMHW_LPC84X_DAC_UPDATA_1MHZ
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_bias_set (amhw_lpc_dac_t *p_hw_dac, uint8_t bias)
{
	bias ? AM_BIT_SET(p_hw_dac->cr, 16):AM_BIT_CLR(p_hw_dac->cr, 16);
}

/**
 * \brief 是否开启DMA中断
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] int_dma   : AMHW_LPC84x_DAC_DMA_EN 或者 AMHW_LPC84x_DAC_DMA_DIS
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_dma_int_req_set (amhw_lpc_dac_t *p_hw_dac,uint8_t int_dma)
{
	int_dma ? AM_BIT_SET(p_hw_dac->ctrl, 0):AM_BIT_CLR(p_hw_dac->ctrl, 0);
}

/**
 * \brief 是否开启dblbuf
 *
 * \param[in] p_hw_dac  : 指向DAC寄存器块的指针
 * \param[in] dblbuf    : AMHW_LPC84x_DAC_DBLBUF_EN 或者 AMHW_LPC84x_DAC_DBLBUF_DIS
 * \param[in] dat       : dat值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_dblbuf_set (amhw_lpc_dac_t *p_hw_dac,uint8_t dblbuf)
{
	dblbuf ? AM_BIT_SET(p_hw_dac->ctrl, 1):AM_BIT_CLR(p_hw_dac->ctrl, 1);
}



/**
 * \brief 是否开启计数中断
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 * \param[in] cnt      ： AMHW_LPC84x_DAC_CNT_EN 或者 AMHW_LPC84x_DAC_CNT_EN
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_cnt_set (amhw_lpc_dac_t *p_hw_dac,uint8_t cnt)
{
	cnt ? AM_BIT_SET(p_hw_dac->ctrl, 2):AM_BIT_CLR(p_hw_dac->ctrl, 2);
}


/**
 * \brief 禁能DAC功能函数
 *
 * \param[in] p_hw_dac : 指向DAC寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_lpc84x_dac_cnt_en_set (amhw_lpc_dac_t *p_hw_dac,uint8_t dma_en)
{
	dma_en ? AM_BIT_SET(p_hw_dac->ctrl, 3):AM_BIT_CLR(p_hw_dac->ctrl, 3);
}

#endif /* __AMHW_FSL_DAC_H */

/* end of file */
