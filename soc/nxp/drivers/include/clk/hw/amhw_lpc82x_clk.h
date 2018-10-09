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
 * \brief 时钟选择和控制接口
 *
 * \internal
 * \par Modification history
 * - 1.02 15-11-23  lnk, modified
 * - 1.01 15-07-21  win, add function amhw_lpc82x_clk_clkout_update() and
 *                       amhw_lpc82x_clk_clkout_update_chk().
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_CLK_H
#define __AMHW_LPC82X_CLK_H

#include "ametal.h"
#include "hw/amhw_lpc82x_syscon.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_clk
 * \copydoc amhw_lpc82x_clk.h
 * @{
 */

/**
 * \name 频率值定义
 *
 * 如果使用了外部管脚（PIO0_22）作为时钟输入管脚，则需要将
 * AMHW_LPC82X_CLK_FREQ_CLKIN 设置为正确的频率值.
 *
 * @{
 */

/** \brief 内部IRC频率         */
#define AMHW_LPC82X_CLK_FREQ_IRC      12000000UL  
/** \brief 外部管脚的输入频率  */
#define AMHW_LPC82X_CLK_FREQ_CLKIN    12000000UL   
/** \brief 外部晶振频率        */
#define AMHW_LPC82X_CLK_FREQ_XTAL     12000000UL   

/** @} */

/**
 * \brief 系统或外设时钟编号
 */
typedef enum amhw_lpc82x_clk_periph {
    AMHW_LPC82X_CLK_SYS = 0,           /**< \brief System 时钟         */
    AMHW_LPC82X_CLK_ROM,               /**< \brief ROM 时钟            */
    AMHW_LPC82X_CLK_SRAM0_1,           /**< \brief SRAM0 SRAM1 时钟    */
    AMHW_LPC82X_CLK_FLASHREG,          /**< \brief FLASH寄存器接口时钟 */
    AMHW_LPC82X_CLK_FLASH,             /**< \brief FLASH 时钟          */
    AMHW_LPC82X_CLK_I2C0,              /**< \brief I2C0时钟            */
    AMHW_LPC82X_CLK_GPIO,              /**< \brief GPIO 时钟           */
    AMHW_LPC82X_CLK_SWM,               /**< \brief SWM 时钟            */
    AMHW_LPC82X_CLK_SCT,               /**< \brief SCT 时钟            */
    AMHW_LPC82X_CLK_WKT,               /**< \brief WKT 时钟            */
    AMHW_LPC82X_CLK_MRT,               /**< \brief MRT时钟             */
    AMHW_LPC82X_CLK_SPI0,              /**< \brief SPI0 时钟           */
    AMHW_LPC82X_CLK_SPI1,              /**< \brief SPI1 时钟           */
    AMHW_LPC82X_CLK_CRC,               /**< \brief CRC 时钟            */
    AMHW_LPC82X_CLK_UART0,             /**< \brief UART0 时钟          */
    AMHW_LPC82X_CLK_UART1,             /**< \brief UART1 时钟          */
    AMHW_LPC82X_CLK_UART2,             /**< \brief UART2 时钟          */
    AMHW_LPC82X_CLK_WWDT,              /**< \brief WWDT 时钟           */
    AMHW_LPC82X_CLK_IOCON,             /**< \brief IOCON 时钟          */
    AMHW_LPC82X_CLK_ACMP,              /**< \brief ACMP 时钟           */
    AMHW_LPC82X_CLK_I2C1 = 21,         /**< \brief I2C1 时钟           */
    AMHW_LPC82X_CLK_I2C2,              /**< \brief I2C2 时钟           */
    AMHW_LPC82X_CLK_I2C3,              /**< \brief I2C3 时钟           */
    AMHW_LPC82X_CLK_ADC0,              /**< \brief ADC 时钟            */
    AMHW_LPC82X_CLK_MTB = 26,          /**< \brief MTB 时钟            */
    AMHW_LPC82X_CLK_DMA = 29           /**< \brief DMA 时钟            */
} amhw_lpc82x_clk_periph_t;

/**
 * \brief PLL 输入时钟源
 */
typedef enum amhw_lpc82x_pllinclk_src {
    AMHW_LPC82X_CLK_PLLIN_SRC_IRC = 0,   /**< \brief 内部IRC           */
    AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC,    /**< \brief 系统振荡器        */
    AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN = 3  /**< \brief 外部引脚输入时钟  */
} amhw_lpc82x_pllinclk_src_t;                                          
                                                                       
/**                                                                    
 * \brief 主时钟源                                                     
 */                                                                    
typedef enum amhw_lpc82x_clk_main_src {                                
    AMHW_LPC82X_CLK_MAIN_SRC_IRC = 0,    /**< \brief 内部IRC           */
    AMHW_LPC82X_CLK_MAIN_SRC_PLLIN,      /**< \brief PLL input         */
    AMHW_LPC82X_CLK_MAIN_SRC_WDTOSC,     /**< \brief 看门狗振荡器      */
    AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT      /**< \brief PLL Output        */
} amhw_lpc82x_clk_main_src_t;                                          
                                                                       
/**                                                                    
 * \brief CLKOUT 引脚的时钟源                                          
 */                                                                    
typedef enum amhw_lpc82x_clk_clkout_src {                              
    AMHW_LPC82X_CLK_CLKOUT_SRC_IRC = 0,  /**< \brief 内部IRC           */
    AMHW_LPC82X_CLK_CLKOUT_SRC_SYSOSC,   /**< \brief 系统振荡器        */
    AMHW_LPC82X_CLK_CLKOUT_SRC_WDTOSC,   /**< \brief 看门狗振荡器      */
    AMHW_LPC82X_CLK_CLKOUT_SRC_MAINCLK   /**< \brief 主时钟            */
} amhw_lpc82x_clk_clkout_src_t;

/**
 * \brief IOCON 时钟分频选择
 */ 
typedef enum amhw_lpc82x_clk_iocon_div_t {
    AMHW_LPC82X_CLK_IOCON_CLKDIV0 = 0,   /**< \brief IOCON 时钟分频器0 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV1,       /**< \brief IOCON 时钟分频器1 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV2,       /**< \brief IOCON 时钟分频器2 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV3,       /**< \brief IOCON 时钟分频器3 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV4,       /**< \brief IOCON 时钟分频器4 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV5,       /**< \brief IOCON 时钟分频器5 */
    AMHW_LPC82X_CLK_IOCON_CLKDIV6,       /**< \brief IOCON 时钟分频器6 */
} amhw_lpc82x_clk_iocon_div_t;

/**
 * \brief 得到内部IRC时钟频率
 *
 * \return 内部IRC时钟频率（单位：Hz）
 */
am_static_inline
uint32_t amhw_lpc82x_clk_irc_rate_get (void)
{
    return AMHW_LPC82X_CLK_FREQ_IRC;
}

/**
 * \brief 得到外部XTAL时钟频率
 *
 * \return 外部XTAL时钟频率（单位：Hz）
 */
am_static_inline
uint32_t amhw_lpc82x_clk_xtal_rate_get (void)
{
    return AMHW_LPC82X_CLK_FREQ_XTAL;
}

/**
 * \brief 得到外部管脚输入时钟频率
 *
 * \return 外部管脚输入时钟频率（单位：Hz）
 */
am_static_inline
uint32_t amhw_lpc82x_clk_clkin_rate_get (void)
{
    return AMHW_LPC82X_CLK_FREQ_CLKIN;
}


/**
 * \brief 得到WDT振荡器的时钟频率
 *
 * \return  WDT振荡器的时钟频率（单位：Hz）
 *
 * \note 这个频率精度是正负40%的误差
 */
uint32_t amhw_lpc82x_clk_wdt_rate_get (void);

/**
 * \brief PLL 控制寄存器配置
 *
 * \param[in] msel : 反馈分频器的值
 * \param[in] psel : 后背分频器的值
 *
 * \return 无
 * \note  M = msel + 1
 *        P = 2^(psel)
 *        PLLOUT = M * PLLIN;
 *        FCCO = 2 * P * PLLOUT;
 *        PLLIN接收输入频率10MHz～25MHz，设置M的值使得PLLOUT输出频率低于100MHz,
 *        设置P的值使FCCO频率在156MHz～320MHz
 */
am_static_inline
void amhw_lpc82x_clk_pllctrl_set (uint8_t msel, uint8_t psel)
{
    LPC82X_SYSCON->syspllctrl = AM_SBF(psel, 5) | msel;
}

/**
 * \brief PLL时钟源选择
 *
 * \param[in] src : PLL时钟源
 * 
 * \return 无
 *
 */
am_static_inline
void amhw_lpc82x_clk_pll_src_set (amhw_lpc82x_pllinclk_src_t src)
{
    LPC82X_SYSCON->syspllclksel = (uint32_t)src;
    
    /* 更新PLL时钟 */
    LPC82X_SYSCON->syspllclkuen = 0x00;
    LPC82X_SYSCON->syspllclkuen = 0x01;
}    

/**
 * \brief 使能指定的系统或外设时钟
 *
 * \param[in] clk : 指定的待使能的系统或外设时钟
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_clk_periph_enable (amhw_lpc82x_clk_periph_t clk)
{
    AM_BIT_SET(LPC82X_SYSCON->sysahbclkctrl, clk);
}

/**
 * \brief 禁能指定的系统或外设时钟
 *
 * \param[in] clk : 指定的待禁能的系统或外设时钟
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_clk_periph_disable (amhw_lpc82x_clk_periph_t clk)
{
    AM_BIT_CLR(LPC82X_SYSCON->sysahbclkctrl, clk);
}

/**
 * \brief 获取系统PLL输出时钟频率
 *
 * \return PLL输出时钟频率
 */
uint32_t amhw_lpc82x_clk_pllout_rate_get (void);

/**
 * \brief 获取系统PLL输入时钟频率
 *
 * \return PLL输入时钟频率
 */
uint32_t amhw_lpc82x_clk_pllin_rate_get (void);

/**
 * \brief 判断PLL是否锁定
 *
 * \return AM_TRUE  锁定
 *          AM_FALSE 未锁定
 */
am_static_inline
am_bool_t amhw_lpc82x_clk_pll_locked_chk (void)
{
    return (am_bool_t)(LPC82X_SYSCON->syspllstat & 0x01);
}

/**
 * \brief 系统振荡器旁路控制使能/禁能
 *
 * \param[in] bypass : 旁路系统振荡器  AM_FALSE-禁能,TURE-使能
 * \param[in] highfr : 振荡器频率范围  AM_FALSE: 1-20MHz TRUE:15-25MHz
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_clk_pll_bypass_set (am_bool_t bypass, am_bool_t highfr)
{
    if (bypass) {
        AM_BIT_SET(LPC82X_SYSCON->sysoscctrl, 0);
    } else {
        AM_BIT_CLR(LPC82X_SYSCON->sysoscctrl, 0);
    }
    
    if (highfr) {
        AM_BIT_SET(LPC82X_SYSCON->sysoscctrl, 1);
    } else {
        AM_BIT_CLR(LPC82X_SYSCON->sysoscctrl, 1);
    }
}

/**
 * \brief 设置 main 时钟源
 *
 * \param[in] src : main时钟源
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_clk_main_src_set (amhw_lpc82x_clk_main_src_t src)
{
    LPC82X_SYSCON->mainclksel = (uint32_t)src;
    
    /* 更新main时钟 */
    LPC82X_SYSCON->mainclkuen = 0x00;
    LPC82X_SYSCON->mainclkuen = 0x01;
}


/**
 * \brief 得到 main 时钟源
 *
 * \return main 时钟源
 */
am_static_inline
amhw_lpc82x_clk_main_src_t amhw_lpc82x_clk_main_src_get (void)
{
    return (amhw_lpc82x_clk_main_src_t)(LPC82X_SYSCON->mainclksel);
}

/**
 * \brief 得到 main 时钟频率
 *
 * \return  main 时钟频率（单位:Hz）
 */
uint32_t amhw_lpc82x_clk_main_clkrate_get (void);


/**
 * \brief 设置 CLKOUT 时钟源和分频值
 *
 * \param[in] src : CLKOUT的时钟源
 * \param[in] div : 对CLKOUT时钟源的分频
 *
 * \return 无
 *
 * \note 分频值为0时将禁能CLKOUT的输出，其它有效值为1~255. 
 *       最终CLKOUT管脚输出的时钟频率为CLKOUT时钟源的频率除以分频系数。
 */
void amhw_lpc82x_clk_clkout_config (amhw_lpc82x_clk_clkout_src_t src,  
                                    uint32_t                     div);

/**
 * \name  WDTOSC可选频率定义
 * \anchor grp_amhw_lpc82x_clk_wdtosc_rate
 *  @{
 */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_0_6MHZ     1   /**< \brief 0.6MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_1_05MHZ    2   /**< \brief 1.05MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_1_4MHZ     3   /**< \brief 1.4MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_1_75MHZ    4   /**< \brief 1.75MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_2_1MHZ     5   /**< \brief 2.1MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_2_4MHZ     6   /**< \brief 2.4MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_2_7MHZ     7   /**< \brief 2.7MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_3_0MHZ     8   /**< \brief 3.0MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_3_25MHZ    9   /**< \brief 3.25MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_3_5MHZ     10  /**< \brief 3.5MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_3_75MHZ    11  /**< \brief 3.75MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_4_0MHZ     12  /**< \brief 4.0MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_4_2MHZ     13  /**< \brief 4.2MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_4_4MHZ     14  /**< \brief 4.4MHz */
#define AMHW_LPC82X_CLK_WDTOSC_RATE_4_6MHZ     15  /**< \brief 4.6MHz */

/** @} */

/**
 * \brief WDTOSC 控制寄存器配置
 *
 * \param[in] rate : 选择看门狗振荡器输出频率, 
 *                   参见 \ref grp_amhw_lpc82x_clk_wdtosc_rate
 * \param[in] div  : 为Fclkana选择分频系数, 2 - 64之间的偶数
 *
 * \return 无
 * 
 * \note  看门狗振荡器频率在复位后需要重新配置
 */
am_static_inline
void amhw_lpc82x_clk_wdtoscc_cfg (uint8_t rate, 
                                  uint8_t div)
{                                          
    LPC82X_SYSCON->wdtoscctrl = AM_SBF((uint8_t) (rate), 5) |
                                     ((div >> 1) - 1);
}

/**
 * \brief 设置系统时钟分频值
 *
 * \param[in] div : 系统时钟分频值
 *
 * \return 无
 *
 * \note 分频值为0时将禁能系统时钟，其它有效值为1~255.
 *       系统时钟频率为主时钟频率除以该处设置的系统时钟分频值。
 */
am_static_inline
void amhw_lpc82x_clk_system_clkdiv_set (uint8_t div)
{
    LPC82X_SYSCON->sysahbclkdiv = div;
}

/**
 * \brief 得到系统时钟分频值
 *
 * \return 系统时钟分频值
 */
am_static_inline
uint8_t amhw_lpc82x_clk_system_clkdiv_get (void)
{
    return LPC82X_SYSCON->sysahbclkdiv & 0xFF;
}

/**
 * \brief 得到系统时钟频率
 *
 * \return 系统时钟频率
 */
uint32_t amhw_lpc82x_clk_system_clkrate_get (void);

/**
 * \brief 串口时钟分频
 *
 * \param[in] div : 分频数值设定
 *
 * \return 无
 *
 * \note 分频值为0时将禁能串口时钟，其它有效值为1~255.
 *       串口时钟频率为主时钟频率除以该处设置的串口时钟分频值。
 */
am_static_inline
void amhw_lpc82x_syscon_uartclkdiv_set(uint8_t div)
{
    LPC82X_SYSCON->uartclkdiv = div;
}

/**
 * \brief 设置串口小数分频器的值
 *
 * \param[in] fmul : 小数分频器的倍乘数
 * \param[in] fdiv : 小数分频器的除数 (必须是0xFF)
 *
 * \return 无
 *
 * \note    U_PCLK = UARTCLKDIV / (1 + MULT/DIV)
 */
am_static_inline
void amhw_lpc82x_syscon_usart_frgctrl_set (uint8_t fmul, uint8_t fdiv)
{
    LPC82X_SYSCON->uartfrgdiv  = fdiv;
    LPC82X_SYSCON->uartfrgmult = fmul;
}

/**
 * \brief 获取串口基本输入频率（通过FRG）
 *
 * \return 串口基本输入频率值
 */
uint32_t amhw_lpc82x_clk_usart_baseclkrate_get (void);

/**
 * \brief 设置串口基本输入频率
 *
 * \param[in] rate : 期望的串口基本输入频率设置值（必须小于系统时钟频率）
 *
 * \retval   AM_OK     : 操作成功
 * \retval  -AM_EINVAL : 无效参数
 *
 * \note 实际的串口基本输入频率值，可能与期望值存在偏差
 */
int amhw_lpc82x_clk_usart_baseclkrate_set (uint32_t rate);

/**
 * \brief 设置 IOCON 时钟分频
 *
 * \param[in] divider : 将要设置的IOCON分频器
 * \param[in] div     : 分频值
 *
 * \return 无
 * \note 设置分频值0使关闭，其他分频值0-255
 */
am_static_inline
void amhw_lpc82x_clk_iocon_div_set (amhw_lpc82x_clk_iocon_div_t divider, 
                                    uint8_t                     div)
{
    int t_divider = AMHW_LPC82X_CLK_IOCON_CLKDIV6 - divider;

    LPC82X_SYSCON->ioconclkdiv[t_divider] = div;
}

/**
 * \brief 获取IOCON 时钟频率
 *
 * \param[in] divider : 将要获取的IOCON分频器
 *
 * \return 对应IOCON脉冲滤波器频率
 */
am_static_inline
uint32_t 
amhw_lpc82x_clk_iocon_rate_get (amhw_lpc82x_clk_iocon_div_t divider)
{
    int32_t div = LPC82X_SYSCON->ioconclkdiv[divider] & 0xFF;

    return (div == 0)? (0) : (amhw_lpc82x_clk_main_clkrate_get() / div);
}

/**
 * \brief 得到外设的输入频率
 *
 * \param[in] p_periph : 指向外设的寄存器块的指针
 *
 * \return 外设的输入频率
 *
 */
uint32_t amhw_lpc82x_clk_periph_freq_get (void *p_periph);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __AMHW_LPC82X_CLK_H */

/* end of file */
