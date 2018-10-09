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
 * \brief LLWU 硬件操作接口
 * 
 * - 配置LLS和VLLSx模式LLWU唤醒源，唤醒源可为引脚，内部模块，滤波通道
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-14  ari, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_LLWU_H
#define __AMHW_FSL_LLWU_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_types.h"
#include "am_bitops.h"
/**
 * \addtogroup amhw_fsl_if_llwu
 * \copydoc amhw_fsl_llwu.h
 * @{
 */
 
/**
 * \brief LLWU 寄存器块结构体
 */
typedef struct amhw_fsl_llwu {
    __IO uint8_t  pe[4];          /**< \brief LLWU 唤醒引脚使能寄存器  */
    __IO uint8_t  me;             /**< \brief LLWU 唤醒模块使能寄存器  */
    __IO uint8_t  f[3];           /**< \brief LLWU 唤醒标志寄存器      */
    __IO uint8_t  filt[2];        /**< \brief LLWU 唤醒引脚滤波寄存器  */
} amhw_fsl_llwu_t;

/**
 * \brief LLWU 唤醒引脚(_X表示该芯片不支持)
 */
typedef enum amhw_fsl_llwu_pin {
    AMHW_FSL_LLWU_PIN_0_X  = 0,     /**< \brief LLWU 唤醒引脚P0    */
    AMHW_FSL_LLWU_PIN_1_X,          /**< \brief LLWU 唤醒引脚P1    */
    AMHW_FSL_LLWU_PIN_2_X,          /**< \brief LLWU 唤醒引脚P2    */
    AMHW_FSL_LLWU_PIN_3_X,          /**< \brief LLWU 唤醒引脚P3    */
    AMHW_FSL_LLWU_PIN_4_X,          /**< \brief LLWU 唤醒引脚P4    */
    AMHW_FSL_LLWU_PIN_5_PTB0,       /**< \brief LLWU 唤醒引脚P5    */
    AMHW_FSL_LLWU_PIN_6_PTC1,       /**< \brief LLWU 唤醒引脚P6    */
    AMHW_FSL_LLWU_PIN_7_PTC3,       /**< \brief LLWU 唤醒引脚P7    */
    AMHW_FSL_LLWU_PIN_8_PTC4,       /**< \brief LLWU 唤醒引脚P8    */
    AMHW_FSL_LLWU_PIN_9_PTC5,       /**< \brief LLWU 唤醒引脚P9    */
    AMHW_FSL_LLWU_PIN_10_PTC6,      /**< \brief LLWU 唤醒引脚P10   */
    AMHW_FSL_LLWU_PIN_11_X,         /**< \brief LLWU 唤醒引脚P11   */
    AMHW_FSL_LLWU_PIN_12_X,         /**< \brief LLWU 唤醒引脚P12   */
    AMHW_FSL_LLWU_PIN_13_X,         /**< \brief LLWU 唤醒引脚P13   */
    AMHW_FSL_LLWU_PIN_14_PTD4,      /**< \brief LLWU 唤醒引脚P14   */
    AMHW_FSL_LLWU_PIN_15_PTD6,      /**< \brief LLWU 唤醒引脚P15   */
} amhw_fsl_llwu_pin_t;

/**
 * \brief LLWU 唤醒引脚参数
 */
typedef enum amhw_fsl_llwu_pin_arg{
    AMHW_FSL_LLWU_PIN_ARG_DISABLE  = 0,  /**< \brief 不使用唤醒引脚 */
    AMHW_FSL_LLWU_PIN_ARG_RISING,        /**< \brief 上升沿唤醒     */
    AMHW_FSL_LLWU_PIN_ARG_FALLING,       /**< \brief 下降沿唤醒     */
    AMHW_FSL_LLWU_PIN_ARG_ANY            /**< \brief 引脚变化唤醒   */
} amhw_fsl_llwu_pin_arg_t;

/**
 * \brief LLWU 唤醒模块(_X表示该芯片不支持)
 */
typedef enum amhw_fsl_llwu_module {
    AMHW_FSL_LLWU_MODULE_0_LPTMR0  = 0,     /**< \brief LLWU 唤醒模块M0   */
    AMHW_FSL_LLWU_MODULE_1_CMP0,            /**< \brief LLWU 唤醒模块M1   */
    AMHW_FSL_LLWU_MODULE_2_X,               /**< \brief LLWU 唤醒模块M2   */
    AMHW_FSL_LLWU_MODULE_3_X,               /**< \brief LLWU 唤醒模块M3   */
    AMHW_FSL_LLWU_MODULE_4_TSI0,            /**< \brief LLWU 唤醒模块M4   */
    AMHW_FSL_LLWU_MODULE_5_RTCA,            /**< \brief LLWU 唤醒模块M5   */
    AMHW_FSL_LLWU_MODULE_6_X,               /**< \brief LLWU 唤醒模块M6   */
    AMHW_FSL_LLWU_MODULE_7_RTCS ,           /**< \brief LLWU 唤醒模块M7   */
} amhw_fsl_llwu_module_t;

/**
 * \brief LLWU 唤醒模块参数
 */
typedef enum amhw_fsl_llwu_module_arg {
    AMHW_FSL_LLWU_MODULE_ARG_DISABLE = 0,     /**< \brief LLWU 唤醒模块禁能   */
    AMHW_FSL_LLWU_MODULE_ARG_ENABLE = 1       /**< \brief LLWU 唤醒模块使能   */
} amhw_fsl_llwu_module_arg_t;

/**
 * \brief LLWU 滤波通道
 */
typedef enum amhw_fsl_llwu_filt_chan {
    AMHW_FSL_LLWU_FILT_CHAN1  = 0,     /**< \brief LLWU 唤醒引脚滤波通道1   */
    AMHW_FSL_LLWU_FILT_CHAN2,          /**< \brief LLWU 唤醒引脚滤波通道2   */
} amhw_fsl_llwu_filt_chan_t;

/**
 * \brief LLWU 滤波通道参数
 */
typedef enum amhw_fsl_llwu_filt_chan_arg{
    AMHW_FSL_LLWU_FILT_DETECT_DISABLE  = 0,  /**< \brief 不使用滤波通道 */
    AMHW_FSL_LLWU_FILT_DETECT_POSEDGE,       /**< \brief 上升沿检测     */
    AMHW_FSL_LLWU_FILT_DETECT_NEGEDGE,       /**< \brief 下降沿检测     */
    AMHW_FSL_LLWU_FILT_DETECT_ANYEDGE        /**< \brief 双边沿检测     */
} amhw_fsl_llwu_filt_chan_arg_t;

/**
 * \name LLWU 唤醒标志(_X表示该芯片不支持)
 * @{
 */
typedef enum amhw_fsl_llwu_wuf {
    AMHW_FSL_LLWU_WUF_P0_X      = AM_BIT(0),    /**< \brief LLWU 唤醒标志P0   */
    AMHW_FSL_LLWU_WUF_P1_X      = AM_BIT(1),    /**< \brief LLWU 唤醒标志P1   */
    AMHW_FSL_LLWU_WUF_P2_X      = AM_BIT(2),    /**< \brief LLWU 唤醒标志P2   */
    AMHW_FSL_LLWU_WUF_P3_X      = AM_BIT(3),    /**< \brief LLWU 唤醒标志P3   */
    AMHW_FSL_LLWU_WUF_P4_X      = AM_BIT(4),    /**< \brief LLWU 唤醒标志P4   */
    AMHW_FSL_LLWU_WUF_P5_PTB0   = AM_BIT(5),    /**< \brief LLWU 唤醒标志P5   */
    AMHW_FSL_LLWU_WUF_P6_PTC1   = AM_BIT(6),    /**< \brief LLWU 唤醒标志P6   */
    AMHW_FSL_LLWU_WUF_P7_PTC3   = AM_BIT(7),    /**< \brief LLWU 唤醒标志P7   */
    AMHW_FSL_LLWU_WUF_P8_PTC4   = AM_BIT(8),    /**< \brief LLWU 唤醒标志P8   */
    AMHW_FSL_LLWU_WUF_P9_PTC5   = AM_BIT(9),    /**< \brief LLWU 唤醒标志P9   */
    AMHW_FSL_LLWU_WUF_P10_PTC6  = AM_BIT(10),   /**< \brief LLWU 唤醒标志P10  */
    AMHW_FSL_LLWU_WUF_P11_X     = AM_BIT(11),   /**< \brief LLWU 唤醒标志P11  */
    AMHW_FSL_LLWU_WUF_P12_X     = AM_BIT(12),   /**< \brief LLWU 唤醒标志P12  */
    AMHW_FSL_LLWU_WUF_P13_X     = AM_BIT(13),   /**< \brief LLWU 唤醒标志P13  */
    AMHW_FSL_LLWU_WUF_P14_PTD4  = AM_BIT(14),   /**< \brief LLWU 唤醒标志P14  */
    AMHW_FSL_LLWU_WUF_P15_PTD6  = AM_BIT(15),   /**< \brief LLWU 唤醒标志P15  */

    /* 内部模块标志不可以直接清除 */
    AMHW_FSL_LLWU_WUF_M0_LPTMR0 = AM_BIT(16),   /**< \brief LLWU 唤醒标志M0   */
    AMHW_FSL_LLWU_WUF_M1_CMP0   = AM_BIT(17),   /**< \brief LLWU 唤醒标志M1   */
    AMHW_FSL_LLWU_WUF_M2_X      = AM_BIT(18),   /**< \brief LLWU 唤醒标志M2   */
    AMHW_FSL_LLWU_WUF_M3_X      = AM_BIT(19),   /**< \brief LLWU 唤醒标志M3   */
    AMHW_FSL_LLWU_WUF_M4_TSI0   = AM_BIT(20),   /**< \brief LLWU 唤醒标志M4   */
    AMHW_FSL_LLWU_WUF_M5_RTCA   = AM_BIT(21),   /**< \brief LLWU 唤醒标志M5   */
    AMHW_FSL_LLWU_WUF_M6_X      = AM_BIT(22),   /**< \brief LLWU 唤醒标志M6   */
    AMHW_FSL_LLWU_WUF_M7RTC     = AM_BIT(23),   /**< \brief LLWU 唤醒标志M7   */

    AMHW_FSL_LLWU_WUF_F1        = AM_BIT(24),   /**< \brief LLWU 唤醒标志F1   */
    AMHW_FSL_LLWU_WUF_F2        = AM_BIT(25)    /**< \brief LLWU 唤醒标志F2   */
} amhw_fsl_llwu_wuf_t;

/** @} */

/**
 * \brief 配置引脚唤醒功能
 *
 * \param[in] p_hw_llwu : 指向LLWU寄存器块的指针
 * \param[in] wu_pin    : 唤醒引脚
 * \param[in] arg       : 引脚参数
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_llwu_pin_cfg (amhw_fsl_llwu_t        *p_hw_llwu,
                             amhw_fsl_llwu_pin_t     wu_pin,
                             amhw_fsl_llwu_pin_arg_t arg)
{
    uint8_t arraypos = wu_pin / 4;
    uint8_t bitpos   = (wu_pin % 4) * 2;
    p_hw_llwu->pe[arraypos] = (arg << bitpos) |
                              (p_hw_llwu->pe[arraypos] & (~(0x3 << bitpos)));
}

/**
 * \brief 配置内部模块唤醒功能
 *
 * \param[in] p_hw_llwu : 指向LLWU寄存器块的指针
 * \param[in] module    : 唤醒模块
 * \param[in] arg       : 模块参数
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_llwu_module_cfg (amhw_fsl_llwu_t            *p_hw_llwu,
                                amhw_fsl_llwu_module_t      module,
                                amhw_fsl_llwu_module_arg_t  arg)
{
    if (arg == AMHW_FSL_LLWU_MODULE_ARG_ENABLE) {
        p_hw_llwu->me |= (1 << module);
    } else {
        p_hw_llwu->me &= ~(1 << module);
    }
}

/**
 * \brief 配置滤波通道参数
 *
 * \param[in] p_hw_llwu : 指向LLWU寄存器块的指针
 * \param[in] chan      : 滤波通道
 * \param[in] wu_pin    : 唤醒引脚
 * \param[in] arg       : 检测参数
 *
 * \return 无
 */
am_static_inline
void amhw_fsl_llwu_filt_cfg (amhw_fsl_llwu_t              *p_hw_llwu,
                              amhw_fsl_llwu_filt_chan_t     chan,
                              amhw_fsl_llwu_pin_t           wu_pin,
                              amhw_fsl_llwu_filt_chan_arg_t arg)
{
    p_hw_llwu->filt[chan] = (arg << 5) | wu_pin;
}

/**
 * \brief 获取LLWU唤醒标志
 * \param[in] p_hw_llwu : 指向LLWU寄存器块的指针
 * \return 唤醒标志
 */
am_static_inline
uint32_t amhw_fsl_llwu_wuflags_get (amhw_fsl_llwu_t *p_hw_llwu)
{
    uint32_t flags =  (uint32_t)p_hw_llwu->f[0]                      | /* Px */
                     ((uint32_t)p_hw_llwu->f[1] << 8)                | /* Px */
                     ((uint32_t)p_hw_llwu->f[2] << 16)               | /* Mx */
                     ((uint32_t)(p_hw_llwu->filt[0] & (0x80)) << 17) | /* F1 */
                     ((uint32_t)(p_hw_llwu->filt[1] & (0x80)) << 18);  /* F2 */

    return flags;
}

/**
 * \brief 清除LLWU唤醒标志
 *
 * \param[in] p_hw_llwu : 指向LLWU寄存器块的指针
 * \param[in] flags     : 唤醒标志宏定义，AMHW_FSL_LLWU_WUF_*宏值
 *                        （# AMHW_FSL_LLWU_WUF_P5_PTB0_）
 *
 * \return 无
 *
 * \note 内部模块标志通过清除外设标志清除
 */
am_static_inline
void amhw_fsl_llwu_wuflags_clr (amhw_fsl_llwu_t *p_hw_llwu, uint32_t flags)
{
    p_hw_llwu->f[0] = flags;                      /* 清除Px */
    p_hw_llwu->f[1] = flags >> 8;

    p_hw_llwu->filt[0] |= ((flags >> 17) & 0x80); /* 清除F1 */
    p_hw_llwu->filt[1] |= ((flags >> 18) & 0x80); /* 清除F2 */
}

/** 
 * @} amhw_fsl_if_llwu
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_LLWU_H */

/*end of file */
