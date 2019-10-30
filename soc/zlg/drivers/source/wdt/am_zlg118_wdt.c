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
 * \brief WDT驱动层接口
 * \internal
 * \par Modification history
 * - 1.00 19-10-10
 * \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_zlg118_wdt.h"

/* 看门狗清零代码 */
#define ZLG118_WDT_RST_COUNT1   (0x1e)
#define ZLG118_WDT_RST_COUNT2   (0xe1)

/****************************************************************************
 * Functions Declaration
 ***************************************************************************/

static int __wdt_feed (void     *p_drv);
static int __wdt_enable (void   *p_drv, uint32_t timeout);
static int __wdt_info_get (void *p_drv, am_wdt_info_t *p_info);

/** \brief 看门狗标准服务函数结构体 */
static const struct am_wdt_drv_funcs __g_wdt_drv_funcs = {
    __wdt_info_get,
    __wdt_enable,
    __wdt_feed
};

/** \brief 获取看门狗信息 */
static int __wdt_info_get (void *p_drv, am_wdt_info_t *p_info)
{
    p_info->min_timeout_ms = 2;
    p_info->max_timeout_ms = 50000;

    return AM_OK;
}

/** \brief 喂狗操作 */
static int __wdt_feed (void *p_drv)
{
    am_zlg118_wdt_dev_t *p_dev     = (am_zlg118_wdt_dev_t *)p_drv;
    amhw_zlg118_wdt_t   *p_hw_wdt =
                           (amhw_zlg118_wdt_t *)p_dev->p_devinfo->wdt_regbase;

    /* 清零WDT定时器及中断标志 */
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT1);
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT2);

    return AM_OK;
}

/** \brief 配置超时时间并启动看门狗 */
static int __wdt_enable (void *p_drv, uint32_t timeout_ms)
{
    am_zlg118_wdt_dev_t *p_dev     = (am_zlg118_wdt_dev_t *)p_drv;
    amhw_zlg118_wdt_t   *p_hw_wdt =
                           (amhw_zlg118_wdt_t *)p_dev->p_devinfo->wdt_regbase;

    if(timeout_ms > ZLG118_WDT_52_4S) {
        return AM_ERROR;
    }

    /* 设置溢出时间 */
    amhw_zlg118_wdt_wov_set(p_hw_wdt, (uint8_t)timeout_ms);

    /* 设置溢出后复位 */
    amhw_zlg118_wdt_en_set(p_hw_wdt, 0);

    /* 启动看门狗 */
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT1);
    amhw_zlg118_wdt_rst_set(p_hw_wdt, ZLG118_WDT_RST_COUNT2);

    return AM_OK;
}

/** \brief 初始化WDT，获取标准服务句柄 */
am_wdt_handle_t
am_zlg118_wdt_init (am_zlg118_wdt_dev_t           *p_dev,
                    const am_zlg118_wdt_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo        = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_wdt_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;

    /* 看门狗平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return &(p_dev->wdt_serv);
}

/** \brief WDT解初始化 */
void am_zlg118_wdt_deinit (am_wdt_handle_t handle)
{
    am_zlg118_wdt_dev_t *p_dev = (am_zlg118_wdt_dev_t *)handle;

    if (NULL == p_dev) {
        return ;
    }

    p_dev->p_devinfo        = NULL;

    p_dev->wdt_serv.p_funcs = NULL;
    p_dev->wdt_serv.p_drv   = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
