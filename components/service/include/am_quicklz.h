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
 * \brief  QuickLZ 压缩解压算法
 *
 * \internal
 * \par Modification History
 * - 1.00 2020-09-10 , yrh, first implementation.
 * \endinternal
 */
#ifndef __AM_QUICKLZ_H
#define __AM_QUICKLZ_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_am_quicklz
 * @copydoc am_quicklz.h
 * @{
 */
#include "ametal.h"

/**
 * \brief 数据压缩
 *
 * \param[in]  p_src    : 源数据地址
 * \param[in]  p_dst    : 压缩后数据存储起始地址，其大小至少为（src_size + 400）字节
 * \param[in]  src_size : 需要压缩的数据大小
 *
 * \return > 0 : 压缩后的数据的字节数
 *         = 0 : 压缩失败
 */
size_t am_quicklz_compress(const void *p_src, char *p_dst, size_t src_size);

/**
 * \brief 数据解压缩
 *
 * \param[in] p_src ：指向解压缩数据的储存地址
 * \param[in] p_dst ：指向解压缩后的数据储存地址，解压后数据量会增大，p_dst 必须有足够大的空间
 *
 * \return > 0 : 解压缩后数据的字节数
 *         = 0 : 解压失败
 */
size_t am_quicklz_decompress(const char *p_src,
                             void       *p_dst);

/**
 * \brief 获取当前压缩包的大小
 *
 * \param[in] p_src ：指向已经压缩后的数据的起始地址
 *
 * \return 压缩包的数据大小
 */
size_t am_quicklz_size_compressed(const char *p_src);

/**
 * \brief 获取压缩包解压后的数据大小（也是压缩之前的数据大小）
 *
 * \param[in] p_src ：指向已经压缩后的数据的起始地址
 *
 * \return 解压缩以后的数据大小
 */
size_t am_quicklz_size_decompressed(const char *p_src);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_QUICKLZ_H */

/* end of file */
