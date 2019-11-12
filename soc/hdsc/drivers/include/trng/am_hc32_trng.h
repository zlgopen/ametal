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
 * \brief trng驱动，服务trng标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-08
 * \endinternal
 */
#ifndef __AM_HC32_trng_H
#define __AM_HC32_trng_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_hc32_if_trng
 * \copydoc am_hc32_trng.h
 * @{
 */

/**
 * \brief 随机数生成环境
 */
#define  AM_HC32_TRNG_POWER_ON      0     /**< \brief 第一次上电产生随机数 */
#define  AM_HC32_TRNG_POWER_NON     1     /**< \brief 非第一次上电产生随机数 */

/**
 * \brief TRNG设备信息结构体
 */
typedef struct am_hc32_trng_info {

    uint32_t    reg_base;  /**< \brief trng寄存器块基地址 */

    uint8_t     rng_cnt;   /**< \brief trng移位次数 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

}am_hc32_trng_devinfo_t;

/**
 * \brief TRNG设备实例
 */
typedef struct am_hc32_trng_dev {

    /** \brief 指向trng设备信息的指针 */
    const am_hc32_trng_devinfo_t *p_devinfo;

} am_hc32_trng_dev_t;

/** \brief TRNG标准服务操作句柄类型定义 */
typedef am_hc32_trng_dev_t *am_trng_handle_t;

/**
 * \brief trng 初始化
 *
 * \param[in] p_dev     : trng 设备实例
 * \param[in] p_devinfo : trng 设备信息实例
 *
 * \retval : 标准TRNG服务句柄
 *
 */
am_trng_handle_t am_hc32_trng_init (am_hc32_trng_dev_t          *p_dev,
                                     const am_hc32_trng_devinfo_t *p_devinfo);

/**
 * \brief TRNG 去初始化
 *
 * \param[in] handle : 标准TRNG服务句柄
 *
 * \retval : 无
 *
 */
void am_hc32_trng_deinit (am_trng_handle_t handle);

/**
 * \brief 随机数生成
 *
 * \param[in] handle   : TRNG 标准服务句柄
 * \param[in] env_flag : 随机数产生环境 （上电产生、非上电产生）
 * \param[in] p_data   : 64位随机数缓存区
 *
 * \retval : AM_ERROR 随机数产生错误
 *           AM_OK    随机数产生成功
 */
int am_hc32_trng_generate (am_trng_handle_t handle,
                             uint8_t          env_flag,
                             uint32_t         *p_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_TRNG_H */

/* end of file */
