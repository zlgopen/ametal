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
 * \brief 标准的LED设备管理
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_LED_DEV_H
#define __AM_LED_DEV_H

#include "ametal.h"
#include "am_led.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_led_dev
 * @{
 */

/**
 * \brief LED驱动函数
 */
typedef struct am_led_drv_funcs {

    /* 设置LED的状态 */
    int (*pfn_led_set)   (void *p_cookie, int id, am_bool_t on);

    /* 翻转LED的状态 */
    int (*pfn_led_toggle)(void *p_cookie, int id);

} am_led_drv_funcs_t;

/**
 * \brief 通用LED服务信息
 */
typedef struct am_led_servinfo {
    int         start_id;        /**< \brief 本设备提供的LED服务的起始编号 */
    int         end_id;          /**< \brief 本设备提供的LED服务的结束编号 */
} am_led_servinfo_t;

/**
 * \brief 通用LED设备
 */
typedef struct am_led_dev {
    const am_led_drv_funcs_t     *p_funcs;   /**< \brief 驱动函数            */
    void                         *p_cookie;  /**< \brief 驱动函数参数  */
    const am_led_servinfo_t      *p_info;    /**< \brief LED服务信息     */
    struct am_led_dev            *p_next;    /**< \brief 下一个LED设备 */
} am_led_dev_t;

/** 
 * \brief LED设备库管理初始化
 * \retval AM_OK : LED设备库管理初始化库初始化成功
 */
int am_led_dev_lib_init (void);

/**
 * \brief 添加一个LED设备
 *
 * \param[in] p_dev    : LED设备实例
 * \param[in] p_info   ：LED设备服务信息
 * \param[in] p_funcs  : LED设备的驱动函数
 * \param[in] p_cookie : 驱动函数的参数
 *
 * \retval AM_OK      : 添加成功
 * \retval -AM_EINVAL ：添加失败，参数存在错误
 * \retval -AM_EPERM  : 添加失败，该设备对应的LED编号已经存在
 */
int am_led_dev_add (am_led_dev_t             *p_dev,
                    const am_led_servinfo_t  *p_info,
                    const am_led_drv_funcs_t *p_funcs,
                    void                     *p_cookie);
/**
 * \brief 删除一个LED设备
 *
 * \param[in] p_dev     : LED设备实例
 *
 * \retval AM_OK      : 删除成功
 * \retval -AM_EINVAL ：删除失败，参数存在错误
 * \retval -AM_ENODEV : 删除失败，无此参数
 */
int am_led_dev_del (am_led_dev_t *p_dev);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LED_DEV_H */

/* end of file */
