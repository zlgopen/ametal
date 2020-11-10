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
 * \brief LVD 驱动，服务 LVD 标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 20-04-27 zcb, Add callback function parameters
 * - 1.00 19-10-08
 * \endinternal
 */
#ifndef __AM_ZSL42x_LVD_H
#define __AM_ZSL42x_LVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_zsl42x_if_lvd
 * \copydoc am_zsl42x_lvd.h
 * @{
 */
/**
 * \brief LVD设备信息结构体
 */
typedef struct am_zsl42x_lvd_info {

    /** \brief LVD 寄存器块基地址 */
    uint32_t                     reg_base;

    /** \brief LVD 中断编号 */
    uint8_t                      inum;

    /** \brief LVD 触发条件 */
    uint32_t                     tri_type;

    /** \brief LVD 数字滤波时间 */
    int                          deb_time;

    /** \brief LVD 阈值电压 */
    int                          vids;

    /** \brief LVD 监测来源 */
    int                          src_sel;

    /** \brief LVD 触发动作 */
    int                          tri_act;

    /** \brief LVD 平台初始化函数，如打开时钟，配置引脚等工作 */
    void                       (*pfn_plfm_init)(void);

    /** \brief LVD 平台解初始化函数 */
    void                       (*pfn_plfm_deinit)(void);

}am_zsl42x_lvd_devinfo_t;

/**
 * \brief LVD设备实例
 */
typedef struct am_zsl42x_lvd_dev {

    /** \brief 指向LVD设备信息的指针 */
    const am_zsl42x_lvd_devinfo_t *p_devinfo;

    /** \brief 中断触发回调函数参数*/
    am_pfnvoid_t                 pfn_trigger_cb;

    /** \brief 中断触发回调函数参数*/
    void                        *p_arg;

} am_zsl42x_lvd_dev_t;

/** \brief LVD 标准服务操作句柄类型定义 */
typedef am_zsl42x_lvd_dev_t       *am_zsl42x_lvd_handle_t;

/**
 * \brief LVD 初始化
 *
 * \param[in] p_dev     : LVD 设备实例
 * \param[in] p_devinfo : LVD 设备信息实例
 *
 * \retval : 标准LVD服务句柄
 *
 */
am_zsl42x_lvd_handle_t am_zsl42x_lvd_init (am_zsl42x_lvd_dev_t           *p_dev,
                                       const am_zsl42x_lvd_devinfo_t *p_devinfo);

/**
 * \brief LVD 去初始化
 *
 * \param[in] handle : 标准LVD服务句柄
 *
 * \retval : 无
 *
 */
void am_zsl42x_lvd_deinit (am_zsl42x_lvd_handle_t     handle);

/**
 * \brief LVD 使能
 *
 * \param[in] handle : 标准LVD服务句柄
 *
 * \retval AM_OK     : 使能成功
 *         AM_ERROR  : 使能失败
 *
 */
int am_zsl42x_lvd_enable (am_zsl42x_lvd_handle_t      handle);

/**
 * \brief LVD 中断触发函数连接
 *
 * \param[in] handle     : 标准LVD服务句柄
 * \param[in] pfn_tri_cb : 用户定义的回调函数
 *
 * \retval AM_OK         : 连接成功
 * \retval AM_ERROR      : 连接失败
 */
int am_zsl42x_lvd_tri_set (am_zsl42x_lvd_handle_t     handle,
                         am_pfnvoid_t             pfn_tri_cb,
                         void                    *p_arg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42x_LVD_H */

/* end of file */
