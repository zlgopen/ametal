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
 * \brief ZM516X 模块 KEY LED 例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 测试本 Demo 需要使用两块 AM824ZB 板，两块板需要下载不同的程序，
 *      其中一块 AM824ZB 板打开宏 USE_BORD0，关闭宏 USE_BORD1，编译并下载程序，
 *      另外一块 AM824ZB 板打开宏 USE_BORD1，关闭宏 USE_BORD0，编译并下载程序。
 *
 * - 实验现象：
 *   1. ZM516X 模块初始化并配置成功后 LED0 长亮，如果初始化失败，LED0 闪烁；
 *   2. 按下 KEY/RST 键，向对方发送 key_code。如果发送成功，将通过串口打印
 *      "send key code xx success"如果发送失败，将通过串口打印
 *      "send key code xx failed"。对方接收到之后，翻转 LED1，并通过串口
 *      打印 "received key code: xx"。
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
 * \snippet demo_zm516x_key_led.c src_zm516x_key_led
 *
 * \internal
 * \par Modification History
 * - 1.01 18-01-16  pea, use the send API with the ACK
 * - 1.00 17-09-20  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x_key_led
 * \copydoc demo_zm516x_key_led.c
 */

/** [src_zm516x_key_led] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_rngbuf.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_input.h"
#include "am_board.h"
#include <string.h>

//#define USE_BORD0                /**< \brief 使用该宏时，本例程用于单板 0 */
#define USE_BORD1              /**< \brief 使用该宏时，本例程用于单板 1 */

#define __MAX_KEY_BUF_SIZE   4   /**< \brief 缓冲区中最多能保存多少次按键数据 */

/** \brief 按键环形缓冲区结构 */
am_local struct am_rngbuf __g_key_rngbuf;

/** \brief 按键环形缓冲区，最多可存放 __MAX_KEY_BUF_SIZE 个键值 */
am_local char             __g_key_buf[sizeof(int) * __MAX_KEY_BUF_SIZE + 1];

/** \brief 接收环形缓冲区结构 */
am_local struct am_rngbuf __g_recv_rngbuf;

/** \brief 接收环形缓冲区，最多可存放 __MAX_KEY_BUF_SIZE 个键值 */
am_local char             __g_recv_buf0[sizeof(int) * __MAX_KEY_BUF_SIZE + 1];
am_local char             __g_recv_buf1[sizeof(int) * __MAX_KEY_BUF_SIZE];

/**
 * \brief 闪烁 LED0
 */
am_local void flash_led (void)
{
    while (1) {
        am_led_on(LED0);
        am_mdelay(50);

        am_led_off(LED0);
        am_mdelay(500);
    }
}

/**
 * \brief 按键回调函数类型
 *
 * \param[in] p_usr_data 用户数据，注册时设定的用户参数
 * \param[in] key_code   按键编码
 * \param[in] key_state  按键状态，AM_INPUT_KEY_STATE_PRESSED
 * \param[in] keep_time  按键长按时间
 *
 * \return 无
 */
am_local void __input_key_proc (void *p_arg, int key_code, int key_state, int keep_time)
{
    if (key_code == KEY_KP0) {
        if (key_state == AM_INPUT_KEY_STATE_PRESSED) {

            /* 判断按键缓冲区是否满 */
            if (am_rngbuf_freebytes(&__g_key_rngbuf) >= sizeof(key_code)) {
                am_rngbuf_put(&__g_key_rngbuf,
                               (const char *)&key_code,
                               sizeof(key_code));
            }
        }
    }
}

/**
 * \brief 按键初始化
 */
am_local void __key_init (void)
{
    am_local am_input_key_handler_t key_handler;

    am_input_key_handler_register(&key_handler, __input_key_proc, NULL);
}

/**
 * \brief 例程入口
 */
void demo_zm516x_key_led_entry (am_zm516x_handle_t zm516x_handle)
{

#ifdef USE_BORD0
    uint8_t              src_addr[2] = {0x20, 0x01};
    uint8_t              dst_addr[2] = {0x20, 0x02};
#endif

#ifdef  USE_BORD1
    uint8_t              src_addr[2] = {0x20, 0x02};
    uint8_t              dst_addr[2] = {0x20, 0x01};
#endif

    int                  ret = 0;
    int                  key_code;

    am_zm516x_addr_t     zb_addr;
    am_zm516x_cfg_info_t zm516x_cfg_info;

    AM_DBG_INFO("src_addr: 0x%02x%02x dst_addr: 0x%02x%02x\n",
                src_addr[0],
                src_addr[1],
                dst_addr[0],
                dst_addr[1]);

    /* 初始化按键 */
    __key_init();

    /* 初始化环形缓冲区 */
    am_rngbuf_init(&__g_key_rngbuf, __g_key_buf, sizeof(__g_key_buf));
    am_rngbuf_init(&__g_recv_rngbuf, __g_recv_buf0, sizeof(__g_recv_buf0));

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

    /* 设置 ZigBee 模块通道号（临时命令：D1） */
    if (am_zm516x_channel_set(zm516x_handle, 25) != AM_OK) {
        AM_DBG_INFO("am_zm516x_channel_set failed\r\n");
        flash_led();
    }

    /* 设置 ZigBee 模块目标地址（临时命令：D2） */
    zb_addr.p_addr    = dst_addr;
    zb_addr.addr_size = sizeof(dst_addr);
    if (am_zm516x_dest_addr_set(zm516x_handle, &zb_addr) != AM_OK) {
        AM_DBG_INFO("am_zm516x_dest_addr_set failed\r\n");
        flash_led();
    }

    /* 设置 ZigBee 模块接收的数据包包头是否显示源地址（临时命令：D3） */
    if (am_zm516x_display_head_set(zm516x_handle, AM_FALSE) != AM_OK) {
    	AM_DBG_INFO("am_zm516x_display_head_set failed\r\n");
        flash_led();
    }

    /* 设置 ZigBee 模块的通讯模式（临时命令：D9） */
    if (am_zm516x_mode_set(zm516x_handle, AM_ZM516X_COMM_UNICAST) != AM_OK) {
        AM_DBG_INFO("am_zm516x_mode_set failed\r\n");
        flash_led();
    }

    /* 获取 ZigBee 模块的配置信息（永久命令：D1） */
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_get failed\r\n");
        flash_led();
    }

    AM_DBG_INFO("dev_name:%s\r\n"
                "dev_pwd:%s\r\n"
                "dev_mode:%d\r\n"
                "chan:%d\r\n"
                "panid:0x%02x%02x\r\n"
                "my_addr:0x%02x%02x\r\n"
                "dst_addr:0x%02x%02x\r\n"
                "power_level:%d\r\n"
                "retry_num:%d\r\n"
                "tran_timeout:%d\r\n"
                "send_mode:%d\r\n",
                zm516x_cfg_info.dev_name,
                zm516x_cfg_info.dev_pwd,
                zm516x_cfg_info.dev_mode,
                zm516x_cfg_info.chan,
                zm516x_cfg_info.panid[0],
                zm516x_cfg_info.panid[1],
                zm516x_cfg_info.my_addr[0],
                zm516x_cfg_info.my_addr[1],
                zm516x_cfg_info.dst_addr[0],
                zm516x_cfg_info.dst_addr[1],
                zm516x_cfg_info.power_level,
                zm516x_cfg_info.retry_num,
                zm516x_cfg_info.tran_timeout,
                zm516x_cfg_info.send_mode);

    /*  配置完成，点亮 LED0 */
    am_led_on(LED0);

    AM_FOREVER {

        /* 判断按键缓冲区中是否有按键数据 */
        if (am_rngbuf_nbytes(&__g_key_rngbuf) >= sizeof(int)) {

            /* 从按键缓冲区中获取键值 */
            am_rngbuf_get(&__g_key_rngbuf,
                           (char *)&key_code,
                           sizeof(key_code));

            /* 发送键值 */
            if (am_zm516x_send_with_ack(zm516x_handle,
                                       &key_code,
                                        sizeof(key_code)) != sizeof(key_code)) {
                AM_DBG_INFO("send key code %d failed\r\n", key_code);

            } else {
                AM_DBG_INFO("send key code %d success\r\n", key_code);
            }
        }

        /*  am_zm516x_receive函数的读超时为10ms */
        ret = am_zm516x_receive(zm516x_handle,
                                __g_recv_buf1,
                                sizeof(__g_recv_buf1));
        if (ret > 0) {

            /* 判断接收缓冲区是否满 */
            if (am_rngbuf_freebytes(&__g_key_rngbuf) >= sizeof(key_code)) {
                am_rngbuf_put(&__g_recv_rngbuf,
                               __g_recv_buf1,
                               ret);
            }

            /* 判断接收缓冲区中是否有按键数据 */
            if (am_rngbuf_nbytes(&__g_recv_rngbuf) >= sizeof(int)) {

                /* 从接收缓冲区中获取键值 */
                am_rngbuf_get(&__g_recv_rngbuf,
                               (char *)&key_code,
                               sizeof(key_code));

                am_led_toggle(LED1);
                AM_DBG_INFO("received key code: %d\r\n", key_code);
            }
        }
    }
}
/** [src_zm516x_key_led] */

/* end of file */
