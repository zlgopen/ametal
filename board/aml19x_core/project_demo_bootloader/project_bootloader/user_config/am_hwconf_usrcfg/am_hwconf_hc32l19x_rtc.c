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
 * \brief HC32 RTC 用户配置文件
 * \sa am_hwconf_hc32_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_hc32.h"
#include "am_clk.h"
#include "am_hc32_rtc.h"
#include "hw/amhw_hc32_rcc.h"
#include "am_hc32l19x_inst_init.h"
#include "hc32_periph_map.h"
#include "hc32_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32_rtc
 * \copydoc am_hwconf_hc32_rtc.c
 * @{
 */

/** \brief RTC 平台初始化 */
void __hc32_plfm_rtc_init()
{
    /* 外部晶振驱动能力选择 */
    amhw_hc32_rcc_xtl_xtal_driver_set(AMHW_HC32_XTL_XTAL_DRIVER_DEFAULT);

    /* XTL晶振振荡幅度的调整 */
    amhw_hc32_rcc_xtl_xtal_amplitude_set(
    AMHW_HC32_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

    /* 外部高速时钟XTL稳定时间选择 */
    amhw_hc32_rcc_xtl_waittime_set(AMHW_HC32_XTL_WAITTIME_16384);

    amhw_hc32_rcc_set_start(0x5A5A);
    amhw_hc32_rcc_set_start(0xA5A5);
    amhw_hc32_rcc_xtl_enable();

    /* 等待稳定*/
    while(amhw_hc32_rcc_xtl_state_get() == AM_FALSE);

    am_clk_enable(CLK_RTC);

    /* RTC 时钟源选择 */
    amhw_hc32_rtc_clk_src_sel(HC32_RTC, AMHW_HC32_RTC_CLK_SRC_XTL_32768Hz);
}

/** 解除 RTC 平台初始化 */
void __hc32_plfm_rtc_deinit(void)
{
    am_clk_disable(CLK_RTC);
}

/** \brief RTC设备信息 */
const struct am_hc32_rtc_devinfo __g_rtc_devinfo = {

    /** \brief RTC设备基地址 */
    HC32_RTC_BASE,

    /**< \brief RTC 中断号 */
    INUM_RTC,

    /** \brief 从 1970-1-1 0:0:0 开始计算 */
    1970,

    /** \brief 平台初始化函数 */
    __hc32_plfm_rtc_init,

    /** \brief 平台去初始化函数 */
    __hc32_plfm_rtc_deinit
};

/** \brief RTC设备 */
am_hc32_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_hc32_rtc_inst_init (void)
{
    return am_hc32_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_hc32_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_hc32_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */
