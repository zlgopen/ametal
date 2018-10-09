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
 * \brief FTFA例程,通过HW层的接口实现
 *
 * - 操作步骤：
 *      - 使用调试串口打印信息
 *      - 写入失败时：打印 “write error!”
 *      - 写入错误时：打印 “data not wrote correct!”
 *      - 成功写入时：打印写入的数据(32bit)和 “flash test finished”
 * - 实验现象：
 *
 *
 * \par 源代码
 * \snippet demo_fsl_hw_ftfa.c src_fsl_hw_ftfa
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_ftfa
 * \copydoc demo_fsl_hw_ftfa.c
 */


/** [src_fsl_hw_ftfa] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_fsl_ftfa.h"
#include "am_int.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

void deme_fsl_hw_ftfa_entry (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    int i = 10;
    uint32_t status;
    uint32_t key = 0;

    key = am_int_cpu_lock();
    /** 将flash命令执行函数拷贝到RAM */
    amhw_fsl_ftfa_func_copy();
    am_int_cpu_unlock (key);

    key = am_int_cpu_lock();
    /** 擦除60扇区  */
    status = amhw_fsl_ftfa_sector_erase(p_hw_ftfa, 60 * 1024);
    am_int_cpu_unlock (key);

    /** 扇区擦除出错， 程序停在此处 */
    if (0 != status) {
        AM_DBG_INFO("erase error!\r\n");

        while (1) {
            ;
        }
    }

    key = am_int_cpu_lock();
    /** 判断60扇区是否全为1，即可编程 */
    status = amhw_fsl_ftfa_1s_read(p_hw_ftfa, 60 * 1024, 1024 / 4, 0x00);
    am_int_cpu_unlock (key);

    if (0 == status) {
        for (i = 0; i < 1024; i += 4) {
            key = am_int_cpu_lock();
            status = amhw_fsl_ftfa_word_program(p_hw_ftfa,
                                                60 * 1024 + i,
                                                i);
            am_int_cpu_unlock (key);

            /** 写入不成功时程序停在此处 */
            if (0 != status) {
                AM_DBG_INFO("write error!\r\n");

                while (1);
            }

            key = am_int_cpu_lock();
            status = amhw_fsl_ftfa_program_check(p_hw_ftfa,
                                                 60 * 1024 + i,
                                                 i,
                                                 0x1);
            am_int_cpu_unlock (key);

            /** 写入不正确时程序停在此处 */
            if (0 != status) {
                AM_DBG_INFO("data not wrote correct!\r\n");

                while (1);
            }
        }
    }


    for (i = 0; i < 1024; i += 4) {
        AM_DBG_INFO("%8x ", *(uint32_t *)(60 * 1024 + i));
    }

    AM_DBG_INFO("flash test finished\r\n");

    /** 死循环，防止MCU重启 */
    while (1) {
        i++;
    }
}

/** [src_fsl_hw_ftfa] */

/* end of file */
