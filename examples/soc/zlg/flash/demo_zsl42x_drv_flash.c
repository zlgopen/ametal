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
 *   对flash进行字、半字、字节编程。
 *   1. 擦除出错：串口打印 "erase error!"；
 *   2. 写入出错：串口打印 "program error!"；
 *   3. 写入成功：串口打印指定页的十六进制数据。
 *
 * \par 源代码
 * \snippet demo_zsl42x_drv_flash.c src_zsl42x_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_drv_flash
 * \copydoc demo_zsl42x_drv_flash.c
 */

/** [src_zsl42x_drv_flash] */
#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_zsl42x_flash.h"

#define __DATA_SIZE ((SECTOR_SIZE / 2) - 1)

/**
 * \brief 例程入口
 */
void demo_zsl42x_drv_flash_entry (void *p_hw_flash, uint16_t sector)
{
    amhw_zsl42x_flash_t *p_flash = (amhw_zsl42x_flash_t *)p_hw_flash;

    int             i;
    uint32_t        status;    /* FLASH 命令执行状态 */

    /* 要写入 FLASH 的数据 */
    static uint32_t data1[__DATA_SIZE];
    static uint16_t data2[__DATA_SIZE];
    static uint8_t  data3[__DATA_SIZE];

    /* 从 FLASH 中读出的数据 */
    static uint32_t temp1[__DATA_SIZE];
    static uint16_t temp2[__DATA_SIZE];
    static uint8_t  temp3[__DATA_SIZE];

    /* 数据初始化 */
    for (i = 0; i < __DATA_SIZE; i++) {
        data1[i] = i;
        data2[i] = i;
        data3[i] = i;
    }

/*******************************************************************************
按字编程
*******************************************************************************/

    /* 擦除页 */
    status = am_zsl42x_flash_sector_erase(p_flash, sector << 9);
    status = am_zsl42x_flash_sector_erase(p_flash, (sector + 1) << 9);    /* 数据量超过了一页 */

    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = am_zsl42x_flash_sector_program(p_flash,
                                          sector << 9,
                                          data1,
                                          __DATA_SIZE);

    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("word program error!\r\n");
        AM_FOREVER;
    }

    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp1[i] = *(uint32_t *)((i * 4) + (sector << 9));

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

/*******************************************************************************
按半字编程
*******************************************************************************/

    /* 擦除页 */
    status = am_zsl42x_flash_sector_erase(p_flash, sector << 9);

    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = am_zsl42x_flash_sector_halfword_program(p_flash,
                                                   sector << 9,
                                                   data2,
                                                   __DATA_SIZE);

    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("half word program error!\r\n");
        AM_FOREVER;
    }

    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp2[i] = *(uint16_t *)((i * 2) + (sector << 9));

        /* 校验数据，校验失败，程序停在此处 */
        if (temp2[i] != data2[i]) {
            AM_DBG_INFO("half word verify error!\r\n");
            AM_FOREVER;
        }
    }

    for (i = 0; i < __DATA_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp2[i]);
    }
    AM_DBG_INFO("\r\n");

/*******************************************************************************
按字节编程
*******************************************************************************/

    /* 擦除页 */
    status = am_zsl42x_flash_sector_erase(p_flash, sector << 9);

    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = am_zsl42x_flash_sector_byte_program(p_flash,
                                              sector << 9,
                                              data3,
                                              __DATA_SIZE);

    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("byte program error!\r\n");
        AM_FOREVER;
    }

    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp3[i] = *(uint8_t *)(i + (sector << 9));

        /* 校验数据，校验失败，程序停在此处 */
        if (temp3[i] != data3[i]) {
            AM_DBG_INFO("byte verify error!\r\n");
            AM_FOREVER;
        }
    }

    for (i = 0; i < __DATA_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp3[i]);
    }
    AM_DBG_INFO("\r\n");

    AM_DBG_INFO("\r\nflash test success!\r\n");

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zsl42x_drv_flash] */

/* end of file */
