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
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PB00输入模拟电压。对应OPA输出引脚PA04也输出与PB00大小相等电压。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_opa_dac.c src_zsn700_hw_opa_dac
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-10  YRZ, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_opa_dac
 * \copydoc demo_zsn700_hw_opa_dac.c
 */

/** [src_zsn700_hw_opa_dac] */
#include "ametal.h"
#include "am_zsn700.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_zsn700_opa.h"
#include "hw/amhw_zsn700_dac.h"
#include "hw/amhw_zsn700_opa.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define OPA_CLK      AMHW_ZSN700_OPA_CLK_16    /**< \brief 校准脉宽 */
#define OPA_AZ_WAY   AMHW_ZSN700_OPA_AZ_SW     /**< \brief 校准方式 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zsn700_opa_t *gp_hw_opa   = NULL;  /**< \brief OPA 外设 */
static amhw_zsn700_dac_t *gp_hw_dac   = NULL;  /**< \brief DAC 外设 */

/**
 * \brief DAC初始化
 */
static void dac_hw_init(amhw_zsn700_dac_t *p_hw_dac,
                        uint32_t           verf)
{

    if (p_hw_dac == NULL){
        return ;
    }

    /* 设置参考电压源 */
    amhw_zsn700_dac_chan_sref_sel(p_hw_dac, verf);

    /* 失能输出缓冲器*/
    amhw_zsn700_dac_chan_output_buf_disable(p_hw_dac);

    /* 触发使能 */
    amhw_zsn700_dac_chan_trg_enable(p_hw_dac);

    /* 触发方式选择  软件触发*/
    amhw_zsn700_dac_chan_trg_sel(p_hw_dac, AMHW_ZSN700_DAC_CHAN_SOFTWARE_TRG);

    /* 使能dac通道 */
    amhw_zsn700_dac_chan_enable(p_hw_dac);

}

/**
 * \brief OPA初始化
 */
void opa_hw_init (uint8_t mode)
{
    if (mode == AM_ZSN700_OPA_DAC){

        /* DAC使用OP3单位增加缓存使能 */
        amhw_zsn700_opabuf_en (gp_hw_opa);
    }else{
        ;
    }
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_opa_dac_entry (void     *p_hw_opa,
                                   void     *p_hw_dac,
                                   uint8_t   mode,
                                   uint16_t *vol_val)
{
    int i = 0;

    gp_hw_opa = (amhw_zsn700_opa_t *)p_hw_opa;
    gp_hw_dac = (amhw_zsn700_dac_t *)p_hw_dac;

    /* 使能BGR */
    amhw_zsn700_bgr_enable(AM_TRUE);

    /* OPA初始化 */
    opa_hw_init (mode);

    /* DAC 初始化 默认参考电压类型 外部参考电压源  PB01*/
    dac_hw_init(gp_hw_dac, AMHW_ZSN700_DAC_CHAN_MASK_EXTER_REF);

    AM_FOREVER {

        /* 软件触发 */
        amhw_zsn700_dac_chan_software_trg_enable(gp_hw_dac);

        if (i < 128)
        {
            amhw_zsn700_dac_chan_12bit_right_aligned_data (gp_hw_dac,
                                                           vol_val[i]);
            i++;
        }else{
            i = 0;
        }

        am_mdelay(1000);
    }
}

/* end of file */
