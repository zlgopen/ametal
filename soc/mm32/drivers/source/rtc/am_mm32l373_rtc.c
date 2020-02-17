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
 * \brief Real-Time Clock(RTC) drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.01 18-06-14  pea, fix bug
 * - 1.00 17-08-28  lqy, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_mm32l373.h"
#include "am_mm32l373_rtc.h"
#include "amhw_mm32l373_rtc.h"
#include "amhw_mm32l373_rcc.h"
#include "amhw_mm32_pwr.h"
#include "amhw_mm32l373_bkp.h"
#include "mm32l373_periph_map.h"
#include <time.h>
#include <string.h>

/*******************************************************************************
  本地函数声明
*******************************************************************************/

/**
 * \brief 设置当前的细分时间
 */
am_local int __rtc_time_set (void *p_drv, am_tm_t *p_tm);

/**
 * \brief 获取当前的细分时间
 */
am_local int __rtc_time_get (void *p_drv, am_tm_t *p_tm);

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief 适配 RTC 标准接口需要的函数 */
am_local am_const struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
    __rtc_time_set,  /* 设置当前的细分时间 */
    __rtc_time_get,  /* 获取当前的细分时间 */
};

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/**
 * \brief 等待 RTC 寄存器同步
 */
am_local void __sync_wait (amhw_mm32l373_rtc_t *p_hw_rtc)
{
    while (amhw_mm32l373_rtc_crl_read_statu(p_hw_rtc, AMHW_MM32L373_RTC_RSF) == 0);
}

/**
 * \brief 等待 RTC 操作完成
 */
am_local void __operation_wait (amhw_mm32l373_rtc_t *p_hw_rtc)
{
    while (amhw_mm32l373_rtc_crl_read_statu(p_hw_rtc, AMHW_MM32L373_RTC_RTOFF) == 0);
}

/**
 * \brief 秒转化为 am_tm_t 时间数据
 * 
 * \param[in]  seconds 秒数据
 * \param[out] p_time  指向 am_tm_t 时间数据的指针
 *
 * \return 无
 */
am_local void __sec2tm (uint32_t sec, am_tm_t *p_time)
{
    time_t     second = sec;
    struct tm *p_tm   = NULL;

    p_tm = localtime(&second);

    if ((NULL != p_time) && (NULL != p_tm)) {
        memcpy(p_time, p_tm, sizeof(am_tm_t));
    }
}

/**
 * \brief 获取当前的细分时间
 */
am_local int __rtc_time_get (void *p_drv, am_tm_t *p_tm)
{
    am_mm32l373_rtc_dev_t *p_dev    = (am_mm32l373_rtc_dev_t *)p_drv;
    amhw_mm32l373_rtc_t   *p_hw_rtc = (amhw_mm32l373_rtc_t  *)p_dev->p_devinfo->rtc_regbase;
    uint32_t             sec;

    if ((NULL == p_tm) || (NULL == p_tm)) {
        return -AM_EINVAL;
    }

    __sync_wait(p_hw_rtc); /* 等待 RTC 寄存器同步 */

    if (AM_OK != am_mm32l373_rtc_cnt_get(&p_dev->rtc_serv, &sec)) {
        return -AM_EINVAL;
    }

    /* 将秒转换成时间信息 */
    __sec2tm(sec, p_tm);

    return AM_OK;
}

/**
 * \brief 设置当前的细分时间
 */
am_local int __rtc_time_set (void *p_drv, am_tm_t *p_tm)
{
    am_mm32l373_rtc_dev_t *p_dev = (am_mm32l373_rtc_dev_t *)p_drv;
    uint32_t             sec;

    if ((NULL == p_tm) || (NULL == p_tm)) {
        return -AM_EINVAL;
    }

    sec = mktime((struct tm *)p_tm);

    am_mm32l373_rtc_cnt_set(&p_dev->rtc_serv, sec);

    return AM_OK;
}

/**
 * \brief RTC 中断服务函数
 */
am_local void __rtc_sec_isr (void *p_arg)
{
    am_mm32l373_rtc_dev_t *p_dev = (am_mm32l373_rtc_dev_t *)p_arg;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    /* 秒中断 */
    if (amhw_mm32l373_rtc_crl_read_statu(p_hw_rtc, AMHW_MM32L373_RTC_SECF)) {
        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_SECF);
        if (p_dev->pfn_callback[0]) {
            p_dev->pfn_callback[0](p_dev->p_arg[0]);
        }
    }

    /* 闹钟中断 */
    if (amhw_mm32l373_rtc_crl_read_statu(p_hw_rtc, AMHW_MM32L373_RTC_ALRF)) {
        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_ALRF);
        if (p_dev->pfn_callback[1]) {
            p_dev->pfn_callback[1](p_dev->p_arg[1]);
        }
    }

    /* 溢出中断 */
    if (amhw_mm32l373_rtc_crl_read_statu(p_hw_rtc, AMHW_MM32L373_RTC_OWF)) {
        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_OWF);
        if (p_dev->pfn_callback[2]) {
            p_dev->pfn_callback[2](p_dev->p_arg[2]);
        }
    }
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief RTC 状态获取，可以用来判断本次运行前备份区域是否断电
 */
am_bool_t am_mm32l373_rtc_state_get (am_rtc_handle_t handle)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;

    if (NULL == handle) {
        return AM_FALSE;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);

    return p_dev->rtc_continue;
}

/**
 * \brief RTC 同步标志复位，当 APB1 曾经被禁止过，需要调用此接口
 */
am_err_t am_mm32l373_rtc_apb1_reflush (am_rtc_handle_t handle)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_RSF);

    return AM_OK;
}

/**
 * \brief 获取 RTC 计数器值
 */
am_err_t am_mm32l373_rtc_cnt_get (am_rtc_handle_t handle, uint32_t *p_cnt)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if ((NULL == handle) || (NULL == p_cnt)) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    __sync_wait(p_hw_rtc); /* 等待 RTC 寄存器同步 */

    *p_cnt = (amhw_mm32l373_rtc_cnth_get(p_hw_rtc) << 16) +
             amhw_mm32l373_rtc_cntl_get(p_hw_rtc);

    return AM_OK;
}

/**
 * \brief 设置 RTC 计数器值
 */
am_err_t am_mm32l373_rtc_cnt_set (am_rtc_handle_t handle, uint32_t cnt)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc = (amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    __operation_wait(p_hw_rtc);                            /* 等待 RTC 操作完成 */
    amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc,             /* 软件清零 RSF 标志位 */
                                     AMHW_MM32L373_RTC_RSF);
    amhw_mm32l373_rtc_crl_cnf_enter(p_hw_rtc);               /* 允许配置 */
    amhw_mm32l373_rtc_cnth_set(p_hw_rtc, cnt >> 16);         /* 写入高 16 位 */
    amhw_mm32l373_rtc_cntl_set(p_hw_rtc, cnt & 0xffff);      /* 写入低 16 位 */
    amhw_mm32l373_rtc_crl_cnf_out(p_hw_rtc);                 /* 配置更新 */
    __operation_wait(p_hw_rtc);                            /* 等待 RTC 操作完成 */

    return AM_OK;
}

/**
 * \brief 设置 RTC 闹钟值
 */
am_err_t am_mm32l373_rtc_alr_set (am_rtc_handle_t handle, uint32_t alr)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    __operation_wait(p_hw_rtc);                       /* 等待 RTC 操作完成 */
    amhw_mm32l373_rtc_crl_cnf_enter(p_hw_rtc);          /* 允许配置 */
    amhw_mm32l373_rtc_alrh_set(p_hw_rtc, alr >> 16);    /* 写入高 16 位 */
    amhw_mm32l373_rtc_alrl_set(p_hw_rtc, alr & 0xFFFF); /* 写入低 16 位 */
    amhw_mm32l373_rtc_crl_cnf_out(p_hw_rtc);            /* 配置更新 */
    __operation_wait(p_hw_rtc);                       /* 等待 RTC 操作完成 */

    return AM_OK;
}

/**
 * \brief 设置 RTC 闹钟时间
 */
am_err_t am_mm32l373_rtc_alarm_set (am_rtc_handle_t handle, am_tm_t *p_tm)
{
    uint32_t sec = 0;

    if ((NULL == handle) || (NULL == p_tm)) {
        return -AM_EINVAL;
    }

    sec = mktime((struct tm *)p_tm);

    am_mm32l373_rtc_alr_set(handle, sec);

    return AM_OK;
}

/**
 * \brief 设置 RTC 中断回调
 */
am_err_t am_mm32l373_rtc_callback_set (am_rtc_handle_t handle,
                                     uint8_t         type,
                                     am_pfnvoid_t    pfn_callback,
                                     void           *p_arg)
{
    am_mm32l373_rtc_dev_t *p_dev = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);

    if (AM_MM32L373_RTC_CALLBACK_SECOND == type) {
        p_dev->pfn_callback[0] = pfn_callback;
        p_dev->p_arg[0] = p_arg;
    } else if (AM_MM32L373_RTC_CALLBACK_ALARM == type) {
        p_dev->pfn_callback[1] = pfn_callback;
        p_dev->p_arg[1] = p_arg;
    } else if (AM_MM32L373_RTC_CALLBACK_OVER == type) {
        p_dev->pfn_callback[2] = pfn_callback;
        p_dev->p_arg[2] = p_arg;
    } else {
        return -AM_EINVAL;
    }

    return AM_OK;
}

/**
 * \brief RTC 中断使能
 */
am_err_t am_mm32l373_rtc_int_enable (am_rtc_handle_t handle,
                                   uint8_t         type)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    __operation_wait(p_hw_rtc); /* 等待 RTC 操作完成 */

    if (AM_MM32L373_RTC_CALLBACK_SECOND == type) {
        amhw_mm32l373_rtc_crh_allow_int(p_hw_rtc, AMHW_MM32L373_RTC_SECIE);
        p_dev->int_state |= AM_MM32L373_RTC_CALLBACK_SECOND;
    } else if (AM_MM32L373_RTC_CALLBACK_ALARM == type) {
        amhw_mm32l373_rtc_crh_allow_int(p_hw_rtc, AMHW_MM32L373_RTC_ALRIE);
        p_dev->int_state |= AM_MM32L373_RTC_CALLBACK_ALARM;
    } else if (AM_MM32L373_RTC_CALLBACK_OVER == type) {
        amhw_mm32l373_rtc_crh_allow_int(p_hw_rtc, AMHW_MM32L373_RTC_OWIE);
        p_dev->int_state |= AM_MM32L373_RTC_CALLBACK_OVER;
    } else {
        return -AM_EINVAL;
    }

    am_int_enable(p_dev->p_devinfo->rtc_inum);

    return AM_OK;
}

/**
 * \brief RTC 中断失能
 */
am_err_t am_mm32l373_rtc_int_disable (am_rtc_handle_t handle,
                                    uint8_t         type)
{
    am_mm32l373_rtc_dev_t *p_dev    = NULL;
    amhw_mm32l373_rtc_t   *p_hw_rtc = NULL;

    if (NULL == handle) {
        return -AM_EINVAL;
    }

    p_dev = (am_mm32l373_rtc_dev_t *)AM_CONTAINER_OF(handle, am_mm32l373_rtc_dev_t, rtc_serv);
    p_hw_rtc =(amhw_mm32l373_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    __operation_wait(p_hw_rtc); /* 等待 RTC 操作完成 */

    if (AM_MM32L373_RTC_CALLBACK_SECOND == type) {
        amhw_mm32l373_rtc_crh_forbid_int(p_hw_rtc, AMHW_MM32L373_RTC_SECIE);
        p_dev->int_state &= ~AM_MM32L373_RTC_CALLBACK_SECOND;
    } else if (AM_MM32L373_RTC_CALLBACK_ALARM == type) {
        amhw_mm32l373_rtc_crh_forbid_int(p_hw_rtc, AMHW_MM32L373_RTC_ALRIE);
        p_dev->int_state &= ~AM_MM32L373_RTC_CALLBACK_ALARM;
    } else if (AM_MM32L373_RTC_CALLBACK_OVER == type) {
        amhw_mm32l373_rtc_crh_forbid_int(p_hw_rtc, AMHW_MM32L373_RTC_OWIE);
        p_dev->int_state &= ~AM_MM32L373_RTC_CALLBACK_OVER;
    } else {
        return -AM_EINVAL;
    }

    if (0 == p_dev->int_state) {
        am_int_disable(p_dev->p_devinfo->rtc_inum);
    }

    return AM_OK;
}

/**
 * \brief 初始化 RTC
 */
am_rtc_handle_t am_mm32l373_rtc_init (am_mm32l373_rtc_dev_t           *p_dev,
                                    const am_mm32l373_rtc_devinfo_t *p_devinfo)
{
    uint32_t           prl_val  = 0;
    uint32_t           rtc_clk  = 0 ;
    amhw_mm32l373_rtc_t *p_hw_rtc = NULL;
    amhw_mm32_pwr_t *p_hw_pwr = NULL;
    amhw_mm32l373_bkp_t *p_hw_bkp = NULL;

    if ((NULL == p_dev) || (NULL == p_devinfo)) {
        return NULL;
    }

    p_hw_rtc                = (amhw_mm32l373_rtc_t *)p_devinfo->rtc_regbase;
    p_hw_pwr                = (amhw_mm32_pwr_t *)p_devinfo->pwr_regbase;
    p_hw_bkp                = (amhw_mm32l373_bkp_t *)p_devinfo->bkp_regbase;
    p_dev->rtc_serv.p_funcs = (struct am_rtc_drv_funcs *)&__g_rtc_drv_funcs;
    p_dev->rtc_serv.p_drv   = p_dev;
    p_dev->int_state        = 0;
    p_dev->rtc_continue     = AM_FALSE;
    p_dev->pfn_callback[0]  = NULL;
    p_dev->pfn_callback[1]  = NULL;
    p_dev->pfn_callback[2]  = NULL;
    p_dev->p_devinfo        = p_devinfo;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    amhw_mm32l373_rcc_apb1_enable(AMHW_MM32L373_RCC_APB1_PWR); /* 使能电源时钟 */
    amhw_mm32l373_rcc_apb1_enable(AMHW_MM32L373_RCC_APB1_BKP); /* 使能备份时钟 */
    amhw_mm32_pwr_bkp_access_enable(p_hw_pwr, 1);           /* 取消备份域的写保护 */

    /* 如果从备份寄存器中读出的值不符合预期，需要重新初始化 */
    if (0x5A5A != amhw_mm32l373_bkp_dr_read(p_hw_bkp, 0)) {

        amhw_mm32l373_rcc_bdcr_bdrst_reset();     /* 备份区域软件复位 */
        amhw_mm32l373_rcc_bdcr_bdrst_reset_end(); /* 备份域软件复位结束 */

        /* 获取 RTC 时钟源，使能时钟并计算预分频值 */
        switch (p_dev->p_devinfo->rtc_clk_sour){

            case AMHW_MM32L373_RTCCLK_LSE:

                /* 使能 LSE */
                amhw_mm32l373_rcc_bdcr_lseon_enable();
                while (!amhw_mm32l373_rcc_bdcr_lserdy_read());

                prl_val = 32768 - 1;
                break;

            case AMHW_MM32L373_RTCCLK_LSI:

                /* 使能 LSI */
                amhw_mm32l373_rcc_lsi_enable();
                while (!amhw_mm32l373_rcc_lsirdy_read());

                prl_val = 40000 - 1;
                break;

            case AMHW_MM32L373_RTCCLK_HSE_DIV128:

                /* 使能 HSE */
                amhw_mm32l373_rcc_hseon_enable();
                while (!amhw_mm32l373_rcc_hserdy_read());

                rtc_clk = am_clk_rate_get(CLK_HSEOSC) / 128;
                prl_val = rtc_clk - 1;
                break;

            default:
               break;
        }

        /* RTC 时钟源选择 */
        amhw_mm32l373_rcc_bdcr_rtc_clk_set((amhw_mm32l373_rtc_clk_src)p_devinfo->rtc_clk_sour);

        /* RTC 时钟使能 */
        amhw_mm32l373_rcc_bdcr_rtc_enable();

        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_RSF);
        __sync_wait(p_hw_rtc); /* 等待 RTC 寄存器同步 */

        __operation_wait(p_hw_rtc); /* 等待 RTC 操作完成 */
        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_RSF);
        amhw_mm32l373_rtc_crl_cnf_enter(p_hw_rtc); /* 允许配置 RTC */
        amhw_mm32l373_rtc_prlh_div_write(p_hw_rtc, prl_val >> 16);
        amhw_mm32l373_rtc_prll_div_write(p_hw_rtc, prl_val & 0xffff);
        amhw_mm32l373_rtc_crl_cnf_out(p_hw_rtc);   /* 配置更新 */
        __operation_wait(p_hw_rtc); /* 等待 RTC 操作完成 */

        amhw_mm32l373_bkp_dr_write(MM32L373_BKP, 0, 0x5A5A);

        p_dev->rtc_continue = AM_FALSE;
    } else {
        if (AMHW_MM32L373_RTCCLK_LSI == p_dev->p_devinfo->rtc_clk_sour) {

            /* 使能 LSI */
            amhw_mm32l373_rcc_lsi_enable();
            while (!amhw_mm32l373_rcc_lsirdy_read());
        } else if (AMHW_MM32L373_RTCCLK_HSE_DIV128 == p_dev->p_devinfo->rtc_clk_sour) {

            /* 使能 HSE */
            amhw_mm32l373_rcc_hseon_enable();
            while (!amhw_mm32l373_rcc_hserdy_read());
        }
        amhw_mm32l373_rtc_clr_status_clear(p_hw_rtc, AMHW_MM32L373_RTC_RSF);
        __sync_wait(p_hw_rtc); /* 等待 RTC 寄存器同步 */
        __operation_wait(p_hw_rtc); /* 等待 RTC 操作完成 */

        p_dev->rtc_continue = AM_TRUE;
    }

    /* 连接中断 */
    am_int_connect(p_devinfo->rtc_inum, __rtc_sec_isr, p_dev);
    am_int_disable(p_devinfo->rtc_inum);

    return &p_dev->rtc_serv;
}

/**
 * \brief 解初始化 RTC
 */
void am_mm32l373_rtc_deinit (am_rtc_handle_t handle)
{
    am_mm32l373_rtc_dev_t *p_dev    = (am_mm32l373_rtc_dev_t *)handle;
    amhw_mm32_pwr_t   *p_hw_pwr    = NULL;

    if ((NULL == p_dev) || (NULL == p_dev->p_devinfo)) {
        return;
    }

    p_hw_pwr = (amhw_mm32_pwr_t *)p_dev->p_devinfo->pwr_regbase;

    p_dev->pfn_callback[0]  = NULL;
    p_dev->pfn_callback[1]  = NULL;
    p_dev->pfn_callback[2]  = NULL;
    p_dev->p_devinfo        = NULL;
    p_dev->rtc_serv.p_funcs = NULL;
    p_dev->rtc_serv.p_drv   = NULL;
    p_dev                   = NULL;

    amhw_mm32l373_bkp_dr_write(MM32L373_BKP, 0, 0);             /* 清除标记 */
    amhw_mm32l373_rcc_apb1_disable(AMHW_MM32L373_RCC_APB1_PWR); /* 禁能电源时钟 */
    amhw_mm32l373_rcc_apb1_disable(AMHW_MM32L373_RCC_APB1_BKP); /* 禁能备份时钟 */
    amhw_mm32_pwr_bkp_access_enable(p_hw_pwr, 0);            /* 备份域的写保护 */
    amhw_mm32l373_rcc_bdcr_rtc_disable();                     /* RTC 时钟失能 */

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/** end of file */
