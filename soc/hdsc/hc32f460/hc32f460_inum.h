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
 * \brief HC32F460 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 20-01-13  cds, first implementation
 * \endinternal
 */

#ifndef __ZLG217_INUM_H
#define __ZLG217_INUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \addtogroup zlg217_if_inum
 * \copydoc zlg217_inum.h
 * @{
 */

/**
 * \name ZLG217 CPU 中断号
 * @{
 */

#define INUM_PORT_EIRQ0      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ1      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ2      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ3      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ4      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ5      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ6      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ7      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ8      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ9      128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ10     128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ11     128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ12     128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ13     128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ14     128    /**< \brief 外部中断 */
#define INUM_PORT_EIRQ15     128    /**< \brief 外部中断 */
#define INUM_DMA1            129    /**< \brief DMA1中断 */
#define INUM_DMA2            129    /**< \brief DMA2中断 */
#define INUM_EFM             129    /**< \brief EFM中断 */
#define INUM_USBFS_SOF       129    /**< \brief INUM_SOF中断 */
#define INUM_QSPI            129    /**< \brief INUM_QSPI中断 */
#define INUM_DCU             129    /**< \brief INUM_DCU中断 */
#define INUM_TIMER0_1        130    /**< \brief TIMER0_1中断 */
#define INUM_TIMER0_2        130    /**< \brief TIMER0_2中断 */
#define INUM_RTC             130    /**< \brief RTC中断 */
#define INUM_XTAL32          130    /**< \brief XTAL32中断 */
#define INUM_XTAL            130    /**< \brief XTAL中断 */
#define INUM_WKTM            130    /**< \brief WKTM中断 */
#define INUM_SWDT            130    /**< \brief SWDT中断 */
#define INUM_TIMER6_1        131    /**< \brief TIMER6_1中断 */
#define INUM_TIMER6_2        131    /**< \brief TIMER6_2中断 */
#define INUM_TIMER6_3        132    /**< \brief TIMER6_3中断 */
#define INUM_TIMERA_1        136    /**< \brief TIMERA_1中断 */
#define INUM_TIMERA_2        136    /**< \brief TIMERA_2中断 */
#define INUM_TIMERA_3        136    /**< \brief TIMERA_3中断 */
#define INUM_TIMERA_4        136    /**< \brief TIMERA_4中断 */
#define INUM_TIMERA_5        136    /**< \brief TIMERA_5中断 */
#define INUM_TIMERA_6        136    /**< \brief TIMERA_6中断 */
#define INUM_USBFS_GLB       136    /**< \brief USBFS_GLB中断 */
#define INUM_USART1          136    /**< \brief USART1中断 */
#define INUM_USART2          136    /**< \brief USART2中断 */
#define INUM_USART3          137    /**< \brief USART3中断 */
#define INUM_USART4          137    /**< \brief USART4中断 */
#define INUM_SPI1            137    /**< \brief SPI1中断 */
#define INUM_SPI2            137    /**< \brief SPI2中断 */
#define INUM_SPI3            137    /**< \brief SPI3中断 */
#define INUM_SPI4            137    /**< \brief SPI4中断 */
#define INUM_AOS_STRG        137    /**< \brief AOS_STRG中断 */
#define INUM_TIMER4_1        138    /**< \brief TIMER4_1中断 */
#define INUM_TIMER4_2        138    /**< \brief TIMER4_2中断 */
#define INUM_TIMER4_3        139    /**< \brief TIMER4_3中断 */
#define INUM_TIMER4_1EVT     139    /**< \brief TIMER4_1EVT中断 */
#define INUM_TIMER4_2EVT     139    /**< \brief TIMER4_2EVT中断 */
#define INUM_TIMER4_3EVT     140    /**< \brief TIMER4_2EVT中断 */
#define INUM_TIMER4_EMB      140    /**< \brief TIMER4_EMB中断 */
#define INUM_EVENT_PORT1     0      /**< \brief EVENT_PORT1中断 */
#define INUM_EVENT_PORT2     1      /**< \brief EVENT_PORT2中断 */
#define INUM_EVENT_PORT3     2      /**< \brief EVENT_PORT3中断 */
#define INUM_EVENT_PORT4     3      /**< \brief EVENT_PORT4中断 */
#define INUM_I2S1            140    /**< \brief I2S1中断 */
#define INUM_I2S2            140    /**< \brief I2S2中断 */
#define INUM_I2S3            140    /**< \brief I2S3中断 */
#define INUM_I2S4            140    /**< \brief I2S4中断 */
#define INUM_ACMP1           4      /**< \brief ACMP1中断 */
#define INUM_ACMP2           5      /**< \brief ACMP2中断 */
#define INUM_ACMP3           6      /**< \brief ACMP3中断 */
#define INUM_USART1_WUPI     7      /**< \brief USART1_WUPI中断 */
#define INUM_PVD             141    /**< \brief PVD中断 */
#define INUM_OTS             8      /**< \brief OTS中断 */
#define INUM_FCM             141    /**< \brief FCM中断 */
#define INUM_WDT             141    /**< \brief WDT中断 */
#define INUM_I2C1            141    /**< \brief I2C1中断 */
#define INUM_I2C2            141    /**< \brief I2C2中断 */
#define INUM_I2C3            141    /**< \brief I2C3中断 */
#define INUM_ADC1            142    /**< \brief ADC1中断 */
#define INUM_ADC2            142    /**< \brief ADC2中断 */
#define INUM_TRNG            142    /**< \brief TRNG中断 */
#define INUM_SDIOC1          143    /**< \brief SDIOC1中断 */
#define INUM_SDIOC2          143    /**< \brief SDIOC2中断 */
#define INUM_CAN             143    /**< \brief CAN中断 */


/** @} */

/**
 * \brief 总中断数为：(INUM_USB - INUM_WWDT + 1),
 *
 */
#define INUM_INTERNAL_COUNT     (144)

/**
 * \brief 最大中断号为： INUM_USB
 */
#define INUM_INTERNAL_MAX        INUM_CAN

/** \brief 最小中断号: INUM_WWDT */
#define INUM_INTERNAL_MIN        0

/**
 * \cond
 * 中断号定义，为 core_cm4.h文件保留
 */
typedef enum {

    /* ----------------------  Cortex-M4核异常号  ------------------- */

    /** \brief 2  不可屏蔽中断, 不能被停止或抢占 */
    NonMaskableInt_IRQn      = -14,

    /** \brief 3  硬件错误中断 */
    HardFault_IRQn           = -13,

    /** \brief 4  内存管理中断 */
    MemManage                = -12,

    /** \brief 5  总线错误中断 */
    BusFault                 = -11,

    /** \brief 6  用户错误中断 */
    UsageFault               = -10,

    /** \brief 11  系统服务调用通过SVC指令 */
    SVCall_IRQn              =  -5,

    /** \brief 14  系统的挂起请求 */
    PendSV_IRQn              =  -2,

    /** \brief 15  系统滴答定时器 */
    SysTick_IRQn             =  -1,

    /******  ZLG217 Specific Interrupt Numbers *******************************************************/
//    WWDG_IRQn                 = 0,         /**< \brief 窗口定时器中断 */
//    PVD_IRQn                  =1,         /**< \brief 电源电压检测中断 */
//    TAMPER_IRQn               =2,         /**< \brief 侵入检测中断 */
//    RTC_IRQn                  =3,         /**< \brief 实时时钟（RTC）全局中断 */
//    FLASH_IRQn                =4,         /**< \brief 闪存全局中断 */
//    RCC_IRQn                  =5,         /**< \brief 复位和时钟控制（RCC）中断 */
//    EXTI0_IRQn                =6,         /**< \brief EXTI线0中断 */
//    EXTI1_IRQn                =7,         /**< \brief EXTI线1中断 */
//    EXTI2_IRQn                =8,         /**< \brief EXTI线2中断 */
//    EXTI3_IRQn                =9,         /**< \brief EXTI线3中断 */
//    EXTI4_IRQn                =10,        /**< \brief EXTI线4中断 */
//    DMA1_1_IRQn               =11,        /**< \brief DMA1通道1全局中断 */
//    DMA1_2_IRQn               =12,        /**< \brief DMA1通道2全局中断 */
//    DMA1_3_IRQn               =13,        /**< \brief DMA1通道3全局中断 */
//    DMA1_4_IRQn               =14,        /**< \brief DMA1通道4全局中断 */
//    DMA1_5_IRQn               =15,        /**< \brief DMA1通道5全局中断 */
//    DMA1_6_IRQn               =16,        /**< \brief DMA1通道6全局中断 */
//    DMA1_7_IRQn               =17,        /**< \brief DMA1通道7全局中断 */
//    ADC1_2_IRQn               =18,        /**< \brief ADC1和ADC2的全局中断 */
//    USB_IRQn                  =19,        /**< \brief USB中断 */
//    Reserved0                 =20,        /**< \brief RESERVED */
//    CAN_RX1_IRQn              =21,        /**< \brief CAN接收1中断 */
//    Reserved1                 =22,        /**< \brief RESERVED */
//    EXTI9_5_IRQn              =23,        /**< \brief EXTI线[9：5]中断 */
//    TIM1_BRK_IRQn             =24,        /**< \brief TIM1断开中断 */
//    TIM1_UP_IRQn              =25,        /**< \brief TIM1更新中断 */
//    TIM1_TRG_COM_IRQn         =26,        /**< \brief TIM1触发和通信中断 */
//    TIM1_CC_IRQn              =27,        /**< \brief TIM1捕获比较中断 */
//    TIM2_IRQn                 =28,        /**< \brief TIM2全局中断 */
//    TIM3_IRQn                 =29,        /**< \brief TIM3全局中断 */
//    TIM4_IRQn                 =30,        /**< \brief TIM14全局中断 */
//    I2C1_EV_IRQn              =31,        /**< \brief I2C1事件中断 */
//    Reserved2                 =32,        /**< \brief RESERVED */
//    I2C2_EV_IRQn              =33,        /**< \brief I2C1事件中断 */
//    Reserved3                 =34,        /**< \brief RESERVED */
//    SPI1_IRQn                 =35,        /**< \brief SPI1全局中断 */
//    SPI2_IRQn                 =36,        /**< \brief SPI2全局中断 */
//    UART1_IRQn                =37,        /**< \brief UART1全局中断 */
//    UART2_IRQn                =38,        /**< \brief UART2全局中断 */
//    UART3_IRQn                =39,        /**< \brief UART3全局中断 */
//    EXTI15_10_IRQn            =40,        /**< \brief EXTI线[15：10]中断 */
//    RTCAlarm_IRQn             =41,        /**< \brief 连到EXTI17的RTC闹钟中断 */
//    USB_WK_IRQn               =42,        /**< \brief 连到EXTI18的从USB待机唤醒中断 */
//    Reserved4                 =43,        /**< \brief RESERVED */
//    Reserved5                 =44,        /**< \brief RESERVED */
//    AES_IRQn                  =45,        /**< \brief AES全局中断 */
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

#endif /* __ZLG217_INUM_H */

/* end of file */
