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
 * \brief ZLG DAC 用户配置文件
 * \sa am_hwconf_mm32l373_dac.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-08-22  fra, first implementation
 * \endinternal
 */

#include "am_mm32l373.h"
#include "am_mm32_dac.h"
#include "am_gpio.h"
#include "am_clk.h"

/**
 * \addtogroup am_if_src_hwconf_mm32l373_dac
 * \copydoc am_hwconf_mm32l373_dac.c
 * @{
 */

/**
 * \brief DAC1 平台初始化
 */
void __mm32l373_plfm_dac1_init (void)
{

    /* 配置PIOA_4为DAC1_OUT功能 */
    am_gpio_pin_cfg(PIOA_4, PIOA_4_DAC1_OUT);

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);
}

/**
 * \brief DAC2 平台去初始化
 */
void __mm32l373_plfm_dac1_deinit (void)
{
    /* 关闭DAC时钟 */
    am_clk_disable(CLK_DAC);
}

/** \brief DAC1 设备信息 */
static const am_mm32_dac_devinfo_t __g_dac1_devinfo =
{
    MM32L373_DAC_BASE,                  /**< \brief 指向DAC寄存器块的指针 */
    12,                               /**< \brief DAC转换精度 */
    3270,                             /**< \brief DAC参考电压，单位：mV */
    __mm32l373_plfm_dac1_init,
    __mm32l373_plfm_dac1_deinit,
};

/** \brief DAC1设备实例 */
static am_mm32_dac_dev_t __g_dac1_dev;

/** \brief DAC1 实例初始化，获得DAC标准服务句柄 */
am_dac_handle_t am_mm32l373_dac1_inst_init (void)
{
    return am_mm32_dac_init(&__g_dac1_dev, &__g_dac1_devinfo);
}

/** \brief DAC1 实例解初始化 */
void am_mm32l373_dac1_inst_deinit (am_dac_handle_t handle)
{
    am_mm32_dac_deinit(handle);
}

/**
 * \brief DAC2 平台初始化
 */
void __mm32l373_plfm_dac2_init (void)
{
    /* 配置PIOA_5为DAC2_OUT功能 */
    am_gpio_pin_cfg (PIOA_5,PIOA_5_DAC2_OUT);

    /* 开启DAC时钟 */
    am_clk_enable(CLK_DAC);
}

/**
 * \brief DAC2 平台去初始化
 */
void __mm32l373_plfm_dac2_deinit (void)
{
    /* 关闭DAC时钟 */
    am_clk_disable(CLK_DAC);
}

/** \brief DAC2 设备信息 */
static const am_mm32_dac_devinfo_t __g_dac2_devinfo =
{
    MM32L373_DAC_BASE,                  /**< \brief 指向DAC寄存器块的指针 */
    12,                               /**< \brief DAC转换精度 */
    3270,                             /**< \brief DAC参考电压，单位：mV */
    __mm32l373_plfm_dac2_init,
    __mm32l373_plfm_dac2_deinit,
};

/** \brief DAC2设备实例 */
static am_mm32_dac_dev_t __g_dac2_dev;

/** \brief DAC2 实例初始化，获得DAC标准服务句柄 */
am_dac_handle_t am_mm32l373_dac2_inst_init (void)
{
    return am_mm32_dac_init(&__g_dac2_dev, &__g_dac2_devinfo);
}

/** \brief DAC2 实例解初始化 */
void am_mm32l373_dac2_inst_deinit (am_dac_handle_t handle)
{
    am_mm32_dac_deinit(handle);
}

/**
 * @}
 */

/* end of file */
