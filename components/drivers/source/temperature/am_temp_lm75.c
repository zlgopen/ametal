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
 * \brief LM75温度传感器驱动
 * 
 * \internal
 * \par Modification history
 * - 1.00 17-06-19  tee, first implementation.
 * \endinternal
 */

#include "am_spi.h"
#include "am_temp_lm75.h"

/*******************************************************************************
  Local functions
*******************************************************************************/
static int __temp_lm75_read (void *p_arg, int32_t *p_temp)
{
    am_temp_lm75_t *p_lm75 = (am_temp_lm75_t *)p_arg;
    int16_t         temp;
    uint8_t         temp_value[2];

    int ret = am_i2c_read(&p_lm75->dev, 0x00, temp_value, 2);

    if (ret < 0) {
        return ret;
    }

    /* 0xE0 = 1110 0000, Only the upper 3-bits valid in fractional part */
    temp_value[1] &= 0xE0;

    /* the temperature is 256 times in actual */
    temp = temp_value[0] << 8 | temp_value[1];

    /*  /256 * 1000 ->  *125 / 32 */
    if (p_temp) {
        *p_temp = temp * 125 / 32;
    }

    return AM_OK;
}

/*******************************************************************************
  Standard driver functions
*******************************************************************************/
static const struct am_temp_drv_funcs __g_temp_lm75_drv_funcs = {
    __temp_lm75_read,
};

/*******************************************************************************
  Public functions
*******************************************************************************/
am_temp_handle_t am_temp_lm75_init (am_temp_lm75_t            *p_lm75,
                                    const am_temp_lm75_info_t *p_info,
                                    am_i2c_handle_t            handle)
{
    if ((p_lm75 == NULL) || (p_info == NULL)) {
        return NULL;
    }

    am_i2c_mkdev(&p_lm75->dev,
                 handle,
                 p_info->i2c_addr,
                 AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);

    p_lm75->p_info       = p_info;
    p_lm75->isa.p_funcs  = &__g_temp_lm75_drv_funcs;
    p_lm75->isa.p_cookie = p_lm75;

    return &p_lm75->isa;
}

/******************************************************************************/
int am_temp_lm75_deinit (am_temp_lm75_t *p_lm75)
{
    if (p_lm75 == NULL) {
        return -AM_EINVAL;
    }

    p_lm75->p_info       = NULL;
    p_lm75->isa.p_funcs  = NULL;
    p_lm75->isa.p_cookie = NULL;

    return AM_OK;
}

/* end of file */
