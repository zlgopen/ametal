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
 * \brief 芯片片上系统硬件配置， 该配置文件将影响 外设寄存器列表的定义
 *
 * 部分硬件可能需要配置，例如，通道数目等， 不同芯片可能存在差异。 本文件包含了指定
 * 芯片对应的硬件配置。每个芯片目录下均应包含一个该文件。
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-26  yrh, first implementation
 * \endinternal
 */

#ifndef __SOC_CFG_H
#define __SOC_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name ZMF237 硬件配置，配置项来自于使用到的各个驱动
 * @{
 */

/** \brief DMA 通道数目配置，该配置项被用于  hw/amhw_zlg_dma.h 文件中 */
#define SOC_CFG_ZLG_DMA_CHAN_CNT          7

/**< \brief flash的大小 以字节(8bit)为单位 */
#define FLASH_SIZE  (128 * 1024UL)

/**< \brief 扇区大小 以字(32bit)为单位 */
#define SECTOR_SIZE (4096UL / 4)

/**@} */

#ifdef __cplusplus
}
#endif

#endif /* __SOC_CFG_H */

/* end of file */
