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
 * \brief  PCF85063 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.10 17-08-16  vir, optimize structure
 * - 1.00 16-08-05  sky, first implementation.
 * \endinternal
 */

#ifndef __AM_PCF85063_H
#define __AM_PCF85063_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_pcf85063
 * \copydoc am_pcf85063.h
 * @{
 */
 
#include "ametal.h"
#include "am_time.h"
#include "am_rtc.h"
#include "am_alarm_clk.h"
#include "am_i2c.h"

/**
 * \brief PCF85063 设备信息结构体
 *
 * \note 主要是配置引脚号
 */
typedef struct am_pcf85063_devinfo {

    /** 
     * \brief PCF85063的中断INT引脚所连接到的GPIO引脚编号，
     *        不需要时，赋值为 -1 
     */
    int int_pin;

    /**
     * \brief PCF85063的CLKOE(CLK_EN)引脚所连接到的GPIO引脚编号
     *  用于控制CLKOUT是否输出频率，不需要时，赋值为 -1
     */
    int clk_en_pin;

} am_pcf85063_devinfo_t;

/**
 * \brief PCF85063 设备结构体
 */
typedef struct am_pcf85063_dev {

    am_bool_t         conn_stat;      /**< \brief 当前INT中断连接状态 */
    
    /** \brief 用于存放标准闹钟接口闹钟时间wdays的值 */
    uint8_t           alarm_clk_wdays;
    
     /** \brief 用于区分用户调用的是标准闹钟接口还是非标准的 */
    uint8_t           alarm_mode;

    uint8_t           status;        /**< \brief 当前状态 */
    uint8_t           intr_flag;     /**< \brief 中断标志 */
    uint8_t           sub_addr[2];   /**< \brief I2C器件子地址 */
    uint8_t           buf[1];        /**< \brief I2C数据缓冲区 */
    uint8_t           nbytes;        /**< \brief 操作数据长度 */
    am_i2c_transfer_t trans[2];      /**< \brief PCF85063状态机transfer */
    am_i2c_message_t  msg;           /**< \brief PCF85063状态机msg */

    am_i2c_device_t  i2c_dev;        /**< \brief PCF85063 I2C设备 */

    /** \brief INT引脚的触发信息类型 */
    struct am_pcf85063_trigger_info {

        /** \brief 触发回调函数 */
        am_pfnvoid_t  pfn_callback;

        /** \brief 回调函数的参数 */
        void *p_arg;

    } triginfo[2];                  /**< \brief INT引脚的触发信息 */

    /** \brief 指向设备信息常量的指针 */
    const am_pcf85063_devinfo_t * p_devinfo;

} am_pcf85063_dev_t;


/** \brief PCF85063操作句柄定义 */
typedef am_pcf85063_dev_t *am_pcf85063_handle_t;

/**
 * \brief PCF85063 设备初始化
 *
 * \param[in] p_dev      : 指向PCF85063设备结构体的指针
 * \param[in] p_devinfo  : 指向PCF85063设备信息的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return PCF85063服务操作句柄,如果为 NULL，表明初始化失败
 *
 * \note 若不需要用频率输出功能，设备信息结构体中的clk_en_pin 赋值为 -1 即可，
 */
am_pcf85063_handle_t am_pcf85063_init (am_pcf85063_dev_t           *p_dev,
                                       const am_pcf85063_devinfo_t *p_devinfo,
                                       am_i2c_handle_t              i2c_handle);

/**
 * \brief PCF85063 设备解初始化
 * \param[in] handle : PCF85063操作句柄
 * \return 无
 */
void am_pcf85063_deinit (am_pcf85063_handle_t handle);


/**
 * \brief 获取RTC标准服务句柄
 *
 * \param[in] handle     : PCF85063操作句柄
 * \param[in] p_rtc      : 指向RTC标准服务
 *
 * return RTC标准服务操作句柄
 */
am_rtc_handle_t am_pcf85063_rtc_init (am_pcf85063_handle_t  handle,
                                      am_rtc_serv_t        *p_rtc);
/**
 * \brief 获取PCF85063 ALARM_CLK标准服务
 *
 * \param[in] handle       : PCF85063操作句柄
 * \param[in] p_alarm_clk  : 指向ALARM_CLK标准服务
 *
 * return ALARM_CLK标准服务操作句柄
 */
am_alarm_clk_handle_t am_pcf85063_alarm_clk_init (am_pcf85063_handle_t   handle,
                                                  am_alarm_clk_serv_t    *p_alarm_clk);

/**
 * \brief 软件复位PCF85063
 *
 * \param[in] handle : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_software_reset (am_pcf85063_handle_t handle);

/**
 * \brief 中止PCF85063
 *
 * \param[in] handle : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_stop (am_pcf85063_handle_t handle);

/**
 * \brief 启动PCF85063(复位时PCF85063已启动，该接口主要是针对 am_pcf85063_stop())
 *
 * \param[in] handle : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_start (am_pcf85063_handle_t handle);

/**
 * \brief 写 PCF85063 RAM_Byte 寄存器
 *
 * \param[in] handle   : PCF85063服务操作句柄
 * \param[in] data     : 待写入 PCF85063 RAM_Byte 寄存器的值
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_ram_write (am_pcf85063_handle_t handle, uint8_t data);

/**
 * \brief 读 PCF85063 RAM_Byte 寄存器
 *
 * \param[in] handle      : PCF85063服务操作句柄
 * \param[out] p_data     : 读取PCF85063 RAM_Byte 寄存器的缓冲区
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 只能读出一个字节数据来
 */
int am_pcf85063_ram_read (am_pcf85063_handle_t handle, uint8_t *p_data);

/**
 * \brief PCF85063时间日期设置函数
 *
 * \param[in] handle : PCF85063服务操作句柄
 * \param[in] p_tm   : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_pcf85063_time_set (am_pcf85063_handle_t handle, am_tm_t *p_tm);

/**
 * \brief PCF85063时间日期获取函数
 *
 * \param[in]  handle : PCF85063服务操作句柄
 * \param[out] p_tm   : 指向时间结构体的指针
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_pcf85063_time_get (am_pcf85063_handle_t handle, am_tm_t *p_tm);

/**
 * \name PCF85063 偏移补偿定义
 * \anchor grp_pcf85063_offset
 * @{
 */

/** \brief 正常模式（每两小时补偿一次） */
#define AM_PCF85063_OFFSET_MODE_NORMAL      0

/** \brief 快速模式（每四分钟补偿一次） */
#define AM_PCF85063_OFFSET_MODE_COURSE      1

/**
 * @}
 */

/**
 * \brief PCF85063时钟源补偿设置
 *
 * \param[in] handle : PCF85063服务操作句柄
 * \param[in] mode   : 补偿模式 #AM_PCF85063_OFFSET_MODE_NORMAL 见 grp_pcf85063_offset
 * \param[in] ppm    : 时钟源的ppm(如晶体振荡器手册里面标注的ppm)
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_pcf85063_offset_set (am_pcf85063_handle_t handle, uint8_t mode, float ppm);

/**
 * \name PCF85063 闹钟使能宏
 * \anchor grp_pcf85063_alarm_enable
 * @{
 */

#define AM_PCF85063_ALARM_SECOND_ENABLE  0x01   /**< \brief 秒钟闹钟使能 */
#define AM_PCF85063_ALARM_MINUTE_ENABLE  0x02   /**< \brief 分钟闹钟使能 */
#define AM_PCF85063_ALARM_HOUR_ENABLE    0x04   /**< \brief 小时闹钟使能 */
#define AM_PCF85063_ALARM_DAY_ENABLE     0x08   /**< \brief 日闹钟使能 */
#define AM_PCF85063_ALARM_WEEKDAY_ENABLE 0x10   /**< \brief 周几闹钟使能 */

/**
 * @}
 */

/**
 * \brief 闹钟信息结构体
 *
 * \note enabled由闹钟使能宏构成 参见 \ref grp_pcf85063_alarm_enable
 *       可以取它们的或值来构建，如使能时分闹钟，则
 *       enable = AM_PCF85063_ALARM_MINUTE_ENABLE | AM_PCF85063_ALARM_HOUR_ENABLE，
 *       这意味着当前时间同时吻合闹钟设置的时分信息时，闹钟标记才会置位。即enable里面所有标志
 *       对应的时间都与当前时间吻合时，闹钟才会响应。
 */
typedef struct am_pcf85063_alarm_info {
    uint8_t     enable;     /**< \brief 闹钟使能 */
    int         sec;        /**< \brief seconds after the minute     - [0, 59]  */
    int         min;        /**< \brief minutes after the hour       - [0, 59]  */
    int         hour;       /**< \brief hours after midnight         - [0, 23]  */
    int         mday;       /**< \brief day of the month             - [1, 31]  */
    int         wday;       /**< \brief days since Sunday            - [0, 6]   */
} am_pcf85063_alarm_info_t;

/**
 * \brief 使能PCF85063闹钟
 *
 * \param[in] handle       : PCF85063服务操作句柄
 * \param[in] p_alarm_info : 闹钟信息设置
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_alarm_enable (am_pcf85063_handle_t      handle,
                              am_pcf85063_alarm_info_t *p_alarm_info);

/**
 * \brief 禁能PCF85063闹钟
 *
 * \param[in] handle : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_alarm_disable (am_pcf85063_handle_t handle);

/**
 * \name PCF85063 CLKOUT输出频率编码
 * \anchor grp_pcf85063_clkout_freq
 * @{
 */

#define AM_PCF85063_CLKOUT_FREQ_32768HZ    0x00 /**< \brief 输出频率32768Hz */
#define AM_PCF85063_CLKOUT_FREQ_16384HZ    0x01 /**< \brief 输出频率16384Hz */
#define AM_PCF85063_CLKOUT_FREQ_8192HZ     0x02 /**< \brief 输出频率8192Hz */
#define AM_PCF85063_CLKOUT_FREQ_4096HZ     0x03 /**< \brief 输出频率4096Hz */
#define AM_PCF85063_CLKOUT_FREQ_2048HZ     0x04 /**< \brief 输出频率2048Hz */
#define AM_PCF85063_CLKOUT_FREQ_1024HZ     0x05 /**< \brief 输出频率1024Hz */
#define AM_PCF85063_CLKOUT_FREQ_1HZ        0x06 /**< \brief 输出频率1Hz */
#define AM_PCA85063A_CLKOUT_FREQ_DISABLE   0x07 /**< \brief 禁能输出频率 */

/**
 * @}
 */

/**
 * \brief 设置PCF85063时钟输出
 *
 * \param[in] handle : PCF85063服务操作句柄
 * \param[in] freq   : 时钟输出频率编码 参见 \ref grp_pcf85063_clkout_freq
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_clkout_set (am_pcf85063_handle_t handle, uint8_t freq);

/**
 * \brief  关闭 clk的输出
 * \param[in] handle     : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_clkout_close(am_pcf85063_handle_t handle);

/**
 * \name PCF85063 定时器时钟源宏定义
 * \anchor grp_pcf85063_timer_src
 * @{
 */

#define AM_PCF85063_TIEMR_SRC_FREQ_4096HZ      (0x00)    /**< \brief 4096Hz */
#define AM_PCF85063_TIEMR_SRC_FREQ_64HZ        (0x08)    /**< \brief 64Hz */
#define AM_PCF85063_TIEMR_SRC_FREQ_1HZ         (0x10)    /**< \brief 1Hz */
#define AM_PCF85063_TIEMR_SRC_FREQ_1_DIV_60HZ  (0x18)    /**< \brief 1/60Hz */

/**
 * @}
 */

/**
 * \brief 使能PCF85063定时器
 *
 * \param[in] handle          : PCF85063服务操作句柄
 * \param[in] clk_freq        : 时钟频率选择 参见 \ref grp_pcf85063_timer_src
 * \param[in] max_timer_count : 最大计数值
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 精度及步进如下:
 *       4096Hz [244us    ~ 62.256ms]
 *       64Hz   [15.625ms ~ 3.984s]
 *       1Hz    [1s       ~ 255s]
 *       1/60Hz [1min     ~ 255min]
 */
int am_pcf85063_timer_enable (am_pcf85063_handle_t handle,
                              uint8_t              clk_freq,
                              uint8_t              max_timer_count);

/**
 * \brief 禁能PCF85063定时器
 *
 * \param[in] handle : PCF85063服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_timer_disable (am_pcf85063_handle_t handle);

/**
 * \brief 获取定时器当前的计数值
 *
 * \param[in]  handle  : PCF85063服务操作句柄
 * \param[out] p_count : 用于获取定时器当前计数值的指针
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_pcf85063_timer_count_get (am_pcf85063_handle_t  handle,
                                 uint8_t              *p_count);

/**
 * \name PCF85063 分钟/定时器中断类型信息
 * \anchor grp_pcf85063_int_type_info
 * @{
 */

#define AM_PCF85063_INT_TIMER       0x1   /**< \brief 定时器中断TI */
#define AM_PCF85063_INT_HMINUTE     0x2   /**< \brief 半分钟中断HMI */
#define AM_PCF85063_INT_MINUTE      0x4   /**< \brief 一分钟中断MI */

/**
 * @}
 */

/**
 * \brief 使能PCF85063分钟/定时器中断
 *
 * \param[in] handle : PCF85063服务操作句柄
 * \param[in] flags  : 中断标志，可以只传一个参数，也可以通过取或的形式传入多个参数，
 *                     如 AM_PCF85063_INT_TIMER | AM_PCF85063_INT_HMINUTE；
 *                     中断标志参见 \ref grp_pcf85063_int_type_info
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 半分钟中断、分钟中断以及定时器中断共用TF标记，没法区分中断源，要注意同时使用的情况
 */
int am_pcf85063_minute_timer_int_enable (am_pcf85063_handle_t handle,
                                         uint8_t              flags);

/**
 * \brief 禁能PCF85063分钟/定时器中断
 *
 * \param[in] handle : PCF85063服务操作句柄
 * \param[in] flags  : 中断标志，可以只传一个参数，也可以通过取或的形式传入多个参数，
 *                     如 AM_PCF85063_INT_TIMER | AM_PCF85063_INT_HMINUTE；
 *                     中断标志参见 \ref grp_pcf85063_int_type_info
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 半分钟中断、分钟中断以及定时器中断共用TF标记，没法区分中断源，要注意同时使用的情况
 */
int am_pcf85063_minute_timer_int_disable (am_pcf85063_handle_t handle,
                                          uint8_t              flags);

/**
 * \brief PCF85063 分钟/定时器回调函数设置
 *
 * \param[in] handle                    : PCF85063服务操作句柄
 * \param[in] pfn_minute_timer_callback : 分钟/定时器中断回调函数
 * \param[in] p_minute_timer_arg        : 分钟/定时器中断回调函数参数
 *
 * \return AM_OK-成功,其他失败
 *
 * \note 定时器中断、半分钟中断以及一分钟中断都触发TF标记，没法区分中断源，
 *       因此将三个中断的回调函数合并为一个。
 */
int am_pcf85063_minute_timer_cb_set (am_pcf85063_handle_t  handle,
                                     am_pfnvoid_t          pfn_minute_timer_callback,
                                     void                 *p_minute_timer_arg);

/**
 * \brief PCF85063 闹钟回调函数设置
 *
 * \param[in] handle                    : PCF85063服务操作句柄
 * \param[in] pfn_alarm_callback        : 闹钟中断回调函数
 * \param[in] p_alarm_arg               : 闹钟中断回调函数参数
 *
 * \return AM_OK-成功,其他失败
 */
int am_pcf85063_alarm_cb_set (am_pcf85063_handle_t  handle,
                              am_pfnvoid_t          pfn_alarm_callback,
                              void                 *p_alarm_arg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_PCF85063_H */

/* end of file */
