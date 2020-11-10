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
 * \brief ADC 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. am_hwconf_zsl42x_adc.c文件__g_adc_ioinfo_list[]数组的第一个有效元素引脚。默认是PA0
 *  am_hwconf_zsl42x_adc.c文件修改设备信息中：
 *   2. 参考电压选择： AMHW_ZSL42x_ADC_REFVOL_AVCC；
 *   3. 参考电压(mv)： 3300(AVCC)；
 *   4. 禁能ADC通道28内部温度传感器.(1：使能，0：禁能)
 *
 * - 实验现象：
 *   1. 串口输出电压采样值。
 *
 * \note
 *    1. 使用 ADC 模块功能，必须保证 ADC 模块上电；
 *    2. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_zsl42x_std_adc.c src_std_adc
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_std_adc
 * \copydoc demo_zsl42x_std_adc.c
 */

/** [src_std_adc] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsl42x_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_zsl42x_core_std_adc_entry (void)
{
    AM_DBG_INFO("demo am42x_core std adc int!\r\n");

    demo_std_adc_entry(am_zsl42x_adc_inst_init(), 10);

}
/** [src_std_adc] */

/* end of file */
