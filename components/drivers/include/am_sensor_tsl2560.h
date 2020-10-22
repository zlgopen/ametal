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
 * \brief 环境光传感器 TSL2560 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TSL2560_H
#define __AM_SENSOR_TSL2560_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 TSL2560 使能设置
 */
#define AM_TSL2560_NPIEN                        (1u << 7)
#define AM_TSL2560_SAI                          (1u << 6)
#define AM_TSL2560_AIEN                         (1u << 4)
#define AM_TSL2560_AEN                          (1u << 1)
#define AM_TSL2560_PON                          (1u << 0)

/**
 * \brief 传感器 TSL2560 ALS增益
 */
#define AM_TSL2560_AGAIN_LOW_GAIN_MODE          (0x00)
#define AM_TSL2560_AGAIN_MEDIUM_GAIN_MODE       (0x01)
#define AM_TSL2560_AGAIN_HIGH_GAIN_MODE         (0x02)
#define AM_TSL2560_AGAIN_MAXIMUM_GAIN_MODE      (0x03)

/**
 * \brief 传感器 TSL2560 ALS ADC的积分时间
 */
#define AM_TSL2560_ATIME_100MS                  (0x00)
#define AM_TSL2560_ATIME_200MS                  (0x01)
#define AM_TSL2560_ATIME_300MS                  (0x02)
#define AM_TSL2560_ATIME_400MS                  (0x03)
#define AM_TSL2560_ATIME_500MS                  (0x04)
#define AM_TSL2560_ATIME_600MS                  (0x05)

/**
 * \brief 传感器 TSL2560 STATUS位
 */
#define AM_TSL2560_STATUS_NPINTR                (1u << 5)
#define AM_TSL2560_STATUS_AINT                  (1u << 4)
#define AM_TSL2560_STATUS_AVALID                (1u << 0)

/**
 * \brief 传感器 TSL2560 控制指令定义
 */
typedef enum am_sensor_tsl2560_control_cmd {
    AM_SENSOR_TSL2560_FUNC_ENABLE = 1,
    AM_SENSOR_TSL2560_FUNC_DISABLE,
    AM_SENSOR_TSL2560_SOFTRST,
    AM_SENSOR_TSL2560_AGAIN_SET,
    AM_SENSOR_TSL2560_ATIME_SET,
    AM_SENSOR_TSL2560_LOW_ALS_THRESHOLD_SET,
    AM_SENSOR_TSL2560_HIGH_ALS_THRESHOLD_SET,
    AM_SENSOR_TSL2560_LOW_ALS_THRESHOLD_GET,
    AM_SENSOR_TSL2560_HIGH_ALS_THRESHOLD_GET,
    AM_SENSOR_TSL2560_NO_PERSIST_LOW_ALS_THRESHOLD_SET,
    AM_SENSOR_TSL2560_NO_PERSIST_HIGH_ALS_THRESHOLD_SET,
    AM_SENSOR_TSL2560_NO_PERSIST_LOW_ALS_THRESHOLD_GET,
    AM_SENSOR_TSL2560_NO_PERSIST_HIGH_ALS_THRESHOLD_GET,
    AM_SENSOR_TSL2560_APERS_SET,
    AM_SENSOR_TSL2560_PID_GET,
    AM_SENSOR_TSL2560_ID_GET,
    AM_SENSOR_TSL2560_NPINTR_GET,
    AM_SENSOR_TSL2560_AINT_GET,
    AM_SENSOR_TSL2560_AVALID_GET,
    AM_SENSOR_TSL2560_INTERRUPT_SET,
    AM_SENSOR_TSL2560_ALS_INTERRUPT_CLEAR,
    AM_SENSOR_TSL2560_ALS_AND_NO_PERSIST_ALS_INTERRUPT_CLEAR,
    AM_SENSOR_TSL2560_NO_PERSIST_ALS_INTERRUPT_CLEAR,
} am_sensor_tsl2560_control_cmd_t;

/*
 * \brief 传感器TSL2560提供的所有通道ID定义
 */
#define AM_TSL2560_CHAN_1       0               /* TSL2560 的通道1，ALS_CH0 */
#define AM_TSL2560_CHAN_2       1               /* TSL2560 的通道2，ALS_CH1 */

/**
 * \brief TSL2560参数配置初始化结构体定义 
 */
typedef struct tsl2560_param_config
{
    uint8_t     again_val;
    uint8_t     atime_val;
    uint8_t     apers_val;
    uint8_t     interrupt_type;
    uint16_t    als_low_threshold;
    uint16_t    als_high_threshold;
    uint16_t    no_persist_als_low_threshold;
    uint16_t    no_persist_als_high_threshold;
} tsl2560_param_config_t;

/**
 * \brief 传感器 TSL2560 设备信息结构体
 */
typedef struct am_sensor_tsl2560_devinfo {

    const   tsl2560_param_config_t      *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_tsl2560_devinfo_t;

/**
 * \brief TSL2560 校准值保存 (此处无作用)
 */
typedef struct am_tsl2560_calibration_data {
    int16_t offset_chx_data;
} am_tsl2560_calibration_data_t;

/**
 * \brief TSL2560 结果结构体定义
 */
typedef struct tsl2560_result_info
{
    uint16_t als_ch0_data;          /**< \brief ALS (CH0) */
    uint16_t als_ch1_data;          /**< \brief ALS (CH1) */
} tsl2560_result_info_t;

/**
 * \breif 传感器 TSL2560 设备结构体定义
 */
typedef struct am_sensor_tsl2560_dev {
    am_sensor_serv_t                    tsl2560_dev;    /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_tsl2560_calibration_data_t       cal_val[1];     /*< \brief 保存校准值 */
    void                                *p_arg[2];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[2];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[2];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[2];

    /** \brief 设备信息 */
    const am_sensor_tsl2560_devinfo_t *dev_info;

} am_sensor_tsl2560_dev_t;

/**
 * \brief 传感器 TSL2560 初始化
 *
 * \param[in] p_dev     : 指向传感器TSL2560设备的指针
 * \param[in] p_devinfo : 指向传感器TSL2560设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tsl2560_init (
        am_sensor_tsl2560_dev_t             *p_dev,
        const am_sensor_tsl2560_devinfo_t   *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 TSL2560 去初始化
 *
 * \param[in] handle : am_sensor_tsl2560_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tsl2560_deinit (am_sensor_handle_t   handle);

/**
 * \brief 传感器 TSL2560 控制函数
 *
 * \param[in] handle    : am_sensor_tsl2560_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_tsl2560_ioctl (am_sensor_handle_t    handle,
                                  int                   cmd,
                                  void                 *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TSL2560_H */

/* end of file */


