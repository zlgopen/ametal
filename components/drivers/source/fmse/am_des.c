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
 * \brief DES
 *
 * \internal
 * \par Modification History
 * - 1.00 20-07-13  yrz, first implementation.
 * \endinternal
 */
#include "string.h"
#include "am_des.h"

#define bit1    0x80
#define bit2    0x40
#define bit3    0x20
#define bit4    0x10
#define bit5    0x08
#define bit6    0x04
#define bit7    0x02
#define bit8    0x01

const uint8_t sbox1[64] = {
    14,  4, 13,  1,  2, 15, 11,  8,   3,  10,  6, 12,  5,  9,  0,  7, 
     0, 15,  7,  4, 14,  2, 13,  1,  10,   6, 12, 11,  9,  5,  3,  8,     
     4,  1, 14,  8, 13,  6,  2, 11,  15,  12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,   5,  11,  3, 14, 10,  0,  6, 13 
};

const uint8_t sbox2[64] = {
    15,  1,   8,  14,  6, 11,   3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,   4,   7, 15,  2,   8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,   7,  11, 10,  4,  13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8,  10,   1,  3, 15,   4,  2, 11,  6,  7, 12,  0,  5, 14,  9
};
const uint8_t sbox3[64] = {
    10,  0,   9,  14,  6,  3,  15,  5,  1, 13, 12,  7, 11,  4,  2,  8, 
    13,  7,   0,   9,  3,  4,   6, 10,  2,  8,  5, 14, 12, 11, 15,  1, 
    13,  6,   4 ,  9,  8, 15,   3,  0, 11,  1,  2, 12,  5, 10, 14,  7, 
     1, 10,  13,   0,  6,  9,   8,  7,  4, 15, 14,  3, 11,  5,  2, 12
};

const uint8_t sbox4[64] = {
     7, 13,  14,   3,  0,  6,   9, 10,  1,  2,  8,  5, 11, 12,  4, 15, 
    13,  8,  11,   5,  6, 15,   0,  3,  4,  7,  2, 12,  1, 10, 14,  9, 
    10,  6,   9,   0, 12, 11,   7, 13, 15,  1,  3, 14,  5,  2,  8,  4, 
     3, 15,   0,   6, 10,  1,  13,  8,  9,  4,  5, 11, 12,  7,  2, 14 
};

const uint8_t sbox5[64] = {
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9, 
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6, 
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14, 
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3 
};

const uint8_t sbox6[64] = {
    12,  1, 10, 15, 9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11, 
    10, 15,  4,  2, 7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8, 
     9, 14, 15,  5, 2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6, 
     4,  3,  2, 12, 9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13 
};

const uint8_t sbox7[64] = {
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1, 
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6, 
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2, 
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12 
};

const uint8_t sbox8[64] = {
    13,  2,  8, 4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7, 
     1, 15, 13, 8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2, 
     7, 11,  4, 1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8, 
     2,  1, 14, 7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

const uint8_t shift_value[16] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/**
 * \brief am_tdes
 *
 * \param[in]  mode      : mode = 0:加密    mode = 1：解密
 * \param[in]  p_msg_in  : 输入数组 －－明文（加密）或密文（解密）
 * \param[in]  p_key     : 16字节密钥数组
 * \param[out] p_msg_out : 密文（加密）或明文（解密）
 *
 * \return
 */
uint8_t am_tdes (uint8_t  mode,
                 uint8_t *p_msg_in,
                 uint8_t *p_key,
                 uint8_t *p_msg_out)
{
    uint8_t i;
    uint8_t des_key_temp[8];
    uint8_t min[8];
    uint8_t mout[8];
    
    /* 加密 */
    if(mode == AM_ENCRY) {

        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i];
            min[i] = p_msg_in[i];
        }
        am_des(AM_ENCRY, min, des_key_temp, mout);
        
        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i + 8];
            min[i] = mout[i];
        }
        am_des(AM_DECRY, min, des_key_temp, mout);
        
        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i];
            min[i] = mout[i];
        }    
        am_des(AM_ENCRY, min, des_key_temp, mout);
    }

    /* 解密 */
    if (mode == AM_DECRY) {
        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i];
            min[i] = p_msg_in[i];
        }
        am_des(AM_DECRY, min, des_key_temp, mout);
        
        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i + 8];
            min[i] = mout[i];
        }
        am_des(AM_ENCRY, min, des_key_temp, mout);
        
        for (i = 0; i < 8; i++) {
            des_key_temp[i] = p_key[i];
            min[i] = mout[i];
        }    
        am_des(AM_DECRY, min, des_key_temp, mout);
    }

    for (i = 0; i < 8; i++) {
        p_msg_out[i] = mout[i];
    }

    return 8;
}

/**
 * \brief MAC 计算
 *
 * \param[in]  p_init_data : 初始化数据
 * \param[in]  p_mac_key   : 16字节密钥数组
 * \param[in]  data_len    : 数据长度
 * \param[in]  p_in_data   : 数据
 * \param[out] p_mac_data  : MAC计算结果
 *
 * \return ret
 */
uint8_t am_triple_mac (uint8_t *p_init_data,
                       uint8_t *p_mac_key,
                       uint8_t  data_len,
                       uint8_t *p_in_data,
                       uint8_t *p_mac_data)

{

    uint8_t mac_data0[8], mac_data1[8];
    uint8_t len;
    uint8_t i, j;    
    uint8_t low[8], high[8];
     
    for (i = 0; i < 8; i++) {
        low[i]  = p_mac_key[i];
        high[i] = p_mac_key[i+8];
        mac_data0[i] = p_init_data[i];
    }
                 
    len = data_len / 8;
    for (i = 0; i < (8 * (len + 1) - data_len); i++) {
        if (i == 0)
            p_in_data[data_len +i ] = 0x80;
        if (i > 0)
            p_in_data[data_len + i] =0x00;
    }
    data_len = data_len + i;
                       
    len = data_len / 8;

    for (j = 0; j < len; j++) {
        for(i = 0; i < 8; i++) {      
            mac_data0[i] ^= p_in_data[i + j * 8];
        }
        am_des(AM_ENCRY, mac_data0, low, mac_data1);     
        memcpy(mac_data0, mac_data1, 8);
    }

    am_des(AM_DECRY, mac_data0, high, mac_data1);
    memcpy(mac_data0, mac_data1, 8);
    am_des(AM_ENCRY, mac_data0, low, mac_data1);
    
    for(i = 0; i < 8; i++) {
        p_mac_data[i] = mac_data1[i];
    }

    return 8;
}

/**
 * \brief MAC 计算
 *
 * \param[in]  p_init_data : 8字节数组，初始化数据
 * \param[in]  p_mac_key   : 16字节密钥数组
 * \param[in]  data_len    : 数据长度
 * \param[in]  p_in_data   : 数据
 * \param[out] p_mac_data  : 8字节数组，MAC计算结果
 *
 * \return ret
 */
uint8_t am_mac (uint8_t *p_init_data,
                uint8_t *p_mac_key,
                uint8_t  data_len,
                uint8_t *p_in_data,
                uint8_t *p_mac_data)
{
    uint8_t mac_data0[8],mac_data1[8];
    uint8_t len;
    uint8_t i, j;

    memcpy(mac_data0, p_init_data, 8);

    len = data_len / 8;
    for (i = 0; i < (8 * (len + 1) - data_len); i++) {
        if (i == 0)
            p_in_data[data_len + i] = 0x80;
        if (i > 0)
            p_in_data[data_len + i] = 0x00;
    }

    data_len = data_len + i;

    len = data_len / 8;

    for (j = 0; j < len; j++) {
        for (i = 0; i < 8; i++) {
            mac_data0[i] ^= p_in_data[i + j * 8];
        }
        am_des(AM_ENCRY, mac_data0, p_mac_key, mac_data1);     
        memcpy(mac_data0, mac_data1, 8);
    }

     memcpy(p_mac_data, mac_data1, 8);

    return 1;  
}

/**
 * \brief 对一个8字节的字符串进行单DES加解密计算
 *
 * \param[in]  mode      : mode=0:加密 ，mode=1：解密
 * \param[in]  p_msg_in  : 输入数组 －－明文（加密）或密文（解密）
 * \param[in]  p_key     : 8字节密钥数组
 * \param[out] p_msg_out : 密文（加密）或明文（解密）
 *
 * \return ret
 */
uint8_t am_des (uint8_t  mode,
                uint8_t *p_msg_in,
                uint8_t *p_key,
                uint8_t *p_msg_out)
{
    uint8_t i;
    uint8_t temp1,temp2, temp3,temp4;
    uint8_t c0[4] = {0, 0, 0, 0}, d0[4] = {0, 0, 0, 0};
    uint8_t ki[8];

    for(i = 0; i < 8; i++) {
        p_msg_out[0] >>= 1;
        p_msg_out[1] >>= 1;
        p_msg_out[2] >>= 1;
        p_msg_out[3] >>= 1;
        p_msg_out[4] >>= 1;
        p_msg_out[5] >>= 1;
        p_msg_out[6] >>= 1;
        p_msg_out[7] >>= 1;
        
        temp1 = p_msg_in[i];

        p_msg_out[4] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[0] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[5] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[1] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[6] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[2] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[7] |= (temp1 & 0x80);
        temp1 <<= 1;
        p_msg_out[3] |= (temp1 & 0x80);
    }

    /* 密钥处理 */
    /* 序列变换１（PC-1） */
    /* 输出：c0, d0,各28bit */
    for (i = 0; i < 8; i++) {
        c0[0] >>= 1;
        c0[1] >>= 1;
        c0[2] >>= 1;
        c0[3] >>= 1;
        d0[0] >>= 1;
        d0[1] >>= 1;
        d0[2] >>= 1;

        temp1 = p_key[i];
        c0[0] |= (temp1 & 0x80);
        temp1 <<= 1;
        c0[1] |= (temp1 & 0x80);
        temp1 <<= 1;
        c0[2] |= (temp1 & 0x80);
        temp1 <<= 1;
        c0[3] |= (temp1 & 0x80);
        temp1 <<= 1;
        d0[2] |= (temp1 & 0x80);
        temp1 <<= 1;
        d0[1] |= (temp1 & 0x80);
        temp1 <<= 1;
        d0[0] |= (temp1 & 0x80);
    }

    d0[3]  = (c0[3] & 0x0f)| (d0[2] << 4);
    d0[2]  = (d0[2] >> 4) | (d0[1] << 4);
    d0[1]  = (d0[1] >> 4) | (d0[0] << 4);
    d0[0]  = (d0[0] >> 4) & 0x0F;
    c0[3] &= 0xf0;

    for(i = 0; i < 16; i++) {

        /* 计算子密钥,输出Ki */
        if(mode == AM_ENCRY) {

            for(temp1 = 0; temp1 < shift_value[i]; temp1++) {

                if((c0[0] & bit1) != 0)  c0[3] |= bit5;
                c0[0] <<= 1;
                if((c0[1] & bit1) != 0)  c0[0] |= bit8;
                c0[1] <<= 1;
                if((c0[2] & bit1) != 0)  c0[1] |= bit8;
                c0[2] <<= 1;
                if((c0[3] & bit1) != 0)  c0[2] |= bit8;
                c0[3] <<= 1;

                d0[0] <<= 1;
                if((d0[1] & bit1) != 0)  d0[0] |= bit8;
                d0[1] <<= 1;
                if((d0[2] & bit1) != 0)  d0[1] |= bit8;
                d0[2] <<= 1;
                if((d0[3] & bit1) != 0)  d0[2] |= bit8;
                d0[3] <<= 1;
                if((d0[0] & bit4) != 0) {
                     d0[3] |= bit8;
                     d0[0] &= 0x0f;
                }
            }
        }

        /* 把c[i]d[i]串联起来，变成ki,共有56bit */
        ki[0] = c0[0]; 
        ki[1] = c0[1]; 
        ki[2] = c0[2]; 
        ki[3] = c0[3] | d0[0]; 
        ki[4] = d0[1]; 
        ki[5] = d0[2]; 
        ki[6] = d0[3]; 

        /* 序列变换２(PC-2)，变换后存入Ki中，共有48bit */
        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        
        if((ki[0] & bit1) != 0) temp1 |= bit5;  //1
        if((ki[0] & bit2) != 0) temp3 |= bit8;  //2
        if((ki[0] & bit3) != 0) temp1 |= bit7;  //3
        if((ki[0] & bit4) != 0) temp2 |= bit8;  //4
        if((ki[0] & bit5) != 0) temp1 |= bit6;  //5
        if((ki[0] & bit6) != 0) temp2 |= bit2;  //6
        if((ki[0] & bit7) != 0) temp3 |= bit4;  //7
        if((ki[0] & bit8) != 0) temp3 |= bit2;  //8

        if((ki[1] & bit2) != 0) temp2 |= bit4;  //10
        if((ki[1] & bit3) != 0) temp1 |= bit3;  //11
        if((ki[1] & bit4) != 0) temp2 |= bit7;  //12
        if((ki[1] & bit5) != 0) temp3 |= bit7;  //13
        if((ki[1] & bit6) != 0) temp1 |= bit1;  //14
        if((ki[1] & bit7) != 0) temp2 |= bit1;  //15
        if((ki[1] & bit8) != 0) temp3 |= bit3;  //16

        if((ki[2] & bit1) != 0) temp1 |= bit2;  //17

        if((ki[2] & bit3) != 0) temp2 |= bit6;  //19
        if((ki[2] & bit4) != 0) temp3 |= bit6;  //20
        if((ki[2] & bit5) != 0) temp2 |= bit3;  //21

        if((ki[2] & bit7) != 0) temp2 |= bit5;  //23
        if((ki[2] & bit8) != 0) temp1 |= bit4;  //24

        if((ki[3] & bit2) != 0) temp3 |= bit1;  //26
        if((ki[3] & bit3) != 0) temp3 |= bit5;  //27
        if((ki[3] & bit4) != 0) temp1 |= bit8;  //28

        ki[0] = temp1;
        ki[1] = temp2;
        ki[2] = temp3;

        temp1 = 0;
        temp2 = 0;
        temp3 = 0;

        if((ki[3] & bit5) != 0) temp3 |= bit7;
        if((ki[3] & bit6) != 0) temp1 |= bit7;
        if((ki[3] & bit7) != 0) temp1 |= bit3;
        if((ki[3] & bit8) != 0) temp3 |= bit8;

        if((ki[4] & bit1) != 0) temp2 |= bit3;
        if((ki[4] & bit2) != 0) temp3 |= bit1;
        if((ki[4] & bit4) != 0) temp3 |= bit6;
        if((ki[4] & bit5) != 0) temp1 |= bit4;
        if((ki[4] & bit7) != 0) temp2 |= bit7;
        if((ki[4] & bit8) != 0) temp1 |= bit8;

        if((ki[5] & bit1) != 0) temp1 |= bit1;
        if((ki[5] & bit2) != 0) temp3 |= bit4;
        if((ki[5] & bit4) != 0) temp2 |= bit5;
        if((ki[5] & bit5) != 0) temp2 |= bit2;
        if((ki[5] & bit6) != 0) temp3 |= bit3;
        if((ki[5] & bit7) != 0) temp1 |= bit5;
        if((ki[5] & bit8) != 0) temp2 |= bit4;

        if((ki[6] & bit1) != 0) temp2 |= bit6;
        if((ki[6] & bit2) != 0) temp3 |= bit5;
        if((ki[6] & bit3) != 0) temp2 |= bit1;
        if((ki[6] & bit4) != 0) temp1 |= bit2;
        if((ki[6] & bit5) != 0) temp3 |= bit2;
        if((ki[6] & bit7) != 0) temp1 |= bit6;
        if((ki[6] & bit8) != 0) temp2 |= bit8;

        ki[3] = temp1;
        ki[4] = temp2;
        ki[5] = temp3;

        /* 此时已计算出Ki */
        /* 把32位R扩展成48位,即把MsgOut[4]、p_msg_out[５] */
        /* p_msg_out[６]、p_msg_out[７]扩展,取得然后与Ki异或 */

        /* 1 */
        temp1 =((p_msg_out[4] >> 1) & 0x7c);
        if((p_msg_out[7] & bit8) != 0) temp1 |= bit1;
        if((p_msg_out[4] & bit4) != 0) temp1 |= bit7;
        if((p_msg_out[4] & bit5) != 0) temp1 |= bit8;

        /* 扩展后R与Ki异或 */
         ki[0] ^= temp1;

        /* 2 */
        temp1 = ((p_msg_out[4] << 5) & 0xE0);
        if((p_msg_out[5] & bit1) != 0) temp1 |= bit4;
        if((p_msg_out[4] & bit8) != 0) temp1 |= bit5;
        temp1 |= ((p_msg_out[5] >> 5) & 0x07);
         
        /* 扩展后R与Ki异或 */
        ki[1] ^= temp1;
         
        /* 3 */
        temp1 = ((p_msg_out[5] << 3) & 0xc0) | ((p_msg_out[5] << 1) & 0x3e);
        if((p_msg_out[6] & bit1) != 0) temp1 |= bit8;
        ki[2] ^= temp1;
         
        /* 4 */
        temp1 = ((p_msg_out[6] >> 1) & 0x7c);
        if((p_msg_out[5] & bit8) != 0) temp1 |= bit1;
        temp1 |= ((p_msg_out[6] >> 3) & 0x03);
        ki[3] ^= temp1;

        /* 5 */
        temp1 = ((p_msg_out[6] << 5) & 0xE0);
        if((p_msg_out[7] & bit1) != 0) temp1 |= bit4;
        if((p_msg_out[6] & bit8) != 0) temp1 |= bit5;
        temp1 |= ((p_msg_out[7] >> 5) & 0x07);

        ki[4] ^= temp1;
         
        /* 6 */
        temp1 = ((p_msg_out[7] << 1) & 0x3E);
        if((p_msg_out[7] & bit4) != 0) temp1 |= (bit1);
        if((p_msg_out[7] & bit5) != 0) temp1 |= (bit2);
        if((p_msg_out[4] & bit1) != 0) temp1 |= bit8;

        /* 扩展后R与Ki异或 */
        ki[5] ^= temp1;

        temp1 = 0;

        if((ki[5] & bit8) != 0) temp1 |= bit4;
        if((ki[5] & bit7) != 0) temp1 |= bit8;
        if((ki[5] & bit6) != 0) temp1 |= bit7;
        if((ki[5] & bit5) != 0) temp1 |= bit6;
        if((ki[5] & bit4) != 0) temp1 |= bit5;
        if((ki[5] & bit3) != 0) temp1 |= bit3;
        ki[7] = sbox8[temp1]; 

        temp1 = 0;
        if((ki[5] & bit2) != 0) temp1 |= bit4;
        if((ki[5] & bit1) != 0) temp1 |= bit8;
        if((ki[4] & bit8) != 0) temp1 |= bit7;
        if((ki[4] & bit7) != 0) temp1 |= bit6;
        if((ki[4] & bit6) != 0) temp1 |= bit5;
        if((ki[4] & bit5) != 0) temp1 |= bit3;
        ki[6] = sbox7[temp1];
         
        temp1 = 0;
        if((ki[4] & bit4) != 0) temp1 |= bit4;
        if((ki[4] & bit3) != 0) temp1 |= bit8;
        if((ki[4] & bit2) != 0) temp1 |= bit7;
        if((ki[4] & bit1) != 0) temp1 |= bit6;
        if((ki[3] & bit8) != 0) temp1 |= bit5;
        if((ki[3] & bit7) != 0) temp1 |= bit3;
        ki[5] = sbox6[temp1];

        temp1 = 0;
        if((ki[3] & bit6) != 0) temp1 |= bit4;
        if((ki[3] & bit5) != 0) temp1 |= bit8;
        if((ki[3] & bit4) != 0) temp1 |= bit7;
        if((ki[3] & bit3) != 0) temp1 |= bit6;
        if((ki[3] & bit2) != 0) temp1 |= bit5;
        if((ki[3] & bit1) != 0) temp1 |= bit3;
        ki[4] = sbox5[temp1];

        temp1 = 0;
        if((ki[2] & bit8) != 0) temp1 |= bit4;
        if((ki[2] & bit7) != 0) temp1 |= bit8;
        if((ki[2] & bit6) != 0) temp1 |= bit7;
        if((ki[2] & bit5) != 0) temp1 |= bit6;
        if((ki[2] & bit4) != 0) temp1 |= bit5;
        if((ki[2] & bit3) != 0) temp1 |= bit3;
        ki[3] = sbox4[temp1];
        
        temp1 = 0;
        if((ki[2] & bit2) != 0) temp1 |= bit4;
        if((ki[2] & bit1) != 0) temp1 |= bit8;
        if((ki[1] & bit8) != 0) temp1 |= bit7;
        if((ki[1] & bit7) != 0) temp1 |= bit6;
        if((ki[1] & bit6) != 0) temp1 |= bit5;
        if((ki[1] & bit5) != 0) temp1 |= bit3;
        ki[2] = sbox3[temp1];

        temp1 = 0;
        if((ki[1] & bit4) != 0) temp1 |= bit4;
        if((ki[1] & bit3) != 0) temp1 |= bit8;
        if((ki[1] & bit2) != 0) temp1 |= bit7;
        if((ki[1] & bit1) != 0) temp1 |= bit6;
        if((ki[0] & bit8) != 0) temp1 |= bit5;
        if((ki[0] & bit7) != 0) temp1 |= bit3;
        ki[1] = sbox2[temp1];

        temp1 = 0;
        if((ki[0] & bit6) != 0) temp1 |= bit4;
        if((ki[0] & bit5) != 0) temp1 |= bit8;
        if((ki[0] & bit4) != 0) temp1 |= bit7;
        if((ki[0] & bit3) != 0) temp1 |= bit6;
        if((ki[0] & bit2) != 0) temp1 |= bit5;
        if((ki[0] & bit1) != 0) temp1 |= bit3;
        ki[0] = sbox1[temp1];

        /* 把上面的结果进行P置换 */
        temp1 = 0;
        temp2 = 0;
        temp3 = 0;
        temp4 = 0;

        if(ki[0] & bit5)        //1
            temp2 |= bit1;
        if(ki[0] & bit6)        //2
            temp3 |= bit1;
        if(ki[0] & bit7)        //3
            temp3 |= bit7;
        if(ki[0] & bit8)        //4
            temp4 |= bit7;

        if(ki[1] & bit5)        //5
            temp2 |= bit5;
        if(ki[1] & bit6)        //6
            temp4 |= bit4;
        if(ki[1] & bit7)        //7
            temp1 |= bit2;
        if(ki[1] & bit8)        //8
            temp3 |= bit2;

        if(ki[2] & bit5)        //9
            temp3 |= bit8;
        if(ki[2] & bit6)        //10
            temp2 |= bit8;
        if(ki[2] & bit7)        //11
            temp4 |=bit6;
        if(ki[2] & bit8)        //12
            temp1 |= bit6;

        if(ki[3] & bit5)        //13
            temp4 |= bit2;
        if(ki[3] & bit6)        //14
            temp3 |= bit4;
        if(ki[3] & bit7)        //15
            temp2 |= bit2;
        if(ki[3] & bit8)        //16
            temp1 |= bit1;

        if(ki[4] & bit5)        //17
            temp1 |= bit8;
        if(ki[4] & bit6)        //18
            temp2 |= bit6;
        if(ki[4] & bit7)        //19
            temp4|=bit1;
        if(ki[4] & bit8)        //20
            temp1 |= bit3;

        if(ki[5] & bit5)        //21
            temp1 |= bit4;
        if(ki[5] & bit6)        //22
            temp4 |= bit5;
        if(ki[5] & bit7)        //23
            temp2 |= bit3;
        if(ki[5] & bit8)        //24
        temp3 |= bit3;

        if(ki[6] & bit5)        //25
            temp4 |= bit8;
        if(ki[6] & bit6)        //26
            temp2 |= bit4;
        if(ki[6] & bit7)        //27
            temp3 |= bit6;
        if(ki[6] & bit8)        //28
            temp1 |= bit7;

        if(ki[7] & bit5)        //29
            temp1 |= bit5;
        if(ki[7] & bit6)        //30
            temp4 |= bit3;
        if(ki[7] & bit7)        //31
            temp2 |= bit7;
        if(ki[7] & bit8)        //32
            temp3 |= bit5;

        /* 保存R */
        ki[0] = p_msg_out[4];
        ki[1] = p_msg_out[5];
        ki[2] = p_msg_out[6];
        ki[3] = p_msg_out[7];
            
        /* 计算新的R */  
        p_msg_out[4] = temp1^p_msg_out[0];    
        p_msg_out[5] = temp2^p_msg_out[1];    
        p_msg_out[6] = temp3^p_msg_out[2];    
        p_msg_out[7] = temp4^p_msg_out[3];

        /* 用上一次的R作为这一次的L */
        p_msg_out[0] = ki[0];
        p_msg_out[1] = ki[1];
        p_msg_out[2] = ki[2];
        p_msg_out[3] = ki[3];

        if (mode != AM_ENCRY) {
            for(temp1 = 0; temp1 < shift_value[15 - i]; temp1++) {

                c0[3] >>= 1;
                if((c0[2] & bit8) != 0) c0[3] |= bit1;
                c0[2] >>= 1;
                if((c0[1] & bit8) != 0) c0[2] |= bit1;
                c0[1] >>= 1;
                if((c0[0] & bit8) != 0) c0[1] |= bit1;
                c0[0] >>= 1;

                if((c0[3] & bit5) != 0) {
                    c0[3] &= 0xF0;
                    c0[0] |= bit1;
                }

                if((d0[3] & bit8) != 0) d0[0] |= bit4;
                d0[3] >>= 1;
                if((d0[2] & bit8) != 0) d0[3] |= bit1;
                d0[2] >>= 1;
                if((d0[1] & bit8) != 0) d0[2] |= bit1;
                d0[1] >>= 1;
                if((d0[0] & bit8) != 0) d0[1] |= bit1;
                d0[0] >>= 1;
            }
    
        }
    }

    /* 对R[16]low[16]进行置换，Final Permutation (IP**-1) */
    ki[0] = p_msg_out[0];
    ki[1] = p_msg_out[4];
    ki[2] = p_msg_out[1];
    ki[3] = p_msg_out[5];
            
    ki[4] = p_msg_out[2];
    ki[5] = p_msg_out[6];
    ki[6] = p_msg_out[3];
    ki[7] = p_msg_out[7];

    for(i = 0; i < 8; i++)
    {
        p_msg_out[0] <<= 1;
        p_msg_out[1] <<= 1;
        p_msg_out[2] <<= 1;
        p_msg_out[3] <<= 1;
        p_msg_out[4] <<= 1;
        p_msg_out[5] <<= 1;
        p_msg_out[6] <<= 1;
        p_msg_out[7] <<= 1;

        p_msg_out[0] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[1] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[2] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[3] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[4] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[5] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[6] |= (ki[i] & 0x01);
        ki[i] >>= 1;
        p_msg_out[7] |= (ki[i] & 0x01);
    }
    return 8;
    /*
    40 8 48 16 56 24 64 32 
    39 7 47 15 55 23 63 31 
    38 6 46 14 54 22 62 30 
    37 5 45 13 53 21 61 29 
    36 4 44 12 52 20 60 28 
    35 3 43 11 51 19 59 27 
    34 2 42 10 50 18 58 26 
    33 1 41 9  49 17 57 25 
    */
}
 
/* end of file */
