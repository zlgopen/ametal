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
 * \brief CAN驱动层实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-27  yrh, first implementation
 * \endinternal
 */

#ifndef __AM_ZMF237_CAN_H
#define __AM_ZMF237_CAN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_can.h"
#include "hw/amhw_zmf237_can.h"

/**
 * \addtogroup am_zmf237_if_can
 * \copydoc am_zmf237_can.h
 * @{
 */

#define AM_ZMF237_FILTER_RTR            (0x1 << 31)
#define AM_ZMF237_FILTER_IDE            (0x1 << 30)
#define AM_ZMF237_FILTER_IDE_0_ID_START (18)

/**
 * \brief 中断回调信息
 */
typedef struct am_zmf237_can_intcb_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_callback;

    /** \brief 回调函数的参数 */
    void         *p_arg;
} am_zmf237_can_intcb_info_t;

/**
 * \brief CAN 设备信息
 */
typedef struct am_zmf237_can_devinfo {

    /** \brief 寄存器基地址 */
    uint32_t                       regbase;

    /** \brief 中断号 */
    uint32_t                       int_num;

    /** \brief 运行类型 */
    amhw_zmf237_can_type_t         type;

    /** \brief 回调信息 */
    am_zmf237_can_intcb_info_t    *p_intcb_info;

    /** \brief 回调信息内存大小 */
    uint8_t                        p_intcb_num;

    /** \brief 平台初始化函数 */
    void                         (*pfn_plfm_init)(void);

    /** \brief 平台去初始化函数 */
    void                         (*pfn_plfm_deinit)(void);
} am_zmf237_can_devinfo_t;

/**
 * \brief 设备
 */
typedef struct am_zmf237_can_dev {
    am_can_serv_t                   handle;

    amhw_zmf237_can_mode_t          mode;

    const am_zmf237_can_devinfo_t  *p_devinfo;
} am_zmf237_can_dev_t;

/**
 * \brief CAN初始化
 *
 * \param[in] p_dev     : 设备实例
 * \param[in] p_devinfo : 设备信息
 *
 * \return handle
 */
am_can_handle_t am_zmf237_can_init (am_zmf237_can_dev_t           *p_dev,
                                    const am_zmf237_can_devinfo_t *p_devinfo);

/**
 * \brief 解初始化
 *
 * \param[in] p_dev     : 设备实例
 *
 * \return 无
 */
void am_zmf237_can_deinit (am_can_handle_t handle);

/**
 * @} am_s32k14x_if_can
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_S32K14X_CAN_H */

/* end of file */

