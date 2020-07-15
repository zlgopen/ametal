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
 * \snippet demo_fmse_data_encrypt_and_decrypt.c
 *          src_fmse_data_encrypt_and_decrypt
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_data_encrypt_and_decrypt
 * \copydoc demo_fmse_data_encrypt_and_decrypt.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

/**
 * \brief 例程入口
 */
void demo_fmse_data_encrypt_and_decrypt_entry (am_fmse_handle_t handle)
{
    uint8_t i;
    uint8_t rbuf[1028];
    uint8_t inbuf[256];
    uint8_t pin[4];
    uint16_t rlen = 0;
    uint16_t ret  = 0;
    uint8_t det_key_index = 0;

    printf("\r\n");
    printf("data endecrypt test\r\n");  

    ret = am_fmse_file_select(handle, 0xDF01, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("1. file selection failed\r\n");
    }

    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;
    ret = am_fmse_pin_verify(handle, 3, pin);
    if(ret != 0x9000) {
        printf("2. pin verify failed\r\n");
    }

    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    /* SM1明文输入 */
    am_fmse_memmove(inbuf, "\xc1\x10", 2);
    for(i = 0; i < 16; i++) {
        inbuf[i + 2] = i;
    }

    /* Import Session Key */
    /* 0110，SM1算法，大端数据格式 */
    ret = am_fmse_session_key_import(handle, 0x0110, 0x12, inbuf, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("3. session key import failed\r\n");
    }

    det_key_index = rbuf[0];

    printf("import session key: ret = %04x, rlen = %d\r\n",ret,rlen);

    /* 导入成功，返回当前密钥索引号+9000，此处索引为00 */
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* pre data */
    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    inbuf[3] = det_key_index;

    /* 00000000，最后一字节表示会话密钥的索引号 */
    for(i = 0; i < 16; i++) {
        inbuf[i + 4] = 0x11;
    }

    /* Data Encrypt calc */
    /* 0040，唯一数据块，加密操作，使用以导入的会话密钥,ECB模式，
     * 数据长度为4+16
     */
    ret = am_fmse_data_encrypt_and_decrypt(handle,
                                           0x0040,
                                           0x14,
                                           inbuf,
                                           rbuf,
                                          &rlen);
    if (ret != 0x9000) {
        printf("4. data encrypt failed\r\n");
    }

    /* 返回计算后的加解密数据 */
    printf("data encrypt: ret = %04x, rlen = %d\r\n", ret, rlen);
    for(i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* pre data */
    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    inbuf[3] = det_key_index;   

    /* 0x00000000，密钥索引，后续十六字节是刚刚完成加密后的数据 */
    for (i = 0; i < 16; i++) {
        inbuf[i + 4] = rbuf[i];
    }

    /* Data Decrypt calc
     * 00C0，解密操作，使用已导入的会话密钥，数据域长度为4+16
     */
    ret = am_fmse_data_encrypt_and_decrypt(handle,
                                           0x00C0,
                                           0x14,
                                           inbuf,
                                           rbuf,
                                          &rlen);
    if (ret != 0x9000) {
        printf("5. data decrypt failed\r\n");
    }
    printf("data decrypt: ret = %04x, rlen = %d\r\n",ret,rlen);
    for(i = 0; i < rlen; i++) {                                                                    
        printf("%02x,",rbuf[i]);                                        
    }                                                                    
    printf("\r\n");

    /* SM4和SM1一样都是对称加密算法，即加解密用同样的密钥 */

    printf("\r\n");
    printf("SM4 TEST! \r\n");  
    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    /* SM4明文输入 */
    am_fmse_memmove(inbuf, "\xc1\x10", 2);
    for(i = 0; i < 16; i++) {
        inbuf[i+2] = i;
    }

    /* Import Session Key */
    /* 0210，SM4算法，大端数据格式 */
    ret = am_fmse_session_key_import(handle, 0x0210, 0x12, inbuf, rbuf, &rlen);
    if(ret != 0x9000) {
        printf("6. session key import failed\r\n");
    }
    printf("import session key SM4: ret = %04x, rlen = %d\r\n", ret, rlen); 

    /* 导入成功，返回当前密钥索引号+9000，此处索引为00 */
    for(i = 0; i < rlen;  i++) {
        printf("%02x,",rbuf[i]);
    }

    /* 保存密钥索引号 */
    det_key_index = rbuf[0];
    printf("\r\n");

    /* pre data */
    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    /* 0x00000000，预留密钥索引位置 */
    for(i = 0; i < 16; i++) {
        inbuf[i + 4] = 0x44;
    }

    /* 更新会话密钥索引0x00000000+zlg_key_num */
    inbuf[3] = det_key_index;

    /* Data Encrypt calc
     * 0040，唯一数据块，加密操作，使用以导入的会话密钥,ECB模式，
     * 数据长度为4+16
     */
    ret = am_fmse_data_encrypt_and_decrypt(handle,
                                           0x0040,
                                           0x14,
                                           inbuf,
                                           rbuf,
                                          &rlen);
    if (ret != 0x9000) {
        printf("7. data encrypt failed\r\n");
    }

    /* 返回计算后的加解密数据 */
    printf("data encrypt SM4: ret = %04x, rlen = %d\r\n", ret, rlen);
    for (i = 0; i < rlen; i++) {
        printf("%02x,",rbuf[i]);
    }
    printf("\r\n");

    /* pre data */
    am_fmse_memset(inbuf, 0, sizeof(inbuf));

    /* 0x00000000，预留密钥索引位置，后续十六字节是刚刚完成加密后的数据 */
    for (i = 0; i < 16; i++) {
        inbuf[i + 4] = rbuf[i];
    }

    /* 更新会话密钥索引0x00000000+zlg_key_num */
    inbuf[3] = det_key_index;

    /* Data Decrypt calc
     * 00C0，解密操作，使用已导入的会话密钥，数据域长度为4+16
     */
    ret = am_fmse_data_encrypt_and_decrypt(handle,
                                           0x00C0,
                                           0x14,
                                           inbuf,
                                           rbuf,
                                          &rlen);
    if (ret != 0x9000) {
        printf("8. data decrypt failed\r\n");
    }
    printf("data decrypt SM4: ret = %04x, rlen = %d\r\n", ret, rlen);
    for (i = 0; i < rlen; i++) {                                                                    
        printf("%02x,",rbuf[i]);                                        
    }                                                                    
    printf("\r\n");
}

/* end of file */
