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
 * \brief CRC TABLE 表定义， 用于软件 CRC 计算
 *
 * 本文件定义了 几种常见CRC的 TABLE表，可直接用于 (\sa am_crc_soft_init()) 函数的
 * p_table 参数.
 *
 * \internal
 * \par modification history
 * - 1.00 18-01-19  tee, first implementation
 * \endinternal
 */

#ifndef __AM_CRC_TABLE_DEF_H
#define __AM_CRC_TABLE_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_crc_table
 * \copydoc am_crc_table.h
 * @{
 */
#include "am_common.h"
#include "am_crc.h"
#include "am_crc_soft.h"

/**
 * \name 8位及8位以下crc table
 *
 * 当前已经支持的模型，表格已经建立好，无需重新建立，直接使用即可。
 * 表的命名规则：g_crc_table_X_Y(_ref)[256] X、Y代表一个数字。
 *   - 紧跟 g_crc_table 的数字X表示crc宽度，如crc5则对应 g_crc_table_5
 *   - 接着下划线后的数字Y表示poly,生成多项式。
 *      - 8位及8位以下：poly使用8位表示，  如09,不省略0
 *      - 9至16位     ：poly使用16位表示，如8005,不省略0
 *      - 17 至32位   ：poly使用32位表示，如04C11DB7,不省略0.
 *   - 后缀带ref的表示该表为反序表，当crc模型中refin == TRUE时，
 *     应该传入后缀带ref的表。
 *
 * 因此，根据使用的crc模型，即可知道该使用哪个表，如crc模型为：
 *     width = 5，poly = 0x09,refin = TRUE.
 * 则直接使用 g_crc_table_5_09_ref 表即可。
 *
 * \note 表的命名规则同样适合于其他位数的CRC table表
 * @{
 */

/** \brief crc4  poly = 0x03, refin = TRUE  */
am_import am_const am_crc_table_t g_crc_table_4_03_ref;

/** \brief crc5  poly = 0x09, refin = FALSE */
am_import am_const am_crc_table_t g_crc_table_5_09;

/** \brief crc5  poly = 0x15, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_5_15_ref;

/** \brief crc5  poly = 0x05, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_5_05_ref;

/** \brief crc6  poly = 0x03, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_6_03_ref;

/** \brief crc7  poly = 0x09, refin = FALSE */
am_import am_const am_crc_table_t g_crc_table_7_09;

/** \brief crc8  poly = 0x07, refin = FALSE */
am_import am_const am_crc_table_t g_crc_table_8_07;

/** \brief crc8  poly = 0x07, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_8_07_ref;

/** \brief crc8  poly = 0x31, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_8_31_ref;

/** @} */

/**
 * \name 9位至16位crc table
 * @{
 */

/** \brief crc16  poly = 0x8005, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_16_8005_ref;

/** \brief crc16  poly = 0x1021, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_16_1021_ref;

/** \brief crc16  poly = 0x1021, refin = FALSE */
am_import am_const am_crc_table_t g_crc_table_16_1021;

/** \brief crc16  poly = 0x3D65, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_16_3d65_ref;

/** @} */

/**
 * \name 17位至32位crc table
 * @{
 */

/** \brief crc32  poly = 0x04c11db7, refin = TRUE */
am_import am_const am_crc_table_t g_crc_table_32_04c11db7_ref;

/** \brief crc32  poly = 0x04c11db7, refin = FALSE */
am_import am_const am_crc_table_t g_crc_table_32_04c11db7;

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CRC_TABLE_H */

/* end of file */
