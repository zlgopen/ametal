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
 * \brief FTFA例程,通过驱动层的接口实现
 *
 * - 操作步骤：
 *      - 串口接调试串口
 *
 * - 实验现象：
 *      - 擦除出错：串口打印 “erase error!”
 *      - 写入出错：串口打印 "program error!"
 *      - 写入成功：串口打印 55扇区的 (1024/4) 个32bit十六进制数据
 *
 * \par 源代码
 * \snippet demo_fsl_dr_ftfa.c src_fsl_dr_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-19  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_dr_ftfa
 * \copydoc demo_fsl_dr_ftfa.c
 */

/** [src_fsl_dr_ftfa] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include "am_fsl_ftfa.h"
#include "am_int.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

void demo_fsl_dr_ftfa_entry (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    int i;
    uint32_t data[1024 / 4]; /** 要写入flash的数据 */
    uint32_t status;         /** flash命令执行状态 */
    uint32_t key = 0;

    /** 数据初始化 */
    for (i = 0; i < 1024 / 4; i++) {
        data[i] = i;
    }

    key = am_int_cpu_lock();
    /** FTFA初始化 */
    am_fsl_ftfa_init();
    am_int_cpu_unlock (key);

    key = am_int_cpu_lock();
    /** 擦除扇区 55 */
    status = am_fsl_ftfa_sector_erase(p_hw_ftfa, 55 * 1024);
    am_int_cpu_unlock (key);

    /** 扇区擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");

        while (1) {
            ;
        }
    }

    key = am_int_cpu_lock();
    /** 向扇区 55中写入数据 */
    status = am_fsl_ftfa_flash_program(p_hw_ftfa,
                                       55 * 1024,
                                       data,
                                       1024 / 4);
    am_int_cpu_unlock (key);

    /** 扇区写入出错，程序停在此处 */
    if ((1024/4) != status) {
        AM_DBG_INFO("program error!\r\n");

        while (1);
    }

    /** 写入成功后将数据从flash中读出 */
    for (i = 0; i < 1024; i += 4) {
        AM_DBG_INFO("%8x ", *(uint32_t *)(1024 * 55 + i));
    }

    AM_DBG_INFO("flash test finished!\r\n");
    /** 死循环，防止重启 */
    while (1) {
        ;
    }
}

/** [src_fsl_dr_ftfa] */

/* end of file */
