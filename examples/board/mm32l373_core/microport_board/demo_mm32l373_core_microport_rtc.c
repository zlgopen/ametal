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
 * \brief MicroPort RTC(PCF85063) 例程，通过 PCF85063 芯片实现
 *        RTC、闹钟的功能，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort RTC 配板连接到 AM217BLE 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 串口将每一秒的时间信息打印出来；
 *   2. 闹钟时间一到就会执行闹钟回调函数；
 *   3. 中断产生时会调用中断回调函数。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
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
#include "am_pcf85063.h"
#include "am_vdebug.h"
#include "am_mm32l373_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief MicroPort RTC 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_mm32l373_core_microport_rtc_entry (void)
{
    AM_DBG_INFO("demo mm32l373_core microport rtc!\r\n");

    demo_std_rtc_entry(am_microport_std_rtc_inst_init());
}
/** [src_microport_rtc] */

/* end of file */
