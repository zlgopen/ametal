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
 * \brief FTFA模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

#include "am_types.h"
#include "am_bitops.h"
#include "ametal.h"
#include "am_fsl_ftfa.h"
#include "hw/amhw_fsl_ftfa.h"


#define SECTOR_SIZE (1024UL / 4)     /**< \brief 扇区大小 以字(32bit)为单位     */

#define FLASH_SIZE  (1024*128UL)      /**< \brief flash的大小 以字节(8bit)为单位 */

/******************************************************************************
*   函数定义
******************************************************************************/

/**
 *  \brief FTFA模块初始化
 *
 *  \return 无
 */
void am_fsl_ftfa_init (void)
{
    amhw_fsl_ftfa_func_copy();
    return;
}

/**
 *  \brief 擦除扇区
 *
 *  \param start_addr 扇区的起始地址
 *
 *  \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_fsl_ftfa_sector_erase (amhw_fsl_ftfa_t * AMHW_FSL_FTFA,uint32_t start_addr)
{
    if (FLASH_SIZE < start_addr)
    {
        return -AM_EINVAL;
    }
    return amhw_fsl_ftfa_sector_erase(AMHW_FSL_FTFA, start_addr);
}

/**
 *  \brief 对扇区编程或部分扇区编程
 *
 *  \param dst_addr 写入到flash的起始地址
 *  \param p_src 要写入到flash中的数据的起始地址
 *  \param size 写入字(32bit)的个数
 *
 *  \retval >=0 实际成功写入的字数
 *  \retval -AM_EINVAL 输入的写入字数过大
 *  \retval -AM_EINVAL 指定的区域存在未擦除区域
 *  \retval -AM_EAGAIN 需编程的flash还未擦除，请擦除后重试
 *  \retval -AM_ERROR  未知错误
 */
int32_t am_fsl_ftfa_flash_program (amhw_fsl_ftfa_t * AMHW_FSL_FTFA,
									uint32_t  dst_addr,
                                    uint32_t *p_src,
                                    uint32_t  size)
{
    uint32_t status;
    int      i;

    /** size不能大于扇区的大小 */
    if (size > SECTOR_SIZE || dst_addr >= FLASH_SIZE) {
        return -AM_EINVAL;
    }

    /** 验证flash已擦除 */
    status = amhw_fsl_ftfa_1s_read(AMHW_FSL_FTFA,
                                    dst_addr,
                                    size,
                                    0);

    /** 对flash编程 */
    if (0 == status) {
        for (i = 0; i < size; i++) {

            /** 编程1个字 */
            status = amhw_fsl_ftfa_word_program(AMHW_FSL_FTFA,
                                                 dst_addr + (i*4),
                                                 p_src[i]);

            if (0 != status) {
                break;
            }

            /** 检测写入的是否正确 */
            status = amhw_fsl_ftfa_program_check(AMHW_FSL_FTFA,
                                                  dst_addr + i*4,
                                                  p_src[i],
                                                  0x01);

            if (0 != status) {
                break;
            }

        }

        if (i <= size && i > 0) {
            return i;          /** 部分写入成功返回 i           */
        }

        if (0 == i) {
            return -AM_EAGAIN; /** 完全没有写入 返回 -AM_EAGAIN */
        }
    } else {                   /** 擦除校验错误 返回 -AM_EAGAIN */
        return -AM_EAGAIN;
    }
    return -AM_ERROR;
}

/**
 *  \brief 擦除所有扇区
 *
 *  \return 执行结果
 *
 */
uint32_t am_fsl_ftfa_all_sector_erase (amhw_fsl_ftfa_t * AMHW_FSL_FTFA)
{
    return amhw_fsl_ftfa_all_blocks_erase(AMHW_FSL_FTFA);
}


