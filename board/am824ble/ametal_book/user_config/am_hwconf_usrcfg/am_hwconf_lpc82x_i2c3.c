/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X I2C3 用户配置文件
 * \sa am_hwconf_lpc82x_i2c3.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_i2c.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_i2c3
 * \copydoc am_hwconf_lpc82x_i2c3.c
 * @{
 */

/**
 * \brief I2C3 平台初始化函数
 */
am_local void __lpc82x_i2c3_plfm_init (void)
{
    am_gpio_pin_cfg(PIO0_6, PIO_FUNC_I2C3_SCL | PIO0_6_OPEN_DRAIN);
    am_gpio_pin_cfg(PIO0_7, PIO_FUNC_I2C3_SDA | PIO0_7_OPEN_DRAIN);

    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_I2C3);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C3);
}

/**
 * \brief I2C3 平台解初始化函数
 */
am_local void __lpc82x_i2c3_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_I2C3);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_I2C3);
}

/** \brief I2C3 总线速率参数定义 */
#define __BUS_SPEED_I2C3     (400000)

/** \brief I2C3 设备信息 */
am_local am_const am_lpc_i2c_devinfo_t __g_lpc82x_i2c3_devinfo = {
    __BUS_SPEED_I2C3,           /* I2C3 总线速率 */
    LPC82X_I2C3_BASE,           /* I2C3 寄存器块基址 */
    INUM_I2C3,                  /* I2C3 中断号 */
    CLK_I2C3,                   /* I2C3 时钟号 */
    10,                         /* 超时时间 */
    __lpc82x_i2c3_plfm_init,    /* 平台初始化函数 */
    __lpc82x_i2c3_plfm_deinit   /* 平台解初始化函数 */
};

/** \brief I2C3 设备实例 */
am_local am_lpc_i2c_dev_t __g_lpc82x_i2c3_dev;

/**
 * \brief I2C3 实例初始化
 */
am_i2c_handle_t am_lpc82x_i2c3_inst_init (void)
{
    return am_lpc_i2c_init(&__g_lpc82x_i2c3_dev, &__g_lpc82x_i2c3_devinfo);
}

/**
 * \brief I2C3 实例解初始化
 */
void am_lpc82x_i2c3_inst_deinit (am_i2c_handle_t handle)
{
    am_lpc_i2c_deinit(handle);
}

/**
 * @}
 */

/* end of file */
