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
 * \brief zsn700_reader写块数据，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 先用密钥A对卡片指定的块进行验证，如果验证不通过则打印key A authent failed。
 *   2. 如果验证成功，则将缓冲区的数据写入指定的块后清空缓冲区，再将块中的数据读取出来，如果与写入的数据
 *      一致，表明写数据成功。
 *
 * \par 源代码
 * \snippet demo_zsn700_reader_picca_write_block.c src_zsn700_reader_picca_write_block
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-21  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_reader_picca_write_block
 * \copydoc demo_zsn700_reader_picca_write_block.c
 */

/** [src_zsn700_reader_picca_write_block] */
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsn700_reader.h"
#include "am_zsn700_reader_reg.h"
#include "am_hwconf_zsn700_reader.h"
 
/**
 * \brief A类卡写块例程
 */
void demo_zsn700_reader_picca_write_block (void)
{
    uint8_t tag_type[2]  = { 0 };      /* ATQA */
    uint8_t uid[10]      = { 0 };      /* UID */
    uint8_t uid_real_len =   0;        /* 接收到的UID的长度 */
    uint8_t sak[3]       = { 0 };      /* SAK */
    uint8_t keya[6]      = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  /* 验证密钥A */
    uint8_t buff[16]     = { 0 };         /* 用于存储块数据的缓冲区 */
    uint8_t i;
    uint8_t blocknum = 10;            /* 要读写的块 */
    am_zsn700_reader_handle_t handle = am_zsn700_reader_inst_init();
    while (1) {

        /* 寻所有未休眠（halt）的卡 */
        if (AM_ZSN700_READER_STATUS_SUCCESS == am_zsn700_reader_picca_active\
                                         (handle,
                                          AM_ZSN700_READER_PICCA_REQ_IDLE,
                                          tag_type,
                                          uid,
                                          &uid_real_len,
                                          sak)) {
            am_kprintf("actived\n");

            /* 验证A密钥 */
            if (AM_ZSN700_READER_STATUS_SUCCESS == am_zsn700_reader_picca_authent\
                                             (handle,
                                              AM_ZSN700_READER_IC_KEY_TYPE_A,
                                              uid,
                                              keya,
                                              blocknum)) {
                /* 写缓冲区 */
                for (i = 0; i < sizeof(buff); i++) {
                    buff[i] = i;
                }
                /* 写块数据 */
                am_zsn700_reader_picca_write(handle, blocknum, buff);

                /* 清空缓冲区 */
                for (i = 0; i < sizeof(buff); i++) {
                    buff[i] = 0;
                }

                /* 读块数据 */
                am_zsn700_reader_picca_read(handle, blocknum, buff);

                am_kprintf("block%d: ", blocknum);
                for (i = 0; i < 16; i++) {
                    am_kprintf("%x ", buff[i]);
                }
                am_kprintf("\n");

            } else {
                am_kprintf("key A authent failed\n\n");
            }
        }
        am_mdelay(200);
    }
}

/** [src_zsn700_reader_picca_write_block] */

/* end of file */
