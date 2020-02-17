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
 * \brief IWDG驱动层接口
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  sss, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mm32_iwdg.h"
#include "hw/amhw_mm32_iwdg.h"

/****************************************************************************
 * Functions Declaration
 ***************************************************************************/

static int __iwdg_feed (void *p_drv);
static int __iwdg_enable (void *p_drv, uint32_t timeout);
static int __iwdg_info_get (void *p_drv, am_wdt_info_t *p_info);

/** \brief 看门狗标准服务函数结构体 */
static const struct am_wdt_drv_funcs __g_iwdg_drv_funcs = {
    __iwdg_info_get,
    __iwdg_enable,
    __iwdg_feed
};

/** \brief 获取看门狗信息 */
static int __iwdg_info_get (void *p_drv, am_wdt_info_t *p_info)
{
    uint32_t wdt_freq      = 40000;
    uint32_t wdt_freq_max  = wdt_freq / 4;
    uint32_t wdt_freq_min  = wdt_freq / 256;

    p_info->min_timeout_ms = 1 * 1000 / wdt_freq_max + 1;
    p_info->max_timeout_ms = (uint64_t)(0xFFF + 1) * 1000 /
                             (uint64_t)wdt_freq_min  ;

    return AM_OK;
}

/** \brief 喂狗操作 */
static int __iwdg_feed (void *p_drv)
{
    am_mm32_iwdg_dev_t *p_dev     = (am_mm32_iwdg_dev_t *)p_drv;
    amhw_mm32_iwdg_t   *p_hw_iwdg =
                           (amhw_mm32_iwdg_t *)p_dev->p_devinfo->iwdg_regbase;

    amhw_mm32_iwdg_keyvalue_set(p_hw_iwdg, 0xAAAA);

    return AM_OK;
}

/** \brief 配置超时时间并启动看门狗 */
static int __iwdg_enable (void *p_drv, uint32_t timeout_ms)
{
    am_mm32_iwdg_dev_t *p_dev     = (am_mm32_iwdg_dev_t *)p_drv;
    amhw_mm32_iwdg_t   *p_hw_iwdg =
                           (amhw_mm32_iwdg_t *)p_dev->p_devinfo->iwdg_regbase;

    uint32_t wdt_freq = 40000;
    uint32_t ticks;
    uint32_t div = 1;

    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    div = ticks / 0xFFF + 1;

    amhw_mm32_iwdg_keyvalue_set(p_hw_iwdg, 0x5555);
    while(amhw_mm32_iwdg_status_get(p_hw_iwdg) & 0x1ul);

    if (div <= 4) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 0);
        p_dev->div = 4;
    } else if (div <= 8) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 1);
        p_dev->div = 8;
    } else if (div <= 16) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 2);
        p_dev->div = 16;
    } else if (div <= 32) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 3);
        p_dev->div = 32;
    } else if (div <= 64) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 4);
        p_dev->div = 64;
    } else if (div <= 128) {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 5);
        p_dev->div = 128;
    } else {
        amhw_mm32_iwdg_div_set (p_hw_iwdg, 6);
        p_dev->div = 256;
    }

    wdt_freq /= p_dev->div;
    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    amhw_mm32_iwdg_keyvalue_set(p_hw_iwdg, 0x5555);
    while(amhw_mm32_iwdg_status_get(p_hw_iwdg) & 0x2ul);
    amhw_mm32_iwdg_reload_set (p_hw_iwdg, ticks);

    /* 启动看门狗 */
    amhw_mm32_iwdg_keyvalue_set(p_hw_iwdg, 0xAAAA);
    amhw_mm32_iwdg_keyvalue_set(p_hw_iwdg, 0xCCCC);

    return AM_OK;
}

/** \brief 初始化IWDG，获取标准服务句柄 */
am_wdt_handle_t am_mm32_iwdg_init (am_mm32_iwdg_dev_t           *p_dev,
                                  const am_mm32_iwdg_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo        = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_iwdg_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;
    p_dev->div              = 4;

    /* 看门狗平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return &(p_dev->wdt_serv);
}

/** \brief IWDG解初始化 */
void am_mm32_iwdg_deinit (am_wdt_handle_t handle)
{
    am_mm32_iwdg_dev_t *p_dev = (am_mm32_iwdg_dev_t *)handle;

    if (NULL == p_dev) {
        return ;
    }

    p_dev->p_devinfo = NULL;

    p_dev->wdt_serv.p_funcs = NULL;
    p_dev->wdt_serv.p_drv   = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
