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
 * \brief 定时器TIM驱动，PWM驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-16  nwt, first implementation
 * \endinternal
 */
#include "hc32_clk.h"
#include "am_hc32_tim_pwm.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 配置PWM */
static int __tim_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns);

/** \brief 使能PWM输出 */
static int __tim_pwm_enable (void *p_drv, int chan);

/** \brief 禁能PWM输出 */
static int __tim_pwm_disable (void *p_drv, int chan);

/** \brief PWM驱动函数 */
static const struct am_pwm_drv_funcs __g_tim_pwm_drv_funcs = {
    __tim_pwm_config,
    __tim_pwm_enable,
    __tim_pwm_disable,
};

/******************************************************************************/

/** \brief 配置PWM */
static int __tim_pwm_config (void          *p_drv,
                             int            chan,
                             unsigned long  duty_ns,
                             unsigned long  period_ns)
{
    am_hc32_tim_pwm_dev_t *p_dev    = (am_hc32_tim_pwm_dev_t *)p_drv;
    amhw_hc32_tim_t       *p_hw_tim = (amhw_hc32_tim_t *)p_dev->p_devinfo->tim_regbase;

    uint32_t clkfreq;
    uint16_t fre_div = 1;
    uint32_t period_c, duty_c, temp;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > 4294967295UL) ||
        (period_ns > 4294967295UL) || (duty_ns > period_ns)) {
        return -AM_EINVAL;
    }

    clkfreq  = am_clk_rate_get(CLK_PCLK);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

    /* 当计数小于65536时，不分频(值为1,1代表为1分频) */
    temp = period_c / 65536 + 1;

    /* 16位定时器需要运算取得合适的分频值 */
    for (fre_div = 1; fre_div < temp; ) {
        fre_div++;          /* 分频系数 */
    }

    /* 分频系数设置 */
    if(fre_div <= 1) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV1);
        fre_div = 1;
    } else if(fre_div <= 2) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV2);
        fre_div = 2;
    } else if(fre_div <= 4) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV4);
        fre_div = 4;
    } else if(fre_div <= 8) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV8);
        fre_div = 8;
    } else if(fre_div <= 16) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV16);
        fre_div = 16;
    } else if(fre_div <= 32) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV32);
        fre_div = 32;
    } else if(fre_div <= 64) {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV64);
        fre_div = 64;
    } else {
        amhw_hc32_tim_mode_clkdiv_set(p_hw_tim, AMHW_HC32_TIM_CLK_DIV256);
        fre_div = 256;
    }

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return -AM_EINVAL;
    }

    /* 设置自动重装寄存器的值 */
    amhw_hc32_tim_arr_count_set(p_hw_tim, period_c - 1);

    /* 清零计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 0);

    /* 互补PWM输出 */
    if(p_dev->p_devinfo->comp == 1) {

        /* 设置某一通道A的比较值 */
        amhw_hc32_tim_mode23_ccr_set(p_hw_tim, (chan / 2) * 2, duty_c - 1);

        /* 设置某一通道B的比较值 */
        amhw_hc32_tim_mode23_ccr_set(p_hw_tim, (chan / 2) * 2 + 1, duty_c - 1);

        /* 设置PWM输出极性设置 */
        amhw_hc32_tim_mode23_compare_set(
           p_hw_tim,
           (amhw_hc32_tim_compare_type_t)(((chan / 2) * 2) * 4),
           (amhw_hc32_tim_compare_t)p_dev->p_devinfo->ocpolarity);

        amhw_hc32_tim_mode23_compare_set(
            p_hw_tim,
            (amhw_hc32_tim_compare_type_t)((((chan / 2) * 2) + 1) * 4),
            (amhw_hc32_tim_compare_t)p_dev->p_devinfo->ocpolarity);

    /* 独立PWM输出*/
    } else {

        /* 设置某一通道的比较值 */
        amhw_hc32_tim_mode23_ccr_set(p_hw_tim, chan, duty_c - 1);

        /* 设置PWM输出模式为PWM1 */
        amhw_hc32_tim_mode23_compare_set(
            p_hw_tim,
            (amhw_hc32_tim_compare_type_t)(chan * 4),
            (amhw_hc32_tim_compare_t)p_dev->p_devinfo->ocpolarity);
    }

    /* 正常输出 */
    amhw_hc32_tim_mode23_phase_same(
        p_hw_tim,
        (amhw_hc32_tim_phase_type_t)(chan * 4 + 3));

    return AM_OK;
}

/**
 * \brief 使能PWM通道输出
 */
static int __tim_pwm_enable (void *p_drv, int chan)
{
    int i = 0, t = 0, t1 = 0, t2 = 0, enable_flag = 0;
    am_hc32_tim_pwm_dev_t    *p_dev    = (am_hc32_tim_pwm_dev_t *)p_drv;
    amhw_hc32_tim_t          *p_hw_tim = (amhw_hc32_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_hc32_tim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

    uint8_t comp_pin_flag = 0;

    /* 判断引脚列表中是否有对应通道配置信息 */
    for(i = 0; i <= p_dev->chan_max; i++){
        if((p_chaninfo[i].channel & 0x7f) ==  chan){

            /* 互补PWM输出 */
            if(p_dev->p_devinfo->comp == 1){

                t1 = (i / 2) * 2;
                t2 = t1 + 1;

                /* 查找A通道引脚号，并初始化 */
                for(t = 0; t <= p_dev->chan_max; t++) {
                    if( p_chaninfo[t].channel == t1) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].func);
                        comp_pin_flag |= (1 << 0);
                    }
                    if( p_chaninfo[t].channel == t2) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].func);
                        comp_pin_flag |= (1 << 1);
                    }

                    if(comp_pin_flag == 0x3) {
                        break;
                    }
                }

                /* 使能互补输出 */
                amhw_hc32_tim_mode23_enable(p_hw_tim, AMHW_HC32_TIM_COMP);

                enable_flag = 1;
                break;

            /* 独立PWM输出 */
            }else{

                am_gpio_pin_cfg(p_chaninfo[i].gpio, p_chaninfo[i].func);

                /* 禁能互补输出 */
                amhw_hc32_tim_mode23_disable(p_hw_tim, AMHW_HC32_TIM_COMP);

                enable_flag = 1;
                break;
            }
        }
    }

    /* 输入通道号无效 */
    if(enable_flag == 0){
        return -AM_EINVAL;
    }

    /* 清除标志位 */
    amhw_hc32_tim_mode23_int_flag_clr(p_hw_tim, AMHW_HC32_TIM_INT_FLAG_ALL);

    /* PWM功能使能 */
    amhw_hc32_tim_mode23_dtr_enable(p_hw_tim, AMHW_HC32_TIM_DTR_MOE);

    /* 使能定时器 */
    amhw_hc32_tim_enable(p_hw_tim);

    return AM_OK;
}

/**
 * \brief 禁能PWM通道输出
 */
static int __tim_pwm_disable (void *p_drv, int chan)
{
    int  i = 0, t = 0, t1 = 0, t2 = 0, disable_flag = 0;
    am_hc32_tim_pwm_dev_t      *p_dev    = (am_hc32_tim_pwm_dev_t *)p_drv;
    amhw_hc32_tim_t            *p_hw_tim = (amhw_hc32_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_hc32_tim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

    uint8_t comp_pin_flag = 0;

    /* 判断引脚列表中是否有对应通道配置信息 */
    for(i = 0; i <= p_dev->chan_max; i++){
        if((p_chaninfo[i].channel & 0x7f) ==  chan){

            /* 互补PWM输出 */
            if(p_dev->p_devinfo->comp == 1){

                t1 = (i / 2) * 2;
                t2 = t1 + 1;

                /* 查找A通道引脚号，并解初始化 */
                for(t = 0; t <= p_dev->chan_max; t++) {
                    if(p_chaninfo[t].channel == t1) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].dfunc);
                        comp_pin_flag |= (1 << 0);
                    }
                    if(p_chaninfo[t].channel == t2) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].dfunc);
                        comp_pin_flag |= (1 << 1);

                    }
                    if(comp_pin_flag == 0x3) {
                         break;
                    }
                }

                disable_flag = 1;
                break;

            /* 独立PWM输出 */
            } else {

                am_gpio_pin_cfg(p_chaninfo[i].gpio, p_chaninfo[i].dfunc);

                disable_flag = 1;
                break;
            }
        }
    }

    /* 输入通道号无效 */
    if(disable_flag == 0){
        return -AM_EINVAL;
    }

    /* 禁能定时器TIM允许计数 */
    amhw_hc32_tim_disable(p_hw_tim);

    /* 禁能通道PWM输出 */
    amhw_hc32_tim_mode23_dtr_disable(p_hw_tim, AMHW_HC32_TIM_DTR_MOE);

    return AM_OK;
}

/**
  * \brief pwm初始化
  */
void __tim_pwm_init (amhw_hc32_tim_t       *p_hw_tim,
                     am_hc32_tim_pwm_dev_t *p_dev,
                     amhw_hc32_tim_type_t   type)
{
    uint8_t i = 0;

    /* 设置定时器模式2 */
    amhw_hc32_tim_mode_set(p_hw_tim, AMHW_HC32_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_hc32_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为比较模式 （CH0A、CH1A、CH2A）*/
    for(i = 0; i < p_dev->chan_max;i = i + 2) {
        amhw_hc32_tim_mode23_csa_compare(p_hw_tim, i);
    }

    /* 设置通道为比较模式 （CH0B、CH1B、CH2B */
    for(i = 1; i < p_dev->chan_max;i = i + 2) {
        amhw_hc32_tim_mode23_csa_compare(p_hw_tim, i);
    }

    /* 定时器时钟为内部时钟 */
    amhw_hc32_tim_mode_clk_src_set(p_hw_tim,AMHW_HC32_TIM_CLK_SRC_TCLK);

    /* 清零计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 0);
}

/**
  * \brief tim pwm服务初始化
  */
am_pwm_handle_t am_hc32_tim_pwm_init (am_hc32_tim_pwm_dev_t           *p_dev,
                                        const am_hc32_tim_pwm_devinfo_t *p_devinfo)
{
    amhw_hc32_tim_t  *p_hw_tim = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    if(p_devinfo->tim_type == AMHW_HC32_TIM_TYPE_TIM3) {
        p_dev->chan_max = 6;
    } else {
        p_dev->chan_max = 2;
    }

    if(p_dev->chan_max < p_devinfo->channels_num) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo        = p_devinfo;
    p_hw_tim                = (amhw_hc32_tim_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_tim_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;

    __tim_pwm_init(p_hw_tim, p_dev, p_devinfo->tim_type);

    return &(p_dev->pwm_serv);
}

void am_hc32_tim_pwm_deinit (am_pwm_handle_t handle)
{

    am_hc32_tim_pwm_dev_t *p_dev    = (am_hc32_tim_pwm_dev_t *)handle;
    amhw_hc32_tim_t       *p_hw_tim = NULL;

    if (p_dev == NULL ) {
        return;
    }

    p_hw_tim   = (amhw_hc32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 清零计数器 */
    amhw_hc32_tim_cnt16_count_set(p_hw_tim, 0);

    /* 禁能定时器TIM允许计数 */
    amhw_hc32_tim_disable(p_hw_tim);

    p_dev->pwm_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
