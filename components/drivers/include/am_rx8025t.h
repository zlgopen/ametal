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
 * \brief  RX8025T 应用接口文件
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-07  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_RX8025T_H
#define __AM_RX8025T_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_rx8025t
 * @copydoc am_rx8025t.h
 * @{
 */

#include "ametal.h"
#include "am_time.h"
#include "am_rtc.h"
#include "am_alarm_clk.h"
#include "am_i2c.h"
#include "am_softimer.h"

/**
 * \brief RX8025T 设备信息结构体
 *
 * \note 主要是配置引脚号
 */
typedef struct am_rx8025t_devinfo {

    /**
     * \brief RX8025T的中断INT引脚所连接到的GPIO引脚编号，
     *        不需要时，赋值为 -1
     */
    int int_pin;

    /**
     * \brief RX8025T的FOE(CLK_EN)引脚所连接到的GPIO引脚编号
     *  用于控制FOUT是否输出频率,不需要时，赋值为 -1
     */
    int clk_en_pin;

} am_rx8025t_devinfo_t;

/**
 * \brief RX8025T 设备结构体
 */
typedef struct am_rx8025t_dev {

    am_bool_t         conn_stat;  /**< \brief 当前INT中断连接状态 */

    /* 以下变量主要用于在中断中读写芯片的寄存器 */
    uint8_t           status;     /**< \brief 当前状态 */
    uint8_t           intr_flag;  /**< \brief 中断标志 */
    uint8_t           sub_addr;   /**< \brief I2C器件子地址 */
    uint8_t           buf[1];     /**< \brief I2C数据缓冲区 */
    uint8_t           nbytes;     /**< \brief 操作数据长度 */
    am_i2c_transfer_t trans[2];   /**< \brief RX8025T状态机transfer */
    am_i2c_message_t  msg;        /**< \brief RX8025T状态机msg */

    am_i2c_device_t  i2c_dev;     /**< \brief RX8025T I2C设备 */

    am_softimer_t timer;          /**< \brief 用于中断延时处理的软件定时器 */

    /**< \brief 即将发生的读取中断状态的次数。（中断来临后会连续读取三次中断状态，该值为0时表示
     * 下一次读取中断状态是由于检测到中断发生；该值为1时表示下一次读取中断状态是因为可能在第一次读取状态
     * 之后，清除状态之前，会产生另一个中断；该值为2时表示下一次读取中断状态是因为可能在第二次读取状态
     * 之后，清除状态之前，会产生另一个中断。第一次和第二次读取中断的时间间隔较短（约0.5ms），确保了中
     * 断的实时性，第二次和第三次读取的间隔时间较长（约2ms）确保产生的中断不会错过） */
    uint8_t read_int_time_status;

    /** \brief INT引脚的触发信息类型 */
    struct am_rx8025t_trigger_info {

        /** \brief 触发回调函数 */
        am_pfnvoid_t  pfn_callback;

        /** \brief 回调函数的参数 */
        void *p_arg;

    } triginfo[3];                         /**< \brief INT引脚的触发信息类型 */

    const am_rx8025t_devinfo_t *p_devinfo; /**< \brief 指向设备信息常量的指针 */

} am_rx8025t_dev_t;

/** \brief RX8025T操作句柄定义 */
typedef am_rx8025t_dev_t *am_rx8025t_handle_t;

/**
 * \brief RX8025T 设备初始化
 *
 * \param[in] p_dev      : 指向RX8025T设备结构体的指针
 * \param[in] p_devinfo  : 指向RX8025T设备信息的指针
 * \param[in] i2c_handle : I2C标准服务操作句柄
 *
 * \return RX8025T服务操作句柄,如果为 NULL，表明初始化失败
 *
 * \note 若不需要用频率输出功能，设备信息结构体中的clk_en_pin 赋值为 -1 即可
 */
am_rx8025t_handle_t am_rx8025t_init (am_rx8025t_dev_t           *p_dev,
                                     const am_rx8025t_devinfo_t *p_devinfo,
                                     am_i2c_handle_t             i2c_handle);

/**
 * \brief RX8025T 设备解初始化
 * \param[in] handle : RX8025T操作句柄
 * \return 无
 */
void am_rx8025t_deinit (am_rx8025t_handle_t handle);

/**
 * \brief 获取RTC标准服务句柄
 *
 * \param[in] handle     : RX8025T操作句柄
 * \param[in] p_rtc      : 指向RTC标准服务
 *
 * return RTC标准服务操作句柄
 */
am_rtc_handle_t am_rx8025t_rtc_init (am_rx8025t_handle_t  handle,
                                     am_rtc_serv_t       *p_rtc);

/**
 * \brief 获取RX8025T 闹钟ALARM_CLK标准服务
 *
 * \param[in] handle           : RX8025T操作句柄
 * \param[in] p_alarm_clk      : 指向ALARM_CLK标准服务
 *
 * return ALARM_CLK标准服务操作句柄
 */
am_alarm_clk_handle_t am_rx8025t_alarm_clk_init (am_rx8025t_handle_t   handle,
                                                 am_alarm_clk_serv_t  *p_alarm_clk);

/**
 * \brief 获取RX8025T ALARM_CLK标准服务
 *
 * \param[in] handle           : RX8025T操作句柄
 * \param[in] p_alarm_clk      : 指向ALARM_CLK标准服务
 *
 * return ALARM_CLK标准服务操作句柄
 */
am_alarm_clk_handle_t am_rx8025t_alarm_clk_init (am_rx8025t_handle_t   handle,
                                                 am_alarm_clk_serv_t  *p_alarm_clk);

/**
 * \brief 软件复位RX8025T
 *
 * \param[in] handle           : RX8025T操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_software_reset (am_rx8025t_handle_t handle);

/**
 * \name 中断标志
 * \anchor grp_int_select
 * @{
 */
 
#define AM_RX8025T_BF_UIE   0X20  /**< \brief 时间更新中断位 */
#define AM_RX8025T_BF_TIE   0X10  /**< \brief 定时器中断位 */
#define AM_RX8025T_BF_AIE   0X08  /**< \brief 闹钟中断位 */

 /** @} */

/**
 * \brief 对  更新中断、定时中断、闹钟中断 使能
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] int_select : 选择使能中断的方式  参考 \ref grp_int_select
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * eg：要使能定时器和闹钟中断      AM_RX8025T_BF_TIE | AM_RX8025T_BF_AIE
 *
 */
int am_rx8025t_int_enable (am_rx8025t_handle_t handle, uint8_t int_select);

/**
 * \brief 对  更新中断、定时中断、闹钟中断 失能
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] int_select : 选择失能中断的方式  参考grp_int_select
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * eg：要失能定时器和闹钟中断      AM_RX8025T_BF_TIE | AM_RX8025T_BF_AIE
 *
 */
int am_rx8025t_int_disable (am_rx8025t_handle_t handle, uint8_t int_select);


/**
 * \name 定时器时钟频率的选择
 * \anchor grp_timer_clk
 * @{
 */

#define AM_RX8025T_TIMER_CLK_4096HZ     0X00   /**< \brief 定时器的时钟频率为4096HZ */
#define AM_RX8025T_TIMER_CLK_64HZ       0X01   /**< \brief 定时器的时钟频率为64HZ   */
#define AM_RX8025T_TIMER_CLK_PER_SEC    0X02   /**< \brief 定时器的时钟频率为每秒一次   */
#define AM_RX8025T_TIMER_CLK_PER_MIN    0X03   /**< \brief 定时器的时钟频率为每分钟一次   */

 /** @} */

/**
 * \brief 使能定时器
 *
 * \param[in] handle : RX8025T服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_timer_enable(am_rx8025t_handle_t  handle);

/**
 * \brief 失能定时器
 *
 * \param[in] handle : RX8025T服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_timer_disable(am_rx8025t_handle_t  handle);

/**
 * \brief 设置定时器一些参数，并开启定时器和使能定时器中断
 *
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] count      ： 计数值 ，范围0~4095 ，递减计数
 * \param[in] timer_clk  : 定时器的时钟频率 ，取值参考 \ref grp_timer_clk
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 根据count和timer_clk就可算出触发中断的周期时间
 */
int am_rx8025t_timer_set (am_rx8025t_handle_t  handle, uint16_t count, uint8_t timer_clk);

/**
 * \brief 设置定时器的回调函数
 *
 * \param[in] handle             : RX8025T服务操作句柄
 * \param[in] pfn_timer_callback : 定时器中断回调函数
 * \param[in] p_timer_arg        : 定时器中断回调函数参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_timer_cb_set( am_rx8025t_handle_t  handle,
                             am_pfnvoid_t         pfn_timer_callback,
                             void                *p_timer_arg);


/**
 * \name 更新中断源选择
 * \anchor grp_update_int_select
 * @{
 */

#define AM_RX8025T_USEL_PER_SEC  0X00  /**< \brief 设置为更新的每秒钟触发中断 */
#define AM_RX8025T_USEL_PER_MIN  0X01  /**< \brief 设置为更新的每分钟触发中断 */

 /** @} */

/**
 * \brief 时间更新触发中断功能设置，并使能更新中断
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] mode       : 更新中断的方式 ，值请参考 \ref grp_update_int_select
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_time_update_set( am_rx8025t_handle_t  handle, uint8_t mode);

/**
 * \brief 设置更新中断的回调函数
 *
 * \param[in] handle                  : RX8025T服务操作句柄
 * \param[in] pfn_update_int_callback : 时间更新中断回调函数
 * \param[in] p_update_int_arg        : 时间更新中断回调函数参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_update_int_cb_set(am_rx8025t_handle_t  handle,
                                 am_pfnvoid_t         pfn_update_int_callback,
                                 void                *p_update_int_arg);

/**
 * \name 闹钟的模式选择
 * \anchor grp_alarm_mode
 * @{
 */

#define AM_RX8025T_ALARM_WEEK  0X01   /**< \brief 设置闹钟功能星期为目标 */
#define AM_RX8025T_ALARM_DAY   0X02   /**< \brief 设置闹钟功能天为目标 */

/** @} */

/**
 * \name 闹钟的星期设置
 * \anchor grp_week
 * @{
 */
#define AM_RX8025T_ALARM_SUNDAY    0X01      /**< \brief 星期天 */
#define AM_RX8025T_ALARM_MONDAY    0X02      /**< \brief 星期一 */
#define AM_RX8025T_ALARM_TUESDAY   0X04      /**< \brief 星期二 */
#define AM_RX8025T_ALARM_WEDNESDAY 0X08      /**< \brief 星期三 */
#define AM_RX8025T_ALARM_THURSDAY  0X10      /**< \brief 星期四 */
#define AM_RX8025T_ALARM_FRIDAY    0X20      /**< \brief 星期五 */
#define AM_RX8025T_ALARM_SATURDAY  0X40      /**< \brief 星期六 */

#define AM_RX8025T_ALARM_EVERYDAY  0X80      /**< \brief 每一天 */

/** @} */

 
#define AM_RX8025T_ALARM_IGNORED_MIN   0X80  /**< \brief 忽略分 */
#define AM_RX8025T_ALARM_IGNORED_HOUR  0X80  /**< \brief 忽略小时 */

/**
 * \brief 闹钟信息结构体
 * eg : 设置每个星期的星期一和星期六18点15分闹钟
 *   am_rx8025t_alarm_info_t alarm_info={AM_RX8025T_ALARM_WEEK,
 *                                       15,
 *                                       18,
 *                                       AM_RX8025T_ALARM_MONDAY | AM_RX8025T_ALARM_SATURDAY}
 *
 */
typedef struct am_rx8025t_alarm_info {
    
    /** \brief 闹钟模式，参考 \ref grp_alarm_mode */
    uint8_t alarm_mode;
    
    /** 
     * \brief 分钟 0 ~ 59，若不匹配分钟，则设置为：#AM_RX8025T_ALARM_IGNORED_MIN
     */
    uint8_t min;
    
    /** 
     * \brief 小时 0 ~ 23，若不匹配小时，则设置为：#AM_RX8025T_ALARM_IGNORED_HOUR
     */
    uint8_t hour;
    
     /** 
      * \brief 星期或者天
      * 若模式为 #AM_RX8025T_ALARM_WEEK, 则该值指定星期：请参考 \ref grp_week
      * 若模式为 #AM_RX8025T_ALARM_DAY,  则该值指定天：表示每个月的第几号
      */
    uint8_t week_day;
    
}am_rx8025t_alarm_info_t;

/**
 * \brief RX8025T 闹钟回调函数和闹钟时间设置
 *
 * \param[in] handle              : RX8025T服务操作句柄
 * \param[in] p_alarm_info        : 闹钟的信息
 * \param[in] pfn_alarm_callback  : 闹钟中断回调函数
 * \param[in] p_alarm_arg         : 闹钟中断回调函数参数
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 */
int am_rx8025t_alarm_set(am_rx8025t_handle_t      handle,
                         am_rx8025t_alarm_info_t *p_alarm_info,
                         am_pfnvoid_t             pfn_alarm_callback,
                         void                    *p_alarm_arg);

/**
 * \name 补偿间距设置
 * \anchor grp_offset_mode
 * @{
 */
#define AM_RX8025T_OFFSET_INTERVAL_0_5_SEC   0X00  /**< \brief 补偿间距为0.5s */
#define AM_RX8025T_OFFSET_INTERVAL_2_0_SEC   0X01  /**< \brief 补偿间距为2s */
#define AM_RX8025T_OFFSET_INTERVAL_10_SEC    0X02  /**< \brief 补偿间距为10s */
#define AM_RX8025T_OFFSET_INTERVAL_30_SEC    0X03  /**< \brief 补偿间距为30s */
/** @} */

/**
 * \brief 温度补偿
 *
 * \param[in] handle      : RX8025T服务操作句柄
 * \param[in] offset_mode : 补偿的时间间距 ，值请参考 \ref grp_offset_mode
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_temperature_offset(am_rx8025t_handle_t handle,uint8_t offset_mode);

/**
 * \name 频率输出大小选择
 * \anchor grp_frequency
 * @{
 */
#define AM_RX8025T_FOUT_32768HZ  0X00   /**< \brief 32768 HZ output */
#define AM_RX8025T_FOUT_1024HZ   0X01   /**< \brief 1024 HZ output */
#define AM_RX8025T_FOUT_1HZ      0X02   /**< \brief 1 HZ output */
/** @} */

/**
 * \brief  clkout 输出时钟频率的设置,并开启频率输出
 *
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] frequency  : clkout(FOUT)输出频率的大小，
 *                         有四种大小可以选择 ，值请参考 \ref grp_frequency
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 若要使用芯片输出的频率就必须配置设备信息中clk_en_pin。
 */
int am_rx8025t_clkout_set(am_rx8025t_handle_t handle, uint8_t frequency);

/**
 * \brief  关闭 clk的输出
 * \param[in] handle     : RX8025T服务操作句柄
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_clkout_close(am_rx8025t_handle_t handle);

/**
 * \brief  将一字节的数据写入到RX8025T的RAM中
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] data       : 要写入的数据
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 */
int am_rx8025t_ram_write (am_rx8025t_handle_t handle, uint8_t data);

/**
 * \brief  clkout 读出RX8025T RAM一个字节的数据
 * \param[in] handle     : RX8025T服务操作句柄
 * \param[in] p_data     : 读出的数据要存放的地址
 *
 * \retval AM_OK : 操作成功
 * \retval 其他  : 操作错误,具体查看返回错误编码
 *
 * \note 只读出一个字节的数据
 */
int am_rx8025t_ram_read (am_rx8025t_handle_t handle, uint8_t *p_data);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_RX8025T_H */

/* end of file */
