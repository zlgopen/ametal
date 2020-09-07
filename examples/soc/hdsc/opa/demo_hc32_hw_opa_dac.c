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
 * - 操作步骤：
 *   1. 示波器接 J8 PA4 脚
 *   2. 将 ZSL420_EVK USB 接口与电脑连接，打开串口助手
 *
 * - 实验现象：
 *   1. DAC 输出正弦波 经过 OPA 后由 PA4 脚输出
 *
 * \note
 *   1. 观察串口输出信息需要在 am_prj_config.h 中使能 AM_CFG_DEBUG_ENABLE
 *
 * \par 源代码
 * \snippet demo_hc32_hw_opa_dac.c src_hc32_hw_opa_dac
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-10  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_hw_opa_dac
 * \copydoc demo_hc32_hw_opa_dac.c
 */

/** [src_hc32_hw_opa_dac] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_hc32_opa.h"
#include "hw/amhw_hc32_dac.h"
#include "hw/amhw_hc32_opa.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define OPA_CLK      AMHW_HC32_OPA_CLK_16    /**< \brief 校准脉宽 */
#define OPA_AZ_WAY   AMHW_HC32_OPA_AZ_SW     /**< \brief 校准方式 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_hc32_opa_t *gp_hw_opa   = NULL;  /**< \brief OPA 外设 */
static amhw_hc32_dac_t *gp_hw_dac   = NULL;  /**< \brief DAC 外设 */

/**
 * \brief DAC初始化
 */
static void dac_hw_init(amhw_hc32_dac_t *p_hw_dac,
                        uint32_t           verf)
{

    if (p_hw_dac == NULL){
        return ;
    }

    /* 设置参考电压源 */
    amhw_hc32_dac_chan_sref_sel(p_hw_dac, verf);

    /* 失能输出缓冲器*/
    amhw_hc32_dac_chan_output_buf_disable(p_hw_dac);

    /* 触发使能 */
    amhw_hc32_dac_chan_trg_enable(p_hw_dac);

    /* 触发方式选择  软件触发*/
    amhw_hc32_dac_chan_trg_sel(p_hw_dac, AMHW_HC32_DAC_CHAN_SOFTWARE_TRG);

    /* 使能dac通道 */
    amhw_hc32_dac_chan_enable(p_hw_dac);

}

/**
 * \brief OPA初始化
 */
void opa_hw_init (uint8_t mode)
{
    if (mode == AM_HC32_OPA_DAC){

        /* DAC使用OP3单位增加缓存使能 */
        amhw_hc32_opabuf_en (gp_hw_opa);
    }else{
        ;
    }
}

/**
 * \brief OPA DAC电压跟随例程，通过 HW 层接口实现
 */
void demo_hc32_hw_opa_dac_entry (void      *p_hw_opa,
                                 void      *p_hw_dac,
                                 uint8_t    mode,
                                 uint16_t  *p_vol_val,
                                 uint32_t   number)
{
    int i = 0;

    gp_hw_opa = (amhw_hc32_opa_t *)p_hw_opa;
    gp_hw_dac = (amhw_hc32_dac_t *)p_hw_dac;

    /* 使能BGR */
    amhw_hc32_bgr_enable(AM_TRUE);

    /* OPA初始化 */
    opa_hw_init (mode);

    /* DAC 初始化 默认参考电压类型 外部参考电压源  PB01*/
    dac_hw_init(gp_hw_dac, AMHW_HC32_DAC_CHAN_MASK_EXTER_REF);

    AM_FOREVER {

        /* 软件触发 */
        amhw_hc32_dac_chan_software_trg_enable(gp_hw_dac);

        if (i < number) {
            amhw_hc32_dac_chan_12bit_right_aligned_data(gp_hw_dac,
                                                        p_vol_val[i]);
            i++;
        }else{
            i = 0;
        }
        am_mdelay(1000);
    }
}

/* end of file */
