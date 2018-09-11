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
 * \brief BME 位操作引擎硬件操作接口
 *
 * \note BME它从硬件上提供了对外设地址空间用读-修改-写的方式来实现位操作
 *       BME支持访问从0x40000000开始的，大小为512K的外设地址空间
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-27  nwt, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_BME_H
#define __AMHW_FSL_BME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"
#include "../../../../kl26/kl26_inum.h"
#include "../../../../kl26/kl26_periph_map.h"

/**
 * \addtogroup amhw_fsl_if_bme
 * \copydoc amhw_fsl_bme.h
 * @{
 */

/**
 * \name  bme指令掩码宏定义
 * @{
 */

#define AMHW_KL26_BME_AND_MASK               (1<<26)   /**< \brief 与命令掩码.  */
#define AMHW_KL26_BME_OR_MASK                (1<<27)   /**< \brief 或命令掩码.  */
#define AMHW_KL26_BME_XOR_MASK               (3<<26)   /**< \brief 异或命令掩码. */

/** \brief BFI命令掩码  */
#define AMHW_KL26_BME_BFI_MASK(BIT,WIDTH)    (1<<28) | (BIT<<23) | ((WIDTH-1)<<19)

/** \brief 读操作清零命令掩码  */
#define AMHW_KL26_BME_LAC1_MASK(BIT)         (1<<27) | (BIT<<21)

/** \brief 读操作置位命令掩码  */
#define AMHW_KL26_BME_LAS1_MASK(BIT)         (3<<26) | (BIT<<21)

/** \brief 提取位段命令掩码  */
#define AMHW_KL26_BME_UBFX_MASK(BIT,WIDTH)   (1<<28) | (BIT<<23) | ((WIDTH-1)<<19)

/** @} */

/**
 * \name  bme封装式存储指令宏定义
 * @{
 */

/* \brief  BME 逻辑指令,用于对addr地址上的内容进行逻辑操作，另外两个的用法与之相似
 * \par 示例
 * \code
 *  AMHW_KL26_BME_AND_*(外设寄存器地址, 数据);
 * \endcode
 */

/** \brief Decorated Store: Logical AND only to write one byte */
#define  AMHW_KL26_BME_AND_B(addr, wdata)      \
             *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_AND_MASK) = wdata;

/** \brief Decorated Store: Logical AND only to write two bytes  */
#define  AMHW_KL26_BME_AND_H(addr, wdata)    \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_AND_MASK) = wdata;

/** \brief Decorated Store: Logical AND only to write four bytes */
#define  AMHW_KL26_BME_AND_W(addr, wdata)    \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_AND_MASK) = wdata;

/** \brief Decorated Store: Logical OR only to write one byte    */
#define  AMHW_KL26_BME_OR_B(addr, wdata)       \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_OR_MASK) = wdata;

/** \brief Decorated Store: Logical OR only to write two bytes   */
#define  AMHW_KL26_BME_OR_H(addr, wdata)     \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_OR_MASK) = wdata;

/** \brief Decorated Store: Logical OR only to write four bytes  */
#define  AMHW_KL26_BME_OR_W(addr, wdata)     \
            *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_OR_MASK) = wdata;

/** \brief Decorated Store: Logical XOR only to write one byte   */
#define  AMHW_KL26_BME_XOR_B(addr, wdata)      \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_XOR_MASK) = wdata;

/** \brief Decorated Store: Logical XOR only to write two bytes  */
#define  AMHW_KL26_BME_XOR_H(addr, wdata)    \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_XOR_MASK) = wdata;

/** \brief Decorated Store: Logical XOR only to write four bytes  */
#define  AMHW_KL26_BME_XOR_W(addr, wdata)    \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_XOR_MASK) = wdata;

/* \brief  BME 位插入逻辑替换指令,用于对addr地址上的内容指定位开始处进行width长度个位替换
 * \par 示例
 * \code
 *  AMHW_KL26_BME_BFI_*(外设寄存器地址, 数据, bit, 替换位的长度);
 * \endcode
 */

/** \brief Decorated Store: Bit Field Insert of one byte */
#define  AMHW_KL26_BME_BFI_B(addr, wdata, bit, width)    \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_BFI_MASK(bit,width)) = wdata;

/** \brief Decorated Store: Bit Field Insert of two bytes */
#define  AMHW_KL26_BME_BFI_H(addr, wdata, bit, width)  \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_BFI_MASK(bit,width)) = wdata;

/** \brief Decorated Store: Bit Field Insert of four bytes */
#define  AMHW_KL26_BME_BFI_W(addr, wdata, bit, width)  \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_BFI_MASK(bit,width)) = wdata;

/** @} */

/**
 * \name  bme封装式载入指令宏定义
 * @{
 */

/** \brief Decorated Load: Load and Clear 1-bit of one byte */
#define  AMHW_KL26_BME_LAC1_B(addr, bit)    \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_LAC1_MASK(bit));

/** \brief Decorated Load: Load and Clear 1-bit of two bytes */
#define  AMHW_KL26_BME_LAC1_H(addr, bit)    \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_LAC1_MASK(bit));

/** \brief Decorated Load: Load and Clear 1-bit of four bytes */
#define  AMHW_KL26_BME_LAC1_W(addr, bit)     \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_LAC1_MASK(bit));

/** \brief Decorated Load: Load and Set 1-bit of one byte  */
#define  AMHW_KL26_BME_LAS1_B(addr, bit)             \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_LAS1_MASK(bit));

/** \brief Decorated Load: Load and Set 1-bit of two bytes  */
#define  AMHW_KL26_BME_LAS1_H(addr, bit)            \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_LAS1_MASK(bit));

/** \brief Decorated Load: Load and Set 1-bit of four bytes */
#define  AMHW_KL26_BME_LAS1_W(addr, bit)            \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_LAS1_MASK(bit));

/** \brief Decorated Load: Unsigned Bit Field Extract of one byte  */
#define  AMHW_KL26_BME_UBFX_B(addr, bit, width)     \
          *(volatile uint8_t*)((uint32_t)addr | AMHW_KL26_BME_UBFX_MASK(bit,width));

/** \brief Decorated Load: Unsigned Bit Field Extract of two bytes  */
#define  AMHW_KL26_BME_UBFX_H(addr, bit, width)    \
          *(volatile uint16_t*)((uint32_t)addr | AMHW_KL26_BME_UBFX_MASK(bit,width));

/** \brief Decorated Load: Unsigned Bit Field Extract of fours bytes */
#define  AMHW_KL26_BME_UBFX_W(addr, bit, width)    \
          *(volatile uint32_t*)((uint32_t)addr | AMHW_KL26_BME_UBFX_MASK(bit,width));

/** @} */

/**
 * @} amhw_FSL_if_bme
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_BME_H */

/* end of file */

