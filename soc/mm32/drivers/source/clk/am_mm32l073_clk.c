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
 * \brief CLK implementation
 *
 * 驱动直接使用了 MM32L073 外设寄存器基地址， 因此，该驱动仅适用于 MM32L073
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-12  sdy, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32l073_clk.h"
#include "mm32l073_clk.h"
#include "mm32l073_periph_map.h"
#include "hw/amhw_mm32l073_rcc.h"
#include "hw/amhw_mm32_flash.h"

am_mm32l073_clk_dev_t *__gp_dev = NULL;

#define __HSI_CLK    48000000ul
#define __LSI_CLK    40000ul

static void __pllin_hse_init (void)
{
    amhw_mm32l073_rcc_hseon_enable();

    while (amhw_mm32l073_rcc_hserdy_read () == AM_FALSE);

//    amhw_mm32l073_rcc_hseforpll_set (0);

    __gp_dev->pllin_clk = __gp_dev->p_devinfo->hse_osc;
}

static void __pllin_hsi_init (void)
{
    amhw_mm32l073_rcc_hsion_enable();

    while (amhw_mm32l073_rcc_hsirdy_read() == AM_FALSE);

//    amhw_mm32l073_rcc_hsiten_enable();

    __gp_dev->pllin_clk = __HSI_CLK / 4;
}

static int __apb2_peri_clk (int peri)
{
    int clk = 0;

    switch (peri) {

    case AMHW_MM32L073_RCC_APB2_TIM1:

    case AMHW_MM32L073_RCC_APB2_TIM14:

    case AMHW_MM32L073_RCC_APB2_TIM16:

    case AMHW_MM32L073_RCC_APB2_TIM17:
        if (amhw_mm32l073_rcc_apb2_div_get() < 4) {
            clk = __gp_dev->apb2_clk;
        } else {
            clk = __gp_dev->apb2_clk << 1;
        }
        break;

    case AMHW_MM32L073_RCC_APB2_ADC1: /* ADC内部有分频器 */
        clk = __gp_dev->apb2_clk;
        break;

    default :
        clk = __gp_dev->apb2_clk;
        break;
    }

    return clk;
}

static int __apb1_peri_clk (int peri)
{
    int clk = 0;

    switch (peri) {

    case AMHW_MM32L073_RCC_APB1_TIM2:

    case AMHW_MM32L073_RCC_APB1_TIM3:
        if (amhw_mm32l073_rcc_apb1_div_get() < 4) {
            clk = __gp_dev->apb1_clk;
        } else {
            clk = __gp_dev->apb1_clk << 1;
        }
        break;

    case AMHW_MM32L073_RCC_APB1_USB:
        clk = __gp_dev->usb_clk;
        break;

    default :
        clk = __gp_dev->apb1_clk;
        break;
    }

    return clk;
}

static int __other_clk (am_clk_id_t clk_id)
{
    int clk = 0;

    switch (clk_id) {

    case CLK_PLLIN:
        clk = __gp_dev->pllin_clk;
        break;

    case CLK_PLLOUT:
        clk = __gp_dev->pllout_clk;
        break;

    case CLK_AHB:
        clk = __gp_dev->ahb_clk;
        break;

    case CLK_APB1:
        clk = __gp_dev->apb1_clk;
        break;

    case CLK_APB2:
        clk = __gp_dev->apb2_clk;
        break;

    case CLK_HSEOSC:
        clk = __gp_dev->p_devinfo->hse_osc;
        break;
    case CLK_LSI:

        if (amhw_mm32l073_rcc_lsirdy_read()) {
            clk = __LSI_CLK;
        } else {
            clk = 0;
        }

        break;

    case CLK_HSI:
        clk = __HSI_CLK;
        break;

    default :
        clk = 0;
        break;
    }

    return clk;
}

/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    int clk = 0;
    int peri, num;

    num  = (clk_id & 0xff00) >> 8;
    peri = clk_id & 0xff;

    switch (num) {

    case 1:
        clk = __apb1_peri_clk(peri);
        break;

    case 2:
        clk = __apb2_peri_clk(peri);
        break;

    case 3:
        clk = __gp_dev->ahb_clk;
        break;

    case 4:
        clk = __other_clk(clk_id);
        break;

    default :
        break;
    }
    return clk;
}

/**
 * \brief 使能指定的系统或外设时钟
 */
int am_clk_enable (am_clk_id_t clk_id)
{
    int peri, num;

    num  = (clk_id & 0xff00) >> 8;
    peri = clk_id & 0xff;

    switch (num) {

    case 1:
        amhw_mm32l073_rcc_apb1_enable ((amhw_mm32l073_apb1_peripheral)peri);
        break;

    case 2:
        amhw_mm32l073_rcc_apb2_enable ((amhw_mm32l073_apb2_peripheral)peri);
        break;

    case 3:
        amhw_mm32l073_rcc_ahb_enable ((amhw_mm32l073_ahb_peripheral)peri);
        break;

    default :
        break;
    }

    return AM_OK;
}

/**
 * \brief 禁能指定的系统或外设时钟
 */
int am_clk_disable (am_clk_id_t clk_id)
{
    int peri, num;

    num  = (clk_id & 0xff00) >> 8;
    peri = clk_id & 0xff;

    switch (num) {

    case 1:
        amhw_mm32l073_rcc_apb1_disable ((amhw_mm32l073_apb1_peripheral)peri);
        break;

    case 2:
        amhw_mm32l073_rcc_apb2_disable ((amhw_mm32l073_apb2_peripheral)peri);
        break;

    case 3:
        amhw_mm32l073_rcc_ahb_disable ((amhw_mm32l073_ahb_peripheral)peri);
        break;

    default :
            break;
    }

    return AM_OK;
}

int am_mm32l073_clk_reset (am_clk_id_t clk_id)
{
    int peri, num;

    num  = (clk_id & 0xff00) >> 8;
    peri = clk_id & 0xff;

    switch (num) {

    case 1:
        amhw_mm32l073_rcc_apb1_reset((amhw_mm32l073_apb1_peripheral)peri);
        amhw_mm32l073_rcc_apb1_reset_stop((amhw_mm32l073_apb1_peripheral)peri);
        break;

    case 2:
        amhw_mm32l073_rcc_apb2_reset((amhw_mm32l073_apb2_peripheral)peri);
        amhw_mm32l073_rcc_apb2_reset_stop((amhw_mm32l073_apb2_peripheral)peri);
        break;

    default :
            break;
    }

    return AM_OK;
}

int am_mm32l073_clk_update (am_clk_id_t clk_id, int clk)
{
    switch (clk_id) {

    case CLK_AHB:
        __gp_dev->ahb_clk = clk;
        break;

    case CLK_APB1:
        __gp_dev->apb1_clk = clk;
        break;

    case CLK_APB2:
        __gp_dev->apb2_clk = clk;
        break;

    default:
        break;
    }

    return AM_OK;
}

const am_mm32l073_clk_dev_t *am_mm32l073_clk_devinfo_get (void)
{
    if (__gp_dev == NULL)  {
        return NULL;
    }

    return __gp_dev;
}

int am_mm32l073_div_get (am_clk_id_t clk_id)
{
    int div = 0;

    if (__gp_dev == NULL)  {
        return -AM_EINVAL;
    }

    switch (clk_id) {

    case CLK_AHB:
        div =  __gp_dev->p_devinfo->ahb_div;
        break;

    case CLK_APB1:
        div =  __gp_dev->p_devinfo->apb1_div;
        break;

    case CLK_APB2:
        div =  __gp_dev->p_devinfo->apb2_div;
        break;

    case CLK_USB:
        div =  __gp_dev->p_devinfo->usb_div;
        break;

    default:
        break;
    }

    return div;
}

/**
 * \brief CLK 初始化
 */
int am_mm32l073_clk_init (am_mm32l073_clk_dev_t           *p_dev,
                        const am_mm32l073_clk_devinfo_t *p_devinfo)
{
    uint8_t i = 0,div = 1;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_dev          = p_dev;

    /* 使用内部hsi时钟 */
    amhw_mm32l073_rcc_hsion_enable();

    /* 等待其稳定 */
    while (amhw_mm32l073_rcc_hsirdy_read() == AM_FALSE);

    /* 复位SW, HPRE, PPRE1, PPRE2, and MCO bits */
    MM32L073_RCC->cfgr &= (uint32_t)0xF8FFC00C;

    /* 复位禁能 HSEON, CSSON and PLLON bits */
    MM32L073_RCC->cr &= (uint32_t)0xFEF6FFFF;

    if (p_devinfo->pllin_src == AMHW_MM32L073_PLLCLK_HSE) {

        /* CLK平台初始化，配置时钟引脚 */
        if (p_devinfo->pfn_plfm_init) {
            p_devinfo->pfn_plfm_init();
        }
        __pllin_hse_init();
    } else {
        __pllin_hsi_init();
    }

    p_dev->pllout_clk = p_dev->pllin_clk   *
                        p_devinfo->pll_mul /
                        p_devinfo->pll_div;

    /* 计算AHB时钟频率 */
    if (p_devinfo->ahb_div < 12) {
        for (i = 0; i < p_devinfo->ahb_div - 7; i++) {
            div = div << 1;
        }
    } else {
        div = 64;
        for (i = 0; i < (p_devinfo->ahb_div & 0xf) - 12; i++) {
            div = div << 1;
        }
    }
    p_dev->ahb_clk = p_dev->pllout_clk / div;
    p_dev->usb_clk = p_dev->pllout_clk / (div + 1);

    /* 计算APB1时钟频率 */
    div = 1;
    for (i = 0; i < (p_devinfo->apb1_div & 0x7); i++) {
        div = div << 1;
    }
    p_dev->apb1_clk = p_dev->ahb_clk / div;

    /* 计算APB2时钟频率 */
    div = 1;
    for (i = 0; i < (p_devinfo->apb2_div & 0x7); i++) {
        div = div << 1;
    }
    p_dev->apb2_clk = p_dev->ahb_clk / div;

    MM32L073_FLASH->acr |= 0x30;

    amhw_mm32_flash_latency_set(MM32L073_FLASH,
                                ((p_dev->pllout_clk - 1) / 24000000));

    amhw_mm32l073_rcc_ahb_div_set(p_devinfo->ahb_div);

    amhw_mm32l073_rcc_apb2_div_set(p_devinfo->apb2_div + 3);

    amhw_mm32l073_rcc_apb1_div_set(p_devinfo->apb1_div + 3);

    amhw_mm32l073_rcc_plldm_set(p_devinfo->pll_div - 1);
    amhw_mm32l073_rcc_plldn_set(p_devinfo->pll_mul - 1);

    amhw_mm32l073_rcc_pll_clk_set(p_devinfo->pllin_src);

    amhw_mm32l073_rcc_pll_enable();

    while (amhw_mm32l073_rcc_pllrdy_read() == AM_FALSE); /* 等待pll就绪 */

    amhw_mm32l073_rcc_sys_clk_set (AMHW_MM32L073_SYSCLK_PLL);

    amhw_mm32l073_rcc_pllforusb_set (p_devinfo->usb_div);

    MM32L073_RCC->apb2enr &= ~0xc;

    return AM_OK;
}

/* end of file */
