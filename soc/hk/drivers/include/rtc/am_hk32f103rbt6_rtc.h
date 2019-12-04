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
 * - 1.00 19-07-23  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HK32F103RBT6_RTC_H
#define __AM_HK32F103RBT6_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_rtc.h"
#include "hw/amhw_hk32f103rbt6_rtc.h"

/**
 * \addtogroup am_hk32f103rbt6_if_rtc
 * \copydoc am_hk32f103rbt6_rtc.h
 * @{
 */

#define  AM_HK32F103RBT6_RTC_CALLBACK_SECOND  (1 << 0)  /**< \brief RTC 秒中断回调函数编号 */
#define  AM_HK32F103RBT6_RTC_CALLBACK_ALARM   (1 << 1)  /**< \brief RTC 闹钟中断回调函数编号 */
#define  AM_HK32F103RBT6_RTC_CALLBACK_OVER    (1 << 2)  /**< \brief RTC 溢出中断回调函数编号 */

/**
 * \brief HK32F103RBT6 RTC 设备信息
 */
typedef struct am_hk32f103rbt6_rtc_devinfo {
    uint32_t rtc_regbase;            /**< \brief RTC 寄存器基地址 */
    uint32_t pwr_regbase;            /**< \brief PWR 寄存器基地址 */
    uint32_t bkp_regbase;            /**< \brief BKP 寄存器基地址 */
    uint32_t rtc_clk_sour;           /**< \brief RTC 时钟源 */
    int      rtc_inum;               /**< \brief RTC 中断号 */
    void   (*pfn_plfm_init)(void);   /**< \brief 平台初始化函数 */
    void   (*pfn_plfm_deinit)(void); /**< \brief 平台解初始化函数 */
} am_hk32f103rbt6_rtc_devinfo_t;

/**
 * \brief HK32F103RBT6 RTC 设备
 */
typedef struct am_hk32f103rbt6_rtc_dev {
    am_rtc_serv_t                  rtc_serv;        /**< \brief RTC 标准服务 */
    uint8_t                        int_state;       /**< \brief 中断使能标志 */
    am_bool_t                      rtc_continue;    /**< \brief RTC 是否继续运行 */
    am_pfnvoid_t                   pfn_callback[3]; /**< \brief 回调函数 */
    void                          *p_arg[3];        /**< \brief 回调函数参数 */
    const am_hk32f103rbt6_rtc_devinfo_t *p_devinfo;       /**< \brief RTC 设备信息 */
}am_hk32f103rbt6_rtc_dev_t;

/**
 * \brief RTC 状态获取，可以用来判断本次运行前备份区域是否断电
 *
 * \param[in] handle RTC 标准服务操作句柄
 *
 * \retval AM_TRUE  备份区域未断电，RTC 计数器接着上一次的状态运行
 * \retval AM_FALSE 备份区域断电，RTC 计数器从 0 开始运行
 */
am_bool_t am_hk32f103rbt6_rtc_state_get (am_rtc_handle_t handle);

/**
 * \brief RTC 同步标志复位，当 APB1 曾经被禁止过，需要调用此接口
 *
 * \param[in] handle RTC 标准服务操作句柄
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 *
 * \note 发生系统复位或电源复位、系统刚从待机模式唤醒、
 *       系统刚从停机模式唤醒时，应该调用此接口
 */
am_err_t am_hk32f103rbt6_rtc_apb1_reflush (am_rtc_handle_t handle);

/**
 * \brief 获取 RTC 计数器值
 *
 * \param[in]  handle RTC 标准服务操作句柄
 * \param[out] p_cnt  指向存储获取到的 RTC 计数器值的指针
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_cnt_get (am_rtc_handle_t handle, uint32_t *p_cnt);

/**
 * \brief 设置 RTC 计数器值
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] cnt    RTC 计数器值
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_cnt_set (am_rtc_handle_t handle, uint32_t cnt);

/**
 * \brief 设置 RTC 闹钟值
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] alr    RTC 闹钟值
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_alr_set (am_rtc_handle_t handle, uint32_t alr);

/**
 * \brief 设置 RTC 闹钟时间
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] p_tm   需要设置的时间
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_alarm_set (am_rtc_handle_t handle, am_tm_t *p_tm);

/**
 * \brief 设置 RTC 中断回调
 *
 * \param[in] handle       RTC 标准服务操作句柄
 * \param[in] type         中断类型，可选 AM_HK32F103RBT6_RTC_CALLBACK_SECOND
 *                                       AM_HK32F103RBT6_RTC_CALLBACK_ALARM
 *                                       AM_HK32F103RBT6_RTC_CALLBACK_OVER
 * \param[in] pfn_callback 回调函数
 * \param[in] p_arg        回调函数参数
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_callback_set (am_rtc_handle_t handle,
                                     uint8_t         type,
                                     am_pfnvoid_t    pfn_callback,
                                     void           *p_arg);

/**
 * \brief RTC 中断使能
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] type   中断类型，可选 AM_HK32F103RBT6_RTC_CALLBACK_SECOND
 *                                 AM_HK32F103RBT6_RTC_CALLBACK_ALARM
 *                                 AM_HK32F103RBT6_RTC_CALLBACK_OVER
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_int_enable (am_rtc_handle_t handle,
                                   uint8_t         type);

/**
 * \brief RTC 中断失能
 *
 * \param[in] handle RTC 标准服务操作句柄
 * \param[in] type   中断类型，可选 AM_HK32F103RBT6_RTC_CALLBACK_SECOND
 *                                 AM_HK32F103RBT6_RTC_CALLBACK_ALARM
 *                                 AM_HK32F103RBT6_RTC_CALLBACK_OVER
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hk32f103rbt6_rtc_int_disable (am_rtc_handle_t handle,
                                    uint8_t         type);
/**
 * \brief 初始化 RTC
 *
 * \param[in] p_dev     指向 RTC 设备的指针
 * \param[in] p_devinfo 指向 RTC 设备信息的指针
 *
 * \return RTC 标准服务操作句柄，如果为 NULL，表明初始化失败
 */
am_rtc_handle_t am_hk32f103rbt6_rtc_init (am_hk32f103rbt6_rtc_dev_t           *p_dev,
                                    const am_hk32f103rbt6_rtc_devinfo_t *p_devinfo);

/**
 * \brief 解初始化 RTC
 *
 * \param[in] handle 使用 am_hk32f103rbt6_rtc_init() 获取到的 RTC 标准服务操作句柄
 *
 * \return 无
 */
void am_hk32f103rbt6_rtc_deinit (am_rtc_handle_t handle);

/** @} */

/**
 * @} hk32f103rbt6_if_rtc
 */

#ifdef __cplusplus
}
#endif

#endif/* __HK32F103RBT6_RTC_H */

/* end of file */
