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
 * \brief 光谱传感器 AS7341 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS7341_H
#define __AM_SENSOR_AS7341_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 AS7341 LED_SEL
 */
#define AM_AS7341_LED_UNCONTROLLED                  (0x00)
#define AM_AS7341_LED_CONTROLLED                    (0x01)

/**
 * \brief 传感器 AS7341 INT_MODE
 */
#define AM_AS7341_INT_MODE_SPM                      (0x00)
#define AM_AS7341_INT_MODE_SYNS                     (0x01)
#define AM_AS7341_INT_MODE_SYND                     (0x03)

/**
 * \brief 传感器 AS7341 PD_GPIO
 */
#define AM_AS7341_PD_GPIO_DISCONNECTED              (0x00)
#define AM_AS7341_PD_GPIO_CONNECTED                 (0x01)

/**
 * \brief 传感器 AS7341 PD_INT
 */
#define AM_AS7341_PD_INT_DISCONNECTED               (0x00)
#define AM_AS7341_PD_INT_CONNECTED                  (0x01)

/**
 * \brief 传感器 AS7341 LED_ACT
 */
#define AM_AS7341_LED_ACT_DISCONNECTED              (0x00)
#define AM_AS7341_LED_ACT_CONNECTED                 (0x01)

/**
 * \brief 传感器 AS7341 使能设置
 */
#define AM_AS7341_FDEN                              (1u << 6)
#define AM_AS7341_SMUXEN                            (1u << 4)
#define AM_AS7341_WEN                               (1u << 3)
#define AM_AS7341_SP_EN                             (1u << 1)
#define AM_AS7341_PON                               (1u << 0)

/**
 * \brief 传感器 AS7341 光谱测量等待时间
    等待时间(单位:ms) ---- (WLONG = 0) : 2.78ms * (WTIME + 1)
                      ---- (WLONG = 1) : 44.48ms * (WTIME + 1)
 */
#define AM_AS7341_WTIME_2_78MS_OR_44_48MS           (0x00)
#define AM_AS7341_WTIME_5_56MS_OR_88_96MS           (0x01)
#define AM_AS7341_WTIME_711MS_OR_11_39S             (0xFF)

/**
 * \brief 传感器 AS7341 STATUS位
 */
#define AM_AS7341_STATUS_ASAT                       (1u << 7)
#define AM_AS7341_STATUS_AINT                       (1u << 3)
#define AM_AS7341_STATUS_FINT                       (1u << 2)
#define AM_AS7341_STATUS_CINT                       (1u << 1)
#define AM_AS7341_STATUS_SINT                       (1u << 0)

/**
 * \brief 传感器 AS7341 低功耗空闲
 */
#define AM_AS7341_LOWPOWER_IDLE_DISABLE             (0x00)
#define AM_AS7341_LOWPOWER_IDLE_ENABLE              (0x01)

/**
 * \brief 传感器 AS7341 REG_BANK
 */
#define AM_AS7341_ACCESS_REG_0X80_AND_ABOVE         (0x00)
#define AM_AS7341_ACCESS_REG_0X60_TO_0X74           (0x01)

/**
 * \brief 传感器 AS7341 光谱增益
 */
#define AM_AS7341_AGAIN_0_5                         (0x00)
#define AM_AS7341_AGAIN_1                           (0x01)
#define AM_AS7341_AGAIN_2                           (0x02)
#define AM_AS7341_AGAIN_4                           (0x03)
#define AM_AS7341_AGAIN_8                           (0x04)
#define AM_AS7341_AGAIN_16                          (0x05)
#define AM_AS7341_AGAIN_32                          (0x06)
#define AM_AS7341_AGAIN_64                          (0x07)
#define AM_AS7341_AGAIN_128                         (0x08)
#define AM_AS7341_AGAIN_256                         (0x09)
#define AM_AS7341_AGAIN_512                         (0x0A)

/**
 * \brief 传感器 AS7341 SMUX_CMD
 */
#define AM_AS7341_SMUX_CMD_0                        (0x00)
#define AM_AS7341_SMUX_CMD_1                        (0x01)
#define AM_AS7341_SMUX_CMD_2                        (0x02)

/**
 * \brief 传感器 AS7341 FIFO_TH
 */
#define AM_AS7341_FIFO_TH_1                         (0x00)
#define AM_AS7341_FIFO_TH_4                         (0x01)
#define AM_AS7341_FIFO_TH_8                         (0x02)
#define AM_AS7341_FIFO_TH_16                        (0x03)

/**
 * \brief 传感器 AS7341 FD_AGC
 */
#define AM_AS7341_FD_AGC_DISABLE                    (0x00)
#define AM_AS7341_FD_AGC_ENABLE                     (0x01)

/**
 * \brief 传感器 AS7341 SP_AGC
 */
#define AM_AS7341_SP_AGC_DISABLE                    (0x00)
#define AM_AS7341_SP_AGC_ENABLE                     (0x01)

/**
 * \brief 传感器 AS7341 SIEN_FD
 */
#define AM_AS7341_SIEN_FD_DISABLE                   (0x00)
#define AM_AS7341_SIEN_FD_ENABLE                    (0x01)

/**
 * \brief 传感器 AS7341 SIEN_SMUX
 */
#define AM_AS7341_SIEN_SMUX_DISABLE                 (0x00)
#define AM_AS7341_SIEN_SMUX_ENABLE                  (0x01)

/**
 * \brief 传感器 AS7341 AGC_H
 */
#define AM_AS7341_AGC_H_50_PERCENT                  (0x00)
#define AM_AS7341_AGC_H_62_5_PERCENT                (0x01)
#define AM_AS7341_AGC_H_75_PERCENT                  (0x02)
#define AM_AS7341_AGC_H_87_5_PERCENT                (0x03)

/**
 * \brief 传感器 AS7341 AGC_L
 */
#define AM_AS7341_AGC_L_12_5_PERCENT                (0x00)
#define AM_AS7341_AGC_L_25_PERCENT                  (0x01)
#define AM_AS7341_AGC_L_37_5_PERCENT                (0x02)
#define AM_AS7341_AGC_L_50_PERCENT                  (0x03)

/**
 * \brief 传感器 AS7341 SP_TH_CH
 */
#define AM_AS7341_SP_TH_CH0                         (0x00)
#define AM_AS7341_SP_TH_CH1                         (0x01)
#define AM_AS7341_SP_TH_CH2                         (0x02)
#define AM_AS7341_SP_TH_CH3                         (0x03)
#define AM_AS7341_SP_TH_CH4                         (0x04)

/**
 * \brief 传感器 AS7341 AGC_FD_GAIN_MAX
 */
#define AM_AS7341_AGC_FD_GAIN_MAX_0_5               (0x00)
#define AM_AS7341_AGC_FD_GAIN_MAX_1                 (0x01)
#define AM_AS7341_AGC_FD_GAIN_MAX_2                 (0x02)
#define AM_AS7341_AGC_FD_GAIN_MAX_4                 (0x03)
#define AM_AS7341_AGC_FD_GAIN_MAX_8                 (0x04)
#define AM_AS7341_AGC_FD_GAIN_MAX_16                (0x05)
#define AM_AS7341_AGC_FD_GAIN_MAX_32                (0x06)
#define AM_AS7341_AGC_FD_GAIN_MAX_64                (0x07)
#define AM_AS7341_AGC_FD_GAIN_MAX_128               (0x08)
#define AM_AS7341_AGC_FD_GAIN_MAX_256               (0x09)
#define AM_AS7341_AGC_FD_GAIN_MAX_512               (0x0A)

/**
 * \brief 传感器 AS7341 AGC_AGAIN_MAX
 */
#define AM_AS7341_AGC_AGAIN_MAX_0_5                 (0x00)
#define AM_AS7341_AGC_AGAIN_MAX_1                   (0x01)
#define AM_AS7341_AGC_AGAIN_MAX_2                   (0x02)
#define AM_AS7341_AGC_AGAIN_MAX_4                   (0x03)
#define AM_AS7341_AGC_AGAIN_MAX_8                   (0x04)
#define AM_AS7341_AGC_AGAIN_MAX_16                  (0x05)
#define AM_AS7341_AGC_AGAIN_MAX_32                  (0x06)
#define AM_AS7341_AGC_AGAIN_MAX_64                  (0x07)
#define AM_AS7341_AGC_AGAIN_MAX_128                 (0x08)
#define AM_AS7341_AGC_AGAIN_MAX_256                 (0x09)
#define AM_AS7341_AGC_AGAIN_MAX_512                 (0x0A)

/**
 * \brief 传感器 AS7341 FD_GAIN
 */
#define AM_AS7341_FD_GAIN_0_5                       (0x00)
#define AM_AS7341_FD_GAIN_1                         (0x01)
#define AM_AS7341_FD_GAIN_2                         (0x02)
#define AM_AS7341_FD_GAIN_4                         (0x03)
#define AM_AS7341_FD_GAIN_8                         (0x04)
#define AM_AS7341_FD_GAIN_16                        (0x05)
#define AM_AS7341_FD_GAIN_32                        (0x06)
#define AM_AS7341_FD_GAIN_64                        (0x07)
#define AM_AS7341_FD_GAIN_128                       (0x08)
#define AM_AS7341_FD_GAIN_256                       (0x09)
#define AM_AS7341_FD_GAIN_512                       (0x0A)

/**
 * \brief 传感器 AS7341 中断使能
 */
#define AM_AS7341_ASIEN                             (1u << 7)
#define AM_AS7341_SP_IEN                            (1u << 3)
#define AM_AS7341_F_IEN                             (1u << 2)
#define AM_AS7341_SIEN                              (1u << 0)

/**
 * \brief 传感器 AS7341 FIFO_MAP
 */
#define AM_AS7341_FIFO_WRITE_CH5_DATA               (1u << 6)
#define AM_AS7341_FIFO_WRITE_CH4_DATA               (1u << 5)
#define AM_AS7341_FIFO_WRITE_CH3_DATA               (1u << 4)
#define AM_AS7341_FIFO_WRITE_CH2_DATA               (1u << 3)
#define AM_AS7341_FIFO_WRITE_CH1_DATA               (1u << 2)
#define AM_AS7341_FIFO_WRITE_CH0_DATA               (1u << 1)
#define AM_AS7341_FIFO_WRITE_ASTATUS                (1u << 0)

/**
 * \brief 传感器 AS7341 控制指令定义
 */
typedef enum am_sensor_as7341_control_cmd {
    AM_SENSOR_AS7341_ITIME_GET = 1,
    AM_SENSOR_AS7341_LED_SEL,
    AM_SENSOR_AS7341_INT_SEL,
    AM_SENSOR_AS7341_INT_MODE_SET,
    AM_SENSOR_AS7341_STAT_GET,
    AM_SENSOR_AS7341_EDGE_SET,
    AM_SENSOR_AS7341_PD_GPIO_SET,
    AM_SENSOR_AS7341_PD_INT_SET,
    AM_SENSOR_AS7341_LED_ACT_SET,
    AM_SENSOR_AS7341_LED_DRIVE_SET,
    AM_SENSOR_AS7341_FUNC_ENABLE,
    AM_SENSOR_AS7341_FUNC_DISABLE,
    AM_SENSOR_AS7341_ATIME_SET,
    AM_SENSOR_AS7341_WTIME_SET,
    AM_SENSOR_AS7341_LOW_SPECTRAL_THRESHOLD_SET,
    AM_SENSOR_AS7341_HIGH_SPECTRAL_THRESHOLD_SET,
    AM_SENSOR_AS7341_LOW_SPECTRAL_THRESHOLD_GET,
    AM_SENSOR_AS7341_HIGH_SPECTRAL_THRESHOLD_GET,
    AM_SENSOR_AS7341_AUXID_GET,
    AM_SENSOR_AS7341_REVID_GET,
    AM_SENSOR_AS7341_ID_GET,
    AM_SENSOR_AS7341_STATUS_FLAG_CLR,
    AM_SENSOR_AS7341_STATUS_FLAG_ALL_CLR,
    AM_SENSOR_AS7341_STATUS_GET,
    AM_SENSOR_AS7341_ASTATUS_GET,
    AM_SENSOR_AS7341_STATUS2_GET,
    AM_SENSOR_AS7341_STATUS3_GET,
    AM_SENSOR_AS7341_STATUS5_GET,
    AM_SENSOR_AS7341_STATUS6_GET,
    AM_SENSOR_AS7341_LOWPOWER_IDLE_SET,
    AM_SENSOR_AS7341_WLONG_SET,
    AM_SENSOR_AS7341_AGAIN_SET,
    AM_SENSOR_AS7341_SAI_SET,
    AM_SENSOR_AS7341_SMUX_CMD_SET,
    AM_SENSOR_AS7341_FIFO_TH_SET,
    AM_SENSOR_AS7341_FD_AGC_SET,
    AM_SENSOR_AS7341_SP_AGC_SET,
    AM_SENSOR_AS7341_SIEN_FD_SET,
    AM_SENSOR_AS7341_SIEN_SMUX_SET,
    AM_SENSOR_AS7341_AGC_H_SET,
    AM_SENSOR_AS7341_AGC_L_SET,
    AM_SENSOR_AS7341_FD_PERS_SET,
    AM_SENSOR_AS7341_SP_TH_CH_SET,
    AM_SENSOR_AS7341_APERS_SET,
    AM_SENSOR_AS7341_GPIO_INV_SET,
    AM_SENSOR_AS7341_GPIO_IN_EN_SET,
    AM_SENSOR_AS7341_GPIO_OUT_SET,
    AM_SENSOR_AS7341_GPIO_IN_GET,
    AM_SENSOR_AS7341_ASTEP_SET,
    AM_SENSOR_AS7341_AGC_FD_GAIN_MAX_SET,
    AM_SENSOR_AS7341_AGC_AGAIN_MAX_SET,
    AM_SENSOR_AS7341_AZ_NTH_ITERATION_SET,
    AM_SENSOR_AS7341_FD_TIME_SET,
    AM_SENSOR_AS7341_FD_GAIN_SET,
    AM_SENSOR_AS7341_FIFO_WRITE_FD_SET,
    AM_SENSOR_AS7341_FD_STATUS_GET,
    AM_SENSOR_AS7341_FD_STATUS_FLAG_CLR,
    AM_SENSOR_AS7341_FD_STATUS_FLAG_ALL_CLR,
    AM_SENSOR_AS7341_INT_ENABLE,
    AM_SENSOR_AS7341_INT_DISABLE,
    AM_SENSOR_AS7341_SP_MAN_AZ_SET,
    AM_SENSOR_AS7341_FIFO_CLR,
    AM_SENSOR_AS7341_CLEAR_SAI_ACT,
    AM_SENSOR_AS7341_FIFO_MAP_ENABLE,
    AM_SENSOR_AS7341_FIFO_MAP_DISABLE,
} am_sensor_as7341_control_cmd_t;

/*
 * \brief 传感器AS7341提供的所有通道ID定义
 */
#define AM_AS7341_CHAN_1      0              /* AS7341 的通道1，Spectral Channel */

/**
 * \brief AS7341参数配置初始化结构体定义 
 */
typedef struct as7341_param_config
{
    uint8_t     led_sel;                    /**< \brief LED_SEL          */
    uint8_t     led_act;                    /**< \brief LED_ACT          */
    uint8_t     led_drive;                  /**< \brief LED_DRIVE        */
    uint8_t     int_sel;                    /**< \brief INT_SEL          */
    uint8_t     int_mode;                   /**< \brief INT_MODE         */
    uint8_t     edge;                       /**< \brief EDGE             */
    uint8_t     pd_gpio;                    /**< \brief PD_GPIO          */
    uint8_t     pd_int;                     /**< \brief PD_INT           */
    uint16_t    astep;                      /**< \brief ASTEP            */
    uint8_t     atime_val;                  /**< \brief ATIME            */
    uint8_t     wtime_val;                  /**< \brief WTIME            */
    uint8_t     lowpower_idle;              /**< \brief LOWPOWER_IDLE    */
    am_bool_t   wlong_en;                   /**< \brief WLONG            */
    uint8_t     again_val;                  /**< \brief AGAIN            */
    am_bool_t   sleep_after_interrupt;      /**< \brief SAI              */
    uint8_t     fifo_th;                    /**< \brief FIFO_TH          */
    uint8_t     fd_agc;                     /**< \brief FD_AGC           */
    uint8_t     sp_agc;                     /**< \brief SP_AGC           */
    uint8_t     agc_h;                      /**< \brief AGC_H            */
    uint8_t     agc_l;                      /**< \brief AGC_L            */
    uint8_t     fd_pers;                    /**< \brief FD_PERS          */
    uint8_t     sp_th_ch;                   /**< \brief SP_TH_CH         */
    uint8_t     apers_val;                  /**< \brief APERS            */
    uint8_t     gpio_inv;                   /**< \brief GPIO_INV         */
    uint8_t     gpio_in_en;                 /**< \brief GPIO_IN_EN       */
    uint8_t     gpio_out;                   /**< \brief GPIO_OUT         */
    uint8_t     agc_fd_gain_max;            /**< \brief AGC_FD_GAIN_MAX  */
    uint8_t     agc_again_max;              /**< \brief AGC_AGAIN_MAX    */
    uint8_t     az_nth_iteration;           /**< \brief AZ_NTH_ITERATION */
    uint16_t    fd_time;                    /**< \brief FD_TIME          */
    uint8_t     fd_gain;                    /**< \brief FD_GAIN          */
    uint8_t     fifo_write_fd;              /**< \brief FIFO_WRITE_FD    */
    uint8_t     fifo_map_en;                /**< \brief FIFO_MAP_ENABLE  */
} as7341_param_config_t;

/**
 * \brief 传感器 AS7341 设备信息结构体
 */
typedef struct am_sensor_as7341_devinfo {

    const   as7341_param_config_t       *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief INT引脚 */

    int                                  gpio_pin;          /**< \brief GPIO引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_as7341_devinfo_t;

/**
 * \brief AS7341 校准值保存 (此处无作用)
 */
typedef struct am_as7341_calibration_data {
    int16_t offset_chx_data;
} am_as7341_calibration_data_t;

/**
 * \brief AS7341 结果结构体定义
 */
typedef struct as7341_result_info
{
    uint16_t raw_data[12];      /**< \brief CHx AD值 */
    uint16_t ch_data[12];       /**< \brief CHx 值 */
    uint32_t synd_itime;        /**< \brief 获取SYND模式的积分时间 */
} as7341_result_info_t;

/**
 * \breif 传感器 AS7341 设备结构体定义
 */
typedef struct am_sensor_as7341_dev {
    am_sensor_serv_t                    as7341_dev;     /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_as7341_calibration_data_t        cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[1];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[1];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    uint8_t                             cfg0_regval;    /*< \brief 记录CFG0寄存器值 */
    uint8_t                             int_mode;       /*< \brief 记录INT_MODE */
    am_sensor_val_t                     data[12];       /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[1];

    /** \brief 设备信息 */
    const am_sensor_as7341_devinfo_t *dev_info;

} am_sensor_as7341_dev_t;

/**
 * \brief 传感器 AS7341 初始化
 *
 * \param[in] p_dev     : 指向传感器AS7341设备的指针
 * \param[in] p_devinfo : 指向传感器AS7341设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as7341_init (
        am_sensor_as7341_dev_t              *p_dev,
        const am_sensor_as7341_devinfo_t    *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 AS7341 去初始化
 *
 * \param[in] handle : am_sensor_as7341_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as7341_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 AS7341 控制函数
 *
 * \param[in] handle    : am_sensor_as7341_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_as7341_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS7341_H */

/* end of file */

