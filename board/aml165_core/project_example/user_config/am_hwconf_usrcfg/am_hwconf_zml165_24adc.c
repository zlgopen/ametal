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
 * \brief ZML165_ADC 配置文件
 * \sa am_hwconf_zml165_adc.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-11  htf, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "zml165_pin.h"
#include "am_zml165_adc.h"
/**
 * \addtogroup am_if_src_hwconf_zml165_adc
 * \copydoc am_hwconf_zml165_adc.c
 * @{
 */

/* 定义 ZML165_ADC 实例 */
static am_zml165_adc_dev_t __g_zml165_adc_dev;

/* 定义 ZML165_ADC 实例信息 */
const am_zml165_adc_devinfo_t __g_zml165_adc_info = {
    {
        AM_ZML165_ADC_PGA_1,
        AM_ZML165_ADC_SPEED_10HZ,
        AM_ZML165_ADC_CHANNEL_A,
        AM_ZML165_ADC_VOUT_ENABLE
    },
    2493
};

am_zml165_adc_handle_t am_zml165_24adc_inst_init(void)
{
    return am_zml165_adc_init(&__g_zml165_adc_dev, &__g_zml165_adc_info);
}
/**
 * @}
 */

/* end of file */

