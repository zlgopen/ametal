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
 * \brief bsdiff/bspatch implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 2020-09-10  yrh, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_mem.h"
#include "bsdiff/bspatch.h"
#include "bsdiff/bsdiff.h"
#include "am_bsdiff.h"

/* 解差分的读取差分数据回调 */
static int __bspatch_read(const struct bspatch_stream* p_stream, void* p_buff, int length)
{
    bsdiff_stream_info_t *p_stream_info = (bsdiff_stream_info_t *)(p_stream->opaque);
    int i;
    int offset = p_stream_info->offset;

    for (i = 0; i < length; i++) {
        ((uint8_t *)(p_buff))[i] = p_stream_info->p_diff[i + offset];
    }

    p_stream_info->offset += length;

    return 0;
}

/* 差分数据写入回调 */
static int __bsdiff_write (struct bsdiff_stream *p_stream,
                           const void           *p_buffer,
                           int                   write_size)
{
    int i;
    bsdiff_stream_info_t *p_stream_info = (bsdiff_stream_info_t *)(p_stream->opaque);
    int offset = p_stream_info->offset;

    for (i = 0; i < write_size; i++) {
        p_stream_info->p_diff[i + offset] = ((uint8_t *)(p_buffer))[i];
    }

    p_stream_info->offset += write_size;

    return 0;
}

/*******************************************************************************
  public functions
*******************************************************************************/
int am_bspatch(const uint8_t *p_old_data,
               int32_t        old_data_size,
               uint8_t       *p_diff_data,
               uint8_t       *p_new_buff,
               int32_t        new_data_size)
{
    if (!p_old_data || !p_new_buff || !p_diff_data) {
        return -1;
    }

    bsdiff_stream_info_t stream_info;

    stream_info.offset = 0;
    stream_info.p_diff = p_diff_data;
    struct bspatch_stream patch_stream = {
        &stream_info,
         __bspatch_read
    };
    if(0 != bspatch(p_old_data,
                    old_data_size,
                    p_new_buff,
                    new_data_size,
                    &patch_stream)) {
        return -1;
    }
    return 0;
}

int am_bsdiff(const uint8_t *p_old_data,
              int32_t        old_data_size,
              const uint8_t *p_new_data,
              int32_t        new_data_size,
              uint8_t       *p_diff_buf)
{
    if (!p_old_data || !p_new_data || !p_diff_buf) {
        return -1;
    }

    bsdiff_stream_info_t stream_info;
    stream_info.offset = 0;
    stream_info.p_diff = p_diff_buf;

    struct bsdiff_stream diff_stream = {
        &stream_info,
        am_mem_alloc,
        am_mem_free,
        __bsdiff_write
    };

    if(0 != bsdiff(p_old_data,
                   old_data_size,
                   p_new_data,
                   new_data_size,
                   &diff_stream)) {
        return -1;
    }
    return stream_info.offset;
}
/* end of file */
