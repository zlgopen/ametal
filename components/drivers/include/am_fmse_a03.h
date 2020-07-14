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
 * \brief 安全芯片FMSE-A03 头文件
 *
 * \internal
 * \par modification history
 * - 1.00 20-07-01  yrz, first implementation.
 * \endinternal
 */

#ifndef __AM_FMSE_H
#define __AM_FMSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_i2c.h"
#include "am_spi.h"

/* gen rsa key cmd para define */
#define RSA1024				0
#define RSA1280				1
#define RSA2048				2

#define EXP65537			0
#define EXP3				1

typedef struct{
    uint8_t cla;
    uint8_t ins;
    uint8_t p1;
    uint8_t p2;
    union{
        uint8_t lc;
        uint8_t le;
    }p3;
    uint8_t capdu[256];
} apdu_pack_t;
    
/**
 * \brief fmse 驱动函数结构体
 */
struct am_fmse_drv_funcs{

    uint8_t (*fmse_apdu_transceive)(void     *p_drv,
                                    uint8_t  *sbuf,
                                    uint16_t  slen,
                                    uint8_t  *rbuf,
                                    uint16_t *rlen, 
                                    uint16_t  poll_inv,
                                    uint32_t  poll_timeout);
};

/**
 * \brief fmse 服务
 */
typedef struct am_fmse_serv {

    /** \brief fmse 驱动函数结构体指针 */
    const struct am_fmse_drv_funcs *p_funcs;

    /** \brief 用于驱动函数的第一个参数 */
    void                           *p_drv;
    
} am_fmse_serv_t;

/** \brief fmse 标准服务操作句柄类型定义  */
typedef am_fmse_serv_t *am_fmse_handle_t;

/**
 * \brief 内存初始化
 *
 * \param[in] p_dst  : 指向待初始化内存的指针
 * \param[in] val    : 初始化的值
 * \param[in] count  : 初始化的内存长度
 *
 * \return 初始化内存首地址
 */
void* am_fmse_memset(void* p_dst, int val, size_t count);

/**
 * \brief 内存数据复制
 *
 * \param[in] p_dst  : 指向目的地址的指针
 * \param[in] p_src  : 指向源地址的指针
 * \param[in] count  : 复制的数据长度
 *
 * \return 目的地址的指针
 */
void* am_fmse_memmove(void* p_dst, const void* p_src, size_t count);

/**
 * \brief 文件创建
 *
 * \param[in] handle : FMSE标准服务句柄
 *
 * \return 状态码
 */
uint16_t am_fmse_file_create (am_fmse_handle_t handle,
                              uint16_t         fid,
                              uint8_t          type,
                              uint16_t         space,
                              uint16_t         rank);

/**
 * \brief 随机数获取
 *
 * \param[in] handle : FMSE标准服务句柄
 *
 * \return 状态码
 */
uint16_t am_fmse_challenge_get (am_fmse_handle_t handle,
                                uint16_t         inlen,
                                uint8_t         *rbuf,
                                uint16_t        *rlen);

/**
 * \brief 文件选择
 *
 * \param[in] handle : FMSE标准服务句柄
 *
 * \return 状态码
 */
uint16_t am_fmse_file_select (am_fmse_handle_t handle,
                              uint16_t         fid,
                              uint8_t         *rbuf,
                              uint16_t        *rlen);

/**
 * \brief 校验
 *
 * \param[in] handle : FMSE标准服务句柄
 *
 * \return 状态码
 */
uint16_t am_fmse_pin_verify (am_fmse_handle_t  handle,
                             uint16_t          inlen,
                             uint8_t          *inbuf);

/**
 * \brief 二进制文件读取
 *
 * \param[in] handle : FMSE标准服务句柄
 * \param[in] sfi    : 文件起始位置
 * \param[in] inlen  : 读取的长度
 * \param[in] rbuf   : 接收缓冲区
 * \param[in] rlen   : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_binary_read (am_fmse_handle_t handle,
                              uint16_t         sfi,
                              uint16_t         inlen,
                              uint8_t         *rbuf,
                              uint16_t         *rlen);

/**
 * \brief RSA 密钥对生成
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] key_len : 密钥对长度
 * \param[in] exp     : 密钥对指数
 * \param[in] fid     : the Fid that save the dest RSA Key Pair
 *                      input 4Bytes Fid format: public key fid + private key fid
 *                      single 2Bytes Fid format:high byte + low byte
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_rsa_key_pair_gen(am_fmse_handle_t handle,
                                  uint8_t          key_len,
                                  uint8_t          exp,
                                  uint32_t         fid,
                                  uint8_t         *rbuf,
                                  uint16_t        *rlen);

/**
 * \brief SM2 密钥对生成
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] fid     : the Fid that save the dest RSA Key Pair
 *                      input 4Bytes Fid format: public key fid + private key fid
 *                      single 2Bytes Fid format:high byte + low byte
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_sm2_key_pair_gen (am_fmse_handle_t handle,
                                   uint32_t         fid,
                                   uint8_t         *rbuf,
                                   uint16_t        *rlen);

/**
 * \brief 导入会话密钥
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input session key buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_session_key_import (am_fmse_handle_t handle,
                                     uint16_t         p1p2,
                                     uint16_t         inlen,
                                     uint8_t         *inbuf,
                                     uint8_t         *rbuf,
                                     uint16_t        *rlen);

/**
 * \brief data encrypt and decrypt
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_data_encrypt_and_decrypt(am_fmse_handle_t handle,
                                          uint16_t         p1p2,
                                          uint16_t         inlen,
                                          uint8_t         *inbuf,
                                          uint8_t         *rbuf,
                                          uint16_t        *rlen);

/**
 * \brief RSA public key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_rsa_pub_key_cal(am_fmse_handle_t handle,
                                 uint16_t         p1p2,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief RSA private key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_rsa_pri_key_cal(am_fmse_handle_t handle,
                                 uint16_t         p1p2,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief SM2 public key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_sm2_pub_key_cal(am_fmse_handle_t handle,
                                 uint8_t          mode,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief SM2 private key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_sm2_pri_key_cal(am_fmse_handle_t handle,
                                 uint8_t          mode,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief Install RSA key
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : key type and data block flag and RSA type
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_rsa_key_install (am_fmse_handle_t handle,
                                  uint16_t         p1p2,
                                  uint16_t         inlen,
                                  uint8_t         *inbuf,
                                  uint8_t         *rbuf,
                                  uint16_t        *rlen);

/**
 * \brief WriteBinary
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] sfi     : file start position to write
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_binary_write (am_fmse_handle_t handle,
                               uint16_t         sfi,
                               uint16_t         inlen,
                               uint8_t         *inbuf,
                               uint8_t         *rbuf,
                               uint16_t        *rlen);

/**
 * \brief Data_Compress
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : 
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_data_compress(am_fmse_handle_t handle,
                               uint16_t         p1p2,
                               uint16_t         inlen,
                               uint8_t         *inbuf,
                               uint8_t         *rbuf,
                               uint16_t        *rlen);

/**
 * \brief Export Public Key
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : 
 * \param[in] inlen   : file length to write
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_pub_key_export(am_fmse_handle_t handle,
                                uint16_t         p1p2,
                                uint16_t         inlen,
                                uint8_t         *inbuf,
                                uint8_t         *rbuf,
                                uint16_t         *rlen);

/**
 * \brief Gen ECC Key Pair
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : 
 * \param[in] Fid     : the Fid that save the dest Key Pair
 *                      input 4Bytes Fid format: public key fid + private key fid
 *                      single 2Bytes Fid format:high byte + low byte
 * \param[in] rbuf    : 接收缓冲区
 * \param[in] rlen    : 接收的长度
 *
 * \return 状态码
 */
uint16_t am_fmse_ecc_key_pair_gen(am_fmse_handle_t handle,
                                  uint16_t         p1p2,
                                  uint32_t         fid,
                                  uint8_t         *rbuf,
                                  uint16_t        *rlen);

/**
 * \brief ECC public key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : 
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : the return data buffer
 * \param[in] rlen    : the return data length
 *
 * \return ret
 */
uint16_t am_fmse_ecc_pub_key_cal(am_fmse_handle_t handle,
                                 uint16_t         p1p2,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief ECC private key calculate
 *
 * \param[in] handle  : FMSE标准服务句柄
 * \param[in] p1p2    : 
 * \param[in] inlen   : file length to read
 * \param[in] inbuf   : input data buffer
 * \param[in] rbuf    : the return data buffer
 * \param[in] rlen    : the return data length
 *
 * \return ret
 */
uint16_t am_fmse_ecc_pri_key_cal(am_fmse_handle_t handle,
                                 uint16_t         p1p2,
                                 uint16_t         inlen,
                                 uint8_t         *inbuf,
                                 uint8_t         *rbuf,
                                 uint16_t        *rlen);

/**
 * \brief External Authentication
 *
 * \param[in]  handle      : FMSE标准服务句柄
 * \param[in]  random_send : input lacal random data 
 * \param[in]  random_get  : input random data from SE
 * \param[in]  ea_key      : input External_Authentication Key
 * \param[out] None
 *
 * \return ret
 */
uint16_t am_fmse_external_authentication (am_fmse_handle_t  handle,
                                          uint8_t          *random_send,
                                          uint8_t          *random_get,
                                          uint8_t          *ea_key);

/**
 * \brief Change PIN
 *
 * \param[in]  handle      : FMSE标准服务句柄
 * \param[in]  new_pin     : input pin 
 * \param[in]  new_pin_len : input new_pin_len
 * \param[in]  old_pin     : input old_pin
 * \param[in]  old_pin_len : input old_pin_len
 *
 * \return ret
 */
uint16_t am_fmse_pin_change (am_fmse_handle_t handle,
                             uint8_t         *new_pin,
                             uint8_t          new_pin_len,
                             uint8_t         *old_pin,
                             uint8_t          old_pin_len);

/**
 * \brief Write Key
 *
 * \param[in]  handle : FMSE标准服务句柄
 * \param[in]  p1p2   : 
 * \param[in]  inlen  : data length to write
 * \param[in]  inbuf  : input data buffer
 * \param[in]  rbuf   : the return data buffer
 * \param[in]  rlen   : the return data length
 *            
 * \return ret
 */
uint16_t am_fmse_key_write (am_fmse_handle_t handle,
                            uint16_t         p1p2,
                            uint16_t         inlen,
                            uint8_t         *inbuf,
                            uint8_t         *rbuf,
                            uint16_t        *rlen);

#ifdef __cplusplus
}
#endif

#endif /* __AM_FMSE_H */

/* end of file */
