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
 * \brief 闪存控制器例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 本程序会将签名信息通过串口打印到终端上；
 *   2. 显示分为两部分，分别为软件计算的结果和硬件计算的结果；
 *   3. 软件计算结果和硬件结算结果应该相同；
 *   4. LED 以 0.5s 的时间间隔闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD。
 *
 * \note
 *   可访问的地址范围为 FLASH 空间(0x0000~0x8000)。
 *
 * \par 源代码
 * \snippet demo_lpc845_hw_flash_ctrl.c src_lpc845_hw_flash_ctrl
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-04  sky, modified
 * - 1.00 15-07-16  aii, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_flash_ctrl
 * \copydoc demo_lpc845_hw_flash_ctrl.c
 */

/** [src_lpc845_hw_flash_ctrl] */
#include "ametal.h"
#include "am_board.h"
#include "am_vdebug.h"
#include "lpc84x_periph_map.h"
#include "hw/amhw_lpc_fmc.h"

/**
 * \brief 软件计算 FLASH 签名
 *
 * \param[in] start 签名起始地址
 * \param[in] end   签名结束地址
 *
 * \note 签名结束长度，不可超过 MCU Flash 总空间(0x0000~0x8000)
 */
am_local uint32_t __cal_sign (uint32_t start, uint32_t end)
{
    uint32_t sign = 0, addr = 0, f_q = 0;

    /* 签名生成地址(对应于 AHB 字节地址位 [20:4] 即 16 位对齐) */
    start = start >> 4;
    end   = end >> 4;

    /* 计算 FLASH 签名 */
    for (addr = start; addr <= end; addr++) {

        /* 读取 FLASH 中值存入数组(字对齐) */
        f_q = (uint32_t)(*(volatile int *)(4 * addr));

        sign = f_q ^  (sign >> 1)
                   ^ ((sign << 31) & 0x80000000)
                   ^ ((sign << 21) & 0x80000000)
                   ^ ((sign << 1 ) & 0x80000000)
                   ^ ((sign << 0 ) & 0x80000000);
    }

    /* 返回 32 位签名信息 */
    return sign;
}

void demo_lpc845_hw_flash_ctrl_entry(amhw_lpc_fmc_time_t time)
{

    amhw_lpc_fmc_flashtim_set(LPC84X_FMC, time);

    /* 读取并打印签名信息 */
    AM_DBG_INFO("hard calculate id: 0x%x\r\n",
                amhw_lpc_fmc_flash_sign(LPC84X_FMC, 0x0, 0x7000));
    AM_DBG_INFO("soft calculate id: 0x%x\r\n\r\n", __cal_sign(0x0, 0x7000));

    AM_FOREVER {
        am_led_on(LED0);
        am_mdelay(200);
        am_led_off(LED0);
        am_mdelay(200);
    }
}

/** [src_lpc845_hw_flash_ctrl] */

/* end of file */
