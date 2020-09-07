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
 * \brief zml166_adc 用户配置文件
 * \sa am_hwconf_zml166_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.01 18-09-07  cml, correction frequency of communication with zml166_adc
 * - 1.00 18-06-11  pea, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_gpio.h"
#include "am_zml166_adc.h"
#include "am_zlg116.h"

/**
 * \addtogroup am_if_src_hwconf_zml166_adc
 * \copydoc am_hwconf_zml166_adc.c
 * @{
 */

/** \brief zml166_adc 设备信息 */
am_local am_const am_zml166_adc_devinfo_t __g_zml166_adc_devinfo = {
    2500,                           /* ADC 参考电压，单位：mV，±2.5V，即 5V */
    5000,                           /* 超时时间，单位为毫秒 */
};

/** \brief zml166_adc 设备实例 */
am_local am_zml166_adc_dev_t __g_zml166_adc_dev;
/**
 * \brief zml166_adc 实例初始化
 */
am_zml166_adc_handle_t am_zml166_adc_inst_init (void)
{
    return am_zml166_adc_init(&__g_zml166_adc_dev, &__g_zml166_adc_devinfo);
}

/**
 * \brief zml166_adc 实例解初始化
 */
void am_zml166_adc_inst_deinit (am_zml166_adc_handle_t handle)
{
    am_zml166_adc_deinit(handle);
}

/**
 * @}
 */

/* end of file */
