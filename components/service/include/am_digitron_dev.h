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
 * \brief 通用数码管设备管理
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_DEV_H
#define __AM_DIGITRON_DEV_H

#include "ametal.h"
#include "am_digitron_disp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_digitron_dev
 * @{
 */

/**
 * \brief 通用数码管操作接口
 * \note 若某一项功能不支持，则可以设置为NULL
 */
typedef struct am_digitron_disp_ops {

    /** \brief 设置解码函数接口 */
    int (*pfn_decode_set) (void *p_cookie, uint16_t (*pfn_decode) (uint16_t ch));

    /** \brief 设置数码管闪烁 */
    int (*pfn_blink_set)(void *p_cookie, int index, am_bool_t blink);

    /** \brief 显示段码 */
    int (*pfn_disp_at)(void *p_cookie, int index, uint16_t seg);

    /** \brief 显示字符 */
    int (*pfn_disp_char_at)(void *p_cookie, int index, const char ch);

    /** \brief 显示字符串 */
    int (*pfn_disp_str)(void *p_cookie, int index, int len, const char *p_str);

    /** \brief 清屏*/
    int (*pfn_clr)(void *p_cookie);

    /** \brief 使能 */
    int (*pfn_enable)(void *p_cookie);

    /** \brief 禁能 */
    int (*pfn_disable)(void *p_cookie);

} am_digitron_disp_ops_t;

/**
 * \brief 通用数码管显示器信息（一个数码管显示器可能包含一个或多个数码管位）
 */
typedef struct am_digitron_devinfo {

    uint8_t         id;        /**< \brief 数码管显示器编号 */

} am_digitron_devinfo_t;

/**
 * \brief 通用数码管设备
 */
typedef struct am_digitron_dev {
    const am_digitron_disp_ops_t *p_ops;     /**< \brief 驱动函数  */
    void                         *p_cookie;  /**< \brief 驱动函数参数 */
    const am_digitron_devinfo_t  *p_info;    /**< \brief 数码管显示器编号 */
    struct am_digitron_dev       *p_next;    /**< \brief 下一个数码管显示器 */
} am_digitron_dev_t;

/** 
 * \brief 数码管设备库管理初始化
 * \retval AM_OK : 数码管设备库管理初始化库初始化成功
 */
int am_digitron_dev_lib_init (void);

/**
 * \brief 添加一个数码管设备
 *
 * \param[in] p_dd     : 数码管设备实例
 * \param[in] p_info   ：数码管设备信息（包含数码管设备的编号等）
 * \param[in] p_ops    : 数码管设备的驱动函数
 * \param[in] p_cookie : 驱动函数的参数
 *
 * \retval AM_OK      : 添加成功
 * \retval -AM_EINVAL ：添加失败，参数存在错误
 * \retval -AM_EPERM  : 添加失败，该编号的设备已经存在
 */
int am_digitron_dev_add (am_digitron_dev_t            *p_dd,
                         const am_digitron_devinfo_t  *p_info,
                         const am_digitron_disp_ops_t *p_ops,
                         void                         *p_cookie);
/**
 * \brief 删除一个数码管设备
 *
 * \param[in] p_dd     : 数码管设备实例
 *
 * \retval AM_OK      : 删除成功
 * \retval -AM_EINVAL ：删除失败，参数存在错误
 * \retval -AM_ENODEV : 删除失败，无此参数
 */
int am_digitron_dev_del (am_digitron_dev_t *p_dd);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_DEV_H */

/* end of file */
