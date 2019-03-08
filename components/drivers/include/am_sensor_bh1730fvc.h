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
 * \brief 光照及距离传感器 BH1730FVC 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 19-02-20  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_BH1730FVC_H
#define __AM_SENSOR_BH1730FVC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器BH1730FVC提供的所有通道ID定义
 */
#define AM_BH1730FVC_CHAN_1      0         /* BH1730FVC 的通道1，光照强度通道 */

/**
 * \brief 传感器 BH1730FVC 设备信息结构体
 */
typedef struct am_sensor_bh1730fvc_devinfo {
    
    /**
     * \brief 数据准备就绪触发引脚
     */
    int     trigger_pin;
    
    /**
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址为 0x29
     */
    uint8_t i2c_addr;

} am_sensor_bh1730fvc_devinfo_t;

/**
 * \breif 传感器 BH1730FVC 设备结构体定义
 */
typedef struct am_sensor_bh1730fvc_dev {
    am_sensor_serv_t               bh1730fvc_dev;/**< \breif 传感器标准服务   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c设备实例      */
    void                          *p_arg[1];     /**< \brief 触发回调函数参数 */
    uint8_t                        flags[1];     /**< \brief 触发标志位       */
    am_isr_defer_job_t             g_myjob;      /**< \brief 中断延时处理任务 */  
    uint8_t                        trigger;      /**< \brief 记录标志位       */
    am_sensor_val_t                data[1];      /**< \brief 内部数据缓存     */
    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t         pfn_trigger_cb[1];
    
    /** \brief 设备信息 */
    const am_sensor_bh1730fvc_devinfo_t *dev_info;

} am_sensor_bh1730fvc_dev_t;

/**
 * \brief 传感器 BH1730FVC 初始化
 *
 * \param[in] p_dev     : 指向传感器BH1730FVC设备的指针
 * \param[in] p_devinfo : 指向传感器BH1730FVC设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_bh1730fvc_init (
        am_sensor_bh1730fvc_dev_t           *p_dev,
        const am_sensor_bh1730fvc_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 BH1730FVC 去初始化
 *
 * \param[in] handle : am_sensor_bh1730fvc_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_bh1730fvc_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_BH1730FVC_H */

/* end of file */
