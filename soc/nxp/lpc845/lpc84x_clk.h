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
 * \brief LPC82x 时钟ID定义
 * 
 * \internal
 * \par Modification History
 * - 1.00 15-12-01  lnk, first implementation.
 * \endinternal
 */
 
#ifndef __LPC84X_CLK_H
#define __LPC84X_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup lpc82x_if_clockid
 * \copydoc lpc82x_clk.h
 * @{
 */
 
 
/**
 * \name 系统或外设的时钟ID
 * \anchor grp_clk_id
 * @{
 */
 
#define CLK_FRO        1            /**< \brief 内部FRO时钟          */
#define CLK_XTAL       2            /**< \brief 外部晶振时钟         */
#define CLK_CLKIN      3            /**< \brief 外设引脚输入时钟     */
#define CLK_WDTOSC     4            /**< \brief 看门狗振荡器         */
#define CLK_PLLIN      5            /**< \brief PLL输入时钟          */
#define CLK_PLLOUT     6            /**< \brief PLL输出时钟          */
#define CLK_MAIN       7            /**< \brief 主时钟               */
#define CLK_SYSTEM     8            /**< \brief 系统时钟             */

#define CLK_ROM         (1 << 8)     /**< \brief ROM 时钟             */
#define CLK_SRAM0_1     (2 << 8)     /**< \brief SRAM0 SRAM1 时钟     */
#define CLK_FLASH       (4 << 8)     /**< \brief FLASH 时钟           */
#define CLK_I2C0        (5 << 8)     /**< \brief I2C0时钟             */
#define CLK_GPIO        (6 << 8)     /**< \brief GPIO 时钟            */
#define CLK_SWM         (7 << 8)     /**< \brief SWM 时钟             */
#define CLK_SCT         (8 << 8)     /**< \brief SCT 时钟             */
#define CLK_WKT         (9 << 8)     /**< \brief WKT 时钟             */
#define CLK_MRT         (10 << 8)    /**< \brief MRT时钟              */
#define CLK_SPI0        (11 << 8)    /**< \brief SPI0 时钟            */
#define CLK_SPI1        (12 << 8)    /**< \brief SPI1 时钟            */
#define CLK_CRC         (13 << 8)    /**< \brief CRC 时钟             */
#define CLK_UART0       (14 << 8)    /**< \brief UART0 时钟           */
#define CLK_UART1       (15 << 8)    /**< \brief UART1 时钟           */
#define CLK_UART2       (16 << 8)    /**< \brief UART2 时钟           */
#define CLK_WWDT        (17 << 8)    /**< \brief WWDT 时钟            */
#define CLK_IOCON       (18 << 8)    /**< \brief IOCON 时钟           */
#define CLK_ACMP        (19 << 8)    /**< \brief ACMP 时钟            */
#define CLK_GPIO1       (20 << 8)    /**< \brief GPIO1 时钟            */
#define CLK_I2C1        (21 << 8)    /**< \brief I2C1 时钟            */
#define CLK_I2C2        (22 << 8)    /**< \brief I2C2 时钟            */
#define CLK_I2C3        (23 << 8)    /**< \brief I2C3 时钟            */
#define CLK_ADC         (24 << 8)    /**< \brief ADC 时钟             */
#define CLK_CTMER0      (25 << 8)    /**< \brief CTMER0 时钟             */
#define CLK_MTB         (26 << 8)    /**< \brief MTB 时钟             */
#define CLK_DAC0        (27 << 8)    /**< \brief DMA 时钟             */
#define CLK_GPIO_INT    (28 << 8)    /**< \brief GPIO_INT 时钟          */
#define CLK_DMA         (29 << 8)    /**< \brief DMA 时钟             */
#define CLK_UART3       (30 << 8)    /**< \brief UART3 时钟        */
#define CLK_UART4       (31 << 8)    /**< \brief UART4 时钟        */
#define CLK_CAPT        (32 << 8)    /**< \brief CAPT 时钟           */
#define CLK_DAC1        (33 << 8)    /**< \brief DMA 时钟              */


/**< \brief 外设选择的时钟源    */
#define DEV_FRO         0    /**< \brief DMA 时钟             */
#define DEV_MAIN        1    /**< \brief DMA 时钟             */
#define DEV_FRG0        2    /**< \brief DMA 时钟             */
#define DEV_FRG1        3    /**< \brief DMA 时钟             */
#define DEV_FRO_DIV     4    /**< \brief DMA 时钟             */

/**< \brief FRG的时钟源    */
#define FRG_FRO             0    /**< \brief DMA 时钟             */
#define FRG_MAIN_CLK        1    /**< \brief DMA 时钟             */
#define FRG_SYS_PLL_CLK     2    /**< \brief DMA 时钟             */
#define FRO_DIV             3    /**< \brief DMA 时钟             */

#define CLK_PERIPH_MIN  CLK_ROM      /**< \brief 外设时钟最小ID值     */
#define CLK_PERIPH_MAX  CLK_DMA      /**< \brief 外设时钟最大ID值     */
/**
 * @} 
 */ 
 
/**
 * @} lpc82x_if_clockid
 */

#ifdef __cplusplus
}
#endif

#endif /* __LPC82X_CLK_H */

/* end of file */
