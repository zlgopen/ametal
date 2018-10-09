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
 * \brief KL26 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */


#ifndef __KL26_INUM_H
#define __KL26_INUM_H

#ifdef __cplusplus
extern "C" {

#endif

/**
 * \addtogroup kl26_if_inum
 * \copydoc kl26_inum.h
 * @{
 */

/**
 * \name KL26 CPU 中断号
 * @{
 */

#define INUM_DMA_CH0             0     /**< \brief DMA通道0中断               */
#define INUM_DMA_CH1             1     /**< \brief DMA通道1中断               */
#define INUM_DMA_CH2             2     /**< \brief DMA通道2中断               */
#define INUM_DMA_CH3             3     /**< \brief DMA通道3中断               */

#define INUM_FTFA                5     /**< \brief FAFT中断             */
#define INUM_PMC                 6     /**< \brief PMC中断               */
#define INUM_LLWU                7     /**< \brief LLWU中断             */
#define INUM_I2C0                8     /**< \brief I2C0中断             */
#define INUM_I2C1                9     /**< \brief I2C1中断             */
#define INUM_SPI0                10    /**< \brief SPI0中断             */
#define INUM_SPI1                11    /**< \brief SPI1中断             */
#define INUM_UART0               12    /**< \brief UART0中断           */
#define INUM_UART1               13    /**< \brief UART1中断           */
#define INUM_UART2               14    /**< \brief UART2中断           */
#define INUM_ADC0                15    /**< \brief ADC0中断             */
#define INUM_CMP0                16    /**< \brief CMP0中断             */
#define INUM_TPM0                17    /**< \brief TPM0中断             */
#define INUM_TPM1                18    /**< \brief TPM1中断             */
#define INUM_TPM2                19    /**< \brief TPM2中断             */
#define INUM_RTC_ALARM           20    /**< \brief RTC 闹钟中断      */
#define INUM_RTC_SEC             21    /**< \brief RTC秒中断           */
#define INUM_PIT                 22    /**< \brief PIT中断               */
#define INUM_I2S                 23    /**< \brief I2S中断               */
#define INUM_USB0                24    /**< \brief USB中断               */
#define INUM_DAC0                25    /**< \brief DAC0中断             */
#define INUM_TSI0                26    /**< \brief TSI0中断             */
#define INUM_MCG                 27    /**< \brief MCG中断               */
#define INUM_LPTMR0              28    /**< \brief LPTMR0中断         */

#define INUM_PORTA               30    /**< \brief PORTA 引脚中断               */
#define INUM_PORTC_PORTD         31    /**< \brief PORTC或PORTD引脚中断    */
/** @} */




/**
 * \brief 总中断数为：(INUM_PORTC_PORTD - INUM_DMA_CH0 + 1),
 *
 */
#define INUM_INTERNAL_COUNT     (INUM_PORTC_PORTD - INUM_DMA_CH0 + 1)

/**
 * \brief 最大中断号为：INUM_PORTC_PORTD
 */
#define INUM_INTERNAL_MAX        INUM_PORTC_PORTD


/** \brief 最小中断号为：INUM_DMA_CH0 */
#define INUM_INTERNAL_MIN        INUM_DMA_CH0

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
    DMA_CH0_IRQn             = 0,    /**< \brief DMA_CH0 Interrupt */
    DMA_CH1_IRQn             = 1,    /**< \brief DMA_CH1 Interrupt */
    DMA_CH2_IRQn             = 2,    /**< \brief DMA_CH2 Interrupt */
    DMA_CH3_IRQn             = 3,    /**< \brief DMA_CH3 Interrupt */
    Reserved1_IRQn           = 4,    /**< \brief Reserved1 Interrupt */
    FTFA_IRQn                = 5,    /**< \brief FTFA Interrupt */
    PMC_IRQn                 = 6,    /**< \brief PMC Interrupt */
    LLWU_IRQn                = 7,    /**< \brief LLWU Interrupt */
    I2C0_IRQn                = 8,    /**< \brief I2C0 Interrupt */
    I2C1_IRQn                = 9,    /**< \brief I2C1 Interrupt */
    SPI0_IRQn                = 10,   /**< \brief SPI0 Interrupt */
    SPI1_IRQn                = 11,   /**< \brief SPI1 Interrupt */
    UART0_IRQn               = 12,   /**< \brief UART0 Interrupt */
    UART1_IRQn               = 13,   /**< \brief UART1 Interrupt */
    UART2_IRQn               = 14,   /**< \brief UART2 Interrupt */
    ADC0_IRQn                = 15,   /**< \brief ADC0 Interrupt  */
    CMP0_IRQn                = 16,   /**< \brief CMP0 Interrupt */
    TPM0_IRQn                = 17,   /**< \brief TPM0 Interrupt */
    TPM1IRQn                 = 18,   /**< \brief TPM1 Interrupt */
    TPM2_IRQn                = 19,   /**< \brief TPM2 Interrupt */
    RTC_ALARM_IRQn           = 20,   /**< \brief RTC_ALARM Interrupt */
    RTC_SEC_IRQn             = 21,   /**< \brief RTC_SEC Interrupt  */
    PIT_IRQn                 = 22,   /**< \brief PIT Interrupt */
    I2S_IRQn                 = 23,   /**< \brief I2S Interrupt */
	USB_IRQn                 = 24,   /**< \brief USB Interrupt */
    DAC0_IRQn                = 25,   /**< \brief DAC0 Interrupt */
    TSI0_IRQn                = 26,   /**< \brief TSI0 Interrupt */
    MCG_IRQn                 = 27,   /**< \brief MCG Interrupt */
    LPTMR0_IRQn              = 28,   /**< \brief LPTMR0 Interrupt */
    Reserved3_IRQn           = 29,   /**< \brief Reserved3 Interrupt */
    PORTA_IRQn               = 30,   /**< \brief PORTA Interrupt */
    PORTC_PORTD_IRQn         = 31,   /**< \brief PORTC_PORTD Interrupt */
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

#endif /* __KL26_INUM_H */

/* end of file */
