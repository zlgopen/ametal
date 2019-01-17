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
 * \brief fm175xx对FM11R005M类型卡进行操作
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *   3. 将FM11R005M类型卡置于天线感应区。
 *
 * - 实验现象：
 *   1. 串口将会打印FM11R005M卡的UID
 *   2. 串口将会打印写入的数据
 *
 * \par 源代码
 * \snippet demo_fm175xx_fm11rf005m_opra.c src_fm175xx_fm11rf005m_opra
 *
 * \internal
 * \par Modification history
 * - 1.00 19-01-09  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fm175xx_fm11rf005m_opra
 * \copydoc demo_fm175xx_fm11rf005m_opra.c
 */

/** [src_fm175xx_fm11rf005m_opra] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "am_delay.h"
/**
 * \brief FM11R005M卡操作例程
 */
void demo_fm175xx_fm11rf005m_operate (am_fm175xx_handle_t handle)
{

    uint8_t   block_num = 0x0a;
    uint8_t   rx[4]  = {0};
    uint8_t   uid[4];
    uint8_t   tx[4]  = {0x11, 0x22, 0x33, 0x44};
    uint8_t   key[6] = {0xff, 0xff, 0xff, 0xff, 0x00, 0x00};
    while(1){

        if(AM_FM175XX_STATUS_SUCCESS ==
                am_fm175xx_fm11rf005m_reqa(handle, AM_FM175XX_PICCA_REQ_IDLE)){
            am_kprintf("FM11RF005M  card reqa success!\n");
        }

        if(AM_FM175XX_STATUS_SUCCESS ==
                am_fm175xx_fm11rf005m_read_block(handle, 0x01, uid)){
            am_kprintf("UID = %02x %02x %02x %02x \n\n",
                        uid[0], uid[1], uid[2], uid[3]);
        }

        if(AM_FM175XX_STATUS_SUCCESS ==
                am_fm175xx_picca_authent(handle, 0x60, uid, key, 0x00)){

            if(AM_FM175XX_STATUS_SUCCESS ==
                    am_fm175xx_fm11rf005m_write_block(handle, block_num, tx)){
                am_kprintf("Write black %d success ...\n", block_num);
                am_kprintf("data is 0x%x 0x%x 0x%x 0x%x \n\n ",
                            tx[0], tx[1], tx[2], tx[3]);

            }

            if(AM_FM175XX_STATUS_SUCCESS ==
                    am_fm175xx_fm11rf005m_read_block(handle, block_num, rx)){
                am_fm175xx_fm11rf005m_read_block(handle, block_num, rx);
                am_kprintf("Read black %d success ...\n", block_num);
                am_kprintf("data is 0x%x 0x%x 0x%x 0x%x \r\n\n ", rx[0], rx[1], rx[2], rx[3]);
            }
        }
        am_mdelay(500);
    }
}

/** [src_fm175xx_fm11rf005m_opra] */

/* end of file */
