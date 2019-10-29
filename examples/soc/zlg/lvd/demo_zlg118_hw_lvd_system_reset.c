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
 * \brief LVD 系统复位例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PIOB_7输入电平低于设置的基准电压。则系统处于复位状态，当再次高于基准电压LED闪烁一次，系统正常工作。
 *   串口连续打印 "Mcu is running!"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_lvd_int.c src_zlg118_hw_lvd_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_lvd_int
 * \copydoc demo_zlg118_hw_lvd_int.c
 */

/** [src_zlg118_hw_lvd_int] */
#include "ametal.h"
#include "am_zlg118.h"
#include "am_int.h"
#include "am_board.h"
#include "am_zlg118_lvd.h"
#include "hw/amhw_zlg118_lvd.h"
#include "hw/amhw_zlg118_rcc_reset.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define LVD_TRI_TYPE AMHW_ZLG118_LVD_HTEN                /**< \brief LVD触发条件 */
#define LVD_DEB_TIME AMHW_ZLG118_LVD_DEB_TIME_7P2_MS     /**< \brief LVD滤波时间 */
#define LVD_VIDS     AMHW_ZLG118_LVD_VIDS_2P5_V          /**< \brief LVD基准电压 */
#define LVD_SRC      ANHW_ZLG118_LVD_SRC_PB07            /**< \brief LVD监测源*/
#define LVD_TRI_ACT  ANHW_ZLG118_LVD_TRI_ACT_SYSTEM_RESET /**< \brief LVD触发动作*/

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zlg118_lvd_t       *gp_hw_lvd   = NULL;  /**< \brief lvd 外设 */

/**
 * \brief LVD初始化
 */
static void lvd_init ()
{
    /* cr寄存器清零 */
    amhw_zlg118_lvd_cr_clr (gp_hw_lvd);

    /* 触发条件使能 */
    amhw_zlg118_lvd_tri_enable (gp_hw_lvd, LVD_TRI_TYPE);

    /* 数字滤波时间选择 */
    amhw_zlg118_lvd_deb_time_sel (gp_hw_lvd, LVD_DEB_TIME);

    /* 使能数字滤波 */
    amhw_zlg118_lvd_deb_time_enable (gp_hw_lvd);

    /* 阈值电压选择 */
    amhw_zlg118_lvd_vids_sel (gp_hw_lvd, LVD_VIDS);

    /* 监测来源选择 */
    amhw_zlg118_lvd_src_sel (gp_hw_lvd, LVD_SRC);

    /* 触发动作配置 */
    amhw_zlg118_lvd_tri_act_sel (gp_hw_lvd, LVD_TRI_ACT);

}

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_lvd_system_reset_entry (void *p_hw_lvd)
{
    if (p_hw_lvd == NULL)
    {
        return ;
    }

    gp_hw_lvd  = (amhw_zlg118_lvd_t *)p_hw_lvd;

    /* 判断复位标志 */
    if (amhw_zlg118_rcc_reset_flag_check (AMHW_ZLG118_RCC_RESET_FLAG_LVD))
        {
        am_led_on(LED0);
        am_mdelay(1000);
        am_led_off(LED0);
        am_mdelay(1000);
        am_led_on(LED0);

        /* 清除复位标志 */
        amhw_zlg118_rcc_reset_flag_clr(AMHW_ZLG118_RCC_RESET_FLAG_LVD);
    }

    /* LVD初始化 */
    lvd_init ();

    /* LVD使能 */
    amhw_zlg118_lvd_enable (p_hw_lvd);

    while (1){
        am_kprintf ("Mcu is running!\r\n");
    }
}

/* end of file */
