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
 * \brief TPM驱动，服务Timer驱动层实现
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务Timer标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-14  sdy, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_TPM_TIMING_H
#define __AM_FSL_TPM_TIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_fsl_tpm.h"

/**
 * \addtogroup am_fsl_if_tpm_timing
 * \copydoc am_fsl_tpm_timing.h
 * @{
 */

/**
 * \brief TPM定时功能相关的设备信息
 */
typedef struct am_fsl_tpm_timing_devinfo {

    amhw_fsl_tpm_t   *p_hw_tpm;    /**< \brief 指向TPM寄存器块的指针 */
    uint8_t           inum;        /**< \brief TPM中断号  */
    uint32_t          clk_id;      /**< \brief 时钟号  */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

} am_fsl_tpm_timing_devinfo_t;
    
/**
 * \brief TPM定时功能设备
 */
typedef struct am_fsl_tpm_timing_dev {

    am_timer_serv_t   timer_serv;       /**< \brief 标准定时(Timer)服务 */

    void (*pfn_callback)(void *);       /**< \brief 回调函数 */
    void *p_arg;                        /**< \brief 回调函数的用户参数 */
    
    /** \brief 指向TPM(定时功能)设备信息常量的指针 */
    const am_fsl_tpm_timing_devinfo_t  *p_devinfo;

} am_fsl_tpm_timing_dev_t;

/**
 * \brief 初始化TPM为定时功能
 *
 * \param[in] p_dev     : 指向TPM(定时功能)设备的指针
 * \param[in] p_devinfo : 指向TPM(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t am_fsl_tpm_timing_init (am_fsl_tpm_timing_dev_t            *p_dev,
                                          const am_fsl_tpm_timing_devinfo_t  *p_devinfo);

/**
 * \brief 不使用TPM定时功能时，解初始化TPM定时功能，释放相关资源
 * \param[in] handle : am_fsl_tpm_timing_init() 初始化函数获得的Timer服务句柄
 * \return 无
 */
void am_fsl_tpm_timing_deinit (am_timer_handle_t handle);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_FSL_TPM_TIMING_H */

/* end of file */
