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
 * \brief LPC84X I2C2 用户配置文件
 * \sa am_hwconf_lpc84x_i2c2.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-13  win, first implementation
 * \endinternal
 */

#include <am_lpc84x.h>
#include "ametal.h"
#include "am_lpc_i2c.h"
#include "hw/amhw_lpc84x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc84x_i2c2
 * \copydoc am_hwconf_lpc84x_i2c2.c
 * @{
 */

/**
 * \brief I2C2 平台初始化函数
 */
am_local void __lpc84x_i2c2_plfm_init (void)
{
    am_gpio_pin_cfg(PIO0_9, PIO_FUNC_I2C2_SCL | PIO0_9_OPEN_DRAIN);
    am_gpio_pin_cfg(PIO0_8, PIO_FUNC_I2C2_SDA | PIO0_8_OPEN_DRAIN);

    amhw_lpc84x_clk_i2c0_clk_sel_set (AMHW_LPC84X_CLK_DEVICE_FRG0CLK);
    amhw_lpc84x_clk_periph_enable(AMHW_LPC84X_CLK_I2C2);
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_I2C2);
}

/**
 * \brief I2C2 平台解初始化函数
 */
am_local void __lpc84x_i2c2_plfm_deinit (void)
{
    amhw_lpc84x_syscon_periph_reset(AMHW_LPC84X_RESET_I2C2);
    amhw_lpc84x_clk_periph_disable(AMHW_LPC84X_CLK_I2C2);
}

/** \brief I2C2 总线速率参数定义 */
#define __BUS_SPEED_I2C2     (400000)

/** \brief I2C2 设备信息 */
am_local am_const am_lpc_i2c_devinfo_t __g_lpc84x_i2c2_devinfo = {
    __BUS_SPEED_I2C2,           /* I2C2 总线速率 */
    LPC84X_I2C2_BASE,           /* I2C2 寄存器块基址 */
    INUM_I2C2,                  /* I2C2 中断号 */
    CLK_I2C2,                   /* I2C2 时钟号 */
    10,                         /* 超时时间 */
    __lpc84x_i2c2_plfm_init,    /* 平台初始化函数 */
    __lpc84x_i2c2_plfm_deinit   /* 平台解初始化函数 */
};

/** \brief I2C2 设备实例 */
am_local am_lpc_i2c_dev_t __g_lpc84x_i2c2_dev;

/**
 * \brief I2C2 实例初始化
 */
am_i2c_handle_t am_lpc84x_i2c2_inst_init (void)
{
    return am_lpc_i2c_init(&__g_lpc84x_i2c2_dev, &__g_lpc84x_i2c2_devinfo);
}

/**
 * \brief I2C2 实例解初始化
 */
void am_lpc84x_i2c2_inst_deinit (am_i2c_handle_t handle)
{
    am_lpc_i2c_deinit(handle);
}

/**
 * @}
 */

/* end of file */
