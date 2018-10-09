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
 * \brief KL26 I2C0 用户配置文件
 * \sa am_kl26_hwconfig_i2c0.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-10-27  LXY , first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "hw/amhw_kl26_sim.h"
#include "kl26_periph_map.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_fsl_i2c.h"
#include "am_gpio.h"
#include "kl26_pin.h"
#include "kl26_clk.h"

/**
 * \addtogroup am_kl26_if_hwconfig_src_i2c0
 * \copydoc am_kl26_hwconfig_i2c0.c
 * @{
 */

/**
 * \name I2C0 控制器参数定义
 * @{
 */

#define BUS_SPEED_I2C0     (100000)        /**< \brief I2C0 总线速率参数定义 */

/** @} */

/** \brief I2C0 平台初始化函数 */
void amhw_kl26_plfm_i2c0_init (void)
{
    am_gpio_pin_cfg(PIOB_1, PIOB_1_I2C0_SDA);
    am_gpio_pin_cfg(PIOB_0, PIOB_0_I2C0_SCL);

    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_I2C0);
}

/** \brief 解除I2C0 平台初始化函数 */
void amhw_kl26_plfm_i2c0_deinit (void)
{
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_I2C0);
}

/**
 * \brief I2C0 设备信息
 */
const  struct am_fsl_i2c_devinfo  __g_i2c0_devinfo = {
    KL26_I2C0,
    BUS_SPEED_I2C0,                        /**< \brief 指向I2C0寄存器块的指针 */
    INUM_I2C0,                             /**< \brief I2C0 中断编号 */
	CLK_I2C0,                              /**< \brief I2C0  时钟号 */
    10,                                    /**< \brief I2C0 超时时间设置 */
    amhw_kl26_plfm_i2c0_init,              /** \brief I2C0 平台初始化函数 */
    amhw_kl26_plfm_i2c0_deinit             /** \brief 解除I2C0 平台初始化函数 */
};

am_fsl_i2c_dev_t __g_i2c0_dev;            /**< \brief I2C0 设备实例 */

/** \brief i2c0实例初始化，获得i2c标准服务句柄 */
am_i2c_handle_t am_kl26_i2c0_inst_init (void)
{
    return am_fsl_i2c_init(&__g_i2c0_dev, &__g_i2c0_devinfo);
}

/** \brief i2c0实例解初始化 */
void am_kl26_i2c0_inst_deinit (am_i2c_handle_t handle)
{
    am_fsl_i2c_deinit(handle);
}

/** @} */

/* end of file */
