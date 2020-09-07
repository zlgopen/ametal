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
 * \snippet demo_fmse_data_compress_test.c src_fmse_data_compress_test
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_data_compress_test
 * \copydoc demo_fmse_data_compress_test.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_fmse_data_compress_test_entry (am_fmse_handle_t handle)
{
    uint8_t  i;  
    uint8_t  dct_rbuf[512];
    uint8_t  dct_inbuf[256];   
    uint8_t  rbuf[1028];
    uint8_t  pin[4];
    uint16_t dct_rlen = 0;
    uint16_t dct_sw   = 0;
    uint16_t rlen     = 0;

    printf("\r\n\r\n"); 
    printf("SHA-1 Test!\r\n");   
    am_fmse_memmove(dct_inbuf, "\xc1\xc8", 2);

    dct_sw = am_fmse_file_select(handle, 0xDF01, rbuf, &rlen);               //选择DF01
    if(dct_sw != 0x9000) {
        printf("1. file selection failed\r\n");
    }

    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;
    dct_sw = am_fmse_pin_verify(handle, 3, pin);                             //校验默认口令
    if(dct_sw != 0x9000) {
        printf("2. pin verify failed\r\n");
    }

    /* 准备任意数据 */
    for(i = 0; i < 200; i++) {
        dct_inbuf[2 + i] = i;
    }

    /* 用SHA-1算法把数据压缩后，输出散列值的长度为20字节 */
    dct_sw = am_fmse_data_compress(handle,
                                   0x0101,
                                   2 + 200,
                                   dct_inbuf,
                                   dct_rbuf,
                                  &dct_rlen);
    if(dct_sw != 0x9000) {
        printf("3. data compress failed\r\n");
    }
    printf("data compress with SHA-1: dct_sw = %04x, dct_rlen = %d\r\n",dct_sw, dct_rlen);          
    for(i = 0; i < dct_rlen; i++) {
        printf("%02x,", dct_rbuf[i]);
    }
    printf("\r\n\r\n");     

    printf("SHA256 Test!\r\n");
    am_fmse_memmove(dct_inbuf, "\xc1\xc8", 2);

    /* 准备任意数据 */
    for(i = 0; i < 200; i++) {
        dct_inbuf[2 + i] = i;
    }

    /* 用SHA256算法把数据压缩后，输出散列值的长度为32字节 */
    dct_sw = am_fmse_data_compress(handle,
                                   0x0102,
                                   2 + 200,
                                   dct_inbuf,
                                   dct_rbuf,
                                  &dct_rlen);
    if(dct_sw != 0x9000) {
        printf("4. data compress failed\r\n");
    }
    printf("data compress with SHA256: dct_sw = %04x, dct_rlen = %d\r\n",
            dct_sw, dct_rlen);

    for(i = 0; i < dct_rlen; i++) {
        printf("%02x,",dct_rbuf[i]);
    }
    printf("\r\n\r\n");       

    printf("SM3 Test!\r\n");    
    am_fmse_memmove(dct_inbuf, "\xc1\xc8", 2);

    /* 准备任意数据 */
    for(i = 0; i < 200; i++) {
        dct_inbuf[2 + i] = i;
    }

    /* 用SM3算法把数据压缩后，输出散列值的长度为32字节 */    
    dct_sw = am_fmse_data_compress(handle,
                                   0x0103,
                                   2 + 200,
                                   dct_inbuf,
                                   dct_rbuf,
                                  &dct_rlen);
    if(dct_sw != 0x9000) {
        printf("5. data compress failed\r\n");
    }
    printf("data compress with SM3: dct_sw = %04x, dct_rlen = %d\r\n", dct_sw, dct_rlen); 
    
    for(i = 0; i < dct_rlen; i++) {
        printf("%02x,",dct_rbuf[i]);
    }
    printf("\r\n\r\n");   

    printf("MD5 Test!\r\n");    
    am_fmse_memmove(dct_inbuf, "\xc1\xc8", 2);

    /* 准备任意数据 */
    for(i = 0; i < 200; i++) {
        dct_inbuf[2 + i] = i;
    }

    /* 用MD5算法把数据压缩后，输出散列值的长度为16字节 */      
    dct_sw = am_fmse_data_compress(handle,
                                   0x0104,
                                   2 + 200,
                                   dct_inbuf,
                                   dct_rbuf,
                                  &dct_rlen);
    if(dct_sw != 0x9000) {
        printf("6. data compress failed\r\n");
    }
    printf("data compress with MD5: dct_sw = %04x, dct_rlen = %d\r\n", dct_sw, dct_rlen);          
    for(i = 0; i < dct_rlen; i++) {
        printf("%02x,",dct_rbuf[i]);
    }
    printf("\r\n\r\n");
}

/* end of file */
