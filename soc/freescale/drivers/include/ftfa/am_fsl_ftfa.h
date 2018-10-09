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
 * \brief FTFA驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_FTFA_H
#define __AM_FSL_FTFA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "hw/amhw_fsl_ftfa.h"
/**
 * \addtogroup am_fsl_if_ftfa
 * \copydoc am_fsl_ftfa.h
 * @{
 */

/**
 *  \brief FTFA模块初始化
 *
 *  \return 无
 */
void am_fsl_ftfa_init (void);

/**
 *  \brief 擦除扇区
 *
 *  \param start_addr 扇区的起始地址
 *
 *  \return >0: 执行结果, -AM_EINVAL: 输入地址过大
 */
int32_t am_fsl_ftfa_sector_erase (amhw_fsl_ftfa_t * AMHW_FSL_FTFA,uint32_t start_addr);

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
                                    uint32_t  size);

/**
 *  \brief 擦除所有扇区
 *
 *  \return 执行结果
 *
 */
uint32_t am_fsl_ftfa_all_sector_erase (amhw_fsl_ftfa_t * AMHW_FSL_FTFA);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_FSL_FTFA_H */
