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
 * \brief ZLG237 电源管理(低功耗模式)用户配置文件
 * \sa am_hwconf_zlg237_pwr.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-13  fra, first implementation
 * \endinternal
 */

#include <am_zlg237.h>
#include <am_zlg237_clk.h>
#include <am_zlg237_pwr.h>

/**
 * \addtogroup am_if_src_hwconf_zlg237_pwr
 * \copydoc am_hwconf_zlg237_pwr.c
 * @{
 */

/** \brief PWR平台初始化 */
void __zlg237_plfm_pwr_init (void)
{
    am_clk_enable(CLK_PWR);
    am_zlg237_clk_reset(CLK_PWR);
}

/** \brief PWR平台去初始化 */
void __zlg237_plfm_pwr_deinit (void)
{
    am_zlg237_clk_reset(CLK_PWR);
    am_clk_disable(CLK_PWR);
}

/** \brief 引脚触发信息内存 */
static struct am_zlg237_pwr_mode_init __g_pwr_mode_init[3] = {
     {AM_ZLG237_PWR_MODE_SLEEP,  PIOC_7},
     {AM_ZLG237_PWR_MODE_STOP,   PIOC_7},
     {AM_ZLG237_PWR_MODE_STANBY, PIOA_0},
};

/** \brief PVD触发信息内存 */
static struct am_zlg237_pwr_pvd_info __g_pwr_pvd_info = {
     AMHW_ZLG237_PVD_LEVER_2_9V,    /**< \brief PVD监控电压 */
     2,                             /**< \brief 监控电压模式 */
     AM_TRUE,                       /**< \brief 使能监控电压 */
};

/** \brief PWR设备信息 */
const am_zlg237_pwr_devinfo_t __g_pwr_devinfo = {
    ZLG237_PWR_BASE,            /**< \brief PWR控制器寄存器块基址 */
    ZLG237_BKP_BASE,            /**< \brief BKP配置寄存器块基址 */
    CLK_AHB,                    /**< \brief AHB时钟ID */
    CLK_APB1,                   /**< \brief APB1时钟ID */
    CLK_APB2,                   /**< \brief APB2时钟ID */
    INUM_PVD,                   /**< \brief pvd中断ID */
    __g_pwr_mode_init,          /**< \brief 指向pwr功耗模式数组的指针 */
    &__g_pwr_pvd_info,          /**< \brief 指向pvd电压监控信息的指针 */
    __zlg237_plfm_pwr_init,
    __zlg237_plfm_pwr_deinit
};

/** \brief PWR设备实例 */
am_zlg237_pwr_dev_t __g_pwr_dev;

/** \brief PWR 实例初始化 */
am_zlg237_pwr_handle_t am_zlg237_pwr_inst_init (void)
{
    return am_zlg237_pwr_init(&__g_pwr_dev, &__g_pwr_devinfo);
}

/** \brief PWR 实例解初始化 */
void am_zlg237_pwr_inst_deinit (void)
{
    am_zlg237_pwr_deinit();
}

/**
 * @}
 */

/* end of file */
