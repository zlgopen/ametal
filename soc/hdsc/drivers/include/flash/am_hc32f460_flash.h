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
 * \brief Flash驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-06-17  cds, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_FLASH_H
#define __AM_HC32F460_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_hc32f460_flash.h"

/**
 * \addtogroup am_hc32f460_if_flash
 * \copydoc am_hc32f460_flash.h
 * @{
 */

/**
 * \brief EFM寄存器写保护解除
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_hc32f460_flash_unlock(amhw_hc32f460_flash_t            *p_hw_flash);

/**
 * \brief 设置FLASH读插入的等待周期
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] waitcycle   周期
 *
 * \return 无
 */
void am_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t            *p_hw_flash,
                                 amhw_hc32f460_flash_read_waittime waitcycle);

/**
 * \brief EFM寄存器写保护开启
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_hc32f460_flash_lock(amhw_hc32f460_flash_t *p_hw_flash);


/**
 * \brief FLASH初始化
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 *
 * \return 无
 */
void am_hc32f460_flash_init (amhw_hc32f460_flash_t *p_hw_flash);


/**
 * \brief 擦除扇区
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 * \param[in] start_addr 扇区的起始地址
 *
 * \return > 0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_hc32f460_flash_sector_erase (amhw_hc32f460_flash_t *p_hw_flash,
                                        uint32_t               start_addr);


/**
 * \brief 全擦除
 *
 * \param[in] p_hw_gpio  指向FLASH寄存器块的指针
 *
 * \return 执行结果
 */
int32_t am_hc32f460_flash_all_sector_erase (amhw_hc32f460_flash_t *p_hw_flash);


/**
 * \brief 对flash编程
 *
 * \param[in] p_hw_gpio 指向FLASH寄存器块的指针
 * \param[in] dst_addr  写入到flash的起始地址
 * \param[in] p_src     要写入到flash中的数据的起始地址
 * \param[in] size      写入字(32bit)的个数
 *
 * \retval 0 实际成功写入的字数
 */
int32_t am_hc32f460_flash_sector_program (amhw_hc32f460_flash_t *p_hw_flash,
                                          uint32_t               dst_addr,
                                          uint32_t              *p_src,
                                          uint32_t               size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_HC32_FLASH_H */
