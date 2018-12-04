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
 * \brief KL26 WDT 用户配置文件
 * \sa am_kl26_hwconfig_wdt.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */
#include "am_kl26_wdt.h"

#include "../../../../../soc/freescale/kl26/kl26_periph_map.h"
/**
 * \addtogroup am_kl26_if_hwconfig_src_wdt
 * \copydoc am_kl26_hwconfig_wdt.c
 * @{
 */

/** \brief WDT 平台初始化 */
static void __kl26_wdt_plfm_init (void)
{
    /* 不需要做其他操作 */
}

/** \brief 解除WDT 平台初始化 */
static void __kl26_wdt_plfm_deinit (void)
{
    /* 不需要做其他操作 */
}

/** \brief WDT 设备信息 */
static const am_kl26_wdt_devinfo_t  __g_wdt_devinfo = {
		KL26_SIM,  /* 指向SIM寄存器块的指针 */

		__kl26_wdt_plfm_init,

		__kl26_wdt_plfm_deinit,

};

/** \brief WDT设备实例  */
static am_kl26_wdt_dev_t            __g_wdt_dev;

/**
 * \brief  WDT 实例初始化，获得WDT标准服务句柄
 *
 * \return WDT句柄值
 */
am_wdt_handle_t am_kl26_wdt_inst_init (void)
{
    return am_kl26_wdt_init(&__g_wdt_dev, &__g_wdt_devinfo);
}

/**
 * \brief WDT 实例解初始化
 *
 * \param[in] handle : WDT看门狗句柄值
 */
void am_kl26_wdt_inst_deinit (am_wdt_handle_t handle)
{
	am_kl26_wdt_deinit(handle);
}


/**
 * @}
 */

/* end of file */
