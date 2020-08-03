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
 * \brief 接近传感器 TMD2635 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_TMD2635_H
#define __AM_SENSOR_TMD2635_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 TMD2635 使能设置
 */
#define AM_TMD2635_PWEN                         (1u << 4)
#define AM_TMD2635_PEN                          (1u << 2)
#define AM_TMD2635_PON                          (1u << 0)

/**
 * \brief 传感器 TMD2635 Prox IR传感器增益
 */
#define AM_TMD2635_PGAIN_1                      (0x00)
#define AM_TMD2635_PGAIN_2                      (0x01)
#define AM_TMD2635_PGAIN_4                      (0x02)
#define AM_TMD2635_PGAIN_8                      (0x03)

/**
 * \brief 传感器 TMD2635 Prox脉冲长度
 */
#define AM_TMD2635_PULSE_LEN_1US                (0x00)
#define AM_TMD2635_PULSE_LEN_2US                (0x01)
#define AM_TMD2635_PULSE_LEN_4US                (0x02)
#define AM_TMD2635_PULSE_LEN_8US                (0x03)
#define AM_TMD2635_PULSE_LEN_12US               (0x04)
#define AM_TMD2635_PULSE_LEN_16US               (0x05)
#define AM_TMD2635_PULSE_LEN_24US               (0x06)
#define AM_TMD2635_PULSE_LEN_32US               (0x07)

/**
 * \brief 传感器 TMD2635 Prox IR VCSEL电流驱动能力设置
 */
#define AM_TMD2635_PLDRIVE_2MA                  (0x00)
#define AM_TMD2635_PLDRIVE_3MA                  (0x01)
#define AM_TMD2635_PLDRIVE_4MA                  (0x02)
#define AM_TMD2635_PLDRIVE_5MA                  (0x03)
#define AM_TMD2635_PLDRIVE_6MA                  (0x04)
#define AM_TMD2635_PLDRIVE_7MA                  (0x05)
#define AM_TMD2635_PLDRIVE_8MA                  (0x06)
#define AM_TMD2635_PLDRIVE_9MA                  (0x07)
#define AM_TMD2635_PLDRIVE_10MA                 (0x08)

/**
 * \brief 传感器 TMD2635 STATUS位
 */
#define AM_TMD2635_STATUS_PHIGH                 (1u << 7)
#define AM_TMD2635_STATUS_PLOW                  (1u << 6)
#define AM_TMD2635_STATUS_PSAT                  (1u << 5)
#define AM_TMD2635_STATUS_PINT                  (1u << 4)
#define AM_TMD2635_STATUS_CINT                  (1u << 3)
#define AM_TMD2635_STATUS_ZINT                  (1u << 2)
#define AM_TMD2635_STATUS_PSAT_REFLECTIVE       (1u << 1)
#define AM_TMD2635_STATUS_PSAT_AMBIENT          (1u << 0)

/**
 * \brief 传感器 TMD2635 Prox等待时间
 */
#define AM_TMD2635_PWTIME_2_78MS_OR_33_4MS      (0x00)
#define AM_TMD2635_PWTIME_5_56MS_OR_66_7MS      (0x01)
#define AM_TMD2635_PWTIME_50MS_OR_600MS         (0x11)
#define AM_TMD2635_PWTIME_100MS_OR_1_2S         (0x23)
#define AM_TMD2635_PWTIME_178MS_OR_2_14S        (0x3F)
#define AM_TMD2635_PWTIME_712MS_OR_8_54S        (0xFF)

/**
 * \brief 传感器 TMD2635 光电二极管选择
 */
#define AM_TMD2635_PDSELECT_NO_PHOTODIODE       (0x00)
#define AM_TMD2635_PDSELECT_FAR_PHOTODIODE      (0x01)
#define AM_TMD2635_PDSELECT_NEAR_PHOTODIODE     (0x02)
#define AM_TMD2635_PDSELECT_BOTH_PHOTODIODES    (0x03)

/**
 * \brief 传感器 TMD2635 APC设置
 */
#define AM_TMD2635_APC_ENABLE                   (0x00)
#define AM_TMD2635_APC_DISABLE                  (0x01)

/**
 * \brief 传感器 TMD2635 设置PMAVG
 */
#define AM_TMD2635_PMAVG_DISABLE                (0x00)
#define AM_TMD2635_PMAVG_2                      (0x01)
#define AM_TMD2635_PMAVG_4                      (0x02)
#define AM_TMD2635_PMAVG_8                      (0x03)

/**
 * \brief 传感器 TMD2635 CALAVG设置
 */
#define AM_TMD2635_NO_HARDWARE_AVERAGING        (0x00)
#define AM_TMD2635_HARDWARE_AVERAGING_ENABLED   (0x01)

/**
 * \brief 传感器 TMD2635 Prox校准类型选择
 */
#define AM_TMD2635_ELECTRICAL_OPTICAL_CROSSTALK (0x00)
#define AM_TMD2635_ELECTRICAL_CROSSTALK_ONLY    (0x01)

/**
 * \brief 传感器 TMD2635 CALPRATE设置
 */
#define AM_TMD2635_PRATE_IGNORED                (0x00)
#define AM_TMD2635_PRATE_APPLIED                (0x01)

/**
 * \brief 传感器 TMD2635 二进制搜索目标
 */
#define AM_TMD2635_BINSRCH_TARGET_3             (0x00)
#define AM_TMD2635_BINSRCH_TARGET_7             (0x01)
#define AM_TMD2635_BINSRCH_TARGET_15            (0x02)
#define AM_TMD2635_BINSRCH_TARGET_31            (0x03)
#define AM_TMD2635_BINSRCH_TARGET_63            (0x04)
#define AM_TMD2635_BINSRCH_TARGET_127           (0x05)
#define AM_TMD2635_BINSRCH_TARGET_255           (0x06)
#define AM_TMD2635_BINSRCH_TARGET_511           (0x07)

/**
 * \brief 传感器 TMD2635 PROX_AVG 采样数量
 */
#define AM_TMD2635_SAMPLE_SIZE_DISABLE          (0x00)
#define AM_TMD2635_SAMPLE_SIZE_2                (0x01)
#define AM_TMD2635_SAMPLE_SIZE_4                (0x02)
#define AM_TMD2635_SAMPLE_SIZE_8                (0x03)
#define AM_TMD2635_SAMPLE_SIZE_16               (0x04)
#define AM_TMD2635_SAMPLE_SIZE_32               (0x05)
#define AM_TMD2635_SAMPLE_SIZE_64               (0x06)
#define AM_TMD2635_SAMPLE_SIZE_128              (0x07)

/**
 * \brief 传感器 TMD2635 中断使能
 */
#define AM_TMD2635_PIM                          (1u << 5)
#define AM_TMD2635_PIEN                         (1u << 4)
#define AM_TMD2635_PSIEN                        (1u << 3)
#define AM_TMD2635_CIEN                         (1u << 2)
#define AM_TMD2635_ZIEN                         (1u << 1)

/**
 * \brief 传感器 TMD2635 控制指令定义
 */
#define AM_SENSOR_TMD2635_FUNC_ENABLE               (1)
#define AM_SENSOR_TMD2635_FUNC_DISABLE              (2)
#define AM_SENSOR_TMD2635_PRATE_SET                 (3)
#define AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_SET    (4)
#define AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_SET   (5)
#define AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_GET    (6)
#define AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_GET   (7)
#define AM_SENSOR_TMD2635_PPERS_SET                 (8)
#define AM_SENSOR_TMD2635_PWLONG_SET                (9)
#define AM_SENSOR_TMD2635_PGAIN_SET                 (10)
#define AM_SENSOR_TMD2635_PPULSE_SET                (11)
#define AM_SENSOR_TMD2635_PPULSE_LEN_SET            (12)
#define AM_SENSOR_TMD2635_PLDRIVE_SET               (13)
#define AM_SENSOR_TMD2635_REVID_GET                 (14)
#define AM_SENSOR_TMD2635_ID_GET                    (15)
#define AM_SENSOR_TMD2635_STATUS_FLAG_CLR           (16)
#define AM_SENSOR_TMD2635_REVID2_GET                (17)
#define AM_SENSOR_TMD2635_SOFTRST                   (18)
#define AM_SENSOR_TMD2635_PWTIME_SET                (19)
#define AM_SENSOR_TMD2635_PDSELECT_SET              (20)
#define AM_SENSOR_TMD2635_APC_SET                   (21)
#define AM_SENSOR_TMD2635_PMAVG_SET                 (22)
#define AM_SENSOR_TMD2635_CALAVG_SET                (23)
#define AM_SENSOR_TMD2635_PROX_CAL_TYPE_SEL         (24)
#define AM_SENSOR_TMD2635_CALPRATE_SET              (25)
#define AM_SENSOR_TMD2635_START_OFFSET_CAL_SET      (26)
#define AM_SENSOR_TMD2635_BINSRCH_TARGET_SET        (27)
#define AM_SENSOR_TMD2635_AUTO_OFFSET_ADJ_SET       (28)
#define AM_SENSOR_TMD2635_PROX_AVG_SET              (29)
#define AM_SENSOR_TMD2635_INT_ENABLE                (30)
#define AM_SENSOR_TMD2635_INT_DISABLE               (31)

/*
 * \brief 传感器TMD2635提供的所有通道ID定义
 */
#define AM_TMD2635_CHAN_1      0              /* TMD2635 的通道1，Prox */

/**
 * \brief TMD2635参数配置初始化结构体定义 
 */
typedef struct tmd2635_param_config
{
    uint8_t     ppers_val;              /**< \brief Prox中断过滤器 */
    am_bool_t   pwlong_en;              /**< \brief PWLONG使能设置，使能后WTIME值将扩大12倍 */
    uint8_t     pgain_val;              /**< \brief Prox脉冲长度 */
    uint8_t     pulse;                  /**< \brief 一个Prox周期的最大脉冲数 */
    uint8_t     pulse_len;              /**< \brief Prox IR传感器增益 */
    uint8_t     pldrive_val;            /**< \brief Prox IR VCSEL电流 */
    uint8_t     pwtime_val;             /**< \brief Prox等待时间 */
    uint8_t     pdselect;               /**< \brief 光电二极管选择 */
    am_bool_t   sleep_after_interrupt;  /**< \brief SAI */
    uint8_t     apc_disable;            /**< \brief APC禁能设置 */
    uint8_t     pmavg_val;              /**< \brief PMAVG设置 */
    uint8_t     calavg;                 /**< \brief CALAVG设置 */
    uint8_t     prox_cal_type;          /**< \brief Prox校准类型 */
    uint8_t     calprate;               /**< \brief CALPRATE设置 */
    uint8_t     binsrch_target;         /**< \brief BINSRCH_TARGET */
    am_bool_t   auto_offset_adj;        /**< \brief AUTO_OFFSET_ADJ */
    uint8_t     prox_avg;               /**< \brief PROX_AVG */
    uint32_t    prox_sample_time_us;    /**< \brief Prox采样持续时间:(PRATE + 1) * 88us */
} tmd2635_param_config_t;

/**
 * \brief 传感器 TMD2635 设备信息结构体
 */
typedef struct am_sensor_tmd2635_devinfo {

    const   tmd2635_param_config_t      *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief 报警输出引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_tmd2635_devinfo_t;

/**
 * \brief TMD2635 Prox校准值保存
 */
typedef struct am_tmd2635_calibration_data {
    int16_t   prox_offset_adj;
} am_tmd2635_calibration_data_t;

/**
 * \brief TMD2635 结果结构体定义
 */
typedef struct tmd2635_result_info
{
    uint16_t raw_prox_value;            /**< \brief Prox AD值 */
    uint16_t prox_value;                /**< \brief Prox 值 */
    int16_t  prox_offset_adjust_value;  /**< \brief Prox 校准值 */
} tmd2635_result_info_t;

/**
 * \breif 传感器 TMD2635 设备结构体定义
 */
typedef struct am_sensor_tmd2635_dev {
    am_sensor_serv_t                    tmd2635_dev;  /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_tmd2635_calibration_data_t       cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[1];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[1];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[1];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[1];

    /** \brief 设备信息 */
    const am_sensor_tmd2635_devinfo_t   *dev_info;

} am_sensor_tmd2635_dev_t;

/**
 * \brief 传感器 TMD2635 初始化
 *
 * \param[in] p_dev     : 指向传感器TMD2635设备的指针
 * \param[in] p_devinfo : 指向传感器TMD2635设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_tmd2635_init (
        am_sensor_tmd2635_dev_t             *p_dev,
        const am_sensor_tmd2635_devinfo_t   *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 TMD2635 去初始化
 *
 * \param[in] handle : am_sensor_tmd2635_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_tmd2635_deinit (am_sensor_handle_t   handle);

/**
 * \brief 传感器 TMD2635 控制函数
 *
 * \param[in] handle    : am_sensor_tmd2635_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *   - AM_SENSOR_TMD2635_FUNC_ENABLE             : 设置功能使能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_FUNC_DISABLE            : 设置功能禁能 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PRATE_SET               : 设置PRATE, p_arg为 uint32_t类型
 *   - AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_SET  : 设置Prox低阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_SET : 设置Prox高阈值, p_arg为 uint16_t类型
 *   - AM_SENSOR_TMD2635_LOW_PROX_THRESHOLD_GET  : 获取Prox低阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2635_HIGH_PROX_THRESHOLD_GET : 获取Prox高阈值, p_arg为 (uint16_t *)类型
 *   - AM_SENSOR_TMD2635_PPERS_SET               : 设置PPERS , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PWLONG_SET              : 设置PWLONG, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2635_PGAIN_SET               : 设置Prox IR传感器增益, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PPULSE_SET              : 设置Prox周期脉冲的最大数量 , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PPULSE_LEN_SET          : 设置PPULSE_LEN , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PLDRIVE_SET             : 设置Prox IR VCSEL电流驱动能力, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_REVID_GET               : 读取REVID , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2635_ID_GET                  : 读取ID, p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2635_STATUS_FLAG_CLR         : STATUS寄存器位清除, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_REVID2_GET              : 读取REVID2 , p_arg为 (uint8_t *)类型
 *   - AM_SENSOR_TMD2635_SOFTRST                 : 软件复位 , p_arg为 NULL
 *   - AM_SENSOR_TMD2635_PWTIME_SET              : 设置PWTIME, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PDSELECT_SET            : 设置光电二极管, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_APC_SET                 : 设置APC, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PMAVG_SET               : 设置PMAVG, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_CALAVG_SET              : 设置CALAVG, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_PROX_CAL_TYPE_SEL       : 选择Prox校准类型, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_CALPRATE_SET            : 设置CALPRATE, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_START_OFFSET_CAL_SET    : 设置偏移校准, p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2635_BINSRCH_TARGET_SET      : 设置BINSRCH_TARGET , p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_AUTO_OFFSET_ADJ_SET     : 设置AUTO_OFFSET_ADJ , p_arg为 am_bool_t类型
 *   - AM_SENSOR_TMD2635_PROX_AVG_SET            : 设置PROX_AVG, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_INT_ENABLE              : 中断使能, p_arg为 uint8_t类型
 *   - AM_SENSOR_TMD2635_INT_DISABLE             : 中断禁能, p_arg为 uint8_t类型
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_tmd2635_ioctl (am_sensor_handle_t    handle,
                                    int                 cmd,
                                    void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_TMD2635_H */

/* end of file */


