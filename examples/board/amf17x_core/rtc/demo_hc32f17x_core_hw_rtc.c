/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2020 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/
/**
 * \file
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 每1秒发生一次周期中断，串口并打印出当前时间信息。
 *   2. 在指定时间产生闹钟中断，打印一行"alarm clock int!"提示信息。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f17x_hw_rtc.c src_hc32f17x_hw_rtc
 *
 * \internal
 * \par History
 * - 1.00 20-05-14  licl, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f17x_hw_rtc
 * \copydoc demo_hc32f17x_hw_rtc.c
 */

/** [src_hc32f17x_hw_rtc] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_vdebug.h"
#include "demo_hc32_entries.h"
#include "am_hc32f17x_inst_init.h"
#include "demo_amf17x_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_hc32f17x_core_hw_rtc_entry (void)
{
    AM_DBG_INFO("demo amf17x_core hw rtc!\r\n");

    demo_hc32_hw_rtc_entry((void *)HC32_RTC);
}
/** [src_hc32f17x_hw_rtc] */

/* end of file */
