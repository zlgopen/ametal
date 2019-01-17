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
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "math.h"


/**
 * \brief 获取系统PLL输入时钟频率
 */
uint32_t amhw_lpc84x_clk_pllin_rate_get (void)
{     
    uint32_t syspllin_rate;
    
    switch ((amhw_lpc84x_pllinclk_src_t) (LPC84X_SYSCON->syspllclksel &
             0x03)) {
          
    case AMHW_LPC84X_CLK_PLLIN_SRC_FRO:            /* 内部RC振荡器 */
        syspllin_rate = AMHW_LPC84X_CLK_FREQ_IRC;
        break;
    
    case AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_CLK:    /* 系统振荡器 */
        syspllin_rate = AMHW_LPC84X_CLK_FREQ_XTAL;
        break;
        
    case AMHW_LPC84X_CLK_PLLIN_SRC_WTD_CLK:        /* WDT时钟 */
    	syspllin_rate = amhw_lpc84x_clk_wdt_rate_get ();
        break;

    case AMHW_LPC84X_CLK_PLLIN_SRC_FRO_DIV:        /* WDT时钟 */
        syspllin_rate = AMHW_LPC84X_CLK_FREQ_IRC / 2;
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
uint32_t amhw_lpc84x_clk_pllout_rate_get (void)
{
    uint32_t syspllout_rate;
    
    syspllout_rate = amhw_lpc84x_clk_pllin_rate_get() *
                     ((LPC84X_SYSCON->syspllctrl & 0x01F) + 1);
    
    return syspllout_rate;
}

/**
 * \brief 得到 main 时钟频率
 */
uint32_t amhw_lpc84x_clk_main_clkrate_get (void)
{
    uint32_t clkrate = 0;

    switch (amhw_lpc84x_clk_main_src_get()) {
        
    case AMHW_LPC84X_MAIN_CLK_PLLIN_SRC_MAIN_CLK_PRE_PLL:
        /* 添加获取主时钟代码 */
    	clkrate = amhw_lpc84x_clk_fro_rate_get();
        break;

    case AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_SYS_PLL:
        clkrate = amhw_lpc84x_clk_pllout_rate_get();
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
uint32_t amhw_lpc84x_clk_wdt_rate_get (void)
{
    am_bool_t   wdtosc_ispowerup;
    uint32_t g_wdtosc_clkfreq;
    
    wdtosc_ispowerup = amhw_lpc84x_syscon_power_check(AMHW_LPC84X_SYSCON_PD_WDT_OSC);
    
     
    if (wdtosc_ispowerup == AM_FALSE) {

        /* 打开wdt振荡器*/
        amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_WDT_OSC);
    }
    
    /* 得到时钟频率*/
    switch ((LPC84X_SYSCON->wdtoscctrl >> 5) & 0xF) {
        
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
    
    g_wdtosc_clkfreq /= (((LPC84X_SYSCON->wdtoscctrl & 0x1F) + 1) << 1);

    return g_wdtosc_clkfreq;
}


/**
 * \brief 得到 outclk时钟频率
 */
uint32_t amhw_lpc84x_outclk_clkrate_get (void)
{
    uint32_t clkrate = 0;

    switch (amhw_lpc84x_outclk_src_get()) {
    case AMHW_LPC84X_CLKOUT_SRC_FRO:
    	clkrate = amhw_lpc84x_clk_fro_rate_get();
        break;
    case AMHW_LPC84X_CLKOUT_SRC_MAIN:
    	clkrate = amhw_lpc84x_clk_main_clkrate_get ();
    	break;
    case AMHW_LPC84X_CLKOUT_SRC_SYS_LPP0_CLK:
    	clkrate = amhw_lpc84x_clk_pllout_rate_get ();
    	break;
    case AMHW_LPC84X_CLKOUT_SRC_FRO_DIV:
    	clkrate = amhw_lpc84x_clk_fro_rate_get() / 2;
    	break;
    case AMHW_LPC84X_CLKOUT_SRC_FRO_WDT_OSC_CLK:
    	clkrate = amhw_lpc84x_clk_wdt_rate_get();
    	break;
    default:
        clkrate = 0;
        break;
    }
    return clkrate;
}

/**
 * \brief 得到系统时钟频率
 */
uint32_t amhw_lpc84x_clk_system_clkrate_get (void)
{   
    /* sysahbclkdiv 为0时，系统时钟关闭，复位后sysahbclkdiv默认为 1*/
    return (amhw_lpc84x_clk_main_clkrate_get() /
            amhw_lpc84x_clk_system_clkdiv_get());
}

/**
 * \brief 获取ADC基本输入频率
 */
uint32_t amhw_lpc84x_clk_adc_baseclkrate_get (void)
{
	uint8_t adc_clk_sel ;

	adc_clk_sel = LPC84X_SYSCON->adcclksel;
	if (adc_clk_sel == AMHW_LPC84X_CLK_ADC_FRO) {
		return amhw_lpc84x_clk_fro_rate_get() / LPC84X_SYSCON->adcclkdiv;
	} else if (adc_clk_sel == AMHW_LPC84X_CLK_ADC_SYS_PLL0_CLK) {
		return amhw_lpc84x_clk_pllout_rate_get() / LPC84X_SYSCON->adcclkdiv;
	} else {
		return 0;
	}
}

/**
 * \brief 获取SCT基本输入频率
 */
uint32_t amhw_lpc84x_clk_sct_baseclkrate_get (void)
{
	uint8_t adc_clk_sel ;

	adc_clk_sel = LPC84X_SYSCON->sctclksel;
	if (adc_clk_sel == AMHW_LPC84X_CLK_SCT_FRO) {
		return amhw_lpc84x_clk_fro_rate_get() / (LPC84X_SYSCON->sctclkdiv & 0xFF);
	} else if (adc_clk_sel == AMHW_LPC84X_CLK_SCT_MAIN_CLK) {
		return amhw_lpc84x_clk_main_clkrate_get() / (LPC84X_SYSCON->sctclkdiv & 0xFF);
	} else if (adc_clk_sel == AMHW_LPC84X_CLK_SCT_SYS_PLL0_CLK) {
		return amhw_lpc84x_clk_pllout_rate_get() / (LPC84X_SYSCON->sctclkdiv & 0xFF);
	} else {
		return 0;
	}
}

/**
 * \brief 获取串口SPIh和I2C基本输入频率
 */
uint32_t amhw_lpc84x_clk_usart_i2c_spi_baseclkrate_get (int id)
{
    uint64_t inclk;
    uint8_t  clk_sel = 0xf;
    uint32_t mult, div, infrg0;


    /* 得到选择的时钟源*/
    if (id == CLK_UART0) {
    	clk_sel = LPC84X_SYSCON->uart0clksel & 0x00000007;
    } else if (id == CLK_UART1) {
    	clk_sel = LPC84X_SYSCON->uart1clksel & 0x00000007;
    } else if (id == CLK_UART2) {
    	clk_sel = LPC84X_SYSCON->uart2clksel & 0x00000007;
    } else if (id == CLK_UART3) {
    	clk_sel = LPC84X_SYSCON->uart3clksel & 0x00000007;
    } else if (id == CLK_UART4) {
    	clk_sel = LPC84X_SYSCON->uart4clksel & 0x00000007;
    } else if (id == CLK_I2C0) {
    	clk_sel = LPC84X_SYSCON->i2c0clksel & 0x00000007;
    } else if (id == CLK_I2C1) {
    	clk_sel = LPC84X_SYSCON->i2c1clksel & 0x00000007;
    } else if (id == CLK_I2C2) {
    	clk_sel = LPC84X_SYSCON->i2c2clksel & 0x00000007;
    } else if (id == CLK_I2C3) {
    	clk_sel = LPC84X_SYSCON->i2c3clksel & 0x00000007;
    } else if (id == CLK_SPI0) {
    	clk_sel = LPC84X_SYSCON->spi0clksel & 0x00000007;
    } else if (id == CLK_SPI1) {
    	clk_sel = LPC84X_SYSCON->spi1clksel & 0x00000007;
    }

    switch (clk_sel) {
    case DEV_FRO:
	    inclk = amhw_lpc84x_clk_fro_rate_get();
        break;
    case DEV_MAIN:
	    inclk = amhw_lpc84x_clk_main_clkrate_get();
        break;
    case DEV_FRG0:
 	   /* 得到FRG0的分频值*/

 	   /* 得到FRG0的时钟源*/
 	   infrg0 = LPC84X_SYSCON->frg0clksel & 0x00000003;

 	   if (infrg0 == FRG_FRO) {
 	 	  inclk = amhw_lpc84x_clk_fro_rate_get() ;
 	   }

 	   if (infrg0 == FRG_MAIN_CLK) {
 	       inclk = amhw_lpc84x_clk_main_clkrate_get();
 	   }
 	   if (infrg0 == FRG_SYS_PLL_CLK) {
 	  	  inclk = amhw_lpc84x_clk_pllout_rate_get() ;
 	   }

 	   if ((LPC84X_SYSCON->frg0mult & 0xFF) != 0x0) {

 	 	  div    = LPC84X_SYSCON->frg0div;
 	      /* 小数部分使能，得到倍频系数*/
 	      mult = LPC84X_SYSCON->frg0mult & 0xFF;

 	      inclk = inclk / (1 + ((float)mult / (div + 1)));
 	   }
        break;
    case DEV_FRG1:

 	   /* 得到FRG0的时钟源*/
 	   infrg0 = LPC84X_SYSCON->frg1clksel & 0x00000003;

 	   if (infrg0 == FRG_FRO) {
 	 	  inclk = amhw_lpc84x_clk_fro_rate_get() ;
 	   }

 	   if (infrg0 == FRG_MAIN_CLK) {
 	       inclk = amhw_lpc84x_clk_main_clkrate_get() ;
 	   }
 	   if (infrg0 == FRG_SYS_PLL_CLK) {
 	  	  inclk = amhw_lpc84x_clk_pllout_rate_get() ;
 	   }
 	   if ((LPC84X_SYSCON->frg1mult & 0xFF) == 0xFF) {

  	 	  div    = LPC84X_SYSCON->frg1div;
  	      /* 小数部分使能，得到倍频系数*/
  	      mult = LPC84X_SYSCON->frg1mult & 0xFF;

  	      inclk = inclk / (1 + ((float)mult / (div + 1)));
 	   }
        break;
    case DEV_FRO_DIV:
    	inclk = amhw_lpc84x_clk_fro_rate_get() / 2;
    	break;
    default:
    	inclk = 0;
   }
      return (uint32_t)inclk;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_uart0_clk_sel_set (uint32_t uart_clk_sel)
{
	LPC84X_SYSCON->uart0clksel = uart_clk_sel;
}

/**
 * \brief 设置UART1时钟输入源
 */
void amhw_lpc84x_clk_uart1_clk_sel_set (uint32_t uart_clk_sel)
{
	LPC84X_SYSCON->uart1clksel = uart_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_uart2_clk_sel_set (uint32_t uart_clk_sel)
{
	LPC84X_SYSCON->uart2clksel = uart_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_uart3_clk_sel_set (uint32_t uart_clk_sel)
{
	LPC84X_SYSCON->uart3clksel = uart_clk_sel;
}


/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_uart4_clk_sel_set (uint32_t uart_clk_sel)
{
	LPC84X_SYSCON->uart4clksel = uart_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_i2c0_clk_sel_set (uint32_t i2c_clk_sel)
{
	LPC84X_SYSCON->i2c0clksel = i2c_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_i2c1_clk_sel_set (uint32_t i2c_clk_sel)
{
	LPC84X_SYSCON->i2c1clksel = i2c_clk_sel;
}
/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_i2c2_clk_sel_set (uint32_t i2c_clk_sel)
{
	LPC84X_SYSCON->i2c2clksel = i2c_clk_sel;
}
/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_i2c3_clk_sel_set (uint32_t i2c_clk_sel)
{
	LPC84X_SYSCON->i2c3clksel = i2c_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_spi0_clk_sel_set (uint32_t spi_clk_sel)
{
	LPC84X_SYSCON->spi0clksel = spi_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_spi1_clk_sel_set (uint32_t spi_clk_sel)
{
	LPC84X_SYSCON->spi1clksel = spi_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_clk_cap_clk_sel_set (uint32_t cap_clk_sel)
{
	LPC84X_SYSCON->captclksel = cap_clk_sel;
}

/**
 * \brief 设置UART0时钟输入源
 */
void amhw_lpc84x_sct_clk_sel_set (uint32_t sct_clk_sel, uint8_t sct_clk_div)
{
	LPC84X_SYSCON->sctclksel = sct_clk_sel;
	LPC84X_SYSCON->sctclkdiv = sct_clk_div;
}

void amhw_lpc84x_clk_adc_sel_set (uint32_t adc_clk_sel, uint8_t adc_clk_div)
{
	LPC84X_SYSCON->adcclksel = adc_clk_sel;
	LPC84X_SYSCON->adcclkdiv = adc_clk_div;
}

/**
 * \brief 设置frg0基本输入频率
 */
int amhw_lpc84x_clk_frg0_baseclkrate_set (uint32_t rate)
{
    uint32_t inclk;
    uint32_t mult;
    uint32_t infrg0;

	   /* 得到FRG0的时钟源*/
	   infrg0 = LPC84X_SYSCON->frg0clksel & 0x00000003;
	   if (infrg0 == FRG_FRO) {
	 	  inclk = amhw_lpc84x_clk_fro_rate_get() ;
	   }
	   if (infrg0 == FRG_MAIN_CLK) {
	       inclk = amhw_lpc84x_clk_main_clkrate_get();
	   }
	   if (infrg0 == FRG_SYS_PLL_CLK) {
	  	  inclk = amhw_lpc84x_clk_pllout_rate_get() ;
	   }

    if (rate > inclk) {
        return -AM_EINVAL;
    }
    LPC84X_SYSCON->frg0div  = 0;

    /* 计算小数分频值 */
    mult = ((inclk / rate)) - 1;

       /* 保留小数部分 */
    LPC84X_SYSCON->frg0mult = (uint8_t)mult;
    return AM_OK;
}

/**
 * \brief 设置frg1基本输入频率
 */
int amhw_lpc84x_clk_frg1_baseclkrate_set (uint32_t rate)
{
    uint32_t inclk;
    uint32_t mult;
    uint32_t infrg0;

	   /* 得到FRG0的时钟源*/
	   infrg0 = LPC84X_SYSCON->frg1clksel & 0x00000003;
	   if (infrg0 == FRG_FRO) {
	 	  inclk = amhw_lpc84x_clk_fro_rate_get() ;
	   }
	   if (infrg0 == FRG_MAIN_CLK) {
	       inclk = amhw_lpc84x_clk_main_clkrate_get();
	   }
	   if (infrg0 == FRG_SYS_PLL_CLK) {
	  	  inclk = amhw_lpc84x_clk_pllout_rate_get() ;
	   }

    if (rate > inclk) {
        return -AM_EINVAL;
    }
    LPC84X_SYSCON->frg1div  = 0;

    /* 计算小数分频值 */
    mult = ((inclk / rate)) - 1;

       /* 保留小数部分 */
    LPC84X_SYSCON->frg1mult = (uint8_t)mult;
    return AM_OK;
}


///**
// * \brief 得到外设的输入频率
// */
//uint32_t amhw_lpc84x_clk_periph_freq_get (void *p_periph)
//{
//    uint32_t base_addr = (uint32_t)(p_periph);
//
//    if ((base_addr == LPC84X_USART0_BASE) ||
//        (base_addr == LPC84X_USART0_BASE) ||
//        (base_addr == LPC84X_USART0_BASE)) {
//
//        return amhw_lpc84x_clk_usart_baseclkrate_get();
//    } else if (base_addr == LPC84X_WWDT_BASE) {
//
//        return amhw_lpc84x_clk_wdt_rate_get();
//
//    } else if (base_addr == LPC84X_WKT_BASE) {
//
//        return amhw_lpc84x_clk_irc_rate_get();
//    } else {
//
//        return amhw_lpc84x_clk_system_clkrate_get();
//    }
//}

/* end of file */
