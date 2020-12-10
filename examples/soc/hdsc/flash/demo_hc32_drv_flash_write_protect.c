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
 * \snippet demo_hc32_drv_flash_write_protect.c src_hc32f07x_drv_flash
 *
 * \internal
 * \par Modification history
 * - 1.00 20-12-10  xjy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_drv_flash
 * \copydoc demo_hc32_drv_flash.c
 */

#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "hc32_pin.h"
#include "hc32_periph_map.h"
#include "math.h"
#include "am_hc32_flash.h"
#include "am_types.h"
#include "am_bitops.h"
#include "am_int.h"

#define AM_HC32_FLASH_END_ADDR        (FLASH_SIZE - 1)

#define AM_HC32_FLASH_BYPASS(p_hw)    amhw_hc32_flash_key_set(p_hw, 0x5A5A);\
                                      amhw_hc32_flash_key_set(p_hw, 0xA5A5)

#define AM_HC32_FLASH_TIMEOUT_INIT    (0xFFu)
#define AM_HC32_FLASH_TIMEOUT_PGM     (0xFFu)
#define AM_HC32_FLASH_TIMEOUT_ERASE   (0xFFu)
#define AM_HC32_FLASH_LOCK_ALL        (0x00u)
#define AM_HC32_FLASH_UNLOCK_ALL      (0xFFFFFFFFu)
#define __DATA_SIZE ((SECTOR_SIZE / 2) - 1)

/**
 * \brief 设置所有页写保护
 */
am_local void flash_lockall(amhw_hc32_flash_t *p_hw_flash)
{
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock0 = AM_HC32_FLASH_LOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock1 = AM_HC32_FLASH_LOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock2 = AM_HC32_FLASH_LOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock3 = AM_HC32_FLASH_LOCK_ALL;
}

/**
 * \brief 取消所有页写保护
 */
am_local void flash_unlockall(amhw_hc32_flash_t *p_hw_flash)
{
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock0 = AM_HC32_FLASH_UNLOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock1 = AM_HC32_FLASH_UNLOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock2 = AM_HC32_FLASH_UNLOCK_ALL;
    AM_HC32_FLASH_BYPASS(p_hw_flash);
    p_hw_flash->slock3 = AM_HC32_FLASH_UNLOCK_ALL;
}


/**
 * \brief 向flash写数据
 */
am_local int32_t flash_write(amhw_hc32_flash_t  *p_hw_flash,
                                    uint32_t            addr,
                                    const void         *data,
                                    uint8_t             data_size)
{
    volatile uint32_t timeout = AM_HC32_FLASH_TIMEOUT_PGM;

    /* busy? */
    timeout = AM_HC32_FLASH_TIMEOUT_PGM;
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    /* set op */
    timeout = AM_HC32_FLASH_TIMEOUT_PGM;
    while(AMHW_HC32_FLASH_WRITE != amhw_hc32_flash_opt_get(p_hw_flash)) {
        if(timeout--) {
            amhw_hc32_flash_opt_set(p_hw_flash, AMHW_HC32_FLASH_WRITE);
        } else {
            return -AM_EAGAIN;
        }
    }

    /* write data */
    switch (data_size) {
    case 1:
        *((volatile uint8_t*)addr) = *(uint8_t *)data;
        break;

    case 2:
        *((volatile uint16_t*)addr) = *(uint16_t *)data;
        break;

    case 4:
        *((volatile uint32_t*)addr) = *(uint32_t *)data;
        break;

    default:
        return -AM_EINVAL;
    }

    /* busy? */
    timeout = AM_HC32_FLASH_TIMEOUT_PGM;
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    return AM_OK;
}

/**
 * \brief 对页按字节编程
 */
int32_t flash_sector_byte_program (amhw_hc32_flash_t    *p_hw_flash,
                                           uint32_t              dst_addr,
                                           const uint8_t        *p_src,
                                           uint32_t              size)
{
    uint32_t i = 0;
    
    if (AM_HC32_FLASH_END_ADDR < (dst_addr + size)) {
        return -AM_EINVAL;
    }

    for(i = 0; i < size; i++) {
        flash_write(p_hw_flash, dst_addr + i, &p_src[i], 1);
    }

    for (i = 0; i < size; i++) {
        if (p_src[i] != *(uint8_t *)(dst_addr + i)) {
            break;
        }
    }    
    return i;
}

/**
 * \brief 擦除页
 */
int32_t flash_sector_erase (amhw_hc32_flash_t *p_hw_flash,
                                    uint32_t           addr)
{
    volatile uint32_t  timeout = AM_HC32_FLASH_TIMEOUT_ERASE;

    if (AM_HC32_FLASH_END_ADDR < addr) {
        return -AM_EINVAL;
    }

    /* busy? */
    timeout = AM_HC32_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }

    /* set op */
    timeout = AM_HC32_FLASH_TIMEOUT_ERASE;
    while(AMHW_HC32_FLASH_SECTOR_ERASE != amhw_hc32_flash_opt_get(p_hw_flash)) {
        if(timeout--) {
            
            amhw_hc32_flash_opt_set(p_hw_flash, AMHW_HC32_FLASH_SECTOR_ERASE);
        } else {
            return -AM_EAGAIN;
        }
    }

    /* write data */
    *((volatile uint8_t*)addr) = 0;

    /* busy? */
    timeout = AM_HC32_FLASH_TIMEOUT_ERASE;
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        if(0 == timeout--) {
            return -AM_EAGAIN;
        }
    }
    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_hc32_drv_flash_write_protect_entry(amhw_hc32_flash_t *p_flash, uint16_t sector)
{
    int             i;
    uint32_t        status;    /* FLASH 命令执行状态 */

    /* 要写入 FLASH 的数据 */
    static uint8_t data1[__DATA_SIZE];
    /* 从 FLASH 中读出的数据 */
    static uint8_t temp1[__DATA_SIZE];
    /* 数据初始化 */
    for (i = 0; i < __DATA_SIZE; i++) {
        data1[i] = i;
    }
    
    flash_unlockall(p_flash);
    /* 擦除页 */
    status = flash_sector_erase(p_flash, sector << 9);
    /* 页擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");
        AM_FOREVER;
    }

    /* 向页中写入数据 */
    status = flash_sector_byte_program(p_flash,
                                              sector << 9,
                                              data1,
                                              __DATA_SIZE);
    /* 页写入出错，程序停在此处 */
    if (__DATA_SIZE != status) {
        AM_DBG_INFO("byte program error!\r\n");
        AM_FOREVER;
    }
    
    /* ###########屏蔽该语句可验证########### */
    flash_lockall(p_flash);  
    /* 擦除页 */
    status = flash_sector_erase(p_flash, sector << 9);
    
    
    /* 从页读取数据 */
    for (i = 0; i < __DATA_SIZE; i++) {
        temp1[i] = *(uint8_t *)(i + (sector << 9));
    }

    for (i = 0; i < __DATA_SIZE; i++) {
        AM_DBG_INFO("%04d  ", temp1[i]);
    }
    AM_DBG_INFO("\r\n");

    AM_DBG_INFO("\r\nflash test success!\r\n");
}


