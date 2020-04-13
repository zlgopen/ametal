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
 * \brief RTC 例程，通过标准接口和中间驱动层函数实现
 *
 * - 实验现象：
 *   1. 每10秒发生一次周期中断，串口并打印出当前时间信息。
 *   2. 周四的11点21分0秒产生闹钟中断，打印四行"alarm clock int!"提示信息。
 *   3. 可在am_hwconf_hc32f460_rtc.c文件中配置1Hz输出控制，若使能，PIOB_14将输出1Hz的方波。
 *   4. 关闭时钟补偿的情况下，测得1Hz方波的实际频率，带入下列计算式计算：
 *           ppm = （1Hz方波的实际频率 - 1Hz）* 10^6
 *      将计算出的结果填入am_hwconf_hc32f460_rtc.c的设备信息对应位置，可自行决定是否使能高速补偿时钟。
 *      编译、下载，输出端口将输出精度较高的1Hz方波
 *
 * \note
 *    
 *
 * \par 源代码
 * \snippet demo_hc32f460_std_rtc.c src_hc32f460_std_rtc
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_std_rtc
 * \copydoc demo_hc32f460_std_rtc.c
 */

/** [src_hc32f460_std_rtc] */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_rtc.h"
#include "am_hc32f460_rtc.h"
#include "demo_std_entries.h"
#include "am_hc32f460_inst_init.h"
#include "demo_hc32f460_core_entries.h"


/**
 * \brief 例程入口
 */
void demo_hc32f460_core_std_rtc_entry (void)
{    
    AM_DBG_INFO("demo hc32f460_core std rtc entry!\r\n");
    demo_std_rtc_entry(am_hc32f460_rtc_inst_init());
}
/** [src_hc32f460_std_rtc] */

/* end of file */
