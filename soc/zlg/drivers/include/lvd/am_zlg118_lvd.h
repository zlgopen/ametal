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
 * \brief LVD驱动，服务LVD标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-08
 * \endinternal
 */
#ifndef __AM_ZLG118_LVD_H
#define __AM_ZLG118_LVD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_zlg118_if_lvd
 * \copydoc am_zlg118_lvd.h
 * @{
 */
/**
 * \brief LVD设备信息结构体
 */
typedef struct am_zlg118_lvd_info {

    uint32_t    reg_base;  /**< \brief lvd寄存器块基地址 */

    uint8_t     inum;      /**< \brief lvd中断编号 */

    uint32_t    tri_type;  /**< \brief lvd触发条件 */

    int         deb_time;  /**< \brief lvd数字滤波时间 */

    int         vids;      /**< \brief lvd阈值电压 */

    int         src_sel;   /**< \brief lvd监测来源 */

    int         tri_act;   /**< \brief lvd触发动作 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);

}am_zlg118_lvd_devinfo_t;

/**
 * \brief LVD设备实例
 */
typedef struct am_zlg118_lvd_dev {

    /** \brief 指向LVD设备信息的指针 */
    const am_zlg118_lvd_devinfo_t *p_devinfo;

    /** \brief 中断触发回调 函数*/
    void (*pfn_trigger_cb)(void *);

} am_zlg118_lvd_dev_t;

/** \brief lvd标准服务操作句柄类型定义 */
typedef am_zlg118_lvd_dev_t *am_lvd_handle_t;

/**
 * \brief lvd 初始化
 *
 * \param[in] p_dev     : LVD 设备实例
 * \param[in] p_devinfo : LVD 设备信息实例
 *
 * \retval : 标准LVD服务句柄
 *
 */
am_lvd_handle_t  am_zlg118_lvd_init (am_zlg118_lvd_dev_t           *p_dev,
                                     const am_zlg118_lvd_devinfo_t *p_devinfo);

/**
 * \brief lvd 去初始化
 *
 * \param[in] handle : 标准LVD服务句柄
 *
 * \retval : 无
 *
 */
void am_zlg118_lvd_deinit (am_lvd_handle_t handle);

/**
 * \brief lvd 使能
 *
 * \param[in] handle : 标准LVD服务句柄
 *
 * \retval AM_OK    : 使能成功
 *         AM_ERROR : 使能失败
 *
 */
int am_zlg118_lvd_enable (am_lvd_handle_t handle);

/**
 * \brief 中断触发函数连接
 *
 * \param[in] handle     : 标准LVD服务句柄
 * \param[in] pfn_tri_cb : 用户定义的回调函数
 *
 * \retval AM_ERROR 连接失败
 * \retval AM_OK    连接成功
 */
int am_zlg118_lvd_tri_set (am_lvd_handle_t handle,
                           void (*pfn_tri_cb) (void *));

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZLG118_LVD_H */

/* end of file */
