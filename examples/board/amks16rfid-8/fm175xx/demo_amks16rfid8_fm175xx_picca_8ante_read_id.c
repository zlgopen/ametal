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
 * \brief fm175xx通过CD4051复用8条天线读A类卡的ID，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 正确连接好天线；
 *   3. 将 MiniPort-LED 板子直接与 AMKS16RFID-8 标号为J4的 MiniPort接口相连接；
 *   4. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印出卡片的ID；
 *   2. 如果天线感应区中有卡片则 MiniPort-LED 对应位置的LED会点亮。
 *
 * - 注意：
 *   1. 要使用该例程，需要在am_prj_config.h里面将AM_CFG_LED_ENABLE定义为0。
 *
 * \par 源代码
 * \snippet demo_amks16rfid8_fm175xx_picca_8ante_read_id.c src_amks16rfid8_fm175xx_picca_8ante_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-06  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_kl26_if_dr_fm175xx_picca_8ante_read_id
 * \copydoc demo_amks16rfid8_fm175xx_picca_8ante_read_id.c
 */

/** [src_amks16rfid8_fm175xx_picca_8ante_read_id] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "kl26_pin.h"
#include "am_kl26_inst_init.h"
#include "am_delay.h"
#include "am_led.h"
#include "am_fm175xx_ant.h"
#include "am_hwconf_fm175xx.h"
#include "demo_components_entries.h"
#include "demo_amks16rfid8_entries.h"

/**
 * \name 定义CD4051的引脚
 * @{
 */
#define __CD4051_PIN_EN PIOB_19
#define __CD4051_PIN_S2 PIOB_18
#define __CD4051_PIN_S1 PIOB_17
#define __CD4051_PIN_S0 PIOB_16

/* 定义 fm175xx 天线切换信息 */
static am_antenna_info_t  __g_antenna_info = {
    {
        __CD4051_PIN_EN,
        __CD4051_PIN_S0,
        __CD4051_PIN_S1,
        __CD4051_PIN_S2
    },
    AM_FM175XX_ANTENNA_EIGHT
};

/**
 * \brief 8天线读取A类卡卡号例程
 */
void demo_amks16rfid8_fm175xx_picca_8ante_read_id (void)
{
    uint8_t tag_type[2]  = { 0 };      /* ATQA */
    uint8_t uid[10]      = { 0 };      /* UID */
    uint8_t uid_real_len = 0;          /* 接收到的UID的长度 */
    uint8_t sak[3]       = { 0 };      /* SAK */
    uint8_t i, j;
    am_fm175xx_handle_t handle = am_fm175xx_inst_init();

    am_cd4051_pin_init(&__g_antenna_info);
    am_miniport_led_inst_init();

    while (1) {
        for (j = 0; j < 8; j++) {
            am_cd4051_channel_selected(&__g_antenna_info, j);   /* 选择天线 */

            if (AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_active\
                                             (handle,
                                              AM_FM175XX_PICCA_REQ_IDLE,
                                              tag_type,
                                              uid,
                                              &uid_real_len,
                                              sak)) {

                am_kprintf("antenna : %d\r\n", j + 1);
                am_kprintf("UID : ");
                for (i = 0; i < uid_real_len; i++) {
                    am_kprintf("%02x ", uid[i]);
                }
                am_kprintf("\r\n\r\n");
                am_led_on(j);
            } else {
                am_led_off(j);
            }
        }
    }
}

/** [src_amks16rfid8_fm175xx_picca_8ante_read_id] */

/* end of file */
