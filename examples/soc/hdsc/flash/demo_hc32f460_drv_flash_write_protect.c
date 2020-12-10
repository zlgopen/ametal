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
 *   flash写保护验证。
 *   1. 擦除出错：串口打印 "erase error!"；
 *   2. 写入出错：串口打印 "program error!"；
 *   3. 打印从写入地址读取的数据；
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_flash_write_protect.c src_hc32f460_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 20-12-10  xjy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_flash
 * \copydoc demo_hc32f460_drv_flash.c
 */

#include "ametal.h"
#include "am_delay.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_hc32f460_flash.h"
#include "am_hc32f460_flash.h"
#include "am_hc32f460.h"

#define __FLASH_SECTOR  60    /**< \brief 扇区 */
#define __SECTOR_SIZE (8 * 1024)
#define __DATA_SIZE ((__SECTOR_SIZE / 4))

#define __AM_HC32F460_FLASH_START_ADDR   0x0
#define __AM_HC32F460_FLASH_END_ADDR     0x7FFDF

/**
 * \brief flash全片写保护
 */
void sector_erase_lock_all(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);
    amhw_hc32f460_flash_efm_fpmtsw_set(p_hw_flash, 0x70000);
    amhw_hc32f460_flash_efm_fpmtew_set(p_hw_flash, 0x6FFFF);
    amhw_hc32f460_flash_lock(p_hw_flash);
}

/**
 * \brief flash规定地址写保护
 */
int32_t sector_erase_lock(amhw_hc32f460_flash_t *p_hw_flash, 
                                        uint32_t start_addr, 
                                        uint32_t end_addr)
{
    if(start_addr > end_addr) {
        return AM_ERROR;
    }
    amhw_hc32f460_flash_unlock(p_hw_flash);
    amhw_hc32f460_flash_efm_fpmtsw_set(p_hw_flash, start_addr);
    amhw_hc32f460_flash_efm_fpmtew_set(p_hw_flash, end_addr);
    amhw_hc32f460_flash_lock(p_hw_flash);
    return AM_OK;
}

/**
 * \brief flash全片解锁写保护
 */
void sector_erase_unlock(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);
    amhw_hc32f460_flash_efm_fpmtsw_set(p_hw_flash, 0x6FFFF);
    amhw_hc32f460_flash_efm_fpmtew_set(p_hw_flash, 0x6FFFF);
    amhw_hc32f460_flash_lock(p_hw_flash);
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_drv_flash_write_protect_entry(void *p_hw_flash, uint16_t sector)
{
    int             i;
    uint32_t        status;    /* FLASH 命令执行状态 */
    /* 要写入 FLASH 的数据 */
    static uint32_t data1[__DATA_SIZE];

    /* 从 FLASH 中读出的数据 */
    static uint32_t temp1[__DATA_SIZE];
    amhw_hc32f460_flash_t *p_flash = (amhw_hc32f460_flash_t *)p_hw_flash;
    
    /* 数据初始化 */
    for (i = 0; i < __DATA_SIZE; i++) {
        data1[i] = i;
    }

    /* 按字编程 */
    am_hc32f460_flash_init(p_flash);
    
    sector_erase_unlock(p_flash);
    /* 擦除页 */
    status = am_hc32f460_flash_sector_erase(p_flash, sector * __SECTOR_SIZE);
    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = am_hc32f460_flash_sector_program(p_flash,
                                   sector * __SECTOR_SIZE,
                                   data1,
                                   __DATA_SIZE);
    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("word program error!\r\n");
        AM_FOREVER;
    }
    
    /* ##########修改此处可验证########## */
    sector_erase_lock_all(p_flash);
    /* 擦除页 */
    am_hc32f460_flash_sector_erase(p_flash, sector * __SECTOR_SIZE);
    sector_erase_unlock(p_flash);
    
    
    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp1[i] = *(uint32_t *)((i * 4) + (sector * __SECTOR_SIZE));
    }

    for (i = 0; i < __DATA_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp1[i]);
    }
    AM_DBG_INFO("\r\n");
}


