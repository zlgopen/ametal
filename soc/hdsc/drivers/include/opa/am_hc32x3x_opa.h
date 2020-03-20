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
 * - 1.00 20-03-13
 * \endinternal
 */
#ifndef __AM_HC32_OPA_H
#define __AM_HC32_OPA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

#define  AM_HC32_OPA_CH1   (1U)
#define  AM_HC32_OPA_CH2   (2U)
#define  AM_HC32_OPA_CH3   (3U)

/**
 * \brief OPA工作模式选择
 */
#define AM_HC32_OPA_MODE_UNITY_GAIN         (1U)    /**< \biref 单位增益模式 */
#define AM_HC32_OPA_MODE_FORWARD_IN         (2U)    /**< \biref 正向输入模式 */
#define AM_HC32_OPA_MODE_OPPOSITE           (3U)    /**< \biref 反向输入模式 */
#define AM_HC32_OPA_MODE_TWO_DIFF           (4U)    /**< \biref 两差分运放模式 */
#define AM_HC32_OPA_MODE_UNIVERSAL          (5U)    /**< \biref 通用模式 */
#define AM_HC32_OPA_MODE_CAS_OPPO           (6U)    /**< \biref 级联反向模式 */
#define AM_HC32_OPA_MODE_CAS_FORWARD        (7U)    /**< \biref 级联正向模式 */


/**
 * \addtogroup am_hc32_if_opa
 * \copydoc am_hc32_opa.h
 * @{
 */
/**
 * \brief OPA设备信息结构体
 */
typedef struct am_hc32_opa_info {

    uint32_t    reg_base;  /**< \brief OPA寄存器块基地址 */

    uint8_t     opa_mode;  /**< \brief OPA模式 */

    uint8_t     gain;      /**< \brief OPA增益  */

    uint8_t     po;        /**< \brief IO端口与内部连接控制 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
}am_hc32_opa_devinfo_t;

/**
 * \brief OPA设备实例
 */
typedef struct am_hc32_opa_dev {

    /** \brief 指向OPA设备信息的指针 */
    const am_hc32_opa_devinfo_t *p_devinfo;

} am_hc32_opa_dev_t;

/** \brief opa标准服务操作句柄类型定义 */
typedef am_hc32_opa_dev_t *am_opa_handle_t;

/**
 * \brief OPA 初始化
 *
 * \param[in] p_dev     :OPA设备初始化实例
 * \param[in] p_devinfo :OPA设备信息初始化实例
 *
 * \retval :OPA标准句柄
 *
 */
am_opa_handle_t am_hc32_opa_init(am_hc32_opa_dev_t           *p_dev,
                                 const am_hc32_opa_devinfo_t *p_devinfo);

/**
 * \brief OPA 去始化
 *
 * \param[in] handle     :OPA标准设备服务句柄
 *
 * \retval :无
 *
 */
void am_hc32_opa_deinit (am_opa_handle_t handle);

/**
 * \brief OPA 使能
 *
 * \param[in] handle     :OPA标准设备服务句柄
 *
 * \retval :AM_OK     使能成功;
 *          -AM_ENXIO 无设备;
 */
error_t am_hc32_opa_enable (am_opa_handle_t handle);

/**
 * \brief OPA 模式选择
 *
 * \param[in] handle     :OPA标准设备服务句柄
 * \param[in] ch         :OPA通道
 * \param[in] mode       :OPA模式
 *
 * \retval :AM_OK      模式设置成功;
 *          -AM_ENXIO  无设备;
 *          -AM_EINVAL 无效参数；
 */
error_t am_hc32_opa_mode_set (am_hc32_opa_dev_t  *p_dev,
                              uint8_t             ch);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_OPA_H */

/* end of file */
