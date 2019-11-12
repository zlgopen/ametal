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
 * \brief 定时器ADTIM驱动，PWM驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-09  zp, first implementation
 * \endinternal
 */
#include "hc32_clk.h"
#include "am_hc32_adtim_pwm.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 配置PWM */
static int __adtim_pwm_config (void          *p_drv,
                               int            chan,
                               unsigned long  duty_ns,
                               unsigned long  period_ns);

/** \brief 使能PWM输出 */
static int __adtim_pwm_enable (void *p_drv, int chan);

/** \brief 禁能PWM输出 */
static int __adtim_pwm_disable (void *p_drv, int chan);

/** \brief PWM驱动函数 */
static const struct am_pwm_drv_funcs __g_adtim_pwm_drv_funcs = {
    __adtim_pwm_config,
    __adtim_pwm_enable,
    __adtim_pwm_disable,
};

/******************************************************************************/

/** \brief 配置PWM */
static int __adtim_pwm_config (void          *p_drv,
                               int            chan,
                               unsigned long  duty_ns,
                               unsigned long  period_ns)
{
    am_hc32_adtim_pwm_dev_t *p_dev      = (am_hc32_adtim_pwm_dev_t *)p_drv;
    amhw_hc32_adtim_t       *p_hw_adtim = (amhw_hc32_adtim_t *)
                                            p_dev->p_devinfo->adtim_regbase;

    amhw_hc32_adtim_basecnt_cfg_t basecnt;

    amhw_hc32_adtim_timx_chx_port_cfg_t  portcfg;
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
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV1;
        fre_div = 1;
    } else if(fre_div <= 2) {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV2;
        fre_div = 2;
    } else if(fre_div <= 4) {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV4;
        fre_div = 4;
    } else if(fre_div <= 8) {
        basecnt.cntclkdiv = AMHW_HC32_ADTIM_PCLK_DIV8;
        fre_div = 8;
    } else if(fre_div <= 16) {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV16;
        fre_div = 16;
    } else if(fre_div <= 64) {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV64;
        fre_div = 64;
    } else if(fre_div <= 256) {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV256;
        fre_div = 256;
    } else {
        basecnt.cntclkdiv  = AMHW_HC32_ADTIM_PCLK_DIV1024;
        fre_div = 1024;
    }

    basecnt.cntmode   = AMHW_HC32_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_HC32_ADTIM_CNT_DIR_UP;

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    amhw_hc32_adtim_init(p_hw_adtim, &basecnt);

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / fre_div;
    duty_c   = duty_c   / fre_div;

    /* 参数不合法 */
    if ((period_c == 0) || (period_c > 65535UL)) {
        return -AM_EINVAL;
    }

    /* 设置计数周期 */
    amhw_hc32_adtim_setperiod(p_hw_adtim, period_c);

    /* 清零计数器 */
    amhw_hc32_adtim_clearcount(p_hw_adtim);

    duty_c = period_c - duty_c;

    portcfg.portmode  = AMHW_HC32_ADTIM_PORT_MODE_CMP_OUTPUT;
    portcfg.outenable = AM_TRUE;
    portcfg.perc      = AMHW_HC32_ADTIM_PERIOD_INV;
    portcfg.cmpc      = AMHW_HC32_ADTIM_COMPARE_INV;
    portcfg.stastps   = AMHW_HC32_ADTIM_STATE_SEL_SS;
    portcfg.disval    = AMHW_HC32_ADTIM_DISVAL_NORM;
    portcfg.dissel    = AMHW_HC32_ADTIM_DISSEL0;
    portcfg.fltenable = AM_FALSE;
    portcfg.fltclk    = AMHW_HC32_ADTIM_FLTCLK_PCLK_DIV1;

    /* 互补PWM输出 */
    if(p_dev->p_devinfo->comp == 1) {

        amhw_hc32_adtim_setcomparevalue(p_hw_adtim, 
                                          AMHW_HC32_ADTIM_COMPARE_A, 
                                          duty_c);
        amhw_hc32_adtim_setcomparevalue(p_hw_adtim, 
                                          AMHW_HC32_ADTIM_COMPARE_B, 
                                          duty_c);

        portcfg.staout = (amhw_hc32_adtim_port_out_t)p_dev->p_devinfo->ocpolarity;
        portcfg.stpout = (amhw_hc32_adtim_port_out_t)p_dev->p_devinfo->ocpolarity;
        amhw_hc32_adtim_timxchxportcfg(p_hw_adtim,
                                         AMHW_HC32_ADTIM_CHX_A,
                                         &portcfg);

        portcfg.staout = (amhw_hc32_adtim_port_out_t)(!p_dev->p_devinfo->ocpolarity);
        portcfg.stpout = (amhw_hc32_adtim_port_out_t)(!p_dev->p_devinfo->ocpolarity);
        amhw_hc32_adtim_timxchxportcfg(p_hw_adtim,
                                         AMHW_HC32_ADTIM_CHX_B,
                                         &portcfg);

    /* 独立PWM输出*/
    } else {

        amhw_hc32_adtim_setcomparevalue(p_hw_adtim, 
                                          (amhw_hc32_adtim_compare_x_t)chan, 
                                          duty_c);

        portcfg.staout = (amhw_hc32_adtim_port_out_t)p_dev->p_devinfo->ocpolarity;
        portcfg.stpout = (amhw_hc32_adtim_port_out_t)p_dev->p_devinfo->ocpolarity;
        amhw_hc32_adtim_timxchxportcfg(p_hw_adtim,
                                         (amhw_hc32_adtim_chx_t)chan,
                                         &portcfg);
    }

    return AM_OK;
}

/**
 * \brief 使能PWM通道输出
 */
static int __adtim_pwm_enable (void *p_drv, int chan)
{
    int i = 0, t = 0, enable_flag = 0;
    am_hc32_adtim_pwm_dev_t      *p_dev      = (am_hc32_adtim_pwm_dev_t *)
                                                 p_drv;
    amhw_hc32_adtim_t            *p_hw_adtim = (amhw_hc32_adtim_t *)
                                                 p_dev->p_devinfo->adtim_regbase;
    am_hc32_adtim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

    uint8_t comp_pin_flag = 0;

    for(i = 0; i <= p_dev->p_devinfo->channels_num; i++) {

        if((p_chaninfo[i].channel & 0x7f) ==  chan) {

            /* 互补PWM输出 */
            if(p_dev->p_devinfo->comp == 1){

                /* 查找B通道，并初始化引脚 */
                for(t = 0; t <= p_dev->p_devinfo->channels_num; t++) {
                    if( p_chaninfo[t].channel == 0) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].func);
                        comp_pin_flag |= (1 << 0);
                    }
                    if( p_chaninfo[t].channel == 1) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].func);
                        comp_pin_flag |= (1 << 1);
                    }
                    if(comp_pin_flag == 0x3) {
                        break;
                    }
                }

                enable_flag = 1;
                break;

            /* 独立PWM输出 */
            } else {
                am_gpio_pin_cfg(p_chaninfo[chan].gpio, p_chaninfo[chan].func);

                enable_flag = 1;
                break;
            }
        }
    }

    /* 输入通道号无效 */
    if(enable_flag == 0){
        return -AM_EINVAL;
    }

    /* 清除所有中断标志 */
    amhw_hc32_adtim_clearallirqflag(p_hw_adtim);

    /* 清零计数器 */
    amhw_hc32_adtim_clearcount(p_hw_adtim);

    /* 定时器启动 */
    amhw_hc32_adtim_startcount(p_hw_adtim);

    return AM_OK;
}

/**
 * \brief 禁能PWM通道输出
 */
static int __adtim_pwm_disable (void *p_drv, int chan)
{
    int i = 0, t = 0, disable_flag = 0;
    am_hc32_adtim_pwm_dev_t      *p_dev    = (am_hc32_adtim_pwm_dev_t *)p_drv;
    amhw_hc32_adtim_t            *p_hw_adtim = (amhw_hc32_adtim_t *)p_dev->p_devinfo->adtim_regbase;
    am_hc32_adtim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

    uint8_t comp_pin_flag = 0;

    for(i = 0; i <= p_dev->p_devinfo->channels_num; i++) {

        if((p_chaninfo[i].channel & 0x7f) ==  chan) {

            /* 互补PWM输出 */
            if(p_dev->p_devinfo->comp == 1){

                /* 查找B通道，并解初始化引脚 */
                for(t = 0; t <= p_dev->p_devinfo->channels_num; t++) {
                    if( p_chaninfo[t].channel == 0) {
                        am_gpio_pin_cfg(p_chaninfo[t].gpio, p_chaninfo[t].dfunc);
                        comp_pin_flag |= (1 << 0);
                    }
                    if( p_chaninfo[t].channel == 1) {
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
            }else{
                am_gpio_pin_cfg(p_chaninfo[chan].gpio, p_chaninfo[chan].dfunc);

                disable_flag = 1;
                break;
            }
        }
    }

    /* 输入通道号无效 */
    if(disable_flag == 0){
        return -AM_EINVAL;
    }

    /* 清零计数器 */
    amhw_hc32_adtim_clearcount(p_hw_adtim);

    /* 定时器停止 */
    amhw_hc32_adtim_stopcount(p_hw_adtim);

    /* 清除所有中断标志 */
    amhw_hc32_adtim_clearallirqflag(p_hw_adtim);

    return AM_OK;
}

/**
  * \brief adtim pwm服务初始化
  */
am_pwm_handle_t am_hc32_adtim_pwm_init (am_hc32_adtim_pwm_dev_t      *p_dev,
                                     const am_hc32_adtim_pwm_devinfo_t *p_devinfo)
{
    amhw_hc32_adtim_basecnt_cfg_t basecnt;
    amhw_hc32_adtim_t  *p_hw_adtim = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->chan_max = 2;

    if(p_dev->chan_max < p_devinfo->channels_num) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo        = p_devinfo;
    p_hw_adtim              = (amhw_hc32_adtim_t *)p_dev->p_devinfo->adtim_regbase;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_adtim_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;

    basecnt.cntmode   = AMHW_HC32_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_HC32_ADTIM_CNT_DIR_UP;
    basecnt.cntclkdiv = AMHW_HC32_ADTIM_PCLK_DIV1;

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    amhw_hc32_adtim_init(p_hw_adtim, &basecnt);

    /* 清零计数器 */
    amhw_hc32_adtim_clearcount(p_hw_adtim);

    return &(p_dev->pwm_serv);
}

void am_hc32_adtim_pwm_deinit (am_pwm_handle_t handle)
{

    am_hc32_adtim_pwm_dev_t *p_dev    = (am_hc32_adtim_pwm_dev_t *)handle;
    amhw_hc32_adtim_t       *p_hw_adtim = NULL;

    if (p_dev == NULL ) {
        return;
    }

    p_hw_adtim   = (amhw_hc32_adtim_t *)p_dev->p_devinfo->adtim_regbase;

    /* 清零计数器 */
    amhw_hc32_adtim_clearcount(p_hw_adtim);

    /* 关闭TIM模块 */
    amhw_hc32_adtim_stopcount(p_hw_adtim);

    /* 手动复位寄存器 */
    amhw_hc32_adtim_deinit(p_hw_adtim);

    p_dev->pwm_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
