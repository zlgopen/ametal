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
 * \brief LPC82X SCT0 CAP 用户配置文件
 * \sa am_hwconf_lpc82x_sct0_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.02 15-11-19  hgo, add annotations.
 * - 1.01 15-09-24  zxl, modified
 * - 1.00 15-07-07  oce, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_sct_cap.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_inmux.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_sct0_cap
 * \copydoc am_hwconf_lpc82x_sct0_cap.c
 * @{
 */

/** \brief SCT0 用于 CAP 的引脚配置信息列表，CAP 的输入个数为 4 个 */
am_local am_lpc_sct_cap_ioinfo_t __g_sct0_cap_ioinfo_list[] = {
    {PIO0_25, PIO_FUNC_SCT_PIN0, PIO0_25_GPIO | PIO0_25_GPIO_INPUT}, /* 通道 0 */
    {PIO0_26, PIO_FUNC_SCT_PIN1, PIO0_26_GPIO | PIO0_26_GPIO_INPUT}, /* 通道 1 */
    {PIO0_27, PIO_FUNC_SCT_PIN2, PIO0_27_GPIO | PIO0_27_GPIO_INPUT}, /* 通道 2 */
    {PIO0_28, PIO_FUNC_SCT_PIN3, PIO0_28_GPIO | PIO0_28_GPIO_INPUT}, /* 通道 3 */
};

/**
 * \brief SCT0 CAP 平台初始化
 */
am_local void __lpc82x_sct0_cap_plfm_init ()
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);

    /*
     * 一共有 4 个输入通道, 每个通道可选择的输入功能达到 8 个,
     * 具体的功能可以参考在 amhw_lpc82x_inmux.h 文件中关于 SCT 的定义
     */
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   0,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN0);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   1,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN1);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   2,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN2);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX,
                                   3,
                                   AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN3);
}

/**
 * \brief SCT0 平台解初始化
 */
am_local void __lpc82x_sct0_cap_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SCT);
}

/** \brief SCT0 CAP 设备信息 */
am_local am_const am_lpc_sct_cap_devinfo_t __g_lpc82x_cap_devinfo = {
    LPC82X_SCT0_BASE,                 /* SCT0 寄存器块基地址 */
    INUM_SCT0,                        /* SCT0 中断号 */
    CLK_SCT,                          /* SCT0 时钟号 */
    4,                                /* 4 个捕获通道 */
    &__g_sct0_cap_ioinfo_list[0],     /* 所有 PWM 引脚配置信息，用首地址参数传递 */
    __lpc82x_sct0_cap_plfm_init,      /* 平台初始化函数 */
    __lpc82x_sct0_cap_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief SCT0 CAP 设备实例 */
am_local am_lpc_sct_cap_dev_t __g_lpc82x_cap_dev;

/**
 * \brief SCT0 CAP 实例初始化
 */
am_cap_handle_t am_lpc82x_sct0_cap_inst_init (void)
{
    return am_lpc_sct_cap_init(&__g_lpc82x_cap_dev, &__g_lpc82x_cap_devinfo);
}

/**
 * \brief SCT0 CAP 实例解初始化
 */
void am_lpc82x_sct0_cap_inst_deinit (am_cap_handle_t handle)
{
    am_lpc_sct_cap_deinit((am_lpc_sct_cap_dev_t *)handle);
}

/**
 * @}
 */

/* end of file */
