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
 * \brief EFM Peripheral Access Layer
 *
 *   The Miscellaneous System Control Module (FTFC) contains CPU configuration
 *   registers and on-chip memory controller registers.
 *
 * \internal
 * \par Modification history
 * - 1.00 20-06-17  cds, first implementation
 *
 * \endinternal
 */

#ifndef __AMHW_HC32F460_FLASH_H
#define __AMHW_HC32F460_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "hc32f460_regbase.h"

/**
 * \addtogroup amhw_hc32f460_if_flash
 * \copydoc amhw_hc32f460_flash.h
 * @{
 */

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

typedef struct amhw_hc32f460_flash {

    __IO uint32_t FAPRT;    /**< \brief FLASH访问保护寄存器 */

    __IO uint32_t FSTP;     /**< \brief FLASH停止寄存器 */

    __IO uint32_t FRMC;     /**< \brief FLASH读模式寄存器 */

    __IO uint32_t FWMC;     /**< \brief FLASH擦写模式寄存器 */

    __IO uint32_t FSR;      /**< \brief FLASH状态寄存器 */

    __IO uint32_t FSCLR;    /**< \brief FLASH状态清除寄存器 */

    __IO uint32_t FITE;     /**< \brief FLASH中断许可寄存器 */

    __IO uint32_t FSWP;     /**< \brief FLASH引导交换状态寄存器 */

    __IO uint32_t FPMTSW;   /**< \brief FLASH改写允许区域起始地址寄存器 */

    __IO uint32_t FPMTEW;   /**< \brief FLASH改写允许区域结束地址寄存器 */

    uint8_t RESERVED10[40]; /**< \brief 保留 */

    __IO uint32_t UQID1;    /**< \brief FLASH 芯片ID寄存器1 */

    __IO uint32_t UQID2;    /**< \brief FLASH 芯片ID寄存器2 */

    __IO uint32_t UQID3;    /**< \brief FLASH 芯片ID寄存器3 */

    uint8_t RESERVED13[164];/**< \brief 保留 */

    __IO uint32_t MMF_REMPRT;

    __IO uint32_t MMF_REMCR0;

    __IO uint32_t MMF_REMCR1;

} amhw_hc32f460_flash_t;

/**
 * \brief 读FLASH周期
 */
typedef enum {
    AMHW_HC32F460_FLASH_READ_WAITTIME_0 = 0,  /**< \brief 不插入等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_1 ,     /**< \brief 插入1个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_2 ,     /**< \brief 插入2个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_3 ,     /**< \brief 插入3个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_4 ,     /**< \brief 插入4个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_5 ,     /**< \brief 插入5个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_6 ,     /**< \brief 插入6个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_7 ,     /**< \brief 插入7个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_8 ,     /**< \brief 插入8个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_9 ,     /**< \brief 插入9个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_10 ,    /**< \brief 插入10个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_11 ,    /**< \brief 插入11个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_12 ,    /**< \brief 插入12个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_13 ,    /**< \brief 插入13个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_14 ,    /**< \brief 插入14个等待周期 */
    AMHW_HC32F460_FLASH_READ_WAITTIME_15 ,    /**< \brief 插入15个等待周期 */
} amhw_hc32f460_flash_read_waittime;

#define __AMHW_HC32F460_EFM_LOCK            (0x00000000u)
#define __AMHW_HC32F460_EFM_UNLOCK          (0x00000001u)
#define __AMHW_HC32F460_EFM_KEY1            (0x0123ul)
#define __AMHW_HC32F460_EFM_KEY2            (0x3210ul)

am_static_inline
void amhw_hc32f460_flash_lock(amhw_hc32f460_flash_t *p_hw_flash)
{
    if (__AMHW_HC32F460_EFM_UNLOCK== p_hw_flash->FAPRT)
    {
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
    }
}

am_static_inline
void amhw_hc32f460_flash_unlock(amhw_hc32f460_flash_t *p_hw_flash)
{
    if (__AMHW_HC32F460_EFM_LOCK == p_hw_flash->FAPRT)
    {
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY1;
        p_hw_flash->FAPRT = __AMHW_HC32F460_EFM_KEY2;
    }
}

/**
 * \brief 设置FLASH读插入的等待周期
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] waitcycle   : FLASH读插入的等待周期
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t            *p_hw_flash,
                                   amhw_hc32f460_flash_read_waittime waitcycle)
{
    uint32_t reg_value;

    reg_value = p_hw_flash->FRMC;
    p_hw_flash->FRMC = ((reg_value & (0xF << 4)) | (waitcycle << 4));
}

am_static_inline
void amhw_hc32f460_flash_enable(amhw_hc32f460_flash_t *p_hw_flash, am_bool_t flag)
{
    p_hw_flash->FSTP = (flag == AM_TRUE) ? 0 : 1;
}

/**
 * \brief FLASH缓存数据复位
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_crst_enable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC |= (1 << 24);
}

/**
 * \brief FLASH缓存数据不复位
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_crst_disable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC &= ~(1 << 24);
}


/**
 * \brief FLASH缓存功能使能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_cache_enable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC |= (1 << 16);
}

/**
 * \brief FLASH缓存功能禁能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_cache_disable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC &= ~(1 << 16);
}


/**
 * \brief FLASH超低速运行模式使能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_lvm_enable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC |= (1 << 8);
}

/**
 * \brief FLASH超低速运行模式禁止
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_lvm_disable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC &= ~(1 << 8);
}

/**
 * \brief FLASH超低功耗读使能
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_slpmd_enable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC |= (1 << 0);
}

/**
 * \brief FLASH超低功耗读禁止
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_readmode_slpmd_disable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FRMC &= ~(1 << 0);
}

/**
 * \brief FLASH编程擦除期间，总线释放
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_fwmc_bushldctl_set(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FWMC |= (1 << 8);
}

/**
 * \brief FLASH编程擦除期间，总线被占用
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_fwmc_bushldctl_clr(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FWMC &= ~(1 << 8);
}

typedef enum {
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_READONLY1       = 0, /**< \brief 只读模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_SINGLEPROGRAM   = 1, /**< \brief 单编程模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_SINGLEPROGRAMRB = 2, /**< \brief 单编程回读模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_SEQPROGRAM      = 3, /**< \brief 连续编程模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_SECTORERASE     = 4, /**< \brief 扇区擦除模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_MASSERASE       = 5, /**< \brief 全擦除模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_READONLY2       = 6, /**< \brief 只读模式 */
    AMHW_HC32F460_FLASH_ERASE_PGM_MD_READONLY3       = 7, /**< \brief 只读模式 */
}amhw_hc32f460_flash_mode_t;

/**
 * \brief FLASH模式设置
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] mode        : 擦除模式标志
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_fwmc_pemod_set(amhw_hc32f460_flash_t            *p_hw_flash,
                                        amhw_hc32f460_flash_mode_t        mode)
{
    uint32_t reg_val = p_hw_flash->FWMC;
    reg_val &= ~(0x7 << 4);
    reg_val |= (mode << 4);
    p_hw_flash->FWMC = reg_val;
}

/**
 * \brief FLASH 允许改写PEMOD
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_fwmc_pemode_enable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FWMC |= 1;
}

/**
 * \brief FLASH 禁止改写PEMOD
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return none
 */
am_static_inline
void amhw_hc32f460_flash_fwmc_pemode_disable(amhw_hc32f460_flash_t *p_hw_flash)
{
    p_hw_flash->FWMC &= ~1;
}

#define AMHW_HC32F460_EFM_STATUS_PEWERR       (1 << 0) /**< \brief 擦写模式错误标志 */
#define AMHW_HC32F460_EFM_STATUS_PEPRTERR     (1 << 1) /**< \brief 对保护地址编程/擦除错误标志位 */
#define AMHW_HC32F460_EFM_STATUS_PGSZERR      (1 << 2) /**< \brief 编程地址和大小不对齐标志位 */
#define AMHW_HC32F460_EFM_STATUS_PGMISMTCHERR (1 << 3) /**< \brief 单编程回读值不一致标志位 */
#define AMHW_HC32F460_EFM_STATUS_OPTEND       (1 << 4) /**< \brief 操作结束标志位 */
#define AMHW_HC32F460_EFM_STATUS_COLERR       (1 << 5) /**< \brief FLASH读写访问错误标志位 */
#define AMHW_HC32F460_EFM_STATUS_RDY          (1 << 8) /**< \brief FLASH忙/空闲标志位 */

/**
 * \brief FLASH EFM状态标志获取
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return EFM状态标志
 */
am_static_inline
uint32_t amhw_hc32f460_flash_efm_status_get(amhw_hc32f460_flash_t *p_hw_flash)
{
    return p_hw_flash->FSR;
}

/**
 * \brief FLASH EFM状态标志清除
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] flag        : 状态标志位
 *
 * return EFM状态标志
 */
am_static_inline
void amhw_hc32f460_flash_efm_status_clr(amhw_hc32f460_flash_t *p_hw_flash,
                                        uint32_t               flag)
{
    p_hw_flash->FSCLR = flag;
}

typedef enum {
    AMHW_HC32F460_EFM_INTTYPE_PEERRITE    = 0, /**< \brief 编程/擦除错误中断许可 */
    AMHW_HC32F460_EFM_INTTYPE_OPTENDITE   = 1, /**< \brief 操作结束中断许可 */
    AMHW_HC32F460_EFM_INTTYPE_RDCOLERRITE = 2, /**< \brief 读冲突错误中断许可 */
} amhw_hc32f460_efm_int_type_t;

/**
 * \brief FLASH EFM中断许可
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] flag        : 中断类型标志位
 *
 * return EFM状态标志
 */
am_static_inline
void amhw_hc32f460_flash_efm_int_enable(amhw_hc32f460_flash_t       *p_hw_flash,
                                        amhw_hc32f460_efm_int_type_t flag)
{
    p_hw_flash->FITE |= (1 << flag);
}

/**
 * \brief FLASH EFM中断禁止
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] flag        : 中断类型标志位
 *
 * return EFM状态标志
 */
am_static_inline
void amhw_hc32f460_flash_efm_int_disable(amhw_hc32f460_flash_t       *p_hw_flash,
                                         amhw_hc32f460_efm_int_type_t flag)
{
    p_hw_flash->FITE &= ~(1 << flag);
}


/**
 * \brief FLASH 窗口保护起始设置
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] addr        : 地址
 *
 * return
 */
am_static_inline
void amhw_hc32f460_flash_efm_fpmtsw_set(amhw_hc32f460_flash_t *p_hw_flash,
                                        uint32_t               addr)
{
    p_hw_flash->FPMTSW = addr;
}

/**
 * \brief FLASH 窗口保护结束地址设置
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 * \param[in] addr        : 地址
 *
 * return
 */
am_static_inline
void amhw_hc32f460_flash_efm_fpmtew_set(amhw_hc32f460_flash_t *p_hw_flash,
                                        uint32_t               addr)
{
    p_hw_flash->FPMTEW = addr;
}

/**
 * \brief FLASH 读取芯片唯一码1
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return 芯片唯一码1
 */
am_static_inline
uint32_t amhw_hc32f460_flash_efm_uqid1_get(amhw_hc32f460_flash_t *p_hw_flash)
{
    return p_hw_flash->UQID1;
}

/**
 * \brief FLASH 读取芯片唯一码2
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return 芯片唯一码1
 */
am_static_inline
uint32_t amhw_hc32f460_flash_efm_uqid2_get(amhw_hc32f460_flash_t *p_hw_flash)
{
    return p_hw_flash->UQID2;
}

/**
 * \brief FLASH 读取芯片唯一码3
 *
 * \param[in] p_hw_flash  : 指向FLASH结构体的指针
 *
 * return 芯片唯一码1
 */
am_static_inline
uint32_t amhw_hc32f460_flash_efm_uqid3_get(amhw_hc32f460_flash_t *p_hw_flash)
{
    return p_hw_flash->UQID3;
}



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

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_FLASH_H */

/* end of file */
