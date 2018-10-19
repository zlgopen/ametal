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
 * \brief CRC(Cyclic Redundancy Check)循环冗余校验 软件计算方法实现
 *
 * \internal
 * \par modification history:
 * - 1.00 18-01-19  tee, first implementation
 * \endinternal
 */

/*******************************************************************************
  includes
*******************************************************************************/

#include "ametal.h"
#include "am_crc.h"
#include "am_crc_soft.h"

/*******************************************************************************
    Local functions
*******************************************************************************/

/* 8位数字bit反序(内部使用) */
static uint8_t __reflect8(uint8_t data)
{
    uint8_t value = 0;
    uint8_t i = 1;

    // 交换 bit0 和 bit7，bit1 和 bit6，类推
    for(; i < 9; i++)
    {
        if(data & 1) {
            value |= 1 << (8 - i);
        }
        data >>= 1;
    }
    return value;
}

/******************************************************************************/

/* n位数字bit反序(内部使用)  */
static uint32_t __reflectn(uint32_t data,uint8_t n)
{
    uint32_t value = 0;
    uint8_t i = 1;

    // 交换 bit0 和 bit7，bit1 和 bit6，类推
    for(; i < (n + 1); i++)
    {
        if(data & 1) {
            value |= 1 << (n - i);
        }
        data >>= 1;
    }
    return value;
}

/******************************************************************************/

/* CRC计算 8位 */
static int __crc_soft_cal_8 (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;

    uint32_t crc      = p_crc->value;
    uint8_t *p_table  = (uint8_t *)(p_crc->p_table->p_data);

    while (nbytes) {
        crc = p_table[crc ^ *p_data++];
        nbytes--;
    }

    p_crc->value = crc;

    return AM_OK;
}

/******************************************************************************/

/* CRC计算 16位 */
static int __crc_soft_cal_16 (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;

    uint8_t  width    = p_crc->p_pattern->width;
    uint32_t bit_mask = ((1ul << width) - 1);
    uint32_t crc      = p_crc->value;

    uint16_t *p_table = (uint16_t *)(p_crc->p_table->p_data);

    while (nbytes) {
        crc = (crc << 8) ^ p_table[(crc >> (width - 8)) ^ (*p_data++)];
        crc &= bit_mask;
        nbytes--;
    }

    p_crc->value = crc;

    return AM_OK;
}

/******************************************************************************/

/* CRC计算 16位, REF */
static int __crc_soft_cal_16_ref (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;

    uint32_t  crc     = p_crc->value;
    uint16_t *p_table = (uint16_t *)(p_crc->p_table->p_data);

    while (nbytes) {
        crc = (crc >> 8) ^ p_table[((crc) ^ (*p_data++)) & 0xff];
        nbytes--;
    }

    p_crc->value = crc;

    return AM_OK;
}

/******************************************************************************/

/* CRC计算 32位 */
static int __crc_soft_cal_32 (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;

    uint8_t  width    = p_crc->p_pattern->width;
    uint32_t bit_mask = ((((1ul << (width - 1)) - 1) << 1) | 1);
    uint32_t crc      = p_crc->value;
    uint32_t *p_table = (uint32_t *)(p_crc->p_table->p_data);

    while (nbytes) {
        crc = (crc << 8) ^ p_table[(crc >> (width - 8)) ^ (*p_data++)];
        crc &= bit_mask;
        nbytes--;
    }

    p_crc->value = crc;

    return AM_OK;
}

/******************************************************************************/

/* CRC计算 32位  REF */
static int __crc_soft_cal_32_ref (void *p_drv, const uint8_t *p_data, uint32_t nbytes)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;

    uint32_t  crc     = p_crc->value;
    uint32_t *p_table = (uint32_t *)(p_crc->p_table->p_data);

    while (nbytes) {
        crc = (crc >> 8) ^ p_table[((crc) ^ (*p_data++)) & 0xff];
        nbytes--;
    }

    p_crc->value = crc;

    return AM_OK;
}

/******************************************************************************/
static int __crc_soft_init (void *p_drv, am_crc_pattern_t *p_pattern)
{
    am_crc_soft_t *p_crc = (am_crc_soft_t  *)p_drv;

    if (p_pattern == NULL) {
        return -AM_EINVAL;
    }

    /* 模型与 TABLE 表信息必须保持一致  */
    if ((p_pattern->width != p_crc->p_table->width) ||
        (p_pattern->poly  != p_crc->p_table->poly)  ||
        (p_pattern->refin != p_crc->p_table->refin)) {

        return -AM_ENOTSUP;
    }

    /*
     * 初始值,位数不足8位时，应该向高位对齐 (ref = false )
     *                      应该向低位对齐(ref = true  )
     */
    if ((p_pattern->refin == AM_FALSE) && (p_pattern->width < 8)) {

        p_crc->value = (p_pattern->initvalue << (8 - p_pattern->width)) ;

    } else {
        p_crc->value = p_pattern->initvalue;
    }

    p_crc->p_pattern = p_pattern;

    return AM_OK;
}

/******************************************************************************/
static int __crc_soft_final (void *p_drv, uint32_t *p_value)
{
    am_crc_soft_t    *p_crc     = (am_crc_soft_t  *)p_drv;
    am_crc_pattern_t *p_pattern = p_crc->p_pattern;

    uint32_t crc = p_crc->value;

    if (p_pattern == NULL) {   /* 未设定计算的CRC模型，无法计算 */
        return -AM_EPERM;
    }

    /*
     * 小于8位时,并且没有反向时，计算过程中，其向高位对齐
     * 需要移位，保持最终结果按低位对齐
     */
    if ((p_pattern->refin == AM_FALSE) && (p_pattern->width < 8)) {
        crc >>= (8 - p_pattern->width);
    }

    /* 通常，不会出现该情况 */
    if (p_pattern->refin != p_pattern->refout) {
        crc = __reflectn(crc,p_pattern->width);    /* 小于8位也是按低位对齐的 */
    }

    crc = crc ^ p_pattern->xorout;                 /* 异或输出值  */

    *p_value = crc;

    /* 计算结束， CRC 模型重置为NULL，只有重新初始化后方能正常使用 */
    p_crc->p_pattern = NULL;

    return AM_OK;
}

/******************************************************************************/
static const struct am_crc_drv_funcs  __g_crc_soft_drv_funcs_8 = {
    __crc_soft_init,
    __crc_soft_cal_8,
    __crc_soft_final
};

/******************************************************************************/
static const struct am_crc_drv_funcs  __g_crc_soft_drv_funcs_16 = {
    __crc_soft_init,
    __crc_soft_cal_16,
    __crc_soft_final
};

/******************************************************************************/
static const struct am_crc_drv_funcs  __g_crc_soft_drv_funcs_16_ref = {
    __crc_soft_init,
    __crc_soft_cal_16_ref,
    __crc_soft_final
};

/******************************************************************************/
static const struct am_crc_drv_funcs  __g_crc_soft_drv_funcs_32 = {
    __crc_soft_init,
    __crc_soft_cal_32,
    __crc_soft_final
};

/******************************************************************************/
static const struct am_crc_drv_funcs  __g_crc_soft_drv_funcs_32_ref = {
    __crc_soft_init,
    __crc_soft_cal_32_ref,
    __crc_soft_final
};

/*******************************************************************************
  Public functions
*******************************************************************************/
int am_crc_table_create (am_crc_table_t  *p_table,
                         uint8_t          width,
                         uint32_t         poly,
                         am_bool_t        refin,
                         void            *p_data)
{
    uint32_t temp,i,j;
    uint32_t data;
    uint32_t topbit;

    if ((p_table == NULL) || (p_data == NULL)) {
        return -AM_EINVAL;
    }

    if (width > 32 || width == 0) {
        return -AM_EINVAL;                                    /* 非法的宽度     */
    }

    p_table->width = width;
    p_table->poly  = poly;
    p_table->refin = refin;

    /* CRC 宽度大于8时  */
    if (width >= 8) {

        topbit = (1ul << (width - 1));

        for (i = 0; i < 256; i++) {

            temp = i;

            if (refin == AM_TRUE) {
                temp = __reflect8(i);
            }

            data = temp << (width - 8);

            for (j = 0; j < 8; j++) {

                if (data & topbit) {
                    data = (data << 1) ^ poly;
                } else {
                    data <<= 1;
                }
            }

            if (refin == AM_TRUE) {
                data = __reflectn(data,width);
            }

            /* 避免32位时移位出界 */
            data = data & ((((1ul << (width - 1)) - 1) << 1) | 1);

            /* 将数据放入数组中 */
            if (width <= 8) {
                ((uint8_t *)p_data)[i] = data;
            } else if (width <= 16){
                ((uint16_t *)p_data)[i] = data;
            } else if (width <= 32){
                ((uint32_t *)p_data)[i] = data;
            }
        }

    } else {                               /* CRC宽度小于8时,需要特殊处理  */

        for (i = 0; i < 256; i++) {

            temp = i;
            if (refin == AM_TRUE) {
                temp = __reflect8(i);
            }
            data = temp << (width);       /* 这里不减去8 */

            for (j = 0; j < 8; j++) {

                topbit = 1ul << (8 + width -1 - j);

                if (data & topbit) {                  /* 此位为1，将其异或为0             */
                    data &= ~topbit;                  /* 该位可不清零，后面只取width位即可  */
                    data = data ^ (poly << (7 - j));  /* 保证对齐                                 */
                }
            }
            if (refin == AM_TRUE) {
                data = __reflectn(data,width);
            }
            data = data & ((1ul << width) - 1);

            if (refin == AM_FALSE) {
                data <<= (8 - width);     /* 向高位对齐，补齐为8位 ，低位补0 */
            }
            //为TRUE时，向低位补齐，默认...
            ((uint8_t *)p_data)[i] = data;
        }
    }

    p_table->p_data = p_data;

    return AM_OK;
}

/******************************************************************************/
am_crc_handle_t am_crc_soft_init (am_crc_soft_t         *p_crc,
                                  const am_crc_table_t  *p_table)
{
    const struct am_crc_drv_funcs *p_funcs;

    if ((p_crc   == NULL)         ||
        (p_table == NULL)         ||
        (p_table->p_data == NULL) ||
        (p_table->width > 32)     ||
        (p_table->width < 1)) {

        return NULL;
    }

    p_crc->p_table   = p_table;
    p_crc->p_pattern = NULL;
    p_crc->value     = 0;

    if (p_table->width <= 8) {
        p_funcs = &__g_crc_soft_drv_funcs_8;
    } else if (p_table->width <= 16) {

        if (p_table->refin) {
            p_funcs = &__g_crc_soft_drv_funcs_16_ref;
        } else {
            p_funcs = &__g_crc_soft_drv_funcs_16;
        }
    } else {
        if (p_table->refin) {
            p_funcs = &__g_crc_soft_drv_funcs_32_ref;
        } else {
            p_funcs = &__g_crc_soft_drv_funcs_32;
        }
    }

    p_crc->serv.p_drv   = (void *)p_crc;
    p_crc->serv.p_funcs = (struct am_crc_drv_funcs *)p_funcs;

    return &(p_crc->serv);
}

/* end of file */
