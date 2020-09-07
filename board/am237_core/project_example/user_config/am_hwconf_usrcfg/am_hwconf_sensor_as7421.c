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
 * \brief 传感器 AS7421 配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#include "am_sensor_as7421.h"
#include "am_common.h"
#include "zlg237_pin.h"
#include "am_zlg237_inst_init.h"

am_const am_local as7421_param_config_t __g_as7421_param_default = {
    AM_FALSE,                                       /* LED_WAIT_OFF    */
    AM_FALSE,                                       /* WAIT_CYCLE_ON   */
    0x0f,                                           /* LED_MULT        */
    10,                                             /* LED_WAIT(t = LED_WAIT * 1024us) */
    1023,                                           /* LTF_CCOUNT      */
    AM_AS7421_INT_PINMAP_INTX,                      /* INT_PINMAP      */
    AM_AS7421_INT_INVERT_DISABLE,                   /* INT_INVERT      */
    AM_AS7421_GPIO_PINMAP_GPIO,                     /* GPIO_PINMAP     */
    AM_AS7421_LED_AUTO_LEDS_OFF,                    /* LED_AUTO        */
    AM_AS7421_CLKMOD_1MHZ,                          /* CLKMOD          */
    65535,                                          /* LTF_ITIME (Tint = (LTF_ITIME + 1) / CLKMOD)  */
    20000,                                          /* LTF_WTIME (Twait = (LTF_WTIME + 1) / CLKMOD)      */
    AM_AS7421_INTEGRATION_CYCLE_4,                  /* LTF_CYCLE       */
    AM_AS7421_SET_LED_ON_LED_DISABLE,               /* SET_LED_ON      */
    AM_AS7421_LED_OFF_EN_NORMAL_MODE,               /* LED_OFF_EN      */
    AM_AS7421_LED_CURRENT_50MA,                     /* LED_CURRENT     */
    1,                                              /* LTF_ICOUNT      */
    0,                                              /* GPIO_INVERT     */
    0,                                              /* GPIO_OEN        */
    0,                                              /* GPIO_OUT        */
    AM_AS7421_AZ_WTIME_32US,                        /* AZ_WTIME        */
    0,                                              /* AZ_ITERATION    */
};

/** \brief 传感器 AS7421 设备信息实例 */
am_const am_local struct am_sensor_as7421_devinfo __g_as7421_info = {
        &__g_as7421_param_default,              /**< \brief 参数初始化 */
        PIOA_7,                                 /**< \brief INT引脚 */
        PIOC_10,                                /**< \brief GPIO引脚 */
        -1,                                     /**< \brief RST引脚 */
        0x64,                                   /**< \brief I2C 7位 设备地址 */
};

/** \breif 传感器 AS7421 设备结构体定义 */
am_local struct am_sensor_as7421_dev __g_as7421_dev;

/** \brief 传感器 AS7421 设备实例化 */
am_sensor_handle_t am_sensor_as7421_inst_init (void)
{
    return am_sensor_as7421_init(&__g_as7421_dev,
                                 &__g_as7421_info,
                                 am_zlg237_i2c2_inst_init());
}

/** \brief 传感器 AS7421 实例解初始化 */
am_err_t am_sensor_as7421_inst_deinit (am_sensor_handle_t handle)
{
    return am_sensor_as7421_deinit(handle);
}

/* end of file */
