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
 * - 1.11 20-04-23 zcb, Add RTC interrupt management interface
 * - 1.10 20-04-17 zcb, Adapt standard drive and delete interfaces that
 *                      are not related to standards
 * - 1.00 19-10-16  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32_RTC_H
#define __AM_HC32_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_rtc.h"
#include "hw/amhw_hc32_rtc.h"
#include "hw/amhw_hc32_rcc.h"

/**
 * \addtogroup am_hc32_if_rtc
 * \copydoc am_hc32_rtc.h
 * @{
 */

/**
 * \name HC32 RTC 中断类型
 * @{
 */

/** \brief RTC 周期中断 */
#define  AM_HC32_RTC_INT_PERIOD  (0x0)

/** \brief RTC 闹钟中断 */
#define  AM_HC32_RTC_INT_ALARM   (0x1)

/** \brief RTC 中断类型数量 */
#define  AM_HC32_RTC_INT_CNT     (AM_HC32_RTC_INT_ALARM + 1)

/** @} */

/**
 * \brief HC32 RTC 设备信息
 */
typedef struct am_hc32_rtc_devinfo {

    /** \brief RTC 寄存器基地址 */
    uint32_t                     regbase;

    /** \brief RTC 中断号 */
    int                          inum;

    /** \brief RTC 年份起始，因为 HC32 RTC 年份表示范围为 0-99，因此必须指定年份起始时间，大于1900 */
    uint32_t                     year_epoch;

    /** \brief 平台初始化函数 */
    void                       (*pfn_plfm_init) (void);
    /** \brief 平台解初始化函数 */
    void                       (*pfn_plfm_deinit) (void);
} am_hc32_rtc_devinfo_t;

/**
 * \brief HC32 RTC 设备
 */
typedef struct am_hc32_rtc_dev {

    /** \brief RTC 标准服务 */
    am_rtc_serv_t                rtc_serv;

    /** \brief RTC设备信息 */
    const am_hc32_rtc_devinfo_t *p_devinfo;

    /** \brief 回调函数 */
    am_pfnvoid_t                 pfn_callback[AM_HC32_RTC_INT_CNT];

    /** \brief 回调函数参数 */
    void                        *p_arg[AM_HC32_RTC_INT_CNT];
}am_hc32_rtc_dev_t;

/**
 * \brief 初始化 RTC
 *
 * \param[in] p_dev        : 指向 RTC 设备的指针
 * \param[in] p_devinfo    : 指向 RTC 设备信息的指针
 *
 * \return RTC 标准服务操作句柄，如果为 NULL，表明初始化失败
 */
am_rtc_handle_t am_hc32_rtc_init (am_hc32_rtc_dev_t           *p_dev,
                                  const am_hc32_rtc_devinfo_t *p_devinfo);

/**
 * \brief 解初始化 RTC
 *
 * \param[in] handle       : 使用 am_hc32_rtc_init() 获取到的 RTC 标准服务操作句柄
 *
 * \return 无
 */
void am_hc32_rtc_deinit (am_rtc_handle_t            handle);

/******************************************************************************/

/**
 * \brief 根据 RTC 中断类型设置回调函数
 *
 * \param[in] handle       : 待操作的RTC 总中断对应 RTC 外设适配的 RTC 操作句柄
 * \param[in] type         : 中断类型，AM_HC32_RTC_INT_xxxx
 * \param[in] pfn_callback : 回调函数
 * \param[in] p_arg        : 回调函数参数
 *
 * \note  因为 RTC 只有一个中断源而闹钟需要适配标准闹钟驱动，周期中断可用于适配标准定时器驱动因
 *        此在 RTC 驱动内除适配标准 RTC 接口外再增加对 RTC 中断的管理
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hc32_rtc_int_connect (am_rtc_handle_t   handle,
                                  uint8_t           type,
                                  am_pfnvoid_t      pfn_callback,
                                  void             *p_arg);

/**
 * \brief RTC 中断使能
 *
 * \param[in] handle       : RTC 标准服务操作句柄
 * \param[in] type         : 中断类型，AM_HC32_RTC_INT_xxxx
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hc32_rtc_int_disable (am_rtc_handle_t   handle,
                                  uint8_t           type);

/**
 * \brief RTC 中断禁能
 *
 * \param[in] handle       : RTC 标准服务操作句柄
 * \param[in] type         : 中断类型，AM_HC32_RTC_INT_xxxx
 *
 * \retval  AM_OK     成功
 * \retval -AM_EINVAL 参数错误
 */
am_err_t am_hc32_rtc_int_enable  (am_rtc_handle_t   handle,
                                  uint8_t           type);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32_RTC_H */

/* end of file */
