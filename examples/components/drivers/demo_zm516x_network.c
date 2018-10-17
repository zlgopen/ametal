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
 * \brief ZM516X 模块组网例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 测试本 Demo 最少需要使用两块 AM824ZB 板，主机只能有 1 个，从机可以有多个，
 *      主机和从机需要下载不同的程序，
 *      作为主机的开发板打开宏 MASTER_BORD_USE，关闭宏 SLAVE_BORD_USE，编译并下载，
 *      作为从机的开发板打开宏 SLAVE_BORD_USE，关闭宏 MASTER_BORD_USE，编译并下载；
 *   2. 将从机的 JOIN 跳线帽短接 3 秒以下，使从机进入入网申请阶段；
 *   3. 按下主机的 KEY/RES 按键，使主机进入 __JOIN_TIME 秒的组网模式；
 *   4. 等待主机退出组网模式后，按下从机的 KEY/RES 按键。
 *
 * - 实验现象：
 *   1. 如果 ZM516X 模块初始化并配置失败，LED0 闪烁；
 *   2. 串口打印读取到的本地模块配置信息；
 *   3. 串口打印提示信息；
 *   4. 主机打印从机列表与从机信息；
 *   5. 主机向从机发送 "I'am Master\r\n";
 *   5. 从机向主机发送 "I'am Slave x\r\n"，其中 x 为从机网络地址;
 *   3. 主机和从机分别通过调试串口打印接收到的数据。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_zm516x_network.c src_zm516x_network
 *
 * \internal
 * \par Modification History
 * - 1.00 18-01-15  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x_network
 * \copydoc demo_zm516x_network.c
 */

/** [src_zm516x_network] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "am_lpc82x.h"
#include <stdio.h>

#define MASTER_BORD_USE    /**< \brief 使用该宏时，本例程用于主机 */
//#define SLAVE_BORD_USE     /**< \brief 使用该宏时，本例程用于从机 */

#ifdef MASTER_BORD_USE
#define __TYPE       0     /**< \brief 组网时为主机 */
#else
#define __TYPE       1     /**< \brief 组网时为从机 */
#endif

#define __JOIN_TIME  3     /**< \brief 主机允许从机加入网络的时间(单位为秒) */

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
 * \brief 打印 ZM516X 模块的配置信息
 */
am_local void __cfg_info_print (am_zm516x_cfg_info_t *p_zm516x_cfg_info)
{
    AM_DBG_INFO("\r\n__cfg_info_print\r\n");
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
                p_zm516x_cfg_info->dev_name,
                p_zm516x_cfg_info->dev_pwd,
                p_zm516x_cfg_info->dev_mode,
                p_zm516x_cfg_info->chan,
                p_zm516x_cfg_info->panid[0],
                p_zm516x_cfg_info->panid[1],
                p_zm516x_cfg_info->my_addr[0],
                p_zm516x_cfg_info->my_addr[1],
                p_zm516x_cfg_info->dst_addr[0],
                p_zm516x_cfg_info->dst_addr[1],
                p_zm516x_cfg_info->power_level,
                p_zm516x_cfg_info->retry_num,
                p_zm516x_cfg_info->tran_timeout,
                p_zm516x_cfg_info->send_mode);
    AM_DBG_INFO("\r\n");
}

#ifdef MASTER_BORD_USE

/**
 * \brief 打印 ZM516X 从机模块信息(通过查询主机模块存储的从机信息命令 E7 获得)
 */
am_local void __slave_info_print (am_zm516x_slave_info_t *p_zm516x_slave_info)
{
    AM_DBG_INFO("\r\n__slave_info_print\r\n");
    AM_DBG_INFO("mac: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\r\n"
                "addr:0x%02x%02x\r\n",
                p_zm516x_slave_info->mac[0],
                p_zm516x_slave_info->mac[1],
                p_zm516x_slave_info->mac[2],
                p_zm516x_slave_info->mac[3],
                p_zm516x_slave_info->mac[4],
                p_zm516x_slave_info->mac[5],
                p_zm516x_slave_info->mac[6],
                p_zm516x_slave_info->mac[7],
                p_zm516x_slave_info->addr[0],
                p_zm516x_slave_info->addr[1]);
}
#endif /* MASTER_BORD_USE */

/**
 * \brief 例程入口
 */
void demo_zm516x_network_entry (am_zm516x_handle_t zm516x_handle)
{
    char                   recv_buf[21]   = {0};
    uint8_t                state          = 0;
    uint8_t                response_state = 0;
    am_zm516x_cfg_info_t   zm516x_cfg_info;

#ifdef MASTER_BORD_USE
    uint8_t                i              = 0;
    uint8_t                get_size       = 0;
    am_zm516x_addr_t       zb_addr;
    am_zm516x_slave_info_t slave_info[5];
#else
    char                   send_buf[21]   = {0};
#endif /* MASTER_BORD_USE */

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
    __cfg_info_print(&zm516x_cfg_info);

    /* 设置自组网功能（永久命令：E5），设置成功需复位 */
    if (am_zm516x_cfg_network_set(zm516x_handle,
                                  1,
                                  __TYPE) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_network_set failed\r\n");
        flash_led();
    }
    am_mdelay(500);

    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

#ifdef MASTER_BORD_USE

    AM_DBG_INFO("Pressed KEY/RES To Enable JOIN %d Seconds!\r\n", __JOIN_TIME);
    while(am_gpio_get(PIO0_1) != 0);

    /* 主机允许从机加入网络（永久命令：E6） */
    if (am_zm516x_cfg_join_enable(zm516x_handle,
                                  __JOIN_TIME) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_join_enable failed\r\n");
        flash_led();
    }

    AM_DBG_INFO("please wait %d seconds!\r\n", __JOIN_TIME);
    am_mdelay(__JOIN_TIME * 1000);

    /* 查询主机模块存储的从机信息（永久命令：E7） */
    if (am_zm516x_cfg_slave_get(zm516x_handle,
                                5,
                               &slave_info[0],
                               &get_size) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_slave_get failed\r\n");
        flash_led();
    } else {
        AM_DBG_INFO("slave number is %d\r\n", get_size);
        for (i = 0; i < get_size; i++) {
            AM_DBG_INFO("-------------the %d-------------\r\n", i);
            __slave_info_print(&slave_info[i]);
        }
    }
#else

    AM_DBG_INFO("Please short JOIN pin less than 3 seconds,"
                "waiting to join the network press the KEY/RES to continue!\r\n");
    while(am_gpio_get(PIO0_1) != 0);
#endif /* MASTER_BORD_USE */

    /* 查询主从机状态（永久命令：E8） */
    if (am_zm516x_cfg_state_get(zm516x_handle,
                               &state,
                               &response_state) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_state_get failed\r\n");
        flash_led();
    }

    AM_DBG_INFO("state: %d, response_state: %d\r\n", state, response_state);

    /* 获取 ZigBee 模块的配置信息（永久命令：D1） */
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_get failed\r\n");
        flash_led();
    }
    __cfg_info_print(&zm516x_cfg_info);

    /* 配置完成，点亮 LED0 */
    am_led_on(LED0);

    AM_FOREVER {

#ifdef MASTER_BORD_USE
        if (am_rngbuf_nbytes(&zm516x_handle->uart_rngbuf_dev.rx_rngbuf)
                                                                    >= 20) {
            if (am_zm516x_receive(zm516x_handle, recv_buf, 20) > 0) {
                AM_DBG_INFO(recv_buf);
            }
        }

        for (i = 0; i < get_size; i++) {

            /* 设置目标地址 */
            zb_addr.p_addr    = slave_info[i].addr;
            zb_addr.addr_size = 2;

            /* 设置 ZM516X 模块目标地址（临时命令：D2） */
            if (am_zm516x_dest_addr_set(zm516x_handle,
                                       &zb_addr) != AM_OK) {
                AM_DBG_INFO("am_zm516x_dest_addr_set failed\r\n");
                continue;
            }

            am_zm516x_send(zm516x_handle, "I'am Master\r\n", 14);
        }
#else
        if (am_rngbuf_nbytes(&zm516x_handle->uart_rngbuf_dev.rx_rngbuf) >= 14) {
            if (am_zm516x_receive(zm516x_handle, recv_buf, 14) > 0) {
                AM_DBG_INFO(recv_buf);
            }
        }

        sprintf(send_buf,
                "I'am Slave 0x%02x%02x\r\n",
                zm516x_cfg_info.my_addr[0],
                zm516x_cfg_info.my_addr[1]);
        am_zm516x_send(zm516x_handle, send_buf, 20);
#endif /* MASTER_BORD_USE */

        am_mdelay(500);
    }
}
/** [src_zm516x_network] */

/* end of file */
