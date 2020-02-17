/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief GS200 模块切换波特率例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 单片机切换波特率连接 GS200 模块；
 *   2. 如果切换成功，通过串口打印当前的波特率，并检测手势动作（成功检测手势动作后将切换 LED1 状态）；
 *      否则 LED0 按 100 ms 的间隔快闪烁。
 *
 * \note
 *   LED0 需要短接 J9 跳线帽，才能被 PIOB_1 控制；LED1 需要短接 J10 跳线帽，才能被 PIOB_2 控制；
 *
 *
 * \par 源代码
 * \snippet demo_gs200_baudrate.c src_gs200_baudrate
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-23  fzb, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_gs200_baudrate
 * \copydoc demo_gs200_baudrate.c
 */

/** [src_gs200_baudrate] */
#include "ametal.h"
#include "am_gs200.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_led.h"

/**
 * \brief 快闪烁 LED0
 */
static void flash_led (void)
{
    while (1) {
        am_led_on(0);
        am_mdelay(100);
        am_led_off(0);
        am_mdelay(100);
    }
}

/**
 * \brief 例程入口
 */
void demo_gs200_baudrate_entry (am_gs200_handle_t gs200_handle,uint32_t gs200_baudrate)
{
    int32_t     i                       = 0;
    int         ret                     = AM_ERROR;
    uint32_t    gs200_baudrate_tab[]    = {9600, 4800, 115200};
    uint8_t     recv_buf[5]             = {0};

    /* 切换 GS200 的波特率 */
    ret = am_gs200_ioctl(gs200_handle,
                         AM_GS200_SET_BAUDRATE,
                        (void *)gs200_baudrate);

    if (ret == AM_OK) {
        AM_DBG_INFO("GS200 Set Baudrate: %d\r\n", gs200_baudrate_tab[gs200_baudrate]);
    } else {
        AM_DBG_INFO("GS200 Set Baudrate Failed!\r\n");
        flash_led();
    }

    /* 切换与 GS200 通信的串口的波特率 */
    am_uart_ioctl(gs200_handle->uart_handle,
                  AM_UART_BAUD_SET,
                  (void *)(gs200_baudrate_tab[gs200_baudrate]));

    /*
     *  在进行波特率修改时，若指令无误，模块将会在1ms内返回相同的指令，若
     *  模块修改新设置的波特率参数成功，模块将在10ms后以新设置的波特率再次返回相同的
     *  设置指令。
     */
    if(am_gs200_recv(gs200_handle, recv_buf, 5) == 5) {
        for (i = 0; i < 5; i++) {
            if (recv_buf[i] != gs200_handle->p_devinfo->p_cmd_txbuf[i]) {
                break;
            }
        }
        if (i == 5) {
            AM_DBG_INFO("GS200 Switch Baudrate Successful!\r\n");
        } else {
            AM_DBG_INFO("GS200 Switch Baudrate Failed!\r\n");
            flash_led();
        }
    }

    /* 设置GS200检测悬停手势动作使能 */
    ret = am_gs200_ioctl(gs200_handle,
                         AM_GS200_SET_GESTURE_STATE,
                        (void *)AM_GS200_SET_HOVER_ENABLE);

    if (ret == AM_OK) {
        AM_DBG_INFO("GS200 Set HOVER Successful!\r\n");
        for (i = 0; i < 5; i++) {
            AM_DBG_INFO("Cmd TxBuffer[%d] = 0x%02x \r\n", i, gs200_handle->p_devinfo->p_cmd_txbuf[i]);
        }
        AM_DBG_INFO("\r\n");
    } else {
        AM_DBG_INFO("GS200 Set HOVER Failed!\r\n");
        flash_led();
    }

    /* 设置GS200悬停指令定时输出使能 */
    ret = am_gs200_ioctl(gs200_handle,
                         AM_GS200_SET_HOVERING_GESTURE_MODE,
                        (void *)AM_GS200_SET_OUTPUT_TIMING_ENABLE);

    if (ret == AM_OK) {
        AM_DBG_INFO("GS200 Timing output hovering gesture Successful!\r\n");
        for (i = 0; i < 5; i++) {
            AM_DBG_INFO("Cmd TxBuffer[%d] = 0x%02x \r\n", i, gs200_handle->p_devinfo->p_cmd_txbuf[i]);
        }
        AM_DBG_INFO("\r\n");
    } else {
        AM_DBG_INFO("GS200 Timing output hovering gesture Failed!\r\n");
        flash_led();
    }

    /* 设置GS200悬停手势定时输出时间间隔 */
    ret = am_gs200_ioctl(gs200_handle,
                         AM_GS200_SET_HOVERING_TIMING_TIME,
                        (void *)1000);

    if (ret == AM_OK) {
        AM_DBG_INFO("GS200 Set interval timing output hovering gesture：%d ms\r\n",(gs200_handle->p_devinfo->p_cmd_txbuf[3] * 50));
        for (i = 0; i < 5; i++) {
            AM_DBG_INFO("Cmd TxBuffer[%d] = 0x%02x \r\n", i, gs200_handle->p_devinfo->p_cmd_txbuf[i]);
        }
        AM_DBG_INFO("\r\n");
    } else {
        AM_DBG_INFO("GS200 Set interval timing output hovering gesture Failed!\r\n");
        flash_led();
    }

    while (1) {

        if(am_gs200_recv(gs200_handle, recv_buf, 5) == 5) {
            for (i = 0; i < 5; i++) {
                AM_DBG_INFO("Receive Buffer[%d] = 0x%02x \r\n", i, recv_buf[i]);
            }
            AM_DBG_INFO("\r\n");
            am_led_toggle(1);
        }

    }
}

/** [src_gs200_baudrate] */

/* end of file */
