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
 * - 1.00 19-10-16  zp, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_time.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_hc32f460_rtc.h"
#include "hw/amhw_hc32f460_rtc.h"
#include "hc32f460_periph_map.h"
#include <time.h>
#include <string.h>
#include "hc32f460_irq_handle.h"
#include "am_vdebug.h"

/*******************************************************************************
  本地函数声明
*******************************************************************************/
#define   RTC_ALM    (1 << 17)
#define   RTC_PRD    (1 << 18)


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
    __rtc_time_set,  /**< \brief 设置当前的细分时间 */
    __rtc_time_get,  /**< \brief 获取当前的细分时间 */
};
/*******************************************************************************
  本地函数定义
*******************************************************************************/
/**
 * \brief 获取当前的细分时间
 */
am_local int __rtc_time_get (void *p_drv, am_tm_t *p_tm)
{
    am_hc32f460_rtc_dev_t *p_dev    = (am_hc32f460_rtc_dev_t *)p_drv;
    amhw_hc32f460_rtc_t   *p_hw_rtc = (amhw_hc32f460_rtc_t  *)p_dev->p_devinfo->rtc_regbase;
    stc_rtc_date_time_t stcCurrDateTime;
    
    if ((NULL == p_tm) || (NULL == p_tm)) {
        return -AM_EINVAL;
    }

    /* Get current time */
    if (amhw_hc32f460_rtc_get_data_time(p_hw_rtc, RtcDataFormatDec, &stcCurrDateTime) != AM_OK)
    {
        while(1); /* get calendar failed */
    }
    
    p_tm->tm_sec  = stcCurrDateTime.u8Second;
    p_tm->tm_min  = stcCurrDateTime.u8Minute;
    p_tm->tm_hour = stcCurrDateTime.u8Hour;
    p_tm->tm_mday = stcCurrDateTime.u8Day;
    p_tm->tm_wday = stcCurrDateTime.u8Weekday;
    p_tm->tm_mon  = stcCurrDateTime.u8Month;
    p_tm->tm_year = stcCurrDateTime.u8Year + 100;
    
    return AM_OK;
}

/**
 * \brief 设置当前的细分时间
 */
am_local int __rtc_time_set (void *p_drv, am_tm_t *p_tm)
{
    am_hc32f460_rtc_dev_t *p_dev = (am_hc32f460_rtc_dev_t *)p_drv;
    amhw_hc32f460_rtc_t   *p_hw_rtc = (amhw_hc32f460_rtc_t  *)p_dev->p_devinfo->rtc_regbase;
    stc_rtc_date_time_t stcRtcDateTimeCfg;
    
    if ((NULL == p_tm) || (NULL == p_tm)) {
        return -AM_EINVAL;
    }

    /* configure structure initialization */
    memset(&stcRtcDateTimeCfg, 0, sizeof(stcRtcDateTimeCfg));

    /* calendar configuration */
    stcRtcDateTimeCfg.u8Year = p_tm->tm_year - 100;
    stcRtcDateTimeCfg.u8Month = p_tm->tm_mon;
    stcRtcDateTimeCfg.u8Day = p_tm->tm_mday;
    stcRtcDateTimeCfg.u8Weekday = p_tm->tm_wday;
    stcRtcDateTimeCfg.u8Hour = p_tm->tm_hour;
    stcRtcDateTimeCfg.u8Minute = p_tm->tm_min;
    stcRtcDateTimeCfg.u8Second = p_tm->tm_sec;
    if (amhw_hc32f460_rtc_set_data_time(p_hw_rtc,
                                        RtcDataFormatDec,
                                        &stcRtcDateTimeCfg,
                                        RTC_ENABLE, RTC_ENABLE) != AM_OK)
    {
        while(1);  /* write calendar failed */
    }

    return AM_OK;
}

/**
 * \brief RTC 中断服务函数
 */
am_local void __rtc_isr (void *p_arg)
{
    am_hc32f460_rtc_dev_t *p_dev = (am_hc32f460_rtc_dev_t *)p_arg;
    amhw_hc32f460_rtc_t   *p_hw_rtc = NULL;

    p_hw_rtc =(amhw_hc32f460_rtc_t *)p_dev->p_devinfo->rtc_regbase;
    if ((p_hw_rtc->CR2 >> 2) & 1ul)
    {
        p_hw_rtc->CR2 &= (~(1ul << 2));
    }
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/
/**
 * \brief 初始化 RTC
 */
am_rtc_handle_t am_hc32f460_rtc_init (am_hc32f460_rtc_dev_t           *p_dev,
                                      const am_hc32f460_rtc_devinfo_t *p_devinfo)
{
    amhw_hc32f460_rtc_t   *p_hw_rtc  = NULL;
    stc_rtc_init_t stcRtcInit;

    if ((NULL == p_dev) || (NULL == p_devinfo)) {
        return NULL;
    }

    p_hw_rtc                = (amhw_hc32f460_rtc_t *)p_devinfo->rtc_regbase;
    p_dev->rtc_serv.p_funcs = (struct am_rtc_drv_funcs *)&__g_rtc_drv_funcs;
    p_dev->rtc_serv.p_drv   = p_dev;
    p_dev->int_state        = 0;
    p_dev->rtc_continue     = AM_FALSE;
    p_dev->pfn_callback[0]  = NULL;
    p_dev->pfn_callback[1]  = NULL;
    p_dev->p_devinfo        = p_devinfo;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }
    
    /* Reset rtc */
    if (amhw_hc32f460_rtc_deinit(p_hw_rtc) == AM_ERROR)
    {
        while(1);       /* reset rtc failed! */
    } else {
        /* Configuration rtc structure */
        stcRtcInit.enClkSource = RtcClkXtal32;
        stcRtcInit.enPeriodInt = RtcPeriodIntOneSec;
        stcRtcInit.enTimeFormat = RtcTimeFormat24Hour;
        stcRtcInit.enCompenWay = RtcOutputCompenDistributed;
        stcRtcInit.enCompenEn = RTC_DISABLE;
        stcRtcInit.u16CompenVal = 0u;
        amhw_hc32f460_rtc_init(p_hw_rtc, &stcRtcInit);

        /* Enable period interrupt */
        amhw_hc32f460_rtc_irq_cmd(p_hw_rtc, RtcIrqPeriod, RTC_ENABLE);        
        amhw_hc32f460_rtc_irq_cmd(p_hw_rtc, RtcIrqAlarm, RTC_DISABLE);
        p_hw_rtc->CR2_f.ALME = 0;
        /* Startup rtc count */
        amhw_hc32f460_rtc_cmd(p_hw_rtc, RTC_ENABLE);
    }
    
    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, IRQ130_Handler, NULL);
    amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum, (RTC_PRD));
    am_int_enable(p_devinfo->inum);
    p_dev->rtc_continue = AM_TRUE;

    return &p_dev->rtc_serv;
}

/**
 * \brief 解初始化 RTC
 */
void am_hc32f460_rtc_deinit (am_rtc_handle_t handle)
{
    am_hc32f460_rtc_dev_t *p_dev    = (am_hc32f460_rtc_dev_t *)handle;
    amhw_hc32f460_rtc_t   *p_hw_rtc = NULL;

    if ((NULL == p_dev) || (NULL == p_dev->p_devinfo)) {
        return;
    }

    p_hw_rtc = (amhw_hc32f460_rtc_t *)p_dev->p_devinfo->rtc_regbase;

    p_dev->pfn_callback[0]  = NULL;
    p_dev->pfn_callback[1]  = NULL;
    p_dev->p_devinfo        = NULL;
    p_dev->rtc_serv.p_funcs = NULL;
    p_dev->rtc_serv.p_drv   = NULL;
    p_dev                   = NULL;

    /* RTC禁能 */
    amhw_hc32f460_rtc_enable(p_hw_rtc, AM_FALSE);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void RTC_ALM_IrqHandler(void *p_arg)
{
    __rtc_isr(p_arg);
}

void RTC_PRD_IrqHandler(void *p_arg)
{
    __rtc_isr(p_arg);
}

/** end of file */
