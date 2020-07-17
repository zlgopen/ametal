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
 * \brief DES 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-07-13  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_DES_H
#define __AM_DES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

#define AM_ENCRY    0x0
#define AM_DECRY    0x1

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
                 uint8_t *p_msg_out);

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
                       uint8_t *p_mac_data);

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
                uint8_t *p_mac_data);

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
                uint8_t *p_msg_out);

#ifdef __cplusplus
}
#endif

#endif /* __AM_DES_H */

/* end of file */
