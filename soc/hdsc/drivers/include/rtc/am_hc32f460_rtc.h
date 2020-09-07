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

#ifndef __AM_HC32_RTC_H
#define __AM_HC32_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_rtc.h"
#include "hw/amhw_hc32f460_rtc.h"

/**
 * \addtogroup am_hc32f460_if_rtc
 * \copydoc am_hc32f460_rtc.h
 * @{
 */

/**
 * \brief HC32 RTC 设备信息
 */
typedef struct am_hc32f460_rtc_devinfo {
    uint32_t                  rtc_regbase;           /**< \brief RTC 寄存器基地址 */
    int                       inum;                  /**< \brief RTC 中断号 */
    void                    (*pfn_plfm_init)(void);  /**< \brief 平台初始化函数 */
    void                    (*pfn_plfm_deinit)(void);/**< \brief 平台解初始化函数 */
} am_hc32f460_rtc_devinfo_t;

/**
 * \brief HC32 RTC 设备
 */
typedef struct am_hc32f460_rtc_dev {
    am_rtc_serv_t                  rtc_serv;        /**< \brief RTC 标准服务 */
    uint8_t                        int_state;       /**< \brief 中断使能标志 */
    am_bool_t                      rtc_continue;    /**< \brief RTC是否继续运行 */
    am_pfnvoid_t                   pfn_callback[2]; /**< \brief 回调函数 */
    void                          *p_arg[2];        /**< \brief 回调函数参数 */
    const am_hc32f460_rtc_devinfo_t *p_devinfo;     /**< \brief RTC设备信息 */
}am_hc32f460_rtc_dev_t;

/**
 * \brief 初始化 RTC
 *
 * \param[in] p_dev     指向 RTC 设备的指针
 * \param[in] p_devinfo 指向 RTC 设备信息的指针
 *
 * \return RTC 标准服务操作句柄，如果为 NULL，表明初始化失败
 */
am_rtc_handle_t am_hc32f460_rtc_init (am_hc32f460_rtc_dev_t           *p_dev,
                                      const am_hc32f460_rtc_devinfo_t *p_devinfo);

/**
 * \brief 解初始化 RTC
 *
 * \param[in] handle 使用 am_hc32f460_rtc_init() 获取到的 RTC 标准服务操作句柄
 *
 * \return 无
 */
void am_hc32f460_rtc_deinit (am_rtc_handle_t handle);

void RTC_ALM_IrqHandler(void *p_arg);

void RTC_PRD_IrqHandler(void *p_arg);

/** @} */

/**
 * @} hc32f460_if_rtc
 */

#ifdef __cplusplus
}
#endif

#endif/* __HC32F460_RTC_H */

/* end of file */
