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
 * \brief ADC 利用板载热敏电阻 (NTC) 进行温度测量例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接 J6 跳线帽，使得 ADC0_7（PIO0_19）测量 R14 的电压；(由于还没有外接的热敏电阻改demo占时无法使用)
 *
 * - 实验现象：
 *   1. 串口输出温度值；
 *   2. 按下 RES 按键，过一会儿，串口输出温度值升高。
 *
 * \note
 *    1. 按键需要将 J14 跳线帽的 KEY 和 RES 短接才能使用；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_14 引脚连接 PC 串口的 TXD，
 *       PIO0_23 引脚连接 PC 串口的 RXD。
 *
 * \warning 请尽量使用独立 5V 电源给板子供电，因为加热电阻是功率电阻，阻值较小
 *          电流大。普通供电方式如 USB 供电，容易引起电源电压波动，影响热敏电
 *          阻阻值判断，进而影响温度值的测量。
 *
 * \par 源代码
 * \snippet demo_am845_core_std_adc_ntc.c src_am845_core_std_adc_ntc
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-10  hgo, modified
 * \endinternal
 */

/**
 * \addtogroup demo_if_am845_core_std_adc_ntc
 * \copydoc demo_aam845_core_std_adc_ntc.c
 */

/** [src_am845_core_std_adc_ntc] */
#include "ametal.h"
//#include "am_adc.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc84x_inst_init.h"
#include "demo_std_entries.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** \brief NTC 温度采集使用的是 ADC 通道 7 */
#define __ADC_CHAN    7

/**
 * \brief 例程入口
 */
void demo_am845_core_std_adc_ntc_entry (void)
{
    AM_DBG_INFO("demo am845_core std adc ntc!\r\n");
  
    demo_std_adc_ntc_entry(am_lpc84x_adc0_int_inst_init(),  __ADC_CHAN, 2000);

}
/** [src_am845_core_std_adc_ntc] */

/* end of file */
