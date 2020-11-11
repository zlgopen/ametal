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
 * \brief MicroPort DS1302 例程，通过 RTC 标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort DS1302 配板连接到 AM118 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 串口将每一秒的时间信息打印出来。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_microport_ds1302.c src_microport_ds1302
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-23  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_microport_ds1302
 * \copydoc demo_microport_ds1302.c
 */

/** [src_microport_ds1302] */
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "am_zsl42x_inst_init.h"
#include "demo_am42x_core_entries.h"

/**
 * \brief MicroPort DS1302 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_zsl42x_core_microport_ds1302_entry (void)
{
    AM_DBG_INFO("demo am42x_core microport ds1302!\r\n");

    demo_std_rtc_entry(am_microport_ds1302_rtc_inst_init());

}
/** [src_microport_ds1302] */

/* end of file */
