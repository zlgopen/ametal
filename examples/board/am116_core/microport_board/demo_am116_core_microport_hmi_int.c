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
 *        HMI的功能，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort RTC 配板连接到 AM116-Core 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 串口将每30秒打印时间信息。
 *   2. LED每30秒翻转一次状态。
 *
 * \note
 *   1.如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *   2.PCF85063的中断INT引脚为 PIOA_15
 *
 * \par 源代码
 * \snippet demo_am116_core_microport_hmi_int.c src_am116_core_microport_hmi_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_microport_hmi_int
 * \copydoc demo_am116_core_microport_hmi_int.c
 */

/** [src_am116_core_microport_hmi_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_std_entries.h"
#include "demo_am116_core_entries.h"

/**
 * \brief MicroPort HMI 例程，通过标准接口实现
 *
 * \param 无
 *
 * \return 无
 */
void demo_am116_core_microport_hmi_int_entry (void)
{
    AM_DBG_INFO("demo am116_core microport hmi int!\r\n");

    demo_std_hmi_int_entry(am_microport_rtc_inst_init());
}
/** [src_am116_core_microport_hmi_int] */

/* end of file */
