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
 * \brief LTR_553ALS 配置文件
 * \sa am_hwconf_ltr_553als.c
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-10  yrz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_ltr_553als_01.h"
#include "am_zlg217_inst_init.h"
#include "zlg217_pin.h"

/**
 * \addtogroup am_if_src_hwconf_ltr_553als
 * \copydoc am_hwconf_ltr_553als.c
 * @{
 */

/** \brief 设备信息 */
am_local am_const am_ltr_553als_devinfo_t __g_ltr_553als_devinfo = {
    ALS_GAIN_48X,                             /**< \brief 0.02~1.3K lux       */
    PS_GAIN_32X,                              /**< \brief X32                 */
    LED_PULSE_FREQ_60KHz,                     /**< \brief LED脉冲频率60KHz    */
    LED_CURRENT_DUTY_100,                     /**< \brief LED电流占空比为100% */
    LED_CURRENT_100MA,                        /**< \brief LED电流100mA        */
    LED_PULSE_NUM_1,                          /**< \brief LED发射1个脉冲      */ 
    PS_MEAS_RATE_100MS,                       /**< \brief 距离测量速度为100ms */
    ALS_INTEGRATE_100MS,                      /**< \brief 光照积分时间为100ms */
    ALS_MEAS_RATE_500MS                       /**< \brief 光强测量速度为500ms */
};

/*******************************************************************************
  LTR_553ALS 实例初始化
*******************************************************************************/

/** \brief 设备定义 */
am_local am_ltr_553als_dev_t __g_ltr_553als_dev;

/** \brief 实例初始化 */
am_ltr_553als_handle_t am_ltr_553als_inst_init (void)
{
    /* 获取 I2C 实例句柄 */
    am_i2c_handle_t i2c_handle =  am_zlg217_i2c1_inst_init();

    return am_ltr_553als_init (&__g_ltr_553als_dev, 
                               &__g_ltr_553als_devinfo,
                               i2c_handle);   
}

/**
 * @}
 */

/* end of file */
