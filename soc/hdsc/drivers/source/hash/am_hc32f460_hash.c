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
 * \brief HASH驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-22
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460.h"
#include "am_delay.h"
#include "am_hc32f460_hash.h"
#include "hw/amhw_hc32f460_hash.h"



static uint32_t __hc32f460_hash_data_fill(uint8_t        *p_data_src,
                                          uint32_t        src_data_size,
                                          uint8_t        *group1,
                                          uint8_t        *group2)
{
    uint32_t i = 0;
    uint32_t group_num_fill = 0;
    uint32_t group1_bytes_fill = 0;
    uint32_t group2_bytes_fill = 0;

    /* 原始数据最后一个组的字节数 */
    uint32_t bytes_src = src_data_size % AMHW_HC32F460_HASH_MAX_GROUP_SIZE;

    uint32_t      u32BitLenHi;
    uint32_t      u32BitLenLo;

    u32BitLenHi    = (src_data_size >> 29u) & 0x7u;
    u32BitLenLo    = (src_data_size << 3u);

    if (bytes_src < AMHW_HC32F460_HASH_LAST_GROUP_SIZE) {
        group1_bytes_fill = AMHW_HC32F460_HASH_LAST_GROUP_SIZE - bytes_src;
        group_num_fill = 1;
    } else {
        group1_bytes_fill = AMHW_HC32F460_HASH_MAX_GROUP_SIZE - bytes_src;
        group2_bytes_fill = AMHW_HC32F460_HASH_LAST_GROUP_SIZE;
        group_num_fill =2;
    }


    if (group_num_fill == 1) {
        for(i = 0; i < bytes_src; i++) {
            group1[i] = p_data_src[src_data_size - bytes_src + i];
        }

        if (group1_bytes_fill > 0) {
            group1[bytes_src] = 0x80;
            for (i = 1; i < group1_bytes_fill; i++) {
                group1[bytes_src + i] = 0;
            }
        }

        group1[63] = (uint8_t)(u32BitLenLo);
        group1[62] = (uint8_t)(u32BitLenLo >> 8u);
        group1[61] = (uint8_t)(u32BitLenLo >> 16u);
        group1[60] = (uint8_t)(u32BitLenLo >> 24u);
        group1[59] = (uint8_t)(u32BitLenHi);
        group1[58] = (uint8_t)(u32BitLenHi >> 8u);
        group1[57] = (uint8_t)(u32BitLenHi >> 16u);
        group1[56] = (uint8_t)(u32BitLenHi >> 24u);
    } else if (group_num_fill == 2) {
        for(i = 0; i < bytes_src; i++) {
            group1[i] = p_data_src[src_data_size - bytes_src + i];
        }

        group1[bytes_src] = 0x80;
        for (i = 1; i < group1_bytes_fill ; i++) {
            group1[bytes_src + i] = 0;
        }

        for (i = 0; i < group2_bytes_fill ; i++) {
            group2[i] = 0;
        }

        group2[63] = (uint8_t)(u32BitLenLo);
        group2[62] = (uint8_t)(u32BitLenLo >> 8u);
        group2[61] = (uint8_t)(u32BitLenLo >> 16u);
        group2[60] = (uint8_t)(u32BitLenLo >> 24u);
        group2[59] = (uint8_t)(u32BitLenHi);
        group2[58] = (uint8_t)(u32BitLenHi >> 8u);
        group2[57] = (uint8_t)(u32BitLenHi >> 16u);
        group2[56] = (uint8_t)(u32BitLenHi >> 24u);
    }

    return group_num_fill;
}


/**
 * \brief HASH 计算
 */
uint32_t am_hc32f460_hash_compute (am_hash_handle_t handle,
                                   uint8_t         *p_hash_data,
                                   uint32_t         data_length,
                                   uint8_t         *p_hash_digest)
{
    amhw_hc32f460_hash_t *p_hw_hash;

    if (handle == NULL) {
        return -AM_ERROR;
    }

    /* 填充数据 */
    uint8_t group1[64] = {0};
    uint8_t group2[64] = {0};
    uint32_t group_num;
    uint8_t group_num_to_fill = 0;
    uint32_t i = 0;

    group_num = data_length / 64 + 1;

    p_hw_hash = (amhw_hc32f460_hash_t *)(handle->p_devinfo->reg_base);

    group_num_to_fill = __hc32f460_hash_data_fill(p_hash_data,
                                                  data_length,
                                                  group1,
                                                  group2);

    while(amhw_hc32f460_hash_status_check (p_hw_hash));
    for (i = 0; i < group_num - 1; i++) {
        amhw_hc32f460_hash_data_write(p_hw_hash, p_hash_data + 64 * i);

        if (i == 0) {
            amhw_hc32f460_hash_first_group(p_hw_hash);
        } else {
            amhw_hc32f460_hash_not_first_group(p_hw_hash);
        }
        amhw_hc32f460_hash_start(p_hw_hash);

        while(amhw_hc32f460_hash_status_check (p_hw_hash));
    }

    if (group_num_to_fill == 1) {
        amhw_hc32f460_hash_data_write(p_hw_hash, group1);
        if (group_num == 1) {
            amhw_hc32f460_hash_first_group(p_hw_hash);
        } else {
            amhw_hc32f460_hash_not_first_group(p_hw_hash);
        }

        amhw_hc32f460_hash_start(p_hw_hash);
        while(amhw_hc32f460_hash_status_check (p_hw_hash));

    } else if (group_num_to_fill == 2) {
        if (group_num == 1) {
            amhw_hc32f460_hash_first_group(p_hw_hash);
        } else {
            amhw_hc32f460_hash_not_first_group(p_hw_hash);
        }
        amhw_hc32f460_hash_data_write(p_hw_hash, group1);
        amhw_hc32f460_hash_start(p_hw_hash);
        while(amhw_hc32f460_hash_status_check (p_hw_hash));
        amhw_hc32f460_hash_data_write(p_hw_hash, group2);
        amhw_hc32f460_hash_start(p_hw_hash);
    }


    /* 等待加密运算结束 */
    while (amhw_hc32f460_hash_status_check (p_hw_hash)){

        am_kprintf ("Compute ... !\r\n");

        /* 延时10us，如果超时则计算失败 */
        am_udelay (10);
        if (amhw_hc32f460_hash_status_check (p_hw_hash)){

            return -AM_ERROR;
        }
    }

    /* 读取hash消息摘要 */
    amhw_hc32f460_hash_read (p_hw_hash, p_hash_digest);

    return AM_OK;
}


/**
 * \brief HASH 初始化
 */
am_hash_handle_t am_hc32f460_hash_init(am_hc32f460_hash_dev_t           *p_dev,
                                       const am_hc32f460_hash_devinfo_t *p_devinfo)
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
 * \brief HASH 去初始化
 */
void am_hc32f460_hash_deinit (am_hash_handle_t handle)
{
    am_hc32f460_hash_dev_t *p_dev  = (am_hc32f460_hash_dev_t *)handle;

    if (handle == NULL){

        return ;
    }

    /* 平台去初始化 */
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
