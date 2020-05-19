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
 * \brief ZML165 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07  sdy, first implementation
 * \endinternal
 */

#ifndef __ZLG_INUM_H
#define __ZLG_INUM_H

#ifdef __cplusplus
extern "C" {

#endif

#include "ametal.h"

/**
 * \addtogroup zml165_if_inum
 * \copydoc zml165_inum.h
 * @{
 */

/**
 * \name ZML165 CPU 中断号
 * @{
 */

#define INUM_WWDT                 0    /**< \brief 窗口定时器中断 */
#define INUM_PVD                  1    /**< \brief 电源电压检测中断 */

#define INUM_FLASH                3    /**< \brief 闪存全局中断 */
#define INUM_RCC_CRS              4    /**< \brief RCC和CRS*全局中断 */
#define INUM_EXTI0_1              5    /**< \brief EXTI线[1：0]中断 */
#define INUM_EXTI2_3              6    /**< \brief EXTI线[3：2]中断 */
#define INUM_EXTI4_15             7    /**< \brief EXTI线[15：4]中断 */

#define INUM_DMA1_1               9    /**< \brief DMA1通道1全局中断 */
#define INUM_DMA1_2_3             10   /**< \brief DMA1通道2、3全局中断 */
#define INUM_DMA1_4_5             11   /**< \brief DMA1通道4、5全局中断 */
#define INUM_ADC_COMP             12   /**< \brief ADC和比较器中断(与 EXTI19、20组合在一起) */
#define INUM_TIM1_BRK_UP_TRG_COM  13   /**< \brief TIM1刹车、更新、触发、COM */
#define INUM_TIM1_CC              14   /**< \brief TIM1捕捉比较中断 */
#define INUM_TIM2                 15   /**< \brief TIM2全局中断 */
#define INUM_TIM3                 16   /**< \brief TIM3全局中断 */

#define INUM_TIM14                19   /**< \brief TIM14全局中断 */

#define INUM_TIM16                21   /**< \brief TIM16全局中断 */
#define INUM_TIM17                22   /**< \brief TIM17全局中断 */
#define INUM_I2C1                 23   /**< \brief I2C1全局中断 */

#define INUM_SPI1                 25   /**< \brief SPI1全局中断 */
#define INUM_SPI2                 26   /**< \brief SPI2全局中断 */
#define INUM_UART1                27   /**< \brief UART1全局中断 */
#define INUM_UART2                28   /**< \brief UART2全局中断 */
#define INUM_AES                  29   /**< \brief AES全局中断 */
#define INUM_CAN                  30   /**< \brief CAN全局中断 */
#define INUM_USB                  31   /**< \brief USB全局中断 */

/** @} */

/**
 * \brief 总中断数为：(INUM_USB - INUM_WWDT + 1),
 *
 */
#define INUM_INTERNAL_COUNT     (INUM_USB - INUM_WWDT + 1)

/**
 * \brief 最大中断号为： INUM_USB
 */
#define INUM_INTERNAL_MAX        INUM_USB

/** \brief 最小中断号: INUM_WWDT */
#define INUM_INTERNAL_MIN        INUM_WWDT

/**
 * \cond
 * 中断号定义，为 core_cm0.h文件保留
 */
typedef enum {

    /* ----------------------  Cortex-M0核异常号  ------------------- */

    /** \brief 2  不可屏蔽中断, 不能被停止或抢占 */
    NonMaskableInt_IRQn      = -14,

    /** \brief 3  硬件错误中断 */
    HardFault_IRQn           = -13,

    /** \brief 11  系统服务调用通过SVC指令 */
    SVCall_IRQn              =  -5,

    /** \brief 14  系统的挂起请求 */
    PendSV_IRQn              =  -2,

    /** \brief 15  系统滴答定时器 */
    SysTick_IRQn             =  -1,

    /******  ZML165 Specific Interrupt Numbers *******************************************************/
    WWDT_IRQn                 = 0,        /**< \brief 窗口定时器中断 */
    PVD_IRQn                  = 1,        /**< \brief 电源电压检测中断 */
    Reserved0                 = 2,        /**< \brief RESERVED */
    FLASH_IRQn                = 3,        /**< \brief 闪存全局中断 */
    RCC_CRS_IRQn              = 4,        /**< \brief RCC和CRS*全局中断 */
    EXTI0_1_IRQn              = 5,        /**< \brief EXTI线[1：0]中断 */
    EXTI2_3_IRQn              = 6,        /**< \brief EXTI线[3：2]中断 */
    EXTI4_15_IRQn             = 7,        /**< \brief EXTI线[15：4]中断 */
    Reserved1                 = 8,        /**< \brief RESERVED */
    DMA1_1_IRQn               = 9,        /**< \brief DMA1通道1全局中断 */
    DMA1_2_3_IRQn             = 10,       /**< \brief DMA1通道2、3全局中断 */
    DMA1_4_5_IRQn             = 11,       /**< \brief DMA1通道4、5全局中断 */
    ADC_COMP_IRQn             = 12,       /**< \brief ADC和比较器中断 */
    TIM1_BRK_UP_TRG_COM_IRQn  = 13,       /**< \brief TIM1刹车、更新、触发、COM */
    TIM1_CC_IRQn              = 14,       /**< \brief TIM1捕捉比较中断 */
    TIM2_IRQn                 = 15,       /**< \brief TIM2全局中断 */
    TIM3_IRQn                 = 16,       /**< \brief TIM3全局中断 */
    Reserved2                 = 17,       /**< \brief RESERVED */
    Reserved3                 = 18,       /**< \brief RESERVED */
    TIM14_IRQn                = 19,       /**< \brief TIM14全局中断 */
    Reserved4                 = 20,       /**< \brief RESERVED */
    TIM16_IRQn                = 21,       /**< \brief TIM16全局中断 */
    TIM17_IRQn                = 22,       /**< \brief TIM17全局中断 */
    I2C1_IRQn                 = 23,       /**< \brief I2C1全局中断 */
    Reserved5                 = 24,       /**< \brief RESERVED */
    SPI1_IRQn                 = 25,       /**< \brief SPI1全局中断 */
    SPI2_IRQn                 = 26,       /**< \brief SPI2全局中断 */
    UART1_IRQn                = 27,       /**< \brief UART1全局中断 */
    UART2_IRQn                = 28,       /**< \brief UART2全局中断 */
    AES_IRQn                  = 29,       /**< \brief AES全局中断 */
    CAN_IRQn                  = 30,       /**< \brief CAN全局中断 */
    USB_IRQn                  = 31,       /**< \brief USB全局中断 */
} IRQn_Type;

/**
 * \endcond
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZLG_INUM_H */

/* end of file */
