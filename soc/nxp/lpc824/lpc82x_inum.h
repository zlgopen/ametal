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
 * \brief LPC82X 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 15-06-21  jon, first implementation.
 * \endinternal
 */


#ifndef __LPC82X_INUM_H
#define __LPC82X_INUM_H

#ifdef __cplusplus
extern "C" {
    
#endif

#include "ametal.h"

/** 
 * \addtogroup lpc82x_if_inum
 * \copydoc lpc82x_inum.h
 * @{
 */

/**
 * \name LPC82x CPU 中断号
 * @{
 */
                                                                                            
#define INUM_SPI0                0    /**< \brief SPI0中断               */
#define INUM_SPI1                1    /**< \brief SPI1中断               */
#define INUM_USART0              3    /**< \brief USART0中断             */
#define INUM_USART1              4    /**< \brief USART1中断             */
#define INUM_USART2              5    /**< \brief USART2中断             */
#define INUM_I2C1                7    /**< \brief I2C1中断               */
#define INUM_I2C0                8    /**< \brief I2C0中断               */
#define INUM_SCT0                9    /**< \brief SCT中断                */
#define INUM_MRT                 10   /**< \brief Multi-rate 定时器中断  */
#define INUM_ACMP                11   /**< \brief 模拟比较器中断         */
#define INUM_WDT                 12   /**< \brief 看门狗中断             */
#define INUM_BOD                 13   /**< \brief BOD中断                */
#define INUM_FLASH               14   /**< \brief FLASH中断              */
#define INUM_WKT                 15   /**< \brief WKT中断                */
#define INUM_ADC0_SEQA           16   /**< \brief ADC0序列A中断          */
#define INUM_ADC0_SEQB           17   /**< \brief ADC0序列B中断          */
#define INUM_ADC0_THCMP          18   /**< \brief ADC0阈值比较和错误中断 */
#define INUM_ADC0_OVR            19   /**< \brief ADC0 overrun 中断      */
#define INUM_DMA                 20   /**< \brief DMA中断                */
#define INUM_I2C2                21   /**< \brief I2C2中断               */
#define INUM_I2C3                22   /**< \brief I2C3中断               */
#define INUM_PIN_INT0            24   /**< \brief 引脚中断0              */
#define INUM_PIN_INT1            25   /**< \brief 引脚中断1              */
#define INUM_PIN_INT2            26   /**< \brief 引脚中断2              */
#define INUM_PIN_INT3            27   /**< \brief 引脚中断3              */
#define INUM_PIN_INT4            28   /**< \brief 引脚中断4              */
#define INUM_PIN_INT5            29   /**< \brief 引脚中断5              */
#define INUM_PIN_INT6            30   /**< \brief 引脚中断6              */
#define INUM_PIN_INT7            31   /**< \brief 引脚中断7              */

/** @} */




/**
 * \brief 总中断数为：(INUM_PIN_INT7 - INUM_SPI0 + 1),
 * 
 */
#define INUM_INTERNAL_COUNT     (INUM_PIN_INT7 - INUM_SPI0 + 1)

/** 
 * \brief 最大中断号为：INUM_PIN_INT7
 */
#define INUM_INTERNAL_MAX        INUM_PIN_INT7


/** \brief 最小中断号: INUM_SPI0 */
#define INUM_INTERNAL_MIN        INUM_SPI0


/**
 * \cond
 * \brief 中断号定义，为core_cm0plus.h文件保留
 */
typedef enum {                                                         
    /** \brief  1  复位向量                                                */
    Reset_IRQn               = -15,                                    
                                                                       
    /** \brief  2  不可屏蔽中断, 不能被停止或抢占                          */
    NonMaskableInt_IRQn      = -14,                                    
                                                                       
    /** \brief  3  硬件错误中断                                            */
    HardFault_IRQn           = -13,                                    
                                                                       
    /** \brief 11  系统服务调用通过SVC指令                                 */
    SVCall_IRQn              =  -5,                                    
                                                                       
    /** \brief 14  系统的挂起请求                                          */
    PendSV_IRQn              =  -2,                                    
                                                                       
    /** \brief 15  系统滴答定时器                                          */
    SysTick_IRQn             =  -1,   
   
    /* ---------------  外设中断 ------------------------------------ */
    SPI0_IRQn                = 0,    /**< \brief SPI0     Interrupt        */
    SPI1_IRQn                = 1,    /**< \brief SPI1     Interrupt        */
    Reserved0_IRQn           = 2,    /**< \brief Reserved Interrupt        */
    UART0_IRQn               = 3,    /**< \brief USART0   Interrupt        */
    UART1_IRQn               = 4,    /**< \brief USART1   Interrupt        */
    UART2_IRQn               = 5,    /**< \brief USART2   Interrupt        */
    Reserved1_IRQn           = 6,    /**< \brief Reserved Interrupt        */
    I2C1_IRQn                = 7,    /**< \brief I2C1     Interrupt        */
    I2C0_IRQn                = 8,    /**< \brief I2C0     Interrupt        */
    SCT_IRQn                 = 9,    /**< \brief SCT      Interrupt        */
    MRT_IRQn                 = 10,   /**< \brief MRT      Interrupt        */
    CMP_IRQn                 = 11,   /**< \brief CMP      Interrupt        */
    WDT_IRQn                 = 12,   /**< \brief WDT      Interrupt        */
    BOD_IRQn                 = 13,   /**< \brief BOD      Interrupt        */
    FLASH_IRQn               = 14,   /**< \brief Flash    Interrupt        */
    WKT_IRQn                 = 15,   /**< \brief WKT      Interrupt        */
    ADC_SEQA_IRQn            = 16,   /**< \brief ADC sequence A completion */
    ADC_SEQB_IRQn            = 17,   /**< \brief ADC sequence B completion */
    ADC_THCMP_IRQn           = 18,   /**< \brief ADC threshold  compare    */
    ADC_OVR_IRQn             = 19,   /**< \brief ADC overrun               */
    DMA_IRQn                 = 20,   /**< \brief DMA      Interrupt        */
    I2C2_IRQn                = 21,   /**< \brief I2C0     Interrupt        */
    I2C3_IRQn                = 22,   /**< \brief I2C3     Interrupt        */
    Reserved2_IRQn           = 23,   /**< \brief Reserved Interrupt        */
    PIN_INT0_IRQn            = 24,   /**< \brief External Interrupt 0      */
    PIN_INT1_IRQn            = 25,   /**< \brief External Interrupt 1      */
    PIN_INT2_IRQn            = 26,   /**< \brief External Interrupt 2      */
    PIN_INT3_IRQn            = 27,   /**< \brief External Interrupt 3      */
    PIN_INT4_IRQn            = 28,   /**< \brief External Interrupt 4      */
    PIN_INT5_IRQn            = 29,   /**< \brief External Interrupt 5      */
    PIN_INT6_IRQn            = 30,   /**< \brief External Interrupt 6      */
    PIN_INT7_IRQn            = 31,   /**< \brief External Interrupt 7      */
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

#endif /* __LPC82X_INUM_H */

/* end of file */
