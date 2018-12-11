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
 * \brief KL26 RTC 用户配置文件
 * \sa am_kl26_hwconfig_rtc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "hw/amhw_kl26_sim.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_fsl_rtc.h"
#include "../../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_rtc
 * \copydoc am_kl26_hwconfig_rtc.c
 * @{
 */

/**
 *  \brief RTC 平台初始化
 *  \return 无
 */
void __kl26_plfm_rtc_init(void)
{
    /** RTC 时钟源选择为外部RTC时钟源 */
    amhw_kl26_sim_ercclk32k_src_set(KL26_SIM_OSC32KSEL_CLKIN);

    /** RTC 模块时钟使能 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_RTC);

    /** PORTC 时钟引脚使能 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_PORTC);

    /** PTC1 复用位 RTC_CLK_INPUT */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_1, 1);
}

/**
 *  \brief 解除 RTC 平台初始化
 *  \return 无
 */
void __kl26_plfm_rtc_deinit(void)
{
    /** RTC 模块时钟使能 */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_RTC);
}

/** \brief RTC设备信息 */
const struct am_fsl_rtc_devinfo __g_rtc_devinfo = {
    KL26_RTC,
    __kl26_plfm_rtc_init,
    __kl26_plfm_rtc_deinit
};

/** \brief RTC设备     */
am_fsl_rtc_dev_t __g_rtc_dev;

/** \brief rtc 实例初始化，获得rtc标准服务句柄 */
am_rtc_handle_t am_kl26_rtc_inst_init (void)
{
    return am_fsl_rtc_init(&__g_rtc_dev, &__g_rtc_devinfo);
}

/**
 * \brief rtc 实例解初始化
 * \param[in] handle : rtc句柄值
 */
void am_kl26_rtc_inst_deinit (am_rtc_handle_t handle)
{
    am_fsl_rtc_deinit(handle);
}

/**
 * @}
 */

/** end of file */

