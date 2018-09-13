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
 * \brief 通用动态扫描类数码管
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_SCAN_H
#define __AM_DIGITRON_SCAN_H

#include "ametal.h"
#include "am_digitron_disp.h"
#include "am_softimer.h"
#include "am_digitron_dev.h"
#include "am_digitron_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_digitron_scan
 * @{
 */

/**
 * \brief 动态扫描类数码管信息
 */
typedef struct am_digitron_scan_devinfo {

    /** \brief 标准数码管设备信息，包含ID号 */
    am_digitron_devinfo_t devinfo; 

    /** \brief 整个数码管的扫描频率，一般 50Hz */
    uint8_t               scan_freq;
 
    /** \brief 一个闪烁周期内，点亮的时间，如 500ms */
    uint16_t              blink_on_time;
    
    /** \brief 一个闪烁周期内，熄灭的时间，如 500ms */
    uint16_t              blink_off_time; 

    /**
     * \brief 数码管显存，具体类型与数码管段数相关
     *
     * 数码管段数 1 ~ 8， 缓冲区类型为 uint8_t，大小与数码管总个数一致
     * 数码管段数 8 ~ 16，缓冲区类型为 uint16_t，大小与数码管总个数一致
     */
    void                  *p_disp_buf;

    /**
     * \brief 数码管扫描显存，用于存放单次扫描的具体类型与数码管段数相关
     *
     * 数码管段数 1 ~ 8， 缓冲区类型为 uint8_t，大小与单次数码管扫描的个数一致
     * 数码管段数 8 ~ 16，缓冲区类型为 uint16_t，大小与单次数码管扫描的个数一致
     *
     * 如按行扫描，则其大小与 num_rows 相同；
     * 如按列扫描，则其大小与 num_cols 相同；
     *
     * 对于绝大部分应用电路，一次只能扫描一个数码管，缓存大小为1
     */
    void                  *p_scan_buf;

} am_digitron_scan_devinfo_t;


/**
 * \brief 驱动函数（段码和位选分开进行），应该由具体驱动提供
 *
 * 段码和位码分开发送，自动处理消影问题，在消影空闲期，执行空间回调函数。
 *
 */
typedef struct am_digitron_scan_ops_separate {
   am_digitron_base_sender_seg_t      *p_send_seg; /**< \brief 段码发送器 */
   am_digitron_base_selector_com_t    *p_sel_com;  /**< \brief 位选器 */
} am_digitron_scan_ops_separate_t;

/**
 * \brief 驱动函数（段码和位选同时进行），应该由具体驱动提供
 *
 *     段码和位码同时发送，需要自行处理消影问题，新的数码管激活后，段选和位选均
 * 处于有效状态
 */
typedef struct am_digitron_scan_ops_unite {
    am_digitron_base_sender_data_t   *p_send_data; /**< \brief 数码发送器 */
} am_digitron_scan_ops_unite_t;

/**
 * \brief 空闲回调函数（可用于引脚复用）
 *
 * 在数码管位开始扫描时，会调用该回调函数，以用于数码管与按键复用等灵活的应用场合。
 * 在回调函数执行期间，数码管位选处于有效状态，段选处于无效状态
 *
 * \param[in] p_cookie : 驱动函数自定义参数
 * \param[in] scan_idx : 当前所处的位选
 *
 * \return 无
 */
typedef void (*am_digitron_scan_cb_t) (void *p_cookie, int scan_idx);

/**
 * \brief 通用动态扫描数码管设备
 */
typedef struct am_digitron_scan_dev {

    am_digitron_dev_t isa;             /**< \brief 是标准的数码管设备      */
    am_softimer_t     timer;           /**< \brief 使用软件定时器          */

    uint8_t           scan_idx;        /**< \brief 当前扫描索引            */
    uint8_t           scan_interval;   /**< \brief 扫描时间间隔            */
    uint8_t           num_scan;        /**< \brief 单次扫描的个数，一般为1 */
    uint8_t           num_digitron;    /**< \brief 数码管个数              */
    uint32_t          blink_flags;     /**< \brief 闪烁标记(bit map)       */
    uint16_t          blink_cnt;       /**< \brief 闪烁计数                */

    uint16_t  (*pfn_decode)(uint16_t code);

    const void                       *pfn_buf_access; 
    am_bool_t                         is_separate;
    void                             *p_ops;
    am_digitron_scan_cb_t             pfn_black_cb;
    void                             *p_arg;
    const am_digitron_scan_devinfo_t *p_info;
    const am_digitron_base_info_t    *p_baseinfo;
} am_digitron_scan_dev_t;

/**
 * \brief 动态扫描类数码管初始化
 *
 * \param[in] p_dev       : 动态扫描类数码管设备实例
 * \param[in] p_info      : 动态扫描类数码管设备实例信息
 * \param[in] is_separate : 段码和位码是否为独立发送， 决定了 p_ops 的实际类型
 *                         - AM_TRUE， 类型为：am_digitron_scan_ops_unite_t *
 *                         - AM_FALSE，类型为：am_digitron_scan_ops_separate_t *
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL ：初始化失败，参数存在错误
 */
int am_digitron_scan_init (am_digitron_scan_dev_t           *p_dev,
                           const am_digitron_scan_devinfo_t *p_info,
                           const am_digitron_base_info_t    *p_baseinfo,
                           am_bool_t                         is_separate,
                           void                             *p_ops);

/**
 * \brief 设置数码管扫描回调函数（仅可设置一个）
 *
 *     当一个新的数码管位开始扫描时，会调用该回调函数
 * 
 * \param[in] p_dev  : 动态扫描类数码管设备实例
 * \param[in] pfn_cb : 回调函数
 * \param[in] p_arg  : 回调函数用户参数
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_EINVAL ：设置失败，参数存在错误
 */
int am_digitron_scan_cb_set (am_digitron_scan_dev_t *p_dev,
                             am_digitron_scan_cb_t   pfn_cb,
                             void                   *p_arg);

/**
 * \brief 动态扫描类数码管解初始化
 *
 * \param[in] p_dev   : 动态扫描类数码管设备实例
 *
 * \retval AM_OK      : 解初始化成功
 * \retval -AM_EINVAL ：解初始化失败，参数存在错误
 */
int am_digitron_scan_deinit (am_digitron_scan_dev_t *p_dev);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_SCAN_H */

/* end of file */
