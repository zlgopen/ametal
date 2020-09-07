/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
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
 *   1. 每1秒发生一次中断，串口并打印出当前时间信息。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f03x_core_std_rtc.c src_hc32f03x_core_std_rtc
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-17  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f03x_core_std_rtc
 * \copydoc demo_hc32f03x_core_std_rtc.c
 */

/** [src_hc32f03x_core_std_rtc] */
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_hc32f03x_inst_init.h"
#include "demo_amf03x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f03x_core_std_rtc_entry (void)
{
    AM_DBG_INFO("demo amf03x_core std rtc!\r\n");

    demo_std_rtc_entry(am_hc32_rtc_inst_init());
}
/** [src_hc32f03x_core_std_rtc] */

/* end of file */
