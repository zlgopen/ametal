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
 * \brief 高光谱传感器 AS7421 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS7421_H
#define __AM_SENSOR_AS7421_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 AS7421 INT_PINMAP
 */
#define AM_AS7421_INT_PINMAP_INTX                           (0x00)
#define AM_AS7421_INT_PINMAP_LTF_READY                      (0x01)
#define AM_AS7421_INT_PINMAP_LTF_BUSY                       (0x02)
#define AM_AS7421_INT_PINMAP_LED_ON                         (0x03)

/**
 * \brief 传感器 AS7421 INT_INVERT
 */
#define AM_AS7421_INT_INVERT_DISABLE                        (0x00)
#define AM_AS7421_INT_INVERT_ENABLE                         (0x01)

/**
 * \brief 传感器 AS7421 GPIO_PINMAP
 */
#define AM_AS7421_GPIO_PINMAP_GPIO                          (0x00)
#define AM_AS7421_GPIO_PINMAP_LTF_READY                     (0x01)
#define AM_AS7421_GPIO_PINMAP_LTF_BUSY                      (0x02)
#define AM_AS7421_GPIO_PINMAP_LED_ON                        (0x03)

/**
 * \brief 传感器 AS7421 LED_AUTO
 */
#define AM_AS7421_LED_AUTO_LEDS_OFF                         (0x00)
#define AM_AS7421_LED_AUTO_1ST_OFF_2ND_ON                   (0x01)
#define AM_AS7421_LED_AUTO_1ST_ON_2ND_OFF                   (0x02)
#define AM_AS7421_LED_AUTO_LEDS_ON                          (0x03)

/**
 * \brief 传感器 AS7421 使能设置
 */
#define AM_AS7421_SYNC_EN                                   (1u << 3)
#define AM_AS7421_TSD_EN                                    (1u << 2)
#define AM_AS7421_LTF_EN                                    (1u << 1)
#define AM_AS7421_PON                                       (1u << 0)

/**
 * \brief 传感器 AS7421 LTF_CYCLE
 */
#define AM_AS7421_INTEGRATION_CYCLE_1                       (0x00)
#define AM_AS7421_INTEGRATION_CYCLE_2                       (0x01)
#define AM_AS7421_INTEGRATION_CYCLE_3                       (0x02)
#define AM_AS7421_INTEGRATION_CYCLE_4                       (0x03)

/**
 * \brief 传感器 AS7421 CLKMOD
 */
#define AM_AS7421_CLKMOD_2MHZ                               (0x02)
#define AM_AS7421_CLKMOD_1MHZ                               (0x04)
#define AM_AS7421_CLKMOD_500KHZ                             (0x05)
#define AM_AS7421_CLKMOD_250KHZ                             (0x06)

/**
 * \brief 传感器 AS7421 SET_LED_ON
 */
#define AM_AS7421_SET_LED_ON_LED_DISABLE                    (0x00)
#define AM_AS7421_SET_LED_ON_ENABLE_LED_PERMANENTLY         (0x01)

/**
 * \brief 传感器 AS7421 LED_OFF_EN
 */
#define AM_AS7421_LED_OFF_EN_NORMAL_MODE                    (0x00)
#define AM_AS7421_LED_OFF_EN_LED_OFF_DURING_MODULATION      (0x01)

/**
 * \brief 传感器 AS7421 LED_OFFSET
 */
#define AM_AS7421_LED_MULT_0_FOR_INTEGRATION_CYCLE_A        (0x00)
#define AM_AS7421_LED_MULT_1_FOR_INTEGRATION_CYCLE_B        (0x01)
#define AM_AS7421_LED_MULT_2_FOR_INTEGRATION_CYCLE_C        (0x02)
#define AM_AS7421_LED_MULT_3_FOR_INTEGRATION_CYCLE_D        (0x03)

/**
 * \brief 传感器 AS7421 LED_CURRENT
 */
#define AM_AS7421_LED_CURRENT_50MA                          (0x00)
#define AM_AS7421_LED_CURRENT_75MA                          (0x01)

/**
 * \brief 传感器 AS7421 REG_BANK
 */
#define AM_AS7421_SELECT_P2RAM_DISABLE                      (0x00)
#define AM_AS7421_SELECT_P2RAM_ENABLE                       (0x01)

/**
 * \brief 传感器 AS7421 RAM_OFFSET
 */
#define AM_AS7421_SMUX_FOR_INTEGRATION_CYCLE_A              (0x0C)
#define AM_AS7421_SMUX_FOR_INTEGRATION_CYCLE_B              (0x0D)
#define AM_AS7421_SMUX_FOR_INTEGRATION_CYCLE_C              (0x0E)
#define AM_AS7421_SMUX_FOR_INTEGRATION_CYCLE_D              (0x0F)
#define AM_AS7421_ASETUP_FOR_INTEGRATION_CYCLE_A_OR_B       (0x10)
#define AM_AS7421_ASETUP_FOR_INTEGRATION_CYCLE_C_OR_D       (0x11)
#define AM_AS7421_COMPDAC_FOR_MODULATORS_AND_INTEGRATORS    (0x12)

/**
 * \brief 传感器 AS7421 中断使能
 */
#define AM_AS7421_EN_DLOST                                  (1u << 5)
#define AM_AS7421_EN_DSAT                                   (1u << 4)
#define AM_AS7421_EN_ASAT                                   (1u << 3)
#define AM_AS7421_EN_TSD                                    (1u << 2)
#define AM_AS7421_EN_AZ                                     (1u << 1)
#define AM_AS7421_EN_ADATA                                  (1u << 0)

/**
 * \brief 传感器 AS7421 AZ_WTIME
 */
#define AM_AS7421_AZ_WTIME_32US                             (0x00)
#define AM_AS7421_AZ_WTIME_64US                             (0x01)
#define AM_AS7421_AZ_WTIME_128US                            (0x02)
#define AM_AS7421_AZ_WTIME_256US                            (0x03)

/**
 * \brief 传感器 AS7421 控制指令定义
 */
typedef enum am_sensor_as7421_control_cmd {
    AM_SENSOR_AS7421_LED_WAIT_OFF_SET = 1,
    AM_SENSOR_AS7421_WAIT_CYCLE_ON_SET,
    AM_SENSOR_AS7421_SW_RST,
    AM_SENSOR_AS7421_LED_MULT_SET,
    AM_SENSOR_AS7421_LTF_CCOUNT_SET,
    AM_SENSOR_AS7421_LTF_CCOUNT_GET,
    AM_SENSOR_AS7421_LED_WAIT_SET,
    AM_SENSOR_AS7421_INT_PINMAP_SET,
    AM_SENSOR_AS7421_INT_INVERT_SET,
    AM_SENSOR_AS7421_GPIO_PINMAP_SET,
    AM_SENSOR_AS7421_LED_AUTO_SET,
    AM_SENSOR_AS7421_FUNC_ENABLE,
    AM_SENSOR_AS7421_FUNC_DISABLE,
    AM_SENSOR_AS7421_ITIME_SET,
    AM_SENSOR_AS7421_ITIME_GET,
    AM_SENSOR_AS7421_WTIME_SET,
    AM_SENSOR_AS7421_WTIME_GET,
    AM_SENSOR_AS7421_LTF_CYCLE_SET,
    AM_SENSOR_AS7421_CLKMOD_SET,
    AM_SENSOR_AS7421_SET_LED_ON_SET,
    AM_SENSOR_AS7421_LED_OFF_EN_SET,
    AM_SENSOR_AS7421_LED_OFFSET_SET,
    AM_SENSOR_AS7421_LED_CURRENT_SET,
    AM_SENSOR_AS7421_LTF_ICOUNT_SET,
    AM_SENSOR_AS7421_REG_BANK_SET,
    AM_SENSOR_AS7421_RAM_OFFSET_SET,
    AM_SENSOR_AS7421_GPIO_INVERT_SET,
    AM_SENSOR_AS7421_GPIO_OEN_SET,
    AM_SENSOR_AS7421_GPIO_OUT_SET,
    AM_SENSOR_AS7421_GPIO_IN_SET,
    AM_SENSOR_AS7421_INT_ENABLE,
    AM_SENSOR_AS7421_INT_DISABLE,
    AM_SENSOR_AS7421_AZ_ON_SET,
    AM_SENSOR_AS7421_AZ_WTIME_SET,
    AM_SENSOR_AS7421_AZ_EN_SET,
    AM_SENSOR_AS7421_AZ_CYCLE_SET,
    AM_SENSOR_AS7421_AZ_ITERATION_SET,
    AM_SENSOR_AS7421_ID_GET,
    AM_SENSOR_AS7421_REVID_GET,
    AM_SENSOR_AS7421_STATUS2_GET,
    AM_SENSOR_AS7421_STATUS3_GET,
    AM_SENSOR_AS7421_STATUS6_GET,
    AM_SENSOR_AS7421_STATUS7_GET,

} am_sensor_as7421_control_cmd_t;

/*
 * \brief 传感器AS7421提供的所有通道ID定义
 */
#define AM_AS7421_CHAN_1      0             /* AS7421 的通道1，temperature integration cycle Channel */
#define AM_AS7421_CHAN_2      1             /* AS7421 的通道2，Spectral Channel */

/**
 * \brief AS7421参数配置初始化结构体定义 
 */
typedef struct as7421_param_config
{
    am_bool_t   led_wait_off;               /**< \brief LED_WAIT_OFF    */
    am_bool_t   wait_cycle_on;              /**< \brief WAIT_CYCLE_ON   */
    uint8_t     led_mult;                   /**< \brief LED_MULT        */
    uint8_t     led_wait;                   /**< \brief LED_WAIT        */
    uint16_t    ltf_ccount;                 /**< \brief LTF_CCOUNT      */
    uint8_t     int_pinmap;                 /**< \brief INT_PINMAP      */
    uint8_t     int_invert;                 /**< \brief INT_INVERT      */
    uint8_t     gpio_pinmap;                /**< \brief GPIO_PINMAP     */
    uint8_t     led_auto;                   /**< \brief LED_AUTO        */
    uint8_t     clkmod;                     /**< \brief CLKMOD          */
    uint32_t    ltf_itime;                  /**< \brief LTF_ITIME       */
    uint32_t    ltf_wtime;                  /**< \brief LTF_WTIME       */
    uint8_t     ltf_cycle;                  /**< \brief LTF_CYCLE       */
    uint8_t     set_led_on;                 /**< \brief SET_LED_ON      */
    uint8_t     led_off_en;                 /**< \brief LED_OFF_EN      */
    uint8_t     led_current;                /**< \brief LED_CURRENT     */
    uint8_t     ltf_icount;                 /**< \brief LTF_ICOUNT      */
    uint8_t     gpio_invert;                /**< \brief GPIO_INVERT     */
    uint8_t     gpio_oen;                   /**< \brief GPIO_OEN        */
    uint8_t     gpio_out;                   /**< \brief GPIO_OUT        */
    uint8_t     az_wtime;                   /**< \brief AZ_WTIME        */
    uint8_t     az_iteration;               /**< \brief AZ_ITERATION    */

} as7421_param_config_t;

/**
 * \brief 传感器 AS7421 设备信息结构体
 */
typedef struct am_sensor_as7421_devinfo {

    const   as7421_param_config_t       *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief INT引脚 */

    int                                  gpio_pin;          /**< \brief GPIO引脚 */

    int                                  rst_pin;           /**< \brief RST引脚 */

    uint8_t                              i2c_addr;          /**< \brief I2C 7位 设备地址 */

} am_sensor_as7421_devinfo_t;

/**
 * \brief AS7421 校准值保存 (此处无作用)
 */
typedef struct am_as7421_calibration_data {
    int16_t offset_chx_data;
} am_as7421_calibration_data_t;

/**
 * \brief AS7421 结果结构体定义
 */
typedef struct as7421_result_info
{
    uint16_t    raw_temp_ic_a;          /**< \brief 积分周期 A 温度AD值 (ic:integration cycle) */
    uint16_t    raw_temp_ic_b;          /**< \brief 积分周期 B 温度AD值 */
    uint16_t    raw_temp_ic_c;          /**< \brief 积分周期 C 温度AD值 */
    uint16_t    raw_temp_ic_d;          /**< \brief 积分周期 D 温度AD值 */
    uint16_t    ch_data[64];            /**< \brief CHx AD值 */
    int16_t     temp_ic_a;              /**< \brief 积分周期 A 温度 */
    int16_t     temp_ic_b;              /**< \brief 积分周期 B 温度 */
    int16_t     temp_ic_c;              /**< \brief 积分周期 C 温度 */
    int16_t     temp_ic_d;              /**< \brief 积分周期 D 温度 */
} as7421_result_info_t;

/**
 * \breif 传感器 AS7421 设备结构体定义
 */
typedef struct am_sensor_as7421_dev {
    am_sensor_serv_t                    as7421_dev;     /*< \breif 传感器标准服务 */
    am_i2c_device_t                     i2c_dev;        /*< \brief i2c设备实例 */
    am_as7421_calibration_data_t        cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[2];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[2];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint8_t                             trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[68];       /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[2];

    /** \brief 设备信息 */
    const am_sensor_as7421_devinfo_t *dev_info;

} am_sensor_as7421_dev_t;

/**
 * \brief 传感器 AS7421 初始化
 *
 * \param[in] p_dev     : 指向传感器AS7421设备的指针
 * \param[in] p_devinfo : 指向传感器AS7421设备信息的指针
 * \param[in] handle    : IIC标准服务句柄
 *
 * \return 传感器标准服务操作句柄如果为 NULL，表明初始化失败
 */
am_sensor_handle_t am_sensor_as7421_init (
        am_sensor_as7421_dev_t              *p_dev,
        const am_sensor_as7421_devinfo_t    *p_devinfo,
        am_i2c_handle_t                      handle);

/**
 * \brief 传感器 AS7421 去初始化
 *
 * \param[in] handle : am_sensor_as7421_init()初始化函数获得的传感器标准服务句柄
 *
 * \return 无
 */
am_err_t am_sensor_as7421_deinit (am_sensor_handle_t handle);

/**
 * \brief 传感器 AS7421 控制函数
 *
 * \param[in] handle    : am_sensor_as7421_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_as7421_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS7421_H */

/* end of file */

