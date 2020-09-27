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
 * \brief ZLG116 I2C从机 用户配置文件
 * \sa am_hwconf_zlg116_i2c_slv.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-09-07  vir, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_zlg116.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_zlg_i2c_slv.h"
#include "am_zlg116_clk.h"
#include "hw/amhw_zlg_i2c.h"

/**
 * \addtogroup am_if_src_hwconf_zlg116_i2c_slv
 * \copydoc am_hwconf_zlg116_i2c_slv.c
 * @{
 */

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
 * \brief I2C1从 设备信息
 */
static const am_zlg_i2c_slv_devinfo_t __g_i2c1_devinfo = {

    ZLG116_I2C1_BASE,                /**< \brief I2C1寄存器块基址 */
    INUM_I2C1,                       /**< \brief I2C1 中断编号 */

    __zlg_i2c1_plfm_init,            /**< \brief 平台初始化 */
    __zlg_i2c1_plfm_deinit           /**< \brief 平台去初始化 */
};

static am_zlg_i2c_slv_dev_t __g_i2c1_dev; /**< \brief I2C1 设备实例 */


/** \brief I2C1 实例初始化，获得I2C标准服务句柄 */
am_i2c_slv_handle_t  am_zlg116_i2c1_slv_inst_init (void)
{
    return am_zlg_i2c_slv_init(&__g_i2c1_dev, &__g_i2c1_devinfo);
}

/** \brief I2C1 实例解初始化 */
void am_zlg116_i2c1_slv_inst_deinit (am_i2c_slv_handle_t handle)
{
    am_zlg_i2c_slv_deinit(handle);
}

/** @} */

/* end of file */
