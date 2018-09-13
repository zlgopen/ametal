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
 * \brief kernel output functions
 *
 * \internal
 * \par modification history:
 * - 1.00 12-11-23  orz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_koutput.h"


void  *gp_kout_file                            = NULL;
int  (*gpfunc_kputc)(const char  c, void *fil) = NULL;
int  (*gpfunc_kputs)(const char *s, void *fil) = NULL;

/**
 * \brief 设置输出信息函数
 */
void am_koutput_set (void  *fil,
                     int  (*f_putc) (const char  c, void *fil),
                     int  (*f_puts) (const char *s, void *fil))
{
    gp_kout_file = fil;
    gpfunc_kputc = f_putc;
    gpfunc_kputs = f_puts;
}

/* @} */

/* end of file */

