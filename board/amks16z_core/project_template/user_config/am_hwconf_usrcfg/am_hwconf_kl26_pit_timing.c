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
 * \brief KL26 PIT 定时功能用户配置文件
 * \sa am_kl26_hwconfig_pit_timing.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-18  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_kl26.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fsl_pit_timing.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_pit_timing
 * \copydoc am_kl26_hwconfig_pit_timing.c
 * @{
 */

/** \brief PIT 平台初始化 */
void __kl26_plfm_pit_timing_init (void)
{
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PIT);
}

/** \brief 解除 PIT 平台初始化 */
void __kl26_plfm_pit_timing_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_PIT);
}

/** \brief PIT 设备信息 */
const am_fsl_pit_timing_devinfo_t  __g_pit_timing_devinfo = {
    KL26_PIT ,                 /**< \brief 指向PIT寄存器块的指针  */
    AM_FSL_PIT_TIMING_2_32BIT,      /**< \brief 配置为2通道的32位定时器 */
    INUM_PIT,                       /**< \brief PIT中断编号 */
    CLK_PIT,                        /**< \brief PIT时钟号 */
    __kl26_plfm_pit_timing_init,    /**< \brief 平台初始化函数 */
    __kl26_plfm_pit_timing_deinit   /**< \brief 平台解初始化函数 */
};

/** \brief PIT 设备定义  */
am_fsl_pit_timing_dev_t  __g_pit_timing_dev;

/** \brief pit Timer 实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_kl26_pit_timing_inst_init (void)
{
    return am_fsl_pit_timing_init(&__g_pit_timing_dev,
                                  &__g_pit_timing_devinfo);
}

/** \brief pit Timer 实例解初始化 */
void am_kl26_pit_timing_inst_deinit (am_timer_handle_t handle)
{
    am_fsl_pit_timing_deinit(handle);
}

/**
 * @}
 */

/* end of file */
