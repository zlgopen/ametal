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
 * \brief 温度传感器 AS621X 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-21  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS621X_H
#define __AM_SENSOR_AS621X_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 AS621X 转换速率选择
 */
#define AM_AS621X_CONVERSION_RATE_0_25HZ        (0x00)  /* 0.25Hz */
#define AM_AS621X_CONVERSION_RATE_1HZ           (0x01)  /* 1Hz */
#define AM_AS621X_CONVERSION_RATE_4HZ           (0x02)  /* 4Hz */
#define AM_AS621X_CONVERSION_RATE_8HZ           (0x03)  /* 8Hz */
#define AM_AS621X_CONVERSION_RATE_4S            (AM_AS621X_CONVERSION_RATE_0_25HZ)
#define AM_AS621X_CONVERSION_RATE_1S            (AM_AS621X_CONVERSION_RATE_1HZ)
#define AM_AS621X_CONVERSION_RATE_250MS         (AM_AS621X_CONVERSION_RATE_4HZ)
#define AM_AS621X_CONVERSION_RATE_125MS         (AM_AS621X_CONVERSION_RATE_8HZ)

/**
 * \brief 传感器 AS621X SM位选择
 */
#define AM_AS621X_CONTINUOUS_CONVERSION_MODE    (0x00)  
#define AM_AS621X_SLEEP_MODE                    (0x01)

/**
 * \brief 传感器 AS621X IM位选择
 */
#define AM_AS621X_COMPARATOR_MODE               (0x00)  /* 比较模式 */
#define AM_AS621X_INTERRUPT_MODE                (0x01)  /* 中断模式 */

/**
 * \brief 传感器 AS621X 连续故障次数选择
 */
#define AM_AS621X_CONSECUTIVE_FAULTS_1          (0x00)
#define AM_AS621X_CONSECUTIVE_FAULTS_2          (0x01)
#define AM_AS621X_CONSECUTIVE_FAULTS_3          (0x02)
#define AM_AS621X_CONSECUTIVE_FAULTS_4          (0x03)

/**
 * \brief 传感器 AS621X 控制指令定义
 */
#define AM_SENSOR_AS621X_ALERT_BIT_GET          (1)
#define AM_SENSOR_AS621X_CR_SET                 (2)
#define AM_SENSOR_AS621X_SM_SET                 (3)
#define AM_SENSOR_AS621X_SM_GET                 (4)
#define AM_SENSOR_AS621X_IM_SET                 (5)
#define AM_SENSOR_AS621X_POLARITY_SET           (6)
#define AM_SENSOR_AS621X_CF_SET                 (7)
#define AM_SENSOR_AS621X_SS_SET                 (8)
#define AM_SENSOR_AS621X_TLOW_THRESHOLD_SET     (9)
#define AM_SENSOR_AS621X_THIGH_THRESHOLD_SET    (10)
#define AM_SENSOR_AS621X_TLOW_THRESHOLD_GET     (11)
#define AM_SENSOR_AS621X_THIGH_THRESHOLD_GET    (12)

/**
 * \brief 传感器AS621X提供的所有通道ID定义
 */
#define AM_AS621X_CHAN_1      0             /* AS621X 的通道1，温度传感器通道 */

/**
 * \brief AS621X参数配置初始化结构体定义 
 */
typedef struct as621x_param_config
{
    uint8_t     conversion_rate;    /* 转换速率 */
    uint8_t     sleep_mode;         /* SM位 */
    am_bool_t   polarity_high;      /* 极性设置:是否高电平激活 */
    uint8_t     consecutive_faults; /* 连续故障数 */
    float       tlow_temp;          /* 低温度阈值 */
    float       thigh_temp;         /* 高温度阈值 */
} as621x_param_config_t;

/**
 * \brief 传感器 AS621X 设备信息结构体
 */
typedef struct am_sensor_as621x_devinfo {

    const   as621x_param_config_t   *p_param_default;   /**< \brief 参数初始化 */

    int                              trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                          i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_as621x_devinfo_t;

/**
 * \brief AS621X 结果结构体定义
 */
typedef struct as621x_result_info
{
    int  as621x_temperature;           /**< \brief 实际温度值扩大10^6倍 */
} as621x_result_info_t;

/**
 * \breif 传感器 AS621X 设备结构体定义
 */
typedef struct am_sensor_as621x_dev {
    am_sensor_serv_t                  as621x_dev; /*< \breif 传感器标准服务 */
    am_i2c_device_t                   i2c_dev;    /*< \brief i2c设备实例 */
    void                             *p_arg[1];   /*< \brief 触发回调函数参数 */
    uint8_t                           flags[1];   /*< \brief 触发标志位 */
    am_isr_defer_job_t                g_myjob;    /*< \brief 中断延时处理任务 */
    uint8_t                           trigger;    /*< \brief 记录标志位 */
    am_sensor_val_t                   data[1];    /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t            pfn_trigger_cb[1];

    /** \brief 设备信息 */
    const am_sensor_as621x_devinfo_t *dev_info;

} am_sensor_as621x_dev_t;

/**
 * \brief 传感器 AS621X 初始化
 *
 * \param[in] p_dev     : 指向传感器AS621X设备的指针
 * \param[in] p_devinfo : 指向传感器AS621X设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as621x_init (
        am_sensor_as621x_dev_t           *p_dev,
        const am_sensor_as621x_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 AS621X 去初始化
 *
 * \param[in] handle : am_sensor_as621x_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as621x_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 AS621X 控制函数
 *
 * \param[in] handle    : am_sensor_as621x_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *   - AM_SENSOR_AS621X_ALERT_BIT_GET       : 获取报警位, p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_AS621X_CR_SET              : 设置转换速率, p_arg为 uint8_t类型
 *   - AM_SENSOR_AS621X_SM_SET              : 设置SM位(Sleep Mode), p_arg为 uint8_t类型
 *   - AM_SENSOR_AS621X_SM_GET              : 获取SM位, p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_AS621X_IM_SET              : 设置IM位(Interrupt Mode), p_arg为 uint8_t类型
 *   - AM_SENSOR_AS621X_POLARITY_SET        : 设置POL位(Polarity), p_arg为 am_bool_t类型
 *   - AM_SENSOR_AS621X_CF_SET              : 设置连续故障数(Consecutive Faults), p_arg为 uint8_t类型
 *   - AM_SENSOR_AS621X_SS_SET              : 设置SS位, p_arg为 am_bool_t类型
 *   - AM_SENSOR_AS621X_TLOW_THRESHOLD_SET  : 设置低温度阈值, p_arg为 float类型
 *   - AM_SENSOR_AS621X_THIGH_THRESHOLD_SET : 设置高温度阈值, p_arg为 float类型
 *   - AM_SENSOR_AS621X_TLOW_THRESHOLD_GET  : 获取低温度阈值, p_arg为 (float *)类型
 *   - AM_SENSOR_AS621X_THIGH_THRESHOLD_GET : 获取高温度阈值, p_arg为 (float *)类型
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_as621x_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS621X_H */

/* end of file */
