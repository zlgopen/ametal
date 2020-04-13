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
 * - 1.00 17-04-17  ari, first implementation
 * \endinternal
 */

#include "am_zlg_flash.h"

#include "am_types.h"
#include "am_bitops.h"
#include "ametal.h"

/*******************************************************************************
  defines
*******************************************************************************/

#define UNLOCK_FALSH_KEY1   0x45670123  /**< \brief unlock key word */
#define UNLOCK_FALSH_KEY2   0xCDEF89AB  /**< \brief unlock key word */

#define FALSH_ADDRESS_BASE   0x08000000
#define FALSH_ADDRESS_SIZE   0x08000000 + FLASH_SIZE
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
void am_zlg_flash_init (amhw_zlg_flash_t *p_hw_flash)
{
    amhw_zlg_flash_key_set(p_hw_flash, UNLOCK_FALSH_KEY1);
    amhw_zlg_flash_key_set(p_hw_flash, UNLOCK_FALSH_KEY2);

    amhw_zlg_flash_status_flag_clr(p_hw_flash,
                                   AMHW_ZLG_FLASH_WRPRTERR_FLAG |
                                   AMHW_ZLG_FLASH_PGERR_FLAG    |
                                   AMHW_ZLG_FLASH_BUSY_FLAG);

    //amhw_zlg_flash_set_prebuff_on(p_hw_flash, AM_FALSE);
    //amhw_zlg_flash_half_cycle_enable(p_hw_flash, AM_FALSE);

    return;
}

/**
 * \brief 擦除扇区
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 * \param[in] start_addr 扇区的起始地址
 *
 * \return > 0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zlg_flash_sector_erase (amhw_zlg_flash_t *p_hw_flash,
                                   uint32_t          start_addr)
{
    if (start_addr < FALSH_ADDRESS_BASE) {
        start_addr += FALSH_ADDRESS_BASE;
    }
    if (FALSH_ADDRESS_SIZE < start_addr) {
        return -AM_EINVAL;
    }
    while (amhw_zlg_flash_status_check(p_hw_flash,
                                       AMHW_ZLG_FLASH_BUSY_FLAG)) {
        ;
    }
    amhw_zlg_flash_cs_reg_set(p_hw_flash,
                              AMHW_ZLG_FLASH_PAGE_ERASE_MASK);

    amhw_zlg_flash_address_set(p_hw_flash, start_addr);

    amhw_zlg_flash_cs_reg_set(p_hw_flash,
                              AMHW_ZLG_FLASH_START_MASK);

    while (amhw_zlg_flash_status_check(p_hw_flash,
                                       AMHW_ZLG_FLASH_BUSY_FLAG)) {
        ;
    }
    return AM_OK;
}

/**
 * \brief 对扇区编程或部分扇区编程
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 * \param[in] dst_addr  写入到flash的起始地址
 * \param[in] p_src     要写入到flash中的数据的起始地址
 * \param[in] size      写入字(32bit)的个数
 *
 * \retval 0 实际成功写入的字数
 */
int32_t am_zlg_flash_sector_program (amhw_zlg_flash_t *p_hw_flash,
                                     uint32_t          dst_addr,
                                     uint32_t         *p_src,
                                     uint32_t          size)
{
    uint32_t i;

    if (dst_addr < FALSH_ADDRESS_BASE) {
        dst_addr += FALSH_ADDRESS_BASE;
    }

    /** size不能大于扇区的大小 */
    if (size > SECTOR_SIZE || dst_addr >= FALSH_ADDRESS_SIZE) {
        return -AM_EINVAL;
    }

    if (amhw_zlg_flash_cs_reg_get(p_hw_flash) & AMHW_ZLG_FLASH_LOCK_MASK) {
        amhw_zlg_flash_key_set(p_hw_flash, UNLOCK_FALSH_KEY1);
        amhw_zlg_flash_key_set(p_hw_flash, UNLOCK_FALSH_KEY2);
    }

    amhw_zlg_flash_cs_reg_set(p_hw_flash, AMHW_ZLG_FLASH_PROGRAM_MASK);

    /** 对flash编程 */
    for (i = 0; i < size; i++) {

        /** 半字写入 */
        *(uint16_t *)(dst_addr + i * 4)     = (uint16_t)p_src[i];

        /** 等待写入完成 */
        while (amhw_zlg_flash_status_check(p_hw_flash,
                                           AMHW_ZLG_FLASH_BUSY_FLAG)) {
           ;
        }

        *(uint16_t *)(dst_addr + i * 4 + 2) = (uint16_t)(p_src[i] >> 16);
        while (amhw_zlg_flash_status_check(p_hw_flash,
                                           AMHW_ZLG_FLASH_BUSY_FLAG)) {
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
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 *
 * \return 执行结果
 *
 */
uint32_t am_zlg_flash_all_sector_erase (amhw_zlg_flash_t *p_hw_flash)
{
    while (amhw_zlg_flash_status_check(p_hw_flash,
                                       AMHW_ZLG_FLASH_BUSY_FLAG)) {
        ;
    }

    amhw_zlg_flash_cs_reg_set(p_hw_flash, AMHW_ZLG_FLASH_MASS_ERASE_MASK);
    amhw_zlg_flash_cs_reg_set(p_hw_flash, AMHW_ZLG_FLASH_START_MASK);

    while (amhw_zlg_flash_status_check(p_hw_flash,
                                       AMHW_ZLG_FLASH_BUSY_FLAG)) {
         ;
    }
    return AM_OK;
}

/* end of file */
