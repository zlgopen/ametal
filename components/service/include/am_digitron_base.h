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
 * \brief 通用数码管基础信息 & 工具
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_BASE_H
#define __AM_DIGITRON_BASE_H

#include "ametal.h"
#include "am_digitron_disp.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_if_digitron_base
 * @{
 */

/**
 * \name 数码管扫描方式
 *
 *     对于绝大多数应用场合，仅存在一行数码管，且每次扫描一个数码管。
 * 即：行数为1，扫描方式为列扫描。扫描方式是由硬件决定的。
 *
 * @{
 */

/** \brief 按行扫描，每次扫描一行数码管  */
#define AM_DIGITRON_SCAN_MODE_ROW      0

/** \brief 按列扫描，每次扫描一列数码管  */
#define AM_DIGITRON_SCAN_MODE_COL      1

/** @} */

/**
 * \brief 数码管基础信息
 */
typedef struct am_digitron_base_info {
    uint8_t        num_segment;       /**< \brief 数码管段数 */
    uint8_t        num_rows;          /**< \brief 数码管矩阵的行数 */
    uint8_t        num_cols;          /**< \brief 数码管矩阵的列数 */
    uint8_t        scan_mode;         /**< \brief 扫描方式：按行扫描或按列扫描 */
    am_bool_t      seg_active_low;    /**< \brief 数码管段端的极性      */
    am_bool_t      com_active_low;    /**< \brief 数码管公共端的极性 */
} am_digitron_base_info_t;

/**
 * \brief 获取数码管的段数
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的段数
 */
am_static_inline
uint8_t am_digitron_num_segment_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->num_segment;
    }
    return 0;
}

/**
 * \brief 获取数码管的行数
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的行数
 */
am_static_inline
uint8_t am_digitron_num_row_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->num_rows;
    }
    return 0;
}

/**
 * \brief 获取数码管的行数
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的行数
 */
am_static_inline
uint8_t am_digitron_num_col_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->num_cols;
    }
    return 0;
}

/**
 * \brief 获取数码管的扫描方式
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的扫描方式（按行扫描或按列扫描）
 */
am_static_inline
uint8_t am_digitron_scan_mode_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->scan_mode;
    }
    return 0;
}

/**
 * \brief 数码管段是否低电平激活
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的段数
 */
am_static_inline
am_bool_t am_digitron_seg_active_low_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->seg_active_low;
    }
    return AM_FALSE;
}

/**
 * \brief 数码管位是否低电平激活
 * \param[in] p_info : 数码管基础信息
 * \return 数码管的段数
 */
am_static_inline
am_bool_t am_digitron_com_active_low_get (const am_digitron_base_info_t *p_info)
{
    if (p_info != NULL) {
        return p_info->com_active_low;
    }
    return AM_FALSE;
}

/**
 * \brief 数码管段码发送器（段码单独发送）
 */
typedef struct am_digitron_base_sender_seg {

    /** \brief 段码发送，一次可能发送多个数码管的位码 */
    int  (*pfn_seg_send) (void    *p_cookie, 
                          void    *p_seg, 
                          uint8_t  num_segment, 
                          uint8_t  num_digitron);

    /** \brief 段码发送函数的自定义参数 */
    void  *p_cookie;

} am_digitron_base_sender_seg_t;

/**
 * \brief 数码管位码发送器（位单独操作）
 */
typedef struct am_digitron_base_sender_com {

    /** \brief 位码发送，一次可能选中多个数码管位（静态扫描数码管使用） */
    int (*pfn_com_send) (void *p_cookie, uint32_t com);

    /** \brief 位码发送函数的自定义参数 */
    void *p_cookie;

} am_digitron_base_sender_com_t;

/**
 * \brief 数码管位选择器（位单独操作）
 */
typedef struct am_digitron_base_selector_com {

    /** \brief 位选，仅选择的数码管位有效（动态扫描使用），其余位无效 */
    int (*pfn_com_sel) (void *p_cookie, int idx);
    
    /** \brief 位选函数的自定义参数 */
    void *p_cookie;

} am_digitron_base_selector_com_t;

/**
 * \brief 数码管位码和段码发送器（位码和段码同时发送）
 */
typedef struct am_digitron_base_sender_data {

    /** \brief 位码和段码同时发送 */
    int (*pfn_data_send) (void    *p_cookie,
                          void    *p_seg,
                          uint8_t  num_segment,
                          uint8_t  num_digitron,
                          uint32_t com);

    /** \brief 数据发送函数的自定义参数 */
    void *p_cookie;

} am_digitron_base_sender_data_t;


/**
 * \brief 数码管段码发送（段码单独发送）
 *
 * \param[in] p_sender     : 段码发送器
 * \param[in] p_seg        : 段码缓冲区
 * \param[in] num_segment  : 单个数码管的段数目
 * \param[in] num_digitron : 当前扫描的数码管个数
 *
 * \return 标准错误号
 */
am_static_inline
int am_digitron_seg_send  (am_digitron_base_sender_seg_t *p_sender,
                           void                          *p_seg,
                           uint8_t                        num_segment,
                           uint8_t                        num_digitron)

{
    if (p_sender && p_sender->pfn_seg_send) {

        return p_sender->pfn_seg_send(p_sender->p_cookie,
                               p_seg,
                               num_segment,
                               num_digitron);
    }
    return -AM_ENOTSUP;
}

/**
 * \brief 数码管位码发送（位码单独发送）
 *
 * \param[in] p_sender : 位码发送器
 * \param[in] com      : 位码
 *
 * \return 标准错误号
 */
am_static_inline
int am_digitron_com_send (am_digitron_base_sender_com_t *p_sender,
                          uint32_t                       com)
{
    if (p_sender && p_sender->pfn_com_send) {
        return p_sender->pfn_com_send(p_sender->p_cookie, com);
    }
    return -AM_ENOTSUP;
}

/**
 * \brief 数码管位码选择
 *
 * \param[in] p_sender : 位码选择器
 * \param[in] idx      : 位选索引
 *
 * \return 标准错误号
 */
am_static_inline
int am_digitron_com_sel (am_digitron_base_selector_com_t *p_sel, int idx)
{
    if (p_sel && p_sel->pfn_com_sel) {
        return p_sel->pfn_com_sel(p_sel->p_cookie, idx);
    }
    return -AM_ENOTSUP;
}

/**
 * \brief 位码和段码同时发送（仅供少部分位码和段码必须同时发送的硬件电路使用）
 *
 * \param[in] p_sender     : 数据发送器
 * \param[in] p_seg        : 扫描段码缓冲区
 * \param[in] num_segment  : 段码数目
 * \param[in] num_digitron : 当前扫描的数码管个数
 * \param[in] com          : 位码
 *
 * \return 标准错误号
 */
am_static_inline
int am_digitron_data_send (am_digitron_base_sender_data_t *p_sender,
                           void                           *p_seg,
                           uint8_t                         num_segment,
                           uint8_t                         num_digitron,
                           uint32_t                        com)

{
    if (p_sender && p_sender->pfn_data_send) {

        return p_sender->pfn_data_send(p_sender->p_cookie,
                                       p_seg,
                                       num_segment,
                                       num_digitron,
                                       com);
    }
    return -AM_EINVAL;
}

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_BASE_H */

/* end of file */
