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
 * \brief AES 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 19-10-10
 * \endinternal
 */

#ifndef __AMHW_ZSL42x_AES_H
#define __AMHW_ZSL42x_AES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_zsl42x_if_aes
 * \copydoc amhw_zsl42x_aes.h
 * @{
 */

/**
 * \brief AES - 寄存器组
 */
typedef struct amhw_zsl42x_aes {
    __IO uint32_t  aescr;         /**< \brief  AES 控制寄存器 */
         uint32_t  res1[3];       /**< \brief  保留 */
    __IO uint32_t  aesdat[4];     /**< \brief  AES 数据寄存器 */
    __IO uint32_t  aeskey[8];     /**< \brief  AES 密钥寄存器 */
} amhw_zsl42x_aes_t;

/**
 * \brief AES 密钥长度掩码
 */
#define  AMHW_ZSL42x_AES_KEY_LEN_128_BIT    (0U)
#define  AMHW_ZSL42x_AES_KEY_LEN_192_BIT    (1U)
#define  AMHW_ZSL42x_AES_KEY_LEN_256_BIT    (2U)

/**
 * \brief AES 密钥长度设置
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 * \param[in] flag     : 密钥长度，参考宏定义：AES 密钥长度掩码
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_key_len_sel (amhw_zsl42x_aes_t *p_hw_aes, uint32_t flag)
{
    p_hw_aes->aescr = (p_hw_aes->aescr & (~(0x3 << 3))) | (flag << 3);
}

/**
 * \brief AES 加密运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_encrypt (amhw_zsl42x_aes_t *p_hw_aes)
{
    p_hw_aes->aescr &= ~(1 << 1);
}

/**
 * \brief AES 解密运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_decrypt (amhw_zsl42x_aes_t *p_hw_aes)
{
    p_hw_aes->aescr |= (1 << 1);
}

/**
 * \brief AES 模块启动运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_start (amhw_zsl42x_aes_t *p_hw_aes)
{
    p_hw_aes->aescr |= (1 << 0);
}

/**
 * \brief AES 模块状态查询
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
int amhw_zsl42x_aes_stu_check (amhw_zsl42x_aes_t *p_hw_aes)
{
    return (p_hw_aes->aescr & (1 << 0));
}

/**
 * \brief AES 密文、明文数据存放
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \note1 :数据寄存器由四个 32位的寄存器组成 128位数据，用于在模块运算前存放需要被 位数据，
 *         用于在模块运算前存放需要被 加密 的明文或者需要被解，并且运算完成后存放加密 的明或
 *         者需要被解，并且运算完成后存放加密 的明文或者需要被解，并且运算完成后存放加密 的
 *         明文或者需要被解，并且运算完成后存放加密后的密文或者解密后的明文。
 *
 * \note2 : 对于本寄存器的写入只能在模块没 有处于运算状态时,否则硬件将自动忽略该操作。
 *          对于本寄存器的读取只能在模块没有处运算状态时 才能进行，否则对本寄存器的读取将得到全 0。
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_data_write (amhw_zsl42x_aes_t *p_hw_aes, uint32_t *p_data)
{
    int i = 0;
    for (i = 0; i < 4; i++){
        p_hw_aes->aesdat[i] = (p_hw_aes->aesdat[i] & ~(0xffff)) | p_data[i];
    }
}

/**
 * \brief AES 密文、明文数据读取
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \note1 :数据寄存器由四个 32位的寄存器组成 128位数据，用于在模块运算前存放需要被 位数据，
 *         用于在模块运算前存放需要被 加密 的明文或者需要被解，并且运算完成后存放加密 的明或
 *         者需要被解，并且运算完成后存放加密 的明文或者需要被解，并且运算完成后存放加密 的
 *         明文或者需要被解，并且运算完成后存放加密后的密文或者解密后的明文。
 *
 * \note2 : 对于本寄存器的写入只能在模块没 有处于运算状态时,否则硬件将自动忽略该操作。
 *          对于本寄存器的读取只能在模块没有处运算状态时 才能进行，否则对本寄存器的读取将得到全 0。
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_data_read (amhw_zsl42x_aes_t *p_hw_aes,
                                uint32_t          *p_data)
{
    int i = 0;

    for (i = 0; i < 4; i++){

        p_data[i] = p_hw_aes->aesdat[i];
    }
}

/**
 * \brief AES 密钥存放
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 * \param[in] flag     : 密钥长度，参考宏定义：AES 密钥长度掩码
 * \param[in] p_data   : 密钥数据（只支持128位、192位、256位）
 *
 * \note1 :数据寄存器由四个 32位的寄存器组成 128位数据，用于在模块运算前存放需要被 位数据，
 *         用于在模块运算前存放需要被 加密 的明文或者需要被解，并且运算完成后存放加密 的明或
 *         者需要被解，并且运算完成后存放加密 的明文或者需要被解，并且运算完成后存放加密 的
 *         明文或者需要被解，并且运算完成后存放加密后的密文或者解密后的明文。
 *
 * \note2 : 对于本寄存器的写入只能在模块没 有处于运算状态时,否则硬件将自动忽略该操作。
 *          对于本寄存器的读取只能在模块没有处运算状态时 才能进行，否则对本寄存器的读取将得到全 0。
 *
 * \retval : 无
 */
am_static_inline
void amhw_zsl42x_aes_key_save (amhw_zsl42x_aes_t *p_hw_aes,
                               uint32_t           flag,
                               uint32_t          *p_data)
{
    int i = 0;

    switch (flag){

        case AMHW_ZSL42x_AES_KEY_LEN_128_BIT:
            for (i = 0; i < 4; i++){
                p_hw_aes->aeskey[i] = (p_hw_aes->aeskey[i] & ~(0xffff)) | p_data[i];
            }
            break;
        case AMHW_ZSL42x_AES_KEY_LEN_192_BIT:
            for (i = 0; i < 6; i++){
                p_hw_aes->aeskey[i] = (p_hw_aes->aeskey[i] & ~(0xffff)) | p_data[i];
            }
            break;
        case AMHW_ZSL42x_AES_KEY_LEN_256_BIT:
            for (i = 0; i < 8; i++){
                p_hw_aes->aeskey[i] = (p_hw_aes->aeskey[i] & ~(0xffff)) | p_data[i];
            }
            break;
        default :
            ;
    }
}

/**
 * @} amhw_if_zsl42x_aes
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ZSL42x_AES_H */

/* end of file */
