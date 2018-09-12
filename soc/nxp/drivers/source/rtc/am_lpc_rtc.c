/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief Real-Time Clock(RTC) drivers implementation
 *
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-30  fft, first implementation.
 * \endinternal
 */

#include "am_lpc_rtc.h"
#include "am_int.h"
#include <time.h>

/*******************************************************************************
* Functions declaration
*******************************************************************************/

static int __rtc_time_get(void *p_drv, am_tm_t *p_tm);

static int __rtc_time_set(void *p_drv, am_tm_t *p_tm);

/** \brief RTC driver functions */
static const struct am_rtc_drv_funcs __g_rtc_drv_funcs = {
     __rtc_time_set,
     __rtc_time_get,
};

/******************************************************************************/

static int __rtc_time_get(void *p_drv, am_tm_t *p_tm)
{
    am_lpc_rtc_dev_t *p_dev    = (am_lpc_rtc_dev_t *)p_drv;
    amhw_lpc_rtc_t     *p_hw_rtc = (amhw_lpc_rtc_t  *)p_dev->p_devinfo->rtc_regbase;
    
    uint32_t        sec;

    if (p_tm == NULL) {
        return -AM_EINVAL;
    }

    sec = amhw_lpc_rtc_count_get(p_hw_rtc);

    /* 将秒转换成时间信息 */
    localtime_r((const time_t *)&sec, (struct tm *)p_tm);

    return AM_OK;
}

static int __rtc_time_set(void *p_drv, am_tm_t *p_tm)
{
    uint32_t sec = 0;

    am_lpc_rtc_dev_t  *p_dev     = (am_lpc_rtc_dev_t *)p_drv;
    amhw_lpc_rtc_t    *p_hw_rtc  = (amhw_lpc_rtc_t  *)p_dev->p_devinfo->rtc_regbase;
    
    if (p_tm == NULL) {
        return -AM_EINVAL;
    }

    sec = mktime((struct tm *)p_tm);

    amhw_lpc_rtc_disable(p_hw_rtc);           /* disable RTC before setting count */
    amhw_lpc_rtc_count_set(p_hw_rtc, sec);
    amhw_lpc_rtc_enable(p_hw_rtc);

    return AM_OK;
}

am_rtc_handle_t am_lpc_rtc_init (am_lpc_rtc_dev_t            *p_dev,
                                 const am_lpc_rtc_devinfo_t  *p_devinfo)
{
    amhw_lpc_rtc_t *p_hw_rtc = NULL;
    
    if (p_devinfo == NULL || p_dev == NULL) {
        return NULL;
    }
    
    p_hw_rtc                = (amhw_lpc_rtc_t *)p_devinfo->rtc_regbase;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->rtc_serv.p_funcs = (struct am_rtc_drv_funcs *)&__g_rtc_drv_funcs;
    p_dev->rtc_serv.p_drv   = p_dev; 

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    amhw_lpc_rtc_reset(p_hw_rtc);                   /* RTC reset         */ 
    
    amhw_lpc_rtc_ofd_clr(p_hw_rtc);

    amhw_lpc_rtc_enable(p_hw_rtc);                  /* Enable RTC        */

    return &(p_dev->rtc_serv);
}

void am_lpc_rtc_deinit (am_lpc_rtc_dev_t *p_dev)
{
    amhw_lpc_rtc_t *p_hw_rtc = (amhw_lpc_rtc_t  *)p_dev->p_devinfo->rtc_regbase;

    if (p_dev == NULL) {
    	return;
    }

    p_dev->rtc_serv.p_funcs = NULL;
    p_dev->rtc_serv.p_drv   = NULL; 

    amhw_lpc_rtc_disable(p_hw_rtc);                     /* Disable RTC            */

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
