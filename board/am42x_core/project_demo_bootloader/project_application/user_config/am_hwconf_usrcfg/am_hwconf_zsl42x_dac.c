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
 * \brief ZLG DAC 用户配置文件
 * \sa am_hwconf_zlg217_dac.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-24
 * \endinternal
 */

#include "am_zsl42x.h"
#include "am_zsl42x_dac.h"
#include "am_gpio.h"
#include "zsl42x_pin.h"
#include "am_clk.h"
#include "hw/amhw_zsl42x_dac.h"

/**
 * \addtogroup am_if_src_hwconf_zsl42x_dac
 * \copydoc am_hwconf_zsl42x_dac.c
 * @{
 */

/**
 * \brief DAC0 平台初始化
 */
void __zsl42x_plfm_dac0_init (void)
{

    /* 配置PIOA_4为DAC1_OUT功能 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_AOUT);

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);
}

/**
 * \brief DAC0 平台去初始化
 */
void __zsl42x_plfm_dac0_deinit (void)
{
    /* 关闭DAC时钟 */
    am_clk_disable(CLK_DAC);
}

/** \brief DAC0 设备信息 */
static const am_zsl42x_dac_devinfo_t __g_dac0_devinfo =
{
    ZSL42x_DAC_BASE,                      /**< \brief 指向DAC寄存器块的指针 */

    12,                                   /**< \brief DAC转换精度 */

    AMHW_ZSL42x_DAC_CHAN_MASK_EXTER_REF,  /**< \brief DAC参考电压源 */

    3300,                                 /**< \brief DAC参考电压,单位：mv*/

    AM_ZSL42x_DAC_ALIGN_WAY_12_RIGHT,     /**< \brief DAC数据对齐方式 */

    __zsl42x_plfm_dac0_init,              /**< \brief DAC平台初始化函数 */

    __zsl42x_plfm_dac0_deinit,            /**< \brief DAC平台解初始化函数  */
};

/** \brief DAC0设备实例 */
static am_zsl42x_dac_dev_t __g_dac0_dev;

/** \brief DAC0 实例初始化，获得DAC标准服务句柄 */
am_dac_handle_t am_zsl42x_dac_inst_init (void)
{
    return am_zsl42x_dac_init(&__g_dac0_dev, &__g_dac0_devinfo);
}

/** \brief DAC0 实例解初始化 */
void am_zsl42x_dac_inst_deinit (am_dac_handle_t handle)
{
    am_zsl42x_dac_deinit(handle);
}

/**
 * @}
 */

/* end of file */
