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
 * \brief 飞行时间传感器 TMF8801 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-02-28  YRZ, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TMF8801_H
#define __AM_SENSOR_TMF8801_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/*
 * \brief 传感器TMF8801提供的所有通道ID定义
 */
#define AM_TMF8801_CHAN_1      0             /* TMF8801 的通道1，距离 */
#define AM_TMF8801_CHAN_2      1             /* TMF8801 的通道2，温度 */

/**
 * \brief APP0 配置初始化结构体定义 
 */
typedef struct app0_config
{    
    uint8_t calibration_state_mask;    
    uint8_t gpio_control;           /**< \brief GPIOx 控制 */
    uint8_t gpio_output_control;    /**< \brief GPIOx 输出控制 */
    uint8_t repetition_period_ms;   /**< \brief 连续测量周期设置 */
    uint16_t iterations;            /**< \brief 迭代次数 */
} app0_config_t;

/**
 * \brief TMF8x01 测量结果结构体定义
 */
typedef struct result_info
{
    int8_t   temperature;           /**< \brief 温度 */
    uint8_t  result_num;            /**< \brief 结果 ID */
    uint8_t  reliability;           /**< \brief 置信度 */
    uint16_t distance_peak;         /**< \brief 距离(mm) */
    uint32_t time_stamp;            /**< \brief 时间戳(100us) */
    uint32_t reference_hits;        /**< \brief 距离模式下，参考 SPADs 被光子击中总数量 */
    uint32_t object_hits;           /**< \brief 距离模式下，目标 SPADs 被光子击中总数量 */
} result_info_t;

/**
 * \brief 传感器 TMF8801 设备信息结构体
 */
typedef struct am_sensor_tmf8801_devinfo {

    /**
     * \brief 使能引脚
     */
    int     enable_pin;

    /**
     * \brief 中断触发引脚
     */
    int     trigger_pin;

    /*
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址默认为 0x41
     */
    uint8_t i2c_addr;

} am_sensor_tmf8801_devinfo_t;

/**
 * \breif 传感器 TMF8801 设备结构体定义
 */
typedef struct am_sensor_tmf8801_dev {
    am_sensor_serv_t                  tmf8801_dev; /**< \breif 传感器标准服务 */
    am_i2c_device_t                   i2c_dev;     /**< \brief i2c设备实例 */
    void                             *p_arg[3];    /**< \brief 触发回调函数参数 */
    uint8_t                           flags[3];    /**< \brief 触发标志位 */
    am_isr_defer_job_t                g_myjob;     /**< \brief 中断延时处理任务 */
    uint8_t                           trigger;     /**< \brief 记录标志位 */
    am_sensor_val_t                   data[3];     /**< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t            pfn_trigger_cb[3];

    /** \brief 设备信息 */
    const am_sensor_tmf8801_devinfo_t *dev_info;

} am_sensor_tmf8801_dev_t;

/**
 * \brief 传感器 TMF8801 初始化
 *
 * \param[in] p_dev     : 指向传感器TMF8801设备的指针
 * \param[in] p_devinfo : 指向传感器TMF8801设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tmf8801_init (
        am_sensor_tmf8801_dev_t           *p_dev,
        const am_sensor_tmf8801_devinfo_t *p_devinfo,
        am_i2c_handle_t                    handle);

/**
 * \brief 传感器 TMF8801 去初始化
 *
 * \param[in] handle : am_sensor_tmf8801_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tmf8801_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TMF8801_H */

/* end of file */
