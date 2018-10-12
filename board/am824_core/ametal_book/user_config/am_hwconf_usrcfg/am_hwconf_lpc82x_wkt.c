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
 * \brief LPC82X WKT 用户配置文件
 * \sa am_hwconf_lpc82x_wkt.c
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-02  sky, modified
 * - 1.00 15-07-08  zxl, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_wkt.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_pmu.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_wkt
 * \copydoc am_hwconf_lpc82x_wkt.c
 * @{
 */

/**
 * \brief WKT 平台初始化
 */
am_local void __lpc82x_wkt_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_WKT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_WKT);
}

/**
 * \brief WKT 平台解初始化
 */
am_local void __lpc82x_wkt_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_WKT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_WKT);
}

/**
 * \brief WKT 平台时钟初始化
 */
am_local
void __lpc82x_wkt_plfm_clk_init (am_const am_lpc_wkt_devinfo_t *p_devinfo)
{
    amhw_lpc_wkt_t *p_hw_wkt = (amhw_lpc_wkt_t *)(p_devinfo->wkt_regbase);

    if (p_devinfo == NULL) {
        return;
    }

    switch (p_devinfo->clksel_str) {

    case AMHW_LPC_WKT_IRC_CLOCK:

        /* 设置 WKT 时钟源为内部时钟 */
        amhw_lpc_wkt_clksel_cfg(p_hw_wkt, AMHW_LPC_WKT_IRC_CLOCK);
        break;

    case AMHW_LPC_WKT_LOW_POWER_CLOCK:

        /* 使能低功耗振荡器 */
        amhw_lpc82x_pmu_lposcen_enable(LPC82X_PMU);

        /* 设置 WKT 时钟源为低功耗振荡器 */
        amhw_lpc_wkt_clksel_cfg(p_hw_wkt, AMHW_LPC_WKT_LOW_POWER_CLOCK);
        break;

    case AMHW_LPC_WKT_EXT_CLOCK:

        /* WAKEUPCLKHYS 引脚迟滞使能 */
        amhw_lpc82x_pmu_wakeupclkhys_enable(LPC82X_PMU);

        /* WAKECLKPAD 引脚使能(使能 PIO0_28 外部时钟功能) */
        amhw_lpc82x_pmu_wakeclkpad_enable(LPC82X_PMU);

        /* 设置 WKT 时钟源为外部时钟 */
        amhw_lpc_wkt_clksel_cfg(p_hw_wkt, AMHW_LPC_WKT_EXT_CLOCK);
        break;
    }
}

/** \brief WKT 设备信息 */
am_local am_const am_lpc_wkt_devinfo_t __g_lpc82x_wkt_devinfo = {
    LPC82X_WKT_BASE,                 /* WKT 寄存器块基地址 */
    INUM_WKT,                        /* WKT 中断号 */
    1,                               /* 支持 1 个通道 */
    AMHW_LPC_WKT_LOW_POWER_CLOCK,    /* 选择 IRC 时钟 */
    0,                               /* 使用外部时钟时的频率设置 */
    __lpc82x_wkt_plfm_clk_init,      /* 平台时钟初始化函数 */
    __lpc82x_wkt_plfm_init,          /* 平台初始化函数 */
    __lpc82x_wkt_plfm_deinit         /* 平台解初始化函数 */
};

/** \brief WKT 设备实例 */
am_local am_lpc_wkt_dev_t __g_lpc82x_wkt_dev;

/**
 * \brief WKT 实例初始化
 */
am_timer_handle_t am_lpc82x_wkt_inst_init (void)
{
    return am_lpc_wkt_init(&__g_lpc82x_wkt_dev, &__g_lpc82x_wkt_devinfo);
}

/**
 * \brief WKT 实例解初始化
 */
void am_lpc82x_wkt_inst_deinit (am_timer_handle_t handle)
{
    am_lpc_wkt_deinit(handle);
}

/**
 * @}
 */

/* end of file */
