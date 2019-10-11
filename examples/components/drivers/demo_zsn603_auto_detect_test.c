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
 * \brief zsn603 自动检测模式进行测试
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 正确连接好天线；
 *   3. 将卡片置于感应区
 *
 * - 实验现象：
 *   1. 将卡片放置天线感应区后串口打印响应信息；
 *
 * \par 源代码
 * \snippet demo_zsn603_auto_detect_test.c src_zsn603_auto_detect_test
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-18  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn603_auto_detect_test
 * \copydoc demo_zsn603_auto_detect_test.c
 */

/** [src_zsn603_auto_detect_test] */
#include "zsn603.h"
#include "ametal.h"
#include "am_vdebug.h"
static int a = 0;

/* 检测到卡片回调函数 */
void __card_input(void *p_arg){
    a = 1;
}

/* ZSN603  自动检测测试程序 */
void demo_zsn603_auto_detect_test_entry(zsn603_handle_t handle)
{
    uint8_t ret;
    /* 使用默认密钥 */
    uint8_t data[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    zsn603_auto_detect_ctrl_t  auto_ctrl;
    zsn603_auto_detect_data_t  auto_data;

    //自动检测模式配置
    auto_ctrl.ad_mode        = ZSN603_AUTO_DETECT_CONTINUE | ZSN603_AUTO_DETECT_INTERRUPT |
                               ZSN603_AUTO_DETECT_SEND;
    auto_ctrl.tx_mode        = ZSN603_ANT_MODE_TX12;
    auto_ctrl.req_code       = ZSN603_MIFARE_REQUEST_IDLE;
    auto_ctrl.auth_mode      = ZSN603_AUTO_DETECT_KEY_AUTH;
    auto_ctrl.key_type       = ZSN603_ICC_KEY_TYPE_A;
    auto_ctrl.p_key          = data;
    auto_ctrl.key_len        = 6;
    auto_ctrl.block          = 4;
    auto_ctrl.pfn_card_input = __card_input;
    auto_ctrl.p_arg          = NULL;

    ret = zsn603_auto_detect(handle, &auto_ctrl);
    if(ret == 0){
        am_kprintf("Entry auto detect card mode success!\r\n");
    }else{
        am_kprintf("Entry auto detect card mode fail beacuse error 0x%02x!\r\n", ret);
        return ;
    }

    /* 等待回调函数触发 */
    while(a == 0);
    ret = zsn603_get_auto_detect(handle, 0, &auto_data);
    a = 0;

    if(ret == 0){
        am_kprintf("Auto detect card success!\r\n");
    }else{
        am_kprintf("Auto detect card fail beacuse error 0x%2x!\r\n", ret);
    }
}  
/** [src_zsn603_auto_detect_test] */

/* end of file */
