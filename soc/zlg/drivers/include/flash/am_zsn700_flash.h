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
 * - 1.00 20-03-17  YRZ, first implementation
 * \endinternal
 */

#ifndef __AM_ZSN700_FLASH_H
#define __AM_ZSN700_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "soc_cfg.h"
#include "hw/amhw_zsn700_flash.h"

/**
 * \addtogroup am_zsn700_if_flash
 * \copydoc am_zsn700_flash.h
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
 * \return 执行结果
 */
int32_t am_zsn700_flash_waitcycle(amhw_zsn700_flash_t              *p_hw_flash,
                                  amhw_zsn700_flash_read_waittime   waitcycle);

/**
 * \brief 擦除扇区
 *
 * \param[in] p_hw_flash  指向FLASH寄存器块的指针
 * \param[in] start_addr  扇区的起始地址
 *
 * \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zsn700_flash_sector_erase (amhw_zsn700_flash_t  *p_hw_flash,
                                      uint32_t              addr);

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
int32_t am_zsn700_flash_sector_program (amhw_zsn700_flash_t    *p_hw_flash,
                                        uint32_t                dst_addr,
                                        uint32_t               *p_src,
                                        uint32_t                size);

/**
 * \brief 擦除所有扇区(仅程序运行在SRAM中有效)
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 *
 * \return 执行结果
 *
 */
int32_t am_zsn700_flash_all_sector_erase (amhw_zsn700_flash_t *p_hw_flash);

/**
 * \brief Flash模块初始化
 *
 * \param[in] p_hw_flash 指向FLASH寄存器块的指针
 * \param[in] freqcfg    flash时间参数
 * \param[in] dpstb_able dpstb使能控制
 *
 * \return 无
 */
int32_t am_zsn700_flash_init(amhw_zsn700_flash_t   *p_hw_flash,
                             uint8_t                freqcfg,
                             am_bool_t              dpstb_able);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_ZSN700_FLASH_H */
