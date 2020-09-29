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
 * \brief ZLG116 I2C 用户配置文件
 * \sa am_hwconf_zlg116_i2c.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg116.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_zlg_i2c.h"
#include "am_zlg116_clk.h"
#include "hw/amhw_zlg_i2c.h"

/**
 * \addtogroup am_if_src_hwconf_zlg116_i2c
 * \copydoc am_hwconf_zlg116_i2c.c
 * @{
 */

/** \brief I2C1 总线恢复函数 */
static void __zlg_i2c1_bus_clean (void)
{
    uint8_t i;

    am_gpio_pin_cfg(PIOB_6, AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(PIOB_7, AM_GPIO_PULLUP | AM_GPIO_INPUT);

    if (am_gpio_get(PIOB_7) != 1) {
        for (i = 0; i < 9; i++) {
            am_gpio_set(PIOB_6, 0);
            am_gpio_set(PIOB_6, 1);
        }
    }

    am_gpio_pin_cfg(PIOB_6, PIOB_6_I2C_SCL | PIOB_6_AF_OD);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_I2C_SDA | PIOB_7_AF_OD);
}

/** \brief I2C1 平台初始化函数 */
static void __zlg_i2c1_plfm_init (void)
{

    /**
     * PIOB_6 ~ I2C1_SCL, PIOB_7 ~ I2C1_SDA
     */
    am_gpio_pin_cfg(PIOB_6, PIOB_6_I2C_SCL | PIOB_6_AF_OD | PIOB_6_SPEED_20MHz);
    am_gpio_pin_cfg(PIOB_7, PIOB_7_I2C_SDA | PIOB_7_AF_OD | PIOB_7_SPEED_20MHz);

    am_clk_enable(CLK_I2C1);
    am_zlg116_clk_reset(CLK_I2C1);
}

/** \brief 解除I2C1 平台初始化函数 */
static void __zlg_i2c1_plfm_deinit (void)
{
    am_clk_disable(CLK_I2C1);
}

/**
 * \name I2C1 控制器参数定义
 * @{
 */

/** @} */

/**
 * \brief I2C1 设备信息
 */
static const am_zlg_i2c_devinfo_t __g_i2c1_devinfo = {

    ZLG116_I2C1_BASE,                 /**< \brief I2C1寄存器块基址 */
    CLK_I2C1,                         /**< \brief 时钟ID值 */
    INUM_I2C1,                        /**< \brief I2C1 中断编号 */

    100000,                           /**< \brief I2C 速率 */
    10,                               /**< \brief 超时值10 */
    __zlg_i2c1_bus_clean,             /**< \brief 总线恢复函数 */
    __zlg_i2c1_plfm_init,             /**< \brief 平台初始化 */
    __zlg_i2c1_plfm_deinit            /**< \brief 平台去初始化 */
};

static am_zlg_i2c_dev_t __g_i2c1_dev;           /**< \brief I2C1 设备实例 */
static am_i2c_handle_t  __g_i2c1_handle = NULL; /**< \brief I2C 标准服务句柄 */

/** \brief I2C1 实例初始化，获得I2C标准服务句柄 */
am_i2c_handle_t am_zlg116_i2c1_inst_init (void)
{
    if (NULL == __g_i2c1_handle) {
        __g_i2c1_handle = am_zlg_i2c_init(&__g_i2c1_dev, &__g_i2c1_devinfo);
    }

    return __g_i2c1_handle;
}

/** \brief I2C1 实例解初始化 */
void am_zlg116_i2c1_inst_deinit (am_i2c_handle_t handle)
{
    am_zlg_i2c_deinit(handle);
    __g_i2c1_handle = NULL;
}

/** @} */

/* end of file */
