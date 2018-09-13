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
 * \brief  RX8025SA 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-07  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_RX8025SA_H
#define __AM_RX8025SA_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_rx8025sa
 * @copydoc am_rx8025sa.h
 * @{
 */
 
#include "ametal.h"
#include "am_time.h"
#include "am_rtc.h"
#include "am_alarm_clk.h"
#include "am_i2c.h"

/**
 * \brief RX8025SA 设备信息结构体
 *
 * \note 主要是配置引脚号
 */
typedef struct am_rx8025sa_devinfo {

    /** \brief INTA引脚连接到的GPIO引脚编号 */
    uint8_t inta_pin;

    /** \brief INTA引脚连接到的GPIO引脚编号 */
    uint8_t intb_pin;
    /**
     * \brief RX8025SA的FOE(CLK_EN)引脚所连接到的GPIO引脚编号
     *  用于控制FOUT是否输出频率,不需要时，赋值为 -1
     */
    int clk_en_pin;

} am_rx8025sa_devinfo_t;

/**
 * \brief RX8025SA 设备结构体
 */
typedef struct am_rx8025sa_dev {

    am_bool_t         conn_stat_inta;  /**< \brief 当前INTA中断连接状态 */
    uint8_t           time_format;            /** \brief 小时制 */

    /* 以下变量主要用于在中断中读写芯片的寄存器 */
    uint8_t           status;     /**< \brief 当前状态 */
    uint8_t           intr_flag;  /**< \brief 中断标志 */
    uint8_t           sub_addr;   /**< \brief I2C器件子地址 */
    uint8_t           buf[1];     /**< \brief I2C数据缓冲区 */
    uint8_t           nbytes;     /**< \brief 操作数据长度 */
    am_i2c_transfer_t trans[2];   /**< \brief RX8025SA状态机transfer */
    am_i2c_message_t  msg;        /**< \brief RX8025SA状态机msg */

    am_i2c_device_t  i2c_dev;     /**< \brief RX8025SA I2C设备 */

    /* INT引脚的触发信息 */
    struct am_rx8025sa_trigger_info {

        /** \brief 触发回调函数 */
        am_pfnvoid_t  pfn_callback;

        /** \brief 回调函数的参数 */
        void *p_arg;

    } triginfo[3];

    const am_rx8025sa_devinfo_t * p_devinfo;  /* 指向设备信息常量的指针 */

} am_rx8025sa_dev_t;

/** \brief RX8025SA操作句柄定义 */
typedef am_rx8025sa_dev_t *am_rx8025sa_handle_t;

/**
 * \brief RX8025SA 设备初始化
 *
 * \param[in] p_dev      : 指向RX8025SA设备结构体的指针
 * \param[in] p_devinfo  : 指向RX8025SA设备信息的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return RX8025SA服务操作句柄,如果为 NULL，表明初始化失败
 *
 * \note 若不需要用频率输出功能，设备信息结构体中的clk_en_pin 赋值为 -1 即可
 */
am_rx8025sa_handle_t am_rx8025sa_init (am_rx8025sa_dev_t           *p_dev,
                                       const am_rx8025sa_devinfo_t *p_devinfo,
                                       am_i2c_handle_t              i2c_handle);

/**
 * \brief RX8025SA 设备解初始化
 * \param[in] handle : RX8025SA操作句柄
 * \return 无
 */
void am_rx8025sa_deinit (am_rx8025sa_handle_t handle);

/**
 * \brief 获取RTC标准服务句柄
 *
 * \param[in] handle     : RX8025SA操作句柄
 * \param[in] p_rtc      : 指向RTC标准服务
 *
 * return RTC标准服务操作句柄
 */
am_rtc_handle_t am_rx8025sa_rtc_init (am_rx8025sa_handle_t  handle,
                                      am_rtc_serv_t        *p_rtc);

/**
 * \brief 获取RX8025SA ALARM_CLK标准服务
 *
 * \param[in] handle       : RX8025SA操作句柄
 * \param[in] p_alarm_clk  : 指向ALARM_CLK标准服务
 *
 * return ALARM_CLK标准服务操作句柄
 */
am_alarm_clk_handle_t am_rx8025sa_alarm_clk_init (am_rx8025sa_handle_t   handle,
                                                  am_alarm_clk_serv_t   *p_alarm_clk);

/**
 * \name rx8025设置星期闹钟的星期值
 * \anchor grp_rx8025sa_alarm_week
 *  eg： 如果要设置星期三和星期星期天 的闹钟   则alarm_week的值为 ：
 *        AM_RX8025SA_ALARM_SUNDAY | AM_RX8025SA_ALARM_WEDNESDAY
 */
#define AM_RX8025SA_ALARM_SUNDAY    0X01
#define AM_RX8025SA_ALARM_MONDAY    0X02
#define AM_RX8025SA_ALARM_TUESDAY   0X04
#define AM_RX8025SA_ALARM_WEDNESDAY 0X08
#define AM_RX8025SA_ALARM_THURSDAY  0X10
#define AM_RX8025SA_ALARM_FRIDAY    0X20
#define AM_RX8025SA_ALARM_SATURDAY  0X40

/**
 * \brief 闹钟信息结构体
 */
typedef struct am_rx8025sa_alarm_info {
    int min;     /**< \brief minutes after the hour       - [0, 59]  */
    int hour;    /**< \brief hours after midnight         - [1, 12] / [0, 23]   */
    int alarm_week;    /**< \brief 值参考grp_rx8025sa_alarm_week  alarm_d闹钟可以忽略*/
}am_rx8025sa_alarm_info_t;

/**
 * \brief 使能RX8025SA的 ALARM_D闹钟
 *
 * \param[in] handle       : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_d_enable (am_rx8025sa_handle_t handle);
/**
 * \brief 禁能RX8025SA 的 ALARM_D 闹钟
 *
 * \param[in] handle : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_d_disable (am_rx8025sa_handle_t handle);


/**
 * \brief 使能RX8025SA的 ALARM_W闹钟
 *
 * \param[in] handle       : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_w_enable (am_rx8025sa_handle_t handle);
/**
 * \brief 禁能RX8025SA 的 ALARM_W 闹钟
 *
 * \param[in] handle : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_w_disable (am_rx8025sa_handle_t handle);

/**
 * \name RX8025SA  周期中断的一些模式
 * \anchor grp_int_mode
 * note：  每分00秒
 *       每时00分00秒
 *       每月1号00时00分00秒                        拉低INTA 引脚
 */

#define AM_RX8025SA_INTA_OFF                 0X00  /**< \brief INTA引脚呈现高电平或高阻 */
#define AM_RX8025SA_INTA_LOW                 0X01  /**< \brief INTA引脚输出低电平 */
#define AM_RX8025SA_PULSE_2HZ_MODE           0X02  /**< \brief 2HZ的脉冲模式 */
#define AM_RX8025SA_PULSE_1HZ_MODE           0X03  /**< \brief 1HZ的脉冲模式 */
#define AM_RX8025SA_LEVEL_PER_SECOND_MODE    0X04  /**< \brief 每秒拉低INTA引脚的电平模式 */
#define AM_RX8025SA_LEVEL_PER_MINUTE_MODE    0X05  /**< \brief 每分拉低INTA引脚的电平模式 */
#define AM_RX8025SA_LEVEL_PER_HOUR_MODE      0X06  /**< \brief 每时拉低INTA引脚的电平模式 */
#define AM_RX8025SA_LEVEL_PER_MONTH_MODE     0X07  /**< \brief 每月拉低INTA引脚的电平模式 */


/**
 * \brief RX8025SA 周期中断函数设置
 *
 * \param[in] handle   : rx8025sa服务操作句柄
 * \param[in] int_mode :周期中断模式请参考grp_int_mode
 *
 * \param[in] pfn_periodic_int_callback   :周期中断的回调函数
 * \param[in] p_periodic_int_arg          :周期中断的回调函数的参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_periodic_int_set (am_rx8025sa_handle_t  handle,
                                   uint8_t              int_mode,
                                  am_pfnvoid_t          pfn_periodic_int_callback,
                                  void                 *p_periodic_int_arg);


/**
 * \brief RX8025SA 闹钟D回调函数和闹钟时间设置
 *
 * \param[in] handle       : rx8025sa服务操作句柄
 * \param[in] p_alarm_info : 闹钟时间信息
 *
 * \param[in] pfn_alarm_callback   :闹钟中断的回调函数
 * \param[in] p_alarm_arg          :闹钟中断的回调函数的参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_d_set(am_rx8025sa_handle_t      handle,
                            am_rx8025sa_alarm_info_t *p_alarm_info,
                            am_pfnvoid_t              pfn_alarm_callback,
                            void                     *p_alarm_arg);
/**
 * \brief RX8025SA 闹钟W回调函数和闹钟时间设置
 *
 * \param[in] handle       : rx8025sa服务操作句柄
 * \param[in] p_alarm_info : 闹钟时间信息
 *
 * \param[in] pfn_alarm_callback   :闹钟中断的回调函数
 * \param[in] p_alarm_arg          :闹钟中断的回调函数的参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_alarm_w_set(am_rx8025sa_handle_t      handle,
                            am_rx8025sa_alarm_info_t *p_alarm_info,
                            am_pfnvoid_t              pfn_alarm_callback,
                            void                     *p_alarm_arg);

/*
 * \brief 电源电压震荡检测
 * \brief\param[in] handle       : rx8025sa服务操作句柄
 *
 * \note     返回值说明：
 *   return 0  电源电压不降低，但振动停止
 *          1  电源电压降低，且振动停止
 *          2  正常
 *          3  虽然电源降低，但振动仍继续
 *          4,5 电源下降到0v
 *          6,7 电源瞬停的可疑性大
 *
 */
int am_rx8025sa_power_check(am_rx8025sa_handle_t  handle);

/**
 * \name 检测低电压的阈值设定
 * \anchor grp_rx8025sa_check_voltage
 */
#define RX8025SA_CHECK_VOLTAGE_1_3V  0   /** \brief 电源 降低检测功能的标准电压值设定为1.3v */
#define RX8025SA_CHECK_VOLTAGE_2_1V  1   /** \brief 电源 降低检测功能的标准电压值设定为2.1v */

/**
 * \brief 低电压检测功能的标准电压值设定
 *
 * \param[in] handle        : RX8025SA服务操作句柄
 * \param[in] check_v_value : 低电压阈值 ，参考 grp_rx8025sa_check_voltage
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_check_voltage_set (am_rx8025sa_handle_t handle, uint8_t check_v_value);

/*
 * \brief 计时频率进行调快或调慢
 * \param[in] handle   : RX8025SA服务操作句柄
 * \param[in] clk      : 要调整到的频率大小  ，调整的范围 ：-189.1*10^-6 <= (clk-32768)/32768 <= +189.1*10^-6
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note clk 的单位为HZ，对FOUT输出的频率没有影响。
 */
int am_rx8025sa_clk_adjust ( am_rx8025sa_handle_t handle, float clk );

/*
 * \brief  设置 clk的输出 ，FOUT将输出32.768KHZ频率
 * \param[in] handle     : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 若要使用芯片输出的频率就必须配置设备信息中clk_en_pin。
 */
int am_rx8025sa_clkout_set(am_rx8025sa_handle_t handle);

/*
 * \brief  关闭 clk的输出
 * \param[in] handle     : RX8025SA服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025sa_clkout_close(am_rx8025sa_handle_t handle);
                              
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* _AM_RX8025SA_H */

/* end of file */

