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
 * \brief LoRa 信道活跃测试例程工程
 *
 * - 实验现象:
 *   模板例程:LED0以1秒的频率闪烁; demo例程入口:详见demo入口函数文件介绍
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-16  win, first implementation
 * - 1.01 20-09-23  yg,  adaptation zsl42x.
 * \endinternal
 */

/**
 * \brief 例程入口
 */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_softimer.h"

#include "am_zsl42x_lora.h"
#include "am_hwconf_zsl42x_lora.h"


/**
 * \brief lora服务句柄
 */
static am_zsl42x_lora_handle_t g_lora_handle;

#define LORA_RF_FREQUENCY           504000000  /**< \brief 信道频率 */
#define LORA_TX_POWER               17         /**< \brief 发射功率17dB */
#define LORA_PREAMBLE_LENGTH        10         /**< \brief 前导码长度 */
#define LORA_BANDWIDTH              0          /**< \brief 125khz 带宽 */
#define LORA_SPREADING_FACTOR       AM_ZSL42X_LORA_SERVER_SF8     /**< \brief 信道速率 SFx */
#define LORA_CODINGRATE             AM_ZSL42X_LORA_SERVER_CR_4_5  /**< \brief 编码率 4/5 */
#define LORA_SYMBOL_TIMEOUT         8         /**< \brief 符号超时数 */
#define LORA_FIX_LENGTH_PAYLOAD_ON  AM_FALSE  /**< \brief 禁能固定长度 */
#define LORA_IQ_INVERSION_OFF       AM_FALSE  /**< \brief 禁能IQ信号翻转 */

/**
 * \brief CAD完成回调函数
 */
static void __on_cad_done (void *p_arg, uint8_t channel_activity_detected)
{
    if (channel_activity_detected == 1) {
        am_kprintf("Channel is active\r\n");
    } else {
        am_kprintf("Channel is negative\r\n");
    }
}

/**
 * \brief LoRa信道活跃测试程序入口
 */
void demo_zsl42x_core_lora_channel_activity_detect_entry (void)
{
    static am_zsl42x_lora_radio_events_t events;

    g_lora_handle = am_zsl42x_lora_inst_init();

    am_zsl42x_lora_memset1((uint8_t *)&events,
                           0x00,
                           sizeof(am_zsl42x_lora_radio_events_t));

    /* 设置回调函数 */
    events.pfn_cad_done_cb = __on_cad_done;
    am_zsl42x_lora_server_radio_events_set(g_lora_handle, &events);

    /* 信道速率配置 */
    am_zsl42x_lora_server_channel_set(g_lora_handle, LORA_RF_FREQUENCY);

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
                                        AM_FALSE,
                                        0,
                                        0,
                                        LORA_IQ_INVERSION_OFF,
                                        0);

    while (1) {

        am_zsl42x_lora_server_cad_start(g_lora_handle);

        am_mdelay(3000);

        am_led_toggle(LED0);
    }
}

/* end of file */
