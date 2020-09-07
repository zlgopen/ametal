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
 *   对flash进行字编程。
 *   1. 擦除出错：串口打印 "erase error!"；
 *   2. 写入出错：串口打印 "program error!"；
 *   3. 写入成功：串口打印指定页的十六进制数据。
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_flash.c src_hc32f460_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_flash
 * \copydoc demo_hc32f460_drv_flash.c
 */

/** [src_hc32f460_drv_flash] */
#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_hc32f460_flash.h"

/**< \brief 扇区大小 以字节(8bit)为单位 */
#define SECTOR_SIZE (8 * 1024)
#define __DATA_SIZE ((SECTOR_SIZE / 4))

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_flash_entry (void *p_hw_flash, uint16_t sector)
{
    amhw_hc32f460_flash_t *p_flash = (amhw_hc32f460_flash_t *)p_hw_flash;

    int             i;
    uint32_t        status;    /* FLASH 命令执行状态 */

    /* 要写入 FLASH 的数据 */
    static uint32_t data1[__DATA_SIZE];

    /* 从 FLASH 中读出的数据 */
    static uint32_t temp1[__DATA_SIZE];

    /* 数据初始化 */
    for (i = 0; i < __DATA_SIZE; i++) {
        data1[i] = i;
    }

/*******************************************************************************
按字编程
*******************************************************************************/

    am_hc32f460_flash_init(p_flash);

    /* 擦除页 */
    status = am_hc32f460_flash_sector_erase(p_flash, sector * SECTOR_SIZE);

    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = am_hc32f460_flash_sector_program(p_flash,
                                              sector * SECTOR_SIZE,
                                              data1,
                                              __DATA_SIZE);

    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("word program error!\r\n");
        AM_FOREVER;
    }

    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp1[i] = *(uint32_t *)((i * 4) + (sector * SECTOR_SIZE));

        /* 校验数据，校验失败，程序停在此处 */
        if (temp1[i] != data1[i]) {
            AM_DBG_INFO("word verify error!\r\n");
            AM_FOREVER;
        }
    }

    for (i = 0; i < __DATA_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp1[i]);
    }
    AM_DBG_INFO("\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f460_drv_flash] */

/* end of file */
