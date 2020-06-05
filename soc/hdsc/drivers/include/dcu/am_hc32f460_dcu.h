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
 * \brief DCU驱动，服务DCU标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-27
 * \endinternal
 */
#ifndef __AM_HC32_DCU_H
#define __AM_HC32_DCU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"



/**
 * \addtogroup am_hc32f460_if_dcu
 * \copydoc am_hc32f460_dcu.h
 * @{
 */
/**
 * \brief dcu设备信息结构体
 */
typedef struct am_hc32f460_dcu_info {

    uint32_t    reg_base;  /**< \brief DCU寄存器块基地址 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
} am_hc32f460_dcu_devinfo_t;

/**
 * \brief DCU设备实例
 */
typedef struct am_hc32f460_dcu_dev {

    /** \brief 指向DCU设备信息的指针 */
    const am_hc32f460_dcu_devinfo_t *p_devinfo;

} am_hc32f460_dcu_dev_t;

/** \brief dcu标准服务操作句柄类型定义 */
typedef am_hc32f460_dcu_dev_t *am_dcu_handle_t;

/**
 * \brief DCU 初始化
 *
 * \param[in] p_dev     :DCU设备初始化实例
 * \param[in] p_devinfo :DCU设备信息初始化实例
 *
 * \retval :无
 *
 */
am_aes_handle_t am_hc32f460_dcu_init(am_hc32f460_dcu_dev_t           *p_dev,
                                     const am_hc32f460_dcu_devinfo_t *p_devinfo);

/**
 * \brief DCU 去始化
 *
 * \param[in] handle     :AES标准设备服务句柄
 *
 * \retval :无
 *
 */
void am_hc32f460_dcu_deinit (am_dcu_handle_t handle);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32F460_DCU_H */

/* end of file */
