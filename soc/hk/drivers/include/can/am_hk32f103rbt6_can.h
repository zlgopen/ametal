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
 * \brief CAN驱动设备信息
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-31  zc, first implementation
 * \endinternal
 */

#ifndef __AM_HK32F103RBT6_CAN_H
#define __AM_HK32F103RBT6_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_can.h"
#include "amhw_hk32f103rbt6_can.h"

/**
 * \addtogroup am_hk32f103rbt6_if_can
 * \copydoc am_hk32f103rbt6_can.h
 * @{
 */


/**
 * \brief can 初始化配置信息
 */
typedef struct am_hk32f103rbt6_can_init_info {

    /** \brief 0 普通模式   1 回环模式 */
    amhw_hk32f103rbt6_can_test_mode_t     can_mode;

    /** \brief 时间触发通信模式  0 非触发  1 触发 */
    am_bool_t   can_ttcm;

    /** \brief 软件自动离线管理
     *   0 软件对MCR的INRQ位进行置‘1’ 后 清‘0’ 一旦硬件检测到128次11位连续隐性位则退出离线模式
     *   1 一旦硬件检测到128次11位连续隐性位则退出离线模式
     */
    am_bool_t   can_abom;

    /** \brief 自动唤醒模式
     *   0 睡眠模式通过清除CAN_MCR的SLEEP位  由软件唤醒
     *   1 睡眠模式通过检测CAN报文，由硬件自动唤醒。唤醒的同时，硬件自动对CAN_MSR 的SLEEP和SLAK清‘0’
     */
    am_bool_t   can_awum;

    /** \brief 禁止报文自动重传
     *   0 硬件在发送报文失败时会一直重传到发送成功
     *   1 CAN报文只发送一次
     */
    am_bool_t   can_nart;

    /** \brief 接收FIFO锁定模式
     *   0 在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有的报文
     *   1 在接受溢出时FIFO被锁定，当接收FIFO的报文未被读出，下一个收到的报文会被丢弃
     */
    am_bool_t   can_rflm;

    /** \brief 发送FIFO优先级
     *   0 优先级由报文的标识符来决定
     *   1 优先级由发送请求的顺序来决定
     */
    am_bool_t   can_txfp;

}am_hk32f103rbt6_can_init_info_t;

/**<brief 滤波器参数配置信息  */
typedef struct {
    uint8_t  can_fliter_ide;         /** <brief 标识符选择 */
    uint8_t  can_fliter_rtr;         /** <brief 远程发送请求 */
    uint16_t can_fliter_fifo;        /** <brief 关联FIFO */
    uint8_t  can_filter_num;         /** <brief 滤波器组*/
    uint8_t  can_filter_mode;        /** <brief 滤波器模式 */
    uint8_t  can_filter_scale;       /** <brief 滤波器位宽*/
    uint8_t  filter_state;           /** <brief 滤波器激活状态 */

}amhw_hk32f103rbt6_can_filter_t;

/**
 * \brief 中断回调信息
 */
typedef struct am_hk32f103rbt6_can_intcb_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void         *p_arg;
} am_hk32f103rbt6_can_intcb_info_t;

/**
 * \brief CAN 设备信息
 */
typedef struct am_hk32f103rbt6_can_devinfo {

    /** \brief 寄存器基地址 */
    uint32_t                         regbase;

    /** \brief 中断号 */
    uint32_t                         int_num;

    /** \brief 回调信息 */
    am_hk32f103rbt6_can_intcb_info_t      *p_intcb_info;

    /** \brief 回调信息内存大小 */
    uint8_t                          p_intcb_num;

    /** \brief 平台初始化函数 */
    void                           (*pfn_plfm_init)(void);

    /** \brief 平台去初始化函数 */
    void                           (*pfn_plfm_deinit)(void);

    amhw_hk32f103rbt6_can_filter_t       *filter;

    am_hk32f103rbt6_can_init_info_t      *init_info;

} am_hk32f103rbt6_can_devinfo_t;

/**
 * \brief 设备
 */
typedef struct am_hk32f103rbt6_can_dev {
    am_can_serv_t                   handle;

    /** \brief 工作模式 */
    amhw_hk32f103rbt6_can_mode_t          mode;

    /** \brief 过滤器匹配序号*/
    uint8_t                         fmi;


    const am_hk32f103rbt6_can_devinfo_t  *p_devinfo;
} am_hk32f103rbt6_can_dev_t;

/**
 * \brief CAN初始化
 *
 * \param[in] p_dev     : 设备实例
 * \param[in] p_devinfo : 设备信息
 *
 * \return handle
 */
am_can_handle_t am_hk32f103rbt6_can_init (am_hk32f103rbt6_can_dev_t           *p_dev,
                                    const am_hk32f103rbt6_can_devinfo_t *p_devinfo);

/**
 * \brief 解初始化
 *
 * \param[in] p_dev     : 设备实例
 *
 * \return 无
 */
void am_hk32f103rbt6_can_deinit (am_can_handle_t handle);

/**
 * @} am_s32k14x_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_S32K14X_CAN_H */

/* end of file */

