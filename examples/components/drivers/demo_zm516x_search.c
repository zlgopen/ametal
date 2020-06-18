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
 * \brief ZM516X 模块搜索例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口打印读取到的本地模块的配置信息；
 *   2. ZM516X 模块从 11 通道到 26 通道搜索其它模块。如果搜索到其它模块，获取该模块
 *      的信号强度和配置信息，并通过串口打印；
 *   3. ZM516X 模块初始化并搜索完成后 LED0 长亮，如果失败，LED0 闪烁；
 *   4. 将本地模块的通道恢复为搜索前的通道号。
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
 * \snippet demo_zm516x_search.c src_zm516x_search
 *
 * \internal
 * \par Modification History
 * - 1.00 18-01-15  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x_search
 * \copydoc demo_zm516x_search.c
 */

/** [src_zm516x_search] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"

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

/**
 * \brief 打印 ZM516X 模块的基本信息(通过搜索命令 D4 获得)
 */
am_local
void __base_info_print (am_zm516x_base_info_t *p_zm516x_base_info, uint8_t num)
{
    uint8_t i = 0;

    AM_DBG_INFO("\r\n__base_info_print\r\n");

    for (i = 0; i < num; i++) {
        AM_DBG_INFO("dev_mode:%d\r\n"
                    "chan:%d\r\n"
                    "data_rate:%d\r\n"
                    "panid:0x%02x%02x\r\n"
                    "addr:0x%02x%02x\r\n"
                    "state:0x%02x\r\n",
                  ((p_zm516x_base_info + i)->dev_mode[0] << 8) ||
                   (p_zm516x_base_info + i)->dev_mode[1],
                   (p_zm516x_base_info + i)->chan,
                   (p_zm516x_base_info + i)->data_rate,
                   (p_zm516x_base_info + i)->panid[0],
                   (p_zm516x_base_info + i)->panid[1],
                   (p_zm516x_base_info + i)->addr[0],
                   (p_zm516x_base_info + i)->addr[1],
                   (p_zm516x_base_info + i)->state);
        AM_DBG_INFO("\r\n");
    }
}

/**
 * \brief 打印 ZM516X 远程模块的配置信息(通过获取远程配置信息 D5 获得)
 */
am_local
void __remote_info_print (am_zm516x_remote_info_t *p_zm516x_remote_info)
{
    AM_DBG_INFO("\r\n__remote_info_print\r\n");
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
                p_zm516x_remote_info->cfg_info.dev_name,
                p_zm516x_remote_info->cfg_info.dev_pwd,
                p_zm516x_remote_info->cfg_info.dev_mode,
                p_zm516x_remote_info->cfg_info.chan,
                p_zm516x_remote_info->cfg_info.panid[0],
                p_zm516x_remote_info->cfg_info.panid[1],
                p_zm516x_remote_info->cfg_info.my_addr[0],
                p_zm516x_remote_info->cfg_info.my_addr[1],
                p_zm516x_remote_info->cfg_info.dst_addr[0],
                p_zm516x_remote_info->cfg_info.dst_addr[1],
                p_zm516x_remote_info->cfg_info.power_level,
                p_zm516x_remote_info->cfg_info.retry_num,
                p_zm516x_remote_info->cfg_info.tran_timeout,
                p_zm516x_remote_info->cfg_info.send_mode);

    AM_DBG_INFO("state:0x%02x\r\n"
                "dev_mode:%d\r\n"
                "version:0x%02x%02x\r\n",
                p_zm516x_remote_info->state,
                (p_zm516x_remote_info->dev_mode[0] << 8) ||
                p_zm516x_remote_info->dev_mode[1],
                p_zm516x_remote_info->version[0],
                p_zm516x_remote_info->version[1]);
    AM_DBG_INFO("\r\n");
}

/**
 * \brief 例程入口
 */
void demo_zm516x_search_entry (am_zm516x_handle_t zm516x_handle)
{
    uint8_t                 i              = 0;
    uint8_t                 j              = 0;
    uint8_t                 get_size       = 0;
    uint8_t                 signal         = 0;
    am_zm516x_base_info_t   base_info[5];
    am_zm516x_cfg_info_t    zm516x_cfg_info;
    am_zm516x_remote_info_t remote_info;
    am_zm516x_addr_t        zb_addr;

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

    /* 设置本地地址，本地地址不可与其他被搜索的模块地址相同 */
    uint8_t              src_addr[2] = {0x20, 0x02};

    zm516x_cfg_info.my_addr[0] = src_addr[0];
    zm516x_cfg_info.my_addr[1] = src_addr[1];

    /* 修改 ZigBee 模块的配置信息（永久命令：D6），设置成功需复位 */
    if (am_zm516x_cfg_info_set(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_set failed\r\n");
        flash_led();
    }
    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);
    __cfg_info_print(&zm516x_cfg_info);

    /* 设置目标地址 */
    zb_addr.p_addr    = zm516x_cfg_info.my_addr;
    zb_addr.addr_size = 2;

    /* 遍历所有通道 */
    for (i = 11; i <= 26; i++) {

        /* 设置 ZM516X 模块通道号（临时命令：D1） */
        if (am_zm516x_channel_set(zm516x_handle, i) != AM_OK) {
            AM_DBG_INFO("am_zm516x_channel_set failed\r\n");
            flash_led();
        }

        /* 搜索其它模块（永久命令：D4） */
        if (am_zm516x_discover(zm516x_handle,
                               5,
                               base_info,
                              &get_size,
                               1000) != AM_OK) {
            AM_DBG_INFO("channel %d has not found the device\r\n", i);
        } else {
            AM_DBG_INFO("\r\nchannel %d has found %d device\r\n", i, get_size);

            for (j = 0; j < get_size; j++) {
                AM_DBG_INFO("-------------the %d-------------\r\n", j);
                __base_info_print(&base_info[j], 1);

                /* 设置目标地址 */
                zb_addr.p_addr    = base_info[j].addr;
                zb_addr.addr_size = 2;

                /* 读取指定地址 ZM516X 模块的信号强度（临时命令：DA） */
                if (am_zm516x_sigal_get(zm516x_handle,
                                       &zb_addr,
                                       &signal) != AM_OK) {
                    AM_DBG_INFO("failed to get 0x%02x%02x signal\r\n");
                } else {
                    AM_DBG_INFO("signal: %ddBm\r\n", signal / 3 - 95);
                }

                /* 获取远程配置信息（永久命令：D5） */
                if (am_zm516x_remote_info_get(zm516x_handle,
                                             &zb_addr,
                                             &remote_info) != AM_OK) {
                    AM_DBG_INFO("failed to get 0x%02x%02x remote info\r\n",
                                base_info[j].addr[0],
                                base_info[j].addr[1]);
                } else {
                    __remote_info_print(&remote_info);
                }
            }
        }
    }

    AM_DBG_INFO("\r\nsearch complete, recovery the channal to %d\r\n",
                zm516x_cfg_info.chan);

    /* 设置 ZigBee 模块通道号（永久命令：D2） */
    if (am_zm516x_cfg_channel_set(zm516x_handle,
                                  zm516x_cfg_info.chan) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_channel_set failed\r\n");
        flash_led();
    }

    /* 配置完成，点亮 LED0 */
    am_led_on(LED0);

    AM_FOREVER {
        am_mdelay(100);
    }
}
/** [src_zm516x_search] */

/* end of file */
