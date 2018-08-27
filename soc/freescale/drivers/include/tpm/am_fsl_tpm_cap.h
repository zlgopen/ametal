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
 * \brief TPM驱动，服务CAP标准接口
 *
 * 1. TPM支持提供如下三种标准服务，本驱动提供的是服务“捕获”标准服务的驱动：
 *     - 定时
 *     - PWM输出
 *     - 捕获
 * 2. TPM0可配置6个捕获通道，TPM1和TPM2可配置2个捕获通道。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-13  sdy, first implementation.
 * \endinternal
 */

#ifndef __AM_FSL_TPM_CAP_H
#define __AM_FSL_TPM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_cap.h"
#include "hw/amhw_fsl_tpm.h"

/**
 * \addtogroup am_fsl_if_tpm_cap
 * \copydoc am_fsl_tpm_cap.h
 * @{
 */

/**
 * \brief TPM捕获功能相关的GPIO信息
 */
typedef struct am_fsl_tpm_cap_ioinfo {
    uint32_t gpio;           /**< \brief 对应的GPIO管脚                       */
    uint32_t func;           /**< \brief 为捕获功能时的GPIO功能设置           */
    uint32_t dfunc;          /**< \brief 禁能管脚捕获功能时的默认GPIO功能设置 */
} am_fsl_tpm_cap_ioinfo_t;

/**
 * \brief TPM捕获功能相关的设备信息
 */
typedef struct am_fsl_tpm_cap_devinfo {
    amhw_fsl_tpm_t           *p_hw_tpm;      /**< \brief 指向TPM寄存器块的指针  */
    uint8_t                   inum;          /**< \brief TPM对应的中断号 */
    uint8_t                   channels_num;  /**< \brief 使用的通道数，最大为6 */
    uint32_t                  clk_id;        /**< \brief 时钟号  */
    am_fsl_tpm_cap_ioinfo_t  *p_ioinfo;      /**< \brief 指向捕获管脚信息结构体 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
} am_fsl_tpm_cap_devinfo_t;

/**
 * \brief TPM捕获功能设备
 */
typedef struct am_fsl_tpm_cap_dev {
    am_cap_serv_t          cap_serv;      /**< \brief 标准CAP服务            */

    struct {
        am_cap_callback_t  callback_func; /**< \brief 回调函数                    */
        void              *p_arg;         /**< \brief 回调函数的用户参数          */
    } callback_info[6];                   /**< \brief 存储最大6通道的回调函数信息 */

    /** \brief 指向TPM(捕获功能)设备信息常量的指针 */
    const am_fsl_tpm_cap_devinfo_t *p_devinfo;

} am_fsl_tpm_cap_dev_t;

/**
 * \brief 初始化TPM为捕获功能
 *
 * \param[in] p_dev     : 指向TPM(捕获功能)设备的指针
 * \param[in] p_devinfo : 指向TPM(捕获功能)设备信息常量的指针
 *
 * \return CAP标准服务操作句柄，值为NULL时表明初始化失败
 */
am_cap_handle_t am_fsl_tpm_cap_init (am_fsl_tpm_cap_dev_t           *p_dev,
                                     const am_fsl_tpm_cap_devinfo_t *p_devinfo);

/**
 * \brief 不使用TPM捕获功能时，解初始化TPM捕获功能，释放相关资源
 * \param[in] handle : am_fsl_tpm_cap_init() 初始化函数获得的cap服务句柄
 * \return 无
 */
void am_fsl_tpm_cap_deinit (am_cap_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_FSL_TPM_CAP_H */

/* end of file */
