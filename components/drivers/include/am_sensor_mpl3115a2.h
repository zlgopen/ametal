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
 * \brief 气压传感器 MPL3115A2 头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-11  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_MPL3115A2_H
#define __AM_SENSOR_MPL3115A2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器MPL3115A2提供的所有通道ID定义
 */
#define AM_MPL3115A2_CHAN_1      0           /* MPL3115A2 的通道1，气压/海拔传感器通道 */
#define AM_MPL3115A2_CHAN_2      1           /* MPL3115A2 的通道2，温度传感器通道      */

/**
 * \brief 传感器MPL3115A2模式
 */
#define __MPL3115A2_PRE_MODE    0x00    /**< \brief 气压计模式            */
#define __MPL3115A2_ALT_MODE    0x01    /**< \brief 海拔计模式            */

/*******************************************************************************
  传感器MPL3115A2可修改宏定义
*******************************************************************************/
/**< \brief 过采样率为2^(__MPL3115A2_OS_VAL)，7:2^7 = 128 */
#define __MPL3115A2_OS_VAL      0x07

/**
 * \brief 传感器MPL3115A2模式设置
 * \param:__MPL3115A2_PRE_MODE：气压计模式
          __MPL3115A2_ALT_MODE：海拔计模式
 */
#define __MPL3115A2_MODE        __MPL3115A2_PRE_MODE

/**
 * \brief 传感器 MPL3115A2 设备信息结构体
 */
typedef struct am_sensor_mpl3115a2_devinfo {
    
    /**
     * \brief 数据准备就绪触发引脚
     */
    int     trigger_pin;

    /**
     * \brief I2C 7位 设备地址
     *
     * 该设备7位地址为 0xC0 或 0xC1
     */
    uint8_t i2c_addr;

    /** \brief I2C重新实例初始化函数 */
    am_i2c_handle_t    (*pfn_i2c_inst_init)(void);

    /** \brief I2C解初始化函数 */
    void    (*pfn_i2c_inst_deinit)(am_i2c_handle_t);
} am_sensor_mpl3115a2_devinfo_t;

/**
 * \breif 传感器 MPL3115A2 设备结构体定义
 */
typedef struct am_sensor_mpl3115a2_dev {
    am_sensor_serv_t               mpl3115a2_dev;/**< \breif 传感器标准服务   */
    am_i2c_device_t                i2c_dev;      /**< \brief i2c设备实例      */
    void                          *p_arg[2];     /**< \brief 触发回调函数参数 */
    uint8_t                        flags[2];     /**< \brief 触发标志位       */
    am_isr_defer_job_t             g_myjob;      /**< \brief 中断延时处理任务 */
    am_sensor_val_t                sam_rate;     /**< \brief 采样频率         */
    uint8_t                        trigger;      /**< \brief 记录标志位       */
    am_sensor_val_t                data[2];      /**< \brief 内部数据缓存     */
    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t         pfn_trigger_cb[2];
    
    /** \brief 设备信息 */
    const am_sensor_mpl3115a2_devinfo_t *dev_info;

} am_sensor_mpl3115a2_dev_t;

/**
 * \brief 传感器 MPL3115A2 初始化
 *
 * \param[in] p_dev     : 指向传感器MPL3115A2设备的指针
 * \param[in] p_devinfo : 指向传感器MPL3115A2设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_mpl3115a2_init (
        am_sensor_mpl3115a2_dev_t           *p_dev,
        const am_sensor_mpl3115a2_devinfo_t *p_devinfo,
        am_i2c_handle_t                   handle);

/**
 * \brief 传感器 MPL3115A2 去初始化
 *
 * \param[in] handle : am_sensor_mpl3115a2_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_mpl3115a2_deinit (am_sensor_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_MPL3115A2_H */

/* end of file */
