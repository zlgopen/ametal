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
 * \brief PMU 掉电模式例程，通过 HW 层接口实现
 *
 * - 实验步骤：
 *   1. 用杜邦线将 PIO0_23 与 GND 连接，把电平拉低。
 *
 * - 实验现象：
 *   1. 运行程序，一秒后 LED0 闪烁一次后进入掉电模式；
 *   2. 当 PIO0_23 与 GND 的连接断开时，CPU 被唤醒，LED0 不断闪烁。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制；
 *    2. 使用该程序后，会导致下次程序烧写的时候检测不到芯片（由于芯片进入掉电
 *       模式将 SWD 调试模式关闭），下次下载时将 P0_12 拉低或按动复位键之后一秒内
 *       即可正常下载调试。
 *
 * \par 源代码
 * \snippet demo_am824_hw_pmu_powerdown.c src_am824_hw_pmu_powerdown
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-01  sky, modified
 * - 1.00 15-07-21  zxl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_pmu_powerdown
 * \copydoc demo_am824_hw_pmu_powerdown.c
 */

/** [src_am824_hw_pmu_powerdown] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_iocon.h"
#include "lpc82x_periph_map.h"
#include "demo_nxp_entries.h"

/**
 * \brief 例程入口
 */
void demo_am824_core_hw_pmu_powerdown_entry (void)
{

    am_kprintf("demo am824_core hw pmu powerdown!\r\n");

    /* 延时一秒，方便下次下载程序 */
    am_mdelay(1000);

    /* 配置 PIO0_23 方向为输入 */
    amhw_lpc82x_gpio_pin_dir_input(LPC82X_GPIO, PIO0_23);

    /* 配置 PIO0_23 上拉 */
    amhw_lpc82x_iocon_mode_set(LPC82X_IOCON,
                               PIO0_23,
                               AMHW_LPC82X_IOCON_MODE_PULLUP);

    /* 配置掉电模式 */
    demo_lpc824_hw_pmu_powerdown_entry(LPC82X_PMU, PIO0_23);


}
/** [src_am824_hw_pmu_powerdown] */

/* end of file */
