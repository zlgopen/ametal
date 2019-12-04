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
 * \brief HK32F103RBT6 RTC 用户配置文件
 * \sa am_hwconf_hk32f103rbt6_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-23  zp, first implementation
 * \endinternal
 */

#include "am_hk32f103rbt6.h"
#include "am_hk32f103rbt6_inst_init.h"
#include "am_hk32f103rbt6_rtc.h"
#include "amhw_hk32f103rbt6_pwr.h"
#include "amhw_hk32f103rbt6_rcc.h"
#include "hk32f103rbt6_periph_map.h"
#include "ametal.h"
#include "am_delay.h"


#define RTC_CLK_SOUR  AMHW_HK32F103RBT6_RTCCLK_HSE_DIV128
/**
 * \addtogroup am_if_src_hwconf_hk32f103rbt6_rtc
 * \copydoc am_hwconf_hk32f103rbt6_rtc.c
 * @{
 */

/** \brief RTC 平台初始化 */
void __hk32f103rbt6_plfm_rtc_init()
{
    amhw_hk32f103rbt6_rcc_apb1_enable(AMHW_HK32F103RBT6_RCC_APB1_PWR); /* 使能电源时钟 */
    amhw_hk32f103rbt6_rcc_apb1_enable(AMHW_HK32F103RBT6_RCC_APB1_BKP); /* 使能备份时钟 */
    amhw_hk32f103rbt6_pwr_bkp_access_enable(HK32F103RBT6_PWR,1);       /* 取消备份域的写保护 */
    amhw_hk32f103rbt6_rcc_bdcr_bdrst_reset();                    /* 备份区域软件复位 */
    am_udelay(5);
    amhw_hk32f103rbt6_rcc_bdcr_bdrst_reset_end();                /* 备份域软件复位结束 */

    /* RTC 时钟源选择为外部RTC时钟源 */
    amhw_hk32f103rbt6_rcc_bdcr_rtc_clk_set((amhw_hk32f103rbt6_rtc_clk_src)RTC_CLK_SOUR);
    am_mdelay(1);
    amhw_hk32f103rbt6_rcc_bdcr_rtc_enable();                     /* RTC时钟使能 */

}

/** 解除 RTC 平台初始化 */
void __hk32f103rbt6_plfm_rtc_deinit(void)
{
    amhw_hk32f103rbt6_rcc_apb1_disable(AMHW_HK32F103RBT6_RCC_APB1_PWR); /* 禁能电源时钟 */
    amhw_hk32f103rbt6_rcc_apb1_disable(AMHW_HK32F103RBT6_RCC_APB1_BKP); /* 禁能备份时钟 */
    amhw_hk32f103rbt6_pwr_bkp_access_enable(HK32F103RBT6_PWR,0);        /* 备份域的写保护 */
    amhw_hk32f103rbt6_rcc_bdcr_rtc_disable();                     /* RTC时钟禁能 */
}

/** \brief RTC设备信息 */
const struct am_hk32f103rbt6_rtc_devinfo __g_rtc_devinfo = {

    /** \brief RTC设备基地址 */
    HK32F103RBT6_RTC_BASE,

    /** \brief 电源控制PWR基地址 */
    HK32F103RBT6_PWR_BASE,

    /** \brief 备份控制BKP基地址 */
    HK32F103RBT6_BKP_BASE,

    /**< \brief RTC 中断号 */
    INUM_RTC,

    /** \brief RTC设备时钟源 */
    RTC_CLK_SOUR,

    /** \brief 平台初始化函数 */
    __hk32f103rbt6_plfm_rtc_init,

    /** \brief 平台去初始化函数 */
    __hk32f103rbt6_plfm_rtc_deinit
};

/** \brief RTC设备 */
am_hk32f103rbt6_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_hk32f103rbt6_rtc_inst_init (void)
{
    return am_hk32f103rbt6_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_hk32f103rbt6_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_hk32f103rbt6_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */
