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
 * \brief ZLG118 RTC 用户配置文件
 * \sa am_hwconf_zlg118_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_zlg118.h"
#include "am_clk.h"
#include "am_zlg118_rtc.h"
#include "hw/amhw_zlg118_rcc.h"
#include "am_zlg118_inst_init.h"
#include "zlg118_periph_map.h"
#include "zlg118_clk.h"

/**
 * \addtogroup am_if_src_hwconf_zlg118_rtc
 * \copydoc am_hwconf_zlg118_rtc.c
 * @{
 */

/** \brief RTC 平台初始化 */
void __zlg118_plfm_rtc_init()
{
    am_clk_enable(CLK_RTC);

    am_gpio_pin_cfg(PIOB_14, PIOB_14_RTC_1HZ | PIOB_14_OUT_PP);
}

/** 解除 RTC 平台初始化 */
void __zlg118_plfm_rtc_deinit(void)
{
    am_clk_disable(CLK_RTC);

    am_gpio_pin_cfg(PIOB_14, PIOB_14_GPIO | PIOB_14_INPUT_PU);
}

/** \brief RTC设备信息 */
const struct am_zlg118_rtc_devinfo __g_rtc_devinfo = {

    /** \brief RTC设备基地址 */
    ZLG118_RTC_BASE,

    /** \brief RTC设备时钟源 */
    AMHW_ZLG118_RTC_CLK_SRC_XTL_32768Hz,

    /**< \brief RTC 中断号 */
    INUM_RTC,

    {
        AM_TRUE,                      /* 是否使能1Hz输出功能
                                       * AM_TRUE ： 使能 （请阅读下面两项配置的注释）
                                       * AM_FALSE： 禁能（下面两项配置无效）
                                       */
        -92.554,                      /* 误差补偿值（-274.6 ~ 212.6）ppm，范围外代表关闭*/
        AM_TRUE,                      /* 在ppm处于范围内时（即开启补偿的情况下），
                                       * 选择是否开启高速时钟作为补偿时钟
                                       *
                                       * AM_TRUE ： 开启高速时钟作为补偿时钟
                                       * AM_FALSE： 关闭高速时钟作为补偿时钟
                                       */
        AMHW_ZLG118_RTCCLK_ADJUST_24M,/* 高速时钟补偿时钟选择 ，和PCLK频率保持一致相同  */
    },

    /** \brief 平台初始化函数 */
    __zlg118_plfm_rtc_init,

    /** \brief 平台去初始化函数 */
    __zlg118_plfm_rtc_deinit
};

/** \brief RTC设备 */
am_zlg118_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_zlg118_rtc_inst_init (void)
{
    return am_zlg118_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_zlg118_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_zlg118_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */
