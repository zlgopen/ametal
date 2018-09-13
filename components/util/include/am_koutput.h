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
 * \brief 设置信息输出接口
 *
 * \internal
 * \par modification history:
 * - 1.00 15-12-08  tee, first implementation from AWorks
 * \endinternal
 */

#ifndef __AM_KOUTPUT_H
#define __AM_KOUTPUT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name am_koutput
 * @{
 */

/** \brief 句柄，作为 kputc 和 kputs函数的第二个参数 */
extern void  *gp_kout_file;

/** 
 * \biref 全局函数指针，指向输出一个字符的函数
 *
 * 使用am_kprintf函数前，应确保该指针被正确赋值
 */ 
extern int  (*gpfunc_kputc)(const char  c, void *fil);
    
/** 
 * \biref 全局函数指针，指向输出一个字符串的函数
 *
 * 使用am_kprintf函数前，应确保该指针被正确赋值
 */ 
extern int  (*gpfunc_kputs)(const char *s, void *fil);

/** 
 * \biref 设置信息输出使用的字符输出函数和字符串输出函数
 *
 * \param[in] fil     :  句柄，作为 f_putc 和 f_puts 函数的第二个参数
 * \param[in] f_putc  :  函数指针，指向的函数用于输出一个字符
 * \param[in] f_puts  :  函数指针，指向的函数用于输出一个字符串
 *
 * \return 无 
 */ 
void am_koutput_set (void  *fil,
                     int  (*f_putc) (const char  c, void *fil),
                     int  (*f_puts) (const char *s, void *fil));

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_K_OUTPUT_H */

/* end of file */
