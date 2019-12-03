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
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 擦除出错：串口打印"erase error!"；
 *   2. 写入出错：串口打印"program error!"；
 *   3. 写入成功：串口打印 55 扇区的(1024/4)个 32bit 十六进制数据。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *    PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_mm32l373_drv_flash.c src_mm32l373_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  ari, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l373_drv_flash
 * \copydoc demo_mm32l373_drv_flash.c
 */

/** [src_mm32l373_drv_flash] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_mm32_flash.h"
#include "am_mm32l373.h"
#include "demo_mm32_entries.h"
#include "demo_mm32l373_core_entries.h"

#define FLASH_SECTOR 55    /**< \brief 扇区 */

/**
 * \brief 例程入口
 */
void demo_mm32l373_core_drv_flash_entry (void)
{
    AM_DBG_INFO("demo mm32l373_core drv flash!\r\n");

    demo_mm32_drv_flash_entry(MM32L373_FLASH, FLASH_SECTOR);
}

/** [src_mm32l373_drv_flash] */

/* end of file */
