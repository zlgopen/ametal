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
 * \brief 多通道ADC采集 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. am_hwconf_zsn700_adc.c文件__g_adc_ioinfo_list[]数组的第一个有效元素引脚。默认是PA0
 *   2. am_hwconf_zsn700_adc.c文件__g_adc_ioinfo_list[]数组的第一个有效元素引脚。默认是PA1
 *   3. am_hwconf_zsn700_adc.c文件__g_adc_ioinfo_list[]数组的第一个有效元素引脚。默认是PA2
 *   4. am_hwconf_zsn700_adc.c文件__g_adc_ioinfo_list[]数组的第一个有效元素引脚。默认是PA3
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 需要保证am_hwconf_zsn700_adc.c文件中的对应通道引脚的初始化注释打开；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsn700_core_std_multichan_adc.c src_zsn700_core_std_multichan_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-24  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_core_std_multichan_adc
 * \copydoc demo_zsn700_core_std_multichan_adc.c
 */

/** [src_std_adc] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsn700_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am700_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsn700_core_std_multichan_adc_entry (void)
{
    /* ADC通道0,1,2,3 */
    int adc_chan[] = {0, 1, 2, 3};

    AM_DBG_INFO("demo am700_core std adc int!\r\n");

    demo_std_multichan_adc_entry(am_zsn700_adc_inst_init(),
                                 adc_chan,
                                 sizeof(adc_chan) / sizeof(adc_chan[0]));

}
/** [src_std_adc] */

/* end of file */
