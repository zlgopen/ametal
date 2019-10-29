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
 * \brief ZLG118 中断号定义
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-03  zp, first implementation
 * \endinternal
 */

#ifndef __ZLG118_INUM_H
#define __ZLG118_INUM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \addtogroup zlg118_if_inum
 * \copydoc zlg118_inum.h
 * @{
 */

/**
 * \name ZLG118 CPU 中断号
 * @{
 */

/**
 * 中断号使用说明
 *
 * 某一外设的中断号独立、单一（如INUM_xx形式），在使用时无特别注意事项
 *
 * 某一外设的中断号不独立、多个外设使用同一中断（如INUM_xx_xx形式），在使用时需要注意，因为是多个外设，
 * 意味着相关控制寄存器的基地址不同，在中断连接、断开时，尽管多次调用，实际上仅一次有效。回调函数、中断服务
 * 函数和传入参数，可能生效的只有一组。
 *
 * 假如，该中断号的所有外设都使用中断，最直接的现象：
 *     1.在多个外设调用初始化函数，只有最后一个初始化的外设中断能正常运行。
 *     2.在多个外设调用解初始化函数，只要发生一个解初始化过程，即中断断开连接，其余所有外设全部解开中断。
 *
 *  考虑到用户使用情况的不确定性，仅在INUM_LPTIM0_1的中间层函数，使用多个设备结构体指针缓存+判断的方式解决了
 *  这一现象，可以参考am_zlg118_lptim_timing.c该解决方法，以解决其他中断号使用时的上述现象。
 *
 */
#define INUM_PORTA                 0    /**< \brief PORTA中断 */
#define INUM_PORTB                 1    /**< \brief PORTB中断 */
#define INUM_PORTC_E               2    /**< \brief PORTC_E中断 */
#define INUM_PORTD_F               3    /**< \brief PORTD_F中断 */
#define INUM_DMA                   4    /**< \brief DMA中断 */
#define INUM_TIM3                  5    /**< \brief TIM3中断 */
#define INUM_UART0_2               6    /**< \brief UART0_2中断 */
#define INUM_UART1_3               7    /**< \brief UART1_3中断 */
#define INUM_LPUART0               8    /**< \brief LPUART0中断 */
#define INUM_LPUART1               9    /**< \brief LPUART1中断 */
#define INUM_SPI0                 10    /**< \brief SPI0中断 */
#define INUM_SPI1                 11    /**< \brief SPI1中断 */
#define INUM_I2C0                 12    /**< \brief I2C0中断 */
#define INUM_I2C1                 13    /**< \brief I2C1中断 */
#define INUM_TIM0                 14    /**< \brief TIM0中断 */
#define INUM_TIM1                 15    /**< \brief TIM1中断 */
#define INUM_TIM2                 16    /**< \brief TIM2中断 */
#define INUM_LPTIM0_1             17    /**< \brief LPTIM0_1中断 */
#define INUM_TIM4                 18    /**< \brief TIM4中断 */
#define INUM_TIM5                 19    /**< \brief TIM5中断 */
#define INUM_TIM6                 20    /**< \brief TIM6中断 */
#define INUM_PCA                  21    /**< \brief PCA中断 */
#define INUM_WDT                  22    /**< \brief WDT中断 */
#define INUM_RTC                  23    /**< \brief RTC中断 */
#define INUM_ADC_DAC              24    /**< \brief ADC_DAC中断 */
#define INUM_PCNT                 25    /**< \brief PCNT中断 */
#define INUM_VC0                  26    /**< \brief VC0中断 */
#define INUM_VC1_VC2              27    /**< \brief VC12中断 */
#define INUM_LVD                  28    /**< \brief LVD中断 */
#define INUM_LCD                  29    /**< \brief LCD中断 */
#define INUM_FLASH_RAM            30    /**< \brief FLASH_RAM中断 */
#define INUM_CLKTRIM              31    /**< \brief CLKTRIM中断 */

/** @} */

/**
 * \brief 总中断数为：(INUM_CLKTRIM - INUM_PORTA + 1),
 *
 */
#define INUM_INTERNAL_COUNT     (INUM_CLKTRIM - INUM_PORTA + 1)

/**
 * \brief 最大中断号为： INUM_CLKTRIM
 */
#define INUM_INTERNAL_MAX        INUM_CLKTRIM

/** \brief 最小中断号: INUM_PORTA */
#define INUM_INTERNAL_MIN        INUM_PORTA


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
	PORTA_IRQn               = 0,    /**< \brief PORTA     Interrupt  */
	PORTB_IRQn               = 1,    /**< \brief PORTB     Interrupt  */
	PORTC_E_IRQn             = 2,    /**< \brief PORTC_E   Interrupt  */
	PORTD_F_IRQn             = 3,    /**< \brief PORTD_F   Interrupt  */
	DMAC_IRQn                = 4,    /**< \brief DMAC      Interrupt  */
	TIM3_IRQn                = 5,    /**< \brief TIM3      Interrupt  */
	UART0_2_IRQn             = 6,    /**< \brief UART0_2   Interrupt  */
	UART1_3_IRQn             = 7,    /**< \brief UART1_3   Interrupt  */
	LPUART0_IRQn             = 8,    /**< \brief LPUART0   Interrupt  */
	LPUART1_IRQn             = 9,    /**< \brief LPUART1   Interrupt  */
	SPI0_IRQn                = 10,   /**< \brief SPI0      Interrupt  */
	SPI1_IRQn                = 11,   /**< \brief SPI1      Interrupt  */
	I2C0_IRQn                = 12,   /**< \brief I2C0      Interrupt  */
	I2C1_IRQn                = 13,   /**< \brief I2C1      Interrupt  */
	TIM0_IRQn                = 14,   /**< \brief TIM0      Interrupt  */
	TIM1_IRQn                = 15,   /**< \brief TIM1      Interrupt  */
	TIM2_IRQn                = 16,   /**< \brief TIM2      Interrupt  */
	LPTIM0_1_IRQn            = 17,   /**< \brief LPTIM0_1  Interrupt  */
	TIM4_IRQn                = 18,   /**< \brief TIM4      Interrupt  */
	TIM5_IRQn                = 19,   /**< \brief TIM5      Interrupt  */
	TIM6_IRQn                = 20,   /**< \brief TIM6      Interrupt  */
	PCA_IRQn                 = 21,   /**< \brief PCA       Interrupt  */
	WDT_IRQn                 = 22,   /**< \brief WDT       Interrupt  */
	RTC_IRQn                 = 23,   /**< \brief RTC       Interrupt  */
	ADC_DAC_IRQn             = 24,   /**< \brief ADC_DAC   Interrupt  */
	PCNT_IRQn                = 25,   /**< \brief PCNT      Interrupt  */
	VC0_IRQn                 = 26,   /**< \brief VC0       Interrupt  */
	VC1_2_IRQn               = 27,   /**< \brief VC1       Interrupt  */
	LVD_IRQn                 = 28,   /**< \brief LVD       Interrupt  */
	LCD_IRQn                 = 29,   /**< \brief LCD       Interrupt  */
	FLASH_RAM_IRQn           = 30,   /**< \brief FLASH_RAM Interrupt  */
	CLKTRIM_IRQn             = 31,   /**< \brief CLKTRIM   Interrupt  */

}IRQn_Type;

/**
 * \endcond
 */



/**
 * \endcond
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __ZLG118_INUM_H */

/* end of file */
