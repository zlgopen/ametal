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
 * \snippet demo_fsl_hw_backdoor_sec.c src_fsl_hw_backdoor_sec
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_backdoor_sec
 * \copydoc demo_fsl_hw_backdoor_sec.c
 */

/** [src_fsl_hw_backdoor_sec] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_ftfa.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/** \brief 后门安全密钥 */
uint8_t backdoor_accesskey[8];

/**
 * \brief 例程入口
 */
void demo_fsl_hw_backdoor_sec_entry (amhw_fsl_ftfa_t *p_hw_ftfa,
                                     am_uart_handle_t uart_handle)
{
    uint8_t key = 0;
    int32_t ret = 0;
    int i = 0;

    AM_DBG_INFO("Please input BackdoorAccess Key!\r\n");
    
    /* 接收字符 */
    am_uart_poll_receive(uart_handle, backdoor_accesskey, sizeof(backdoor_accesskey));

    AM_DBG_INFO("BackdoorAccess Key is:\r\n ");

    for (i = 0; i < 8; i++) {
        AM_DBG_INFO("0x%x  ", backdoor_accesskey[i]);
    }

    AM_DBG_INFO("\r\n");

    /* 使能FLASH忙延时等待 */
   // amhw_kl26_mcm_flash_stalling_enable(KL26_MCM);

    key = am_int_cpu_lock();
    /** 将flash命令执行函数拷贝到RAM */
    amhw_fsl_ftfa_func_copy();
    am_int_cpu_unlock (key);


    key = am_int_cpu_lock();
    /* 将flash命令执行函数拷贝到RAM */
    ret = amhw_fsl_ftfa_backdoor_access_key_verify(p_hw_ftfa, backdoor_accesskey);
    am_int_cpu_unlock (key);

    if (ret != 0) {
        AM_DBG_INFO(" Verify BackdoorAccess Key Fail!\r\n");
        while(1);
    }


    while (1) {

        am_led_toggle(LED0);
        am_mdelay(200);
    }
}


/** [src_fsl_hw_backdoor_sec] */

/* end of file */
