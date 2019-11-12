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
 * \brief AES驱动，服务AES标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-08
 * \endinternal
 */
#ifndef __AM_HC32_AES_H
#define __AM_HC32_AES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \brief AES密钥长度
 */
#define AM_HC32_AES_KEY_LEN_128_BIT    0    /**< \brief 128位密钥长度 */
#define AM_HC32_AES_KEY_LEN_192_BIT    1    /**< \brief 192位密钥长度 */
#define AM_HC32_AES_KEY_LEN_256_BIT    2    /**< \brief 256位密钥长度 */

/**
 * \addtogroup am_hc32_if_aes
 * \copydoc am_hc32_aes.h
 * @{
 */
/**
 * \brief aes设备信息结构体
 */
typedef struct am_hc32_aes_info {

    uint32_t    reg_base;  /**< \brief AES寄存器块基地址 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
}am_hc32_aes_devinfo_t;

/**
 * \brief AES设备实例
 */
typedef struct am_hc32_aes_dev {

    /** \brief 指向AES设备信息的指针 */
    const am_hc32_aes_devinfo_t *p_devinfo;

} am_hc32_aes_dev_t;

/** \brief aes标准服务操作句柄类型定义 */
typedef am_hc32_aes_dev_t *am_aes_handle_t;

/**
 * \brief AES 初始化
 *
 * \param[in] p_dev     :AES设备初始化实例
 * \param[in] p_devinfo :AES设备信息初始化实例
 *
 * \retval :无
 *
 */
am_aes_handle_t am_hc32_aes_init(am_hc32_aes_dev_t           *p_dev,
                                   const am_hc32_aes_devinfo_t *p_devinfo);

/**
 * \brief AES 去始化
 *
 * \param[in] handle     :AES标准设备服务句柄
 *
 * \retval :无
 *
 */
void am_hc32_aes_deinit (am_aes_handle_t handle);

/**
 * \brief AES 加密
 *
 * \param[in] : handle 标准AES服务句柄
 * \param[in] : key_len 密钥长度
 * \param[in] : p_aes_data 加密数据
 * \param[in] : p_key_data 密钥
 * \param[in] : p_ency_data 加密数据缓存区
 *
 * \retval : AM_ERROR 加密运算成功
 *           AM_ERROR 加密运算失败
 */
uint32_t am_hc32_aes_encrypt (am_aes_handle_t handle,
                                uint8_t         key_len,
                                uint32_t       *p_aes_data,
                                uint32_t       *p_key_data,
                                uint32_t       *p_ency_data);

/**
 * \brief AES 解密
 *
 * \param[in] : handle 标准AES服务句柄
 * \param[in] : key_len 密钥长度
 * \param[in] : p_ency_data 解密数据
 * \param[in] : p_key_data 密钥
 * \param[in] : p_decy_data 解密数据缓存区
 *
 * \retval : AM_OK 解密运算结束
 *           AM_ERROR 揭秘运算失败
 */
uint32_t am_hc32_aes_decrypt (am_aes_handle_t handle,
                                uint8_t         key_len,
                                uint32_t       *p_aes_data,
                                uint32_t       *p_key_data,
                                uint32_t       *p_decy_data);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC32_AES_H */

/* end of file */
