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
 * \brief HC32F460 TRNG 用户配置文件
 * \sa am_hwconf_hc32f460_trng.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-26
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32f460.h"
#include "am_hc32f460_trng.h"
#include "hw/amhw_hc32f460_trng.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f460_trng
 * \copydoc am_hwconf_hc32f460_trng.c
 * @{
 */

/**
 * \brief TRNG 平台初始化
 */
void __hc32f460_plfm_trng_init (void)
{
    /** \brief 开启TRNG时钟 */
    am_clk_enable (CLK_TRNG);
}

/**
 * \brief TRNG 平台去初始化
 */
void __hc32f460_plfm_trng_deinit (void)
{
    /** \brief 关闭TRNG时钟 */
    am_clk_disable (CLK_TRNG);
}

/** \brief TRNG 设备信息 */
static const am_hc32f460_trng_devinfo_t __g_trng_devinfo =
{
    /** \brief trng寄存器块基地址  */
    HC32F460_TRNG_BASE,

    /**
     * \brief TRNG 反馈移位次数
     */
    AMHW_HC32F460_TRNG_CNT_256,

    /** \brief trng平台初始化函数 */
    __hc32f460_plfm_trng_init,

    /** \brief trng平台解初始化函数  */
    __hc32f460_plfm_trng_deinit,
};

/** \brief TRNG设备实例 */
static am_hc32f460_trng_dev_t __g_trng_dev;

/** \brief TRNG 实例初始化，获得TRNG标准服务句柄 */
am_trng_handle_t am_hc32f460_trng_inst_init (void)
{
    return am_hc32f460_trng_init(&__g_trng_dev, &__g_trng_devinfo);
}

/** \brief TRNG 实例解初始化 */
void am_hc32f460_trng_inst_deinit (am_trng_handle_t handle)
{
    am_hc32f460_trng_deinit(handle);
}

/**
 * @}
 */

/* end of file */
