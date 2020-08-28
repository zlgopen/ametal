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
 * \brief ZLG217 I2C 从机用户配置文件
 * \sa am_hwconf_hc32f17x_i2c.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-25
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_hc32_i2c_slv.h"
#include "am_hc32_clk.h"
#include "hw/amhw_hc32_i2c.h"

/**
 * \addtogroup am_if_src_hwconf_hc32f17x_i2c_slv
 * \copydoc am_hwconf_hc32f17x_i2c_slv.c
 * @{
 */

/** \brief I2C0 平台初始化函数 */
static void __hc32f17x_i2c0_plfm_init (void)
{
    am_gpio_pin_cfg(PIOB_8, PIOB_8_I2C0_SCL | PIOB_8_OUT_OD);
    am_gpio_pin_cfg(PIOB_9, PIOB_9_I2C0_SDA | PIOB_9_OUT_OD);

    am_clk_enable(CLK_I2C0);
}

/** \brief 解除I2C0 平台初始化函数 */
static void __hc32f17x_i2c0_plfm_deinit (void)
{
    am_clk_disable(CLK_I2C0);
}

/**
 * \name I2C0 控制器参数定义
 * @{
 */

/** @} */

/**
 * \brief I2C0 从机设备信息
 */
static const am_hc32_i2c_slv_devinfo_t __g_i2c0_devinfo = {
    HC32_I2C0_BASE,               /**< \brief I2C0 寄存器块基址 */
    INUM_I2C0,                      /**< \brief I2C0 中断编号 */
    __hc32f17x_i2c0_plfm_init,           /**< \brief 平台初始化 */
    __hc32f17x_i2c0_plfm_deinit          /**< \brief 平台去初始化 */
};

/**< \brief I2C0 设备实例 */
static am_hc32_i2c_slv_dev_t __g_i2c0_dev;

/** \brief I2C0 实例初始化，获得I2C标准服务句柄 */
am_i2c_slv_handle_t  am_hc32_i2c0_slv_inst_init (void)
{
    return am_hc32_i2c_slv_init(&__g_i2c0_dev, &__g_i2c0_devinfo);
}

/** \brief I2C0 实例解初始化 */
void am_hc32_i2c0_slv_inst_deinit (am_i2c_slv_handle_t handle)
{
    am_hc32_i2c_slv_deinit(handle);
}


/** \brief I2C1 平台初始化函数 */
static void __hc32f17x_i2c1_plfm_init (void)
{
    am_gpio_pin_cfg(PIOA_11, PIOA_11_I2C1_SCL | PIOB_11_OUT_OD);
    am_gpio_pin_cfg(PIOA_12, PIOA_12_I2C1_SDA | PIOB_12_OUT_OD);

    am_clk_enable(CLK_I2C1);
}

/** \brief 解除I2C1 平台初始化函数 */
static void __hc32f17x_i2c1_plfm_deinit (void)
{
    am_clk_disable(CLK_I2C1);
}

/**
 * \name I2C1 控制器参数定义
 * @{
 */

/** @} */

/**
 * \brief I2C1 从机设备信息
 */
static const am_hc32_i2c_slv_devinfo_t __g_i2c1_devinfo = {
    HC32_I2C1_BASE,           /**< \brief I2C1寄存器块基址 */
    INUM_I2C1,                  /**< \brief I2C1 中断编号 */
    __hc32f17x_i2c1_plfm_init,       /**< \brief 平台初始化 */
    __hc32f17x_i2c1_plfm_deinit      /**< \brief 平台去初始化 */
};

/**< \brief I2C1 设备实例 */
static am_hc32_i2c_slv_dev_t __g_i2c1_dev;

/** \brief I2C1 实例初始化，获得I2C标准服务句柄 */
am_i2c_slv_handle_t  am_hc32_i2c1_slv_inst_init (void)
{
    return am_hc32_i2c_slv_init(&__g_i2c1_dev, &__g_i2c1_devinfo);
}

/** \brief I2C1 实例解初始化 */
void am_hc32_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle)
{
    am_hc32_i2c_slv_deinit(handle);
}

/** @} */

/* end of file */
