/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X FMC 硬件操作接口描述
 *
 * 1. 可控制闪存访问时间；
 * 2. 提供用于生成闪存签名的寄存器。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-04  sky, modified.
 * - 1.00 15-07-16  aii, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_FMC_H
#define __AMHW_LPC82X_FMC_H

#include "ametal.h"
#include "am_lpc82x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_fmc
 * \copydoc amhw_lpc82x_fmc.h
 * @{
 */

/**
 * \brief Flash控制器寄存器结构体块
 */
typedef struct amhw_lpc82x_fmc {
    __I  uint32_t reserved0[4];         /**< \brief 保留 */
    __IO uint32_t flashcfg;             /**< \brief 闪存配置寄存器 */
    __I  uint32_t reserved1[3];         /**< \brief 保留 */
    __IO uint32_t fmsstart;             /**< \brief 签名起始地址寄存器 */
    __IO uint32_t fmsstop;              /**< \brief 签名停止地址寄存器 */
    __I  uint32_t reserved2[1];         /**< \brief 保留 */
    __I  uint32_t fmsw0;                /**< \brief 签名字 */
} amhw_lpc82x_fmc_t;

/**
 * \brief 用于闪存访问时间配置
 *        1个系统时钟的闪存访问时间只能在20MHz时钟下
 *        2个系统时钟的闪存访问时间只能在30MHz时钟下
 */
typedef enum amhw_lpc82x_fmc_time {
    AMHW_LPC82X_FMC_FLASHTIM_1 = 0,     /**< \brief 1个系统时钟的闪存访问时间 */
    AMHW_LPC82X_FMC_FLASHTIM_2 = 1      /**< \brief 2个系统时钟的闪存访问时间 */
} amhw_lpc82x_fmc_time_t;

/**
 * \brief 闪存访问时间配置
 * \param[in] time : 闪存访问时间(系统时钟周期为单位)
 * \return 无
 */
am_static_inline
void amhw_lpc82x_fmc_flashtim_set (amhw_lpc82x_fmc_time_t time)
{
    LPC82X_FMC->flashcfg = time & 0x3;
}

/**
 * \brief 读取闪存访问时间
 * \param[in] : 无
 * \return 签名访问时间(系统时钟周期为单位)
 */
am_static_inline
amhw_lpc82x_fmc_time_t amhw_lpc82x_fmc_flashtim_get (void)
{
    return (amhw_lpc82x_fmc_time_t)(LPC82X_FMC->flashcfg & 0x3);
}

/**
 * \brief 签名生成起始地址
 * \param[in] addr : 签名生成起始地址(对应于AHB字节地址位【20:4】即16位对齐)
 * \return 无
 */
am_static_inline
void amhw_lpc82x_fmc_fmsstart_set (uint32_t addr)
{
    LPC82X_FMC->fmsstart = (addr >> 4) & 0x1ffff;
}

/**
 * \brief 获取签名生成起始地址
 * \param[in] : 无
 * \return 签名生成起始地址
 */
am_static_inline
uint32_t amhw_lpc82x_fmc_fmsstart_get (void)
{
    return ((LPC82X_FMC->fmsstart) & 0x1ffff) << 4;
}

/**
 * \brief 签名生成结束地址
 * \param[in] addr : 签名生成结束地址(对应于AHB字节地址位【20:4】即16位对齐)
 * \return 无
 * \note STOPA指定字包含在地址范围，地址单位为存储字而非字节
 */
am_static_inline
void amhw_lpc82x_fmc_fmsstop_set (uint32_t addr)
{
    LPC82X_FMC->fmsstop = (addr >> 4) & 0x1ffff;
}

/**
 * \brief 获取签名生成结束地址
 * \param[in] : 无
 * \return 签名生成结束地址
 */
am_static_inline
uint32_t amhw_lpc82x_fmc_fmsstop_get (void)
{
    return ((LPC82X_FMC->fmsstop) & 0x1ffff) << 4;
}

/**
 * \brief 开始签名生成
 */
am_static_inline
void amhw_lpc82x_fmc_strtbist (void)
{
    LPC82X_FMC->fmsstop |= (1UL << 31);
}

/**
 * \brief 获取签名信息
 * \param[in] : 无
 * \return 返回flash签名信息
 */
am_static_inline
uint32_t amhw_lpc82x_fmc_flash_sign_get (void)
{
    return LPC82X_FMC->fmsw0;
}

/**
 * \brief 快速获取签名信息
 *
 * \param[in] start_addr : 签名生成起始地址(对应于AHB字节地址位【20:4】即16位对齐)
 * \param[in] end_addr   : 签名生成结束地址(对应于AHB字节地址位【20:4】即16位对齐)
 *
 * \return 返回flash签名信息
 */
am_static_inline
uint32_t amhw_lpc82x_fmc_flash_sign (uint32_t start_addr, uint32_t end_addr)
{
    LPC82X_FMC->fmsstart = (start_addr >> 4) & 0x1ffff;
    LPC82X_FMC->fmsstop  = ((end_addr >> 4) & 0x1ffff) | 0x80000000;
    return LPC82X_FMC->fmsw0;
}

/**
 * @} amhw_lpc82x_if_fmc
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_FMC_H */

/* end of file */
