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
 * \brief 环境光传感器 TSL2540 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TSL2540_H
#define __AM_SENSOR_TSL2540_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 TSL2540 使能设置
 */
#define AM_TSL2540_WEN                      (1u << 3)
#define AM_TSL2540_AEN                      (1u << 1)
#define AM_TSL2540_PON                      (1u << 0)

/**
 * \brief 传感器 TSL2540 ALS ADC的积分时间
 */
#define AM_TSL2540_ATIME_2_8MS              (0x00)
#define AM_TSL2540_ATIME_5_6MS              (0x01)
#define AM_TSL2540_ATIME_180MS              (0x3F)
#define AM_TSL2540_ATIME_721MS              (0xFF)

/**
 * \brief 传感器 TSL2540 ALS等待时间
 */
#define AM_TSL2540_WTIME_2_8MS_OR_33_8MS    (0x00)
#define AM_TSL2540_WTIME_5_6MS_OR_67_6MS    (0x01)
#define AM_TSL2540_WTIME_180MS_OR_2_16S     (0x3F)
#define AM_TSL2540_WTIME_721MS_OR_8_65S     (0xFF)

/**
 * \brief 传感器 TSL2540 ALS传感器增益
 */
#define AM_TSL2540_AGAIN_1_2                (0x01)  /**< \brief 1/2 Gain */
#define AM_TSL2540_AGAIN_1                  (0x02)  /**< \brief 1 Gain */
#define AM_TSL2540_AGAIN_4                  (0x03)  /**< \brief 4 Gain */
#define AM_TSL2540_AGAIN_16                 (0x04)  /**< \brief 16 Gain */
#define AM_TSL2540_AGAIN_64                 (0x05)  /**< \brief 64 Gain */
#define AM_TSL2540_AGAIN_128                (0x06)  /**< \brief 128 Gain */

/**
 * \brief 传感器 TSL2540 STATUS位
 */
#define AM_TSL2540_STATUS_ASAT              (1u << 7)
#define AM_TSL2540_STATUS_AINT              (1u << 4)
#define AM_TSL2540_STATUS_CINT              (1u << 3)

/**
 * \brief 传感器 TSL2540 中断使能
 */
#define AM_TSL2540_ASIEN                    (1u << 7)
#define AM_TSL2540_AIEN                     (1u << 4)

/**
 * \brief 传感器 TSL2540 控制指令定义
 */
#define AM_SENSOR_TSL2540_FUNC_ENABLE               (1)
#define AM_SENSOR_TSL2540_FUNC_DISABLE              (2)
#define AM_SENSOR_TSL2540_ATIME_SET                 (3)
#define AM_SENSOR_TSL2540_WTIME_SET                 (4)
#define AM_SENSOR_TSL2540_LOW_ALS_THRESHOLD_SET     (5)
#define AM_SENSOR_TSL2540_HIGH_ALS_THRESHOLD_SET    (6)
#define AM_SENSOR_TSL2540_LOW_ALS_THRESHOLD_GET     (7)
#define AM_SENSOR_TSL2540_HIGH_ALS_THRESHOLD_GET    (8)
#define AM_SENSOR_TSL2540_APERS_SET                 (9)
#define AM_SENSOR_TSL2540_WLONG_SET                 (10)
#define AM_SENSOR_TSL2540_AGAIN_SET                 (11)
#define AM_SENSOR_TSL2540_REVID_GET                 (12)
#define AM_SENSOR_TSL2540_ID_GET                    (13)
#define AM_SENSOR_TSL2540_STATUS_FLAG_CLR           (14)
#define AM_SENSOR_TSL2540_REVID2_GET                (15)
#define AM_SENSOR_TSL2540_AZ_NTH_ITERATION_SET      (16)
#define AM_SENSOR_TSL2540_INT_ENABLE                (17)
#define AM_SENSOR_TSL2540_INT_DISABLE               (18)

/*
 * \brief 传感器TSL2540提供的所有通道ID定义
 */
#define AM_TSL2540_CHAN_1      0        /* TSL2540 的通道1，visible */
#define AM_TSL2540_CHAN_2      1        /* TSL2540 的通道2，IR */

/**
 * \brief TSL2540参数配置初始化结构体定义 
 */
typedef struct tsl2540_param_config
{
    uint8_t     atime_val;              /**< \brief ALS ADC的积分时间 */
    uint8_t     wtime_val;              /**< \brief ALS等待时间 */
    uint8_t     apers_val;              /**< \brief ALS中断过滤器 */
    am_bool_t   wlong_en;               /**< \brief WLONG使能设置，使能后WTIME值将扩大12倍 */
    uint8_t     again_val;              /**< \brief ALS传感器增益 */
    am_bool_t   sleep_after_interrupt;  /**< \brief SAI */
    uint8_t     az_nth_iteration_val;   /**< \brief AZ_NTH_ITERATION */
} tsl2540_param_config_t;

/**
 * \brief 传感器 TSL2540 设备信息结构体
 */
typedef struct am_sensor_tsl2540_devinfo {

    const   tsl2540_param_config_t  *p_param_default;   /**< \brief 参数初始化 */

    int                              trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                          i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_tsl2540_devinfo_t;

/**
 * \brief TSL2540 结果结构体定义
 */
typedef struct tsl2540_result_info
{
    uint16_t visible_channel_data;      /**< \brief visible */
    uint16_t ir_channel_data;           /**< \brief IR */
} tsl2540_result_info_t;

/**
 * \breif 传感器 TSL2540 设备结构体定义
 */
typedef struct am_sensor_tsl2540_dev {
    am_sensor_serv_t                    tsl2540_dev;    /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    void                                *p_arg[2];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[2];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[2];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[2];

    /** \brief 设备信息 */
    const am_sensor_tsl2540_devinfo_t   *dev_info;

} am_sensor_tsl2540_dev_t;

/**
 * \brief 传感器 TSL2540 初始化
 *
 * \param[in] p_dev     : 指向传感器TSL2540设备的指针
 * \param[in] p_devinfo : 指向传感器TSL2540设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tsl2540_init (
        am_sensor_tsl2540_dev_t             *p_dev,
        const am_sensor_tsl2540_devinfo_t   *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 TSL2540 去初始化
 *
 * \param[in] handle : am_sensor_tsl2540_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tsl2540_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 TSL2540 控制函数
 *
 * \param[in] handle    : am_sensor_tsl2540_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *   - AM_SENSOR_TSL2540_FUNC_ENABLE            : 设置功能使能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_FUNC_DISABLE           : 设置功能禁能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_ATIME_SET              : 设置ATIME, p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_WTIME_SET              : 设置WTIME, p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_LOW_ALS_THRESHOLD_SET  : 设置ALS低阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TSL2540_HIGH_ALS_THRESHOLD_SET : 设置ALS高阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TSL2540_LOW_ALS_THRESHOLD_GET  : 获取ALS低阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TSL2540_HIGH_ALS_THRESHOLD_GET : 获取ALS高阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TSL2540_APERS_SET              : 设置APERS , p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_WLONG_SET              : 设置WLONG, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TSL2540_AGAIN_SET              : 设置ALS传感器增益 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_REVID_GET              : 读取REVID , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TSL2540_ID_GET                 : 读取ID, p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TSL2540_STATUS_FLAG_CLR        : STATUS寄存器位清除, p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_REVID2_GET             : 读取REVID2 , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TSL2540_AZ_NTH_ITERATION_SET   : 设置AZ_NTH_ITERATION, p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_INT_ENABLE             : 中断使能, p_arg为 uint8_t类型
 *   - AM_SENSOR_TSL2540_INT_DISABLE            : 中断禁能, p_arg为 uint8_t类型
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_tsl2540_ioctl (am_sensor_handle_t    handle,
                                  int                   cmd,
                                  void                 *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TSL2540_H */

/* end of file */


