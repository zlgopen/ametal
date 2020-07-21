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
 * \brief 中断向量表
 *
 * \internal
 * \par modification history
 * - 1.02 17-12-11  pea, amend annotation
 * - 1.01 11-04-29  mifi, call SystemInit, and set the Vector Table Offset
*                   before copy of data and bss segment
 * - 1.00 11-04-09  mifi, first implementationS
 * \endinternal
 */

#define __CRT_C__

#include <stdint.h>

/***************************************************************************
  外部变量声明
***************************************************************************/
extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

/***************************************************************************
  外部函数声明
***************************************************************************/
extern  int main (void);

/***************************************************************************
  本地函数声明
***************************************************************************/
void SystemInit (void) __attribute__((weak));

/***************************************************************************
  本地函数定义
***************************************************************************/

/**
 * \brief SystemInit() 是 CMSIS 接口中需要的一个函数，如果没有这个函数链接时
 *        会出错，所以在这里定义一个 weak 函数
 */
void SystemInit (void)
{
    ; /* VOID */
}

/**
 * \brief 复位中断处理函数，初始化 C 运行环境，包括中断向量表、.data 段、.bss 段，
 *        最后调用 main() 函数
 */
void ResetHandler_function (void)
{
    uint32_t *p_src;
    uint32_t *p_dest;

    SystemInit();

    /* 中断向量表重定位 */
    *((uint32_t*)0xE000ED08) = (uint32_t)&_stext;

    /* 从 .text 段中读取数据初始化 .data 段 */
    p_src  = &_etext;
    p_dest = &_sdata;
    while (p_dest < &_edata) {
        *p_dest++ = *p_src++;
    }

    /* 初始化 .bss 段为 0 */
    p_dest = &_sbss;
    while (p_dest < &_ebss) {
        *p_dest++ = 0;
    }

    (void)main();

    /* 正常情况下不应该运行到这里 */
    while (1) {
        ; /* VOID */
    }
}

/* end of file */
