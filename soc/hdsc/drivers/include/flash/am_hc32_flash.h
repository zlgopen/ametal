/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
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
 * - 1.01 20-05-13  licl, add byte and half word program
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32_FLASH_H
#define __AM_HC32_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hc32_soc_cfg.h"
#include "hw/amhw_hc32_flash.h"

/**
 * \addtogroup am_hc32_if_flash
 * \copydoc am_hc32_flash.h
 * @{
 */

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/**
 * \brief 设置读FLASH周期
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] waitcycle   周期
 *
 * \return AM_OK: 成功
 */
int32_t am_hc32_flash_waitcycle(amhw_hc32_flash_t              *p_hw_flash,
                                amhw_hc32_flash_read_waittime   waitcycle);

/**
 * \brief 擦除页
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] start_addr  页的起始地址
 *
 * \return AM_OK: 成功, -AM_EINVAL: 输入地址过大
 */
int32_t am_hc32_flash_sector_erase (amhw_hc32_flash_t  *p_hw_flash,
                                    uint32_t            addr);

/**
 * \brief 对页按字节编程
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] dst_addr   写入到flash的起始地址
 * \param[in] p_src      要写入到flash中的数据的起始地址
 * \param[in] size       写入字节(8bit)的个数（当size大于页大小(SECTOR_SIZE)时，不应只先擦除一页）
 *
 * \return >0: 实际成功写入的字数， <0: 失败
 */
int32_t am_hc32_flash_sector_byte_program (amhw_hc32_flash_t    *p_hw_flash,
                                           uint32_t              dst_addr,
                                           const uint8_t        *p_src,
                                           uint32_t              size);

/**
 * \brief 对页按半字编程
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] dst_addr   写入到flash的起始地址
 * \param[in] p_src      要写入到flash中的数据的起始地址
 * \param[in] size       写入半字(16bit)的个数（当size * 2 大于页大小(SECTOR_SIZE)时，不应只先擦除一页）
 *
 * \return >0: 实际成功写入的字数， <0: 失败
 */
int32_t am_hc32_flash_sector_halfword_program (amhw_hc32_flash_t    *p_hw_flash,
                                               uint32_t              dst_addr,
                                               const uint16_t       *p_src,
                                               uint32_t              size);

/**
 * \brief 对页按字编程
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] dst_addr   写入到flash的起始地址
 * \param[in] p_src      要写入到flash中的数据的起始地址
 * \param[in] size       写入字(32bit)的个数（当size * 4 大于页大小(SECTOR_SIZE)时，不应只先擦除一页）
 *
 * \return >0: 实际成功写入的字数， <0: 失败
 */
int32_t am_hc32_flash_sector_program (amhw_hc32_flash_t    *p_hw_flash,
                                      uint32_t              dst_addr,
                                      const uint32_t       *p_src,
                                      uint32_t              size);

/**
 * \brief 擦除所有页(仅程序运行在SRAM中有效)
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 *
 * \return AM_OK: 成功，其它: 失败
 */
int32_t am_hc32_flash_all_sector_erase (amhw_hc32_flash_t *p_hw_flash);

/**
 * \brief Flash模块初始化
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] freqcfg    flash时间参数
 * \param[in] dpstb_able dpstb使能控制
 *
 * \return AM_OK: 成功，其它: 失败
 */
int32_t am_hc32_flash_init(amhw_hc32_flash_t   *p_hw_flash,
                           uint8_t              freqcfg,
                           am_bool_t            dpstb_able);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_HC32_FLASH_H */
