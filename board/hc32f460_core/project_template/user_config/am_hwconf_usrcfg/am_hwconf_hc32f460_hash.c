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
 * \brief HC32F460 HASH 用户配置文件
 * \sa am_hwconf_hc32f460_hash.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32f460.h"
#include "am_hc32f460_hash.h"
#include "hw/amhw_hc32f460_hash.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_hash
 * \copydoc am_hwconf_hc32f460_hash.c
 * @{
 */

/**
 * \brief HASH 平台初始化
 */
static void __hc32f460_plfm_hash_init (void)
{
    /* 开启HASH钟 */
    am_clk_enable(CLK_HASH);
}

/**
 * \brief HASH 平台去初始化
 */
static void __hc32f460_plfm_hash_deinit (void)
{
    /* 关闭HASH时钟 */
    am_clk_disable(CLK_HASH);
}

/** \brief HASH 设备信息 */
static const am_hc32f460_hash_devinfo_t __g_hash_devinfo =
{
    /** \brief 指向hash寄存器块的指针 */
    HC32F460_HASH_BASE,

    /** \brief hash平台初始化函数 */
    __hc32f460_plfm_hash_init,

    /** \brief hash平台解初始化函数  */
    __hc32f460_plfm_hash_deinit,
};

/** \brief HASH设备实例 */
static am_hc32f460_hash_dev_t __g_hash_dev;

/** \brief HASH 实例初始化，获得HASH标准服务句柄 */
am_hash_handle_t am_hc32f460_hash_inst_init (void)
{
    return am_hc32f460_hash_init(&__g_hash_dev, &__g_hash_devinfo);
}

/** \brief HASH 实例解初始化 */
void am_hc32f460_hash_inst_deinit (am_hash_handle_t handle)
{
    am_hc32f460_hash_deinit(handle);
}

/**
 * @}
 */

/* end of file */
