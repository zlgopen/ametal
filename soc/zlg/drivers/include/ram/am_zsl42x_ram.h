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
 * - 1.00 19-10-08
 * \endinternal
 */

#ifndef __AM_ZSL42x_RAM_H
#define __AM_ZSL42x_RAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hw/amhw_zsl42x_ram.h"

/**
 * \addtogroup am_zsl42x_if_ram
 * \copydoc am_zsl42x_ram.h
 * @{
 */

/* RAM地址范围 */
#define RAM_SIZE            0x7FFF
#define RAM_ADDRESS_BASE    0x20000000
#define RAM_ADDRESS_SIZE   (0x20000000 + RAM_SIZE)

/**
 * \brief RAM初始化
 *
 * \param[in] p_hw_ram 指向RAM寄存器块的指针
 *
 * \return 无
 */
void am_zsl42x_ram_init (amhw_zsl42x_ram_t *p_hw_ram);

/**
 * \brief 擦除RAM区域
 *
 * \param[in] p_hw_ram    指向RAM寄存器块的指针
 * \param[in] start_addr  起始地址
 * \param[in] size        大小
 *
 * \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zsl42x_ram_erase (amhw_zsl42x_ram_t *p_hw_ram,
                             uint32_t           start_addr,
                             uint16_t           size);

/**
 * \brief 对RAM编程
 *
 * \param[in] p_hw_ram   指向RAM寄存器块的指针
 * \param[in] dst_addr   写入到ram的起始地址
 * \param[in] p_src      要写入到ram中的数据的起始地址
 * \param[in] size       写入字节的个数
 *
 * \retval i 实际成功写入的字数
 */
int32_t am_zsl42x_ram_write (amhw_zsl42x_ram_t *p_hw_ram,
                             uint32_t           dst_addr,
                             uint8_t           *p_src,
                             uint16_t           size);

/**
 * \brief 从RAM读取数据
 *
 * \param[in] p_hw_ram   指向RAM寄存器块的指针
 * \param[in] dst_addr   读取的ram起始地址
 * \param[in] p_src      要读取ram中的数据的起始地址
 * \param[in] size       读取字节的个数
 *
 * \retval i 实际成功读取的字数
 */
int32_t am_zsl42x_ram_read (amhw_zsl42x_ram_t *p_hw_ram,
                            uint32_t           dst_addr,
                            uint8_t           *p_src,
                            uint16_t           size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_ZSL42x_RAM_H */
