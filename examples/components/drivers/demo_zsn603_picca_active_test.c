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
 * \brief zsn603 A类卡测试程序
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口；
 *   2. 将A类卡放置于天线感应区。
 *
 * - 实验现象：
 *   1. MCU将会打印出A类卡的相关信息
 *
 * \par 源代码
 * \snippet demo_zsn603_picca_test.c src_zsn603_picca_test
 *
 * \internal
 * \par Modification history
 * - 1.00 19-06-18  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn603_picca_test
 * \copydoc demo_zsn603_picca_test.c
 */

/** [src_zsn603_picca_test] */
#include "ametal.h"
#include "zsn603.h"
#include "am_vdebug.h"

/* ZSN603  A类卡激活测试程序 */
void demo_zsn603_picca_active_test_entry (zsn603_handle_t handle)
{
    unsigned short  atq[2] = {0};
    unsigned char   saq = 0;
    unsigned char   len = 0;
    unsigned char   uid[10] = {0};
    unsigned char   ret = 0;
    //A卡请求接口函数  请求模式为 0x26  IDLE
    ret = zsn603_picca_active(handle, 0x26, atq, &saq, &len, uid);
    if(ret == 0){
        unsigned char i = 0;
        am_kprintf("ATQ  is : %02x  %02x\r\n", atq[0], atq[1]);
        am_kprintf("SAQ  is : %02x \r\n", saq);
        am_kprintf("UID is :");
        for(i = 0; i < len ; i ++ ){
            am_kprintf("%02x ",  uid[i]);
        }
        am_kprintf("\r\n");
    }else{
        am_kprintf("active fail beacuse error 0x%02x", ret);
    }
}
/** [src_zsn603_picca_test] */

/* end of file */
