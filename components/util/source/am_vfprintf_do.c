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
 * \brief do vprintf jobs
 *
 * \internal
 * \par modification history:
 * - 1.00 15-12-08  tee, first implementation from AWorks
 * \endinternal
 */

#include "am_vdebug.h"
#include <stdarg.h>

/**
 * \brief Format a string and output it.
 *
 * This function do all the vXXXprintf() works, and it take 2 functions as
 * arguments to output the characters.
 * all other vXXXprintf() functions are based on this function.
 *
 * \param fil       The handle for f_putc and f_puts functions
 * \param f_putc    Pointer to function to output a char
 * \param f_puts    Pointer to function to output a string
 * \param fmt       The format string
 * \param args      The arguments for the format string
 *
 * \return The number of characters output
 */
int am_vfprintf_do (void       *fil,
                    int       (*f_putc) (const char  c, void *fil),
                    int       (*f_puts) (const char *s, void *fil),
                    const char *fmt,
                    va_list     args)
{
    unsigned char   c, f;
    int             r;
    unsigned long   val;
    char            s[16];
    int             i, w, res, cc;


    for (cc = res = 0; cc >= 0; res += cc >= 0 ? cc : 0) {
        if ((c = *fmt++) == 0) {
            break;                      /* End of string */
        } else if (c != '%') {          /* Non escape cahracter */
            cc = f_putc(c, fil);
            continue;
        }
        w = f = 0;
        c = *fmt++;
        if (c == '0') {                 /* Flag: '0' padding */
            f = 1;
            c = *fmt++;
        }
        while (c >= '0' && c <= '9') {  /* Precision */
            w = w * 10 + (c - '0');
            c = *fmt++;
        }
        if (c == 'l') {                 /* Prefix: Size is long int */
            f |= 2;
            c  = *fmt++;
        }
        if (c == 's') {                 /* Type is string */
            cc = f_puts(va_arg(args, char *), fil);
            continue;
        }
        if (c == 'c') {                 /* Type is character */
            cc = f_putc(va_arg(args, int), fil);
            continue;
        }
        if (c == '%') {                 /* Char is '%' */
            cc = f_putc('%', fil);
            continue;
        }
        r = 0;
        if (c == 'd') {
            r = 10;             /* Type is signed decimal */
        } else if (c == 'u') {
            r = 10;             /* Type is unsigned decimal */
        } else if (c == 'X') {
            r = 16;             /* Type is unsigned hexdecimal, upper case */
        } else if (c == 'x') {
            r = 0x100 + 16;     /* Type is unsigned hexdecimal, lower case */
        }else {
            break;              /* Unknown type */
        }
        if (f & 2) {            /* Get the value */
            val = (unsigned long)va_arg(args, long);
        } else {
            val = (c == 'd')
                ? (unsigned long)(long)va_arg(args, int)
                : (unsigned long)va_arg(args, unsigned int);
        }
        /* Put numeral string */
        if ((c == 'd') && (val & 0x80000000)) {
            val = 0 - val;
            f  |= 4;
        }
        i    = sizeof(s) - 1;
        s[i] = 0;
        do {
            c = (unsigned char)(val % (r & 0xff) + '0');
            if (c > '9') {
                if (r & 0x100) {
                    c += 39;
                } else {
                    c += 7;
                }
            }
            s[--i] = c;
            val   /= (r & 0xff);
        } while (i && val);
        if (i && (f & 4)) {
            s[--i] = '-';
        }
        w = sizeof(s) - 1 - w;
        while (i && i > w) {
            s[--i] = (f & 1) ? '0' : ' ';
        }
        cc = f_puts(&s[i], fil);
    }

    return res;
}

/* end of file */

