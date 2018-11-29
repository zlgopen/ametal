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
 * \brief sensor implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 18-11-20  wan, first implementation.
 * \endinternal
 */

#include "am_sensor.h"
#include "am_common.h"

/*******************************************************************************
  local functions
*******************************************************************************/
am_local uint32_t __pow10 (int n)
{
    /* 32位数最大表示： 4 294 967 296， 最多返回：10^9 ，即 ：1 000 000 000 */
    am_local const uint32_t pow10[] = {
        1UL,                  /* 0 */
        10UL,                 /* 1 */
        100UL,                /* 2 */
        1000UL,               /* 3 */
        10000UL,              /* 4 */
        100000UL,             /* 5 */
        1000000UL,            /* 6 */
        10000000UL,           /* 7 */
        100000000UL,          /* 8 */
        1000000000UL,         /* 9 */
    };

    if ((n >= 0) && (n <= 9)) {
        return pow10[n];
    }

    return 0;
}

am_local am_err_t __sensor_val_unit_convert (am_sensor_val_t *p_val, int32_t to_unit)
{
    uint32_t max_val;
    if (p_val->val != 0) {                   /* 值为0，单位设置为目标单位即可 */

        uint32_t mul;

        int unit_err = to_unit - p_val->unit;

        if (unit_err < 0) {                                    /* 单位缩小    */

            unit_err = (-1) * unit_err;

            if (unit_err > 9) {                                /* 超出范围    */

                return -AM_ERANGE;

            } else {

                mul = __pow10(unit_err);                       /* 扩大的倍数  */

                /* 只有当  val 值 不超过 max_val 时，才不会超过范围 */
                max_val = 2147483647 / mul;

                if ((p_val->val > 0) && (p_val->val > max_val)) {
                    return -AM_ERANGE;
                }

                if ((p_val->val < 0) && (p_val->val < -max_val)) {
                    return -AM_ERANGE;
                }

                p_val->val *= mul;                              /* val值扩大  */
            }

        } else if (unit_err > 0) {                              /* 单位扩大   */

            if (unit_err > 9) {                                 /* 超出范围   */
                p_val->val = 0;
            } else {

                mul = __pow10(unit_err);                        /* 缩小的倍数 */

                /* val 值缩小 ，保证四舍五入 */
                p_val->val = (p_val->val + (mul / 2)) / mul;
            }
        }
    }

    p_val->unit = to_unit;

    return AM_OK;
}


/*******************************************************************************
  public functions
*******************************************************************************/

/**
 * \brief 辅助函数，获取一个单位对应的前缀符号，如"M"（兆）、"k"（千）
 */
const char *am_sensor_unit_symbol_get (int32_t unit)
{
    /* 单位从小到大的顺序排列 */
    static const char *symbol[] = {
        "y",  "z", "a", "f", "p", "n", "μ", "m",     /* 间隔 3, offset = 0    */
        "c",  "d",                                   /* 间隔 1, offset = 8    */
        "",                                          /* 0, offset = 10        */
        "da", "h",                                   /* 间隔 1, offset = 11   */
        "k",  "M", "G", "T", "P", "E", "Z", "Y",     /* 间隔 3, offset = 13   */
    };

    /* 当前单位有效范围 */
    if ((unit >= AM_SENSOR_UNIT_YOCTO) && (unit <= AM_SENSOR_UNIT_YOTTA)) {

        /* 厘  ~ 百 */
        if ((unit >= AM_SENSOR_UNIT_CENTI) && (unit <= AM_SENSOR_UNIT_HECTO)) {
            return symbol[10 + unit];
        } else if ((unit % 3) == 0) {
            unit = unit / 3;
            if (unit < 0) {
                return symbol[10 + AM_SENSOR_UNIT_CENTI + unit];
            } else {
                return symbol[10 + AM_SENSOR_UNIT_HECTO + unit];
            }
        }
    }
    return NULL;
}

/**
 * \brief 传感器单位的转换
 */
am_err_t am_sensor_val_unit_convert (am_sensor_val_t *p_buf, int num, int32_t to_unit)
{
    int      i;
    am_err_t ret = AM_OK;
    am_err_t ret_cur;

    if ((p_buf == NULL) || (num <= 0)) {
        return -AM_EINVAL;
    }

    for (i = 0; i < num; i++) {

        if (AM_SENSOR_VAL_IS_VALID(p_buf[i])) {

            ret_cur = __sensor_val_unit_convert(&p_buf[i], to_unit);

            if ((ret == AM_OK) && (ret_cur != AM_OK)) {
                 ret = ret_cur;
            }
        }
    }

    return ret;
}

/* end of file */
