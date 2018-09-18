/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X SCT0 用于捕获功能的用户配置文件
 * \sa am_hwconf_lpc82x_sct0_cap.c
 *
 * \internal
 * \par Modification history
 * - 1.02 15-11-19  hgo, add annotations.
 * - 1.01 15-09-24  zxl, modified.
 * - 1.00 15-07-07  oce, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "am_lpc_sct_cap.h"
#include "hw/amhw_lpc82x_inmux.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_sct0_cap
 * \copydoc am_hwconf_lpc82x_sct0_cap.c
 * @{
 */

/** \brief SCT0用于CAP的输出引脚配置信息列表  CAP的输入个数为4个 */
static am_lpc_sct_cap_ioinfo_t __g_sct0_cap_ioinfo_list[] = {

    /** \brief 输入 0 */
    {PIO0_6, PIO_FUNC_SCT_PIN0 , PIO0_6_GPIO | PIO0_6_GPIO_INPUT},

    /** \brief 输入 1 */
    {PIO0_7, PIO_FUNC_SCT_PIN1 , PIO0_7_GPIO | PIO0_7_GPIO_INPUT},

    /** \brief 输入 2 */
    {PIO0_10, PIO_FUNC_SCT_PIN2 , PIO0_10_GPIO | PIO0_10_GPIO_INPUT},

    /** \brief 输入 3 */
    {PIO0_11, PIO_FUNC_SCT_PIN3 , PIO0_11_GPIO | PIO0_11_GPIO_INPUT},
};

/** \brief SCT0用于捕获功能的平台初始化 */
static void __lpc82x_sct0_cap_plfm_init ()
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);

    /**
     * \brief 一共有4个输入通道, 每个通道可选择的输入功能达到8个,
     *        具体的所有功能在amhw_lpc82x_inmux.h文件中关于SCT的定义。
     *        这里只配置一般会用到的, 具体可通过am_lpc82x_sct_cap_set_input
     *        函数输入的引脚
     */
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX, 0, AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN0);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX, 1, AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN1);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX, 2, AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN2);
    amhw_lpc82x_inmux_sct_trig_set(LPC82X_INMUX, 3, AMHW_LPC82X_INMUX_SCTTRIG_SCT_PIN3);
}

/** \brief  解除SCT0平台初始化 */
static void __lpc82x_sct0_cap_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SCT);
}

/** \brief SCT0用于捕获功能的设备信息 */
static const am_lpc_sct_cap_devinfo_t  __g_sct0_cap_devinfo = {
    LPC82X_SCT0_BASE,            /**< \brief SCT0寄存器块的基地址 */
    INUM_SCT0,                   /**< \brief SCT0中断编号           */
    CLK_SCT,                     /**< \brief SCT0时钟编号           */
    4,                           /**< \brief 4个捕获通道            */
    &__g_sct0_cap_ioinfo_list[0],/**< \brief 所有PWM引脚配置信息，用首地址参数传递 */

    __lpc82x_sct0_cap_plfm_init,   /**< \brief 平台初始化函数       */
    __lpc82x_sct0_cap_plfm_deinit, /**< \brief 平台解初始化函数     */
};

/** \brief SCT0用于捕获功能的设备定义 */
static am_lpc_sct_cap_dev_t  __g_sct0_cap_dev;

/** \brief SCT0 CAP 实例初始化，获得CAP标准服务句柄 */
am_cap_handle_t am_lpc82x_sct0_cap_inst_init (void)
{
    return am_lpc_sct_cap_init(&__g_sct0_cap_dev, &__g_sct0_cap_devinfo);
}

/** \brief SCT0 CAP 实例解初始化 */
void am_lpc82x_sct0_cap_inst_deinit (am_cap_handle_t handle)
{
    am_lpc_sct_cap_deinit((am_lpc_sct_cap_dev_t *)handle);
}
/**
 * @}
 */

/* end of file */
