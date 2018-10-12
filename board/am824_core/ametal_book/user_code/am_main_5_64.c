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
 * \brief 程序清单5.64   该程序是没有任何效果的
 *
 * 
 * \internal
 * \par Modification history
 * - 1.00 18-09-17  adw, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_spi.h"

static void __spi_msg_complete_callback (void *p_arg)
{
    // 消息处理完毕
}

int am_main(void)
{
    am_spi_message_t   spi_msg;           // 定义一个SPI消息结构体

    am_spi_msg_init (                     
        &spi_msg,                         
        __spi_msg_complete_callback,      // 消息处理完成回调函数
        NULL);                            // 未使用回调函数的参数p_arg，设置为NULL
    return 0;
}



/* end of file */

