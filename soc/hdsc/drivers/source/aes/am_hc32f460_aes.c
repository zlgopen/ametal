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
 * \brief AES驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-22
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_delay.h"
#include "am_hc32f460_aes.h"
#include "hw/amhw_hc32f460_aes.h"

/**
 * \brief AES 加密
 */
uint32_t am_hc32f460_aes_encrypt (am_aes_handle_t handle,
                                  uint32_t       *p_aes_data,
                                  uint32_t       *p_key_data,
                                  uint32_t       *p_ency_data)
{
    amhw_hc32f460_aes_t *p_hw_aes;

    if (handle == NULL) {
        return -AM_ERROR;
    }

    p_hw_aes = (amhw_hc32f460_aes_t *)(handle->p_devinfo->reg_base);

    /* 保存需要加密的明文数据 */
    amhw_hc32f460_aes_data_write (p_hw_aes, p_aes_data);

    /* 加载密钥  */
    amhw_hc32f460_aes_key_save (p_hw_aes, p_key_data);

    /* 使能加密运算 */
    amhw_hc32f460_aes_encrypt (p_hw_aes);

    /* 启动加密运算 */
    amhw_hc32f460_aes_start (p_hw_aes);

    /* 等待加密运算结束 */
    while (amhw_hc32f460_aes_stu_check (p_hw_aes)){

        am_kprintf ("Encrypt ... !\r\n");

        /* 延时10us，如果超时则加密失败 */
        am_udelay (10);
        if (amhw_hc32f460_aes_stu_check (p_hw_aes)){

            return -AM_ERROR;
        }
    }

    /* 读取加密后的密文 */
    amhw_hc32f460_aes_data_read (p_hw_aes, p_ency_data);

    return AM_OK;
}

/**
 * \brief AES 解密
 */
uint32_t am_hc32f460_aes_decrypt (am_aes_handle_t handle,
                                  uint32_t       *p_aes_data,
                                  uint32_t       *p_key_data,
                                  uint32_t       *p_decy_data)
{
    amhw_hc32f460_aes_t *p_hw_aes;

    if (handle == NULL) {
        return -AM_ERROR;
    }

    p_hw_aes = (amhw_hc32f460_aes_t *)(handle->p_devinfo->reg_base);

    /* 保存需要解密的密文数据 */
    amhw_hc32f460_aes_data_write (p_hw_aes, p_aes_data);

    /* 加载密钥  */
    amhw_hc32f460_aes_key_save (p_hw_aes, p_key_data);

    /* 使能加密运算 */
    amhw_hc32f460_aes_decrypt (p_hw_aes);

    /* 启动解密运算 */
    amhw_hc32f460_aes_start (p_hw_aes);

    /* 等待解密运算结束 */
    while (amhw_hc32f460_aes_stu_check (p_hw_aes)){

        am_kprintf ("Decrypt ... !\r\n");

        /* 延时10us，如果超时则解密失败 */
        am_udelay (10);
        if (amhw_hc32f460_aes_stu_check (p_hw_aes)){

            return -AM_ERROR;
        }
    }

    /* 读取解密后的密文 */
    amhw_hc32f460_aes_data_read (p_hw_aes, p_decy_data);

    return AM_OK;
}

/**
 * \brief AES 初始化
 */
am_aes_handle_t am_hc32f460_aes_init(am_hc32f460_aes_dev_t           *p_dev,
                                     const am_hc32f460_aes_devinfo_t *p_devinfo)
{
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return p_dev;
}

/**
 * \brief AES 去初始化
 */
void am_hc32f460_aes_deinit (am_aes_handle_t handle)
{
    am_hc32f460_aes_dev_t *p_dev  = (am_hc32f460_aes_dev_t *)handle;

    if (handle == NULL){

        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
