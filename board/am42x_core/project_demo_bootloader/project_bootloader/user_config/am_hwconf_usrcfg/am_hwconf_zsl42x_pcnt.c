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
 * \brief ZSL42x PCNT 用户配置文件
 * \sa am_hwconf_zsl42x_pcnt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-12
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_zsl42x.h"
#include "zsl42x_clk.h"
#include "zsl42x_inum.h"
#include "zsl42x_regbase.h"
#include "am_zsl42x_clk.h"
#include "am_zsl42x_pcnt.h"

/**
 * \addtogroup am_if_src_hwconf_zsl42x_pcnt
 * \copydoc am_hwconf_zsl42x_pcnt.c
 * @{
 */

/**
 * \brief PCNT 平台初始化函数
 */
am_local void __zsl42x_pcnt_plfm_init (void)
{
    am_gpio_pin_cfg(PIOB_5, PIOB_5_INPUT_FLOAT | PIOB_5_PCNT_S0);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_INPUT_FLOAT | PIOB_7_PCNT_S1);

    am_clk_enable(CLK_PCNT);
}

/**
 * \brief PCNT 平台解初始化函数
 */
am_local void __zsl42x_pcnt_plfm_deinit (void)
{
    am_clk_disable(CLK_PCNT);
}

/** \brief PCNT 设备信息 */
am_local am_const am_zsl42x_pcnt_devinfo_t __g_zsl42x_pcnt_devinfo = {
        ZSL42x_PCNT_BASE,
        CLK_PCNT,
        INUM_PCNT,
        __zsl42x_pcnt_plfm_init,
        __zsl42x_pcnt_plfm_deinit
};

/** \brief PCNT 设备实例 */
am_local am_zsl42x_pcnt_dev_t __g_zsl42x_pcnt_dev;

/**
 * \brief PCNT 实例初始化
 */
am_zsl42x_pcnt_handle_t am_zsl42x_pcnt_inst_init (void)
{
    return am_zsl42x_pcnt_init(&__g_zsl42x_pcnt_dev, &__g_zsl42x_pcnt_devinfo);
}

/**
 * \brief PCNT 实例解初始化
 */
void am_zsl42x_pcnt_inst_deinit (am_zsl42x_pcnt_handle_t handle)
{
    am_zsl42x_pcnt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
