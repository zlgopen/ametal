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
 * \brief LoRa信道活跃检测程序，检测到则接受数据，若没有检测到则发送数据
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-31  sky, first implementation.
 * - 1.01 20-09-23  yg,  adaptation zsl42x.
 * \endinternal
 */

#include "am_board.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "string.h"

#include "am_zsl42x_lora.h"
#include "am_hwconf_zsl42x_lora.h"


/**
 * \brief lora服务句柄
 */
static am_zsl42x_lora_handle_t g_lora_handle;

#define LORA_RF_FREQUENCY            504000000  /**< \brief 信道频率 */
#define LORA_BANDWIDTH               0          /**< \brief 带宽125Khz */
#define LORA_SPREADING_FACTOR        AM_ZSL42X_LORA_SERVER_SF8     /**< \brief 信道速率 SFx */
#define LORA_CODINGRATE              AM_ZSL42X_LORA_SERVER_CR_4_5  /**< \brief 编码率 4/5 */
#define LORA_PREAMBLE_LENGTH         10         /**< \brief 前导码长度 */
#define LORA_FIX_LENGTH_PAYLOAD_ON   AM_FALSE   /**< \brief 禁能固定长度 */
#define LORA_SYMBOL_TIMEOUT          8          /**< \brief 符号超时数 */
#define LORA_IQ_INVERSION_ON         AM_FALSE   /**< \brief 禁能IQ信号翻转 */
#define LORA_TX_OUTPUT_POWER         17         /**< \brief 功率17dB */

/**
 * \brief CAD完成回调函数
 */
static void __on_cad_done (void *p_arg, uint8_t channel_activity_detected)
{
    if (channel_activity_detected == AM_TRUE) {
        am_kprintf("Channel is active. rx data\r\n");

        /* 接收配置 */
        am_zsl42x_lora_server_rx_config_set(g_lora_handle,
                                            AM_ZSL42X_LORA_MODEM_LORA,
                                            LORA_BANDWIDTH,
                                            LORA_SPREADING_FACTOR,
                                            LORA_CODINGRATE,
                                            0,
                                            LORA_PREAMBLE_LENGTH,
                                            LORA_SYMBOL_TIMEOUT,
                                            LORA_FIX_LENGTH_PAYLOAD_ON,
                                            0,
                                            AM_TRUE,
                                            0,
                                            0,
                                            LORA_IQ_INVERSION_ON,
                                            AM_FALSE);

        am_zsl42x_lora_server_receive(g_lora_handle, 3000);

    } else {

        am_kprintf("Channel is negative.tx data\r\n");
        am_zsl42x_lora_server_tx_config_set(g_lora_handle,
                                            AM_ZSL42X_LORA_MODEM_LORA,
                                            LORA_TX_OUTPUT_POWER,
                                            AM_FALSE,
                                            LORA_BANDWIDTH,
                                            LORA_SPREADING_FACTOR,
                                            LORA_CODINGRATE,
                                            LORA_PREAMBLE_LENGTH,
                                            LORA_FIX_LENGTH_PAYLOAD_ON,
                                            AM_TRUE,
                                            0,
                                            0,
                                            LORA_IQ_INVERSION_ON,
                                            3000);

        /* 发送数据 */
        am_zsl42x_lora_server_send(g_lora_handle,
                                   (uint8_t *)"hello,lora!",
                                   strlen("hello,lora!"));

    }
}

/**
 * \brief 接收完成回调函数
 */
static void __on_rx_done (void    *p_arg,
                          uint8_t *p_payload,
                          uint16_t size,
                          int16_t  rssi,
                          int8_t   snr)
{
    char  buffer[256] = {0};

    memcpy(buffer, p_payload, size);

    am_kprintf("rssi    : %d\r\n", rssi);
    am_kprintf("snr     : %d\r\n", snr);
    am_kprintf("payload : %s\r\n\r\n", buffer);
}

/**
 * \brief 接收超时回调函数
 */
static void __on_rx_timeout (void *p_arg)
{
    am_kprintf("RX Timeout!\r\n");
}

/**
 * \brief 发送超时回调函数
 */
static void __on_tx_done (void *p_arg)
{
    am_kprintf("TX Done!\r\n");
}

/**
 * \brief 发送超时回调函数
 */
static void __on_tx_timeout (void *p_arg)
{
    am_kprintf("TX Timeout!\r\n");
}

/**
 * \brief LoRa信道活跃测试程序入口
 */
void demo_zsl42x_core_lora_cad_rx_tx_entry (void)
{
    static am_zsl42x_lora_radio_events_t events;
    uint8_t                              status;

    g_lora_handle = am_zsl42x_lora_inst_init();

    am_zsl42x_lora_memset1((uint8_t *)&events,
                           0x00,
                           sizeof(am_zsl42x_lora_radio_events_t));

    /* 设置回调函数 */
    events.pfn_cad_done_cb   = __on_cad_done;
    events.pfn_rx_done_cb    = __on_rx_done;
    events.pfn_rx_timeout_cb = __on_rx_timeout;
    events.pfn_tx_done_cb    = __on_tx_done;
    events.pfn_tx_timeout_cb = __on_tx_timeout;

    am_zsl42x_lora_server_radio_events_set(g_lora_handle, &events);

    am_zsl42x_lora_server_tx_config_set(g_lora_handle,
                                        AM_ZSL42X_LORA_MODEM_LORA,
                                        LORA_TX_OUTPUT_POWER,
                                        AM_FALSE,
                                        LORA_BANDWIDTH,
                                        LORA_SPREADING_FACTOR,
                                        LORA_CODINGRATE,
                                        LORA_PREAMBLE_LENGTH,
                                        LORA_FIX_LENGTH_PAYLOAD_ON,
                                        AM_TRUE,
                                        0,
                                        0,
                                        LORA_IQ_INVERSION_ON,
                                        3000);

    /* 信道频率配置 */
    am_zsl42x_lora_server_channel_set(g_lora_handle, LORA_RF_FREQUENCY);

    while (1) {

        /* 启动CAD检测 */
        am_zsl42x_lora_server_status_get(g_lora_handle, &status);

        if (AM_ZSL42X_LORA_RF_IDLE_STATE == status) {
            am_zsl42x_lora_server_cad_start(g_lora_handle);
        }

        am_mdelay(200);

        am_led_toggle(LED0);
    }
}

/* end of file */
