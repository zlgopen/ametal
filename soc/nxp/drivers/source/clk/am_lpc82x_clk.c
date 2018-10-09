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
 * - 1.00 15-12-01  lnk, first implementation.
 * \endinternal
 */
 
#include "ametal.h"
#include "am_lpc82x_clk.h"
#include "lpc82x_clk.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"

/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    uint32_t clk = 0;
    
    switch (clk_id) {
    
    /* 系统内部时钟 */
    case CLK_IRC:
        clk = amhw_lpc82x_clk_irc_rate_get();
        break;
    
    case CLK_XTAL:
        clk = amhw_lpc82x_clk_xtal_rate_get();
        break;
    
    case CLK_CLKIN:
        clk = amhw_lpc82x_clk_clkin_rate_get();
        break;
    
    case CLK_WDTOSC:
        clk = amhw_lpc82x_clk_wdt_rate_get();
        break;
    
    case CLK_PLLIN:
        clk = amhw_lpc82x_clk_pllin_rate_get();
        break;
    
    case CLK_PLLOUT:
        clk = amhw_lpc82x_clk_pllout_rate_get();
        break;
    
    case CLK_MAIN:
        clk = amhw_lpc82x_clk_main_clkrate_get();
        break;
    
    case CLK_SYSTEM:
        clk = amhw_lpc82x_clk_system_clkrate_get();
        break;
    
    case CLK_UART0:    /* UART 基本输入时钟*/
    case CLK_UART1:
    case CLK_UART2:
        clk = amhw_lpc82x_clk_usart_baseclkrate_get();
        break;

    case CLK_WWDT:    /* WWDT 时钟*/
        clk = amhw_lpc82x_clk_wdt_rate_get();
        break;
    
    case CLK_WKT:    /* WKT 时钟*/
        clk = amhw_lpc82x_clk_irc_rate_get();
        break;

    /* 使用系统时钟的外设 */
    case CLK_ROM:
    case CLK_SRAM0_1:
    case CLK_FLASHREG:
    case CLK_FLASH:
    case CLK_I2C0:
    case CLK_GPIO:
    case CLK_SWM:
    case CLK_SCT:
    case CLK_MRT:
    case CLK_SPI0:
    case CLK_SPI1:
    case CLK_CRC:
    case CLK_ACMP:
    case CLK_I2C1:
    case CLK_I2C2:
    case CLK_I2C3:
    case CLK_ADC0:
    case CLK_MTB:
    case CLK_DMA:
        clk =  amhw_lpc82x_clk_system_clkrate_get();
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
    
    amhw_lpc82x_clk_periph_enable((amhw_lpc82x_clk_periph_t) (clk_id >> 8));
    
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
        
    amhw_lpc82x_clk_periph_disable ((amhw_lpc82x_clk_periph_t) (clk_id >> 8));
    
    return AM_OK;
}

/**
 * \brief CLK 初始化
 */
int am_lpc82x_clk_init (am_lpc82x_clk_dev_t           *p_dev,
                        const am_lpc82x_clk_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;

    /* CLK平台初始化，配置时钟引脚 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 系统振荡器作为PLL时钟源 */
    if (p_devinfo->pllin_src == AMHW_LPC82X_CLK_PLLIN_SRC_SYSOSC) {

        /* 外部XTAL < 15MHZ */
        amhw_lpc82x_clk_pll_bypass_set(AM_FALSE, AM_FALSE);
        
        /* 使能系统 Osc */
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_OSC);
    }

    /* 外部管脚输入作为PLL时钟源   */
    if (p_devinfo->pllin_src == AMHW_LPC82X_CLK_PLLIN_SRC_CLKIN) {

        /* 外部时钟输入，关闭OSC  CLKIN < 15MHz */
        amhw_lpc82x_clk_pll_bypass_set(AM_TRUE, AM_FALSE);
    }

    /* 设置PLL时钟源 */
    amhw_lpc82x_clk_pll_src_set((amhw_lpc82x_pllinclk_src_t)
                                (p_devinfo->pllin_src ));

    if (p_devinfo->main_src == AMHW_LPC82X_CLK_MAIN_SRC_PLLOUT) { 

        /* PLL输出作为mail时钟源 */
        amhw_lpc82x_clk_pllctrl_set(p_devinfo->pll_msel, p_devinfo->pll_psel);
        
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_SYS_PLL);

        /* 等待PLL锁定  */
        while (!amhw_lpc82x_clk_pll_locked_chk());
    }
    
    
    if (p_devinfo->main_src == AMHW_LPC82X_CLK_MAIN_SRC_WDTOSC) {

        /* WDT_OSC作为mian时钟源 */
        amhw_lpc82x_clk_wdtoscc_cfg(p_devinfo->wdtosc_freq, 
                                    p_devinfo->wdtosc_div);
        amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_WDT_OSC);
    }

    /* 设置main时钟源 */
    amhw_lpc82x_clk_main_src_set((amhw_lpc82x_clk_main_src_t)
                                 (p_devinfo->main_src));
  
    /* 系统时钟分频 */
    amhw_lpc82x_clk_system_clkdiv_set(p_devinfo->sysclk_div);

    /* 更新主时钟频率 */
    p_dev->main_clkrate = amhw_lpc82x_clk_main_clkrate_get();

    /* 更新系统频率 */
    p_dev->system_clkrate = (p_dev->main_clkrate / 
                                (LPC82X_SYSCON->sysahbclkdiv & 0xFF));

    return AM_OK;

}

/**
 * \brief 设置时钟频率
 */
int am_lpc82x_clk_rate_set (am_clk_id_t clk_id, uint32_t rate)
{
    int stat;

    switch (clk_id) {

    /* UART 基本输入时钟*/
    case CLK_UART0:
    case CLK_UART1:
    case CLK_UART2:
        stat = amhw_lpc82x_clk_usart_baseclkrate_set(rate);
        break;

    /* 时钟ID不支持 */
    default :
        stat = -AM_ENXIO;
        break;
    }

    return stat;
}

/* end of file */
