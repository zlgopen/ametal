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
 * \brief 光谱传感器 AS7262 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-04-26  fzb, first implementation.
 * \endinternal
 */

#ifndef __AM_SENSOR_AS7262_H
#define __AM_SENSOR_AS7262_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_spi.h"
#include "am_sensor.h"
#include "am_isr_defer.h"

/**
 * \brief 传感器 AS7262 INT引脚输出
 */
#define AM_AS7262_INT_OUTPUT_DISABLE        (0x00)
#define AM_AS7262_INT_OUTPUT_ENABLE         (0x01)

/**
 * \brief 传感器 AS7262 增益
 */
#define AM_AS7262_GAIN_1                    (0x00)
#define AM_AS7262_GAIN_3P7                  (0x01)
#define AM_AS7262_GAIN_16                   (0x02)
#define AM_AS7262_GAIN_64                   (0x03)

/**
 * \brief 传感器 AS7262 BANK_MODE
 */
#define AM_AS7262_BANK_MODE_0               (0x00)
#define AM_AS7262_BANK_MODE_1               (0x01)
#define AM_AS7262_BANK_MODE_2               (0x02)
#define AM_AS7262_BANK_MODE_3               (0x03)
#define AM_AS7262_SENSORS_OFF               (0x04)  /* 仅在UART模式下有效 */

/**
 * \brief 传感器 AS7262 LED_DRV电流限制
 */
#define AM_AS7262_LED_DRV_12P5_MA           (0x00)
#define AM_AS7262_LED_DRV_25_MA             (0x01)
#define AM_AS7262_LED_DRV_50_MA             (0x02)
#define AM_AS7262_LED_DRV_100_MA            (0x03)

/**
 * \brief 传感器 AS7262 LED_DRV使能设置
 */
#define AM_AS7262_LED_DRV_DISABLE           (0x00)
#define AM_AS7262_LED_DRV_ENABLE            (0x01)

/**
 * \brief 传感器 AS7262 LED_IND电流限制
 */
#define AM_AS7262_LED_IND_1_MA              (0x00)
#define AM_AS7262_LED_IND_2_MA              (0x01)
#define AM_AS7262_LED_IND_4_MA              (0x02)
#define AM_AS7262_LED_IND_8_MA              (0x03)

/**
 * \brief 传感器 AS7262 LED_IND使能设置
 */
#define AM_AS7262_LED_IND_DISABLE           (0x00)
#define AM_AS7262_LED_IND_ENABLE            (0x01)

/**
 * \brief 传感器 AS7262 控制指令定义
 */
typedef enum am_sensor_as7262_control_cmd {
    AM_SENSOR_AS7262_ITIME_SET = 1,         /* ATINTTIME=<value>    */
    AM_SENSOR_AS7262_ITIME_GET,             /* ATINTTIME            */
    AM_SENSOR_AS7262_GAIN_SET,              /* ATGAIN=<value>       */
    AM_SENSOR_AS7262_GAIN_GET,              /* ATGAIN               */
    AM_SENSOR_AS7262_DEVICE_TEMP_GET,       /* ATTEMP               */
    AM_SENSOR_AS7262_BANK_SET,              /* ATTCSMD=<value>      */
    AM_SENSOR_AS7262_BANK_GET,              /* ATTCSMD              */
    AM_SENSOR_AS7262_LED_IND_SET,           /* ATLED0=<value>       */
    AM_SENSOR_AS7262_LED_IND_GET,           /* ATLED0               */
    AM_SENSOR_AS7262_LED_DRV_SET,           /* ATLED1=<value>       */
    AM_SENSOR_AS7262_LED_DRV_GET,           /* ATLED1               */
    AM_SENSOR_AS7262_ICL_IND_SET,           /* ATLEDC=<value>       */
    AM_SENSOR_AS7262_ICL_DRV_SET,           /* ATLEDC=<value>       */
    AM_SENSOR_AS7262_ICL_IND_GET,           /* ATLEDC               */
    AM_SENSOR_AS7262_ICL_DRV_GET,           /* ATLEDC               */
    AM_SENSOR_AS7262_SW_RST,                /* ATRST                */
    AM_SENSOR_AS7262_HW_VERSION_GET,        /* ATVERHW              */

    /* 以下指令仅在UART模式下有效 */
    AM_SENSOR_AS7262_ATDATA,                /* ATDATA               */
    AM_SENSOR_AS7262_ATCDATA,               /* ATCDATA              */
    AM_SENSOR_AS7262_ATBURST,               /* ATBURST=<value>      */
    AM_SENSOR_AS7262_AT,                    /* AT                   */
    AM_SENSOR_AS7262_ATVERSW,               /* ATVERSW              */
    AM_SENSOR_AS7262_ATFWU,                 /* ATFWU                */
    AM_SENSOR_AS7262_ATFW,                  /* ATFW                 */
    AM_SENSOR_AS7262_ATFWA,                 /* ATFWA                */
    AM_SENSOR_AS7262_ATFWS,                 /* ATFWS                */
    AM_SENSOR_AS7262_NREAD,
    AM_SENSOR_AS7262_NWRITE,
    AM_SENSOR_AS7262_FLUSH,
    AM_SENSOR_AS7262_WFLUSH,
    AM_SENSOR_AS7262_RFLUSH,
    AM_SENSOR_AS7262_TIMEOUT,

    /* 以下指令仅在I2C模式下有效 */
    AM_SENSOR_AS7262_DEVICE_TYPE_GET,
    AM_SENSOR_AS7262_INT_SET,
    AM_SENSOR_AS7262_DATA_RDY_GET,
} am_sensor_as7262_control_cmd_t;

/*
 * \brief 传感器AS7262提供的所有通道ID定义
 */
#define AM_AS7262_CHAN_1      0             /* AS7262 的通道1，V */
#define AM_AS7262_CHAN_2      1             /* AS7262 的通道2，B */
#define AM_AS7262_CHAN_3      2             /* AS7262 的通道3，G */
#define AM_AS7262_CHAN_4      3             /* AS7262 的通道4，Y */
#define AM_AS7262_CHAN_5      4             /* AS7262 的通道5，O */
#define AM_AS7262_CHAN_6      5             /* AS7262 的通道6，R */

/**
 * \brief AS7262参数配置初始化结构体定义 
 */
typedef struct as7262_param_config
{
    uint8_t     gain_val;           /**< \brief GAIN    */
    uint8_t     bank_mode;          /**< \brief BANK    */
    uint8_t     itime_val;          /**< \brief ITIME   */
    uint8_t     icl_drv;            /**< \brief ICL_DRV */
    uint8_t     led_drv;            /**< \brief LED_DRV */
    uint8_t     icl_ind;            /**< \brief ICL_IND */
    uint8_t     led_ind;            /**< \brief LED_IND */
} as7262_param_config_t;

/**
 * \brief AS7262 校准值保存 (此处无作用)
 */
typedef struct am_as7262_calibration_data {
    float       cal[6];
} am_as7262_calibration_data_t;

/**
 * \brief AS7262 结果结构体定义
 */
typedef struct as7262_result_info
{
    uint16_t    raw_data[6];        /* 原始数据 */
    float       cal_data[6];        /* 校准数据 */
    uint32_t    channel_v;          /* 校准后的数据值 */
    uint32_t    channel_b;
    uint32_t    channel_g;
    uint32_t    channel_y;
    uint32_t    channel_o;
    uint32_t    channel_r;
    int8_t      device_temp;        /* 温度值 */
} as7262_result_info_t;

/**
 * \brief 传感器 AS7262 设备信息结构体
 */
typedef struct am_sensor_as7262_devinfo {

    const   as7262_param_config_t       *p_param_default;   /**< \brief 参数初始化 */

    int                                  trigger_pin;       /**< \brief INT引脚 */

    int                                  rst_pin;           /**< \brief RST引脚 */

} am_sensor_as7262_devinfo_t;

/**
 * \breif 传感器 AS7262 设备结构体定义
 */
typedef struct am_sensor_as7262_dev {
    am_sensor_serv_t                    as7262_dev;     /*< \breif 传感器标准服务 */
    am_as7262_calibration_data_t        cal_val[1];     /*< \brief 保存Prox校准值 */
    void                                *p_arg[6];      /*< \brief 触发回调函数参数 */
    uint8_t                             flags[6];       /*< \brief 触发标志位 */
    am_isr_defer_job_t                  g_myjob;        /*< \brief 中断延时处理任务 */
    uint16_t                            trigger;        /*< \brief 记录标志位 */
    am_sensor_val_t                     data[6];        /*< \brief 内部数据缓存 */

    /** \brief 数据准备就绪触发回调函数 */
    am_sensor_trigger_cb_t              pfn_trigger_cb[6];

    /** \brief 传感器控制函数 */
    am_err_t (*pfn_ioctl)(void *p_drv, int cmd, void *p_arg);

    /** \brief 设备信息 */
    const am_sensor_as7262_devinfo_t    *dev_info;

} am_sensor_as7262_dev_t;

/**
 * \brief 传感器 AS7262 控制函数
 *
 * \param[in] handle    : am_sensor_as7262_init()初始化函数获得的传感器标准服务句柄
 * \param[in] cmd       : 控制指令
 *
 * \param[in,out] p_arg : 该指令对应的参数
 *
 * \retval AM_OK       : 控制指令执行成功
 * \retval -AM_ENOTSUP : 指令不支持
 */
am_err_t am_sensor_as7262_ioctl (am_sensor_handle_t  handle,
                                 int                 cmd,
                                 void               *p_arg);

#ifdef __cplusplus
}
#endif

#endif /* __AM_SENSOR_AS7262_H */

/* end of file */

