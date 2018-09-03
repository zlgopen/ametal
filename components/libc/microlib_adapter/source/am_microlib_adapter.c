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
 * \brief Micro LIB 适配器
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-17  pea, first implementation.
 * \endinternal
 */
 
#ifdef __MICROLIB

/** \brief 防止使用半主机模式 */
#pragma import(__use_no_semihosting)

/*******************************************************************************
  头文件包含
*******************************************************************************/
#include "ametal.h"
#include "am_mem.h"
#include "am_microlib_adapter.h"
#include <stdio.h>
#include <errno.h>

/*******************************************************************************
  本地全局变量声明
*******************************************************************************/

/** \brief Micro LIB适配器结构 */
typedef struct am_microlib_adapter {
    const am_microlib_adapter_ops_t *p_ops; /** < \brief 驱动函数 */
    void                            *p_arg; /** < \brief 驱动函数参数 */
} am_microlib_adapter_t;

/*******************************************************************************
  本地全局变量定义
*******************************************************************************/

/** \brief Micro LIB适配器 */
static am_microlib_adapter_t __g_adapter;

#ifdef __MICROLIB

/** \brief 标准输入文件 */
FILE __stdin  = {0};

/** \brief 标准输出文件 */
FILE __stdout = {1};

/** \brief 标准错误文件 */
FILE __stderr = {2};

#endif /* __MICROLIB */

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
int am_microlib_adapter_init (const am_microlib_adapter_ops_t *p_ops,
                              void                            *p_arg)
{
    __g_adapter.p_ops = p_ops;
    __g_adapter.p_arg = p_arg;

    return AM_OK;
}

#endif /* __MICROLIB */

/* end of file */
