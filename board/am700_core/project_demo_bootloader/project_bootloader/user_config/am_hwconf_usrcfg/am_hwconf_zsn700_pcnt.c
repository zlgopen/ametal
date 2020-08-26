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
 * \brief ZSN700 PCNT 用户配置文件
 * \sa am_hwconf_zsn700_pcnt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_zsn700.h"
#include "zsn700_clk.h"
#include "zsn700_inum.h"
#include "zsn700_regbase.h"
#include "am_zsn700_clk.h"
#include "am_zsn700_pcnt.h"

/**
 * \addtogroup am_if_src_hwconf_zsn700_pcnt
 * \copydoc am_hwconf_zsn700_pcnt.c
 * @{
 */

/**
 * \brief PCNT 平台初始化函数
 */
am_local void __zsn700_pcnt_plfm_init (void)
{
    am_gpio_pin_cfg(PIOB_5, PIOB_5_INPUT_FLOAT | PIOB_5_PCNT_S0);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_INPUT_FLOAT | PIOB_7_PCNT_S1);

    am_clk_enable(CLK_PCNT);
}

/**
 * \brief PCNT 平台解初始化函数
 */
am_local void __zsn700_pcnt_plfm_deinit (void)
{
    am_clk_disable(CLK_PCNT);
}

/** \brief PCNT 设备信息 */
am_local am_const am_zsn700_pcnt_devinfo_t __g_zsn700_pcnt_devinfo = {
        ZSN700_PCNT_BASE,
        CLK_PCNT,
        INUM_PCNT,
        __zsn700_pcnt_plfm_init,
        __zsn700_pcnt_plfm_deinit
};

/** \brief PCNT 设备实例 */
am_local am_zsn700_pcnt_dev_t __g_zsn700_pcnt_dev;

/**
 * \brief PCNT 实例初始化
 */
am_zsn700_pcnt_handle_t am_zsn700_pcnt_inst_init (void)
{
    return am_zsn700_pcnt_init(&__g_zsn700_pcnt_dev, &__g_zsn700_pcnt_devinfo);
}

/**
 * \brief PCNT 实例解初始化
 */
void am_zsn700_pcnt_inst_deinit (am_zsn700_pcnt_handle_t handle)
{
    am_zsn700_pcnt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
