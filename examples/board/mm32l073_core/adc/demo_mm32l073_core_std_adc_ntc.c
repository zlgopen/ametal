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
 * \brief ADC 利用板子的热敏电阻(NTC)，推算出温度例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 短接 J6 跳线帽，使得 ADC_IN8（PIOB_0）测量 R9 的电压；
 *   2. 将 J14 的 KEY 和 RES 短接在一起，使得按下 RES 键接通功率电阻。
 *
 * - 实验现象：
 *   1. 串口输出温度值；
 *   2. 按下 RES 按键，过一会儿，串口输出温度值升高。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD；
 *
 * \warning 请尽量使用独立 5V 电源给板子供电，因为加热电阻是功率电阻，阻值较小电流大。
 *          普通供电方式如 USB 供电，容易引起电源电压波动，影响热敏电阻阻值判断，进而
 *          影响温度值的测量。
 *
 * \par 源代码
 * \snippet demo_mm32l073_core_std_adc_ntc.c src_mm32l073_core_std_adc_ntc
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-10  hgo, modified
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_core_std_adc_ntc
 * \copydoc demo_mm32l073_core_std_adc_ntc.c
 */

/** [src_mm32l073_core_std_adc_ntc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l073_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l073_core_entries.h"

/* \brief 例程入口  */
void demo_mm32l073_core_std_adc_ntc_entry (void)
{
    AM_DBG_INFO("demo mm32l073_core std adc ntc!\r\n");

    /* NTC温度采集使用的是ADC通道 8 （PIOB_0）， 分压电阻为 2000欧姆 */
    demo_std_adc_ntc_entry(am_mm32l073_adc_inst_init(),  8, 2000);
}
/** [src_mm32l073_core_std_adc_ntc] */

/* end of file */
