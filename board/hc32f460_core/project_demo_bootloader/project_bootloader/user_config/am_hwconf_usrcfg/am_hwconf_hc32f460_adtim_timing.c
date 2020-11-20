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
  \file
  \brief HC32F460 TIM 定时功能用户配置文件
  \sa am_hwconf_hc32f460_tim_timing.c
 *
 *
  \internal
  \par Modification history
  - 1.00 
  \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_hc32f460.h"
#include "am_gpio.h"
#include "am_hc32f460_clk.h"
#include "am_hc32f460_adtim_timing.h"

/**
  \addtogroup am_if_src_hwconf_hc32f460_tim_timing
  \copydoc am_hwconf_hc32f460_tim_timing.c
  @{
 */
/*******************************************************************************
  ADTIM61 配置
*******************************************************************************/

/** \brief ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim61_timing_init (void)
{
    am_clk_enable(CLK_TIMER6_1);
}

/** \brief 解除 ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim61_timing_deinit (void)
{
    am_clk_disable(CLK_TIMER6_1);
}

/** \brief ADTIM6 设备信息 */
const am_hc32f460_adtim_timing_devinfo_t  __g_adtim61_timing_devinfo = {
    HC32F460_TMR61_BASE,                    /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_1,                          /**< \brief ADTIM6中断编号 */
    __hc32f460_plfm_adtim61_timing_init,    /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim61_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM6 设备定义 */
am_hc32f460_adtim_timing_dev_t __g_adtim61_timing_dev;

/** \brief 定时器ADTIM6 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32f460_tim61_timing_inst_init (void)
{
    return am_hc32f460_adtim_timing_init(&__g_adtim61_timing_dev,
                                         &__g_adtim61_timing_devinfo);
}

/** \brief ADTIM6 Timing实例解初始化 */
void am_hc32f460_tim61_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32f460_adtim_timing_deinit(handle);
}

/*******************************************************************************
  ADTIM62 配置
*******************************************************************************/
/** \brief ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim62_timing_init (void)
{
    am_clk_enable(CLK_TIMER6_2);
}

/** \brief 解除 ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim62_timing_deinit (void)
{
    am_clk_disable(CLK_TIMER6_2);
}

/** \brief ADTIM6 设备信息 */
const am_hc32f460_adtim_timing_devinfo_t  __g_adtim62_timing_devinfo = {
    HC32F460_TMR62_BASE,                    /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_2,                          /**< \brief ADTIM6中断编号 */
    __hc32f460_plfm_adtim62_timing_init,    /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim62_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM6 设备定义 */
am_hc32f460_adtim_timing_dev_t __g_adtim62_timing_dev;

/** \brief 定时器ADTIM6 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32f460_tim62_timing_inst_init (void)
{
    return am_hc32f460_adtim_timing_init(&__g_adtim62_timing_dev,
                                         &__g_adtim62_timing_devinfo);
}

/** \brief ADTIM6 Timing实例解初始化 */
void am_hc32f460_tim62_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32f460_adtim_timing_deinit(handle);
}

/*******************************************************************************
  ADTIM63 配置
*******************************************************************************/
/** \brief ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim63_timing_init (void)
{
    am_clk_enable(CLK_TIMER6_3);
}

/** \brief 解除 ADTIM6 平台初始化 */
void __hc32f460_plfm_adtim63_timing_deinit (void)
{
    am_clk_disable(CLK_TIMER6_3);
}

/** \brief ADTIM6 设备信息 */
const am_hc32f460_adtim_timing_devinfo_t  __g_adtim63_timing_devinfo = {
    HC32F460_TMR63_BASE,                    /**< \brief ADTIM6寄存器块的基地址 */
    INUM_TIMER6_3,                          /**< \brief ADTIM6中断编号 */
    __hc32f460_plfm_adtim63_timing_init,    /**< \brief 平台初始化函数 */
    __hc32f460_plfm_adtim63_timing_deinit   /**< \brief 平台解析初始化函数 */
};

/** \brief ADTIM6 设备定义 */
am_hc32f460_adtim_timing_dev_t __g_adtim63_timing_dev;

/** \brief 定时器ADTIM6 Timing实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_hc32f460_tim63_timing_inst_init (void)
{
    return am_hc32f460_adtim_timing_init(&__g_adtim63_timing_dev,
                                         &__g_adtim63_timing_devinfo);
}

/** \brief ADTIM6 Timing实例解初始化 */
void am_hc32f460_tim63_timing_inst_deinit (am_timer_handle_t handle)
{
    am_hc32f460_adtim_timing_deinit(handle);
}

/**
  @}
 */

/* end of file */
