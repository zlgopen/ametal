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
 * \brief ZLG117 RTC 用户配置文件
 * \sa am_hwconf_mm32l373_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-28  lqy, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_mm32l373.h"
#include "am_mm32l373_rtc.h"
#include "amhw_mm32l373_rcc.h"
#include "am_mm32l373_inst_init.h"
#include "amhw_mm32_pwr.h"
#include "mm32l373_periph_map.h"


#define RTC_CLK_SOUR  AMHW_MM32L373_RTCCLK_HSE_DIV128
/**
 * \addtogroup am_if_src_hwconf_mm32l373_rtc
 * \copydoc am_hwconf_mm32l373_rtc.c
 * @{
 */

/** \brief RTC 平台初始化 */
void __mm32l373_plfm_rtc_init()
{
    amhw_mm32l373_rcc_apb1_enable(AMHW_MM32L373_RCC_APB1_PWR); /* 使能电源时钟 */
    amhw_mm32l373_rcc_apb1_enable(AMHW_MM32L373_RCC_APB1_BKP); /* 使能备份时钟 */
    amhw_mm32_pwr_bkp_access_enable(MM32L373_PWR,1);          /* 取消备份域的写保护 */
    amhw_mm32l373_rcc_bdcr_bdrst_reset();                    /* 备份区域软件复位 */
    am_udelay(5);
    amhw_mm32l373_rcc_bdcr_bdrst_reset_end();                /* 备份域软件复位结束 */
    amhw_mm32l373_rcc_bdcr_rtc_clk_set((amhw_mm32l373_rtc_clk_src)RTC_CLK_SOUR);/* RTC 时钟源选择为外部RTC时钟源 */
    am_mdelay(1);
    amhw_mm32l373_rcc_bdcr_rtc_enable();                     /* RTC时钟使能 */

}

/** 解除 RTC 平台初始化 */
void __mm32l373_plfm_rtc_deinit(void)
{
    amhw_mm32l373_rcc_apb1_disable(AMHW_MM32L373_RCC_APB1_PWR); /* 禁能电源时钟 */
    amhw_mm32l373_rcc_apb1_disable(AMHW_MM32L373_RCC_APB1_BKP); /* 禁能备份时钟 */
    amhw_mm32_pwr_bkp_access_enable(MM32L373_PWR,0);           /* 备份域的写保护 */
    amhw_mm32l373_rcc_bdcr_rtc_disable();                     /* RTC时钟禁能 */
}

/** \brief RTC设备信息 */
const struct am_mm32l373_rtc_devinfo __g_rtc_devinfo = {

    /** \brief RTC设备基地址 */
    MM32L373_RTC_BASE,

	/** \brief 电源控制PWR基地址 */
	MM32L373_PWR_BASE,

	/** \brief 备份控制BKP基地址 */
	MM32L373_BKP_BASE,

	/**< \brief RTC 中断号 */
	INUM_RTC,

    /** \brief RTC设备时钟源 */
	RTC_CLK_SOUR,

    /** \brief 平台初始化函数 */
    __mm32l373_plfm_rtc_init,

    /** \brief 平台去初始化函数 */
    __mm32l373_plfm_rtc_deinit
};

/** \brief RTC设备 */
am_mm32l373_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_mm32l373_rtc_inst_init (void)
{
    return am_mm32l373_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_mm32l373_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_mm32l373_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */
