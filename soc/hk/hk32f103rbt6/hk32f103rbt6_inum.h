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
 * \brief HK32F103RBT6 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-26  yrh, first implementation
 * \endinternal
 */

#ifndef __HK32F103RBT6_INUM_H
#define __HK32F103RBT6_INUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \addtogroup hk32f103rbt6_if_inum
 * \copydoc hk32f103rbt6_inum.h
 * @{
 */

/**
 * \name HK32F103RBT6 CPU 中断号
 * @{
 */

#define INUM_WWDG                 0    /**< \brief 窗口定时器中断 */
#define INUM_PVD                  1    /**< \brief 电源电压检测中断 */
#define INUM_TAMPER               2    /**< \brief 侵入检测中断 */
#define INUM_RTC                  3    /**< \brief 实时时钟（RTC）全局中断 */
#define INUM_FLASH                4    /**< \brief 闪存全局中断 */
#define INUM_RCC                  5    /**< \brief 复位和时钟控制（RCC）中断 */
#define INUM_EXTI0                6    /**< \brief EXTI线0中断 */
#define INUM_EXTI1                7    /**< \brief EXTI线1中断 */
#define INUM_EXTI2                8    /**< \brief EXTI线2中断 */
#define INUM_EXTI3                9    /**< \brief EXTI线3中断 */
#define INUM_EXTI4                10   /**< \brief EXTI线4中断 */
#define INUM_DMA1_1               11   /**< \brief DMA1通道1全局中断 */
#define INUM_DMA1_2               12   /**< \brief DMA1通道2全局中断 */
#define INUM_DMA1_3               13   /**< \brief DMA1通道3全局中断 */
#define INUM_DMA1_4               14   /**< \brief DMA1通道4全局中断 */
#define INUM_DMA1_5               15   /**< \brief DMA1通道5全局中断 */
#define INUM_DMA1_6               16   /**< \brief DMA1通道6全局中断 */
#define INUM_DMA1_7               17   /**< \brief DMA1通道7全局中断 */
#define INUM_ADC1_2               18   /**< \brief ADC1和ADC2的全局中断 */
#define INUM_USB_HP_CAN_TX        19   /**< \brief USB高优先级或CAN发送中断 */
#define INUM_USB_HP_CAN_RX        20   /**< \brief USB高优先级或CAN接收中断 */
#define INUM_CAN1_RX1             21   /**< \brief CAN接收1中断 */
#define INUM_CAN1_SCE             22   /**< \brief CAN SCE 中断 */
#define INUM_EXTI9_5              23   /**< \brief EXTI线[9：5]中断 */
#define INUM_TIM1_BRK             24   /**< \brief TIM1断开中断 */
#define INUM_TIM1_UP              25   /**< \brief TIM1更新中断 */
#define INUM_TIM1_TRG_COM         26   /**< \brief TIM1触发和通信中断 */
#define INUM_TIM1_CC              27   /**< \brief TIM1捕获比较中断 */
#define INUM_TIM2                 28   /**< \brief TIM2全局中断 */
#define INUM_TIM3                 29   /**< \brief TIM3全局中断 */
#define INUM_TIM4                 30   /**< \brief TIM4全局中断 */
#define INUM_I2C1_EV              31   /**< \brief I2C1事件中断 */
#define INUM_I2C1_ER              32   /**< \brief I2C1错误中断 */
#define INUM_I2C2_EV              33   /**< \brief I2C2事件中断 */
#define INUM_I2C2_ER              34   /**< \brief I2C2错误中断 */
#define INUM_SPI1                 35   /**< \brief SPI1全局中断 */
#define INUM_SPI2                 36   /**< \brief SPI2全局中断 */
#define INUM_USART1               37   /**< \brief UART1全局中断 */
#define INUM_USART2               38   /**< \brief UART2全局中断 */
#define INUM_USART3               39   /**< \brief UART3全局中断 */
#define INUM_EXTI15_10            40   /**< \brief EXTI线[15：10]中断 */
#define INUM_RTC_Alarm            41   /**< \brief 连到EXTI17的RTC闹钟中断 */
#define INUM_USB_WK               42   /**< \brief 连到EXTI18的从USB待机唤醒中断 */


/** @} */

/**
 * \brief 总中断数为：(INUM_USB - INUM_WWDT + 1),
 *
 */
#define INUM_INTERNAL_COUNT     (INUM_USB_WK - INUM_WWDG + 1)

/**
 * \brief 最大中断号为： INUM_USB
 */
#define INUM_INTERNAL_MAX        INUM_USB_WK

/** \brief 最小中断号: INUM_WWDT */
#define INUM_INTERNAL_MIN        INUM_WWDG

/**
 * \cond
 * 中断号定义，为 core_cm3.h文件保留
 */
typedef enum {

    /* ----------------------  Cortex-M3核异常号  ------------------- */

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

    /******  HK32F103RBT6 Specific Interrupt Numbers *******************************************************/
    WWDG_IRQn                = 0,         /**< \brief 窗口定时器中断 */
    PVD_IRQn                  =1,         /**< \brief 电源电压检测中断 */
    TAMPER_IRQn               =2,         /**< \brief 侵入检测中断 */
    RTC_IRQn                  =3,         /**< \brief 实时时钟（RTC）全局中断 */
    FLASH_IRQn                =4,         /**< \brief 闪存全局中断 */
    RCC_IRQn                  =5,         /**< \brief 复位和时钟控制（RCC）中断 */
    EXTI0_IRQn                =6,         /**< \brief EXTI线0中断 */
    EXTI1_IRQn                =7,         /**< \brief EXTI线1中断 */
    EXTI2_IRQn                =8,         /**< \brief EXTI线2中断 */
    EXTI3_IRQn                =9,         /**< \brief EXTI线3中断 */
    EXTI4_IRQn                =10,        /**< \brief EXTI线4中断 */
    DMA1_1_IRQn               =11,        /**< \brief DMA1通道1全局中断 */
    DMA1_2_IRQn               =12,        /**< \brief DMA1通道2全局中断 */
    DMA1_3_IRQn               =13,        /**< \brief DMA1通道3全局中断 */
    DMA1_4_IRQn               =14,        /**< \brief DMA1通道4全局中断 */
    DMA1_5_IRQn               =15,        /**< \brief DMA1通道5全局中断 */
    DMA1_6_IRQn               =16,        /**< \brief DMA1通道6全局中断 */
    DMA1_7_IRQn               =17,        /**< \brief DMA1通道7全局中断 */
    ADC1_2_IRQn               =18,        /**< \brief ADC1和ADC2的全局中断 */
    USB_CAN_TX_IRQn           =19,        /**< \brief USB高优先级或CAN发送中断 */
    USB_CAN_RX_IRQn           =20,        /**< \brief USB高优先级或CAN接收中断 */
    CAN1_RX1_IRQn             =21,        /**< \brief CAN接收1中断 */
    CAN1_SCE_IRQn             =22,        /**< \brief CAN_SCE 中断 */
    EXTI9_5_IRQn              =23,        /**< \brief EXTI线[9：5]中断 */
    TIM1_BRK_IRQn             =24,        /**< \brief TIM1断开中断 */
    TIM1_UP_IRQn              =25,        /**< \brief TIM1更新中断 */
    TIM1_TRG_COM_IRQn         =26,        /**< \brief TIM1触发和通信中断 */
    TIM1_CC_IRQn              =27,        /**< \brief TIM1捕获比较中断 */
    TIM2_IRQn                 =28,        /**< \brief TIM2全局中断 */
    TIM3_IRQn                 =29,        /**< \brief TIM3全局中断 */
    TIM4_IRQn                 =30,        /**< \brief TIM14全局中断 */
    I2C1_EV_IRQn              =31,        /**< \brief I2C1事件中断 */
    Reserved2                 =32,        /**< \brief RESERVED */
    I2C2_EV_IRQn              =33,        /**< \brief I2C1事件中断 */
    I2C2_ER_IRQn              =34,        /**< \brief I2C2错误中断 */
    SPI1_IRQn                 =35,        /**< \brief SPI1全局中断 */
    SPI2_IRQn                 =36,        /**< \brief SPI2全局中断 */
    UART1_IRQn                =37,        /**< \brief UART1全局中断 */
    UART2_IRQn                =38,        /**< \brief UART2全局中断 */
    UART3_IRQn                =39,        /**< \brief UART3全局中断 */
    EXTI15_10_IRQn            =40,        /**< \brief EXTI线[15：10]中断 */
    RTCAlarm_IRQn             =41,        /**< \brief 连到EXTI17的RTC闹钟中断 */
    USB_WK_IRQn               =42,        /**< \brief 连到EXTI18的从USB待机唤醒中断 */
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

#endif /* __HK32F103RBT6_INUM_H */

/* end of file */
