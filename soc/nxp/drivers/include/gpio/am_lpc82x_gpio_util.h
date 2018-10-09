/*******************************************************************************
* AMetal
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
 * \brief GPIO平台驱动层辅助工具宏
 *
 * - 该文件被包含于lpc8xx_pin.h \sa lpc82x_pin.h 其定义了GPIO平台引脚的各个功能。
 *
 * \internal
 * \par Modification History
 * - 1.01 15-11-26  mem, modified.
 * - 1.02 15-07-28  zxl, modified.
 * - 1.00 15-07-08  zxl, first implementation.
 * \endinternal
 */

#ifndef __AM_LPC82X_GPIO_UTIL_H
#define __AM_LPC82X_GPIO_UTIL_H

#include "ametal.h"
#include "am_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * \addtogroup am_lpc82x_if_gpio_util
 * \copydoc am_lpc82x_gpio_util.h
 * @{
 */
 
/* 
 * bit[31:12] 平台定义
 *
 * - bit[13:12] 中继模式配置 
 * - bit[15:14] 引脚迟滞配置 
 * - bit[17:16] 输入反转配置 
 * - bit[23:18] 功能配置（SWM）      
 * - bit[25:24] I2C模式配置          
 * - bit[28:26] 滤波周期            
 * - bit[31:29] 滤波器分频
 */

/**
 * \name 引脚中继模式配置工具宏
 *
 * @{
 */
 
/** \brief 中继模式码偏移 */
#define AM_LPC82X_GPIO_REP_BITS_START   12
      
/** \brief 中继模式码长度 */             
#define AM_LPC82X_GPIO_REP_BITS_LEN     2
      
/** \brief 设置中继模式编码 */             
#define AM_LPC82X_GPIO_REP_CODE(a)      AM_SBF(((a) +1), \
                                               AM_LPC82X_GPIO_REP_BITS_START)

/** \brief 中继模式位域掩码 */             
#define AM_LPC82X_GPIO_REP_MASK         AM_SBF(0x03, \
                                               AM_LPC82X_GPIO_REP_BITS_START)
      
/** \brief 获取中继模式码 */                
#define AM_LPC82X_GPIO_REP_GET(data) \
                           (AM_BITS_GET((data), \
                                        AM_LPC82X_GPIO_REP_BITS_START, \
                                        AM_LPC82X_GPIO_REP_BITS_LEN) \
                            - 1)

/** @} */

/**
 * \name 引脚迟滞配置工具宏
 *
 * @{
 */
 
/** \brief 迟滞码偏移 */
#define AM_LPC82X_GPIO_HYS_BITS_START   14
  
/** \brief 迟滞码长度 */               
#define AM_LPC82X_GPIO_HYS_BITS_LEN     2
  
/** \brief 设置迟滞编码 */              
#define AM_LPC82X_GPIO_HYS_CODE(a)      AM_SBF(((a) + 1), \
                                               AM_LPC82X_GPIO_HYS_BITS_START)

/** \brief 迟滞位域掩码 */              
#define AM_LPC82X_GPIO_HYS_MASK         AM_SBF(0x03, \
                                               AM_LPC82X_GPIO_HYS_BITS_START)
  
/** \brief 获取迟滞码 */               
#define AM_LPC82X_GPIO_HYS_GET(data) \
                           (AM_BITS_GET((data), \
                                        AM_LPC82X_GPIO_HYS_BITS_START, \
                                        AM_LPC82X_GPIO_HYS_BITS_LEN) \
                            - 1)

/** @} */

/**
 * \name 引脚输入反转配置工具宏
 *
 * @{
 */
 
/** \brief 输入反转码偏移 */
#define AM_LPC82X_GPIO_INV_BITS_START   16

/** \brief 输入反转码长度 */
#define AM_LPC82X_GPIO_INV_BITS_LEN     2

/** \brief 设置输入反转编码 */
#define AM_LPC82X_GPIO_INV_CODE(a)      AM_SBF(((a) + 1), \
                                               AM_LPC82X_GPIO_INV_BITS_START)

/** \brief 输入反转位域掩码 */            
#define AM_LPC82X_GPIO_INV_MASK         AM_SBF(0x03, \
                                               AM_LPC82X_GPIO_INV_BITS_START)

/** \brief 获取输入反转码 */
#define AM_LPC82X_GPIO_INV_GET(data) \
                           (AM_BITS_GET((data), \
                                        AM_LPC82X_GPIO_INV_BITS_START, \
                                        AM_LPC82X_GPIO_INV_BITS_LEN) \
                            - 1)

/** @} */

/**
 * \name 引脚功能配置工具宏
 *
 * @{
 */
 
/** \brief 功能码偏移 */
#define AM_LPC82X_GPIO_FUNC_BITS_START  18
  
/** \brief 功能码长度 */               
#define AM_LPC82X_GPIO_FUNC_BITS_LEN    6
  
/** \brief 设置功能编码 */              
#define AM_LPC82X_GPIO_FUNC_CODE(a)     AM_SBF(((a) + 1), \
                                               AM_LPC82X_GPIO_FUNC_BITS_START)

/** \brief 功能位域掩码 */              
#define AM_LPC82X_GPIO_FUNC_MASK        AM_SBF(0x3F, \
                                               AM_LPC82X_GPIO_FUNC_BITS_START)
               
/** \brief 获取功能码 */               
#define AM_LPC82X_GPIO_FUNC_GET(data) \
                           (AM_BITS_GET((data), \
                                        AM_LPC82X_GPIO_FUNC_BITS_START, \
                                        AM_LPC82X_GPIO_FUNC_BITS_LEN) \
                            - 1)

/** @} */

/**
 * \name 引脚I2C模式配置工具宏
 *
 * @{
 */

/** \brief I2C模式码偏移 */
#define AM_LPC82X_GPIO_I2C_BITS_START   24

/** \brief I2C模式码长度 */
#define AM_LPC82X_GPIO_I2C_BITS_LEN     2

/** \brief 设置I2C模式码编码 */
#define AM_LPC82X_GPIO_I2C_CODE(a)      AM_SBF(((a) + 1), \
                                               AM_LPC82X_GPIO_I2C_BITS_START)

/** \brief I2C模式域掩码 */             
#define AM_LPC82X_GPIO_I2C_MASK         AM_SBF(0x03, \
                                               AM_LPC82X_GPIO_I2C_BITS_START)

/** \brief I2C模式码设置获取 */
#define AM_LPC82X_GPIO_I2C_GET(data) \
                           (AM_BITS_GET((data), \
                                        AM_LPC82X_GPIO_I2C_BITS_START, \
                                        AM_LPC82X_GPIO_I2C_BITS_LEN) \
                            - 1)
           
/** @} */

/**
 * \name 引脚滤波周期配置工具宏
 *
 * @{
 */

/** \brief 滤波周期码偏移 */
#define AM_LPC82X_GPIO_FIL_CYC_BITS_START  26
                                           
/** \brief 滤波周期码长度 */               
#define AM_LPC82X_GPIO_FIL_CYC_BITS_LEN    3
                                           
/** \brief 设置滤波周期编码 */             
#define AM_LPC82X_GPIO_FIL_CYC_CODE(a)     AM_SBF(((a) + 1), \
                                                  AM_LPC82X_GPIO_FIL_CYC_BITS_START)
                                           
/** \brief 滤波周期掩码 */                 
#define AM_LPC82X_GPIO_FIL_CYC_MASK        AM_SBF(0x07, \
                                                  AM_LPC82X_GPIO_FIL_CYC_BITS_START)
                                           
/** \brief 获取滤波周期 */                 
#define AM_LPC82X_GPIO_FIL_CYC_GET(data)  \
                              (AM_BITS_GET((data), \
                                           AM_LPC82X_GPIO_FIL_CYC_BITS_START, \
                                           AM_LPC82X_GPIO_FIL_CYC_BITS_LEN) \
                               -1)
        
/** @} */

/**
 * \name 引脚滤波分频配置工具宏
 *
 * @{
 */

/** \brief 滤波分频码偏移 */
#define AM_LPC82X_GPIO_FIL_DIV_BITS_START  29
  
/** \brief 滤波分频码长度 */           
#define AM_LPC82X_GPIO_FIL_DIV_BITS_LEN    3
  
/** \brief 设置滤波分频编码 */         
#define AM_LPC82X_GPIO_FIL_DIV_CODE(a)     AM_SBF(((a) + 1), \
                                                  AM_LPC82X_GPIO_FIL_DIV_BITS_START)
  
/** \brief 滤波分频掩码 */                
#define AM_LPC82X_GPIO_FIL_DIV_MASK        AM_SBF(0x07UL,\
                                                  AM_LPC82X_GPIO_FIL_DIV_BITS_START)

/** \brief 获取滤波分频 */
#define AM_LPC82X_GPIO_FIL_DIV_GET(data) \
                              (AM_BITS_GET((data), \
                                           AM_LPC82X_GPIO_FIL_DIV_BITS_START, \
                                           AM_LPC82X_GPIO_FIL_DIV_BITS_LEN) \
                               - 1)

/** @} */

/*************************LPC8xx引脚特性***************************************/

/**
 * \name 引脚模式
 *
 * @{
 */

/** \brief 消极模式(无上拉/下拉) */
#define AM_LPC82X_GPIO_INACTIVE           AM_GPIO_FLOAT

/** \brief 下拉模式 */               
#define AM_LPC82X_GPIO_PULLDOWN           AM_GPIO_PULLDOWN

/** \brief 上拉模式 */               
#define AM_LPC82X_GPIO_PULLUP             AM_GPIO_PULLUP

/** \brief 开漏模式 */               
#define AM_LPC82X_GPIO_OPEN_DRAIN         AM_GPIO_OPEN_DRAIN

/** \brief 中继模式 */               
#define AM_LPC82X_GPIO_REPEATER           AM_LPC82X_GPIO_REP_CODE(0)

/** @} */

/**
 * \name 引脚方向
 *
 * @{
 */

/** \brief 输入  */
#define AM_LPC82X_GPIO_INPUT              AM_GPIO_INPUT

/** \brief 输出高  */                  
#define AM_LPC82X_GPIO_OUTPUT_HIGH        AM_GPIO_OUTPUT_INIT_HIGH

/** \brief 输出低  */                  
#define AM_LPC82X_GPIO_OUTPUT_LOW         AM_GPIO_OUTPUT_INIT_LOW

/** @} */

/**
 * \name 引脚迟滞使能和禁能
 *
 * @{
 */

/** \brief 不开启 */
#define AM_LPC82X_GPIO_HYS_DISABLE        AM_LPC82X_GPIO_HYS_CODE(0UL)

/** \brief 开启  */                  
#define AM_LPC82X_GPIO_HYS_ENABLE         AM_LPC82X_GPIO_HYS_CODE(1UL)

/** @} */

/**
 * \name 引脚输入反转使能和禁能
 *
 * @{
 */

/** \brief 输入不反转 */
#define AM_LPC82X_GPIO_INV_DISABLE        AM_LPC82X_GPIO_INV_CODE(0UL)

/** \brief 输入发转  */              
#define AM_LPC82X_GPIO_INV_ENABLE         AM_LPC82X_GPIO_INV_CODE(1UL)

/** @} */

/**
 * \name 引脚滤波周期
 *
 * @{
 */

/** \brief 不使用滤波 */
#define AM_LPC82X_GPIO_FIL_DISABLE        AM_LPC82X_GPIO_FIL_CYC_CODE(0UL)

/** \brief 滤除小于一个周期信号 */   
#define AM_LPC82X_GPIO_FIL_1CYCLE         AM_LPC82X_GPIO_FIL_CYC_CODE(1UL)

/** \brief 滤除小于两个周期信号 */   
#define AM_LPC82X_GPIO_FIL_2CYCLE         AM_LPC82X_GPIO_FIL_CYC_CODE(2UL)

/** \brief 滤除小于三个周期信号 */   
#define AM_LPC82X_GPIO_FIL_3CYCLE         AM_LPC82X_GPIO_FIL_CYC_CODE(3UL)

/** @} */

/**
 * \name 引脚滤波分频
 *
 * @{
 */

/** \brief 分频系数位0 */
#define AM_LPC82X_GPIO_FIL_DIV0           AM_LPC82X_GPIO_FIL_DIV_CODE(0UL)

/** \brief 分频系数位1 */            
#define AM_LPC82X_GPIO_FIL_DIV1           AM_LPC82X_GPIO_FIL_DIV_CODE(1UL)

/** \brief 分频系数位2 */            
#define AM_LPC82X_GPIO_FIL_DIV2           AM_LPC82X_GPIO_FIL_DIV_CODE(2UL)

/** \brief 分频系数位3 */            
#define AM_LPC82X_GPIO_FIL_DIV3           AM_LPC82X_GPIO_FIL_DIV_CODE(3UL)

/** \brief 分频系数位4 */            
#define AM_LPC82X_GPIO_FIL_DIV4           AM_LPC82X_GPIO_FIL_DIV_CODE(4UL)

/** \brief 分频系数位5 */            
#define AM_LPC82X_GPIO_FIL_DIV5           AM_LPC82X_GPIO_FIL_DIV_CODE(5UL)

/** \brief 分频系数位6 */            
#define AM_LPC82X_GPIO_FIL_DIV6           AM_LPC82X_GPIO_FIL_DIV_CODE(6UL)

/** @} */

/** 
 * \name 引脚可转移功能
 * @{
 */

/** \brief UART0_TXD  功能 */
#define AM_LPC82X_GPIO_FUNC_U0_TXD        AM_LPC82X_GPIO_FUNC_CODE(0UL)

/** \brief UART0_RXD  功能 */        
#define AM_LPC82X_GPIO_FUNC_U0_RXD        AM_LPC82X_GPIO_FUNC_CODE(1UL)

/** \brief UART0_RTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U0_RTS        AM_LPC82X_GPIO_FUNC_CODE(2UL)

/** \brief UART0_CTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U0_CTS        AM_LPC82X_GPIO_FUNC_CODE(3UL)

/** \brief UART0_SCLK 功能 */        
#define AM_LPC82X_GPIO_FUNC_U0_SCLK       AM_LPC82X_GPIO_FUNC_CODE(4UL)

/** \brief UART1_TXD  功能 */        
#define AM_LPC82X_GPIO_FUNC_U1_TXD        AM_LPC82X_GPIO_FUNC_CODE(5UL)

/** \brief UART1_RXD  功能 */        
#define AM_LPC82X_GPIO_FUNC_U1_RXD        AM_LPC82X_GPIO_FUNC_CODE(6UL)

/** \brief UART1_RTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U1_RTS        AM_LPC82X_GPIO_FUNC_CODE(7UL)

/** \brief UART1_CTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U1_CTS        AM_LPC82X_GPIO_FUNC_CODE(8UL)

/** \brief UART1_SCLK 功能 */        
#define AM_LPC82X_GPIO_FUNC_U1_SCLK       AM_LPC82X_GPIO_FUNC_CODE(9UL)

/** \brief UART2_TXD  功能 */        
#define AM_LPC82X_GPIO_FUNC_U2_TXD        AM_LPC82X_GPIO_FUNC_CODE(10UL)

/** \brief UART2_RXD  功能 */        
#define AM_LPC82X_GPIO_FUNC_U2_RXD        AM_LPC82X_GPIO_FUNC_CODE(11UL)

/** \brief UART2_RTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U2_RTS        AM_LPC82X_GPIO_FUNC_CODE(12UL)

/** \brief UART2_CTS  功能 */        
#define AM_LPC82X_GPIO_FUNC_U2_CTS        AM_LPC82X_GPIO_FUNC_CODE(13UL)

/** \brief UART2_SCLK 功能 */        
#define AM_LPC82X_GPIO_FUNC_U2_SCLK       AM_LPC82X_GPIO_FUNC_CODE(14UL)

/** \brief SPI0_SCK   功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_SCK      AM_LPC82X_GPIO_FUNC_CODE(15UL)

/** \brief SPI0_MOSI  功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_MOSI     AM_LPC82X_GPIO_FUNC_CODE(16UL)

/** \brief SPI0_MISO  功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_MISO     AM_LPC82X_GPIO_FUNC_CODE(17UL)

/** \brief SPI0_SSEL0 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_SSEL0    AM_LPC82X_GPIO_FUNC_CODE(18UL)

/** \brief SPI0_SSEL1 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_SSEL1    AM_LPC82X_GPIO_FUNC_CODE(19UL)

/** \brief SPI0_SSEL2 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_SSEL2    AM_LPC82X_GPIO_FUNC_CODE(20UL)

/** \brief SPI0_SSEL3 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI0_SSEL3    AM_LPC82X_GPIO_FUNC_CODE(21UL)

/** \brief SPI1_SCK   功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI1_SCK      AM_LPC82X_GPIO_FUNC_CODE(22UL)

/** \brief SPI1_MOSI  功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI1_MOSI     AM_LPC82X_GPIO_FUNC_CODE(23UL)

/** \brief SPI1_MISO  功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI1_MISO     AM_LPC82X_GPIO_FUNC_CODE(24UL)

/** \brief SPI1_SSEL0 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI1_SSEL0    AM_LPC82X_GPIO_FUNC_CODE(25UL)

/** \brief SPI1_SSEL1 功能 */        
#define AM_LPC82X_GPIO_FUNC_SPI1_SSEL1    AM_LPC82X_GPIO_FUNC_CODE(26UL)

/** \brief SCT_PIN_PIN0 功能 */      
#define AM_LPC82X_GPIO_FUNC_SCT_PIN0      AM_LPC82X_GPIO_FUNC_CODE(27UL)

/** \brief SCT_PIN_PIN1 功能 */      
#define AM_LPC82X_GPIO_FUNC_SCT_PIN1      AM_LPC82X_GPIO_FUNC_CODE(28UL)

/** \brief SCT_PIN_PIN2 功能 */      
#define AM_LPC82X_GPIO_FUNC_SCT_PIN2      AM_LPC82X_GPIO_FUNC_CODE(29UL)

/** \brief SCT_PIN_PIN3 功能 */      
#define AM_LPC82X_GPIO_FUNC_SCT_PIN3      AM_LPC82X_GPIO_FUNC_CODE(30UL)

/** \brief SCT_OUT0 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT0      AM_LPC82X_GPIO_FUNC_CODE(31UL)

/** \brief SCT_OUT1 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT1      AM_LPC82X_GPIO_FUNC_CODE(32UL)

/** \brief SCT_OUT2 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT2      AM_LPC82X_GPIO_FUNC_CODE(33UL)

/** \brief SCT_OUT3 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT3      AM_LPC82X_GPIO_FUNC_CODE(34UL)

/** \brief SCT_OUT4 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT4      AM_LPC82X_GPIO_FUNC_CODE(35UL)

/** \brief SCT_OUT5 功能 */          
#define AM_LPC82X_GPIO_FUNC_SCT_OUT5      AM_LPC82X_GPIO_FUNC_CODE(36UL)

/** \brief I2C1_SDA 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C1_SDA      AM_LPC82X_GPIO_FUNC_CODE(37UL)

/** \brief I2C1_SCL 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C1_SCL      AM_LPC82X_GPIO_FUNC_CODE(38UL)

/** \brief I2C2_SDA 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C2_SDA      AM_LPC82X_GPIO_FUNC_CODE(39UL)

/** \brief I2C2_SCL 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C2_SCL      AM_LPC82X_GPIO_FUNC_CODE(40UL)

/** \brief I2C3_SDA 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C3_SDA      AM_LPC82X_GPIO_FUNC_CODE(41UL)

/** \brief I2C3_SCL 功能 */          
#define AM_LPC82X_GPIO_FUNC_I2C3_SCL      AM_LPC82X_GPIO_FUNC_CODE(42UL)

/** \brief ADC_PINTRIG0 功能 */      
#define AM_LPC82X_GPIO_FUNC_ADC_PINTRIG0  AM_LPC82X_GPIO_FUNC_CODE(43UL)

/** \brief ADC_PINTRIG1 功能 */      
#define AM_LPC82X_GPIO_FUNC_ADC_PINTRIG1  AM_LPC82X_GPIO_FUNC_CODE(44UL)

/** \brief ACMP 功能 */              
#define AM_LPC82X_GPIO_FUNC_ACMP_O        AM_LPC82X_GPIO_FUNC_CODE(45UL)

/** \brief CLKOUT 功能 */            

#define AM_LPC82X_GPIO_FUNC_CLKOUT        AM_LPC82X_GPIO_FUNC_CODE(46UL)

/** \brief GPIO_INT_BMAT 功能 */     
#define AM_LPC82X_GPIO_FUNC_GPIO_INT_BMAT AM_LPC82X_GPIO_FUNC_CODE(47UL)

/** @} */                            

/**  
 * \name 引脚 GPIO功能             
 * @{
 */  

/** \brief GPIO  功能 */             
#define AM_LPC82X_GPIO_FUNC_GPIO          AM_LPC82X_GPIO_FUNC_CODE(48UL)

/** @} */                            

/**  
 * \name 引脚 特殊功能             
 * @{
 */  

/** \brief 特殊功能1 */         
#define AM_LPC82X_GPIO_FUNC_OTHER1        AM_LPC82X_GPIO_FUNC_CODE(49UL)

/** \brief 特殊功能2 */         
#define AM_LPC82X_GPIO_FUNC_OTHER2        AM_LPC82X_GPIO_FUNC_CODE(50UL)

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LPC82X_GPIO_UTIL_H */

/* end of file */
