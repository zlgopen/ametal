/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief FLASH 例程，通过驱动层接口实现
 *
 * - 实验现象：
 *   1. 擦除出错：串口打印 "erase error!"；
 *   2. 写入出错：串口打印 "program error!"；
 *   3. 写入成功：串口打印指定扇区的(SECTOR_SIZE)个 32bit 十六进制数据。
 *
 * \par 源代码
 * \snippet demo_zsn700_drv_flash.c src_zsn700_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-08  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_drv_flash
 * \copydoc demo_zsn700_drv_flash.c
 */

/** [src_zsn700_drv_flash] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_zsn700_flash.h"

/**
 * \brief 例程入口
 */
void demo_zsn700_drv_flash_entry (void *p_hw_flash, uint16_t sector)
{
    amhw_zsn700_flash_t *p_flash = (amhw_zsn700_flash_t *)p_hw_flash;

    int             i;
    uint32_t        status;         /* FLASH 命令执行状态 */
    static uint32_t data[SECTOR_SIZE]; /* 要写入 FLASH 的数据 */
    static uint32_t temp[SECTOR_SIZE]; /* 从 FLASH 中读出的数据 */

    /* 数据初始化 */
    for (i = 0; i < SECTOR_SIZE; i++) {
        data[i] = i;
    }

    /* 擦除扇区 */
    status = am_zsn700_flash_sector_erase(p_flash, sector << 9);

    /* 扇区擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");

        AM_FOREVER;
    }

    /* 向扇区中写入数据 */
    status = am_zsn700_flash_sector_program(p_flash,
                                            sector << 9,
                                            data,
                                            SECTOR_SIZE);

    /* 扇区写入出错，程序停在此处 */
    if ((SECTOR_SIZE) != status) {
        AM_DBG_INFO("program error!\r\n");

        AM_FOREVER;
    }

    /* 从扇区读取数据 */
    for (i = 0; i < SECTOR_SIZE; i++) {
        temp[i] = *(uint32_t *)((i * 4) + (sector << 9));

        /* 校验数据，校验失败，程序停在此处 */
        if (temp[i] != data[i]) {
            AM_DBG_INFO("verify error!\r\n");
            AM_FOREVER;
        }
    }

    for (i = 0; i < SECTOR_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp[i]);
    }
    AM_DBG_INFO("\r\nflash test success!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zsn700_drv_flash] */

/* end of file */
