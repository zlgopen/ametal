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
 * \brief MicroPort-RTC(PCF85063) 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort-RTC 扩展板连接到 AM824-Core 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 串口将每一秒的时间信息打印出来；
 *   2. 闹钟时间一到就会执行闹钟回调函数；
 *   3. 中断产生时会调用中断回调函数。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *    PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_microport_rtc.c src_microport_rtc
 *
 * \internal
 * \par Modification History
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_microport_rtc
 * \copydoc demo_microport_rtc.c
 */

/** [src_microport_rtc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_lpc82x_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_microport_rtc_entry (void)
{

    AM_DBG_INFO("demo am824_core microport rtc!\r\n");

    demo_std_rtc_entry(am_microport_rtc_rtc_inst_init());

}
/** [src_microport_rtc] */

/* end of file */
