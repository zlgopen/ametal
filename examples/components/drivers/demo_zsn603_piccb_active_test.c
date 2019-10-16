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
 * \brief zsn603 B类卡测试程序
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 将B类卡放置于天线感应区。
 *
 * - 实验现象：
 *   1. MCU将会打印出B类卡的相关信息
 *
 * \par 源代码
 * \snippet demo_zsn603_piccb_test.c src_zsn603_piccb_test
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-18  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn603_piccb_test
 * \copydoc demo_zsn603_piccb_test.c
 */

/** [src_zsn603_piccb_test] */
#include "ametal.h"
#include "zsn603.h"
#include "am_vdebug.h"
/* ZSN603  B类卡激活测试程序 */
void demo_zsn603_piccb_active_test_entry (zsn603_handle_t handle)
{
    unsigned char  info[12] = {0};
    unsigned char  ret = 0;
    /* 在使用B卡相关的函数前，需设置协议为B类卡 */
    ret = zsn603_set_ios_type (handle, ZSN603_ICC_ISO_TYPE_B);
    if(ret  !=  0 ){
        am_kprintf("ios set fail beacuse %0x2", ret);
        return;
    }
    //B卡请求接口函数  请求模式为 0x00  IDLE
    ret = zsn603_piccb_active(handle, 0x00, info);
    if(ret == 0){
        unsigned char i = 0;
        am_kprintf("CARD INFO  is :");
        for(i = 0; i < 12 ; i ++ ){
            am_kprintf("%02x ",  info[i]);
       }
        am_kprintf("\r\n");
    }else{
        am_kprintf("active fail beacuse error 0x%02x", ret);
    }
}
/** [src_zsn603_piccb_test] */

/* end of file */
