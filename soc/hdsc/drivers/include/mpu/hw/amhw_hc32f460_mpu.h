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
 * \brief MPU操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-13
 * \endinternal
 */

#ifndef __AMHW_HC32F460_MPU_H
#define __AMHW_HC32F460_MPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

  /* 默认使能匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

/**
 * \addtogroup amhw_hc32f460_if_mpu
 * \copydoc amhw_hc32f460_mpu.h
 * @{
 */

/**
 * \brief MPU寄存器块结构体
 */
typedef struct amhw_hc32f460_mpu {
    __IO uint32_t MPU_RGD[16];   /**< \brief 区域0~15范围描述寄存器 */
    __IO uint32_t MPU_RGCR[16];  /**< \brief 区域0~15控制寄存器 */
    __IO uint32_t MPU_CR;        /**< \brief MPU控制寄存器 */
    __I  uint32_t MPU_SR;        /**< \brief MPU状态寄存器  */
    __IO uint32_t MPU_ECLR;      /**< \brief MPU错误标志清除寄存器  */
    __IO uint32_t MPU_WP;        /**< \brief MPU写保护寄存器  */

} amhw_hc32f460_mpu_t;

/** \brief MPU地址/控制寄存器写允许 */
#define AMHW_HC32F460_MPU_WRITE_ENABLE  (0x96A5UL)
/** \brief MPU地址/控制寄存器写禁止 */
#define AMHW_HC32F460_MPU_WRITE_DISABLE (0x96A4UL)

/** \brief IP访问保护寄存器地址 */
#define AMHW_HC32F460_MPU_IPPR_REG_ADDR    (0x40054000 + 0x1c)

/**
 * \brief 系统DMA1存储保护单元共16区域，8区域系统DMA专用，8区域所有DMA共用
 *        系统DMA2存储保护单元共16区域，8区域系统DMA专用，8区域所有DMA共用
 *        USBFS-DMA存储保护单元共8区域，所有DMA共用
 */

typedef enum {
    AMHW_HC32F460_MPU_REGION_NUM0 = 0,
    AMHW_HC32F460_MPU_REGION_NUM1 = 1,
    AMHW_HC32F460_MPU_REGION_NUM2 = 2,
    AMHW_HC32F460_MPU_REGION_NUM3 = 3,
    AMHW_HC32F460_MPU_REGION_NUM4 = 4,
    AMHW_HC32F460_MPU_REGION_NUM5 = 5,
    AMHW_HC32F460_MPU_REGION_NUM6 = 6,
    AMHW_HC32F460_MPU_REGION_NUM7 = 7,
    AMHW_HC32F460_MPU_REGION_NUM8 = 8,
    AMHW_HC32F460_MPU_REGION_NUM9 = 9,
    AMHW_HC32F460_MPU_REGION_NUM10 = 10,
    AMHW_HC32F460_MPU_REGION_NUM11 = 11,
    AMHW_HC32F460_MPU_REGION_NUM12 = 12,
    AMHW_HC32F460_MPU_REGION_NUM13 = 13,
    AMHW_HC32F460_MPU_REGION_NUM14 = 14,
    AMHW_HC32F460_MPU_REGION_NUM15 = 15
} amhw_hc32f460_mpu_region_num_t;

/**
 * \brief MPU寄存器写允许
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_reg_wr_enable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
}

/**
 * \brief MPU寄存器写禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_reg_wr_disable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief 设置区域基地址
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 计数周期标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_region_base_set(amhw_hc32f460_mpu_t  *p_hw_mpu, uint8_t num, uint32_t addr)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGD[num] = (p_hw_mpu->MPU_RGD[num] & 0x1F) | (addr & (~0x1F));
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

typedef enum {
    AMHW_HC32F460_MPU_RGSIZE_32BYTE   = 4U,  /**< \brief 32 Byte */
    AMHW_HC32F460_MPU_RGSIZE_64BYTE   = 5U,  /**< \brief 64 Byte */
    AMHW_HC32F460_MPU_RGSIZE_128BYTE  = 6U,  /**< \brief 126 Byte */
    AMHW_HC32F460_MPU_RGSIZE_256BYTE  = 7U,  /**< \brief 256 Byte */
    AMHW_HC32F460_MPU_RGSIZE_512BYTE  = 8U,  /**< \brief 512 Byte */
    AMHW_HC32F460_MPU_RGSIZE_1KBYTE   = 9U,  /**< \brief 1K Byte */
    AMHW_HC32F460_MPU_RGSIZE_2KBYTE   = 10U, /**< \brief 2K Byte */
    AMHW_HC32F460_MPU_RGSIZE_4KBYTE   = 11U, /**< \brief 4K Byte */
    AMHW_HC32F460_MPU_RGSIZE_8KBYTE   = 12U, /**< \brief 8K Byte */
    AMHW_HC32F460_MPU_RGSIZE_16KBYTE  = 13U, /**< \brief 16K Byte */
    AMHW_HC32F460_MPU_RGSIZE_32KBYTE  = 14U, /**< \brief 32K Byte */
    AMHW_HC32F460_MPU_RGSIZE_64KBYTE  = 15U, /**< \brief 64K Byte */
    AMHW_HC32F460_MPU_RGSIZE_128KBYTE = 16U, /**< \brief 128K Byte */
    AMHW_HC32F460_MPU_RGSIZE_256KBYTE = 17U, /**< \brief 256K Byte */
    AMHW_HC32F460_MPU_RGSIZE_512KBYTE = 18U, /**< \brief 512K Byte */
    AMHW_HC32F460_MPU_RGSIZE_1MBYTE   = 19U, /**< \brief 1M Byte */
    AMHW_HC32F460_MPU_RGSIZE_2MBYTE   = 20U, /**< \brief 2M Byte */
    AMHW_HC32F460_MPU_RGSIZE_4MBYTE   = 21U, /**< \brief 4M Byte */
    AMHW_HC32F460_MPU_RGSIZE_8MBYTE   = 22U, /**< \brief 8M Byte */
    AMHW_HC32F460_MPU_RGSIZE_16MBYTE  = 23U, /**< \brief 16M Byte */
    AMHW_HC32F460_MPU_RGSIZE_32MBYTE  = 24U, /**< \brief 32M Byte */
    AMHW_HC32F460_MPU_RGSIZE_64MBYTE  = 25U, /**< \brief 64M Byte */
    AMHW_HC32F460_MPU_RGSIZE_128MBYTE = 26U, /**< \brief 128M Byte */
    AMHW_HC32F460_MPU_RGSIZE_256MBYTE = 27U, /**< \brief 256M Byte */
    AMHW_HC32F460_MPU_RGSIZE_512MBYTE = 28U, /**< \brief 512M Byte */
    AMHW_HC32F460_MPU_RGSIZE_1GBYTE   = 29U, /**< \brief 1G Byte */
    AMHW_HC32F460_MPU_RGSIZE_2GBYTE   = 30U, /**< \brief 2G Byte */
    AMHW_HC32F460_MPU_RGSIZE_4GBYTE   = 31U, /**< \brief 4G Byte */
} amhw_hc32f460_mpu_rgsize_t;


/**
 * \brief 设置区域大小
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_region_size_set(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                       uint8_t                    num,
                                       amhw_hc32f460_mpu_rgsize_t size)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGD[num] = (p_hw_mpu->MPU_RGD[num] & (~0x1F)) | size;
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                         uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 23);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                          uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 23);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_wr_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                            uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 17);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n写权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_wr_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 17);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_rd_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                            uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 16);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU区域n读权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] size     : 区域大小标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_fmpu_rd_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 16);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}


/**
 * \brief SMPU1区域n使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                          uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 15);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1区域n禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                           uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 15);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1区域n写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_wr_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 9);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1区域n写权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_wr_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 9);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1区域n读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_rd_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 8);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1区域n读权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu1_rd_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                              uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 8);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                          uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 7);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                           uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 7);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_wr_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 1);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n写权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_wr_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 1);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_rd_enable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                             uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] &= ~(1 << 0);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2区域n读权限禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] num      : 区域编号
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_rgcrn_smpu2_rd_disable(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                              uint8_t                    num)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_RGCR[num] |= (1 << 0);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_enable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 23);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_disable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 23);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}


typedef enum {
    AMHW_HC32F460_MPU_ACT_NONEACTION = 0,
    AMHW_HC32F460_MPU_ACT_BUSERR     = 1,
    AMHW_HC32F460_MPU_ACT_NMI        = 2,
    AMHW_HC32F460_MPU_ACT_RESET      = 3,
} amhw_hc32f460_mpu_act_t;

#define AMHW_HC32F460_MPU_CR_FMPUACT_MASK   (0x3 << 18)
#define AMHW_HC32F460_MPU_CR_SMPU1ACT_MASK  (0x3 << 10)
#define AMHW_HC32F460_MPU_CR_SMPU2ACT_MASK  (0x3 << 2)

/**
 * \brief 选择FMPU发生被禁止的访问时的动作
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpuact_set(amhw_hc32f460_mpu_t         *p_hw_mpu,
                                   amhw_hc32f460_mpu_act_t      type)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~AMHW_HC32F460_MPU_CR_FMPUACT_MASK;
    p_hw_mpu->MPU_CR |= type;
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU背景读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_background_rd_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 17);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU背景读权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_background_rd_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 17);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU背景写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_background_wr_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 16);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief FMPU背景写权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_fmpu_background_wr_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 16);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}


/**
 * \brief SMPU1使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_enable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 15);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_disable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 15);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief 选择SMPU1发生被禁止的访问时的动作
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1act_set(amhw_hc32f460_mpu_t         *p_hw_mpu,
                                    amhw_hc32f460_mpu_act_t      type)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~AMHW_HC32F460_MPU_CR_SMPU1ACT_MASK;
    p_hw_mpu->MPU_CR |= type;
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1背景读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_background_rd_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 9);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1背景读权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_background_rd_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 9);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1背景写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_background_wr_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 8);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU1背景写权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu1_background_wr_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 8);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}


/**
 * \brief SMPU2使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_enable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 7);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}


/**
 * \brief SMPU2禁能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_disable(amhw_hc32f460_mpu_t       *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 7);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief 选择SMPU2发生被禁止的访问时的动作
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2act_set(amhw_hc32f460_mpu_t         *p_hw_mpu,
                                    amhw_hc32f460_mpu_act_t      type)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~AMHW_HC32F460_MPU_CR_SMPU2ACT_MASK;
    p_hw_mpu->MPU_CR |= type;
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2背景读权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_background_rd_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 1);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2背景读权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_background_rd_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 1);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2背景写权限使能
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_background_wr_enable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR &= ~(1 << 0);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}

/**
 * \brief SMPU2背景写权限禁止
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_smpu2_background_wr_disable(amhw_hc32f460_mpu_t *p_hw_mpu)
{
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_ENABLE;
    p_hw_mpu->MPU_CR |= (1 << 0);
    p_hw_mpu->MPU_WP = AMHW_HC32F460_MPU_WRITE_DISABLE;
}



typedef enum {
    AMHW_HC32F460_STATUS_FMPU_ERR  = (1 << 16),
    AMHW_HC32F460_STATUS_SMPU1_ERR = (1 << 8),
    AMHW_HC32F460_STATUS_SMPU2_ERR = (1 << 0),
} amhw_hc32f460_mpu_status_t;

/**
 * \brief 获取MPU状态标志
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] type     : 状态类型掩码
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_hc32f460_mpu_status_flag_check(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                              amhw_hc32f460_mpu_status_t status)
{
    return (p_hw_mpu->MPU_SR & status) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除MPU状态标志
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] type     : 状态类型掩码
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_mpu_status_flag_clr(amhw_hc32f460_mpu_t       *p_hw_mpu,
                                       amhw_hc32f460_mpu_status_t status)
{
    p_hw_mpu->MPU_ECLR |= status;
}

/**
 * @}
 */
/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

  /* 允许匿名联合体使能 */
#elif defined(__GNUC__)

  /* 默认使用匿名联合体 */
#elif defined(__TMS470__)

  /* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

#ifdef __cplusplus
}
#endif /* __AMHW_HC32F460_MPU_H */

#endif

/* end of file */
