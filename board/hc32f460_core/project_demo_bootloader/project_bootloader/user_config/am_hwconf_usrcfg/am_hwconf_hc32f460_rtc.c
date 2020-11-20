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
 * \brief HC32F460 RTC 用户配置文件
 * \sa am_hwconf_hc32f460_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_hc32f460.h"
#include "am_clk.h"
#include "am_hc32f460_rtc.h"
#include "hw/amhw_hc32f460_rcc.h"
#include "am_hc32f460_inst_init.h"
#include "hc32f460_periph_map.h"
#include "hc32f460_clk.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_rtc
 * \copydoc am_hwconf_hc32f460_rtc.c
 * @{
 */

/** \brief RTC 平台初始化 */
void __hc32f460_plfm_rtc_init()
{
    am_clk_enable(CLK_XTAL32);
}

/** 解除 RTC 平台初始化 */
void __hc32f460_plfm_rtc_deinit(void)
{
    am_clk_disable(CLK_XTAL32);
}

/** \brief RTC设备信息 */
const struct am_hc32f460_rtc_devinfo __g_rtc_devinfo = {

    /** \brief RTC设备基地址 */
    HC32F460_RTC_BASE,

    /**< \brief RTC 中断号 */
    INUM_RTC,

    /** \brief 平台初始化函数 */
    __hc32f460_plfm_rtc_init,

    /** \brief 平台去初始化函数 */
    __hc32f460_plfm_rtc_deinit
};

/** \brief RTC设备 */
am_hc32f460_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_hc32f460_rtc_inst_init (void)
{
    return am_hc32f460_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_hc32f460_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_hc32f460_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */
