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
 * \brief 环境光、接近传感器 TMD2750 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TMD2750_H
#define __AM_SENSOR_TMD2750_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 TMD2750 使能设置
 */
#define AM_TMD2750_WEN                            (1u << 3)
#define AM_TMD2750_PEN                            (1u << 2)
#define AM_TMD2750_AEN                            (1u << 1)
#define AM_TMD2750_PON                            (1u << 0)

/**
 * \brief 传感器 TMD2750 ALS ADC的积分时间
 */
#define AM_TMD2750_ATIME_2_78MS                   (0x00)
#define AM_TMD2750_ATIME_5_56MS                   (0x01)
#define AM_TMD2750_ATIME_50MS                     (0x11)
#define AM_TMD2750_ATIME_100MS                    (0x23)
#define AM_TMD2750_ATIME_178MS                    (0x3F)
#define AM_TMD2750_ATIME_712MS                    (0xFF)

/**
 * \brief 传感器 TMD2750 ALS与Prox等待时间
 */
#define AM_TMD2750_WTIME_2_78MS_OR_33_4MS         (0x00)
#define AM_TMD2750_WTIME_5_56MS_OR_66_7MS         (0x01)
#define AM_TMD2750_WTIME_50MS_OR_600MS            (0x11)
#define AM_TMD2750_WTIME_100MS_OR_1_2S            (0x23)
#define AM_TMD2750_WTIME_178MS_OR_2_14S           (0x3F)
#define AM_TMD2750_WTIME_712MS_OR_8_54S           (0xFF)

/**
 * \brief 传感器 TMD2750 Prox脉冲长度
 */
#define AM_TMD2750_PULSE_LEN_4US_OR_64US          (0x00)
#define AM_TMD2750_PULSE_LEN_8US_OR_128US         (0x01)
#define AM_TMD2750_PULSE_LEN_16US_OR_256US        (0x02)
#define AM_TMD2750_PULSE_LEN_32US_OR_512US        (0x03)

/**
 * \brief 传感器 TMD2750 Prox IR传感器增益
 */
#define AM_TMD2750_PGAIN_1                        (0x00)
#define AM_TMD2750_PGAIN_2                        (0x01)
#define AM_TMD2750_PGAIN_4                        (0x02)
#define AM_TMD2750_PGAIN_8                        (0x03)

/**
 * \brief 传感器 TMD2750 Prox IR VCSEL电流驱动能力设置
 */
#define AM_TMD2750_PLDRIVE_2MA                    (0x00)
#define AM_TMD2750_PLDRIVE_4MA                    (0x01)
#define AM_TMD2750_PLDRIVE_6MA                    (0x02)
#define AM_TMD2750_PLDRIVE_8MA                    (0x03)
#define AM_TMD2750_PLDRIVE_10MA                   (0x04)
#define AM_TMD2750_PLDRIVE_12MA                   (0x05)
#define AM_TMD2750_PLDRIVE_14MA                   (0x06)
#define AM_TMD2750_PLDRIVE_17MA                   (0x07)
#define AM_TMD2750_PLDRIVE_19MA                   (0x08)

/**
 * \brief 传感器 TMD2750 ALS传感器增益
 */
#define AM_TMD2750_AGAIN_1                        (0x01)
#define AM_TMD2750_AGAIN_4                        (0x03)
#define AM_TMD2750_AGAIN_16                       (0x05)
#define AM_TMD2750_AGAIN_64                       (0x07)
#define AM_TMD2750_AGAIN_128                      (0x08)
#define AM_TMD2750_AGAIN_256                      (0x09)
#define AM_TMD2750_AGAIN_512                      (0x0A)

/**
 * \brief 传感器 TMD2750 STATUS位
 */
#define AM_TMD2750_STATUS_ASAT                    (1u << 7)
#define AM_TMD2750_STATUS_PSAT                    (1u << 6)
#define AM_TMD2750_STATUS_PINT                    (1u << 5)
#define AM_TMD2750_STATUS_AINT                    (1u << 4)
#define AM_TMD2750_STATUS_CINT                    (1u << 3)
#define AM_TMD2750_STATUS_ZINT                    (1u << 2)
#define AM_TMD2750_STATUS_PSAT_REFLECTIVE         (1u << 1)
#define AM_TMD2750_STATUS_PSAT_AMBIENT            (1u << 0)


/**
 * \brief 传感器 TMD2750 APC设置
 */
#define AM_TMD2750_APC_ENABLE                     (0x00)
#define AM_TMD2750_APC_DISABLE                    (0x01)

/**
 * \brief 传感器 TMD2750 Prox校准类型选择
 */
#define AM_TMD2750_ELECTRICAL_OPTICAL_CROSSTALK   (0x00)
#define AM_TMD2750_ELECTRICAL_CROSSTALK_ONLY      (0x01)

/**
 * \brief 传感器 TMD2750 二进制搜索目标
 */
#define AM_TMD2750_BINSRCH_TARGET_3               (0x00)
#define AM_TMD2750_BINSRCH_TARGET_7               (0x01)
#define AM_TMD2750_BINSRCH_TARGET_15              (0x02)
#define AM_TMD2750_BINSRCH_TARGET_31              (0x03)
#define AM_TMD2750_BINSRCH_TARGET_63              (0x04)
#define AM_TMD2750_BINSRCH_TARGET_127             (0x05)
#define AM_TMD2750_BINSRCH_TARGET_255             (0x06)
#define AM_TMD2750_BINSRCH_TARGET_511             (0x07)

/**
 * \brief 传感器 TMD2750 PROX_AVG 采样数量
 */
#define AM_TMD2750_SAMPLE_SIZE_DISABLE            (0x00)
#define AM_TMD2750_SAMPLE_SIZE_2                  (0x01)
#define AM_TMD2750_SAMPLE_SIZE_4                  (0x02)
#define AM_TMD2750_SAMPLE_SIZE_8                  (0x03)
#define AM_TMD2750_SAMPLE_SIZE_16                 (0x04)
#define AM_TMD2750_SAMPLE_SIZE_32                 (0x05)
#define AM_TMD2750_SAMPLE_SIZE_64                 (0x06)
#define AM_TMD2750_SAMPLE_SIZE_128                (0x07)

/**
 * \brief 传感器 TMD2750 中断使能
 */
#define AM_TMD2750_ASIEN                          (1u << 7)
#define AM_TMD2750_PSIEN                          (1u << 6)
#define AM_TMD2750_PIEN                           (1u << 5)
#define AM_TMD2750_AIEN                           (1u << 4)
#define AM_TMD2750_CIEN                           (1u << 3)
#define AM_TMD2750_ZIEN                           (1u << 2)

/**
 * \brief 传感器 TMD2750 控制指令定义
 */
typedef enum am_sensor_tmd2750_control_cmd {
    AM_SENSOR_TMD2750_FUNC_ENABLE = 1,
    AM_SENSOR_TMD2750_FUNC_DISABLE,
    AM_SENSOR_TMD2750_ATIME_SET,
    AM_SENSOR_TMD2750_PRATE_SET,
    AM_SENSOR_TMD2750_WTIME_SET,
    AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_SET,
    AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_SET,
    AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_SET,
    AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_SET,
    AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_GET,
    AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_GET,
    AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_GET,
    AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_GET,
    AM_SENSOR_TMD2750_PPERS_SET,
    AM_SENSOR_TMD2750_APERS_SET,
    AM_SENSOR_TMD2750_WLONG_SET,
    AM_SENSOR_TMD2750_PPULSE_LEN_16X_SET,
    AM_SENSOR_TMD2750_PPULSE_LEN_SET,
    AM_SENSOR_TMD2750_PPULSE_SET,
    AM_SENSOR_TMD2750_PGAIN_SET,
    AM_SENSOR_TMD2750_PLDRIVE_SET,
    AM_SENSOR_TMD2750_AGAIN_SET,
    AM_SENSOR_TMD2750_REVID_GET,
    AM_SENSOR_TMD2750_ID_GET,
    AM_SENSOR_TMD2750_STATUS_FLAG_CLR,
    AM_SENSOR_TMD2750_REVID2_GET,
    AM_SENSOR_TMD2750_SOFTRST,
    AM_SENSOR_TMD2750_APC_SET,
    AM_SENSOR_TMD2750_PROX_CAL_TYPE_SEL,
    AM_SENSOR_TMD2750_START_OFFSET_CAL_SET,
    AM_SENSOR_TMD2750_BINSRCH_TARGET_SET,
    AM_SENSOR_TMD2750_AUTO_OFFSET_ADJ_SET,
    AM_SENSOR_TMD2750_PROX_AVG_SET,
    AM_SENSOR_TMD2750_INT_ENABLE,
    AM_SENSOR_TMD2750_INT_DISABLE,
} am_sensor_tmd2750_control_cmd_t;

/*
 * \brief 传感器TMD2750提供的所有通道ID定义
 */
#define AM_TMD2750_CHAN_1      0              /* TMD2750 的通道1，Prox */
#define AM_TMD2750_CHAN_2      1              /* TMD2750 的通道2，ALS */

/**
 * \brief TMD2750参数配置初始化结构体定义 
 */
typedef struct tmd2750_param_config
{
    uint8_t     atime_val;              /**< \brief ALS ADC的积分时间 */
    uint8_t     wtime_val;              /**< \brief ALS与Prox之间的等待时间 */
    uint8_t     ppers_val;              /**< \brief Prox中断过滤器 */
    uint8_t     apers_val;              /**< \brief ALS中断过滤器 */
    am_bool_t   wlong_en;               /**< \brief WLONG使能设置，使能后WTIME值将扩大12倍 */
    am_bool_t   ppulse_len_16x_en;      /**< \brief Prox脉冲长度扩大16倍设置 */
    uint8_t     pulse_len;              /**< \brief Prox脉冲长度 */
    uint8_t     pulse;                  /**< \brief 一个Prox周期的最大脉冲数 */
    uint8_t     pgain_val;              /**< \brief Prox IR传感器增益 */
    uint8_t     pldrive_val;            /**< \brief Prox IR VCSEL电流 */
    uint8_t     again_val;              /**< \brief ALS传感器增益 */
    am_bool_t   sleep_after_interrupt;  /**< \brief SAI */
    uint8_t     apc_disable;            /**< \brief APC禁能设置 */
    uint8_t     prox_cal_type;          /**< \brief Prox校准类型 */
    uint8_t     binsrch_target;         /**< \brief BINSRCH_TARGET */
    am_bool_t   auto_offset_adj;        /**< \brief AUTO_OFFSET_ADJ */
    uint8_t     prox_avg;               /**< \brief PROX_AVG */
    uint32_t    prox_sample_time_us;    /**< \brief Prox采样持续时间:(PRATE + 1) * 88us */
} tmd2750_param_config_t;

/**
 * \brief 传感器 TMD2750 设备信息结构体
 */
typedef struct am_sensor_tmd2750_devinfo {

    const   tmd2750_param_config_t      *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_tmd2750_devinfo_t;

/**
 * \brief TMD2750 Prox校准值保存
 */
typedef struct am_tmd2750_calibration_data {
    int16_t   prox_offset_adj;
} am_tmd2750_calibration_data_t;

/**
 * \brief TMD2750 结果结构体定义
 */
typedef struct tmd2750_result_info
{
    uint16_t als_channel_data;          /**< \brief ALS (CH0) */
    uint16_t ir_channel_data;           /**< \brief IR  (CH1) */
    uint16_t raw_prox_value;            /**< \brief Prox AD值 */
    uint16_t prox_value;                /**< \brief Prox值 */
    int16_t  prox_offset_adjust_value;  /**< \brief Prox校准值 */
} tmd2750_result_info_t;

/**
 * \breif 传感器 TMD2750 设备结构体定义
 */
typedef struct am_sensor_tmd2750_dev {
    am_sensor_serv_t                    tmd2750_dev;    /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_tmd2750_calibration_data_t       cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[2];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[2];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[3];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[2];

    /** \brief 设备信息 */
    const am_sensor_tmd2750_devinfo_t *dev_info;

} am_sensor_tmd2750_dev_t;

/**
 * \brief 传感器 TMD2750 初始化
 *
 * \param[in] p_dev     : 指向传感器TMD2750设备的指针
 * \param[in] p_devinfo : 指向传感器TMD2750设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tmd2750_init (
        am_sensor_tmd2750_dev_t             *p_dev,
        const am_sensor_tmd2750_devinfo_t   *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 TMD2750 去初始化
 *
 * \param[in] handle : am_sensor_tmd2750_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tmd2750_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 TMD2750 控制函数
 *
 * \param[in] handle    : am_sensor_tmd2750_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *   - AM_SENSOR_TMD2750_FUNC_ENABLE              : 设置功能使能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_FUNC_DISABLE             : 设置功能禁能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_ATIME_SET                : 设置ATIME, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_PRATE_SET                : 设置PRATE, p_arg为 uint32_t类型
 *   - AM_SENSOR_TMD2750_WTIME_SET                : 设置WTIME, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_SET    : 设置ALS低阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_SET   : 设置ALS高阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_SET   : 设置Prox低阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_SET  : 设置Prox高阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2750_LOW_ALS_THRESHOLD_GET    : 获取ALS低阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2750_HIGH_ALS_THRESHOLD_GET   : 获取ALS高阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2750_LOW_PROX_THRESHOLD_GET   : 获取Prox低阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2750_HIGH_PROX_THRESHOLD_GET  : 获取Prox高阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2750_PPERS_SET                : 设置PPERS , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_APERS_SET                : 设置APERS , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_WLONG_SET                : 设置WLONG, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2750_PPULSE_LEN_16X_SET       : 设置PPULSE_LEN_16X, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2750_PPULSE_LEN_SET           : 设置PPULSE_LEN , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_PPULSE_SET               : 设置Prox周期脉冲的最大数量 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_PGAIN_SET                : 设置Prox IR传感器增益, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_PLDRIVE_SET              : 设置Prox IR VCSEL电流驱动能力, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_AGAIN_SET                : 设置ALS传感器增益 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_REVID_GET                : 读取REVID , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2750_ID_GET                   : 读取ID, p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2750_STATUS_FLAG_CLR          : STATUS寄存器位清除, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_REVID2_GET               : 读取REVID2 , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2750_SOFTRST                  : 软件复位 , p_arg为 NULL
 *   - AM_SENSOR_TMD2750_APC_SET                  : 设置APC, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_PROX_CAL_TYPE_SEL        : 选择Prox校准类型, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_START_OFFSET_CAL_SET     : 设置偏移校准, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2750_BINSRCH_TARGET_SET       : 设置BINSRCH_TARGET , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_AUTO_OFFSET_ADJ_SET      : 设置AUTO_OFFSET_ADJ , p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2750_PROX_AVG_SET             : 设置PROX_AVG, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_INT_ENABLE               : 中断使能, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2750_INT_DISABLE              : 中断禁能, p_arg为 uint8_t类型
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_tmd2750_ioctl (am_sensor_handle_t  handle,
                                  int                 cmd,
                                  void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TMD2750_H */

/* end of file */


