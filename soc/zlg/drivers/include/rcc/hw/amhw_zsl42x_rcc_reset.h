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
 * \brief
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-05  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_RCC_RESET_H
#define __AMHW_ZSL42x_RCC_RESET_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "zsl42x_periph_map.h"

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_zsl42x_rcc_if_reset
 * \copydoc amhw_zsl42x_rcc_reset.h
 * @{
 */

/**
 * \brief CRC寄存器块结构体
 */
typedef struct {
  __IO uint32_t flag;             /**< \brief 复位标识寄存器     0x4000201C*/
  __IO uint32_t reserve[2];       /**< \brief 保留位  0x40002020、0x40002024*/
  __IO uint32_t reset[2];         /**< \brief 内部高速时钟控制寄存器  0x40002028、0x4000202C*/
} amhw_zsl42x_rcc_reset_t;

#define ZSL42x_RCC_RESET  ((amhw_zsl42x_rcc_reset_t *)(0x4000201CUL))

/**
 * \brief 外设复位控制位枚举
 */
typedef enum {

	/* reset[0]控制的外设复位使能位  */
    AMHW_ZSL42x_RCC_RESET_DMA     = 29,
    AMHW_ZSL42x_RCC_RESET_GPIO    = 28,
    AMHW_ZSL42x_RCC_RESET_AES     = 27,
    AMHW_ZSL42x_RCC_RESET_CRC     = 26,
    AMHW_ZSL42x_RCC_RESET_SWD     = 25,
    AMHW_ZSL42x_RCC_RESET_SYSTICK = 24,
    AMHW_ZSL42x_RCC_RESET_LCD     = 22,
    AMHW_ZSL42x_RCC_RESET_CLKTRIM = 21,
    AMHW_ZSL42x_RCC_RESET_RTC     = 20,
    AMHW_ZSL42x_RCC_RESET_PCNT    = 19,
    AMHW_ZSL42x_RCC_RESET_RNG     = 18,
	AMHW_ZSL42x_RCC_RESET_VC      = 17,
	AMHW_ZSL42x_RCC_RESET_ADC     = 16,
	AMHW_ZSL42x_RCC_RESET_PCA     = 14,
	AMHW_ZSL42x_RCC_RESET_OPA     = 13,
	AMHW_ZSL42x_RCC_RESET_TIM3    = 11,
	AMHW_ZSL42x_RCC_RESET_ADVTIM  = 10,
    AMHW_ZSL42x_RCC_RESET_LPTIM0  = 9,
    AMHW_ZSL42x_RCC_RESET_TIM012  = 8,
    AMHW_ZSL42x_RCC_RESET_SPI1    = 7,
	AMHW_ZSL42x_RCC_RESET_SPI0    = 6,
	AMHW_ZSL42x_RCC_RESET_I2C1    = 5,
	AMHW_ZSL42x_RCC_RESET_I2C0    = 4,
	AMHW_ZSL42x_RCC_RESET_LPUART1 = 3,
    AMHW_ZSL42x_RCC_RESET_LPUART0 = 2,
    AMHW_ZSL42x_RCC_RESET_UART1   = 1,
    AMHW_ZSL42x_RCC_RESET_UART0   = 0,

	/* reset[1]控制的外设复位使能位  */
	AMHW_ZSL42x_RCC_RESET_UART3   = 9 + 31,
    AMHW_ZSL42x_RCC_RESET_UART2   = 8 + 31,
    AMHW_ZSL42x_RCC_RESET_LPTIM1  = 4 + 31,
    AMHW_ZSL42x_RCC_RESET_DAC     = 3 + 31,
}amhw_zsl42x_rcc_reset_peripheral;

/**
 * \brief 外设复位标志枚举
 */
typedef enum {
    AMHW_ZSL42x_RCC_RESET_FLAG_PORT   = 7,  /**< \brief RSTB端口复位标志 */
	AMHW_ZSL42x_RCC_RESET_FLAG_SOFT   = 6,  /**< \brief Cotrex-M0+CPU 软件复位标志 */
	AMHW_ZSL42x_RCC_RESET_FLAG_LOCKUP = 5,  /**< \brief Cotrex-M0+CPU Lockup复位标志 */
	AMHW_ZSL42x_RCC_RESET_FLAG_PCA    = 4,  /**< \brief PCA复位标志 */
	AMHW_ZSL42x_RCC_RESET_FLAG_WDT    = 3,  /**< \brief WDT复位标志 */
    AMHW_ZSL42x_RCC_RESET_FLAG_LVD    = 2,  /**< \brief LVD复位标志 */
    AMHW_ZSL42x_RCC_RESET_FLAG_POR15V = 1,  /**< \brief Vcore域复位标志 */
    AMHW_ZSL42x_RCC_RESET_FLAG_POR5V  = 0,  /**< \brief VCC电源域复位标志 */
}amhw_zsl42x_rcc_reset_flag_peripheral;

/**
 * \brief 外设复位标志有效性获取
 *
 * \param[in] flag : 复位标志     amhw_zsl42x_rcc_reset_flag_peripheral枚举成员
 *
 * return AM_TRUE :  有复位动作发生
 *        AM_FALSE:  无复位动作发生
 */
am_static_inline
am_bool_t amhw_zsl42x_rcc_reset_flag_check(amhw_zsl42x_rcc_reset_flag_peripheral flag)
{

    return (ZSL42x_RCC_RESET->flag & (0x1ul << flag)) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 外设复位标志有效性清除
 *
 * \param[in] flag : 复位标志     amhw_zsl42x_rcc_reset_flag_peripheral枚举成员
 *
 * return none
 */
am_static_inline
void amhw_zsl42x_rcc_reset_flag_clr(amhw_zsl42x_rcc_reset_flag_peripheral flag)
{
	ZSL42x_RCC_RESET->flag &= ~(0x1ul << flag);
}

/**
 * \brief 使某些外设复位
 *
 * \param[in]  peri ：需要复位的外设
 *
 * \return 无
 *
 */
void amhw_zsl42x_rcc_reset (amhw_zsl42x_rcc_reset_peripheral peri);


/**
 * \brief 使某些外设正常工作
 *
 * \param[in]  peri ：需要正常工作的外设
 *
 * \return 无
 *
 */
void amhw_zsl42x_rcc_reset_stop (amhw_zsl42x_rcc_reset_peripheral peri);

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __ZSL42x_RCC_RESET_H */

#endif

/* end of file */
