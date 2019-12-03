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
 * \brief MM32L073电源管理(低功耗模式配置文件)
 * \sa am_hwconf_mm32l073_pwr.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-04-18  nwt, first implementation.
 * \endinternal
 */

#include "am_mm32l073.h"
#include "am_mm32l073_pwr.h"
#include "am_mm32l073_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l073_pwr
 * \copydoc am_hwconf_mm32l073_pwr.c
 * @{
 */

/** \brief PWR平台初始化 */
void __mm32l073_plfm_pwr_init (void)
{

    /* PWR电源接口时钟使能 */
    am_clk_enable(CLK_PWR);

    /* 复位PWR电源接口 */
    am_mm32l073_clk_reset(CLK_PWR);
}

/** \brief PWR平台去初始化 */
void __mm32l073_plfm_pwr_deinit (void)
{

    /* 复位PWR电源接口 */
    am_mm32l073_clk_reset(CLK_PWR);

    /* 禁能PWR相关外设时钟 */
    am_clk_disable(CLK_PWR);

}
/** \brief 引脚触发信息内存 */
static struct am_mm32l073_pwr_mode_init __g_pwr_mode_init[3] = {
     {AM_MM32L073_PWR_MODE_SLEEP,   PIOA_8},
     {AM_MM32L073_PWR_MODE_STOP,    PIOA_8},
     {AM_MM32L073_PWR_MODE_STANBY,  PIOA_0},
};

/** \brief PVD触发信息内存 */
static struct am_mm32l073_pwr_pvd_info __g_pwr_pvd_info = {
     AMHW_MM32_PVD_LEVER_3_3V,    /**< \brief PVD监控电压 */
     AM_TRUE,                    /**< \brief 使能监控电压 */
     2,                          /**< \brief 监控电压模式 */
};

/** \brief PWR设备信息 */
const am_mm32l073_pwr_devinfo_t __g_pwr_devinfo = {
    MM32L073_PWR_BASE,        /**< \brief PWR控制器寄存器块基址 */
    MM32L073_BKP_BASE,        /**< \brief BKP配置寄存器块基址 */
	MM32L073_EXTI_BASE,       /**< \brief EXTI寄存器块基址 */
    CLK_AHB,                /**< \brief AHB时钟ID */
    CLK_APB1,               /**< \brief APB1时钟ID */
    CLK_APB2,               /**< \brief APB2时钟ID */
    INUM_PVD,               /**< \brief pvd中断ID */
    __g_pwr_mode_init,      /**< \brief 指向pwr功耗模式数组的指针 */
    &__g_pwr_pvd_info,      /**< \brief 指向pvd电压监控信息的指针 */
    __mm32l073_plfm_pwr_init,
    __mm32l073_plfm_pwr_deinit
};

/** \brief PWR设备实例 */
am_mm32l073_pwr_dev_t __g_pwr_dev;

/** \brief PWR 实例初始化 */
am_mm32l073_pwr_handle_t am_mm32l073_pwr_inst_init (void)
{
    return am_mm32l073_pwr_init(&__g_pwr_dev, &__g_pwr_devinfo);
}

/** \brief PWR 实例解初始化 */
void am_mm32l073_pwr_inst_deinit (void)
{
    am_mm32l073_pwr_deinit();
}

/**
 * @}
 */

/* end of file */
