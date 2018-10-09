/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X I2C2 用户配置文件
 * \sa am_hwconf_lpc82x_i2c2.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_i2c.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_i2c2
 * \copydoc am_hwconf_lpc82x_i2c2.c
 * @{
 */

/** \brief I2C2 平台初始化函数 */
static void __lpc82x_i2c2_plfm_init (void)
{

    /**
     * PIO0_9 ~ I2C2_SCL, PIO0_8 ~ I2C2_SDA
     */
    am_gpio_pin_cfg(PIO0_9, PIO_FUNC_I2C2_SCL | PIO0_9_OPEN_DRAIN);
    am_gpio_pin_cfg(PIO0_8, PIO_FUNC_I2C2_SDA | PIO0_8_OPEN_DRAIN);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_I2C2);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C2);
}

/** \brief 解除I2C2 平台初始化函数 */
static void __lpc82x_i2c2_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C2);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_I2C2);
}

/**
 * \name I2C2 控制器参数定义
 * @{
 */

#define BUS_SPEED_I2C2     (400000)     /**< \brief I2C2 总线速率参数定义 */

/** @} */

/**
 * \brief I2C2 设备信息
 */
static const am_lpc_i2c_devinfo_t __g_i2c2_devinfo = {

    BUS_SPEED_I2C2,                     /**< \brief I2C2 总线时钟速率 */
    LPC82X_I2C2_BASE,                   /**< \brief I2C2寄存器块基址 */
    INUM_I2C2,                          /**< \brief I2C2 中断编号 */
    CLK_I2C2,                           /**< \brief I2C0 时钟ID */
    0,                                  /**< \brief 超时值0 */

    __lpc82x_i2c2_plfm_init,            /**< \brief 平台初始化 */
    __lpc82x_i2c2_plfm_deinit           /**< \brief 平台去初始化 */
};

static am_lpc_i2c_dev_t __g_i2c2_dev; /**< \brief I2C2 设备实例 */


/** \brief I2C2 实例初始化，获得I2C标准服务句柄 */
am_i2c_handle_t am_lpc82x_i2c2_inst_init (void)
{
    return am_lpc_i2c_init(&__g_i2c2_dev, &__g_i2c2_devinfo);
}

/** \brief I2C2 实例解初始化 */
void am_lpc82x_i2c2_inst_deinit (am_i2c_handle_t handle)
{
    am_lpc_i2c_deinit(handle);
}

/** @} */

/* end of file */
