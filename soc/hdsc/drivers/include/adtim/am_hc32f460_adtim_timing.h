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
 * \brief 定时器ADTIM驱动，服务Timer驱动层实现
 *
 * 1. ADTIM支持提供如下三种标准服务，本驱动提供的是服务Timer标准服务的驱动。
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. 一个ADTIM模块，只有一个定时通道
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-09  zp, first implementation
 * \endinternal
 */

#ifndef __AM_HC32F460_ADTIM_ADTIMING_H
#define __AM_HC32F460_ADTIM_ADTIMING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"
#include "hw/amhw_hc32f460_adtim.h"

/**
 * \addtogroup am_hc32f460_if_adtim_timing
 * \copydoc am_hc32f460_adtim_timing.h
 * @{
 */

/**
 * \brief ADTIM定时功能相关的设备信息
 */
typedef struct am_hc32f460_adtim_timing_devinfo {

    /**< \brief ADTIM寄存器块基址 */
    uint32_t                        adtim_regbase;

    /**< \brief ADTIM中断号 */
    uint8_t                         inum;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                            (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void                            (*pfn_plfm_deinit)(void);

} am_hc32f460_adtim_timing_devinfo_t;

/**
 * \brief ADTIM定时功能设备
 */
typedef struct am_hc32f460_adtim_timing_dev {

    am_timer_serv_t adtimer_serv;       /**< \brief 标准定时(Timer)服务 */

    void (*pfn_callback)(void *);       /**< \brief 回调函数 */
    void  *p_arg;                       /**< \brief 回调函数的用户参数 */

    uint16_t           prescale_to;     /**< \brief 期望分频值  */
    uint16_t           prescale_in;     /**< \brief 实际分频值  */

    /** \brief 指向ADTIM(定时功能)设备信息常量的指针 */
    const am_hc32f460_adtim_timing_devinfo_t  *p_devinfo;

} am_hc32f460_adtim_timing_dev_t;

/**
 * \brief 初始化ADTIM为定时功能
 *
 * \param[in] p_dev     : 指向ADTIM(定时功能)设备的指针
 * \param[in] p_devinfo : 指向ADTIM(定时功能)设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t
am_hc32f460_adtim_timing_init (am_hc32f460_adtim_timing_dev_t           *p_dev,
                               const am_hc32f460_adtim_timing_devinfo_t *p_devinfo);

/**
 * \brief 不使用ADTIM定时功能时，解初始化ADTIM定时功能，释放相关资源
 *
 * \param[in] handle : am_hc32f460_adtim_timing_init() 初始化函数获得的Timer服务句柄
 *
 * \return 无
 */
void am_hc32f460_adtim_timing_deinit (am_timer_handle_t handle);

void Timer61GCMA_IrqHandler(void *p_arg);
void Timer61GCMB_IrqHandler(void *p_arg);
void Timer61GCMC_IrqHandler(void *p_arg);
void Timer61GCMD_IrqHandler(void *p_arg);
void Timer61GCME_IrqHandler(void *p_arg);
void Timer61GCMF_IrqHandler(void *p_arg);
void Timer61GOV_IrqHandler(void *p_arg);
void Timer61GUD_IrqHandler(void *p_arg);
void Timer61GDT_IrqHandler(void *p_arg);
void Timer61SCMA_IrqHandler(void *p_arg);
void Timer61SCMB_IrqHandler(void *p_arg);

void Timer62GCMA_IrqHandler(void *p_arg);
void Timer62GCMB_IrqHandler(void *p_arg);
void Timer62GCMC_IrqHandler(void *p_arg);
void Timer62GCMD_IrqHandler(void *p_arg);
void Timer62GCME_IrqHandler(void *p_arg);
void Timer62GCMF_IrqHandler(void *p_arg);
void Timer62GOV_IrqHandler(void *p_arg);
void Timer62GUD_IrqHandler(void *p_arg);
void Timer62GDT_IrqHandler(void *p_arg);
void Timer62SCMA_IrqHandler(void *p_arg);
void Timer62SCMB_IrqHandler(void *p_arg);

void Timer63GCMA_IrqHandler(void *p_arg);
void Timer63GCMB_IrqHandler(void *p_arg);
void Timer63GCMC_IrqHandler(void *p_arg);
void Timer63GCMD_IrqHandler(void *p_arg);
void Timer63GCME_IrqHandler(void *p_arg);
void Timer63GCMF_IrqHandler(void *p_arg);
void Timer63GOV_IrqHandler(void *p_arg);
void Timer63GUD_IrqHandler(void *p_arg);
void Timer63GDT_IrqHandler(void *p_arg);
void Timer63SCMA_IrqHandler(void *p_arg);
void Timer63SCMB_IrqHandler(void *p_arg);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_ADTIM_ADTIMING_H */

/* end of file */
