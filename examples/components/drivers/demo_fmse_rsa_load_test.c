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
 * \snippet demo_fmse_rsa_load_test.c src_fmse_rsa_load_test
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-01  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_fmse_rsa_load_test
 * \copydoc demo_fmse_rsa_load_test.c
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_fmse_a03.h"
#include "am_delay.h"

#define FILE_ID_11			0x0A080A98
#define FILE_ID_22			0x0A010A91

#define ZLG_RSA_ECPT_LOAD_TEST  1

/**
 * \brief 例程入口
 */
void demo_fmse_rsa_load_test_entry (am_fmse_handle_t  handle)
{
    uint8_t i;
    uint8_t  relt_n[150] = {0}; /* 大小要大于等于144字节 */
    uint8_t  relt_e[16] = {0};  /* 大小要大于等于16字节 */  
    uint8_t relt_rbuf[512];
    uint8_t relt_inbuf[256];   
    uint16_t relt_rlen = 0;
    uint16_t relt_sw = 0;    
    uint8_t relt_session_key_index = 0;   
    uint8_t pin[4];

    relt_sw = am_fmse_file_select(handle, 0xDF01, relt_rbuf, &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("1. file selection failed\r\n");
    }

    pin[0] = 0x11;
    pin[1] = 0x22;
    pin[2] = 0x33;
    relt_sw = am_fmse_pin_verify(handle, 3, pin);
    if(relt_sw != 0x9000) {
        printf("2. pin verify failed\r\n");
    }

    /* gen rsa1024 key pair */
    printf("gen 1024RSA key test!\r\n");
    relt_sw = am_fmse_rsa_key_pair_gen(handle,
                                       RSA1024,
                                       EXP65537,
                                       FILE_ID_11,
                                       relt_rbuf,
                                      &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("3. rsa key pair gen failed\r\n");
    }
    printf("gen RSA key pair: sw = %04x, rlen = %d\r\n", relt_sw, relt_rlen);
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

    /* key len: 2 + 128 + 4 = 134 即0x86 */
    relt_sw = am_fmse_binary_read(handle, 0, 0x86, relt_rbuf, &relt_rlen);
    if(relt_sw != 0x9000) { 
        printf("4. binary reading failed\r\n");
    }
    printf("read binary: sw = %04x, rlen = %d\r\n", relt_sw, relt_rlen);          
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

/* 密文导入测试 */
#ifdef ZLG_RSA_ECPT_LOAD_TEST

    printf("\r\n");
    printf("RSA ECPT LOAD TEST ！\r\n");
    relt_n[0] = 128;           /* 加密是对“len(N)||N”进行加密，预先写入长度 */
    relt_e[0] = 4;             /* 加密是对“len(E)||E”进行加密，预先写入长度 */
    am_fmse_memmove(&relt_n[1], &relt_rbuf[2], 128);
    am_fmse_memmove(&relt_e[1], &relt_rbuf[2 + 128], 4);

    am_fmse_memmove(relt_inbuf, 0, sizeof(relt_inbuf));
    am_fmse_memmove(relt_inbuf, "\xc1\x10", 2);                                 
    for(i = 0; i < 16; i++) {
        relt_inbuf[i + 2] = i + 2;                                                         
    }

    /* Import Session Key */
    /* 0110，SM1算法，大端数据格式 */
    relt_sw = am_fmse_session_key_import(handle,
                                         0x0110,
                                         0x12,
                                         relt_inbuf,
                                         relt_rbuf,
                                        &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("5. session key import failed\r\n");
    }
    relt_session_key_index = relt_rbuf[0];
    printf("import session key: relt_sw = %04x, relt_rlen = %d\r\n", relt_sw, relt_rlen);

    /* 导入成功，返回当前密钥索引号+9000 */
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

    /* 导入会话密钥后，用会话密钥加密N参数和E参数 */
    /* pre data N */
    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));

    /* 0x000000xx，最后一字节表示会话密钥的索引号 */
    relt_inbuf[3] = relt_session_key_index;
    for(i = 0; i < 144; i++) {
        relt_inbuf[i + 4] = relt_n[i];                                                
    }

    /* Data Encrypt calc
     * 0040，唯一数据块，加密操作，使用以导入的会话密钥,ECB模式，
     * 数据包括4字节密钥索引号（不算做数据长度），1字节的N参数长度，
     * 128字节N参数，15个字节的填充（填充为0）
     */
    relt_sw = am_fmse_data_encrypt_and_decrypt(handle,
                                               0x0040,
                                               4 + 1 + 128 + 15,
                                               relt_inbuf,
                                               relt_rbuf,
                                              &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("6. data encrypt failed\r\n");
    }

    /* 返回计算后的加解密数据 */
    printf("data encrypt N: relt_sw = %04x, relt_rlen = %d\r\n", relt_sw, relt_rlen);
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

    /* 将N参数的加密数据保存,加密后的数据长度是原数据填充后的长度，
     * 即1+128+15 = 144
     */
    am_fmse_memmove(relt_n, relt_rbuf, 144);

    /* pre data E */
    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));

    /* 0x000000xx，最后一字节表示会话密钥的索引号 */   
    relt_inbuf[3] = relt_session_key_index;

    for(i = 0; i < 16; i++) {
        relt_inbuf[i + 4] = relt_e[i];                                            
    }

    /* Data Encrypt calc
    * 0040，唯一数据块，加密操作，使用以导入的会话密钥,ECB模式，
    * 4字节密钥索引（不算做待加密的数据长度），1字节E参数长度，
    * 4字节E参数，11个字节填充（填充为0）
    */
    relt_sw = am_fmse_data_encrypt_and_decrypt(handle,
                                               0x0040,
                                               4 + 1 + 4 + 11,
                                               relt_inbuf,
                                               relt_rbuf,
                                              &relt_rlen);
    if(relt_sw != 0x9000){
        printf("7. data decrypt failed\r\n");
    }

    /* 返回计算后的加解密数据 */
    printf("data encrypt E: relt_sw = %04x, relt_rlen = %d\r\n", relt_sw, relt_rlen);
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

    /* 将E参数的加密数据保存,加密后的数据长度是原数据
     * 填充后的长度，即1+4+11= 16
     */
    am_fmse_memmove(relt_e, relt_rbuf, 16);

    /* 分两次向0A05公钥文件中导入原0A08当中的公钥参数N和E(已加密) */   
    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf)); 
    am_fmse_memmove(relt_inbuf, "\xc0\x02\x0A\x05\xc3\x81\x90", 7);

    /* 装载到0a05文件,内容是加密后的参数N */
    for(i = 0; i < 144; i++) {                       
        relt_inbuf[i + 7] = relt_n[i]; 
    }

    /* 0x8001,加密传入，RSA1024 */
    relt_sw = am_fmse_rsa_key_install(handle,
                                      0x8001,
                                      7 + 144,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("8. rsa key install failed\r\n");
    }

    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));
    am_fmse_memmove(relt_inbuf, "\xc0\x02\x0A\x05\xc9\x10", 6);

    /* 装载到0a05文件，内容是加密后的参数E */
    for(i = 0; i < 16; i++) {
        relt_inbuf[i + 6] = relt_e[i];
    }

    /* 0x8001,加密传入，RSA1024 */
    relt_sw = am_fmse_rsa_key_install(handle,
                                      0x8001,
                                      6 + 16,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("9. rsa key install failed\r\n");
    }

#else  /* 明文导入公钥 */
    printf("\r\n");
    printf("RSA LOAD TEST！\r\n");
    am_fmse_memmove(relt_n, &relt_rbuf[2], 128);
    am_fmse_memmove(relt_e, &relt_rbuf[2 + 128], 4);

    /* 分两次导入公钥的N和E参数 */     
    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));

    /* 装载到0a05文件，内容是参数N */
    am_fmse_memmove(relt_inbuf, "\xc0\x02\x0a\x05\xc3\x81\x80", 7);
    for(i = 0; i < 128; i++) {
        relt_inbuf[i + 7] = relt_n[i];
    }

    /* 0x0001,明文传入，RSA1024 */
    relt_sw = am_fmse_rsa_key_install(handle,
                                      0x0001,
                                      7 + 128,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("10. rsa key install failed\r\n");
    }

    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));

    /* 导入0a05文件 */
    am_fmse_memmove(relt_inbuf, "\xc0\x02\x0A\x05\xc9\x04", 6);
    for(i = 0; i < 4; i++) {
        relt_inbuf[i + 6] = relt_e[i];
    }

    /* 0x0001,明文传入，RSA1024 */
    relt_sw = am_fmse_rsa_key_install(handle,
                                      0x0001,
                                      6+4,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("11. rsa key install failed\r\n");
    }    

#endif /* ZLG_RSA_ECPT_LOAD_TEST */

    /* 导入后，用导入好的0A05公钥进行加解密测试，并用0A98的私钥进行解密，
     * 0A05的公钥其实是从0A01公钥文件里“拷贝”过来的
     */
    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));
    am_fmse_memmove(relt_inbuf, "\xc0\x02\x0a\x05\xc1\x81\x80", 7);

    /* 准备任意的待加密的数据，因此原始数据是1~128 */
    for(i = 0; i < 128; i++) {
        relt_inbuf[i + 7] = i + 1;
    }

    /* RSA PUB key calc
     * 公钥计算，4000表示计算完送回结果，且为RSA1024计算
     */
    relt_sw = am_fmse_rsa_pub_key_cal(handle,
                                      0x4000,
                                      128 + 7,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000){
        relt_sw = 8;
    }

    printf("RSA pub key cal: relt_sw = %04x, relt_rlen = %d\r\n", relt_sw, relt_rlen);
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");

    am_fmse_memset(relt_inbuf, 0, sizeof(relt_inbuf));

    /* 切换成私钥文件 */
    am_fmse_memmove(relt_inbuf, "\xc2\x02\x0a\x98\xc1\x81\x80", 7);

    relt_inbuf[2] = (FILE_ID_11 >> 8) & 0xff;
    relt_inbuf[3] = (FILE_ID_11 >> 0) & 0xff; 

    /* 把公钥对数据计算后的结果，作为后续私钥的待计算数据 */
    for(i = 0; i < 128; i++) {
        relt_inbuf[i + 7] = relt_rbuf[i];
    }

    /* RSA PRI key calc
     * 私钥计算，4000表示计算完送回结果，且为RSA1024计算 ，0x87为结果的定长
     */
    relt_sw = am_fmse_rsa_pri_key_cal(handle,
                                      0x4000,
                                      0x87,
                                      relt_inbuf,
                                      relt_rbuf,
                                     &relt_rlen);
    if(relt_sw != 0x9000) {
        printf("12. rsa pri key cal failed\r\n");
    }

    /* 公钥计算的数据经私钥计算后，变为最开始的原始数据 */
    printf("RSA pri key cal: relt_sw = %04x, relt_rlen = %d\r\n", relt_sw, relt_rlen);
    for(i = 0; i < relt_rlen; i++) {
        printf("%02x,",relt_rbuf[i]);
    }
    printf("\r\n");   
}

/* end of file */
