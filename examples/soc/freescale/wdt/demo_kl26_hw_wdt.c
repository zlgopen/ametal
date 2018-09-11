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
 * \brief WDT 喂狗和复位例子，通过硬件层实现
 *
 * - 实验现象：
 *   1. 看门狗设置超时时间为256ms，延时50ms后喂狗，程序正常执行，
 *      串口打印喂狗信息；
 *   2. 程序执行到延时1s时，喂狗超时，系统复位，程序重新开始执行。
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \par 源代码
 * \snippet demo_kl26_hw_wdt.c src_kl26_hw_wdt
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-30  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_hw_wdt
 * \copydoc demo_kl26_hw_wdt.c
 */

/** [src_kl26_hw_wdt] */
#include "ametal.h"
#include "am_delay.h"
#include "am_uart.h"
#include "am_kl26_inst_init.h"
#include "hw/amhw_kl26_sim.h"
#include "demo_fsl_entrys.h"

/**
 * \brief WDT入口函数，HW层实现
 *
 * \note 测试看门狗例程时请将am_prj_config.h文件中的宏AM_CFG_SIM_COP置为1
 *
 * \return 无
 */
void demo_kl26_hw_wdt_entry (void)
{
    am_uart_handle_t uhandle;          /**< \brief 串口标准服务句柄     */

    /* UART设备初始化，获取UART句柄值  */
    uhandle = am_kl26_uart0_inst_init();

    /* 配置看门狗 */
    amhw_kl26_sim_cop_cfg(KL26_SIM_COP_MODE_NORMAL,    /* 普通模式 */
                          KL26_SIM_COP_CLK_INTERNAL,   /* 内部1KHZ的LPO时钟 */
                          KL26_SIM_COP_TIMOUT_VALUE2); /* 超时时间设为256ms */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(50);

    amhw_kl26_sim_srvcop_set(0x55);     /* 喂狗，先写0x55 */
    amhw_kl26_sim_srvcop_set(0XAA);     /* 喂狗，再写0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("50ms后喂狗成功！\r\n"),
                      sizeof("50ms后喂狗成功！\r\n"));

    am_uart_poll_send(uhandle,
                     (uint8_t *)("等待喂狗...\r\n"),
                      sizeof("等待喂狗...\r\n"));

    am_mdelay(1000);

    /* 如果喂狗超时，下面语句将不被执行 */
    amhw_kl26_sim_srvcop_set(0x55);     /* 喂狗，先写0x55 */
    amhw_kl26_sim_srvcop_set(0XAA);     /* 喂狗，再写0xAA */

    am_uart_poll_send(uhandle,
                     (uint8_t *)("1s后喂狗成功！\r\n"),
                      sizeof("1s后喂狗成功！\r\n"));

    while(1) {
        am_uart_poll_send(uhandle,
                         (uint8_t *)("等待喂狗...\r\n"),
                          sizeof("等待喂狗...\r\n"));

        am_mdelay(1000);
    }
}

/** [src_kl26_hw_wdt] */

/* end of file */
