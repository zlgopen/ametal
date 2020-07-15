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
 * \snippet demo_fmse_ecc_calc_test.c src_fmse_ecc_calc_test
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_ecc_calc_test
 * \copydoc demo_fmse_ecc_calc_test.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

#define FILE_ID_TEMP_FOR_ECC        0x0A060A96

/**
 * \brief 例程入口
 */
void demo_fmse_ecc_calc_test_entry (am_fmse_handle_t handle)
{
    uint8_t i;  
    uint8_t et_rbuf[300] = {0};
    uint8_t et_inbuf[256] = {0};
    uint8_t pin[4];
    uint16_t et_rlen = 0;
    uint16_t et_sw   = 0;    

    /* 待操作的数据 */
    uint8_t et_sha256[32] = {0x19, 0x01, 0xda, 0x1c, 0x9f, 0x69, 0x9b, 0x48,
                             0xf6, 0xb2, 0x63, 0x6e, 0x65, 0xcb, 0xf7, 0x3a,
                             0xbf, 0x99, 0xd0, 0x44, 0x1e, 0xf6, 0x7f, 0x5c, 
                             0x54, 0x0a, 0x42, 0xf7, 0x05, 0x1d, 0xec, 0x6f};
    
    printf("\r\n"); 
    printf("gen ecc key test\r\n");

    /* 选择 SE APP */ 
    et_sw = am_fmse_file_select(handle, 0xDF01, et_rbuf, &et_rlen);
    if(et_sw != 0x9000) {
        printf("1. file selection failed\r\n");
    }

    /* SE芯片出厂默认PIN值 */
    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;

    et_sw = am_fmse_pin_verify(handle, 3, pin);
    if(et_sw != 0x9000) {
        printf("2. pin verify failed\r\n");
    }

    /* 生成ECC密钥对,SECG-P256K1，并放于对应文件当中(0A06和0A96) */
    et_sw = am_fmse_ecc_key_pair_gen(handle,
                                    0000,
                                    FILE_ID_TEMP_FOR_ECC,
                                    et_rbuf,
                                   &et_rlen);
    if(et_sw != 0x9000) {
        printf("3. gen ecc key pair failed\r\n");
    }

    printf("gen ecc key pair: sw = %04x, rlen = %d\r\n", et_sw, et_rlen);

    for(i = 0; i < et_rlen; i++) {
        printf("%02x,",et_rbuf[i]);
    }
    printf("\r\n");

    /* key len: 64 = 0x40 */
    /* 由于生成后密钥对后自动选择了公钥文件，在此处读出公钥 */
    et_sw = am_fmse_binary_read(handle, 0, 0x40, et_rbuf, &et_rlen);
    if(et_sw != 0x9000) {
        printf("4. binary reading failed \r\n");
    }
    printf("binary read: sw = %04x, rlen = %d\r\n", et_sw, et_rlen);
    for(i = 0; i < et_rlen; i++) {
        
        printf("%02x,",et_rbuf[i]);
    }
    printf("\r\n");

    printf("ecc cal test\r\n");
    am_fmse_memset(et_inbuf, 0, sizeof(et_inbuf));

    /* x0a\x96是私钥文件，SECG-P256K1，x82\x00\x20,待签名的数据块，0x20是长度 */
    am_fmse_memmove(et_inbuf, "\xc2\x02\x0a\x96\xc1\x82\x00\x20", 8);
    for(i = 0; i < sizeof(et_sha256); i++) {
        et_inbuf[i + 8] = et_sha256[i];
    }

    /* ECC PRI key calc 签名操作 */
    et_sw = am_fmse_ecc_pri_key_cal(handle,
                                    0x0001,
                                    8 + 32,
                                    et_inbuf,
                                    et_rbuf,
                                   &et_rlen);
    if(et_sw != 0x9000) {
        printf("5. ecc pri_key cal failed\r\n");
    }

    printf("ecc pri_key cal: sw = %04x, rlen = %d\r\n", et_sw, et_rlen);
    for(i = 0; i < et_rlen; i++) {
        printf("%02x,",et_rbuf[i]);
    }
    printf("\r\n");    

    am_fmse_memset(et_inbuf, 0, sizeof(et_inbuf));

    /* 0A06是公钥文件 */
    am_fmse_memmove(et_inbuf, "\xc0\x02\x0a\x06\xc1\x82\x00\x20", 8);

    /* 加上签名的数据长度 */
    et_inbuf[7] += et_rlen - 2;
    for(i = 0; i < 32; i++) {
        et_inbuf[i + 8] = et_sha256[i];         /* 放入哈希值 */
    }

    for(i = 0; i < (et_rlen - 2); i++) {
        et_inbuf[i + 8 + 32] = et_rbuf[i];      /* 放入签名内容 */
    }

    /* ECC PUB key calc */
    /* 公钥计算，SECG-P256K1 */
    et_sw = am_fmse_ecc_pub_key_cal(handle,
                                    0x0001,
                                    8 + 32 + et_rlen - 2,
                                    et_inbuf,
                                    et_rbuf,
                                   &et_rlen);
    if(et_sw != 0x9000) {
        printf("6. ecc pub key cal failed\r\n");
    }

    printf("ecc pub key cal: sw = %04x, rlen = %d\r\n", et_sw, et_rlen);
    for(i = 0; i < et_rlen; i++) {
        printf("%02x,",et_rbuf[i]);
    }
    printf("\r\n");
}

/* end of file */
