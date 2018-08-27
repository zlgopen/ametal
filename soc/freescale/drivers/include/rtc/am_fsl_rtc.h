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
 * \brief RTC 模块的驱动层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_RTC_H
#define __AM_FSL_RTC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "am_rtc.h"
#include "hw/amhw_fsl_rtc.h"

/**
 * \addtogroup am_fsl_if_rtc
 * \copydoc am_fsl_rtc.h
 * @{
 */

/**
 * \brief RTC 设备信息结构体
 */
typedef struct am_fsl_rtc_devinfo {
    amhw_fsl_rtc_t  *p_hw_rtc;      /**< \brief 指向RTC寄存器块的结构体指针 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_fsl_rtc_devinfo_t;

/**
 * \brief RTC 设备结构体
 */
typedef struct am_fsl_rtc_dev {
    am_rtc_serv_t               rtc_serv;     /**< \brief RTC标准服务           */
    const am_fsl_rtc_devinfo_t *p_devinfo;    /**< \brief 指向RTC设备信息的指针 */
} am_fsl_rtc_dev_t;

/**
 * \brief 初始化RTC
 *
 * \param[in] p_dev     : 指向RTC设备
 * \param[in] p_devinfo : 指向RTC设备信息
 *
 * \return RTC标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_rtc_handle_t am_fsl_rtc_init (am_fsl_rtc_dev_t           *p_dev,
                                 const am_fsl_rtc_devinfo_t *p_devinfo);

/**
 *  \brief 解除RTC初始化
 *
 * \param[in] handle : am_fsl_rtc_init() 初始化函数获得的RTC服务句柄
 *
 *  \return 无
 */
void am_fsl_rtc_deinit (am_rtc_handle_t handle);

/**
 *  \brief 根据日期计算一年中的第几天
 *
 *  \param day
 *  \param month
 *  \param year
 *
 *  \return 一年中的第几天 (1月1日记为第0天)
 *
 *  \details 如 am_fsl_rtc_date2yday(4, 11, 2015)计算的是11月4日是
 *           2015年的第307天
 */
int am_fsl_rtc_date2yday (uint8_t day, uint8_t month, uint32_t year);


/**
 *  \brief 设置RTC的校准配置
 *
 *  \param p_dev 指向RTC设备的指针
 *
 *  \param frq RTC时钟的实际频率，以Hz计
 *
 *  \return 实际校准后的频率
 */
int am_fsl_rtc_compensation_set (am_fsl_rtc_dev_t *p_dev, float frq);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AM_FSL_RTC_H */

