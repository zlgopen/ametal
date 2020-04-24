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
 * \brief zsn700_reader读B类卡的ID，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将B类卡（如二代身份证）置于天线感应区。
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印出卡片的ID。
 *
 * - 注意：
 *   1. 将卡片放置天线感应区后串口只能打印一次ID，若要再次打印ID需要将卡片移除感应区再移入;
 *   2. fm17510不支持B类卡的读写。
 *
 * \par 源代码
 * \snippet demo_zsn700_reader_piccb_read_id.c src_zsn700_reader_piccb_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 18-02-05  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_reader_piccb_read_id
 * \copydoc demo_zsn700_reader_piccb_read_id.c
 */

/** [src_zsn700_reader_piccb_read_id] */
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsn700_reader.h"
#include "am_zsn700_reader_reg.h"
#include "am_hwconf_zsn700_reader.h"
 
/**
 * \brief B类卡读卡号例程
 */
void demo_zsn700_reader_piccb_read_id (void)
{ 
    uint8_t uid[12]     = { 0 };       /* UID */
    uint8_t id_card[12] = { 0 };
    uint8_t i;
    am_zsn700_reader_handle_t handle = am_zsn700_reader_inst_init();

    am_zsn700_reader_config(handle, AM_ZSN700_READER_PROT_TYPE_ISO14443B_106);

    while (1) {
        if (!am_zsn700_reader_piccb_active(handle,
                                           AM_ZSN700_READER_PICCB_REQ_IDLE,
                                           0x00,
                                           uid)) {
            am_kprintf("PUPI : ");
            for(i = 0; i < 4; i ++){
                am_kprintf("%02x ", uid[i]);
            }
            am_kprintf("\r\n");
            am_kprintf("UID : ");
            for(i = 4; i < 12; i ++){
                am_kprintf("%02x ", uid[i]);
            }
            am_kprintf("\r\n");

            // 如果该卡为身份即可使用该函数进行物理ID读取
            am_zsn700_reader_idcard_readr_id(handle, id_card);
            am_kprintf("IDCard ID : ");
            for(i = 0; i < 10; i ++){
                am_kprintf("%02x ", id_card[i]);
            }
            am_kprintf("\r\n");
        }
        am_mdelay(500);
    }
}

/** [src_zsn700_reader_piccb_read_id] */

/* end of file */
