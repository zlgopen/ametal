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
 * \brief TIM驱动，输入捕获驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-12  zp, first implementation
 * \endinternal
 */

#include "zlg118_clk.h"
#include "am_zlg118_tim_cap.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/
static uint16_t  __update_num = 0;

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** \brief 捕获参数配置 */
static int __zlg118_tim_cap_config (void              *p_cookie,
                                    int                chan,
                                    unsigned int       flags,
                                    am_cap_callback_t  pfn_callback,
                                    void              *p_arg);

/** \brief 使能捕获通道 */
static int __zlg118_tim_cap_enable (void *p_drv, int chan);

/** \brief 禁能捕获通道 */
static int __zlg118_tim_cap_disable (void *p_drv, int chan);

/** \brief 复位捕获通道计数值 */
static int __zlg118_tim_cap_reset (void *p_drv, int chan);

static int __zlg118_tim_cap_count_to_time (void         *p_drv,
                                           int           chan,
                                           unsigned int  count1,
                                           unsigned int  count2,
                                           unsigned int *p_time_ns);

static void __zlg118_tim_cap_irq_handler (void *p_arg);

/** \brief 捕获服务驱动函数 */
static const struct am_cap_drv_funcs __g_tim_cap_drv_funcs = {
    __zlg118_tim_cap_config,
    __zlg118_tim_cap_enable,
    __zlg118_tim_cap_disable,
    __zlg118_tim_cap_reset,
    __zlg118_tim_cap_count_to_time,
};

/******************************************************************************/

/** \brief 配置一个输入捕获通道 */
static int __zlg118_tim_cap_config (void              *p_drv,
                                    int                chan,
                                    unsigned int       options,
                                    am_cap_callback_t  pfn_callback,
                                    void              *p_arg)
{
    am_zlg118_tim_cap_dev_t *p_dev    = (am_zlg118_tim_cap_dev_t *)p_drv;
    amhw_zlg118_tim_t       *p_hw_tim = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;

    amhw_zlg118_tim_filter_type_t filter_type = AMHW_ZLG118_TIM_FLITER_TYPE_FLTA0;

    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->chan_max) {
        return -AM_EINVAL;
    }

    /* 分频系数设置 */
    amhw_zlg118_tim_mode_clkdiv_set(p_hw_tim, p_dev->p_devinfo->clk_div);

    filter_type = (amhw_zlg118_tim_filter_type_t)(chan * 4);

    /* 设置输入滤波器的分频值为0(默认不使用滤波) */
    amhw_zlg118_tim_mode23_fliter_set(p_hw_tim,
                                      filter_type,
                                      AMHW_ZLG118_TIM_FILTER_NO);

    /*  选择上升沿触发 */
    if ((options & AM_CAP_TRIGGER_RISE) == AM_CAP_TRIGGER_RISE) {
        if(chan & 0x1) {
            amhw_zlg118_tim_mode23_crb_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfb_disable(p_hw_tim, chan);
        } else {
            amhw_zlg118_tim_mode23_cra_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfa_disable(p_hw_tim, chan);
        }
    }

    /*  选择下降沿触发 */
    if ((options & AM_CAP_TRIGGER_FALL) == AM_CAP_TRIGGER_FALL) {
        if(chan & 0x1) {
            amhw_zlg118_tim_mode23_crb_disable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfb_enable(p_hw_tim, chan);

        } else {
            amhw_zlg118_tim_mode23_cra_disable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfa_enable(p_hw_tim, chan);
        }
    }

    if((options & AM_CAP_TRIGGER_BOTH_EDGES) == AM_CAP_TRIGGER_BOTH_EDGES ) {
        if(chan & 0x1) {
            amhw_zlg118_tim_mode23_crb_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfb_enable(p_hw_tim, chan);
        } else {
            amhw_zlg118_tim_mode23_cra_enable(p_hw_tim, chan);
            amhw_zlg118_tim_mode23_cfa_enable(p_hw_tim, chan);
        }
    }


    p_dev->callback_info[chan].callback_func = pfn_callback;
    p_dev->callback_info[chan].p_arg         = p_arg;

    return AM_OK;
}

/** \brief 捕获使能 */
static int __zlg118_tim_cap_enable (void *p_drv, int chan)
{
    int i = 0, enable_flag = 0;
    am_zlg118_tim_cap_dev_t    *p_dev    = (am_zlg118_tim_cap_dev_t *)p_drv;
    amhw_zlg118_tim_t          *p_hw_tim = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_zlg118_tim_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->chan_max) {
        return -AM_EINVAL;
    }

    /* 引脚配置 */
    for(i = 0; i < p_dev->p_devinfo->channels_num; i++) {
        if((p_ioinfo[i].channel & 0x7f) ==  chan){
            am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].func);
            enable_flag = 1;
            break;
        }
    }

    /* 输入通道号无效 */
    if(enable_flag == 0){
        return -AM_EINVAL;
    }

    /* 中断连接并使能 */
    am_int_connect(p_dev->p_devinfo->inum, __zlg118_tim_cap_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    /* 设置自动重装寄存器的值 */
    amhw_zlg118_tim_arr_count_set(p_hw_tim, 0xffff);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    /* 允许更新中断 */
    amhw_zlg118_tim_mode23_int_enable(p_hw_tim, AMHW_ZLG118_TIM_INT_UIE);

    /* 捕获中断使能 */
    if((chan & 0x01) == 0) {
        amhw_zlg118_tim_mode23_ciea_int_enable(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_cieb_int_enable(p_hw_tim, chan);
    }

    /* 使能定时器 */
    amhw_zlg118_tim_enable(p_hw_tim);

    /* 清除中断标志 */
    amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZLG118_TIM_INT_FLAG_ALL);

    return AM_OK;
}

/** \brief 捕获禁能 */
static int __zlg118_tim_cap_disable (void *p_drv, int chan)
{
    int i = 0, disable_flag = 0;
    am_zlg118_tim_cap_dev_t    *p_dev    = (am_zlg118_tim_cap_dev_t *)p_drv;
    amhw_zlg118_tim_t          *p_hw_tim = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;
    am_zlg118_tim_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    /* 有效通道号范围 0 ~ (channels_num - 1) */
    if (chan >= p_dev->p_devinfo->channels_num) {
        return -AM_EINVAL;
    }

    /* 还原引脚配置 */
    for(i = 0; i < p_dev->p_devinfo->channels_num; i++) {
        if((p_ioinfo[i].channel & 0x7f) ==  chan){
            am_gpio_pin_cfg(p_ioinfo[chan].gpio, p_ioinfo[chan].dfunc);
            disable_flag = 1;
            break;
        }
    }

    /* 输入通道号无效 */
    if(disable_flag == 0){
        return -AM_EINVAL;
    }

    /* 断开NVIC中断回调函数 */
    am_int_disconnect(p_dev->p_devinfo->inum, __zlg118_tim_cap_irq_handler, (void *)p_dev);
    am_int_disable(p_dev->p_devinfo->inum);

    /* 允许更新中断 */
    amhw_zlg118_tim_mode23_int_disable(p_hw_tim, AMHW_ZLG118_TIM_INT_UIE);

    /* 捕获中断使能 */
    if((chan & 0x01) == 0) {
        amhw_zlg118_tim_mode23_ciea_int_disable(p_hw_tim, chan);
    } else {
        amhw_zlg118_tim_mode23_cieb_int_disable(p_hw_tim, chan);
    }

    /* 禁能定时器TIM允许计数 */
    amhw_zlg118_tim_disable(p_hw_tim);

    return AM_OK;
}

/**
  * \brief 复位捕获通道计数值
  */
static int __zlg118_tim_cap_reset (void *p_drv, int chan)
{
    am_zlg118_tim_cap_dev_t *p_dev    = (am_zlg118_tim_cap_dev_t *)p_drv;
    amhw_zlg118_tim_t       *p_hw_tim = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 禁能定时器TIM允许计数 */
    amhw_zlg118_tim_disable(p_hw_tim);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    /* 使能定时器 */
    amhw_zlg118_tim_enable(p_hw_tim);

    /* 清除中断标志 */
    amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, AMHW_ZLG118_TIM_INT_FLAG_ALL);

    return AM_OK;
}

/**
  * \brief 转换两次捕获值为时间值
  */
static int __zlg118_tim_cap_count_to_time (void         *p_drv,
                                           int           chan,
                                           unsigned int  count1,
                                           unsigned int  count2,
                                           unsigned int *p_time_ns)
{
    am_zlg118_tim_cap_dev_t *p_dev    = (am_zlg118_tim_cap_dev_t *)p_drv;
    amhw_zlg118_tim_t       *p_hw_tim = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;

    uint32_t count_err;
    uint64_t time_ns;
    uint16_t reg_pre, pre;

    uint32_t clkfreq  = am_clk_rate_get(CLK_PCLK);

    reg_pre = (uint16_t)amhw_zlg118_tim_mode_clkdiv_get(p_hw_tim);

    if(reg_pre == 7) {
        pre = 256;
    } else {
        pre = 1 << reg_pre;
    }

    count_err =  count2 - count1;

    /* 将两次读取值的差转换成时间 */
    time_ns = (uint64_t)1000000000 * (uint64_t)count_err * pre / (uint64_t)clkfreq;

    *p_time_ns = time_ns;

    return AM_OK;
}

/**
  * \brief 中断服务函数
  */
static void __zlg118_tim_cap_irq_handler (void *p_arg)
{
    am_zlg118_tim_cap_dev_t *p_dev     = (am_zlg118_tim_cap_dev_t *)p_arg;
    amhw_zlg118_tim_t       *p_hw_tim  = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;

    uint8_t           i = 0;
    am_cap_callback_t callback_func;
    uint32_t          value;

    if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim,
             AMHW_ZLG118_TIM_INT_FLAG_UPDATE)) == AM_TRUE) {

        __update_num++;

        amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim,
                                            AMHW_ZLG118_TIM_INT_FLAG_UPDATE);
    }

    /* 判断标志 （CH0A、CH1A、CH2A）*/
    for(i = 0; i < p_dev->chan_max;i = i + 2) {
        if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim, (i / 2) + 2)) == AM_TRUE) {

            callback_func = p_dev->callback_info[i].callback_func;

            /* 得到对应通道的值 */
            value = amhw_zlg118_tim_mode23_ccrxy_get(p_hw_tim, i) + (__update_num << 16ul);

            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }

            /* 清除通道i标志 */
            amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim, (i / 2) + 2);
        }
    }

    /* 判断标志  （CH0B、CH1B、CH2B */
    for(i = 1; i < p_dev->chan_max;i = i + 2) {
        if ((amhw_zlg118_tim_mode23_int_flag_check(p_hw_tim, (i / 2) + 5)) == AM_TRUE) {

            callback_func = p_dev->callback_info[i].callback_func;

            /* 得到对应通道的值 */
            value = amhw_zlg118_tim_mode23_ccrxy_get(p_hw_tim, i) + (__update_num << 16ul);

            if (callback_func != NULL) {
                callback_func(p_dev->callback_info[i].p_arg, value);
            }

            /* 清除通道i标志 */
            amhw_zlg118_tim_mode23_int_flag_clr(p_hw_tim,  (i / 2) + 5);
        }
    }
}

/**
  * \brief 捕获初始化
  */
void __zlg118_tim_cap_init (amhw_zlg118_tim_t       *p_hw_tim,
                            am_zlg118_tim_cap_dev_t *p_dev,
                            amhw_zlg118_tim_type_t   type)
{
    uint8_t i = 0;

    /* 设置定时器模式2 */
    amhw_zlg118_tim_mode_set(p_hw_tim, AMHW_ZLG118_TIM_MODE_ZIGZAG);

    /* 向上计数 */
    amhw_zlg118_tim_mode23_counter_up(p_hw_tim);

    /* 设置通道为捕获模式 （CH0A、CH1A、CH2A）*/
    for(i = 0; i < p_dev->chan_max;i = i + 2) {
        amhw_zlg118_tim_mode23_csa_cap(p_hw_tim, i);
    }

    /* 设置通道为捕获模式 （CH0B、CH1B、CH2B */
    for(i = 1; i < p_dev->chan_max;i = i + 2) {
        amhw_zlg118_tim_mode23_csb_cap(p_hw_tim, i);
    }

    /* 定时器时钟为内部时钟 */
    amhw_zlg118_tim_mode_clk_src_set(p_hw_tim,AMHW_ZLG118_TIM_CLK_SRC_TCLK);

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    __update_num = 0;

}

am_cap_handle_t am_zlg118_tim_cap_init (am_zlg118_tim_cap_dev_t           *p_dev,
                                        const am_zlg118_tim_cap_devinfo_t *p_devinfo)
{
    amhw_zlg118_tim_t *p_hw_tim = NULL;
    uint8_t    i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    if(p_devinfo->tim_type == AMHW_ZLG118_TIM_TYPE_TIM3) {
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
    p_hw_tim                = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;
    p_dev->cap_serv.p_funcs = (struct am_cap_drv_funcs *)&__g_tim_cap_drv_funcs;
    p_dev->cap_serv.p_drv   = p_dev;

    for (i = 0; i < p_devinfo->channels_num; i++) {
        p_dev->callback_info[i].callback_func = NULL;
    }

    /* 捕获初始化 */
    __zlg118_tim_cap_init(p_hw_tim, p_dev, p_devinfo->tim_type);

    return &(p_dev->cap_serv);
}

void am_zlg118_tim_cap_deinit (am_cap_handle_t handle)
{
    uint8_t i = 0;

    am_zlg118_tim_cap_dev_t *p_dev    = (am_zlg118_tim_cap_dev_t *)handle;
    amhw_zlg118_tim_t       *p_hw_tim = NULL;

    am_zlg118_tim_cap_ioinfo_t *p_ioinfo = p_dev->p_devinfo->p_ioinfo;

    if (p_dev == NULL || p_dev->p_devinfo == NULL ) {
        return ;
    }

    p_hw_tim     = (amhw_zlg118_tim_t *)p_dev->p_devinfo->tim_regbase;

    /* 清零计数器 */
    amhw_zlg118_tim_cnt16_count_set(p_hw_tim, 0);

    __update_num = 0;

    /* 关闭TIM模块 */
    amhw_zlg118_tim_disable(p_hw_tim);

    am_int_disable(p_dev->p_devinfo->inum);

    p_dev->cap_serv.p_drv = NULL;

    /* 还原GPIO配置 */
    for (i = 0; i <p_dev->p_devinfo->channels_num; i++) {

        am_gpio_pin_cfg(p_ioinfo[i].gpio, p_ioinfo[i].dfunc);
    }

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
