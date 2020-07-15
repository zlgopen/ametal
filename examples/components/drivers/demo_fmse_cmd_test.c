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
 * \brief 安全芯片FMSE-A03 例程，通过标准接口实现
 *
 * - 实验现象：
 *
 * \par 源代码
 * \snippet demo_fmse_cmd_test.c src_fmse_cmd_test
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_cmd_test
 * \copydoc demo_fmse_cmd_test.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_fmse_cmd_test_entry (am_fmse_handle_t handle)
{
    uint8_t  i;
    uint8_t  rbuf[1028];
    uint8_t  pin[4];
    uint16_t rlen   = 0;
    uint16_t ret    = 0;

    /* 选择安全目录DF01，这个目录ID是默认的，芯片出厂就做好的 */
    ret = am_fmse_file_select(handle, 0xDF01, rbuf, &rlen);              
    if(ret != 0x9000) {
        printf("1. file selection failed\r\n");
    }
    printf("select file: ret = %04x, rlen = %d\r\n", ret, rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* 选择主目录 */    
    ret = am_fmse_file_select(handle, 0x3F00, rbuf, &rlen);
    if(ret != 0x9000) { 
        printf("2. file selection failed\r\n");
    }
    printf("select file: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* 获取八字节随机数 */
    ret = am_fmse_challenge_get(handle, 8, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("3. challenge get failed\r\n");
    }
    printf("get challenge: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* 重新选择DF01 */
    ret = am_fmse_file_select(handle, 0xDF01, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("4. file selection failed\r\n");
    }
    printf("select file: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* 选择0A08公钥文件，这个文件也是默认存在的，出厂即有 */
    ret = am_fmse_file_select(handle, 0x0A08, rbuf, &rlen); 
    if(ret != 0x9000) {
        printf("5. file selection failed\r\n");
    }
    printf("select file: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* 选择0A98私钥文件，这个文件也是默认存在的，出厂即有 */
    ret = am_fmse_file_select(handle, 0x0A98, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("6. file selection failed\r\n");
    }
    printf("select file: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;

    /* 校验默认口令 */
    ret = am_fmse_pin_verify(handle, 3, pin);
    if(ret != 0x9000) {
        printf("7. pin verify failed\r\n");
    }
    printf("verify pin: ret = %04x\r\n",ret);
}

/* end of file */
