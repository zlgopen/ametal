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
 * \brief 循环冗余校验(CRC) 软件实现
 *
 * \internal
 * \par modification history
 * - 1.00 18-01-19  tee, first implementation
 * \endinternal
 */

#ifndef __AM_CRC_SOFT_H
#define __AM_CRC_SOFT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_crc_soft
 * \copydoc am_crc_soft.h
 * @{
 */
#include "am_common.h"
#include "am_crc.h"

/**
 * \brief CRC TABLE表
 *
 *     为了加快软件 CRC 计算的速度，使用了查表法，因此需要在初始化一个软件CRC计
 * 算器时(\sa am_crc_soft_init())提供一个用于CRC计算的表。该表的具体内容与CRC模
 * 型中：width、poly 和 refin 三个成员的值相关，只要一个模型中这三个值相等，它
 * 们就可以使用同一个 table 表。
 */
typedef struct am_crc_table {

    uint8_t     width;                     /**< \brief CRC宽度                */
    uint32_t    poly;                      /**< \brief CRC生成多项式          */
    am_bool_t   refin;                     /**< \brief 输入字节bit反序        */

    /**
     * \brief 指向实际 TABLE 数据信息 的指针
     *
     * TBALE的长度固定为256，其实际类型与 width的值相关
     *     - 8位及8位以下，p_data 指向的  TABLE 原型应该为： uint8_t data[256]
     *     - 9  至16位，    p_data 指向的  TABLE 原型应该为：uint16_t data[256]
     *     - 17 至32位，   p_data 指向的  TABLE 原型应该为：uint32_t data[256]
     */
    const void *p_data;

} am_crc_table_t;

/**
 * \brief 软件CRC
 */
typedef struct am_crc_soft {

    /** \brief 软件CRC能够提供标准的CRC计算服务    */
    am_crc_serv_t            serv;

    /** \brief 软件CRC使用的TABLE索引表 */
    const am_crc_table_t    *p_table;

    /* 当前的 CRC 计算模型  */
    am_crc_pattern_t        *p_pattern;

    /** \brief CRC计算值          */
    uint32_t                 value;

} am_crc_soft_t;

/**
 * \brief 一个通用的 CRC table 创建函数
 *
 *   为了加快软件 CRC 计算的运算速率，使用了查表法，因此需要在初始化一个软件CRC
 * 计算器时(\sa am_crc_soft_init())提供一个用于CRC计算的表。该表的具体内容与CRC
 * 模型中：width、poly 和 refin 三个成员的值相关，只要一个模型中这三个值相等，则
 * 它们就可以使用同一个 table 表。
 *
 * \param[in] p_table : TABLE表实例
 * \param[in] width   : CRC宽度
 * \param[in] poly    : 生成多项式
 * \param[in] refin   : 输入字节bit反序
 * \param[in] p_data  : 存储数据的数组空间，其实际类型与 width的值相关。
 *                     - 8位及8位以下，其指向的 TABLE 原型为：uint8_t  data[256]
 *                     - 9  至16位，   其指向的 TABLE 原型为：uint16_t data[256]
 *                     - 17 至32位，   其指向的 TABLE 原型为：uint32_t data[256]
 *
 * \retval AM_OK      : 生成成功
 * \retval -AM_EINVAL : 生成失败，参数存在错误
 *
 * \note 为了便于用户使用，已经提供了常见的 table 表，详见 \ref am_if_crc_table_def.
 * 用户可以直接使用这些TABLE表的名字作为初始化函数 p_table 的值，若在已提供的table列
 * 表中未找到待使用的CRC模型。则可以使用 该函数创建一个table表。
 */
int am_crc_table_create (am_crc_table_t  *p_table,
                         uint8_t          width,
                         uint32_t         poly,
                         am_bool_t        refin,
                         void            *p_data);

/**
 * \brief 初始化一个软件 CRC 计算器
 *
 * \param[in] p_crc   : CRC 计算器实例
 * \param[in] p_table : 本软件 CRC 使用的 TABLE 表，TABLE表与使用的CRC模型相关，
 *                      详见 \ref am_if_crc_table_def. 若在已提供的table列表中未
 *                      找到待使用的CRC模型。则可以使用 \sa am_crc_table_create()
 *                      函数创建一个table表。
 *
 * \return 标准的CRC服务句柄，若为NULL， 则表示初始化失败
 *
 * \warning 这里传入的TABLE表，决定了该 CRC 计算器支持的CRC模型，即支持的CRC模型 \n
 * 中，width、poly、refin 的值必须与 TABLE 表的这三个值一致。
 *
 * \note 可以使用该函数初始化多个CRC计算器，以便支持多种不同模型的CRC。
 */
am_crc_handle_t am_crc_soft_init (am_crc_soft_t         *p_crc,
                                  const am_crc_table_t  *p_table);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CRC_SOFT_H */

/* end of file */
