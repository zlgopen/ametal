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
 * \brief SWM (开关矩阵) 硬件操作接口
 * 
 * 1. 配置引脚的复用功能；
 *  - 特殊功能(模拟功能、调试功能等)；
 *  - GPIO功能(普通IO功能)；
 *  - 可转移功能(外设的对外引脚功能)；
 * 3. 取消了引脚特殊功能后，默认为GPIO功能；
 * 4. 取消了引脚特殊功能后，如果配置为可转移功能，则GPIO功能不可用，
 *    取消引脚所有可转移功能后，变为GPIO功能。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  mem, modified.
 * - 1.01 15-07-16  zxl, modified.
 * - 1.00 15-07-07  zxl, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_SWM_H
#define __AMHW_LPC82X_SWM_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup amhw_lpc82x_if_swm
 * \copydoc amhw_lpc82x_swm.h
 * @{
 */

/**
 * \brief LPC82x SWM 寄存器块结构体
 */
typedef struct amhw_lpc82x_swm {
    __IO uint32_t pinassign[12];   /**< \brief 功能引脚配置寄存器     */
    __I  uint32_t reserved[100];   /**< \brief 保留寄存器             */
    __IO uint32_t pinenable;       /**< \brief 特殊功能使能位         */
} amhw_lpc82x_swm_t;

/**
 * \brief LPC8XX SWM 可转移功能编号
 */
typedef enum amhw_lpc82x_swm_moved_func {

    /* SWM_UART0 功能编号 */
    AMHW_LPC82X_SWM_U0_TXD_O = 0,    /**< \brief UART0 TX   */
    AMHW_LPC82X_SWM_U0_RXD_I,        /**< \brief UART0 RX   */
    AMHW_LPC82X_SWM_U0_RTS_O,        /**< \brief UART0 RTS  */
    AMHW_LPC82X_SWM_U0_CTS_I,        /**< \brief UART0 CTS  */
    AMHW_LPC82X_SWM_U0_SCLK_IO,      /**< \brief UART0 SCLK */
                                     
    /* SWM_UART1 功能编号 */         
    AMHW_LPC82X_SWM_U1_TXD_O,        /**< \brief UART1 TX   */
    AMHW_LPC82X_SWM_U1_RXD_I,        /**< \brief UART1 RX   */
    AMHW_LPC82X_SWM_U1_RTS_O,        /**< \brief UART1 RTS  */
    AMHW_LPC82X_SWM_U1_CTS_I,        /**< \brief UART1 CTS  */
    AMHW_LPC82X_SWM_U1_SCLK_IO,      /**< \brief UART1 SCLK */
                                     
    /* SWM_UART2 功能编号 */         
    AMHW_LPC82X_SWM_U2_TXD_O,        /**< \brief UART2 TX   */
    AMHW_LPC82X_SWM_U2_RXD_I,        /**< \brief UART2 RX   */
    AMHW_LPC82X_SWM_U2_RTS_O,        /**< \brief UART2 RTS  */
    AMHW_LPC82X_SWM_U2_CTS_I,        /**< \brief UART2 CTS  */
    AMHW_LPC82X_SWM_U2_SCLK_IO,      /**< \brief UART2 SCLK */
                                     
                                     
    /* SWM_SPI0 功能编号 */          
    AMHW_LPC82X_SWM_SPI0_SCK_IO,     /**< \brief SPI0 SCK   */
    AMHW_LPC82X_SWM_SPI0_MOSI_IO,    /**< \brief SPI0 MOSI  */
    AMHW_LPC82X_SWM_SPI0_MISO_IO,    /**< \brief SPI0 MISO  */
    AMHW_LPC82X_SWM_SPI0_SSEL0_IO,   /**< \brief SPI0 SSEL0 */
    AMHW_LPC82X_SWM_SPI0_SSEL1_IO,   /**< \brief SPI0 SSEL1 */
    AMHW_LPC82X_SWM_SPI0_SSEL2_IO,   /**< \brief SPI0 SSEL2 */
    AMHW_LPC82X_SWM_SPI0_SSEL3_IO,   /**< \brief SPI0 SSEL3 */
                                     
                                     
                                     
    /* SWM_SPI1 功能编号 */          
                                     
    AMHW_LPC82X_SWM_SPI1_SCK_IO,     /**< \brief SPI1 SCK   */
    AMHW_LPC82X_SWM_SPI1_MOSI_IO,    /**< \brief SPI1 MOSI  */
    AMHW_LPC82X_SWM_SPI1_MISO_IO,    /**< \brief SPI1 MISO  */
    AMHW_LPC82X_SWM_SPI1_SSEL0_IO,   /**< \brief SPI1 SSEL0 */
    AMHW_LPC82X_SWM_SPI1_SSEL1_IO,   /**< \brief SPI1 SSEL1 */
                                     
                                     
    /* SWM_SCT_PIN 功能编号 */       
                                     
    AMHW_LPC82X_SWM_SCT_PIN0_I,      /**< \brief SCT PIN0   */
    AMHW_LPC82X_SWM_SCT_PIN1_I,      /**< \brief SCT PIN1   */
    AMHW_LPC82X_SWM_SCT_PIN2_I,      /**< \brief SCT PIN2   */
    AMHW_LPC82X_SWM_SCT_PIN3_I,      /**< \brief SCT PIN3   */
                                                            
    /* SWM_SCT_OUT 功能编号 */                              
    AMHW_LPC82X_SWM_SCT_OUT0_O,      /**< \brief SCT OUT0   */
    AMHW_LPC82X_SWM_SCT_OUT1_O,      /**< \brief SCT OUT1   */
    AMHW_LPC82X_SWM_SCT_OUT2_O,      /**< \brief SCT OUT2   */
    AMHW_LPC82X_SWM_SCT_OUT3_O,      /**< \brief SCT OUT3   */
    AMHW_LPC82X_SWM_SCT_OUT4_O,      /**< \brief SCT OUT4   */
    AMHW_LPC82X_SWM_SCT_OUT5_O,      /**< \brief SCT OUT5   */
                                                            
    /* SWM_I2C1 功能编号 */                                 
    AMHW_LPC82X_SWM_I2C1_SDA_IO,     /**< \brief I2C1 SDA   */
    AMHW_LPC82X_SWM_I2C1_SCL_IO,     /**< \brief I2C1 SCL   */
                                                            
    /* SWM_I2C2 功能编号 */                                 
    AMHW_LPC82X_SWM_I2C2_SDA_IO,     /**< \brief I2C2 SDA   */
    AMHW_LPC82X_SWM_I2C2_SCL_IO,     /**< \brief I2C2 SCL   */
                                                            
    /* SWM_I2C3 功能编号 */                                 
    AMHW_LPC82X_SWM_I2C3_SDA_IO,     /**< \brief I2C3 SDA   */
    AMHW_LPC82X_SWM_I2C3_SCL_IO,     /**< \brief I2C3 SCL   */
                                     
                                     
    /* SWM_ADC 功能编号 */           
    AMHW_LPC82X_SWM_ADC_PINTRIG0_I,  /**< \brief ADC 引脚触发0     */
    AMHW_LPC82X_SWM_ADC_PINTRIG1_I,  /**< \brief ADC 引脚触发1     */
                                                                   
    /* SWM_ACMP 功能编号 */                                        
    AMHW_LPC82X_SWM_ACMP_O_O,        /**< \brief SWM_ACMP          */
                                                                   
    /* SWM_CLKOUT 功能编号 */                                      
    AMHW_LPC82X_SWM_CLKOUT_O,        /**< \brief SWM_CLKOUT        */
                                     
                                     
    /* SWM_GPIO_INT_BMAT 功能编号 */ 
    AMHW_LPC82X_SWM_GPIO_INT_BMAT_O, /**< \brief SWM_GPIO_INT_BMAT */
    
} amhw_lpc82x_swm_moved_func_t;

/**
 * \brief LPC8XX 引脚特殊功能号
 *
 * \note 要使用GPIO功能或者可转移功能，必须先取消引脚的特殊功能
 */
typedef enum {
    AMHW_LPC82X_SWM_PIO0_0_ACMP_I1 = 0,    /**< \brief PIO0_0  ACMP_I1  */
    AMHW_LPC82X_SWM_PIO0_1_ACMP_I2,        /**< \brief PIO0_1  ACMP_I2  */
    AMHW_LPC82X_SWM_PIO0_14_ACMP_I3,       /**< \brief PIO0_14 ACMP_I3  */
    AMHW_LPC82X_SWM_PIO0_23_ACMP_I4,       /**< \brief PIO0_23 ACMP_I4  */
    AMHW_LPC82X_SWM_PIO0_3_SWCLK,          /**< \brief PIO0_3  SWCLK    */
    AMHW_LPC82X_SWM_PIO0_2_SWDIO,          /**< \brief PIO0_2  SWDIO    */
    AMHW_LPC82X_SWM_PIO0_8_XTALIN,         /**< \brief PIO0_8  XTALIN   */
    AMHW_LPC82X_SWM_PIO0_9_XTALOUT,        /**< \brief PIO0_9  XTALOUT  */
    AMHW_LPC82X_SWM_PIO0_5_RESETN,         /**< \brief PIO0_5  RESETN   */
    AMHW_LPC82X_SWM_PIO0_1_CLKIN,          /**< \brief PIO0_1  CLKIN    */
    AMHW_LPC82X_SWM_PIO0_6_VDDCMP,         /**< \brief PIO0_6  VDDCMP   */
    AMHW_LPC82X_SWM_PIO0_11_I2C0_SDA,      /**< \brief PIO0_11 I2C0_SDA */
    AMHW_LPC82X_SWM_PIO0_10_I2C0_SCL,      /**< \brief PIO0_10 I2C0_SCL */
    AMHW_LPC82X_SWM_PIO0_7_ADC_0,          /**< \brief PIO0_7  ADC_0    */
    AMHW_LPC82X_SWM_PIO0_6_ADC_1,          /**< \brief PIO0_6  ADC_1    */
    AMHW_LPC82X_SWM_PIO0_14_ADC_2,         /**< \brief PIO0_14 ADC_2    */
    AMHW_LPC82X_SWM_PIO0_23_ADC_3,         /**< \brief PIO0_23 ADC_3    */
    AMHW_LPC82X_SWM_PIO0_22_ADC_4,         /**< \brief PIO0_22 ADC_4    */
    AMHW_LPC82X_SWM_PIO0_21_ADC_5,         /**< \brief PIO0_21 ADC_5    */
    AMHW_LPC82X_SWM_PIO0_20_ADC_6,         /**< \brief PIO0_20 ADC_6    */
    AMHW_LPC82X_SWM_PIO0_19_ADC_7,         /**< \brief PIO0_19 ADC_7    */
    AMHW_LPC82X_SWM_PIO0_18_ADC_8,         /**< \brief PIO0_18 ADC_8    */
    AMHW_LPC82X_SWM_PIO0_17_ADC_9,         /**< \brief PIO0_17 ADC_9    */
    AMHW_LPC82X_SWM_PIO0_13_ADC_10,        /**< \brief PIO0_13 ADC_10   */
    AMHW_LPC82X_SWM_PIO0_4_ADC_11,         /**< \brief PIO0_4  ADC_11   */
} amhw_lpc82x_swm_fixed_func_t;

/**
 * \brief SWM 配置可转移功能的引脚(配置外设引脚)
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 * \param[in] func     : 可转移功能编号(#AMHW_LPC82X_SWM_U0_TXD_O)
 * \param[in] pin      : 引脚编号，值为 PIO* (#PIO0_0) \n
 *                       pin = 0xFF ：断开可转移功能与引脚的连接
 *
 * \return 无
 *
 * \note 分配引脚可转移功能前，如果引脚有特殊功能，应先取消引脚的特殊功能 \n
         #amhw_lpc82x_swm_pin_fixed_func_disable() 取消引脚的特殊功能
 */
am_static_inline  
void amhw_lpc82x_swm_moved_func_cfg (amhw_lpc82x_swm_t            *p_hw_swm,
                                     amhw_lpc82x_swm_moved_func_t  func,
                                     int                           pin)
{
    uint32_t reg;
    uint8_t  shift;
    
    shift = (func & 3) << 3;                    /* (func % 4) * 8 */
    reg   = p_hw_swm->pinassign[func >> 2];     /* (func / 4)     */

    AM_BIT_CLR_MASK(reg, AM_SBF(0xFF, shift));
    AM_BIT_SET_MASK(reg, AM_SBF(pin, shift));

    p_hw_swm->pinassign[func >> 2] = reg;
}

/**
 * \brief SWM模块复位
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_swm_reset (amhw_lpc82x_swm_t *p_hw_swm)
{
    uint8_t i = 0;
    
    for (i = 0; i < 12; i++) {
        p_hw_swm->pinassign[i] = 0xFFFFFFFF;
    }
    p_hw_swm->pinenable        = 0xFFFFECF;
}

/**
 * \brief 使能特殊功能
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 * \param[in] func     : 特殊功能编号
 *
 * \return 无
 */
am_static_inline  
void amhw_lpc82x_swm_fixed_func_enable (amhw_lpc82x_swm_t            *p_hw_swm,
                                        amhw_lpc82x_swm_fixed_func_t  func)
{
    AM_BIT_CLR_MASK(p_hw_swm->pinenable, AM_BIT(func));
}

/**
 * \brief 取消特殊功能
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 * \param[in] func     : 特殊功能编号
 *
 * \return 无
 */
am_static_inline 
void amhw_lpc82x_swm_fixed_func_disable (amhw_lpc82x_swm_t           *p_hw_swm,
                                         amhw_lpc82x_swm_fixed_func_t func)
{
    AM_BIT_SET_MASK(p_hw_swm->pinenable, AM_BIT(func));
}

/**
 * \brief 取消引脚的所有特殊功能
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 * \param[in] pin      : 引脚编号，值为 PIO* (#PIO0_0)
 *
 * \return 无
 *
 * \note 如果引脚没有特殊功能，则不进行操作
 */
void amhw_lpc82x_swm_pin_fixed_func_disable (amhw_lpc82x_swm_t *p_hw_swm, 
                                             int                pin);

/**
 * \brief 使能引脚的特殊功能
 *
 * \param[in] p_hw_swm : 指向SWM寄存器块的指针
 * \param[in] pin      : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] func     : 功能宏，PIO0_*_*(#PIO0_0_ACMP_I1),
 *                       在LPC82x_pin.h文件中定义 \see LPC82x_pin.h
 *
 * \return 无
 *
 * \note 如果引脚没有特殊功能，则不进行操作
 */
void amhw_lpc82x_swm_pin_fixed_func_enable (amhw_lpc82x_swm_t *p_hw_swm,
                                            int                pin,
                                            uint32_t           func);

/**
 * @} amhw_lpc82x_if_swm
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_SWM_H */

/* end of file */
