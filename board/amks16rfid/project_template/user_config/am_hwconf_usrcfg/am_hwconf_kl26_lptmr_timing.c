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
 * \brief KL26 LPTMR 定时功能用户配置文件
 * \sa am_kl26_hwconfig_lptmr_timing.c
 * 
 * \note LPTM只有一个定时通道，标准接口层参数通道号无意义。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_kl26.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fsl_lptmr_timing.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_lptmr_timing
 * \copydoc am_kl26_hwconfig_lptmr_timing.c
 * @{
 */

/** \brief LPTMR 平台初始化 */
void __kl26_plfm_lptmr_timing_init (void)
{
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_LPTMR);
    
    /* 关闭LPTMR模块 */
    amhw_fsl_lptmr_ctl_clear(KL26_LPTMR, AMHW_FSL_LPTMR_CSR_TEN);

    /* 时钟源选择 */
    amhw_fsl_lptmr_clock_set(KL26_LPTMR, AMHW_FSL_LPTMR_CLOCK_SRC_LPOCLK);
}

/** \brief 解除 LPTMR 平台初始化 */
void __kl26_plfm_lptmr_timing_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_LPTMR);
}

/** \brief LPTMR 设备信息 */
const am_fsl_lptmr_timing_devinfo_t  __g_lptmr_timing_devinfo = {
    KL26_LPTMR,                  /**< \brief 指向LPTMR寄存器块的指针 */
    INUM_LPTMR0,                      /**< \brief LPTMR0中断编号 */
    CLK_LPTMR,                        /**< \brief LPTMR时钟号 */
    __kl26_plfm_lptmr_timing_init,
    __kl26_plfm_lptmr_timing_deinit
};

/** \brief LPTMR 设备定义  */
am_fsl_lptmr_timing_dev_t  __g_lptmr_timing_dev;

/** \brief LPTMR Timer 实例初始化，获得Timer标准服务句柄 */
am_timer_handle_t am_kl26_lptmr_timing_inst_init (void)
{
    return am_fsl_lptmr_timing_init(&__g_lptmr_timing_dev,
                                    &__g_lptmr_timing_devinfo);
}

/** \brief LPTMR Timer 实例解初始化 */
void am_kl26_lptmr_timing_inst_deinit (am_timer_handle_t handle)
{
    am_fsl_lptmr_timing_deinit(handle);
}

/**
 * @}
 */

/* end of file */
