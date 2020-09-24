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
 * \brief QuickLZ implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 2020-09-10  yrh, first implementation.
 * \endinternal
 */

#include "am_mem.h"
#include "QuickLZ/quicklz.h"
#include "am_quicklz.h"

size_t am_quicklz_compress(const void *p_src, char *p_dst, size_t src_size)
{
    size_t size = 0;

    if ((!p_src) || (!p_dst)) {
        return 0;
    }

    qlz_state_compress *p_qlz_state =
        (qlz_state_compress *)am_mem_alloc(sizeof(qlz_state_compress));

    if (NULL == p_qlz_state) {
        return 0;
    }

    size = qlz_compress(p_src, p_dst, src_size, p_qlz_state);

    am_mem_free(p_qlz_state);

    return size;
}

size_t am_quicklz_decompress(const char *p_src,
                             void       *p_dst)
{
    size_t size = 0;
    if ((!p_src) || (!p_dst)) {
        return 0;
    }

    qlz_state_decompress *p_qlz_state =
        (qlz_state_decompress *)am_mem_alloc(sizeof(qlz_state_decompress));

    if (NULL == p_qlz_state) {
        return 0;
    }

    size = qlz_decompress(p_src, p_dst, p_qlz_state);

    am_mem_free(p_qlz_state);

    return size;
}

size_t am_quicklz_size_compressed(const char *p_src)
{
    return qlz_size_compressed(p_src);
}

size_t am_quicklz_size_decompressed(const char *p_src)
{
    return qlz_size_decompressed(p_src);
}

/* end of file */
