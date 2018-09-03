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
 * \brief ARM LIB 适配器
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-23  pea, first implementation.
 * \endinternal
 */

#ifndef __MICROLIB

/** \brief 防止使用半主机模式 */
#pragma import(__use_no_semihosting)

/** \brief 使用heap2 */
#pragma import(__use_realtime_heap)

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "ametal.h"
#include "am_mem.h"
#include "am_armlib_adapter.h"
#include <stdio.h>
#include <errno.h>
#include <rt_sys.h>
#include <rt_locale.h>
#include <rt_misc.h>
#include <time.h>

/*******************************************************************************
  本地全局变量声明
*******************************************************************************/

/** \brief ARM LIB适配器结构 */
typedef struct am_armlib_adapter {
    const am_armlib_adapter_ops_t *p_ops; /** < \brief 驱动函数 */
    void                          *p_arg; /** < \brief 驱动函数参数 */
} am_armlib_adapter_t;

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

static am_armlib_adapter_t __g_adapter; /**< \brief ARM LIB适配器 */

const char __stdin_name[]  = ":tt";     /**< \brief 定义stdin的文件名 */
const char __stdout_name[] = ":tt";     /**< \brief 定义stdout的文件名 */
const char __stderr_name[] = ":tt";     /**< \brief 定义stderr的文件名 */

/*******************************************************************************
  本地函数定义
*******************************************************************************/

/** 
 * \brief 输出一个字符
 *
 * \param[in] ch       需要输出的字符
 * \param[in] p_stream 指向FILE对象的指针
 *
 * \return 如果没有发生错误，返回被输出的字符。否则返回EOF
 */
int fputc (int ch, FILE *p_stream) 
{
    if ((p_stream->handle >= 0) && (p_stream->handle < 3)) {
        if(__g_adapter.p_ops && __g_adapter.p_ops->pfn_putc) {
            return __g_adapter.p_ops->pfn_putc(__g_adapter.p_arg, (char)ch);
        }
    }

    return EOF;
}

/** 
 * \brief 输入一个字符
 *
 * \param[in] p_stream 指向FILE对象的指针
 *
 * \return 如果没有发生错误，返回输入的字符。否则返回EOF
 */
int fgetc (FILE *p_stream)
{        
    if ((p_stream->handle >= 0) && (p_stream->handle < 3)) {
        if (__g_adapter.p_ops && __g_adapter.p_ops->pfn_getc) {
            return __g_adapter.p_ops->pfn_getc(__g_adapter.p_arg);
        }
    }
        
    return EOF;
};

/** 
 * \brief 将上次读取的字符返回到FILE流中
 *
 * \param[in] p_stream 指向FILE对象的指针
 *
 * \retval 0   成功
 * \retval EOF 失败
 */
int __backspace (FILE *p_stream)
{
    return EOF;
}

/** 
 * \brief ARM LIB退出函数，不可返回
 *
 * \param[in] return_code 调用该函数的返回值
 *
 * \return 无
 */
void _sys_exit (int return_code)
{
    AM_FOREVER;
}

/** 
 * \brief 断言失败处理函数
 *
 * \param[in] p_expr 表达式
 * \param[in] p_file 文件名
 * \param[in] line   行号
 *
 * \return 无
 */
void __aeabi_assert (const char *p_expr, const char *p_file, int line)
{
    printf("*** assertion failed: %s, %s, %d\r\n", p_expr, p_file, line);

    abort();
}

/** 
 * \brief 打开文件
 *
 * \param[in] p_name   文件名
 * \param[in] openmode 打开模式
 *
 * \return 成功返回文件句柄，失败返回-1
 */
FILEHANDLE _sys_open (const char *p_name, int openmode)
{
    return 1;
}

/** 
 * \brief 关闭文件
 *
 * \param[in] fh 文件句柄
 *
 * \return 成功返回0，失败返回负值
 */
int _sys_close (FILEHANDLE fh)
{
    return -1;
}

/** 
 * \brief 写文件
 *
 * \param[in] fh    文件句柄
 * \param[in] p_buf 缓冲区地址
 * \param[in] len   待写入字节数
 * \param[in] mode  模式(因为历史原因保留，必须忽略)
 *
 * \return 成功返回0，失败返回负值且大小为写入失败的字节数
 */
int _sys_write (FILEHANDLE           fh, 
                const unsigned char *p_buf,
                unsigned             len, 
                int                  mode)
{
    return -1;
}

/** 
 * \brief 读文件
 *
 * \param[in]  fh    文件句柄
 * \param[out] p_buf 缓冲区地址
 * \param[in]  len   待写入字节数
 * \param[in]  mode  模式(因为历史原因保留，必须忽略)
 *
 * \return 成功返回0，但读取到文件末尾时返回EOF
 *         只成功读取部分数据时，返回未读取的字节数
 *         从文件末尾开始读取且只成功读取部分数据时，返回未读取的字节数且将最高位置位
 *         失败返回-1
 */
int _sys_read (FILEHANDLE     fh, 
               unsigned char *p_buf,
               unsigned       len, 
               int            mode)
{
    return -1;
}

/** 
 * \brief 向终端输出一个字符
 *
 * \param[in] ch 待输出的字符
 *
 * \return 无
 */
void _ttywrch (int ch)
{
    fputc(ch, stdout);
}

/** 
 * \brief 判断一个文件句柄是否为终端
 *
 * \param[in] fh 文件句柄
 *
 * \return 是终端返回非0，不是终端返回0
 */
int _sys_istty (FILEHANDLE fh)
{
    return 0;
}

/** 
 * \brief 移动文件指针
 *
 * \param[in] fh  文件句柄
 * \param[in] pos 待移动到的位置(相对0地址偏移)
 *
 * \return 成功返回>=0, 失败返回<0
 */
int _sys_seek (FILEHANDLE fh, long pos)
{
    return -1;
}

/** 
 * \brief Flush缓冲区内容
 *
 * \param[in] fh 文件句柄
 *
 * \return 成功返回>=0, 失败返回<0
 */
int _sys_ensure(FILEHANDLE fh)
{
    return -1;
}

/** 
 * \brief 返回文件长度
 *
 * \param[in] fh 文件句柄
 *
 * \return 成功返回文件长度, 失败返回<0
 */
long _sys_flen (FILEHANDLE fh)
{
    return -1;
}

/** 
 * \brief 获取时钟
 *
 * \param 无
 *
 * \return 时钟数
 */
clock_t clock (void)
{
    return (clock_t)-1;
}

/** 
 * \brief 时钟初始化
 */
void _clock_init (void)
{
    /* VOID */
}

/**
 * \brief 分配一段内存空间（内存空间会被初始化为0）
 *
 * \param[in] nmemb 元素个数
 * \param[in] size  一个元素的大小，分配内存空间的大小即为 nmemb * size
 *
 * \return 分配空间的首地址，NULL表明分配失败
 *
 * \note 分配的内存空间会被初始化为0
 */
void *calloc (size_t nmemb, size_t size)
{
    return am_mem_calloc(nmemb, size);
}

/**
 * \brief 释放已分配的内存空间
 *
 * \param[in] ptr 已分配的内存空间的首地址
 *
 * \return 无
 */
void free (void *ptr)
{
    am_mem_free(ptr);
}

/**
 * \brief 分配一段内存空间
 *
 * \param[in] size 分配空间的大小
 *
 * \return 分配空间的首地址，NULL表明分配失败
 */
void *malloc (size_t size)
{
    return am_mem_alloc(size);
}

/**
 * \brief 动态内存调整，改变已分配空间的大小
 *
 * \param[in] ptr  已分配空间的首地址
 * \param[in] size 重新分配的空间的大小
 *
 * \return 重新分配空间的首地址，NULL表明分配失败
 *
 * \note 函数先判断当前的指针是否有足够的连续空间，如果有，扩大 ptr 指向地址的
 * 空间，直接将ptr的值返回。如果空间不够，先按照size指定的大小分配空间，将
 * 原有数据从头到尾拷贝到新分配的内存区域，而后释放原来ptr所指内存区域（原来
 * 指针自动释放，不需要 am_memheap_free()），同时返回新分配的内存区域的首地址。
 */
void *realloc (void *ptr, size_t size)
{
    return am_mem_realloc(ptr, size);
}

/*******************************************************************************
  外部函数定义
*******************************************************************************/

/**
 * \brief 适配器初始化
 *
 * \param[in] p_ops 驱动函数
 * \param[in] p_arg 驱动函数参数
 *
 * \return AM_OK
 */
int am_armlib_adapter_init (const am_armlib_adapter_ops_t *p_ops,
                            void                          *p_arg)
{
    __g_adapter.p_ops = p_ops;
    __g_adapter.p_arg = p_arg;

    return AM_OK;
}

#endif /* __MICROLIB */

/* end of file */
