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
 *   1. PIOA_0 (ADC 通道 0) 连接模拟输入。
 *   2. PIOA_1 (ADC 通道 1) 连接模拟输入。
 *   3. PIOA_2 (ADC 通道 2) 连接模拟输入。
 *   4. PIOA_3 (ADC 通道 3) 连接模拟输入。
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC 模块功能，必须保证 ADC 模块上电；
 *    2. 需要保证am_hwconf_zlg237_adc.c文件中的对应通道引脚的初始化注释打开；
 *    3. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *    4. 若用户用户需要添加ADC轮询读取通道的数目，或者变更读取通道的顺序，只需修改
 *       adc_chan[]数组中的通道值即可，注意需要在am_hwconf_zlg237_adc.c
 *       文件中将通道引脚的初始化注释打开。
 *
 * \par 源代码
 * \snippet demo_zlg237_core_std_multichan_adc.c src_zlg237_core_std_multichan_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-02-29  ipk, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg237_core_std_multichan_adc
 * \copydoc demo_zlg237_core_std_multichan_adc.c
 */

/** [src_std_adc] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zlg237_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am237_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zlg237_core_std_multichan_adc_entry (void)
{
    /* ADC通道0,1,2,3 */
    int adc_chan[] = {0, 1, 2, 3};

    AM_DBG_INFO("demo am237_core std adc int!\r\n");

    demo_std_multichan_adc_entry(am_zlg237_adc1_inst_init(),
                                 adc_chan,
                                 sizeof(adc_chan) / sizeof(adc_chan[0]));

}
/** [src_std_adc] */

/* end of file */
