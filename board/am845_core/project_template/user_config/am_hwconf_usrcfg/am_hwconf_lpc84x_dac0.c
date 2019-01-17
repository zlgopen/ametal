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
 * \brief lpc845 DAC 用户配置文件。
 * \sa am_lpc845_hwconfig_dac.c
 *s
 * \internal
 * \par Modification history
 * - 1.00 16-09-26  mkr, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc84x.h"
#include <am_lpc84x_dac.h>
#include <hw/amhw_lpc84x_dac.h>
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc84x_iocon.h"

/**
 * \addtogroup am_lpc845_if_hwconfig_src_dac
 * \copydoc am_lpc845_hwconfig_dac.c
 * @{
 */

/**
 * \brief DAC 平台初始化。
 */
void __lpc84x_plfm_dac0_init (void)
{
    /* 配置PIOE_30为DAC0_OUT功能                 */
	amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_DAC0);
	amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_DAC0);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_SWM);
    amhw_lpc84x_swm_fixed_func_enable(LPC84X_SWM,
    		                          AMHW_LPC84X_SWM_PIO0_17_DACOUT0);
    amhw_lpc84x_iocon_mode_set(LPC84X_IOCON,
                               PIO0_17,
                               AMHW_LPC84X_IOCON_MODE_INACTIVE);
    amhw_lpc84x_iocon_dac_mode_set(LPC84X_IOCON,
                                   PIO0_17,
								   AMHW_LPC84X_IOCON_DAC_MODE_ENABLE);
    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_DAC0);
}

/**
 * \brief DAC 平台去初始化。
 */
void __lpc84x_plfm_dac0_deinit (void)
{
   /* 关闭DAC时钟                  */
	amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_DAC0);

}

/** \brief DAC 设备信息 */
static const am_lpc_dac_devinfo_t _g_dac0_devinfo =
{
	LPC84X_DAC0,                   /**< \brief 指向DAC0寄存器块的指针 */
	INUM_DAC0,                     /**< \brief DAC0中断号             */
    10,                            /**< \brief DAC转换精度            */
    3300,                          /**< \brief DAC参考电压，单位：mV  */
	__lpc84x_plfm_dac0_init,
	__lpc84x_plfm_dac0_deinit,
};

/** \brief DAC0设备实例 */
static am_lpc_dac_dev_t _g_dac0_dev;

/** \brief DAC0 实例初始化，获得DAC标准服务句柄 */
am_dac_handle_t am_lpc84x_dac0_inst_init (void)
{
    return am_lpc_dac_init(&_g_dac0_dev, &_g_dac0_devinfo);
}

/** \brief DAC0 实例解初始化 */
void am_lpc84x_dac0_inst_deinit (am_dac_handle_t handle)
{
    am_lpc_dac_deinit(handle);
}


/**
 * @}
 */

/* end of file */
