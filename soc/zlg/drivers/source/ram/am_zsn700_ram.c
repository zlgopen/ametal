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
 * \brief RAM模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-18
 * \endinternal
 */

#include "zsn700_inum.h"
#include "am_zsn700_ram.h"
#include "am_types.h"
#include "am_bitops.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "ametal.h"

/******************************************************************************
*   函数定义
******************************************************************************/

/** \brief RAM 中断处理函数 */
static void __ram_irq_handler (void *p_arg)
{
    amhw_zsn700_ram_err_clear((amhw_zsn700_ram_t *)p_arg);
    am_kprintf("RAM IRQ...\r\n");
}

/**
 * \brief RAM模块初始化
 *
 * \param[in] p_hw_ram 指向RAM寄存器块的指针
 *
 * \return 无
 */
void am_zsn700_ram_init (amhw_zsn700_ram_t *p_hw_ram)
{
    /* 清除出错标志 */
    amhw_zsn700_ram_err_clear(p_hw_ram);
    /* 使能出错报警中断 */
    amhw_zsn700_ram_ie_enable(p_hw_ram);

    /* 连接中断 */
    am_int_connect(INUM_FLASH_RAM, __ram_irq_handler, p_hw_ram);
    am_int_enable(INUM_FLASH_RAM);
}

/**
 * \brief 擦除RAM区域
 *
 * \param[in] p_hw_ram    指向RAM寄存器块的指针
 * \param[in] start_addr  起始地址
 * \param[in] size        大小
 *
 * \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_zsn700_ram_erase (amhw_zsn700_ram_t *p_hw_ram,
                             uint32_t           start_addr,
                             uint16_t           size)
{
    uint16_t i;

    if (start_addr < RAM_ADDRESS_BASE) {
        start_addr += RAM_ADDRESS_BASE;
    }
    if (RAM_ADDRESS_SIZE < start_addr) {
        return -AM_EINVAL;
    }

    for (i = 0; i < size; i++) {
        if(start_addr + i > RAM_ADDRESS_SIZE) {
            break;
        }
        *(uint8_t *)(start_addr + i) = 0;
    }

    return AM_OK;
}

/**
 * \brief 对扇区编程或部分扇区编程
 *
 * \param[in] p_hw_ram  指向RAM寄存器块的指针
 * \param[in] dst_addr  写入到ram的起始地址
 * \param[in] p_src     要写入到ram中的数据的起始地址
 * \param[in] size      写入字(32bit)的个数
 *
 * \retval i 实际成功写入的字数
 */
int32_t am_zsn700_ram_write (amhw_zsn700_ram_t *p_hw_ram,
                             uint32_t           dst_addr,
                             uint8_t           *p_src,
                             uint16_t           size)
{
    uint16_t i;

    if (dst_addr < RAM_ADDRESS_BASE) {
        dst_addr += RAM_ADDRESS_BASE;
    }

    /** size不能大于扇区的大小 */
    if (size > RAM_SIZE || dst_addr >= RAM_ADDRESS_SIZE) {
        return -AM_EINVAL;
    }

    for (i = 0; i < size; i++) {
        if(dst_addr + i > RAM_ADDRESS_SIZE) {
            break;
        }
        *(uint8_t *)(dst_addr + i) = *(p_src + i);
    }

    return i;
}

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
int32_t am_zsn700_ram_read (amhw_zsn700_ram_t *p_hw_ram,
                            uint32_t           dst_addr,
                            uint8_t           *p_src,
                            uint16_t           size)
{
    uint16_t i;

    if (dst_addr < RAM_ADDRESS_BASE) {
        dst_addr += RAM_ADDRESS_BASE;
    }

    /** size不能大于扇区的大小 */
    if (size > RAM_SIZE || dst_addr >= RAM_ADDRESS_SIZE) {
        return -AM_EINVAL;
    }

    for (i = 0; i < size; i++) {
        if(dst_addr + i > RAM_ADDRESS_SIZE) {
            break;
        }
        *(p_src + i) = *(uint8_t *)(dst_addr + i);
    }

    return i;
}

/* end of file */
