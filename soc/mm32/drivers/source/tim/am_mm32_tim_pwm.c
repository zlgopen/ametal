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
 * - 1.00 17-04-21  nwt, first implementation
 * \endinternal
 */

#include "am_mm32_tim_pwm.h"
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
    am_mm32_tim_pwm_dev_t *p_dev    = (am_mm32_tim_pwm_dev_t *)p_drv;
    amhw_mm32_tim_t       *p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    uint32_t clkfreq;
    uint32_t period_c, duty_c, temp;
    uint16_t  pre_real = 1, pre_reg = 0;

    /* 参数不合法 */
    if ((period_ns == 0) || (duty_ns > 4294967295UL) ||
        (period_ns > 4294967295UL) || (duty_ns > period_ns)) {
        return -AM_EINVAL;
    }

    clkfreq = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    /* 计算出来得到的是计数值CNT, 公式ns * 10e-9= cnt * (1/clkfrq) */
    period_c = (uint64_t)(period_ns) * (clkfreq) / (uint64_t)1000000000;
    duty_c   = (uint64_t)(duty_ns)   * (clkfreq) / (uint64_t)1000000000;

   {

        /* 当计数小于65536时，不分频(值为1,1代表为1分频) */
        temp = period_c / 65536 + 1;

        /* 16位定时器需要运算取得合适的分频值 */
        for (pre_real = 1; pre_real < temp; ) {
             pre_reg++;           /* 计算写入寄存器的分频值0,1,2,... */
             pre_real++;          /* 分频数 */
        }
    }

    /* 写入分频值 */
    amhw_mm32_tim_prescale_set(p_hw_tim, pre_reg);

    /* 重新计算PWM的周期及脉冲频率 */
    period_c = period_c / pre_real;
    duty_c = duty_c / pre_real;

    /* 共用一个重载寄存器，PWM波所有的周期都相同 */
    amhw_mm32_tim_arr_set(p_hw_tim, period_c - 1);

    /* 使能定ARR预装载 */
    amhw_mm32_tim_arpe_enable(p_hw_tim);

    /* Low level time */
    duty_c = period_c - duty_c;

    /* 填充的值为PWM输出低电平的时间 */
    amhw_mm32_tim_ccr_ouput_reload_val_set(p_hw_tim, duty_c - 1, chan);

    /* 选择该通道为输出 */
    amhw_mm32_tim_ccs_set(p_hw_tim, 0, chan);

    /* 选择该通道的PWM模式 */
    amhw_mm32_tim_ocm_set(p_hw_tim, p_dev->p_devinfo->pwm_mode, chan);

    /* PWM输出通道预装载使能 */
    amhw_mm32_tim_ccs_ocpe_enable(p_hw_tim, chan);

    if ((p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE0) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE2) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE3)) {

        /* 设置CR2寄存器确定N极极性 */
    }
    /* 使能互补输出高电平有效 */
    amhw_mm32_tim_ccne_set(p_hw_tim, p_dev->p_devinfo->ocpolarity, chan);

    /* 设置比较输出通道高电平极性有效 */
    amhw_mm32_tim_ccp_output_set(p_hw_tim, p_dev->p_devinfo->ocpolarity, chan);

    return AM_OK;
}

/**
 * \brief 使能PWM通道输出
 */
static int __tim_pwm_enable (void *p_drv, int chan)
{
    int i = 0, enable_flag = 0;
    am_mm32_tim_pwm_dev_t    *p_dev    = (am_mm32_tim_pwm_dev_t *)p_drv;
    amhw_mm32_tim_t          *p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_mm32_tim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

      /* 判断引脚列表中是否有对应通道配置信息 */
    for(i = 0; i <= p_dev->p_devinfo->channels_num; i++){
        if((p_chaninfo[i].channel & 0x7f) ==  chan){
            am_gpio_pin_cfg(p_chaninfo[i].gpio, p_chaninfo[i].func);
            if((p_chaninfo[i].channel & 0x80) == 0x80){
                /* 使能互补输出 */
                amhw_mm32_tim_ccne_enable(p_hw_tim, chan);
                enable_flag = 1;
            }else{
                /* 使能通道PWM输出 */
                amhw_mm32_tim_cce_output_enable(p_hw_tim, chan);
                enable_flag = 1;
            }
        }
    }
	
    /* 输入通道号无效 */
    if(enable_flag == 0){
        return -AM_EINVAL;
    }
	
    /* 高级定时器使能主输出MOE */
    if ((p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE0) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE2) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE3)) {

        amhw_mm32_tim_bdtr_enable(p_hw_tim, AMHW_MM32_TIM_MOE);
    }

    /* 产生更新事件，重新初始化Prescaler计数器及Repetition计数器 */
    amhw_mm32_tim_egr_set(p_hw_tim, AMHW_MM32_TIM_UG);

    if (amhw_mm32_tim_status_flg_get(p_hw_tim, AMHW_MM32_TIM_UG) != 0) {

        /* 更新定时器时会产生更新事件,清除标志位 */
        amhw_mm32_tim_status_flg_clr(p_hw_tim, AMHW_MM32_TIM_UG);
    }

    /* 使能定时器TIM允许计数 */
    amhw_mm32_tim_enable(p_hw_tim);

    return AM_OK;
}

/**
 * \brief 禁能PWM通道输出
 */
static int __tim_pwm_disable (void *p_drv, int chan)
{
    int  i = 0, disable_flag = 0;
    am_mm32_tim_pwm_dev_t      *p_dev    = (am_mm32_tim_pwm_dev_t *)p_drv;
    amhw_mm32_tim_t            *p_hw_tim = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_mm32_tim_pwm_chaninfo_t *p_chaninfo = p_dev->p_devinfo->p_chaninfo;

    /* 判断引脚列表中是否有对应通道配置信息 */
    for(i = 0; i <= p_dev->p_devinfo->channels_num; i++){
        if((p_chaninfo[i].channel & 0x7f) ==  chan){
            am_gpio_pin_cfg(p_chaninfo[i].gpio, p_chaninfo[i].dfunc);
            disable_flag = 1;
        }
    }
	
    /* 输入通道号无效 */
    if(disable_flag == 0){
        return -AM_EINVAL;
    }
	
    /* 禁能定时器TIM允许计数 */
    amhw_mm32_tim_disable(p_hw_tim);

    /* 禁能通道PWM输出 */
    amhw_mm32_tim_cce_output_disable(p_hw_tim, chan);

    /* 高级定时器禁能主输出MOE */
    if ((p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE0) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE2) ||
        (p_dev->p_devinfo->tim_type == AMHW_MM32_TIM_TYPE3)) {

        amhw_mm32_tim_bdtr_disable(p_hw_tim, AMHW_MM32_TIM_MOE);
    }

    return AM_OK;
}

/**
  * \brief pwm初始化
  */
void __tim_pwm_init (amhw_mm32_tim_t     *p_hw_tim,
                     amhw_mm32_tim_type_t type)
{
    if ((type == AMHW_MM32_TIM_TYPE0) || (type == AMHW_MM32_TIM_TYPE1)) {

        /* 边沿对齐模式 */
        amhw_mm32_tim_cms_set(p_hw_tim, 0);

        /* 向上计数 */
        amhw_mm32_tim_dir_set(p_hw_tim, 0);
    }

    /* 设置时钟分割(用于确定输入滤波器的输入时钟频率,默认 Fdts = Fck_in */
    amhw_mm32_tim_ckd_set(p_hw_tim, 0);

    /* 清零计数器 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 设置分频器 */
    amhw_mm32_tim_prescale_set(p_hw_tim, 0x00);

    /* 允许更新事件 */
    amhw_mm32_tim_udis_enable(p_hw_tim);
}

/**
  * \brief tim pwm服务初始化
  */
am_pwm_handle_t am_mm32_tim_pwm_init (am_mm32_tim_pwm_dev_t           *p_dev,
                                     const am_mm32_tim_pwm_devinfo_t *p_devinfo)
{
    amhw_mm32_tim_t  *p_hw_tim = NULL;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo        = p_devinfo;
    p_hw_tim                = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->pwm_serv.p_funcs = (struct am_pwm_drv_funcs *)&__g_tim_pwm_drv_funcs;
    p_dev->pwm_serv.p_drv   = p_dev;

    __tim_pwm_init(p_hw_tim, p_devinfo->tim_type);

    return &(p_dev->pwm_serv);
}

void am_mm32_tim_pwm_deinit (am_pwm_handle_t handle)
{

    am_mm32_tim_pwm_dev_t *p_dev    = (am_mm32_tim_pwm_dev_t *)handle;
    amhw_mm32_tim_t       *p_hw_tim = NULL;

    if (p_dev == NULL ) {
        return;
    }

    p_hw_tim   = (amhw_mm32_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 清零计数器 */
    amhw_mm32_tim_count_set(p_hw_tim, 0);

    /* 禁能定时器TIM允许计数 */
    amhw_mm32_tim_disable(p_hw_tim);

    p_dev->pwm_serv.p_drv = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
