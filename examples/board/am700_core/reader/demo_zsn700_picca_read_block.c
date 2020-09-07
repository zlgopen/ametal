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
 * \brief zsn700_reader读块数据，通过驱动接口实现
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将A类卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 先用密钥A对卡片的块2进行验证，如果验证不通过则打印key A authent failed。
 *   2. 如果验证成功，则读取块0（包含卡片序列号）、块3（包含密钥A，存取控制部分和密钥B）、块4（和块2
 *      不在同一个扇区），并打印出来。
 *
 * - 注意：
 *   1. 每个扇区包含4个块，其中第0到第2个块为数据块，第3个块为控制块。验证其实是对扇区而言的，但是给
 *      am_zsn700_reader_picca_m1_authent()函数传入的最后一个参数为块地址，因此该函数的意义
 *      其实是验证传入的块所在的扇区，验证成功后便可以根据控制块中的控制权限对本扇区所有的块进行读写。
 *   2. 块0中的数据是卡片序列号，该块只读。
 *   3. 块3位控制块，虽然本例传入的A密钥为全F，但是读取出来的A密钥却为全0，因为A密钥在任何情况下都没有
 *      读的权限。
 *   4. 块4与验证的块2不在同一个扇区，因此读出来的数据是不正确的，除非对块4所在的扇区进行验证。
 *
 * \par 源代码
 * \snippet demo_zsn700_reader_picca_read_block.c src_zsn700_reader_picca_read_block
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-21  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_reader_picca_read_block
 * \copydoc demo_zsn700_reader_picca_read_block.c
 */

/** [src_zsn700_reader_picca_read_block] */
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsn700_reader.h"
#include "am_zsn700_reader_reg.h"
#include "am_hwconf_zsn700_reader.h"
 
/**
 * \brief A类卡读块例程
 */
void demo_zsn700_reader_picca_read_block (void)
{
    uint8_t tag_type[2]  = { 0 };      /* ATQA */
    uint8_t uid[10]      = { 0 };      /* UID */
    uint8_t uid_real_len =   0;        /* 接收到的UID的长度 */
    uint8_t sak[1]       = { 0 };      /* SAK */
    uint8_t keya[6]      = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  /* 验证密钥A */
    uint8_t buff[16]     = { 0 };      /* 用于存储块数据的缓冲区 */
    uint8_t i;
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
                     2)) {     /* 验证的是块2 */

                /* 读块数据 */
                am_zsn700_reader_picca_read(handle, 0, buff); /* 读取块0 */
                am_kprintf("block0: ");
                for (i = 0; i < 16; i++) {
                    am_kprintf("%02x ", buff[i]);
                }
                am_kprintf("\n");

                am_zsn700_reader_picca_read(handle, 3, buff); /* 读取块3 */
                am_kprintf("block3: ");
                for (i = 0; i < 16; i++) {
                    am_kprintf("%02x ", buff[i]);
                }
                am_kprintf("\n");

                am_zsn700_reader_picca_authent(handle,
                                               AM_ZSN700_READER_IC_KEY_TYPE_A,
                                               uid,
                                               keya,
                                               4);

                am_zsn700_reader_picca_read(handle, 4, buff); /* 读取块4 */
                am_kprintf("block4: ");
                for (i = 0; i < 16; i++) {
                    am_kprintf("%02x ", buff[i]);
                }
                am_kprintf("\n\n");
            } else {
                am_kprintf("key A authent failed\n\n");
            }
        }
        am_mdelay(200);
    }
}

/** [src_zsn700_reader_picca_read_block] */

/* end of file */
