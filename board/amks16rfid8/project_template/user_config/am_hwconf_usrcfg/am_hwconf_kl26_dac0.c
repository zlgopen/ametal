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
 * \brief KL26 DAC 用户配置文件。
 * \sa am_kl26_hwconfig_dac.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-26  mkr, first implementation.
 * \endinternal
 */

#include "am_kl26.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fsl_dac.h"
#include "am_gpio.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_dac
 * \copydoc am_kl26_hwconfig_dac.c
 * @{
 */

/**
 * \brief DAC 平台初始化。
 */
void __kl26_plfm_dac0_init (void)
{
    /* 配置PIOE_30为DAC0_OUT功能                 */
     am_gpio_pin_cfg (PIOE_30,PIOE_30_DAC0_OUT);

     /* 开启DAC时钟                  */
     amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_DAC0);
}

/**
 * \brief DAC 平台去初始化。
 */
void __kl26_plfm_dac0_deinit (void)
{
   /* 关闭DAC时钟                  */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_DAC0);
}

/** \brief DAC 设备信息 */
static const am_fsl_dac_devinfo_t _g_dac0_devinfo =
{
    KL26_DAC0,                /**< \brief 指向DAC0寄存器块的指针 */
    INUM_DAC0,                     /**< \brief DAC0中断号             */
    12,                            /**< \brief DAC转换精度            */
    3270,                          /**< \brief DAC参考电压，单位：mV  */
    AMHW_FSL_DAC_VREF_2,          /**< \brief DAC电压源              */
    __kl26_plfm_dac0_init,
    __kl26_plfm_dac0_deinit,
};

/** \brief DAC0设备实例 */
static am_fsl_dac_dev_t _g_dac0_dev;

/** \brief DAC0 实例初始化，获得DAC标准服务句柄 */
am_dac_handle_t am_kl26_dac0_inst_init (void)
{
    return am_fsl_dac_init(&_g_dac0_dev, &_g_dac0_devinfo);
}

/** \brief DAC0 实例解初始化 */
void am_kl26_dac0_inst_deinit (am_dac_handle_t handle)
{
    am_fsl_dac_deinit(handle);
}


/**
 * @}
 */

/* end of file */
