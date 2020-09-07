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
 * \snippet demo_fmse_gen_key_pair.c src_fmse_gen_key_pair
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_gen_key_pair
 * \copydoc demo_fmse_gen_key_pair.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

#define FILE_ID_11			0x0A080A98
#define FILE_ID_22			0x0A010A91

/**
 * \brief 例程入口
 */
void demo_fmse_gen_key_pair_entry (am_fmse_handle_t handle, uint8_t mode)
{
    uint8_t i;
    uint8_t rbuf[512];
    uint16_t rlen = 0;
    uint16_t ret = 0;
    uint8_t pin[4];

    ret = am_fmse_file_select(handle, 0xDF01, rbuf, &rlen);
    printf("select file: ret = %04x, rlen = %d\r\n", ret, rlen);

    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;
    ret = am_fmse_pin_verify(handle, 3, pin);
    printf("verify PIN: ret = %04x\r\n",ret);

    if(mode == 2) {
        /* gen SM2 key pair */
        ret = am_fmse_sm2_key_pair_gen(handle, FILE_ID_22, rbuf, &rlen);
        printf("gen sm2 key pair: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

        /* key len: 64 = 0x40 */
        ret = am_fmse_binary_read(handle, 0, 0x40, rbuf, &rlen);
        printf("read binary: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

    } else if(mode == 1) {
        /* gen rsa2048 key pair */
        ret = am_fmse_rsa_key_pair_gen(handle,
                                       RSA2048,
                                       EXP65537,
                                       FILE_ID_11,
                                       rbuf,
                                      &rlen);
        printf("gen rsa key pair: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

        /* key len: 256 + 6 = 262 */
        ret = am_fmse_binary_read(handle, 0, 0xE0, rbuf, &rlen);
        printf("read binary: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

        ret = am_fmse_binary_read(handle, 0x00E0, 0x26, rbuf, &rlen);
        printf("read binary: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

    } else if(mode == 0) {
        /* 1024RSA */
        /* gen rsa1024 key pair */
        ret = am_fmse_rsa_key_pair_gen(handle,
                                       RSA1024,
                                       EXP65537,
                                       FILE_ID_11,
                                       rbuf,
                                      &rlen);
        printf("gen rsa key pair: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");

        /* key len: 128 + 6 = 134 */
        ret = am_fmse_binary_read(handle, 0, 0x86, rbuf, &rlen);
        printf("read binary: ret = %04x, rlen = %d\r\n", ret, rlen);
        for(i = 0; i < rlen; i++) {
            printf("%02x,",rbuf[i]);
        }
        printf("\r\n");
    }
}

/* end of file */
