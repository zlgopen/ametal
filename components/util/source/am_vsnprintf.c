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
 * \brief am_vsnprintf() implementation.
 *
 * \internal
 * \par modification history:
 * - 1.00 15-12-08  tee, first implementation from AWorks
 * \endinternal
 */

#include "am_vdebug.h"
#include <stdarg.h>

/** \brief argument type for output into string buffer */
struct __str_put_arg {
    char *buf;
    char *end;
};

/**
 * \brief put a char into a string buffer
 *
 * \param c     the char to put
 * \param fil   handle of the string buffer
 *
 * \return 1 for success put, -1 for no more space
 */
static int __str_putc (const char c, void *fil)
{
    struct __str_put_arg *p_arg = (struct __str_put_arg *)fil;

    if (p_arg->buf < p_arg->end) {
        *p_arg->buf++ = c;
        return 1;
    }
    return -1;
}

/**
 * \brief put a string into a string buffer
 *
 * \param s     the string to put
 * \param fil   handle of the string buffer
 *
 * \return number of char successfully put, -1 for no more space
 */
static int __str_puts (const char *s, void *fil)
{
    struct __str_put_arg *p_arg = (struct __str_put_arg *)fil;
    char                 *start = p_arg->buf;
    char                  c;

    if (start >= p_arg->end) {
        return -1;
    }

    while (((c = *s++) != '\0') && (p_arg->buf < p_arg->end)) {
        *p_arg->buf++ = c;
    }
    return p_arg->buf - start;
}

/**
 * \brief format a string into a string buffer with known size
 *
 * \param buf   the string buffer to hold the formatted string
 * \param sz    the size of string buffer
 * \param fmt   the format string
 * \param args  The arguments for the format string
 *
 * \return number of char put into buffer
 */
int am_vsnprintf (char *buf, size_t sz, const char *fmt, va_list args)
{
    struct __str_put_arg spa;
    int                  len;

    /* check if space is enough */
    if (sz < 2) {
        if (sz == 1) {
            *buf = '\0';
        }
        return 0;
    }

    spa.buf = buf;
    spa.end = buf + sz - 1; /* reserve 1 byte for terminate NUL */

    len = am_vfprintf_do(&spa, __str_putc, __str_puts, fmt, args);

    buf[len] = '\0';        /* add a terminate NUL */

    return len;
}

/* end of file */
