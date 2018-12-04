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
 * \brief ADC例程，软件触发转换，DMA传输转换结果，通过标准接口实现
 *
 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. J12跳线帽短接（此时参考电压为2.5v）；
 *   4. PIOE_20(ADC通道0) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \par 源代码
 * \snippet demo_amks16z_core_std_adc_st.c src_amks16z_core_std_adc_st
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-18  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_std_adc_st
 * \copydoc demo_amks16z_core_std_adc_st.c
 */

/** [src_amks16z_core_std_adc_st] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_adc.h"
#include "am_kl26_inst_init.h"
#include "am_board.h"
#include "demo_std_entries.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief adc软件触发，DMA传输结果
 * \return 无
 */
void demo_amks16z_core_std_adc_st_entry (void)
{
    am_adc_handle_t adc0_handle;

    am_kl26_dma_inst_init();                 /* DMA实例初始化 */
    adc0_handle = am_kl26_adc0_inst_init();  /* ADC实例初始化，并获取ADC句柄值 */

    demo_std_adc_entry(adc0_handle, 0);
}

/** [src_amks16z_core_std_adc_st] */

/* end of file */
