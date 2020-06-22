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
 * \brief CRC驱动实现
 *
 * 只能支持2字节对齐的序列计算
 * 即： 序列的字节数 % 2 == 0
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-18
 * \endinternal
 */

#include "ametal.h"
#include "am_hc32f460_crc.h"
#include "hw/amhw_hc32f460_crc.h"

/*******************************************************************************
* CRC驱动函数声明
*******************************************************************************/

/** \biref 根据CRC模型初始化CRC，为CRC计算做好准备 */
static int __crc_init (void *p_cookie, am_crc_pattern_t *p_pattern);

/** \brief CRC计算 */
static int __crc_cal (void *p_cookie,const uint8_t *p_data, uint32_t nbytes);

/** \brief 获取CRC计算结果 */
static int __crc_final (void *p_cookie, uint32_t *p_value);


/******************************************************************************/

/**
 * \brief CRC服务函数
 */
static const struct am_crc_drv_funcs __g_crc_drvfuncs = {
    __crc_init,
    __crc_cal,
    __crc_final
};

/**
 * \brief 初始化CRC
 */

static int __crc_init (void *p_cookie, am_crc_pattern_t *p_pattern)
{
    am_hc32f460_crc_dev_t *p_dev      = (am_hc32f460_crc_dev_t *)p_cookie;
    amhw_hc32f460_crc_t   *p_hw_crc   = NULL;

    if (p_dev == NULL || p_pattern == NULL) {
       return -AM_EINVAL;
    }

    if ((p_pattern->poly  != 0x04C11DB7 || p_pattern->width != 32 ||
         p_pattern->initvalue != 0xFFFFFFFF) &&
        (p_pattern->poly  != 0x1021     || p_pattern->width != 16 ||
         p_pattern->initvalue != 0xFFFF)) {

        return -AM_ENOTSUP;
    }

    p_dev->p_pattern = p_pattern;

    p_hw_crc = (amhw_hc32f460_crc_t *)(p_dev->p_devinfo->crc_reg_base);

    /* 输入数据字节内是否按位颠倒顺序 */
    amhw_hc32f460_crc_refin_sel(p_hw_crc, p_dev->p_pattern->refin);

    /* 输出数据是否按位颠倒顺序 */
    amhw_hc32f460_crc_refout_sel(p_hw_crc, p_dev->p_pattern->refout);

    amhw_hc32f460_crc_xorout_sel(p_hw_crc, AM_FALSE);

    /* 根据模型选择校验方式（crc-16 或  crc-32） */
    if (p_dev->p_pattern->width == 32){

        amhw_hc32f460_crc_bits_sel(p_hw_crc, AMHW_HC32F460_CRC_BITS_32);

        /* 初始化结果寄存器值 */
        amhw_hc32f460_crc_res_32data_init(p_hw_crc, p_pattern->initvalue);

    }else{

        amhw_hc32f460_crc_bits_sel(p_hw_crc, AMHW_HC32F460_CRC_BITS_16);

        /* 初始化结果寄存器值 */
        amhw_hc32f460_crc_res_16data_init(p_hw_crc, p_pattern->initvalue);
    }

    return AM_OK;
}

/**
 * \brief CRC计算
 */
static int __crc_cal (void *p_cookie, const uint8_t *p_data, uint32_t nbytes)
{
    uint32_t i;
    uint32_t tdata;
    uint16_t data_16;
    am_hc32f460_crc_dev_t *p_dev    = (am_hc32f460_crc_dev_t *)p_cookie;
    amhw_hc32f460_crc_t   *p_hw_crc =
        (amhw_hc32f460_crc_t *)(p_dev->p_devinfo->crc_reg_base);

    if (p_dev->p_pattern == NULL || p_data == NULL) {
        return -AM_EINVAL;
    }

    if (nbytes % 2 != 0) {
        return -AM_ENOTSUP;
    }

    if (p_dev->p_pattern->width == 32) {
        for (i = 0; i < nbytes; i+=4) {

           tdata = (p_data[i + 3] << 24) | (p_data[i+2] << 16) |
                   (p_data[i+1] << 8)    | p_data[i];

            amhw_hc32f460_crc_32bit_write_data(p_hw_crc, tdata);
        }
    } else if (p_dev->p_pattern->width == 16) {
        for (i = 0; i < nbytes; i+=2) {

            data_16 = ( p_data[i + 1] << 8 ) | p_data[i] ;

            amhw_hc32f460_crc_16bit_write_data(p_hw_crc, data_16);
        }
    }

    return AM_OK;
}

/**
 * \brief 获取CRC计算结果
 */
static int __crc_final (void *p_cookie, uint32_t *p_value)
{
    am_hc32f460_crc_dev_t *p_dev    = (am_hc32f460_crc_dev_t *)p_cookie;
    amhw_hc32f460_crc_t   *p_hw_crc =
        (amhw_hc32f460_crc_t   *)(p_dev->p_devinfo->crc_reg_base);

    if (p_dev->p_pattern == NULL) {
        return -AM_EINVAL;
    }

    if (p_dev->p_pattern->width == 32){

        *p_value = amhw_hc32f460_crc_32bit_read_data(p_hw_crc);

    }else {

        *p_value = amhw_hc32f460_crc_16bit_read_data(p_hw_crc);
    }

    /* 计算结果与xorout异或 */
    *p_value ^= p_dev->p_pattern->xorout;

    p_dev->p_pattern = NULL;

    return AM_OK;
}

/**
 * \brief CRC初始化
 */
am_crc_handle_t am_hc32f460_crc_init (am_hc32f460_crc_dev_t           *p_dev,
                                      const am_hc32f460_crc_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo         = p_devinfo;
    p_dev->p_pattern         = NULL;

    p_dev->crc_serve.p_funcs = (struct am_crc_drv_funcs *)&__g_crc_drvfuncs;
    p_dev->crc_serve.p_drv   = p_dev;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    return &(p_dev->crc_serve);
}

/**
 * \brief CRC解初始化
 */
void am_hc32f460_crc_deinit (am_crc_handle_t handle)
{
    am_hc32f460_crc_dev_t *p_dev    = (am_hc32f460_crc_dev_t *)handle;

    if (NULL == handle) {
        return ;
    }

    p_dev->p_pattern = NULL;

    p_dev->crc_serve.p_funcs = NULL;
    p_dev->crc_serve.p_drv   = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
