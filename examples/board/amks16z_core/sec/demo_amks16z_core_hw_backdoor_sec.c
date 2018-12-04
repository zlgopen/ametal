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
 * \brief 后门安全访问flash例程
 *
 * - 实验现象：
 *   1. 串口打印出提示信息 Please input Verify BackdoorAccess Key!
 *   2. 当 Verify BackdoorAccess Key 不匹配时，串口输出 Verify BackdoorAccess Key Fail!
 *   3. 密钥匹配可以读出flash的内容，SWD调试口开放，LED闪烁
 *
 * - 操作步骤
 *   1. 如果是在eclipse下编译则将am_kl26_gcc_vector中的宏AM_KL26_FLASH_SEC_BACKDOOR打开，
 *      如果是在keil下编译则将am_kl26_armcc_startup.s中FSEC的值改为0x82（大约在第225行）。
 *   2. 当串口输出提示信息时，在字符串模式下发送43218765。
 *
 * \note: 后门安全密钥是通过flash config配置段的 gFlashConfig 这一个变量前8个字节的内容决定，请
 *        用户谨慎操作这个demo，以免锁死芯片
 *
 * \par 源代码
 * \snippet demo_amks16z_core_hw_backdoor_sec.c src_amks16z_core_hw_backdoor_sec
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_amks16z_core_if_hw_backdoor_sec
 * \copydoc demo_amks16z_core_hw_backdoor_sec.c
 */

/** [src_amks16z_core_hw_backdoor_sec] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "demo_fsl_entrys.h"
#include "am_kl26_inst_init.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"
#include "demo_amks16z_core_all_entries.h"

/**
 * \brief 例程入口
 */
void demo_amks16z_core_hw_backdoor_sec_entry (void)
{

    am_uart_handle_t uart_handle;

    AM_DBG_INFO("demo amks16z_core hw backdoor sec!\r\n");

    uart_handle = am_kl26_uart0_inst_init();

    demo_fsl_hw_backdoor_sec_entry(KL26_FTFA, uart_handle);
}


/** [src_amks16z_core_hw_backdoor_sec] */

/* end of file */
