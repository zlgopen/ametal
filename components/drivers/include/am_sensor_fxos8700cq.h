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
 * \brief 六轴传感器 FXOS8700CQ 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 19-09-29  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_FXOS8700CQ_H
#define __AM_SENSOR_FXOS8700CQ_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

typedef enum am_sensor_fxos8700cq_m_hms_type{
    AM_FXOS8700CQ_ACCELEROMETER_SENSOR = 0,
    AM_FXOS8700CQ_MAGNETOMETER_SENSOR  = 1,
    AM_FXOS8700CQ_HYBRID_MODE = 3,
} am_sensor_fxos8700cq_m_hms_type_t;

typedef enum am_sensor_fxos8700cq_auto_cal{
    AM_FXOS8700CQ_AUTO_CALIBRATION_FEATURE_DISABLED = 0,
    AM_FXOS8700CQ_AUTO_CALIBRATION_FEATURE_ENABLED,
} am_sensor_fxos8700cq_auto_cal_t;

typedef enum am_sensor_fxos8700cq_hyb_autoinc_mode{
    AM_FXOS8700CQ_HYB_AUTOINC_DISABLED = 0,
    AM_FXOS8700CQ_HYB_AUTOINC_ENABLED,
} am_sensor_fxos8700cq_hyb_autoinc_mode_t;

typedef enum am_sensor_fxos8700cq_acc_wake_mode_osr{
    AM_FXOS8700CQ_NORMAL = 0,
    AM_FXOS8700CQ_LOW_NOISE_LOW_POWER,
    AM_FXOS8700CQ_HIGH_RESOLUTION,
    AM_FXOS8700CQ_LOW_POWER,
} am_sensor_fxos8700cq_acc_wake_mode_osr_t;

typedef enum am_sensor_fxos8700cq_lnoise_mode{
    AM_FXOS8700CQ_LNOISE_NORMAL_MODE = 0,
    AM_FXOS8700CQ_LNOISE_REDUCED_NOISE_MODE,
} am_sensor_fxos8700cq_lnoise_mode_t;

#define AM_FXOS8700CQ_DATA_RATE_1250US          0
#define AM_FXOS8700CQ_DATA_RATE_2500US          1
#define AM_FXOS8700CQ_DATA_RATE_5MS             2
#define AM_FXOS8700CQ_DATA_RATE_10MS            3
#define AM_FXOS8700CQ_DATA_RATE_20MS            4
#define AM_FXOS8700CQ_DATA_RATE_80MS            5
#define AM_FXOS8700CQ_DATA_RATE_160MS           6
#define AM_FXOS8700CQ_DATA_RATE_640MS           7

#define AM_FXOS8700CQ_ODR_ONLY_MODE_800HZ       (AM_FXOS8700CQ_DATA_RATE_1250US)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_400HZ       (AM_FXOS8700CQ_DATA_RATE_2500US)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_200HZ       (AM_FXOS8700CQ_DATA_RATE_5MS)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_100HZ       (AM_FXOS8700CQ_DATA_RATE_10MS)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_50HZ        (AM_FXOS8700CQ_DATA_RATE_20MS)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_12_5HZ      (AM_FXOS8700CQ_DATA_RATE_80MS)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_6_25HZ      (AM_FXOS8700CQ_DATA_RATE_160MS)
#define AM_FXOS8700CQ_ODR_ONLY_MODE_1_56HZ      (AM_FXOS8700CQ_DATA_RATE_640MS)

#define AM_FXOS8700CQ_ODR_HYBRID_MODE_400HZ     (AM_FXOS8700CQ_DATA_RATE_1250US)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_200HZ     (AM_FXOS8700CQ_DATA_RATE_2500US)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_100HZ     (AM_FXOS8700CQ_DATA_RATE_5MS)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_50HZ      (AM_FXOS8700CQ_DATA_RATE_10MS)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_25HZ      (AM_FXOS8700CQ_DATA_RATE_20MS)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_6_25HZ    (AM_FXOS8700CQ_DATA_RATE_80MS)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_3_12HZ    (AM_FXOS8700CQ_DATA_RATE_160MS)
#define AM_FXOS8700CQ_ODR_HYBRID_MODE_0_78HZ    (AM_FXOS8700CQ_DATA_RATE_640MS)

/**
 * \brief 传感器FXOS8700CQ提供的所有通道ID定义
 */
#define AM_FXOS8700CQ_CHAN_1      0           /* FXOS8700CQ 的通道1，X轴加速度      */
#define AM_FXOS8700CQ_CHAN_2      1           /* FXOS8700CQ 的通道2，Y轴加速度      */
#define AM_FXOS8700CQ_CHAN_3      2           /* FXOS8700CQ 的通道3，Z轴加速度      */
#define AM_FXOS8700CQ_CHAN_4      3           /* FXOS8700CQ 的通道4，X轴磁感应强度  */
#define AM_FXOS8700CQ_CHAN_5      4           /* FXOS8700CQ 的通道5，Y轴磁感应强度  */
#define AM_FXOS8700CQ_CHAN_6      5           /* FXOS8700CQ 的通道6，Z轴磁感应强度  */

/**
 * \brief 传感器 FXOS8700CQ 设备信息结构体
 */
typedef struct am_sensor_fxos8700cq_devinfo {

    int     trigger_pin;                                            //数据准备就绪触发引脚

    uint8_t i2c_addr;                                               //I2C 7位 设备地址

    uint8_t odr_selection;                                          //输出数据速率

    am_sensor_fxos8700cq_m_hms_type_t           m_hms_type; 

    uint8_t m_os;                                                   //磁力仪数据过采样率

    am_sensor_fxos8700cq_auto_cal_t             m_acal;             //自校准是否使能

    am_sensor_fxos8700cq_hyb_autoinc_mode_t     hyb_autoinc_mode;   //混合模式下地址自增是否使能

    am_sensor_fxos8700cq_acc_wake_mode_osr_t    mods;               //加速度计唤醒模式OSR模式选择

    am_sensor_fxos8700cq_lnoise_mode_t          lnoise;             //降低噪音模式是否使能 (FSR限制为±2g/±4g)

} am_sensor_fxos8700cq_devinfo_t;

/**
 * \breif 传感器 FXOS8700CQ 设备结构体定义
 */
typedef struct am_sensor_fxos8700cq_dev {
    am_sensor_serv_t               fxos8700cq_dev;  /**< \breif 传感器标准服务   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c设备实例      */
    void                          *p_arg[6];     /**< \brief 触发回调函数参数 */
    uint8_t                        flags[6];     /**< \brief 触发标志位       */
    am_isr_defer_job_t             g_myjob;      /**< \brief 中断延时处理任务 */ 
    uint16_t                       trigger;      /**< \brief 记录标志位       */
    am_sensor_val_t                data[6];      /**< \brief 内部数据缓存     */
    am_sensor_val_t                sam_range;    /**< \brief 采样范围         */
    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t         pfn_trigger_cb[6];
    
    /** \brief 设备信息 */
    const am_sensor_fxos8700cq_devinfo_t *dev_info;

} am_sensor_fxos8700cq_dev_t;

/**
 * \brief 传感器 FXOS8700CQ 初始化
 *
 * \param[in] p_dev     : 指向传感器FXOS8700CQ设备的指针
 * \param[in] p_devinfo : 指向传感器FXOS8700CQ设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_fxos8700cq_init (
        am_sensor_fxos8700cq_dev_t           *p_dev,
        const am_sensor_fxos8700cq_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 FXOS8700CQ 去初始化
 *
 * \param[in] handle : am_sensor_fxos8700cq_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_fxos8700cq_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_FXOS8700CQ_H */

/* end of file */
