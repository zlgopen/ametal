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
 * \brief NVRAM service
 *
 * \internal
 * \par Modification history
 * - 1.00 16-08-03  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_uart.h"
#include "am_nvram.h"
#include "am_int.h"
#include "string.h"

/*******************************************************************************
  global variables
*******************************************************************************/
am_local am_nvram_dev_t           *__gp_nvram_dev_list = NULL;
am_local const am_nvram_segment_t *__gp_seg_list       = NULL;

/*******************************************************************************
  local functions
*******************************************************************************/

am_local const am_nvram_segment_t *__nvram_segment_query (const char *p_name, int unit)
{
    const am_nvram_segment_t *p_seg = __gp_seg_list;

    while (p_seg && (p_seg->p_name)) {

        if ((p_seg->unit == unit) && (strcmp(p_seg->p_name, p_name) == 0)) {
            return p_seg;
        }
        p_seg++;
    }

    return NULL;
}

/******************************************************************************/

am_local am_nvram_dev_t *__nvram_dev_query (const char *p_name)
{
    am_nvram_dev_t *p_dev = __gp_nvram_dev_list;

    while (p_dev && (p_dev->p_dev_name)) {

        if (strcmp(p_dev->p_dev_name, p_name) == 0) {      /* find the device */
            return p_dev;
        }
        p_dev = p_dev->p_next;
    }

    return NULL;
}

/******************************************************************************/

am_local int __nvram_process (const char        *p_name,
                              int                unit,
                              uint8_t           *p_buf,
                              int                offset,
                              int                len,
                              am_bool_t          is_get)
{
    const am_nvram_segment_t *p_seg = NULL;
    const am_nvram_dev_t     *p_dev = NULL;

    p_seg = __nvram_segment_query(p_name, unit);

    if (p_seg == NULL) {     /* Can't find the segment */
        return -AM_EINVAL;
    }

    p_dev = __nvram_dev_query(p_seg->p_dev_name);

    if (p_dev == NULL) {     /* Can't find the device */
        return -AM_ENODEV;
    }

    /* no data will be read or written */
    if (len == 0) {
        return AM_OK;
    }

    /* the start address beyond this seg's capacity */
    if (offset >= p_seg->seg_size) {
        return -AM_ENXIO;
    }

    /* adjust len that will not beyond eeprom's capacity */
    if ((offset + len) > p_seg->seg_size) {
        len = p_seg->seg_size - offset;
    }

    if ((is_get) && (p_dev->p_funcs->pfn_nvram_get)) {

        return p_dev->p_funcs->pfn_nvram_get(p_dev->p_drv,
                                             p_seg->seg_addr + offset,
                                             p_buf,
                                             len);

    }

    if ((!(is_get)) && (p_dev->p_funcs->pfn_nvram_set)) {

        return p_dev->p_funcs->pfn_nvram_set(p_dev->p_drv,
                                             p_seg->seg_addr + offset,
                                             p_buf,
                                             len);
    }

    return -AM_EIO;
}

/*******************************************************************************
  public functions
*******************************************************************************/

/* NVRAM service init */
int am_nvram_init (const am_nvram_segment_t *p_seglist)
{
    __gp_nvram_dev_list = NULL;
    __gp_seg_list       = p_seglist;

    return AM_OK;
}
/******************************************************************************/

/* register one nvram device into system */
int am_nvram_dev_register (am_nvram_dev_t *p_dev)
{
    int key;

    if ((p_dev             == NULL) ||
        (p_dev->p_dev_name == NULL) ||
        (p_dev->p_funcs    == NULL)) {

        return -AM_EINVAL;
    }

    /* Check the device whether already exist or not */
    if (__nvram_dev_query(p_dev->p_dev_name) != NULL) {
        return -AM_EEXIST;
    }

    /* insert to the head of the device list */
    key = am_int_cpu_lock();

    p_dev->p_next       = __gp_nvram_dev_list;
    __gp_nvram_dev_list = p_dev;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/

/* unregister one nvram service into system */
int am_nvram_dev_unregister (am_nvram_dev_t *p_dev)
{
    am_nvram_dev_t   *p_head;
    int               key;

    key = am_int_cpu_lock();

    p_head = AM_CONTAINER_OF(&__gp_nvram_dev_list, am_nvram_dev_t, p_next);

    while (p_head->p_next) {
        if (p_head->p_next == p_dev) {
            p_head->p_next = p_dev->p_next;
            p_dev->p_next  = NULL;
            break;
        }
        p_head = p_head->p_next;
    }

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/

/* write data to nvram */
int am_nvram_set (char              *p_name,
                  int                unit,
                  uint8_t           *p_buf,
                  int                offset,
                  int                len)
{
    return __nvram_process(p_name, unit, p_buf, offset, len, AM_FALSE);
}

/******************************************************************************/

/* read data from nvram */
int am_nvram_get (char              *p_name,
                  int                unit,
                  uint8_t           *p_buf,
                  int                offset,
                  int                len)
{
    return __nvram_process(p_name, unit, p_buf, offset, len, AM_TRUE);
}

/******************************************************************************/

int am_nvram_size_get (char *p_name, int unit, size_t *p_size)
{
    const am_nvram_segment_t *p_seg = NULL;

    p_seg = __nvram_segment_query(p_name, unit);

    if (p_seg == NULL) {     /* Can't find the segment */
        return -AM_EINVAL;
    }

    if (p_size) {
        *p_size = p_seg->seg_size;
    }

    return AM_OK;
}

/* end of file */
