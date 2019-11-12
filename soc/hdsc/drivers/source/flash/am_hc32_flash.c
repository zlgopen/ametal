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
 * \brief Flash模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#include "am_hc32_flash.h"

#include "am_types.h"
#include "am_bitops.h"
#include "ametal.h"

/*******************************************************************************
  defines
*******************************************************************************/

#define UNLOCK_HC32_FLASH_KEY1   0x5A5A  /**< \brief unlock key word */
#define UNLOCK_HC32_FLASH_KEY2   0xA5A5  /**< \brief unlock key word */

#define FLASH_ADDRESS_BASE   0x00000000
#define FLASH_ADDRESS_SIZE   0x00000000 + FLASH_SIZE
/******************************************************************************
*   函数定义
******************************************************************************/

/**
 * \brief Flash模块初始化
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_hc32_flash_init (amhw_hc32_flash_t *p_hw_flash)
{
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

    amhw_hc32_flash_int_flag_clr(p_hw_flash, AMHW_HC32_FLASH_INT_ALL_FLAG);
}

/**
 * \brief 擦除扇区
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] start_addr 扇区的起始地址
 *
 * \return > 0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_hc32_flash_sector_erase (amhw_hc32_flash_t *p_hw_flash,
                                      uint32_t             start_addr)
{
    if (FLASH_ADDRESS_SIZE < start_addr) {
        return -AM_EINVAL;
    }

    /* 忙，就一直等待*/
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        ;
    }

    /* 写入指定数据，使能寄存器改写 */
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

    /* 选择FLASH擦除功能 */
    amhw_hc32_flash_opt_set(p_hw_flash, AMHW_HC32_FLASH_OPT_SECTOR_ERASE);

    /* 写入指定数据，使能寄存器改写 */
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

    /* 允许某对某扇区进行擦写 */
    amhw_hc32_flash_erase_enable(p_hw_flash,
                                   (start_addr - FLASH_ADDRESS_BASE) >> 11);

    /* 对扇区内的每一页的任意地址写入任意数据，触发页擦除，以完成扇区擦除功能 */
    *((uint8_t *)(start_addr + 0 * 512)) = 0x00;
    *((uint8_t *)(start_addr + 1 * 512)) = 0x00;
    *((uint8_t *)(start_addr + 2 * 512)) = 0x00;
    *((uint8_t *)(start_addr + 3 * 512)) = 0x00;

    /* 忙，就一直等待*/
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        ;
    }

    return AM_OK;
}

/**
 * \brief 对扇区编程或部分扇区编程
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] dst_addr   写入到flash的起始地址
 * \param[in] p_src      要写入到flash中的数据的起始地址
 * \param[in] size       写入字(32bit)的个数
 *
 * \retval 0 实际成功写入的字数
 */
int32_t am_hc32_flash_flash_program (amhw_hc32_flash_t *p_hw_flash,
                                       uint32_t             dst_addr,
                                       uint32_t            *p_src,
                                       uint32_t             size)
{
    uint32_t i;
    uint32_t t;

    /** size不能大于扇区的大小 */
    if (size > SECTOR_SIZE || dst_addr >= FLASH_ADDRESS_SIZE) {
        return -AM_EINVAL;
    }

    /* 写入指定数据，使能寄存器改写 */
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

    /* 选择FLASH写功能 */
    amhw_hc32_flash_opt_set(p_hw_flash, AMHW_HC32_FLASH_OPT_WRITE);

    /* 可能写入数据个数大于一个扇区，因此需要对多个扇区取消擦写保护 */
    t =  size / SECTOR_SIZE + 1;

    /* 允许某对某扇区进行擦写 */
    for (i = 0; i < t; i++) {

        /* 写入指定数据，使能寄存器改写 */
        amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
        amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

        /* 允许某对某扇区进行擦写 */
        amhw_hc32_flash_erase_enable(p_hw_flash,
                                       ((dst_addr - FLASH_ADDRESS_BASE) >> 11) + i);
    }

    for (i = 0; i < size; i++) {

        /** 半字写入 */
        *(uint16_t *)(dst_addr + i * 4)     = (uint16_t)p_src[i];

        /* 忙，就一直等待*/
        while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
            ;
        }

        /** 半字写入 */
        *(uint16_t *)(dst_addr + i * 4 + 2) = (uint16_t)(p_src[i] >> 16);

        /* 忙，就一直等待*/
        while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
            ;
        }
    }

    for (i = 0; i < size; i++) {

       /** 半字写入 */
      if (p_src[i] != *(uint32_t *)(dst_addr + i * 4)) {
          break;
      }
    }
    return i;
}

/**
 * \brief 擦除所有扇区
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 *
 * \return 执行结果
 *
 */
uint32_t am_hc32_flash_all_sector_erase (amhw_hc32_flash_t *p_hw_flash)
{
    uint8_t i;

    /* 写入指定数据，使能寄存器改写 */
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
    amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

    /* 选择FLASH写功能 */
    amhw_hc32_flash_opt_set(p_hw_flash, AMHW_HC32_FLASH_OPT_CHIP_ERASE);

    /* 取消所有扇区擦写保护 */
    for(i = 0; i < 128; i++) {

        /* 写入指定数据，使能寄存器改写 */
        amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY1);
        amhw_hc32_flash_key_set(p_hw_flash, UNLOCK_HC32_FLASH_KEY2);

        /* 允许某对某扇区进行擦写 */
        amhw_hc32_flash_erase_enable(p_hw_flash, i);
    }

    /* 对片内的任意地址进行写操作，触发全片擦除 */
    *((uint8_t *) 0x00000000) = 0x00;

    /* 忙，就一直等待*/
    while (AM_TRUE == amhw_hc32_flash_busy_check(p_hw_flash)) {
        ;
    }

    return AM_OK;
}

/* end of file */
