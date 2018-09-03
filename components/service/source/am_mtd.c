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
 * \brief AMetal MTD (Memory Technology Device) driver.
 *
 * \internal
 * \par modification history:
 * - 15-09-15 tee, first implementation,copy from amorks.
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/
 
#include "am_mtd.h"
#include "string.h"
#include "am_int.h"

/*******************************************************************************
  Public functions
*******************************************************************************/

/**
 * Todo: add asynchronous operation.
 */
int am_mtd_erase (am_mtd_handle_t     handle,
                  uint32_t            addr,
                  uint32_t            len)
{
    struct am_mtd_erase_info  ei;

    int ret;

    if ((addr > handle->size) || (len > (handle->size - addr))) {
        return -AM_EINVAL;
    }

    if (!(handle->flags & AM_MTD_FLAG_WRITEABLE)) {
        return -AM_EROFS;
    }

    if (handle->flags & AM_MTD_FLAG_NO_ERASE) {
        return AM_OK;
    }

    if (!len) {
        return AM_OK;
    }

    ei.addr         = addr;
    ei.len          = len;
    ei.pfn_callback = NULL;
    ei.priv         = NULL;
    ei.retries      = 1;

    /* Todo : Set a callback function (wait event)*/
    ret = handle->p_ops->pfn_mtd_erase(handle->p_drv, &ei);

    return ret;
}

int am_mtd_read (am_mtd_handle_t     handle,
                 uint32_t            addr,
                 void               *p_buf,
                 uint32_t            len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((addr > handle->size) || (len > (handle->size - addr))) {
        return -AM_EINVAL;
    }

    if (!len) {
        return AM_OK;
    }

    /*
     * In the absence of an error, drivers return a non-negative integer
     * representing the maximum number of bitflips that were corrected on
     * any one ecc region (if applicable; zero otherwise).
     */
    return handle->p_ops->pfn_mtd_read(handle->p_drv, addr, p_buf, len);
}

/******************************************************************************/
int am_mtd_write (am_mtd_handle_t     handle,
                  uint32_t            addr,
                  const void         *p_buf,
                  uint32_t            len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((addr > handle->size) || (len > (handle->size - addr))) {
        return -AM_EINVAL;
    }

    if (!(handle->p_ops->pfn_mtd_write)         ||
        !(handle->flags & AM_MTD_FLAG_WRITEABLE)) {
        return -AM_EROFS;
    }

    if (!len) {
        return AM_OK;
    }
    return handle->p_ops->pfn_mtd_write(handle->p_drv, addr, p_buf, len);
}

/* end of file */
