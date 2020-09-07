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
 *   2. 如果验证成功，并且指定的块中的格式为数值块的格式那么将块中的数值减5再打印出来。
 *   3. 如果验证成功，但是指定块中的格式不是数值块的格式，那么不会打印数值，此时可以将130行的条件编译
 *      改为1，编译后执行，刷卡时会不断的打印同一个数字，之后再将130行的条件编译改为0，编译后执行，再
 *      刷卡，则串口将打印出从15开始，不断-5的数值。
 *
 * \par 源代码
 * \snippet demo_zsn700_reader_picca_val_operate.c src_zsn700_reader_picca_val_operate
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-22  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_reader_picca_val_operate
 * \copydoc demo_zsn700_reader_picca_val_operate.c
 */

/** [src_zsn700_reader_picca_val_operate] */
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_zsn700_reader.h"
#include "am_zsn700_reader_reg.h"
#include "am_hwconf_zsn700_reader.h"

/**
 * \brief A类卡值操作例程
 */
void demo_zsn700_reader_picca_val_operate (void)
{
 
    uint8_t tag_type[2]  = { 0 };      /* ATQA */
    uint8_t uid[10]      = { 0 };      /* UID */
    uint8_t uid_real_len = 0;          /* 接收到的UID的长度 */
    uint8_t sak[3]       = { 0 };      /* SAK */
    uint8_t keya[6]      = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  /* 验证密钥A */
    uint8_t blocknum     = 9;          /* 要读写的块 */
    int32_t value        = 0;          /* 数值块中的值 */
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
            if (AM_ZSN700_READER_STATUS_SUCCESS == \
                    am_zsn700_reader_picca_authent (handle,
                                                    AM_ZSN700_READER_IC_KEY_TYPE_A,
                                                    uid,
                                                    keya,
                                                    blocknum)) {

#if 0
                /* 若卡块格式不是数值块的格式，则使用下面这段程序将卡块初始化为数值块的格式 */
                uint8_t once_flag = 0;            /* 只进行一次初始化块的标识 */
                if (once_flag == 0) {
                    once_flag = 1;
                    /* 先将数据块初始化为值块的格式，初始值为20 */
                    am_zsn700_reader_picca_val_set(handle,
                                                   blocknum,
                                                   20);
                }
#endif

                /* 减值操作，减5 */
                if (AM_ZSN700_READER_STATUS_SUCCESS == am_zsn700_reader_picca_val_operate\
                                                  (handle,
                                                   AM_ZSN700_READER_PICCA_PICC_SUB,
                                                   blocknum,
                                                   blocknum,
                                                   5)) {
                    /* 读值块数据 */
                    if (AM_ZSN700_READER_STATUS_SUCCESS == am_zsn700_reader_picca_val_get\
                            (handle, blocknum, &value)) {
                        am_kprintf("value :%d\n", value);
                    }
                } else {
                    am_kprintf("value operate failed\n");
                }
            } else {
                am_kprintf("key A authent failed\n\n");
            }
        }
        am_mdelay(200);
    }
}

/** [src_zsn700_reader_picca_val_operate] */

/* end of file */
