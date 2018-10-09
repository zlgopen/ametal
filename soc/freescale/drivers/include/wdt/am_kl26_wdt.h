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
 * \brief  WDT驱动，服务WDT标准接口
 *
 * 1. 看门狗的配置寄存器只能写一次，所以在使用KL26芯片时，当不需要使
 *    用看门狗外设时将工程配置文件里的看门狗开关宏置为0，系统将在板级初
 *    始化时关闭看门狗，当需要使用看门狗外设时，将看门狗开关宏置1，系统将
 *    在看门狗使能函数中配置看门狗，使之正常工作。
 * 2. 看门狗使用LPO时钟时，其超时时间只能设置为32ms,256ms,1024ms这三个时间中的一个，
 *    默认使用LPO时钟输入。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_WDT_H
#define __AM_KL26_WDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_wdt.h"
#include "hw/amhw_kl26_sim.h"

/**
 * \addtogroup am_kl26_if_wdt
 * \copydoc am_kl26_wdt.h
 * @{
 */

/**
 * \brief WDT 设备信息结构体
 */
typedef struct am_kl26_wdt_devinfo {

	/** \brief 指向WDT寄存器块的结构体指针 */
    amhw_kl26_sim_t *p_hw_wdt;

    /** \brief 平台初始化函数 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_kl26_wdt_devinfo_t;

/**
 * \brief WDT 设备结构体
 */
typedef struct am_kl26_wdt_dev {

    am_wdt_serv_t                 wdt_serv;   /**< \brief WDT标准服务         */
    const am_kl26_wdt_devinfo_t  *p_devinfo;  /**< \brief 指向WDT设备信息的指针 */

} am_kl26_wdt_dev_t;

/**
 * \brief 初始化WDT
 *
 * \param[in] p_dev     : 指向WDT设备
 * \param[in] p_devinfo : 指向WDT设备信息
 *
 * \return WDT标准服务操作句柄。如果为 NULL，表明初始化失败。
 */
am_wdt_handle_t am_kl26_wdt_init(am_kl26_wdt_dev_t           *p_dev,
                                 const am_kl26_wdt_devinfo_t *p_devinfo);

/**
 * \brief 解除WDT初始化
 * \param[in] handle : WDT设备句柄值
 * \return  无
 */
void am_kl26_wdt_deinit(am_wdt_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_WDT_H */

/* end of file */
