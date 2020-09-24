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
 * \brief  bsdiff/bspatch 差分算法
 *
 * \internal
 * \par Modification History
 * - 1.00 2020-09-10 , yrh, first implementation.
 * \endinternal
 */
#ifndef __AM_BSDIFF_H
#define __AM_BSDIFF_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup am_if_am_bsdiff
 * @copydoc am_bsdiff.h
 * @{
 */

/**
 * \brief 差分数据流信息
 *
 */
typedef struct {
    uint8_t *p_diff;    /**< \brief 差分数据 */
    int      offset;    /**< \brief 已经读取的差分数据的偏移 */
}bsdiff_stream_info_t;

/**
 * \brief 解差分数据
 *
 * \param[in]  p_old_data    : 旧数据
 * \param[in]  old_data_size : 旧数据大小
 * \param[in]  p_diff_data   : 差分数据
 * \param[out] p_new_buff    : 存放解差分后的新数据的缓冲区
 * \param[in]  new_data_size : 新数据的实际大小（并不是指传入的p_new_buff指向的大小）
 *
 * \return  0  ：成功;
 *          -1 ：失败；
 */
int am_bspatch(const uint8_t *p_old_data,
               int32_t        old_data_size,
               uint8_t       *p_diff_data,
               uint8_t       *p_new_buff,
               int32_t        new_data_size);
/**
 * \brief 计算差分数据
 *
 * \param[in]  p_old_data    : 旧数据
 * \param[in]  old_data_size : 旧数据大小
 * \param[in]  p_new_data    : 新数据
 * \param[in]  new_data_size : 新数据大小
 * \param[out] p_diff_buf    : 存放差分数据
 *
 * \return  > 0  ：差分数据的大小（p_diff_buf实际存放的有效数据）;
 *          =< 0 ：失败；
 */
int am_bsdiff(const uint8_t *p_old_data,
              int32_t        old_data_size,
              const uint8_t *p_new_data,
              int32_t        new_data_size,
              uint8_t       *p_diff_buf);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BSDIFF_H */

/* end of file */
