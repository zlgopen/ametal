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
 * \brief OPA驱动，服务OPA标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-08
 * \endinternal
 */
#ifndef __AM_ZSL42x_OPA_H
#define __AM_ZSL42x_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \brief OPA工作模式选择
 */
#define AM_ZSL42x_OPA_GENERAL  0 /**< \brief 通用模式 */
#define AM_ZSL42x_OPA_DAC      1 /**< \brief DAC跟随模式 */

/**
 * \addtogroup am_zsl42x_if_opa
 * \copydoc am_zsl42x_opa.h
 * @{
 */
/**
 * \brief OPA设备信息结构体
 */
typedef struct am_zsl42x_opa_info {

    uint32_t    reg_base;  /**< \brief OPA寄存器块基地址 */

    uint8_t     clk_sel;   /**< \brief OPA自动校准脉宽*/

    uint8_t     az_way;    /**< \brief OPA校准方式 */

    uint8_t     opa_mode;  /**< \brief OPA工作模式 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
}am_zsl42x_opa_devinfo_t;

/**
 * \brief LVD设备实例
 */
typedef struct am_zsl42x_opa_dev {

    /** \brief 指向OPA设备信息的指针 */
    const am_zsl42x_opa_devinfo_t *p_devinfo;

} am_zsl42x_opa_dev_t;

/** \brief opa标准服务操作句柄类型定义 */
typedef am_zsl42x_opa_dev_t *am_opa_handle_t;

/**
 * \brief OPA 初始化
 *
 * \param[in] p_dev     :OPA设备初始化实例
 * \param[in] p_devinfo :OPA设备信息初始化实例
 *
 * \retval :无
 *
 */
am_opa_handle_t am_zsl42x_opa_init(am_zsl42x_opa_dev_t           *p_dev,
                                   const am_zsl42x_opa_devinfo_t *p_devinfo);

/**
 * \brief OPA 去始化
 *
 * \param[in] handle     :OPA标准设备服务句柄
 *
 * \retval :无
 *
 */
void am_zsl42x_opa_deinit (am_opa_handle_t handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_OPA_H */

/* end of file */
