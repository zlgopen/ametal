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
 * \brief memory management
 *
 * \internal
 * \par modification history:
 * - 1.00 14-06-13  zen, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_mem.h"
#include "am_memheap.h"
#include "am_board.h"

#include <string.h>

/*******************************************************************************
  locals
*******************************************************************************/
static struct am_memheap __g_system_heap;       /* system heap object */

/*******************************************************************************
  implementation
*******************************************************************************/

/******************************************************************************/
void am_bsp_system_heap_init (void *heap_start, void *heap_end)
{
    /* initialize a default heap in the system */
    am_memheap_init(&__g_system_heap,
                    "system_heap",
                    (void *)heap_start,
                    (uint32_t)heap_end - (uint32_t)heap_start);
}

/******************************************************************************/
void *am_mem_align(size_t size, size_t align)
{
    void    *align_ptr;
    void    *ptr;
    size_t   align_size;

    /* align the alignment size to 4 byte */
    align = AM_ROUND_UP(align, sizeof(void *));

    /* get total aligned size */
    align_size =  AM_ROUND_UP(size, sizeof(void *)) + align + sizeof(void *);

    /* allocate memory block from heap */
    ptr = am_memheap_alloc(&__g_system_heap, align_size);
    if (ptr != NULL) {

        /* round up ptr to align */
        align_ptr = (void *)AM_ROUND_UP((uint32_t)ptr + sizeof(void *), align);

        /* set the pointer before alignment pointer to the real pointer */
        *((uint32_t *)((uint32_t)align_ptr - sizeof(void *))) = (uint32_t)ptr;

        ptr = align_ptr;
    }

    return ptr;
}

/******************************************************************************/
void *am_mem_alloc(size_t size)
{
    /* allocate in the system heap */
    return am_mem_align(size, 4);
}

/******************************************************************************/
void *am_mem_calloc(size_t nelem, size_t size)
{
    void       *ptr;
    size_t      total_size;

    total_size = nelem * size;
    ptr = am_mem_alloc(total_size);
    if (ptr != NULL) {
        memset(ptr, 0, total_size);
    }

    return ptr;
}

/******************************************************************************/
void am_mem_free(void *ptr)
{
    void *real_ptr = NULL;

    if (ptr != NULL) {
        real_ptr = (void *)*(uint32_t *)((uint32_t)ptr - sizeof(void *));
    }
    am_memheap_free(real_ptr);
}

/******************************************************************************/
size_t am_mem_size(void *ptr)
{
    void *real_ptr = NULL;

    if (ptr != NULL) {
        real_ptr = (void *)*(uint32_t *)((uint32_t)ptr - sizeof(void *));
    }
    return am_memheap_memsize( &__g_system_heap, real_ptr);
}

/******************************************************************************/
void *am_mem_realloc(void *ptr, size_t newsize)
{
    void  *real_ptr     = NULL;
    void  *new_ptr      = NULL;

    if (ptr != NULL) {
        /* get real pointer */
        real_ptr = (void *)*(uint32_t *)((uint32_t)ptr - sizeof(void *));
    } else {
        /* equal malloc when ptr==NULL */
        return am_mem_alloc(newsize);
    }

    /* allocate newsize plus one more pointer, used for save real pointer */
    real_ptr = am_memheap_realloc(&__g_system_heap,
                                  real_ptr,
                                  newsize + sizeof(void *));

    if (real_ptr != NULL) {
        *(uint32_t *)real_ptr = (uint32_t)real_ptr;
        new_ptr               = (void *)((size_t)real_ptr + sizeof(void *));
    } else {
        new_ptr = NULL;
    }

    return new_ptr;
}

/* end of file */
