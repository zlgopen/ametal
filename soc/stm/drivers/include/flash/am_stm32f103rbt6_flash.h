/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg237.cn/
*******************************************************************************/

/**
 * \file
 * \brief Flash驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  ari, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG237_FLASH_H
#define __AM_ZLG237_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "soc_cfg.h"
#include "hw/amhw_zlg237_flash.h"

/**
 * \addtogroup am_zlg237_if_flash
 * \copydoc am_zlg237_flash.h
 * @{
 */

/**
 * \brief Flash模块初始化
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_zlg237_flash_init (amhw_zlg237_flash_t *p_hw_flash);

/**
 * \brief 擦除页区（页大小为1024）
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] start_addr  扇区的起始地址
 *
 * \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zlg237_flash_page_erase (amhw_zlg237_flash_t *p_hw_flash,
                                    uint32_t             start_addr);

/**
 * \brief 擦除半页（页大小为1024）
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 * \param[in] start_addr 半页的起始地址
 *
 * \return > 0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zlg237_flash_half_page_erase (amhw_zlg237_flash_t *p_hw_flash,
                                         uint32_t             start_addr);

/**
 * \brief 对扇区区编程或部分扇区编程
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] dst_addr   写入到flash的起始地址
 * \param[in] p_src      要写入到flash中的数据的起始地址
 * \param[in] size       写入字(32bit)的个数
 *
 * \retval 0 实际成功写入的字数
 */
int32_t am_zlg237_flash_sector_program (amhw_zlg237_flash_t *p_hw_flash,
                                        uint32_t             dst_addr,
                                        uint32_t            *p_src,
                                        uint32_t             size);

/**
 * \brief 擦除所有扇区
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 *
 * \return 执行结果
 *
 */
uint32_t am_zlg237_flash_all_sector_erase (amhw_zlg237_flash_t *p_hw_flash);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_ZLG237_FLASH_H */
