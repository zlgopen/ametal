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
 * \brief kprintf for kernel and driver debugging
 *
 * to make kprintf safe, define AM_KPRINTF_LOCK_ENABLE in config.h
 *
 * \internal
 * \par modification history:
 * - 1.00 12-11-22  orz, first implementation from RTK
 * \endinternal
 */

#include "am_vdebug.h"
#include "am_koutput.h"

/**
 * \brief kernel format print function
 *
 * This function output the formatted string into the current console device.
 * The console output functions is set by am_console_set().
 *
 * \param fmt the format string
 * \param ... the arguments list for the format string
 *
 * \return number of chars output into console
 */
int am_kprintf (const char *fmt, ...)
{
    int     len;
    va_list args;

    if ((NULL == gpfunc_kputc) || (NULL == gpfunc_kputs)) {
        return 0;
    }

    va_start(args, fmt);
    
    len = am_vfprintf_do(gp_kout_file, gpfunc_kputc, gpfunc_kputs, fmt, args);
    va_end(args);

    return len;
}

/* end of file */

