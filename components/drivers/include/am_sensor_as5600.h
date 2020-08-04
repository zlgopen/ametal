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
 * \brief 角度传感器 AS5600 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS5600_H
#define __AM_SENSOR_AS5600_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 AS5600 Watchdog
 */
#define AM_AS5600_WD_OFF                        (0x00)
#define AM_AS5600_WD_ON                         (0x01)

/**
 * \brief 传感器 AS5600 Fast Filter Threshold
 */
#define AM_AS5600_FTH_SLOW_FILTER_ONLY          (0x00)
#define AM_AS5600_FTH_6_LSBS                    (0x01)
#define AM_AS5600_FTH_7_LSBS                    (0x02)
#define AM_AS5600_FTH_9_LSBS                    (0x03)
#define AM_AS5600_FTH_18_LSBS                   (0x04)
#define AM_AS5600_FTH_21_LSBS                   (0x05)
#define AM_AS5600_FTH_24_LSBS                   (0x06)
#define AM_AS5600_FTH_10_LSBS                   (0x07)

/**
 * \brief 传感器 AS5600 Slow Filter
 */
#define AM_AS5600_SF_16                         (0x00)  /* 强制在低功耗模式(LPM) */
#define AM_AS5600_SF_8                          (0x01)
#define AM_AS5600_SF_4                          (0x02)
#define AM_AS5600_SF_2                          (0x03)

/**
 * \brief 传感器 AS5600 PWM Frequency
 */
#define AM_AS5600_PWMF_115_HZ                   (0x00)
#define AM_AS5600_PWMF_230_HZ                   (0x01)
#define AM_AS5600_PWMF_460_HZ                   (0x02)
#define AM_AS5600_PWMF_920_HZ                   (0x03)

/**
 * \brief 传感器 AS5600 Output Stage
 */
#define AM_AS5600_OUTS_ANALOG_0_TO_100          (0x00)
#define AM_AS5600_OUTS_ANALOG_10_TO_90          (0x01)
#define AM_AS5600_OUTS_DIGITAL_PWM              (0x02)

/**
 * \brief 传感器 AS5600 Hysteresis
 */
#define AM_AS5600_HYST_OFF                      (0x00)
#define AM_AS5600_HYST_1_LSB                    (0x01)
#define AM_AS5600_HYST_2_LSBS                   (0x02)
#define AM_AS5600_HYST_3_LSBS                   (0x03)

/**
 * \brief 传感器 AS5600 Power Mode
 */
#define AM_AS5600_PM_NOM                        (0x00)
#define AM_AS5600_PM_LPM1                       (0x01)
#define AM_AS5600_PM_LPM2                       (0x02)
#define AM_AS5600_PM_LPM3                       (0x03)

/**
 * \brief 传感器 AS5600 Burn Commands
 */
#define AM_AS5600_BURN_ANGLE                    (0x80)
#define AM_AS5600_BURN_SETTING                  (0x40)

/**
 * \brief 传感器 AS5600 Direction polarity
 */
#define AM_AS5600_INCREASE_CLOCKWISE            (0x00)
#define AM_AS5600_INCREASE_COUNTERCLOCKWISE     (0x01)

/**
 * \brief 传感器 AS5600 option
 */
#define AM_AS5600_OPTION_DISABLE                (0x00)
#define AM_AS5600_OPTION_ENABLE                 (0x01)

/**
 * \brief 传感器 AS5600 programming the angular range
 */
#define AM_AS5600_OPTION_A                      (0x00)
#define AM_AS5600_OPTION_B                      (0x01)
#define AM_AS5600_OPTION_C                      (0x02)

/**
 * \brief 传感器 AS5600 控制指令定义
 */
typedef enum am_sensor_as5600_control_cmd {
    AM_SENSOR_AS5600_ZMCO_GET = 1,
    AM_SENSOR_AS5600_ZPOS_SET,
    AM_SENSOR_AS5600_ZPOS_GET,
    AM_SENSOR_AS5600_MPOS_SET,
    AM_SENSOR_AS5600_MPOS_GET,
    AM_SENSOR_AS5600_MANG_SET,
    AM_SENSOR_AS5600_MANG_GET,
    AM_SENSOR_AS5600_WD_SET,
    AM_SENSOR_AS5600_FTH_SET,
    AM_SENSOR_AS5600_SF_SET,
    AM_SENSOR_AS5600_PWMF_SET,
    AM_SENSOR_AS5600_OUTS_SET,
    AM_SENSOR_AS5600_HYST_SET,
    AM_SENSOR_AS5600_PM_SET,
    AM_SENSOR_AS5600_CONF_GET,
    AM_SENSOR_AS5600_RAW_ANGLE_GET,
    AM_SENSOR_AS5600_ANGLE_GET,
    AM_SENSOR_AS5600_MD_GET,
    AM_SENSOR_AS5600_ML_GET,
    AM_SENSOR_AS5600_MH_GET,
    AM_SENSOR_AS5600_AGC_GET,
    AM_SENSOR_AS5600_MAGNITUDE_GET,
    AM_SENSOR_AS5600_BURN_SET,
} am_sensor_as5600_control_cmd_t;

/*
 * \brief 传感器AS5600提供的所有通道ID定义
 */
#define AM_AS5600_CHAN_1      0              /* AS5600 的通道1，ANGLE */

/**
 * \brief AS5600参数配置初始化结构体定义 
 */
typedef struct as5600_param_config
{
    uint8_t     watchdog;
    uint8_t     fast_filter_threshold;
    uint8_t     slow_filter;
    uint8_t     pwm_frequency;
    uint8_t     output_stage;
    uint8_t     hysteresis;
    uint8_t     power_mode;
    uint8_t     direction_polarity;
    uint8_t     option_enable;
    uint8_t     option;
    uint16_t    maximum_angle;
} as5600_param_config_t;

/**
 * \brief 传感器 AS5600 设备信息结构体
 */
typedef struct am_sensor_as5600_devinfo {

    const   as5600_param_config_t      *p_param_default;    /**< \brief 参数初始化 */

    int                                 trigger_pin;        /**< \brief 报警输出引脚 */

    int                                 out_pin;            /**< \brief OUT引脚 */

    int                                 pgo_pin;            /**< \brief PGO引脚 */

    int                                 dir_pin;            /**< \brief DIR引脚 */

    uint8_t                             i2c_addr;           /**< \brief I2C 7位 设备地址 */

} am_sensor_as5600_devinfo_t;

/**
 * \brief AS5600 校准值保存 (此处无作用)
 */
typedef struct am_as5600_calibration_data {
    int16_t offset_chx_data;
} am_as5600_calibration_data_t;

/**
 * \brief AS5600 结果结构体定义
 */
typedef struct as5600_result_info
{
    uint16_t raw_angle;                 /**< \brief RAW_ANGLE */
    uint16_t angle;                     /**< \brief ANGLE */
} as5600_result_info_t;

/**
 * \breif 传感器 AS5600 设备结构体定义
 */
typedef struct am_sensor_as5600_dev {
    am_sensor_serv_t                    as5600_dev;     /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_as5600_calibration_data_t        cal_val[1];     /*< \brief 保存校准值 */
    void                                *p_arg[1];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[1];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[1];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[1];

    /** \brief 设备信息 */
    const am_sensor_as5600_devinfo_t *dev_info;

} am_sensor_as5600_dev_t;

/**
 * \brief 传感器 AS5600 初始化
 *
 * \param[in] p_dev     : 指向传感器AS5600设备的指针
 * \param[in] p_devinfo : 指向传感器AS5600设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as5600_init (
        am_sensor_as5600_dev_t             *p_dev,
        const am_sensor_as5600_devinfo_t   *p_devinfo,
        am_i2c_handle_t                     handle);

/**
 * \brief 传感器 AS5600 去初始化
 *
 * \param[in] handle : am_sensor_as5600_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as5600_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 AS5600 控制函数
 *
 * \param[in] handle    : am_sensor_as5600_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_as5600_ioctl (am_sensor_handle_t     handle,
                                 int                    cmd,
                                 void                  *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS5600_H */

/* end of file */


