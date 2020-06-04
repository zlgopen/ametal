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
 * \brief MicroPort FLASH 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 MicroPort FLASH 配板连接到 aml165-Core 的 MicroPort 接口。
 *
 * - 实验现象：
 *   1. 主机写数据到 FLASH；
 *   2. 主机从 FLASH 读取数据，并通过串口打印处理；
 *   3. 串口打印出测试结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOB_3 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_aml165_core_microport_flash.c src_aml165_core_microport_flash
 *
 * \internal
 * \par Modification history
 * - 1.00  17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_microport_flash
 * \copydoc demo_aml165_core_microport_flash.c
 */

/** [src_aml165_core_microport_flash] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_components_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_microport_flash_entry (void)
{
    AM_DBG_INFO("demo aml165_core microport flash!\r\n");

    demo_mx25xx_entry(am_microport_flash_inst_init(), 8);
}
/** [src_aml165_core_microport_flash] */

/* end of file */
