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
 * - 1.00 20-05-22
 * \endinternal
 */

#ifndef __AMHW_HC32F460_AES_H
#define __AMHW_HC32F460_AES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32f460_if_aes
 * \copydoc amhw_hc32f460_aes.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

/* 默认使能匿名联合体 */
#elif defined(__TMS470__)

/* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

/**
 * \brief AES - 寄存器组
 */

/**
 * \brief AES控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t START                     : 1;  /** <brief 启动 */
    __IO uint32_t MODE                      : 1;  /** <brief 功能选择 */
    uint32_t RESERVED2                      :30;  /** <brief 保留 */
} stc_aes_cr_field_t;

typedef struct amhw_hc32f460_aes {
    union
    {
        __IO uint32_t CR;       /** <brief AES控制寄存器 */
        stc_aes_cr_field_t CR_f;
    };
    uint8_t RESERVED1[12];      /** <brief 保留 */
    __IO uint32_t DR[4];        /** <brief AES数据寄存器 */
    __IO uint32_t KR[4];        /** <brief AES密钥寄存器 */
} amhw_hc32f460_aes_t;



/**
 * \brief AES 加密运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_aes_encrypt (amhw_hc32f460_aes_t *p_hw_aes)
{
    p_hw_aes->CR &= ~(1 << 1);
}

/**
 * \brief AES 解密运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_aes_decrypt (amhw_hc32f460_aes_t *p_hw_aes)
{
    p_hw_aes->CR |= (1 << 1);
}

/**
 * \brief AES 模块启动运算
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_aes_start (amhw_hc32f460_aes_t *p_hw_aes)
{
    p_hw_aes->CR |= (1 << 0);
}

/**
 * \brief AES 模块状态查询
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 *
 * \retval : 无
 */
am_static_inline
int amhw_hc32f460_aes_stu_check (amhw_hc32f460_aes_t *p_hw_aes)
{
    return (p_hw_aes->CR & (1 << 0));
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
void amhw_hc32f460_aes_data_write (amhw_hc32f460_aes_t *p_hw_aes, uint32_t *p_data)
{
    int i = 0;
    for (i = 0; i < 4; i++){
        p_hw_aes->DR[i] = p_data[i];
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
void amhw_hc32f460_aes_data_read (amhw_hc32f460_aes_t *p_hw_aes,
                                  uint32_t          *p_data)
{
    int i = 0;

    for (i = 0; i < 4; i++){

        p_data[i] = p_hw_aes->DR[i];
    }
}

/**
 * \brief AES 密钥存放
 *
 * \param[in] p_hw_aes : 指向AES寄存器组基地址指针
 * \param[in] flag     : 密钥长度，参考宏定义：AES 密钥长度掩码
 * \param[in] p_data   : 密钥数据（只支持128位）
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
void amhw_hc32f460_aes_key_save (amhw_hc32f460_aes_t *p_hw_aes,
                                 uint32_t            *p_data)
{
    int i = 0;

    for (i = 0; i < 4; i++){
        p_hw_aes->KR[i] = p_data[i];
    }

}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

/* 允许匿名联合体使能 */
#elif defined(__GNUC__)

/* 默认使用匿名联合体 */
#elif defined(__TMS470__)

/* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @} amhw_if_hc32f460_aes
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_AES_H */

/* end of file */
