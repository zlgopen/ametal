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
 * \internal
 * \par Modification history
 * - 1.00 19-09-05  zp, first implementation
 * \endinternal
 */

#include "am_zlg118_clk.h"
#include "ametal.h"
#include "am_zlg118_flash.h"
#include "zlg118_periph_map.h"
#include "hw/amhw_zlg118_rcc.h"
#include "hw/amhw_zlg118_rcc_reset.h"
#include "zlg118_clk.h"
#include "hw/amhw_zlg118_lpmode.h"
#include "am_zlg118_lpmode.h"


am_zlg118_clk_dev_t    *__gp_clk_dev;
am_zlg118_lpmode_dev_t  __g_lpmode_dev;

static void __rcc_unlock (void)
{
    amhw_zlg118_rcc_set_start(0x5A5A);
    amhw_zlg118_rcc_set_start(0xA5A5);
}

/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    int clk = 0;
    int peri, num;

    num  = clk_id >> CLK_PCLK;
    peri = clk_id & 0xff;

    switch (num) {

        /* 系统初始相关 时钟 */
        case 0:

            switch(peri) {

                /* XTH外部高速时钟 */
                case 0:
                    clk = __gp_clk_dev->p_devinfo->xth_osc;
                    break;

                /* XTL外部低速时钟 */
                case 1:
                    clk = __gp_clk_dev->p_devinfo->xtl_osc;
                    break;

                /* RCH内部高速时钟*/
                case 2:
                    /* 判断内部低速时钟是否使能，已使能*/
                    if(amhw_zlg118_rcc_rch_enable_get()) {

                        if(amhw_zlg118_rcc_rch_trim_get() ==
                           (*((uint16_t*)0x00100C00))) {
                            clk = 24000000;
                        } else if(amhw_zlg118_rcc_rch_trim_get() ==
                                  (*((uint16_t*)0x00100C02))) {
                            clk = 22120000;
                        } else if(amhw_zlg118_rcc_rch_trim_get() ==
                                  (*((uint16_t*)0x00100C04))) {
                            clk = 16000000;
                        } else if(amhw_zlg118_rcc_rch_trim_get() ==
                                  (*((uint16_t*)0x00100C06))) {
                            clk = 8000000;
                        } else if(amhw_zlg118_rcc_rch_trim_get() ==
                                  (*((uint16_t*)0x00100C08))) {
                            clk = 4000000;
                        }
                    } else {
                        clk = 0;
                    }
                    break;

                /* RCL内部低速时钟*/
                case 3:

                    /* 判断内部低速时钟是否使能，已使能*/
                    if(amhw_zlg118_rcc_rcl_enable_get()) {

                        if(amhw_zlg118_rcc_rcl_trim_get() ==
                           (*((uint16_t*)0x00100C20))) {
                            clk = 38400;
                        } else {
                            clk = 32768;
                        }
                    } else {
                        clk = 0;
                    }
                    break;

                /* PLL输入时钟 */
                case 4:
                    clk = __gp_clk_dev->pllin_clk;
                    break;

                /* PLL输出时钟 */
                case 5:
                    clk = __gp_clk_dev->pllout_clk;
                    break;

                /* SYS时钟*/
                case 6:
                    clk = __gp_clk_dev->sys_clk;
                    break;

                /* HCLK主时钟*/
                case 7:
                    clk = __gp_clk_dev->hclk;
                    break;

                /* PCLK外设时钟*/
                case 8:
                    clk = __gp_clk_dev->pclk;
                    break;
                default:
                    clk = 0;
            }
            break;

        /* zlg118所有外设时钟均挂载在PCLK时钟下 */
        case 1:
            clk = __gp_clk_dev->pclk;
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
    int peri;

    peri = clk_id & 0xff;

    /* 判断外设时钟是否已经使能，否，使能时钟且复位时钟 */
    if(amhw_zlg118_rcc_peripheral_enable_check((amhw_zlg118_peripheral)peri) == 0) {
        amhw_zlg118_rcc_peripheral_enable((amhw_zlg118_peripheral)peri);
        am_zlg118_clk_reset(clk_id);
    }

    return AM_OK;
}

/**
 * \brief 禁能指定的系统或外设时钟
 */
int am_clk_disable (am_clk_id_t clk_id)
{
    int peri;

    peri = clk_id & 0xff;

    amhw_zlg118_rcc_peripheral_disable((amhw_zlg118_peripheral)peri);

    return AM_OK;
}

int am_zlg118_clk_reset (am_clk_id_t clk_id)
{
    int peri;

    peri = clk_id & 0xff;

    amhw_zlg118_rcc_reset((amhw_zlg118_rcc_reset_peripheral)peri);

    amhw_zlg118_rcc_reset_stop((amhw_zlg118_rcc_reset_peripheral)peri);

    return AM_OK;
}

int am_zlg118_clk_update (am_clk_id_t clk_id, int clk)
{
    switch (clk_id) {

    case CLK_PLLIN:
        __gp_clk_dev->pllin_clk = clk;
        break;

    case CLK_PLLOUT:
        __gp_clk_dev->pllout_clk = clk;
        break;

    case CLK_SYSCLK:
        __gp_clk_dev->sys_clk = clk;
        break;

    case CLK_HCLK:
        __gp_clk_dev->hclk = clk;
        break;

    case CLK_PCLK:
        __gp_clk_dev->pclk = clk;
        break;

    default:
        break;
    }

    return AM_OK;
}

const am_zlg118_clk_dev_t *am_zlg118_clk_devinfo_get (void)
{
    if (__gp_clk_dev == NULL)  {
        return NULL;
    }

    return __gp_clk_dev;
}

int am_zlg118_div_get (am_clk_id_t clk_id)
{
    int div = 0;

    if (__gp_clk_dev == NULL)  {
        return -AM_EINVAL;
    }

    switch (clk_id) {

    case CLK_HCLK:
        div =  __gp_clk_dev->p_devinfo->hclk_div;
        break;

    case CLK_PCLK:
        div =  __gp_clk_dev->p_devinfo->pclk_div;
        break;

    default:
        break;
    }

    return div;
}

static void __rcl_enable (amhw_zlg118_rcl_fre_t rcl_clk, uint32_t *sysclk)
{
    if(rcl_clk == AMHW_ZLG118_RCl_FRE_32768Hz) {

        /* 设置RCL时钟频率校准值 （32.768KHz）*/
        amhw_zlg118_rcc_rcl_trim_set(*((uint16_t*)rcl_clk));
        *sysclk = 32768;
    } else {

        /* 设置RCL时钟频率校准值 （38.4KHz）*/
        amhw_zlg118_rcc_rcl_trim_set(*((uint16_t*)rcl_clk));
        *sysclk = 38400;
    }

    amhw_zlg118_rcc_rcl_waittime_set(AMHW_ZLG118_RCL_WAITTIME_256);

    __rcc_unlock();
    amhw_zlg118_rcc_rcl_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_rcl_state_get() == AM_FALSE);
}

static void __rch_enable (amhw_zlg118_rch_fre_t rch_clk, uint32_t *sysclk)
{
    uint16_t temp      = amhw_zlg118_rcc_rch_trim_get();
    uint32_t trim_addr = 0x00100C08;
    uint32_t trim_set  = 0;
    uint8_t  setnum    = 0;
    uint8_t  i         = 0;

    /* 获取当前存放的频率校准值 */
    if(temp == (*((uint16_t*)AMHW_ZLG118_RCH_FRE_24MHz))) {
        trim_addr = 0x00100C00;
    } else if(temp == (*((uint16_t*)AMHW_ZLG118_RCH_FRE_22MHz))) {
        trim_addr = 0x00100C02;
    } else if(temp == (*((uint16_t*)AMHW_ZLG118_RCH_FRE_16MHz))) {
        trim_addr = 0x00100C04;
    } else if(temp == (*((uint16_t*)AMHW_ZLG118_RCH_FRE_8MHz))) {
        trim_addr = 0x00100C06;
    } else if(temp == (*((uint16_t*)AMHW_ZLG118_RCH_FRE_4MHz))) {
        trim_addr = 0x00100C08;
    }

    /* 设置校准频率 */
    trim_set = (uint32_t)rch_clk;
    if(trim_addr == trim_set) {
        amhw_zlg118_rcc_rch_trim_set(*((volatile uint16_t*)trim_set));
    } else if(trim_addr > trim_set){

        setnum   = (trim_addr - trim_set) >> 1;
        trim_set = trim_addr;

        for(i = 0; i < setnum; i++) {
            trim_set = trim_set - 2;
            amhw_zlg118_rcc_rch_trim_set(*((volatile uint16_t*)trim_set));
        }
    } else {

        setnum   = (trim_set - trim_addr) >> 1;
        trim_set = trim_addr;

        for(i = 0; i < setnum; i++) {
            trim_set = trim_set + 2;
            amhw_zlg118_rcc_rch_trim_set(*((uint16_t*)trim_set));
        }
    }

    switch(rch_clk) {
    case AMHW_ZLG118_RCH_FRE_24MHz:
        *sysclk = 24000000;
        break;
    case AMHW_ZLG118_RCH_FRE_22MHz:
        *sysclk = 22120000;
        break;
    case AMHW_ZLG118_RCH_FRE_16MHz:
        *sysclk = 16000000;
        break;
    case AMHW_ZLG118_RCH_FRE_8MHz:
        *sysclk = 8000000;
        break;
    case AMHW_ZLG118_RCH_FRE_4MHz:
        *sysclk = 4000000;
        break;
    }

    __rcc_unlock();
    amhw_zlg118_rcc_rch_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_rch_state_get() == AM_FALSE);
}

static void __xth_enable (uint32_t xth_clk, uint32_t *sysclk)
{
    /* 外部晶振驱动能力选择 */
    amhw_zlg118_rcc_xth_xtal_driver_set(AMHW_ZLG118_XTH_XTAL_DRIVER_DEFAULT);

    /* 外部晶振工作频率选择 */
    if((xth_clk >= 4000000) && (xth_clk < 6000000)) {
        amhw_zlg118_rcc_xth_xtal_fre_set(AMHW_ZLG118_XTH_XTAL_FRE_4_6);
    } else if((xth_clk >= 6000000) && (xth_clk < 12000000)) {
        amhw_zlg118_rcc_xth_xtal_fre_set(AMHW_ZLG118_XTH_XTAL_FRE_6_12);
    } else if((xth_clk >= 12000000) && (xth_clk < 20000000)) {
        amhw_zlg118_rcc_xth_xtal_fre_set(AMHW_ZLG118_XTH_XTAL_FRE_12_20);
    } else {
        amhw_zlg118_rcc_xth_xtal_fre_set(AMHW_ZLG118_XTH_XTAL_FRE_20_32);
    }

    /* 外部高速时钟XTH稳定时间选择 */
    amhw_zlg118_rcc_xth_waittime_set(AMHW_ZLG118_XTH_WAITTIME_16384);

    __rcc_unlock();

    amhw_zlg118_rcc_xth_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_xth_state_get() == AM_FALSE);

    *sysclk = xth_clk;
}

static void __xtl_enable (uint32_t xtl_clk, uint32_t *sysclk)
{

    /* 外部晶振驱动能力选择 */
    amhw_zlg118_rcc_xtl_xtal_driver_set(AMHW_ZLG118_XTL_XTAL_DRIVER_DEFAULT);

    /* XTL晶振振荡幅度的调整 */
    amhw_zlg118_rcc_xtl_xtal_amplitude_set(AMHW_ZLG118_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

    /* 外部高速时钟XTL稳定时间选择 */
    amhw_zlg118_rcc_xtl_waittime_set(AMHW_ZLG118_XTL_WAITTIME_16384);

    __rcc_unlock();

    amhw_zlg118_rcc_xtl_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_xtl_state_get() == AM_FALSE);

    *sysclk = xtl_clk;
}

static void __pll_enable (am_zlg118_clk_dev_t           *p_dev,
                          const am_zlg118_clk_devinfo_t *p_devinfo)
{
    uint32_t temp;

    /* PLL输入时钟选择 */
    if(p_devinfo->pllin_src == AMHW_ZLG118_PLL_INPUT_FRE_SRC_XTH_PF00) {
        amhw_zlg118_rcc_pll_input_fre_src_set(AMHW_ZLG118_PLL_INPUT_FRE_SRC_XTH_PF00);
        __xth_enable(p_devinfo->xth_osc, &temp);

    } else if(p_devinfo->pllin_src == AMHW_ZLG118_PLL_INPUT_FRE_SRC_XTH_XTAL) {
        amhw_zlg118_rcc_pll_input_fre_src_set(AMHW_ZLG118_PLL_INPUT_FRE_SRC_XTH_XTAL);
        __xth_enable(p_devinfo->xth_osc, &temp);
    } else {
        amhw_zlg118_rcc_pll_input_fre_src_set(AMHW_ZLG118_PLL_INPUT_FRE_SRC_RCH);
        __rch_enable(p_devinfo->rch_osc, &temp);
    }

    p_dev->pllin_clk  = temp;

    p_dev->pllout_clk = temp * p_devinfo->pll_mul;

    p_dev->sys_clk    = p_dev->pllout_clk;

    /* PLL输入频率范围选择 */
    if((p_dev->pllin_clk >= 4000000) && (p_dev->pllin_clk < 6000000)) {
        amhw_zlg118_rcc_pll_input_fre_set(AMHW_ZLG118_PLL_INPUT_FRE_4_6);
    } else if((p_dev->pllin_clk >= 6000000) &&
              (p_dev->pllin_clk < 12000000)) {
        amhw_zlg118_rcc_pll_input_fre_set(AMHW_ZLG118_PLL_INPUT_FRE_6_12);
    } else if((p_dev->pllin_clk >= 12000000) &&
              (p_dev->pllin_clk < 20000000)) {
        amhw_zlg118_rcc_pll_input_fre_set(AMHW_ZLG118_PLL_INPUT_FRE_12_20);
    } else {
        amhw_zlg118_rcc_pll_input_fre_set(AMHW_ZLG118_PLL_INPUT_FRE_20_24);
    }

    /* PLL输出频率范围选择 */
    if((p_dev->pllout_clk >= 8000000) && (p_dev->pllout_clk < 12000000)) {
        amhw_zlg118_rcc_pll_output_fre_set(AMHW_ZLG118_PLL_OUTPUT_FRE_8_12);
    } else if((p_dev->pllout_clk >= 12000000) &&
              (p_dev->pllout_clk < 18000000)) {
        amhw_zlg118_rcc_pll_output_fre_set(AMHW_ZLG118_PLL_OUTPUT_FRE_12_18);
    } else if((p_dev->pllout_clk >= 18000000) &&
              (p_dev->pllout_clk < 24000000)) {
        amhw_zlg118_rcc_pll_output_fre_set(AMHW_ZLG118_PLL_OUTPUT_FRE_18_24);
    } else if((p_dev->pllout_clk >= 24000000) &&
              (p_dev->pllout_clk < 36000000)) {
        amhw_zlg118_rcc_pll_output_fre_set(AMHW_ZLG118_PLL_OUTPUT_FRE_24_36);
    } else {
        amhw_zlg118_rcc_pll_output_fre_set(AMHW_ZLG118_PLL_OUTPUT_FRE_36_48);
    }

    /* PLL倍频系数选择 */
    amhw_zlg118_rcc_pll_divn_set(p_devinfo->pll_mul);

    /* PLL稳定时间选择 */
    amhw_zlg118_rcc_pll_waittime_set(AMHW_ZLG118_PLL_WAITTIME_16384);

    __rcc_unlock();
    amhw_zlg118_rcc_pll_enable();

    /* 等待稳定*/
    while(amhw_zlg118_rcc_pll_state_get() == AM_FALSE);
}

/**
 * \brief CLK 初始化
 */
int am_zlg118_clk_init (am_zlg118_clk_dev_t           *p_dev,
                        const am_zlg118_clk_devinfo_t *p_devinfo)

{
    uint8_t hclk_unit = 0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_clk_dev      = p_dev;


    if (p_devinfo->pllin_src == AMHW_ZLG118_PLL_INPUT_FRE_SRC_XTH_PF00) {

        /* CLK平台初始化，配置时钟引脚 */
        if (p_devinfo->pfn_plfm_init) {
            p_devinfo->pfn_plfm_init();
        }
    }

    switch(p_devinfo->sysclk_src) {

    case AMHW_ZLG118_SYSCLK_RCH:

        __rch_enable(p_devinfo->rch_osc,  &p_dev->sys_clk);
        break;

    case AMHW_ZLG118_SYSCLK_XTH:

        __xth_enable(p_devinfo->xth_osc, &p_dev->sys_clk);
        break;

    case AMHW_ZLG118_SYSCLK_RCL:

        __rcl_enable(p_devinfo->rcl_osc,  &p_dev->sys_clk);
        break;

    case AMHW_ZLG118_SYSCLK_XTL:

        __xtl_enable(p_devinfo->xtl_osc,  &p_dev->sys_clk);
        break;

    case AMHW_ZLG118_SYSCLK_PLL:

        __pll_enable(p_dev, p_devinfo);
        break;

    default :
        return AM_ERROR;
    }

    /* 更新系统时钟源类型 */
    p_dev->sys_type = p_devinfo->sysclk_src;

    if((p_dev->sys_clk) > 24000000) {
        am_zlg118_flash_waitcycle(ZLG118_FLASH,
                                  AMHW_ZLG118_FLASH_READ_WAITTIME_2);
    }

    __rcc_unlock();
    amhw_zlg118_rcc_sys_clk_set(p_devinfo->sysclk_src);

    if((p_dev->sys_clk) <= 24000000) {
            am_zlg118_flash_waitcycle(ZLG118_FLASH,
                                      AMHW_ZLG118_FLASH_READ_WAITTIME_1);
    }

    __rcc_unlock();
    amhw_zlg118_rcc_hclk_set((amhw_zlg118_hclk_src)p_devinfo->hclk_div);

    __rcc_unlock();
    amhw_zlg118_rcc_pclk_set((amhw_zlg118_pclk_src)p_devinfo->pclk_div);

    p_dev->hclk = p_dev->sys_clk / (0x1ul << p_devinfo->hclk_div);
    p_dev->pclk = p_dev->hclk / (0x1ul << p_devinfo->pclk_div);

    hclk_unit = p_dev->hclk / 4000000;

    am_clk_enable(CLK_FLASH);

    am_zlg118_flash_init(ZLG118_FLASH, hclk_unit, AM_TRUE);

    return AM_OK;
}

am_zlg118_lpmode_handle_t am_zlg118_lpmode_init (void)
{
    __g_lpmode_dev.lpmode_mode = AM_ZLG118_LPMODE_MODE_RUN;

    return &__g_lpmode_dev;
}

/**
 * \brief LPMODE模式的系统时钟配置
 *
 * \param[in] mode LPMODE 模式定义
 *
 * \retval  AM_OK      模式切换有效
 * \retval  AM_ERROR   模式切换错误
 * \retval -AM_EINVAL  式参数错误
 */
int am_zlg118_lpmode_clk_change (am_zlg118_lpmode_mode_t mode)
{
    switch(mode) {

    /* 直接重新初始化CLK配置 */
    case AM_ZLG118_LPMODE_MODE_RUN:
        __g_lpmode_dev.lpmode_mode = AM_ZLG118_LPMODE_MODE_RUN;
        amhw_zlg118_lpmode_sevonpend(AM_FALSE);
        amhw_zlg118_lpmode_sleepdeep(AM_FALSE);
        amhw_zlg118_lpmode_sleeponexit(AM_FALSE);
        if(__gp_clk_dev->sys_type == AMHW_ZLG118_SYSCLK_RCL) {
            am_zlg118_clk_init(__gp_clk_dev, __gp_clk_dev->p_devinfo);
            __rcc_unlock();
            amhw_zlg118_rcc_rcl_disable();
        }
        break;

    /* 直接重新初始化CLK配置,再配置睡眠模式 */
    case AM_ZLG118_LPMODE_MODE_SLEEP:
        __g_lpmode_dev.lpmode_mode = AM_ZLG118_LPMODE_MODE_SLEEP;
        amhw_zlg118_lpmode_sevonpend(AM_FALSE);
        amhw_zlg118_lpmode_sleepdeep(AM_FALSE);
        amhw_zlg118_lpmode_sleeponexit(AM_FALSE);
        if(__gp_clk_dev->sys_type == AMHW_ZLG118_SYSCLK_RCL) {
            am_zlg118_clk_init(__gp_clk_dev, __gp_clk_dev->p_devinfo);
            __rcc_unlock();
            amhw_zlg118_rcc_rcl_disable();
        }
        break;
    case AM_ZLG118_LPMODE_MODE_DEEPSLEEP:
        __g_lpmode_dev.lpmode_mode = AM_ZLG118_LPMODE_MODE_DEEPSLEEP;
        amhw_zlg118_lpmode_sevonpend(AM_FALSE);
        amhw_zlg118_lpmode_sleepdeep(AM_FALSE);
        amhw_zlg118_lpmode_sleeponexit(AM_FALSE);

        if((__gp_clk_dev->sys_type != AMHW_ZLG118_SYSCLK_RCL) ||
           (__gp_clk_dev->sys_type != AMHW_ZLG118_SYSCLK_XTL)) {


            __rcl_enable(AMHW_ZLG118_RCl_FRE_32768Hz,  &__gp_clk_dev->sys_clk);

            /* 更新系统时钟源类型 */
            __gp_clk_dev->sys_type = AMHW_ZLG118_SYSCLK_RCL;

            /* 切换时钟源 */
            __rcc_unlock();
            amhw_zlg118_rcc_sys_clk_set(AMHW_ZLG118_SYSCLK_RCL);

            __gp_clk_dev->hclk = __gp_clk_dev->sys_clk /
                                 (0x1ul << __gp_clk_dev->p_devinfo->hclk_div);
            __gp_clk_dev->pclk = __gp_clk_dev->hclk /
                                 (0x1ul << __gp_clk_dev->p_devinfo->pclk_div);
        }
        break;
    default:
        return -AM_EINVAL;
    }

    return AM_OK;
}

/**
 * \brief 获取 LPMODE 模式
 *
 * \param[in] 无
 *
 * \return am_zlg118_lpmode_mode_t 这一枚举类型的值
 */
am_zlg118_lpmode_mode_t am_zlg118_lpmode_mode_get (void)
{
    return __g_lpmode_dev.lpmode_mode;
}

/**
 * \brief 深度睡眠模式
 *
 * \param[in] stata  AM_TRUE  ： 当退出中断处理后，自动再次进入休眠；
 *                   AM_FALSE ： 唤醒后不再自动进入休眠
 *
 * \return 无
 */
void am_zlg118_lpmode_deepsleep (am_bool_t stata)
{
    amhw_zlg118_lpmode_sleepdeep(AM_TRUE);

    if(stata) {
        amhw_zlg118_lpmode_sleeponexit(AM_TRUE);
    } else {
        amhw_zlg118_lpmode_sleeponexit(AM_FALSE);
    }

    amhw_zlg118_lpmode_wfi();

    /* 中断导致的睡眠唤醒后，下一句程序应该从此处开始，切换回正常时钟，才能保证功能正常 */
    am_zlg118_lpmode_clk_change(AM_ZLG118_LPMODE_MODE_RUN);
}

/**
 * \brief 普通睡眠模式
 *
 * \param[in] stata  AM_TRUE  ： 当退出中断处理后，自动再次进入休眠；
 *                   AM_FALSE ： 唤醒后不再自动进入休眠
 *
 * \return 无
 */
void am_zlg118_lpmode_sleep (am_bool_t stata)
{
    amhw_zlg118_lpmode_sleepdeep(AM_FALSE);

    if(stata) {
        amhw_zlg118_lpmode_sleeponexit(AM_TRUE);
    } else {
        amhw_zlg118_lpmode_sleeponexit(AM_FALSE);
    }

    amhw_zlg118_lpmode_wfi();
}

/* end of file */
