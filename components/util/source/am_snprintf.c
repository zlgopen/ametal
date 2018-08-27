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
 * \brief am_snprintf() implementation.
 *
 * \internal
 * \par modification history:
 * - 1.01 14-09-25  orz, re-implementation with am_vsnprintf.
 * - 1.00 12-11-22  orz, first implementation.
 * \endinternal
 */

#include "am_vdebug.h"
#include <stdarg.h>

/**
 * \brief format a string into a string buffer with known size
 *
 * \param buf   the string buffer to hold the formatted string
 * \param sz    the size of string buffer
 * \param fmt   the format string
 * \param ...   the arguments list for the format string
 *
 * \return number of char put into buffer
 */
int am_snprintf (char *buf, size_t sz, const char *fmt, ...)
{
    va_list args;
    int     len;

    va_start(args, fmt);
    len = am_vsnprintf(buf, sz, fmt, args);
    va_end(args);

    return len;
}

/* end of file */
