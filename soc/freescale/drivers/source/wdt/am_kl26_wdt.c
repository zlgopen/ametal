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
 * \brief WWDT drivers implementation
 *
 * 1. 看门狗的配置寄存器只能写一次，所以在使用KS16XX芯片时，当不需要使
 *    用看门狗外设时将工程配置文件里的看门狗开关宏置为0，系统将在板级初
 *    始化时关闭看门狗，当需要使用看门狗外设时，将看门狗开关宏置1，系统将
 *    在看门狗使能函数中配置看门狗，使之正常工作。
 * 2. 看门狗的超时时间只能设置为32ms,256ms,1024ms这三个时间中的一个。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-13  mkr, first implementation.
 * \endinternal
 */

#include "am_kl26_wdt.h"
#include "am_int.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

static int __wdt_feed(void *p_drv);
static int __wdt_enable(void *p_drv, uint32_t timeout_ms);
static int __wdt_info_get(void *p_drv, am_wdt_info_t *p_info);

/** \brief wdt driver functions */
static const struct am_wdt_drv_funcs __g_wwdt_drv_funcs = {
    __wdt_info_get,
    __wdt_enable,
    __wdt_feed,
};

static int __wdt_info_get(void *p_drv, am_wdt_info_t *p_info)
{
    p_info->min_timeout_ms = 32;    /* 2的5次方  LPO */
    p_info->max_timeout_ms = 1024;  /* 2的10次方 LPO */

    return AM_OK;
}

static int __wdt_feed(void *p_drv)
{
    amhw_kl26_sim_srvcop_set(0x55);     /* 喂狗，先写0x55 */
    amhw_kl26_sim_srvcop_set(0XAA);     /* 喂狗，再写0xAA */

    return AM_OK;
}

static int __wdt_enable(void *p_drv, uint32_t timeout_ms)
{
    if (timeout_ms == 32) {
        timeout_ms = KL26_SIM_COP_TIMOUT_VALUE1;   /* 2的5次方LPO 32ms*/
    } else if (timeout_ms == 256) {
        timeout_ms = KL26_SIM_COP_TIMOUT_VALUE2;   /* 2的8次方LPO 256ms*/
    } else if (timeout_ms == 1024) {
        timeout_ms = KL26_SIM_COP_TIMOUT_VALUE3;   /* 2的10次方LPO 1024ms*/
    } else {
        return -AM_EINVAL;
    }
    
    /* 配置为普通模式，时钟为内部1KHZ */
    amhw_kl26_sim_cop_cfg(KL26_SIM_COP_MODE_NORMAL,
                          KL26_SIM_COP_CLK_INTERNAL,
                          (amhw_kl26_sim_cop_timeout_t)timeout_ms);

    return AM_OK;
}

am_wdt_handle_t am_kl26_wdt_init (am_kl26_wdt_dev_t           *p_dev,
                                  const am_kl26_wdt_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL || p_devinfo->p_hw_wdt == NULL) {
        return NULL;
    }

    p_dev->p_devinfo        = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_wwdt_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;

    return &(p_dev->wdt_serv);
}

void am_kl26_wdt_deinit (am_wdt_handle_t handle)
{
    /* The WWDT can not deinit */
    am_kl26_wdt_dev_t *p_dev = (am_kl26_wdt_dev_t *)handle;

    /* 禁能软件看门狗 */
    amhw_kl26_sim_cop_cfg(KL26_SIM_COP_MODE_NORMAL,
                          KL26_SIM_COP_CLK_INTERNAL,
                          KL26_SIM_COP_DISABLED);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

}

/* end of file */
