/*******************************************************************************
*                                AMetal
*                      ----------------------------
*                      innovating embedded platform
*
*Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
*All rights reserved.
*
*Contact information:
*web site:    http://www.zlg.cn/
*e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 时钟硬件操作接口实现。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-23  lnk,modified.
 * - 1.00 15-07-09  win, first implementation.
 * \endinternal
 */
 
#include "ametal.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "math.h"


/**
 * \brief 获取系统PLL输入时钟频率
 */
uint32_t amhw_lpc82x_clk_pllin_rate_get (void)
{     
    uint32_t syspllin_rate;
    
    switch ((amhw_lpc82x_pllinclk_src_t) (LPC82X_SYSCON->syspllclksel &
             0x03)) {
          
    case AMHW_LPC82X_CLK_PLLIN_SRC_IRC:       /* 内部RC振荡器 */
        syspllin_rate = AMHW_LPC82X_CLK_FREQ_IRC;
        break;
    
    case AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC:    /* 系统振荡器 */
        syspllin_rate = AMHW_LPC82X_CLK_FREQ_XTAL;
        break;
        
    case AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN:    /* CLKIN 管脚时钟 */
        syspllin_rate = AMHW_LPC82X_CLK_FREQ_CLKIN;
        break;
    
    default:    /* 默认处理 */
        syspllin_rate = 0;
        break;
    }
    
    return syspllin_rate;
}

/**
 * \brief 获取系统PLL输出时钟频率
 */
uint32_t amhw_lpc82x_clk_pllout_rate_get (void)
{
    uint32_t syspllout_rate;
    
    syspllout_rate = amhw_lpc82x_clk_pllin_rate_get() *
                     ((LPC82X_SYSCON->syspllctrl & 0x01F) + 1);
    
    return syspllout_rate;
}

/**
 * \brief 得到 main 时钟频率
 */
uint32_t amhw_lpc82x_clk_main_clkrate_get (void)
{
    uint32_t clkrate = 0;

    switch (amhw_lpc82x_clk_main_src_get()) {
        
    case AMHW_LPC82X_CLK_MAIN_SRC_IRC:
        clkrate = AMHW_LPC82X_CLK_FREQ_IRC;
        break;

    case AMHW_LPC82X_CLK_MAIN_SRC_PLLIN:
        clkrate = amhw_lpc82x_clk_pllin_rate_get();
        break;

    case AMHW_LPC82X_CLK_MAIN_SRC_WDTOSC:
        clkrate = amhw_lpc82x_clk_wdt_rate_get();
        break;
        
    case AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT:
        clkrate = amhw_lpc82x_clk_pllout_rate_get();
        break;
    
    default:
        clkrate = 0;
        break;
    }
    
    return clkrate;
}


/**
 * \brief 得到WDT振荡器的时钟频率
 */
uint32_t amhw_lpc82x_clk_wdt_rate_get (void)
{
    am_bool_t   wdtosc_ispowerup;
    uint32_t g_wdtosc_clkfreq;
    
    wdtosc_ispowerup = amhw_lpc82x_syscon_power_check(AMHW_LPC82X_SYSCON_PD_WDT_OSC);
    
     
    if (wdtosc_ispowerup == AM_FALSE) {

        /* 打开wdt振荡器*/
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);
    }
    
    /* 得到时钟频率*/
    switch ((LPC82X_SYSCON->wdtoscctrl >> 5) & 0xF) {
        
    case 1:  
        g_wdtosc_clkfreq =  600000; 
        break;
    
    case 2:  
        g_wdtosc_clkfreq = 1050000;
        break;
    
    case 3:  
        g_wdtosc_clkfreq = 1400000; 
        break;
    
    case 4: 
        g_wdtosc_clkfreq = 1750000; 
        break;
    
    case 5:  
        g_wdtosc_clkfreq = 2100000; 
        break;
    
    case 6:  
        g_wdtosc_clkfreq = 2400000; 
        break;
    
    case 7:  
        g_wdtosc_clkfreq = 2700000; 
        break;
    
    case 8:  
        g_wdtosc_clkfreq = 3000000; 
        break;
    
    case 9:  
        g_wdtosc_clkfreq = 3250000; 
        break;
    
    case 10: 
        g_wdtosc_clkfreq = 3500000; 
        break;
    
    case 11: 
        g_wdtosc_clkfreq = 3750000; 
        break;
    
    case 12: 
        g_wdtosc_clkfreq = 4000000; 
        break;
    
    case 13: 
        g_wdtosc_clkfreq = 4200000; 
        break;
    
    case 14: 
        g_wdtosc_clkfreq = 4400000; 
        break;
    
    case 15: 
        g_wdtosc_clkfreq = 4600000; 
        break;
    
    default: 
        g_wdtosc_clkfreq =  600000; 
        break;
    }
    
    g_wdtosc_clkfreq /= (((LPC82X_SYSCON->wdtoscctrl & 0x1F) + 1) << 1);

    return g_wdtosc_clkfreq;
}

/**
 * \brief 设置 CLKOUT 时钟源和分频值
 */
void amhw_lpc82x_clk_clkout_config (amhw_lpc82x_clk_clkout_src_t src,
                                    uint32_t                     div)
{
    /* CLKOUT 时钟源选择 */
    LPC82X_SYSCON->clkoutsel = src;
    
    /* 更新CLKOUT时钟 */
    LPC82X_SYSCON->clkoutuen = 0x00;
    LPC82X_SYSCON->clkoutuen = 0x01;
    
    /* CLKOUT 分频系数*/
    LPC82X_SYSCON->clkoutdiv = div;         
}

/**
 * \brief 得到系统时钟频率
 */
uint32_t amhw_lpc82x_clk_system_clkrate_get (void)
{   
    /* sysahbclkdiv 为0时，系统时钟关闭，复位后sysahbclkdiv默认为 1*/
    return (amhw_lpc82x_clk_main_clkrate_get() /
            amhw_lpc82x_clk_system_clkdiv_get());
}

/**
 * \brief 获取串口基本输入频率（通过FRG）
 */
uint32_t amhw_lpc82x_clk_usart_baseclkrate_get (void)
{
    uint64_t inclk;
    uint32_t mult, div;
   
    /* 得到USART小数分频系数*/
    div = LPC82X_SYSCON->uartclkdiv;
    if (div == 0) {    

        /* 分频系数为0所以UART时钟关闭 */
       inclk = 0; 
    } else{
        inclk = (uint64_t)amhw_lpc82x_clk_main_clkrate_get() / div;
        
        if ((LPC82X_SYSCON->uartfrgdiv & 0xFF) == 0xFF) {
            
            /* 小数部分使能，得到倍频系数*/
            mult = LPC82X_SYSCON->uartfrgmult & 0xFF;
            
            inclk = (inclk * 256) / (uint64_t)(256 + mult);
        }
    }

    return (uint32_t)inclk;
}

/**
 * \brief 设置串口基本输入频率
 */
int amhw_lpc82x_clk_usart_baseclkrate_set (uint32_t rate)
{
    uint32_t div, inclk;
    uint32_t mult;

    /* main clk 作为FRG块时钟源 */
    inclk = amhw_lpc82x_clk_main_clkrate_get() ;
    if (rate > inclk) {
        return -AM_EINVAL;
    }
    
    div = inclk / rate;
    if (div == 0) {
        div = 1;
    }

    /* 设置串口时钟分频值 */
    amhw_lpc82x_syscon_uartclkdiv_set((uint8_t) div);

    /* 计算小数分频值 */
    mult = ((inclk / div) * 256) / rate;

    /* 保留小数部分 */
    LPC82X_SYSCON->uartfrgmult = (uint8_t) mult;
    LPC82X_SYSCON->uartfrgdiv  = 0xFF;
    
    return AM_OK;
}

/**
 * \brief 得到外设的输入频率
 */
uint32_t amhw_lpc82x_clk_periph_freq_get (void *p_periph)
{
    uint32_t base_addr = (uint32_t)(p_periph);
    
    if ((base_addr == LPC82X_USART0_BASE) ||
        (base_addr == LPC82X_USART0_BASE) ||
        (base_addr == LPC82X_USART0_BASE)) {
        
        return amhw_lpc82x_clk_usart_baseclkrate_get();
    } else if (base_addr == LPC82X_WWDT_BASE) {
        
        return amhw_lpc82x_clk_wdt_rate_get();
        
    } else if (base_addr == LPC82X_WKT_BASE) {
        
        return amhw_lpc82x_clk_irc_rate_get();
    } else {
        
        return amhw_lpc82x_clk_system_clkrate_get();
    }
}

/* end of file */
