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
 * \brief WWDG驱动层接口
 * \internal
 * \par Modification history
 * - 1.00 15-07-10  sss, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_hk32f103rbt6_wwdg.h"
#include "am_clk.h"
#include "hw/amhw_hk32f103rbt6_wwdg.h"

/****************************************************************************
 * Functions Declaration
 ***************************************************************************/

static int __wwdg_feed (void     *p_drv);
static int __wwdg_enable (void   *p_drv, uint32_t timeout);
static int __wwdg_info_get (void *p_drv, am_wdt_info_t *p_info);

/** \brief 看门狗标准服务函数结构体 */
static const struct am_wdt_drv_funcs __g_wwdg_drv_funcs = {
    __wwdg_info_get,
    __wwdg_enable,
    __wwdg_feed
};
/** \brief 获取看门狗信息 */
static int __wwdg_info_get (void *p_drv, am_wdt_info_t *p_info)
{
    am_hk32f103rbt6_wwdg_dev_t    *p_dev        = (am_hk32f103rbt6_wwdg_dev_t *)p_drv;
    uint32_t              wdt_freq     = am_clk_rate_get (p_dev->p_devinfo->clk_num) / 4096;
    uint32_t              wdt_freq_max = wdt_freq / 1;
    uint32_t              wdt_freq_min = wdt_freq / 8;

    p_info->min_timeout_ms = 1 * 1000 / wdt_freq_max + 1;
    p_info->max_timeout_ms = (uint64_t)(0x40) * 1000 /
                             (uint64_t)wdt_freq_min  ;

    return AM_OK;
}

/** \brief 喂狗操作 */
static int __wwdg_feed (void *p_drv)
{
    am_hk32f103rbt6_wwdg_dev_t *p_dev     = (am_hk32f103rbt6_wwdg_dev_t *)p_drv;
    amhw_hk32f103rbt6_wwdg_t   *p_hw_wwdg =
                           (amhw_hk32f103rbt6_wwdg_t *)p_dev->p_devinfo->wwdg_regbase;

    amhw_hk32f103rbt6_wwdg_counter_set(p_hw_wwdg, p_dev->count);

    return AM_OK;
}

/** \brief 配置超时时间并启动看门狗 */
static int __wwdg_enable (void *p_drv, uint32_t timeout_ms)
{
    am_hk32f103rbt6_wwdg_dev_t *p_dev     = (am_hk32f103rbt6_wwdg_dev_t *)p_drv;
    amhw_hk32f103rbt6_wwdg_t   *p_hw_wwdg =
                           (amhw_hk32f103rbt6_wwdg_t *)p_dev->p_devinfo->wwdg_regbase;

    uint32_t wdt_freq = am_clk_rate_get (p_dev->p_devinfo->clk_num) / 4096;
    uint32_t ticks;
    uint32_t div = 1;

    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;

    div = ticks / 0x40 + 1;

    amhw_hk32f103rbt6_wwdg_winvalue_set (p_hw_wwdg, 0x7f);

    if (div <= 1) {
        amhw_hk32f103rbt6_wwdg_timerbase_set(p_hw_wwdg, 0);
        p_dev->div = 1;
    } else if (div <= 2) {
        amhw_hk32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 1);
        p_dev->div = 2;
    } else if (div <= 4) {
        amhw_hk32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 2);
        p_dev->div = 4;
    } else if (div <= 8) {
        amhw_hk32f103rbt6_wwdg_timerbase_set (p_hw_wwdg, 3);
        p_dev->div = 8;
    } else {
        return AM_ERROR;
    }

    wdt_freq /= p_dev->div;
    ticks = (uint64_t) (timeout_ms) * wdt_freq / 1000;
    p_dev->count = (ticks + 0x3f) % 0x7f;

    amhw_hk32f103rbt6_wwdg_counter_set (p_hw_wwdg, p_dev->count);

    /* 启动看门狗 */
    amhw_hk32f103rbt6_wwdg_enable(p_hw_wwdg);

    return AM_OK;
}

/** \brief 初始化WWDT，获取标准服务句柄 */
am_wdt_handle_t
am_hk32f103rbt6_wwdg_init (am_hk32f103rbt6_wwdg_dev_t              *p_dev,
                     const am_hk32f103rbt6_wwdg_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo        = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_wwdg_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;
    p_dev->div              = 4;
    p_dev->count            = 0x7f;

    /* 看门狗平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return &(p_dev->wdt_serv);
}

/** \brief WWDT解初始化 */
void am_hk32f103rbt6_wwdg_deinit (am_wdt_handle_t handle)
{
    am_hk32f103rbt6_wwdg_dev_t *p_dev = (am_hk32f103rbt6_wwdg_dev_t *)handle;

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
