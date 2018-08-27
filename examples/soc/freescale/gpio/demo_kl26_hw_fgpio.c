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
 * \brief FGPIO 硬件层引脚输入输出例子
 *
 * - 操作：
 *   1.将J14的key和PIOC_0短接在一起;
 *   2.将J9的LED0和PIOC_4短接在一起;
 *   3.将J10的LED1和PIOC_8短接在一起;
 *
 * -实现现象：程序下载到demo板后，按下一次按键LED0会进行一次翻转，若长按，demo板上的LED0会闪。
 *
 * \par 源代码
 * \snippet demo_kl26_hw_fgpio.c src_kl26_hw_fgpio
 *
 * \internal
 * \par Modification History
 * - 1.01 16-09-18  nwt, make some changes.
 * - 1.00 15-10-16  wxj, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_kl26_if_hw_fgpio
 * \copydoc demo_kl26_hw_fgpio.c
 */
 
/** [src_kl26_hw_fgpio] */
#include "ametal.h"
#include "am_led.h"
#include "am_gpio.h"
#include "hw/amhw_kl26_gpio.h"
#include "hw/amhw_kl26_pinctrl.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
#include "../../../../soc/freescale/kl26/am_kl26.h"

/**
 * \brief 例程入口
 */
void demo_kl26_hw_fgpio_entry ()
{
    int pin_val=0;

    /* 设置 PORTC 0,4号管脚为GPIO功能 */
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_4, 0x1);
    amhw_kl26_port_pin_func_cfg(KL26_PORT, PIOC_0, 0x1);

    /* PORTC 0管脚上/下拉使能 */
    amhw_kl26_port_pin_pull_enable(KL26_PORT, PIOC_0);

    /* PORTC 0引脚输入上拉 */
    amhw_kl26_port_pin_pull_up(KL26_PORT, PIOC_0);

    /* 设置GPIOC 4号管脚初始电平 */
    amhw_kl26_fgpio_pin_init_out_low(KL26_FGPIO, PIOC_4);

    /* 设置FGPIOC 4号管脚为输出方向 */
    amhw_kl26_fgpio_pin_dir_output(KL26_FGPIO, PIOC_4);

    /* 设置FGPIOC 0号管脚为输入方向 */
    amhw_kl26_fgpio_pin_dir_input(KL26_FGPIO, PIOC_0);

    while (1) {

        /* 获取引脚输入电平 */
        pin_val = amhw_kl26_fgpio_pin_input_get(KL26_FGPIO, PIOC_0);

        if (pin_val == 0) {

            /* 翻转引脚输出电平 */
            amhw_kl26_fgpio_pin_out_toggle (KL26_FGPIO, PIOC_4);

            am_mdelay(200);
        }
    }
}

/** [src_kl26_hw_fgpio] */
 
/* end of file */
