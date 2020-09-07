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
 * \brief
 * - 实验现象：
 *   1. 连接好串口,连接好PT100。
 *   2. 串口将会打印对应的温度值
 *
 * \par 源代码
 * \snippet demo_aml165_core_adc24_vol_measure.c src_aml165_core_adc24_vol_measure
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-15  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_aml165_core_adc24_vol_measure
 * \copydoc demo_aml165_core_adc24_vol_measure.c
 */

/** [src_aml165_core_adc24_vol_measure] */

#include <am_hwconf_zml165_24adc.h>
#include "am_zml165_adc.h"
#include "demo_zlg_entries.h"
#include "demo_aml165_core_entries.h"


/**
 * \brief ZML165  24位ADC 固定电压测量例程
 */
void demo_aml165_core_zml165_adc_vol_measure (void)
{
    am_zml165_adc_handle_t handle = am_zml165_24adc_inst_init();
    demo_zml165_adc_vol_measure_entry((void *)handle);
}

/** [src_aml165_core_adc24_vol_measure] */

/* end of file */
