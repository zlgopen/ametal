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
 * \brief CRC 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口打印 "CRC-16 result is     : 0xbb3d\r\n"；
 *   2. 串口打印 "CRC-CCITT result is  : 0x29b1\r\n"；
 *   3. 串口打印 "CRC-32 result is     : 0xcbf43926\r\n"。
 *
 * \par 源代码
 * \snippet demo_std_crc.c src_std_crc
 *
 * \internal
 * \par Modification History
 * - 1.00 15-07-09  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_crc
 * \copydoc demo_std_crc.c
 */

/** [src_std_crc] */
#include "ametal.h"
#include "am_crc.h"
#include "am_vdebug.h"

/** \brief CRC 测试数据 */
am_local am_const uint8_t __g_data_test[] =
                              {'1', '2', '3', '4', '5', '6', '7', '8'};

/** \brief CRC 测试数据的长度 */
am_local uint32_t __g_length_test = AM_NELEMENTS(__g_data_test);

/**
 * \brief 例程入口
 */
void demo_std_crc_entry (am_crc_handle_t crc_handle)
{
    uint32_t         crc_result = 0;
    am_crc_pattern_t crc_pattern;   /* 定义 CRC 模型 */

    /* 定义 CRC-16 模型 */
    crc_pattern.width     = 16;      /* CRC 宽度(如：CRC5 的宽度为 5) */
    crc_pattern.poly      = 0x8005;  /* CRC 生成多项式 */
    crc_pattern.initvalue = 0x0000;  /* CRC 初始值 */
    crc_pattern.refin     = AM_TRUE; /* 如果输入数据需要位反转，则该值为 TRUE */
    crc_pattern.refout    = AM_TRUE; /* 如果输出结果值需要位反转，则该值为 TRUE */
    crc_pattern.xorout    = 0x0000;  /* 输出异或值 */

    /* 初始化 CRC 为 CRC-16 模型 */
    if (am_crc_init(crc_handle, &crc_pattern) != AM_OK) {
        AM_DBG_INFO("The crc-16 init failed\r\n");
    } else {
        am_crc_cal(crc_handle, __g_data_test, __g_length_test);
        am_crc_final(crc_handle, &crc_result);

        /* 计算结果应为：0xbb3d */
        AM_DBG_INFO("CRC-16 result is        : 0x%x \r\n", crc_result);
    }

    /* 改变模型为 CRC-CCITT 模型 */
    crc_pattern.width     = 16;
    crc_pattern.poly      = 0x1021;
    crc_pattern.initvalue = 0xFFFF;
    crc_pattern.refin     = AM_FALSE;
    crc_pattern.refout    = AM_FALSE;
    crc_pattern.xorout    = 0x0000;

    /* 初始化 CRC 为 CRC-CCITT 模型 */
    if (am_crc_init(crc_handle, &crc_pattern) != AM_OK) {
        AM_DBG_INFO("The crc-ccitt init failed\r\n");
    } else {
        am_crc_cal(crc_handle, __g_data_test, __g_length_test);
        am_crc_final(crc_handle, &crc_result);

        /* 计算结果应为：0x29b1 */
        AM_DBG_INFO("CRC-CCITT result is     : 0x%x \r\n", crc_result);
    }

    /* 改变模型为 CRC-32 模型 */
    crc_pattern.width     = 32;
    crc_pattern.poly      = 0x04C11DB7;
    crc_pattern.initvalue = 0xFFFFFFFF;
    crc_pattern.refin     = AM_TRUE;
    crc_pattern.refout    = AM_TRUE;
    crc_pattern.xorout    = 0xFFFFFFFF;

    /* 初始化 CRC 为 CRC-32 模型 */
    if (am_crc_init(crc_handle, &crc_pattern) != AM_OK) {
        AM_DBG_INFO("The crc-32 init failed\r\n");
    } else {
        am_crc_cal(crc_handle, __g_data_test, __g_length_test);
        am_crc_final(crc_handle, &crc_result);

        /* 计算结果应为：0xcbf43926 */
        AM_DBG_INFO("CRC-32 result is        : 0x%x \r\n", crc_result);
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_crc] */

/* end of file */
