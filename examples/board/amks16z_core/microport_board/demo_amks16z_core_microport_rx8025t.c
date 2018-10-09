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
 * \brief MicroPort RX8025T 例程，通过 RX8025T 芯片实现
 *        RTC、闹钟的功能，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort RX8025T 配板连接到 AMKS16RFID 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 调试串口将每一秒的时间信息打印出来。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_1 引脚连接 PC 串口的 TXD，
 *    PIOA_2 引脚连接 PC 串口的 RXD。并且需要查看调试串口是否使用了485，
 *    如果使用了485，可能485的数据方向控制引脚会和 MicroPort RX8025T的中
 *    断引脚产生冲突。
 *
 * \par 源代码
 * \snippet demo_microport_rx8025t.c src_microport_rx8025t
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-14  vir, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_microport_rx8025t
 * \copydoc demo_microport_rx8025t.c
 */

/** [src_microport_rx8025t] */
#include "am_vdebug.h"
#include "am_hwconf_microport.h"
#include "demo_std_entries.h"

/**
 * \brief MicroPort RX8025T 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_amks16z_core_microport_rx8025t_entry (void)
{
    AM_DBG_INFO("demo am116_core microport rx8025t!\r\n");

    demo_std_rtc_entry(am_microport_rx8025t_rtc_inst_init());
}

/** [src_microport_rx8025t] */

/* end of file */
