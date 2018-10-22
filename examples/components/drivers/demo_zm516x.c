/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZM516X 模块演示例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 测试本 Demo 需要使用两块 AM824ZB 板，两块板需要下载不同的程序，
 *      其中一块 AM824ZB 板打开宏 USE_BORD0，关闭宏 USE_BORD1，编译并下载程序，
 *      另外一块 AM824ZB 板打开宏 USE_BORD1，关闭宏 USE_BORD0，编译并下载程序。
 *
 * - 实验现象：
 *   1. 两块板初始化并配置成功后 LED0 长亮，如果初始化失败，LED0 闪烁；
 *   2. 两块板间隔 1S 向对方发送一次数据，接收函数收到对方发送过来的数据后，调试串口
 *      将接收到的数据内容输出，内容为 "received data from 0x****: I'm 0x**"。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 使用按键功能需要将 J14 的 KEY 和 PIO0_1 短接在一起；
 *    3. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    4. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE 定义为 1
 *       但该宏已经默认配置为 1， 用户不必再次配置；
 *    5. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_zm516x.c src_zm516x
 *
 * \internal
 * \par Modification History
 * - 1.01 18-01-16  pea, simplify demo, enable display head
 * - 1.00 17-05-26  mex, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x
 * \copydoc demo_zm516x.c
 */

/** [src_zm516x] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include <string.h>


#define USE_BORD0            /**< \brief 使用该宏时，本例程用于单板 0 */
//#define USE_BORD1          /**< \brief 使用该宏时，本例程用于单板 1 */

/**
 * \brief 闪烁 LED0
 */
static void flash_led (void)
{
    while (1) {
        am_led_on(LED0);
        am_mdelay(50);

        am_led_off(LED0);
        am_mdelay(500);
    }
}

/**
 * \brief 例程入口
 */
void demo_zm516x_entry (am_zm516x_handle_t zm516x_handle)
{
    char                 send_buf[11] = "I'm 0x00\r\n";
    am_zm516x_addr_t     zb_addr;
    int                  ret = 0;

#ifdef USE_BORD0
    uint8_t              src_addr[2] = {0x20, 0x01};
    uint8_t              dst_addr[2] = {0x20, 0x02};
#endif

#ifdef  USE_BORD1
    uint8_t              src_addr[2] = {0x20, 0x02};
    uint8_t              dst_addr[2] = {0x20, 0x01};
#endif

    uint8_t              buf[20] = {0};
    uint32_t             snd_tick = 0;
    am_zm516x_cfg_info_t zm516x_cfg_info;

    AM_DBG_INFO("src_addr: 0x%02x%02x dst_addr: 0x%02x%02x\n",
                src_addr[0],
                src_addr[1],
                dst_addr[0],
                dst_addr[1]);

    /* 恢复 ZM516X 模块出厂设置（永久命令：DA），设置成功需复位 */
    if (am_zm516x_default_set(zm516x_handle) != AM_OK) {
        AM_DBG_INFO("am_zm516x_default_set failed\r\n");
        flash_led();
    }

    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

    /* 获取 ZigBee 模块的配置信息（永久命令：D1） */
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_get failed\r\n");
        flash_led();
    }

    zm516x_cfg_info.my_addr[0] = src_addr[0];
    zm516x_cfg_info.my_addr[1] = src_addr[1];
    zm516x_cfg_info.dst_addr[0] = dst_addr[0];
    zm516x_cfg_info.dst_addr[1] = dst_addr[1];
    zm516x_cfg_info.panid[0] = 0x10;
    zm516x_cfg_info.panid[1] = 0x01;

    /* 修改 ZigBee 模块的配置信息（永久命令：D6），设置成功需复位 */
    if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_set failed\r\n");
        flash_led();
    }

    /* 使 ZigBee 模块复位（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

    /* 设置目标地址 */
    zb_addr.p_addr    = src_addr;
    zb_addr.addr_size = 2;

    /* 设置 ZigBee 模块接收的数据包包头是否显示源地址（永久命令：DC） */
    if (am_zm516x_cfg_display_head_set(zm516x_handle,
                                      &zb_addr,
                                       AM_TRUE) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_display_head_set failed\r\n");
        flash_led();
    }

    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

    /* 设置 ZM516X 模块接收的数据包包头是否显示源地址（临时命令：D3） */
    if (am_zm516x_display_head_set(zm516x_handle, AM_TRUE) != AM_OK) {
        AM_DBG_INFO("am_zm516x_display_head_set failed\r\n");
        flash_led();
    }

    /**
     * 设置 ZigBee 模块进入睡眠模式（临时命令：D8），需要单独测试，ZM516X 睡眠后
     * 只能复位模块或把模块的 WAKE 引脚拉低来唤醒模块
     */
//    am_zm516x_enter_sleep(zm516x_handle);

    /* 所有测试通过，点亮 LED0 */
    am_led_on(LED0);

    AM_FOREVER {

        if (snd_tick++ > 100) {
            snd_tick = 0;
            send_buf[7] = (src_addr[1] & 0x0F) + '0';
            am_zm516x_send(zm516x_handle, send_buf, strlen(send_buf));
        }

        /* am_zm516x_receive 函数的读超时为 10ms */
        ret = am_zm516x_receive(zm516x_handle, buf, sizeof(buf));

        if (ret > 0) {
            AM_DBG_INFO("received data from 0x%02x%02x: %s\r\n",
                        buf[0],
                        buf[1],
                        buf + 2);
            memset(buf, 0x00, sizeof(buf));
        }
    }
}
/** [src_zm516x] */

/* end of file */
