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
 * \brief SC16IS7XX 例程
 *
 * - 操作步骤：
 *   1. 将 MicroPort SC16IS7XX 配板连接到 AM116-Core 的 MicroPort 接口。
 *   2. 将 宏__DEBUG_UART的值改为2
 *   3. 将 PA9、PA10相连与PC串口相连
 *
 * - 实验现象：
 *   1. PC端发送数据，串口打印出发送的测试结果。
 * 
 * \par 源代码
 * \snippet demo_sc16is7xx.c src_sc16is7xx
 *
 * \internal
 * \par Modification history
 * - 1.00  18-09-13  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_sc16is7xx
 * \copydoc demo_sc16is7xx.c
 */

/** [src_sc16is7xx] */
#include "ametal.h"
#include "am_delay.h"
#include "am_sc16is7xx.h"
#include "am_hwconf_sc16is7xx.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
void demo_sc16is7xx_entry (void)
{  
    char get_char[64] = {0};
    am_sc16is7xx_handle_t handle;

    handle = am_sc16is7xx_inst_init();

    while (1) {
        
        am_sc16is7xx_uart_poll_receive(handle, 0, get_char, 64);
        
        am_sc16is7xx_uart_poll_send(handle, 0, get_char, 64);
    
    }
}
/** [src_sc16is7xx] */

/* end of file */
