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
 * \brief 模拟比较器比较电压例程，通过HW层的接口中断方式实现

 * - 操作步骤：
 *   1. PIOA_1引脚连接PC串口的TXD；
 *   2. PIOA_2引脚连接PC串口的RXD；
 *   3. PIOC_6外接可调的电压输入（可用滑动变阻器）。
 * - 实验现象：
 *   当输入电压经过1.65v附近时，LED翻转（类似于过零检测），串口打印信息：“CMP INT！”
 *
 *
 * \par 源代码
 * \snippet demo_fsl_hw_cmp_int.c src_fsl_hw_cmp_int
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-22  mkr, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_fsl_if_hw_cmp_int
 * \copydoc demo_fsl_hw_cmp_int.c
 */

/** [src_fsl_hw_cmp_int] */
#include "hw/amhw_fsl_cmp.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"
/**
 * \brief CMP中断回调函数
 * \param[in] p_arg : 回调函数参数
 */
static void cmp_complete(void *p_arg)
{
    amhw_fsl_cmp_t *p_hw_cmp = (amhw_fsl_cmp_t *)p_arg;

    /* 清除中断标志位         */
    amhw_fsl_cmp_cff_clr(p_hw_cmp);

    am_kprintf("CMP INIT!\r\n");
    am_led_toggle(0);
}

/**
 * \brief  CMP 配置函数
 * \param[in] 无
 * \return    无
 */
static void cmp_init (amhw_fsl_cmp_t *p_hw_cmp,
                      int             inum)
{
    /* 配置6位DAC */
    /* 选择VDD（Vin2）作为DAC参考输入电压Vin */
    amhw_fsl_cmp_dac_vin2_enable(p_hw_cmp);

    /* 配置电压输出为1.65V = (3.3 / 64) * (31 + 1))*/
    amhw_fsl_cmp_dac_vol_set(p_hw_cmp, 31);

    /* 使能6位DAC */
    amhw_fsl_cmp_dac_enable(p_hw_cmp);

    /* 6位DAC输出连接到反向输入端 */
    amhw_fsl_cmp_msel_set(p_hw_cmp, 7);

    /* CMP0_IN0输出连接到同相输入端 */
    amhw_fsl_cmp_psel_set(p_hw_cmp, 0);

    /* 使能比较输出引脚 */
    amhw_fsl_cmp_ope_enable(p_hw_cmp);

    /* 使能比较下降沿中断 */
    amhw_fsl_cmp_ief_enable(p_hw_cmp);

    /* 清除中断标志 */
    amhw_fsl_cmp_cff_clr(p_hw_cmp);

    extern int am_int_connect (int inum, am_pfnvoid_t pfn_isr, void *p_arg);
    extern int am_int_enable (int inum);

    /* 连接中断 */
    am_int_connect(inum, cmp_complete, p_hw_cmp);
    am_int_enable(inum);

    /* 使能CMP模块*/
    amhw_fsl_cmp_enable(p_hw_cmp);
}

/**
 * \brief CMP比较器HW层中断例程
 * \return 无
 */
void demo_fsl_hw_cmp_int_entry (amhw_fsl_cmp_t *p_hw_cmp,
                                int             inum)
{
    am_kprintf("CMP Demo:\r\n");

    /* 初始化CMP模块 */
    cmp_init(p_hw_cmp, inum);

    while (1) {
        am_mdelay(1000);
        am_kprintf("CMP...\r\n");
    }

}

/** [src_fsl_hw_cmp_int] */

/* end of file */
