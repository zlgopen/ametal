/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X MRT 用户配置文件
 * \sa am_hwconf_lpc82x_mrt.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-08  mem,modified
 * - 1.00 15-07-15  win, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_mrt.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_mrt
 * \copydoc am_hwconf_lpc82x_mrt.c
 * @{
 */

/**
 * \brief MRT 平台初始化
 */
am_local void __lpc82x_mrt_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_MRT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_MRT);
}

/**
 * \brief MRT 平台解初始化
 */
am_local void __lpc82x_mrt_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_MRT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_MRT);
}

/** \brief MRT 设备信息 */
am_local am_const am_lpc_mrt_devinfo_t __g_lpc82x_mrt_devinfo = {
    LPC82X_MRT_BASE,             /* MRT 寄存器块基址 */
    INUM_MRT,                    /* MRT 中断号 */
    CLK_MRT,                     /* MRT 时钟号 */
    4,                           /* 使用的通道数 */
    __lpc82x_mrt_plfm_init,      /* 平台初始化函数 */
    __lpc82x_mrt_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief MRT 设备实例 */
am_local am_lpc_mrt_dev_t __g_lpc82x_mrt_dev;

/**
 * \brief MRT 实例初始化
 */
am_timer_handle_t am_lpc82x_mrt_inst_init (void)
{
    return am_lpc_mrt_init(&__g_lpc82x_mrt_dev, &__g_lpc82x_mrt_devinfo);
}

/**
 * \brief MRT 实例解初始化
 */
void am_lpc82x_mrt_inst_deinit (am_timer_handle_t handle)
{
    am_lpc_mrt_deinit((am_lpc_mrt_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
