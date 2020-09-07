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
 * \brief VC驱动，服务VC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 20-04-27 zcb, Add callback function parameters
 * - 1.00 19-09-27
 * \endinternal
 */
#ifndef __AM_HC32_VC_H
#define __AM_HC32_VC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "hw/amhw_hc32_vc.h"

/**
 * \addtogroup am_hc32_if_vc
 * \copydoc am_hc32_vc.h
 * @{
 */

/**
 * \brief VC设备信息结构体
 */
typedef struct am_hc32_vc_info {

    /** \brief VC 寄存器块基地址 */
    uint32_t    vc_reg_base;

    /** \brief VC 中断编号 */
    uint8_t     vc_inum;

    /** \brief VC 通道 选择*/
    uint32_t    vc_chan;

    /** \brief VC 参考 选择*/
    uint32_t    vc_vref;

    /** \brief VC 迟滞时间 选择*/
    uint32_t    vc_delay;

    /** \brief VC 功耗 选择*/
    uint32_t    vc_bias;

    /** \brief VC 滤波时间 选择 如不使用滤波则为NULL*/
    uint32_t    vc_flt_time;

    /** \brief VC p端输入 选择*/
    uint32_t    vc_p_in;

    /** \brief VC n端输入 选择*/
    uint32_t    vc_n_in;

    /** \brief VC 输出配置*/
    uint32_t    vc_out_cfg;

    /** \brief VC 输出触发中断类型选择*/
    uint32_t    vc_out_int;

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init) (void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit) (void);

}am_hc32_vc_devinfo_t;

/**
 * \brief VC设备实例
 */
typedef struct am_hc32_vc_dev {

    /** \brief 指向VC设备信息的指针 */
    const am_hc32_vc_devinfo_t *p_devinfo;

    /** \brief 当前转换的通道 */
    uint32_t                    chan;

    /** \brief 中断触发回调 函数 */
    am_pfnvoid_t                pfn_trigger_cb;

    /** \brief 中断触发回调 函数参数 */
    void                       *p_arg;

} am_hc32_vc_dev_t;

/** \brief VC 标准服务操作句柄类型定义 */
typedef am_hc32_vc_dev_t *am_hc32_vc_handle_t;

/**
 * \brief VC 初始化
 *
 * \param[in] p_dev     : VC设备实例
 * \param[in] p_devinfo : VC设备信息实例
 *
 * \retval 标准VC服务句柄
 */
am_hc32_vc_handle_t am_hc32_vc_init (am_hc32_vc_dev_t           *p_dev,
                                     const am_hc32_vc_devinfo_t *p_devinfo);

/**
 * \brief VC 去初始化
 *
 * \param[in] handle    : 标准VC服务句柄
 */
void am_hc32_vc_deinit (am_hc32_vc_handle_t        handle);

/**
 * \brief 中断触发函数连接
 *
 * \param[in] handle    : 标准VC服务句柄
 * \param[in] pfn_cb    : 用户定义的中断触发回调函数
 * \param[in] p_arg     : 用户定义的中断触发回调函数参数
 *
 * \retval AM_ERROR 连接失败
 * \retval AM_ERROR 连接成功
 *
 */
int am_hc32_tri_set (am_hc32_vc_handle_t           handle,
                     am_pfnvoid_t                  pfn_cb,
                     void                         *p_arg);

/**
 * \brief VC通道使能
 *
 * \param[in] handle    : 标准VC服务句柄
 *
 * \retval AM_ERROR 使能失败
 * \retval AM_ERROR 使能成功
 */
int am_hc32_vc_chan_enable (am_hc32_vc_handle_t    handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_VC_H */

/* end of file */
