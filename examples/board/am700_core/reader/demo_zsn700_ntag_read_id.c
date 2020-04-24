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
 * \brief ZSN700 读卡器例程
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将NTAG卡置于天线感应区。

 *
 * \par 源代码
 * \snippet demo_zsn700_reader_read_id.c
 *
 * \internal
 * \par Modification history
 * - 1.00 20-04-02  htf, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_reader_read_id
 * \copydoc demo_zsn700_reader_read_id.c
 */

/** [src_std_led] */
#include "string.h"
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "am_zsn700_reader.h"
#include "demo_std_entries.h"
#include "demo_am700_core_entries.h"
#include "am_hwconf_zsn700_reader.h"

#define NTAG_CARD_OPREAT_PAGE 0x16

/**
 * \brief NTAG卡读卡类型和卡号例程
 */
void demo_zsn700_reader_ntag_read_id (void)
{
    uint8_t              tag_type[2]  = { 0 };      /* ATQA */
    uint8_t              uid[10]      = { 0 };      /* UID */
    uint8_t              uid_real_len =   0;        /* 接收到的UID的长度 */
    uint8_t              sak[3]       = { 0 };      /* SAK */
    uint8_t              tx_buf[10]   = { 0 };
    uint8_t              rx_buf[20]   = { 0 };
    uint8_t              key[4]     = {0xff, 0xff, 0xff, 0xff};
    uint32_t             rx_len = 0;
    uint8_t              ret = 0;

    am_zsn700_reader_handle_t handle = am_zsn700_reader_inst_init();
    while (1) {
        int i ;
        if (AM_ZSN700_READER_STATUS_SUCCESS == am_zsn700_reader_picca_active\
                                                  (handle,
                                                    AM_ZSN700_READER_PICCA_REQ_IDLE,
                                                    tag_type,
                                                    uid,
                                                   &uid_real_len,
                                                    sak)) {
            am_kprintf("ATQA :%02x %02x\n", tag_type[0], tag_type[1]);

            /* 打印UID */
            am_kprintf("UID  :");
            for (i = 0; i < uid_real_len; i++) {
                am_kprintf("%02x ", uid[i]);
            }
            am_kprintf("\n");

            /* 打印SAK */
            am_kprintf("SAK  :");
            for (i = 4; i <= uid_real_len; i+=3) {
                am_kprintf("%02x ", sak[(i - 4) / 3]);
            }
            am_kprintf("\n\n");

            /* 密钥验证*/
            tx_buf[0] = 6;
            tx_buf[1] = 0x1b;
            memcpy(&tx_buf[2], &key[0], 4);
            ret = am_zsn700_reader_mifare_ultralight_transfer(
                    handle, tx_buf, rx_buf, &rx_len);

            /* 读取数据  */
            tx_buf[0] = 3;
            tx_buf[1] = 0x30;
            tx_buf[2] = NTAG_CARD_OPREAT_PAGE;
            ret = am_zsn700_reader_mifare_ultralight_transfer(
                    handle, tx_buf, rx_buf, &rx_len);

            am_kprintf("Read page %d data:", NTAG_CARD_OPREAT_PAGE);
            for(i = 0 ; i < rx_len; i ++){
                am_kprintf("%02x ", rx_buf[i]);
            }
            am_kprintf("\n\n");

            tx_buf[0] = 0x07;
            tx_buf[1] = 0xa2;
            tx_buf[2] = 0x16;
            tx_buf[3] = 0x01;
            tx_buf[4] = 0x02;
            tx_buf[5] = 0x03;
            tx_buf[6] = 0x04;
            ret = am_zsn700_reader_mifare_ultralight_transfer(
                    handle, tx_buf, rx_buf, &rx_len);
            if(ret == AM_OK){
                am_kprintf("Date write %d page success.\n", NTAG_CARD_OPREAT_PAGE);
                /* 读取数据  */
                tx_buf[0] = 3;
                tx_buf[1] = 0x30;
                tx_buf[2] = 0x16;
                ret = am_zsn700_reader_mifare_ultralight_transfer(
                        handle, tx_buf, rx_buf, &rx_len);

                am_kprintf("Read page %d data:", NTAG_CARD_OPREAT_PAGE);
                for(i = 0 ; i < rx_len; i ++){
                    am_kprintf("%02x ", rx_buf[i]);
                }
                am_kprintf("\n\n");
            }else{
                am_kprintf("Date write %d page fail.\n", NTAG_CARD_OPREAT_PAGE);
            }
        }
        am_mdelay(200);
    }
}
/** [src_std_led] */

/* end of file */
