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
 * \brief aes 系统复位例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   AES对数据加密然后解密，如果解密出来数据与加密前数据一样，则串口打印“Aes ency-decy success!\r\n”。
 *   反之打印"Aes ency-decy failed!\r\n"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32_hw_aes_ency_decy.c src_hc32_hw_aes_ency_decy
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32_hw_aes_ency_decy
 * \copydoc demo_hc32_hw_aes_ency_decy.c
 */

/** [src_hc32_hw_aes_ency_decy] */
#include "ametal.h"
#include "am_hc32.h"
#include "am_board.h"
#include "am_hc32_aes.h"
#include "hw/amhw_hc32_aes.h"

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_hc32_aes_t       *gp_hw_aes   = NULL;  /**< \brief aes 外设 */

/**< \brief 存放加密后的数据 */
static uint32_t ency_data[4] = {0};

/**< \brief 存放解密后的数据 */
static uint32_t decy_data[4]  = {0};

/**
 * \brief AES 加密
 */
static void aes_ency (uint8_t key_len,
                     uint32_t *p_aes_data,
                     uint32_t *p_key_data,
                     uint32_t *p_ency_data)
{
    /* 保存需要加密的明文数据 */
    amhw_hc32_aes_data_write (gp_hw_aes, p_aes_data);

    /* 加载密钥  */
    switch (key_len){

        case AM_HC32_AES_KEY_LEN_128_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_128_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_128_BIT,
                                      p_key_data);
            break;
        case AM_HC32_AES_KEY_LEN_192_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_192_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_192_BIT,
                                      p_key_data);
            break;
        case AM_HC32_AES_KEY_LEN_256_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_256_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_256_BIT,
                                      p_key_data);
            break;
    }

    /* 使能加密运算 */
    amhw_hc32_aes_encrypt (gp_hw_aes);

    /* 启动加密运算 */
    amhw_hc32_aes_start (gp_hw_aes);

    /* 等待加密运算结束 */
    while (amhw_hc32_aes_stu_check (gp_hw_aes)){

        am_kprintf ("Encrypt ... !\r\n");

        /* 延时10us，如果超时则加密失败 */
        am_udelay (10);
        if (amhw_hc32_aes_stu_check (gp_hw_aes)){

            return ;
        }
    }

    /* 读取加密后的密文 */
    amhw_hc32_aes_data_read (gp_hw_aes, p_ency_data);
}

/**
 * \brief AES 解密
 */
static void aes_decy (uint8_t key_len,
                     uint32_t *p_ency_data,
                     uint32_t *p_key_data,
                     uint32_t *p_decy_data)
{
    /* 保存需要解密的明文数据 */
    amhw_hc32_aes_data_write (gp_hw_aes, p_ency_data);

    /* 加载密钥  */
    switch (key_len){

        case AM_HC32_AES_KEY_LEN_128_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_128_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_128_BIT,
                                      p_key_data);
            break;
        case AM_HC32_AES_KEY_LEN_192_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_192_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_192_BIT,
                                      p_key_data);
            break;
        case AM_HC32_AES_KEY_LEN_256_BIT:
            /* 设置密钥长度 */
            amhw_hc32_aes_key_len_sel (gp_hw_aes,
                                         AMHW_HC32_AES_KEY_LEN_256_BIT);

            amhw_hc32_aes_key_save (gp_hw_aes,
                                      AMHW_HC32_AES_KEY_LEN_256_BIT,
                                      p_key_data);
            break;
    }

    /* 使能解密运算 */
    amhw_hc32_aes_decrypt (gp_hw_aes);

    /* 启动解密运算 */
    amhw_hc32_aes_start (gp_hw_aes);

    /* 等待解密运算结束 */
    while (amhw_hc32_aes_stu_check (gp_hw_aes)){

        am_kprintf ("Encrypt ... !\r\n");

        /* 延时10us，如果超时则解密失败 */
        am_udelay (10);
        if (amhw_hc32_aes_stu_check (gp_hw_aes)){

            return ;
        }
    }

    /* 读取解密后的明文 */
    amhw_hc32_aes_data_read (gp_hw_aes, p_decy_data);
}

/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 */
void demo_hc32_hw_aes_ency_decy_entry (void     *p_hw_aes,
                                       uint8_t   key_len,
                                       uint32_t *p_aes_data,
                                       uint32_t *p_key_data)
{
    int i = 0;

    if (p_hw_aes == NULL)
    {
        return ;
    }

    gp_hw_aes  = (amhw_hc32_aes_t *)p_hw_aes;

    /* 加密 */
    aes_ency (key_len, p_aes_data, p_key_data, ency_data);

    /* 解密 */
    aes_decy (key_len, ency_data, p_key_data, decy_data);

    /* 判断加密解密后的明文与初始明文是否一样 */
    for (i = 0; i < 4; i++){

        if (decy_data[i] != p_aes_data[i]){
            am_kprintf ("Aes ency-decy failed!\r\n");
            return ;
        }

        if (i == 3){
            am_kprintf ("Aes ency-decy success!\r\n");
        }
    }

    while (1){
        ;
    }
}

/* end of file */
