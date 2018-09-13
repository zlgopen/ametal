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
 * \par 说明
 * 1. 内存管理接口只能在ARM LIB与ametal系统接口之间选择一个使用
 *
 * 在使用前，必须在"Project->Options for Target xxx->Target"中取消勾选"Use MicroLIB"
 * 并初始化适配器，若使用UART输出信息，则初始化函数的调用范例如下：
 *
 * \code
 * static int __dbg_fputc (void *p_arg, const char ch)
 * {  
 *     while (-AM_EAGAIN == am_uart_poll_putchar(p_arg, ch)) {
 *         ;
 *     }
 * 
 *     return ch;
 * }
 * 
 * static int __dbg_fgetc (void *p_arg)
 * {  
 *     char ch;
 *     
 *     while (-AM_EAGAIN == am_uart_poll_getchar(p_arg, &ch)) {
 *         ;
 *     }
 * 
 *     return ch;
 * }
 * 
 * static am_armlib_adapter_ops_t g_armlib_adapter_ops = {
 *     __dbg_fputc,
 *     __dbg_fgetc
 * };
 * 
 * int am_armlib_init (am_uart_handle_t uart_handle)
 * {
 *     return am_armlib_adapter_init(&g_armlib_adapter_ops, uart_handle);
 * }
 * \endcode
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-17  pea, first implementation.
 * \endinternal
 */
 
#ifndef __AM_ARMLIB_ADAPTER_H
#define __AM_ARMLIB_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_armlib_adapter
 * \copydoc am_armlib_adapter.h
 * @{
 */

/**
 * \brief __FILE结构,在<stdio.h>中被typedef为FILE类型
 */
struct __FILE {
    
    int handle; /**< \brief 文件句柄 */
    
};

/**
 * \brief 配置选项，根据实际硬件平台提供
 */
typedef struct am_armlib_adapter_ops {
    
    int (*pfn_putc) (void *p_arg, const char ch); /**< 函数指针，输出一个字符 */
    int (*pfn_getc) (void *p_arg);                /**< 函数指针，输入一个字符 */
    
} am_armlib_adapter_ops_t;

/**
 * \brief 适配器初始化
 *
 * \param[in] p_ops 驱动函数
 * \param[in] p_arg 驱动函数参数
 *
 * \return AM_OK
 */
int am_armlib_adapter_init (const am_armlib_adapter_ops_t *p_ops,
                            void                          *p_arg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ARMLIB_ADAPTER_H */

/* end of file */
