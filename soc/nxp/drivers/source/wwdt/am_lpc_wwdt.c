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
 * \brief WWDT drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-25  fft, first implementation.
 * \endinternal
 */
 
#include "am_lpc_wwdt.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

static int __wwdt_feed(void *p_drv);
static int __wwdt_enable(void *p_drv, uint32_t timeout_ms);
static int __wwdt_info_get(void *p_drv, am_wdt_info_t *p_info);
    
/** \brief wwdt driver functions */
static const struct am_wdt_drv_funcs __g_wwdt_drv_funcs = {
    __wwdt_info_get,
    __wwdt_enable,
    __wwdt_feed,
};

static int __wwdt_info_get(void *p_drv, am_wdt_info_t *p_info)
{
    am_lpc_wwdt_dev_t *p_dev     = (am_lpc_wwdt_dev_t *)p_drv;
    uint32_t         wdt_freq;
    
    wdt_freq  = am_clk_rate_get(p_dev->p_devinfo->clk_id) / 4;
    
    p_info->min_timeout_ms = (0xFF + 1) * 1000 / wdt_freq;
    p_info->max_timeout_ms = (uint64_t)(0xFFFFFF + 1) / (uint64_t)wdt_freq * 1000;
    
    return AM_OK;
}

static int __wwdt_feed(void *p_drv)
{
    am_lpc_wwdt_dev_t *p_dev     = (am_lpc_wwdt_dev_t *)p_drv;
    amhw_lpc_wwdt_t     *p_hw_wwdt = (amhw_lpc_wwdt_t   *)p_dev->p_devinfo->wwdt_regbase;
    
    amhw_lpc_wwdt_feed(p_hw_wwdt);

    return AM_OK;
}

static int __wwdt_enable(void *p_drv, uint32_t timeout_ms)
{
    am_lpc_wwdt_dev_t *p_dev     = (am_lpc_wwdt_dev_t *)p_drv;
    amhw_lpc_wwdt_t   *p_hw_wwdt = (amhw_lpc_wwdt_t   *)p_dev->p_devinfo->wwdt_regbase;
    uint32_t         wdt_freq;
    uint32_t         ticks;
    
    wdt_freq  = am_clk_rate_get(p_dev->p_devinfo->clk_id) / 4;
    
    ticks = (uint64_t)(timeout_ms) * (uint64_t)1000000 / (1000000000 / wdt_freq);
    
    if (ticks <= 0xFF) {
        ticks = 0xFF;
    } else if (ticks >= 0xFFFFFF) {
        ticks = 0xFFFFFF;
    }
    
    amhw_lpc_wwdt_timeout_set(p_hw_wwdt, ticks);
    
    /* Check if reset is due to Watchdog */
    if (amhw_lpc_wwdt_status_get(p_hw_wwdt) & AMHW_LPC_WWDT_WDMOD_WDTOF) {
        amhw_lpc_wwdt_status_clr(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDTOF);
    }
 
    /* Configure WWDT to reset on time out */
    amhw_lpc_wwdt_option_set(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_LOCK | AMHW_LPC_WWDT_WDMOD_WDRESET);
    
    /* Clear watchdog warning and time out interrupts */
    amhw_lpc_wwdt_status_clr(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDTOF | AMHW_LPC_WWDT_WDMOD_WDINT);
    
    /* The WWDT start up */
    amhw_lpc_wwdt_start(p_hw_wwdt);
    
    return AM_OK;
}

am_wdt_handle_t am_lpc_wwdt_init (am_lpc_wwdt_dev_t           *p_dev,
                                  const am_lpc_wwdt_devinfo_t *p_devinfo)
{
    amhw_lpc_wwdt_t *p_hw_wwdt = NULL;

    
    if (p_devinfo == NULL || p_dev == NULL) {
        return NULL;
    }
    
    p_hw_wwdt               = (amhw_lpc_wwdt_t *)p_devinfo->wwdt_regbase;
    p_dev->p_devinfo        = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_wwdt_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    amhw_lpc_wwdt_init(p_hw_wwdt);
    
    /* Check if reset is due to Watchdog */
    if (amhw_lpc_wwdt_status_get(p_hw_wwdt) & AMHW_LPC_WWDT_WDMOD_WDTOF) {
        amhw_lpc_wwdt_status_clr(p_hw_wwdt, AMHW_LPC_WWDT_WDMOD_WDTOF);
    }
    
    return &(p_dev->wdt_serv);
}

void am_lpc_wwdt_deinit (am_lpc_wwdt_dev_t *p_dev)
{
    /* The WWDT can not deinit */
}

/* end of file */
