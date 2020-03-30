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
 * \brief LVD 中断例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   PIOB_7输入电压从高于基准电压变为低于基准电压，则产生LVD中断。
 *   串口打印 "LVD interrupt"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_lvd_int.c src_zsn700_hw_lvd_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-03-09  yrz, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_lvd_int
 * \copydoc demo_zsn700_hw_lvd_int.c
 */

/** [src_zsn700_hw_lvd_int] */

#include "ametal.h"
#include "am_zsn700.h"
#include "am_int.h"
#include "am_board.h"
#include "am_zsn700_lvd.h"
#include "hw/amhw_zsn700_lvd.h"
#include "hw/amhw_zsn700_rcc_reset.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define LVD_TRI_TYPE AMHW_ZSN700_LVD_RTEN                /**< \brief LVD触发条件 */
#define LVD_DEB_TIME AMHW_ZSN700_LVD_DEB_TIME_7P2_MS     /**< \brief LVD滤波时间 */
#define LVD_VIDS     AMHW_ZSN700_LVD_VIDS_2P5_V          /**< \brief LVD基准电压 */
#define LVD_SRC      ANHW_ZSN700_LVD_SRC_PB07            /**< \brief LVD监测源*/
#define LVD_TRI_ACT  ANHW_ZSN700_LVD_TRI_ACT_NVIC_INT    /**< \brief LVD触发动作*/
#define LVD_INUM     INUM_LVD                            /**< \brief LVD中断号*/

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zsn700_lvd_t       *gp_hw_lvd   = NULL;  /**< \brief lvd 外设 */
static void (*gp_lvd_cb)(void *);                    /**< \brief lvd 回调函数指针*/
/**
 * \brief LVD中断服务函数
 */
static void __lvd_irq_handler (void *p_cookie)
{
    if (amhw_zsn700_int_status_check (gp_hw_lvd)){

        /* 清除中断标志 */
        amhw_zsn700_int_falg_clr (gp_hw_lvd);

        /* 用户回调函数 */
        gp_lvd_cb(NULL);
    }else {
        ;
    }
}

/**
 * \brief LVD初始化
 */
static void lvd_init ()
{
    /* cr寄存器清零 */
    amhw_zsn700_lvd_cr_clr (gp_hw_lvd);

    /* 触发条件使能 */
    amhw_zsn700_lvd_tri_enable (gp_hw_lvd, LVD_TRI_TYPE);

    /* 数字滤波时间选择 */
    amhw_zsn700_lvd_deb_time_sel (gp_hw_lvd, LVD_DEB_TIME);

    /* 使能数字滤波 */
    amhw_zsn700_lvd_deb_time_enable (gp_hw_lvd);

    /* 阈值电压选择 */
    amhw_zsn700_lvd_vids_sel (gp_hw_lvd, LVD_VIDS);

    /* 监测来源选择 */
    amhw_zsn700_lvd_src_sel (gp_hw_lvd, LVD_SRC);

    /* 触发动作配置 */
    amhw_zsn700_lvd_tri_act_sel (gp_hw_lvd, LVD_TRI_ACT);

}

/**
 * \brief LVD中断初始化
 */
static void lvd_int_init (void (*p_lvd_cb) (void *))
{
        /* 中断状态标志清除 */
        amhw_zsn700_int_falg_clr (gp_hw_lvd);

        /* 使能中断 */
        amhw_zsn700_lvd_int_enable (gp_hw_lvd);

        /* 中断连接 */
        am_int_connect(LVD_INUM,
                       __lvd_irq_handler,
                       NULL);

        gp_lvd_cb = p_lvd_cb;

        /* 开启中断 */
        am_int_enable (LVD_INUM);

}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_lvd_int_entry (void *p_hw_lvd,
                                   void (*p_lvd_cb)(void *))
{
    if (p_hw_lvd == NULL)
    {
        return ;
    }

    gp_hw_lvd  = (amhw_zsn700_lvd_t *)p_hw_lvd;

    /* LVD初始化 */
    lvd_init ();

    /* 中断初始化 */
    lvd_int_init (p_lvd_cb);

    /* LVD使能 */
    amhw_zsn700_lvd_enable (p_hw_lvd);

    while (1){
        ;
    }
}

/* end of file */
