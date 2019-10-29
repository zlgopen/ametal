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
 * \brief RTC drivers for standard interface
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-16  zp, first implementation
 * \endinternal
 */

#ifndef __AM_ZLG118_RTC_H
#define __AM_ZLG118_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_rtc.h"
#include "hw/amhw_zlg118_rtc.h"
#include "hw/amhw_zlg118_rcc.h"

/**
 * \addtogroup am_zlg118_if_rtc
 * \copydoc am_zlg118_rtc.h
 * @{
 */

#define  AM_ZLG118_RTC_CALLBACK_PERIOD  (1 << 0) /**< \brief RTC周期中断回调函数编号 */
#define  AM_ZLG118_RTC_CALLBACK_ALARM   (1 << 1) /**< \brief RTC闹钟中断回调函数编号 */

/**
 * \brief ZLG118 1Hz输出控制结构体
 */
typedef struct {
    am_bool_t enable;                 /* 是否使能1Hz输出功能
                                       * AM_TRUE ： 使能 （请阅读下面两项配置的注释）
                                       * AM_FALSE： 禁能（下面两项配置无效）
                                       */
    float     ppm;                    /* 误差补偿值（-274.6 ~ 212.6）ppm，范围外代表关闭*/
    am_bool_t adjust_enable;          /* 在ppm处于范围内时（即开启补偿的情况下），
                                       * 选择是否开启高速时钟作为补偿时钟
                                       *
                                       * AM_TRUE ： 开启高速时钟作为补偿时钟
                                       * AM_FALSE： 关闭高速时钟作为补偿时钟
                                       */
    amhw_zlg118_rtcclk_adjust adjust_clk; /* 高速时钟补偿时钟选择 ，建议和PCLK频率相同  */
} am_zlg118_rtc_1hz_out_t;


/**
 * \brief ZLG118 RTC 设备信息
 */
typedef struct am_zlg118_rtc_devinfo {
    uint32_t                  rtc_regbase;           /**< \brief RTC 寄存器基地址 */
    amhw_zlg118_rtc_clk_src_t rtc_clk_sour;          /**< \brief RTC 时钟源 */
    int                       rtc_inum;              /**< \brief RTC 中断号 */
    am_zlg118_rtc_1hz_out_t   ctrl_1hz_out;
    void                    (*pfn_plfm_init)(void);  /**< \brief 平台初始化函数 */
    void                    (*pfn_plfm_deinit)(void);/**< \brief 平台解初始化函数 */
} am_zlg118_rtc_devinfo_t;

/**
 * \brief ZLG118 RTC 设备
 */
typedef struct am_zlg118_rtc_dev {
    am_rtc_serv_t                  rtc_serv;        /**< \brief RTC 标准服务 */
    uint8_t                        int_state;       /**< \brief 中断使能标志 */
    am_bool_t                      rtc_continue;    /**< \brief RTC是否继续运行 */
    am_pfnvoid_t                   pfn_callback[2]; /**< \brief 回调函数 */
    void                          *p_arg[2];        /**< \brief 回调函数参数 */
    const am_zlg118_rtc_devinfo_t *p_devinfo;       /**< \brief RTC设备信息 */
}am_zlg118_rtc_dev_t;

/**
 * \brief 获取 RTC 计数器值
 *
 * \param[in]  handle RTC 标准服务操作句柄
 * \param[in]  handle RTC 标准服务操作句柄
 * \param[in] *sec    秒（0-59）
 * \param[in] *min    分（0-59）
 * \param[in] *hour   时（0-23）
 * \param[in] *mday   每月的第几天（0-29,0-28,0-30,0-31）
 * \param[in] *wday   一周的第几天（0-6，对应周日-周六）
 * \param[in] *mon    月（1-12）
 * \param[in] *year   年（0-99）
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_cnt_get (am_rtc_handle_t handle,
                                uint8_t        *sec,
                                uint8_t        *min,
                                uint8_t        *hour,
                                uint8_t        *mday,
                                uint8_t        *wday,
                                uint8_t        *mon,
                                uint8_t        *year);

/**
 * \brief 设置 RTC 计数器值
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] sec    秒（0-59）
 * \param[in] min    分（0-59）
 * \param[in] hour   时（0-23）
 * \param[in] mday   每月的第几天（0-29,0-28,0-30,0-31）
 * \param[in] wday   一周的第几天（0-6，对应周日-周六）
 * \param[in] mon    月（1-12）
 * \param[in] year   年（0-99）
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_cnt_set (am_rtc_handle_t handle,
                                uint8_t         sec,
                                uint8_t         min,
                                uint8_t         hour,
                                uint8_t         mday,
                                uint8_t         wday,
                                uint8_t         mon,
                                uint8_t         year);

/**
 * \brief 设置 RTC 中断回调
 *
 * \param[in] handle       RTC 标准服务操作句柄
 * \param[in] type         中断类型，可选 AM_ZLG118_RTC_CALLBACK_SECOND
 *                                   AM_ZLG118_RTC_CALLBACK_PERIOD
 * \param[in] pfn_callback 回调函数
 * \param[in] p_arg        回调函数参数
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_callback_set (am_rtc_handle_t handle,
                                     uint8_t         type,
                                     am_pfnvoid_t    pfn_callback,
                                     void           *p_arg);

/**
 * \brief RTC周期中断使能
 *
 * \param[in] handle RTC标准服务操作句柄
 * \param[in] type   周期中断类型，见枚举定义amhw_zlg118_rtc_period_int_type_t
 * \param[in] set_s  周期类型为系统定义时，所设置周期中断产生时间amhw_zlg118_rtc_period_int_time_t
 * \param[in] set_c  周期类型为自定义时，所设置周期中断产生时间（0.5 ~ 32s）
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_period_int_enable(
              am_rtc_handle_t                   handle,
              amhw_zlg118_rtc_period_int_type_t type,
              amhw_zlg118_rtc_period_int_time_t set_s,
              float                             set_c);

/**
 * \brief RTC闹钟中断使能
 *
 * \param[in] handle RTC标准服务操作句柄
 * \param[in] sec    闹钟时间-秒（0-59）
 * \param[in] min    闹钟时间-分（0-59）
 * \param[in] hour   闹钟时间-时（0-59）
 * \param[in] wday   闹钟时间-周几（0-6，对应周日-周六）
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_alarm_clock_int_enable(am_rtc_handle_t handle,
                                              uint8_t         sec,
                                              uint8_t         min,
                                              uint8_t         hour,
                                              uint8_t         wday);

/**
 * \brief RTC 中断失能
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] type   中断类型，可选 AM_ZLG118_RTC_CALLBACK_PERIOD
 *                             AM_ZLG118_RTC_CALLBACK_ALARM
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_zlg118_rtc_int_disable (am_rtc_handle_t handle,
                                    uint8_t         type);

/**
 * \brief 初始化 RTC
 *
 * \param[in] p_dev     指向 RTC 设备的指针
 * \param[in] p_devinfo 指向 RTC 设备信息的指针
 *
 * \return RTC 标准服务操作句柄，如果为 NULL，表明初始化失败
 */
am_rtc_handle_t am_zlg118_rtc_init (am_zlg118_rtc_dev_t           *p_dev,
                                    const am_zlg118_rtc_devinfo_t *p_devinfo);

/**
 * \brief 解初始化 RTC
 *
 * \param[in] handle 使用 am_zlg118_rtc_init() 获取到的 RTC 标准服务操作句柄
 *
 * \return 无
 */
void am_zlg118_rtc_deinit (am_rtc_handle_t handle);

/** @} */

/**
 * @} zlg_if_rtc
 */

#ifdef __cplusplus
}
#endif

#endif/* __ZLG118_RTC_H */

/* end of file */
