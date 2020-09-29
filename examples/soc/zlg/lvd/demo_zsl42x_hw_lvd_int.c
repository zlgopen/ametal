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
 * \snippet demo_zsl42x_hw_lvd_int.c src_zsl42x_hw_lvd_int
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-09  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsl42x_hw_lvd_int
 * \copydoc demo_zsl42x_hw_lvd_int.c
 */

/** [src_zsl42x_hw_lvd_int] */

#include "ametal.h"
#include "am_zsl42x.h"
#include "am_int.h"
#include "am_board.h"
#include "am_zsl42x_lvd.h"
#include "hw/amhw_zsl42x_lvd.h"
#include "hw/amhw_zsl42x_rcc_reset.h"

/** \brief LVD触发条件 */
#define __LVD_TRI_TYPE         AMHW_ZSL42x_LVD_RTEN

/** \brief LVD滤波时间 */
#define __LVD_DEB_TIME         AMHW_ZSL42x_LVD_DEB_TIME_7P2_MS

/** \brief LVD触发动作*/
#define __LVD_TRI_ACT          AMHW_ZSL42x_LVD_TRI_ACT_NVIC_INT


/** \brief 用户回调函数*/
static am_pfnvoid_t            __gpfn_app_lvd_cb;
/** \brief 用户回调函数参数*/
static void                   *__gp_arg;


/**
 * \brief LVD中断服务函数
 */
static void __lvd_irq_handler (void *p_arg)
{
    amhw_zsl42x_lvd_t *p_hw_base = (amhw_zsl42x_lvd_t *)p_arg;

    if (amhw_zsl42x_int_status_check (p_hw_base) == 0){
        return;
    }

    /* 清除中断标志 */
    amhw_zsl42x_int_falg_clr (p_hw_base);

    /* 用户回调函数 */
    if (__gpfn_app_lvd_cb) {
        __gpfn_app_lvd_cb(__gp_arg);
    }
}

/**
 * \brief LVD 初始化
 */
static void __lvd_hw_init (amhw_zsl42x_lvd_t       *p_hw_base,
                           amhw_zsl42x_lvd_vids_t   lvd_vids,
                           uint32_t               lvd_src)
{
    /* CR 寄存器清零 */
    amhw_zsl42x_lvd_cr_clr(p_hw_base);

    /* 触发条件使能 */
    amhw_zsl42x_lvd_tri_enable(p_hw_base, __LVD_TRI_TYPE);

    /* 数字滤波时间选择 */
    amhw_zsl42x_lvd_deb_time_sel(p_hw_base, __LVD_DEB_TIME);

    /* 使能数字滤波 */
    amhw_zsl42x_lvd_deb_time_enable(p_hw_base);

    /* 阈值电压选择 */
    amhw_zsl42x_lvd_vids_sel(p_hw_base, lvd_vids);

    /* 监测来源选择 */
    amhw_zsl42x_lvd_src_sel(p_hw_base, lvd_src);

    /* 触发动作配置 */
    amhw_zsl42x_lvd_tri_act_sel(p_hw_base, __LVD_TRI_ACT);
}

/**
 * \brief LVD中断初始化
 */
static void __lvd_int_init (amhw_zsl42x_lvd_t  *p_hw_base)
{
    /* 中断状态标志清除 */
    amhw_zsl42x_int_falg_clr (p_hw_base);

    /* 使能中断 */
    amhw_zsl42x_lvd_int_enable (p_hw_base);

    /* 中断连接 */
    am_int_connect(INUM_LVD, __lvd_irq_handler, p_hw_base);

    /* 开启中断 */
    am_int_enable (INUM_LVD);
}

/**
 * \brief LVD系统复位例程，hw接口层实现
 */
void demo_zsl42x_hw_lvd_int_entry (amhw_zsl42x_lvd_t      *p_hw_base,
                                 amhw_zsl42x_lvd_vids_t  lvd_vids,
                                 uint32_t              lvd_src,
                                 am_pfnvoid_t          pfn_lvd_cb,
                                 void                 *p_arg)
{
    if (p_hw_base == NULL) {
        return;
    }

    /* 保存用户回调 */
    __gpfn_app_lvd_cb = pfn_lvd_cb;
    __gp_arg          = p_arg;

    /* LVD 初始化 */
    __lvd_hw_init(p_hw_base, lvd_vids, lvd_src);

    /* 中断初始化 */
    __lvd_int_init(p_hw_base);

    /* LVD 使能 */
    amhw_zsl42x_lvd_enable (p_hw_base);

    AM_FOREVER {
        ;
    }
}

/* end of file */
