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
 * \brief 三轴磁力传感器 MAG3110 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 19-07-03  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_MAG3110_H
#define __AM_SENSOR_MAG3110_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器MAG3110提供的所有通道ID定义
 */
#define AM_MAG3110_CHAN_1      0           /* MAG3110 的通道1，X轴磁力        */
#define AM_MAG3110_CHAN_2      1           /* MAG3110 的通道2，Y轴磁力        */
#define AM_MAG3110_CHAN_3      2           /* MAG3110 的通道3，Z轴磁力        */
#define AM_MAG3110_CHAN_4      3           /* MAG3110 的通道4，温度           */
    
/**
 * \brief 传感器 MAG3110 设备信息结构体
 */
typedef struct am_sensor_mag3110_devinfo {
    
    /**
     * \brief 数据准备就绪触发引脚
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址为 0x1C
     */
    uint8_t i2c_addr;

} am_sensor_mag3110_devinfo_t;

/**
 * \breif 传感器 MAG3110 设备结构体定义
 */
typedef struct am_sensor_mag3110_dev {
    am_sensor_serv_t               mag3110_dev;  /**< \breif 传感器标准服务   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c设备实例      */
    void                          *p_arg[4];     /**< \brief 触发回调函数参数 */
    uint8_t                        flags[4];     /**< \brief 触发标志位       */
    am_isr_defer_job_t             g_myjob;      /**< \brief 中断延时处理任务 */ 
    uint16_t                       trigger;      /**< \brief 记录标志位       */
    am_sensor_val_t                data[4];      /**< \brief 内部数据缓存     */
    am_sensor_val_t                sam_range;    /**< \brief 测量量程         */
    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t         pfn_trigger_cb[4];
    
    /** \brief 设备信息 */
    const am_sensor_mag3110_devinfo_t *dev_info;

} am_sensor_mag3110_dev_t;

/**
 * \brief 传感器 MAG3110 初始化
 *
 * \param[in] p_dev     : 指向传感器MAG3110设备的指针
 * \param[in] p_devinfo : 指向传感器MAG3110设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_mag3110_init (
        am_sensor_mag3110_dev_t           *p_dev,
        const am_sensor_mag3110_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 MAG3110 去初始化
 *
 * \param[in] handle : am_sensor_mag3110_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_mag3110_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_MAG3110_H */

/* end of file */
