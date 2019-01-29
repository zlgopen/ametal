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
 * \brief 掉电检测例程，通过 HW 层接口实现
 *
 * - 实验步骤：
 *   1. 改变供电电压，使之低于 2.85V。
 *
 * - 实验现象：
 *   1. 若外部引脚引起的复位，串口输出 "Reset by External Reset Pin"；
 *   2. 若掉电检测引起的复位，串口输出 "Reset by BOD"；
 *   3. 正常供电下，LED0 不亮，若供电电压不断降低，低于 2.85V 时，LED0 亮。
 *
 * \note
 *    LED0 需要短接 J9 跳线帽，才能被 PIO0_20 控制。
 *
 * \par 源代码
 * \snippet demo_lpc824_hw_bod.c src_lpc824_hw_bod
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-17  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_lpc824_hw_bod
 * \copydoc demo_lpc824_hw_bod.c
 */

/** [src_lpc824_hw_bod] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc84x_syscon.h"

/**
 * \brief BOD 中断处理函数
 *
 * \param[in] p_arg 中断入口参数
 *
 * \return 无
 */
am_local void __bod_irq_callback (void *p_arg)
{
    am_led_on(LED0);
}

/**
 * \brief 例程入口
 */
void demo_lpc845_hw_bod_entry (void)
{

    /* 若掉电检测引起复位，串口输出提示字符，LED0 亮 */
    if ((amhw_lpc84x_syscon_rst_stat_get() &
         AMHW_LPC84X_SYSCON_RSTSTAT_BOD) != 0) {

        amhw_lpc84x_syscon_rst_stat_clr(AMHW_LPC84X_SYSCON_RSTSTAT_BOD);

        AM_DBG_INFO("Reset by BOD\r\n");

    } else if ((amhw_lpc84x_syscon_rst_stat_get() &
                AMHW_LPC84X_SYSCON_RSTSTAT_EXTRST) != 0) {

        AM_DBG_INFO("Reset by External Reset Pin\r\n");
    }

    amhw_lpc84x_syscon_powerup(AMHW_LPC84X_SYSCON_PD_BOD);

    /* 设定掉电检测中断电压 2.85V，设备复位电压 1.46V */
    amhw_lpc84x_syscon_bod_level_set(AMHW_LPC84X_SYSCON_BODRSTLVL_1_46V,
                                     AMHW_LPC84X_SYSCON_BODINTLVL_2_85V);

    amhw_lpc84x_syscon_bod_rst_enable();

    am_int_connect(INUM_BOD, __bod_irq_callback, (void *)LED0);
    am_int_enable(INUM_BOD);

    AM_FOREVER {
        ; /* VOID */
    }

}
/** [src_lpc824_hw_bod] */

/* end of file */
