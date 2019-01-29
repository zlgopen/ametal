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
 * \brief CLK implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 19-01-02  lnk, first implementation.
 * \endinternal
 */
 
#include <lpc84x_clk.h>
#include "ametal.h"
#include "am_lpc84x_clk.h"
#include "hw/amhw_lpc84x_clk.h"
#include "hw/amhw_lpc84x_syscon.h"

/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    uint32_t clk = 0;
    
    switch (clk_id) {
    
    /* 系统内部时钟 */
    case CLK_FRO:
        clk = amhw_lpc84x_clk_fro_rate_get();
        break;
    
    case CLK_XTAL:
        clk = amhw_lpc84x_clk_xtal_rate_get();
        break;
    
    case CLK_CLKIN:
        clk = amhw_lpc84x_clk_clkin_rate_get();
        break;
    
    case CLK_WDTOSC:
        clk = amhw_lpc84x_clk_wdt_rate_get();
        break;
    
    case CLK_PLLIN:
        clk = amhw_lpc84x_clk_pllin_rate_get();
        break;
    
    case CLK_PLLOUT:
        clk = amhw_lpc84x_clk_pllout_rate_get();
        break;
    
    case CLK_MAIN:
        clk = amhw_lpc84x_clk_main_clkrate_get();
        break;
    
    case CLK_SYSTEM:
        clk = amhw_lpc84x_clk_system_clkrate_get();
        break;
    
    case CLK_UART0:    /* UART 基本输入时钟*/
    case CLK_UART1:
    case CLK_UART2:
    case CLK_UART3:
    case CLK_UART4:
        clk = amhw_lpc84x_clk_usart_i2c_spi_baseclkrate_get(clk_id);
        break;

    case CLK_I2C0:    /* I2C 基本输入时钟*/
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    	clk = amhw_lpc84x_clk_usart_i2c_spi_baseclkrate_get(clk_id);
    	break;

    case CLK_SPI0:    /* I2C 基本输入时钟*/
    case CLK_SPI1:
    	clk = amhw_lpc84x_clk_usart_i2c_spi_baseclkrate_get(clk_id);
    	break;

    case CLK_WWDT:    /* WWDT 时钟*/
        clk = amhw_lpc84x_clk_wdt_rate_get();
        break;
    
    case CLK_WKT:    /* WKT 时钟*/
        clk = amhw_lpc84x_clk_fro_rate_get();
        break;

    case CLK_ADC:
    	 clk = amhw_lpc84x_clk_adc_baseclkrate_get();
    	break;

    case CLK_SCT:
   	     clk = amhw_lpc84x_clk_sct_baseclkrate_get();
   	     break;

    /* 使用系统时钟的外设 */
    case CLK_ROM:
    case CLK_SRAM0_1:
    case CLK_FLASH:
    case CLK_GPIO:
    case CLK_SWM:
    case CLK_MRT:
    case CLK_CRC:
    case CLK_ACMP:
    case CLK_MTB:
    case CLK_DMA:
    case CLK_DAC0:
    case CLK_DAC1:
        clk =  amhw_lpc84x_clk_system_clkrate_get();
        break;
    
    default : 
        clk = 0;
        break;  
    }
    
    return clk;
}

/**
 * \brief 使能指定的系统或外设时钟
 */
int am_clk_enable (am_clk_id_t clk_id)
{
    if ((clk_id < CLK_PERIPH_MIN) ||
        (clk_id > CLK_PERIPH_MAX)) {

        /* 时钟ID不在范围内*/
        return -AM_ENXIO;
    }
    
    amhw_lpc84x_clk_periph_enable((amhw_lpc84x_clk_periph_t) (clk_id >> 8));
    
    return AM_OK;
}

/**
 * \brief 禁能指定的系统或外设时钟
 */
int am_clk_disable (am_clk_id_t clk_id)
{
    if ((clk_id < CLK_PERIPH_MIN) ||
        (clk_id > CLK_PERIPH_MAX)) {

        /* 时钟ID不在范围内*/
        return -AM_ENXIO;
    }
        
    amhw_lpc84x_clk_periph_disable ((amhw_lpc84x_clk_periph_t) (clk_id >> 8));
    
    return AM_OK;
}

#if 1
int am_lpc84x_clk_init (am_lpc84x_clk_dev_t           *p_dev,
                        const am_lpc84x_clk_devinfo_t *p_devinfo)
{
	uint32_t i;

	for (i = 1; i < 1; i++) __NOP();

	if (p_devinfo->frocillator == 0) {
		 LPC_PWRD_API->set_fro_frequency(18000);
	} else if (p_devinfo->frocillator == 1) {
		LPC_PWRD_API->set_fro_frequency(30000);
	} else if (p_devinfo->frocillator == 2) {
		LPC_PWRD_API->set_fro_frequency(24000);
	}

    /* CLK平台初始化，配置时钟引脚 */
    if (p_devinfo->pfn_plfm_init) {
    	p_devinfo->pfn_plfm_init();
    }

    /* 设置FRO时钟 */
    amhw_lpc84x_clk_fro_set(AMHW_LPC84X_SYSCON_PD_FRO);

	/* Wait for update to take effect */
	while (!(LPC84X_SYSCON->frodirectclkuen & 1)) __NOP();

	LPC84X_SYSCON->extclksel = p_devinfo->external_in_src;

	/* 使用看门狗时钟作为时钟源则初始化看门狗时钟 */
	if (p_devinfo->pllin_src == AMHW_LPC84X_CLK_PLLIN_SRC_WTD_CLK ) {

        /* WDT_OSC作为mian时钟源 */
        amhw_lpc84x_clk_wdtoscc_cfg(p_devinfo->wdtosc_freq,
                                    p_devinfo->wdtosc_div);
		amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_WDT_OSC);
		for (i = 0; i < 200; i++) __NOP();
	}

	/* 配置PLL */
    if (p_devinfo->main_src == AMHW_LPC84X_CLK_PLLIN_SRC_EXTERNAL_SYS_PLL ) {
    	LPC84X_SYSCON->pdruncfg       |= (AMHW_LPC84X_SYSCON_PD_SYS_PLL);
    	LPC84X_SYSCON->syspllclksel    = p_devinfo->pllin_src;
    	LPC84X_SYSCON->syspllclkuen    = 0;
    	LPC84X_SYSCON->syspllclkuen    = 1;
  	    while (!(LPC84X_SYSCON->syspllclkuen & 1)) __NOP();
    	amhw_lpc84x_clk_pllctrl_set(p_devinfo->pll_msel, p_devinfo->pll_psel);
  	    LPC84X_SYSCON->pdruncfg        &= ~(AMHW_LPC84X_SYSCON_PD_SYS_PLL);
  	    while (!(LPC84X_SYSCON->syspllstat & 1)) __NOP();
    }

    /* 切换主时钟源 */
    amhw_lpc84x_clk_mainclk_set( p_devinfo->main_src);

    /* 系统时钟分频值 */
    amhw_lpc84x_clk_main_div_set( p_devinfo->sysclk_div);

    /* Select main_clk as the source for FRG0 */
    LPC84X_SYSCON->frg0clksel = p_devinfo->frg0_src;

    LPC84X_SYSCON->frg1clksel = p_devinfo->frg1_src;

	while (!(LPC84X_SYSCON->mainclkplluen & 1)) __NOP();

	return AM_OK;
}
#endif

/**
 * \brief 通过设备基地址得到外设的输入频率
 */
uint32_t amhw_lpc84x_clk_periph_freq_get (void *p_periph)
{
    uint32_t base_addr = (uint32_t)(p_periph);
    if ((base_addr == LPC84X_USART0_BASE)) {
    	return am_clk_rate_get (CLK_UART0);
    } else if (base_addr == LPC84X_USART1_BASE) {
    	return am_clk_rate_get (CLK_UART1);
    } else if (base_addr == LPC84X_USART2_BASE) {
    	return am_clk_rate_get (CLK_UART2);
    } else if (base_addr == LPC84X_USART3_BASE) {
    	return am_clk_rate_get (CLK_UART3);
    } else if (base_addr == LPC84X_USART4_BASE) {
    	return am_clk_rate_get (CLK_UART4);
    } else if (base_addr == LPC84X_I2C0_BASE) {
    	return am_clk_rate_get (CLK_I2C0);
    } else if (base_addr == LPC84X_I2C1_BASE) {
    	return am_clk_rate_get (CLK_I2C1);
    } else if (base_addr == LPC84X_I2C2_BASE) {
    	return am_clk_rate_get (CLK_I2C2);
    } else if (base_addr == LPC84X_I2C3_BASE) {
    	return am_clk_rate_get (CLK_I2C3);
    } else if (base_addr == LPC84X_SPI0_BASE) {
    	return am_clk_rate_get (CLK_SPI0);
    } else if (base_addr == LPC84X_SPI1_BASE) {
    	return am_clk_rate_get (CLK_SPI1);
    } else if (base_addr == LPC84X_WWDT_BASE) {

        return amhw_lpc84x_clk_wdt_rate_get();

    } else if (base_addr == LPC84X_WKT_BASE) {

        return amhw_lpc84x_clk_fro_rate_get();
    } else {

        return amhw_lpc84x_clk_system_clkrate_get();
    }
}

/* end of file */
