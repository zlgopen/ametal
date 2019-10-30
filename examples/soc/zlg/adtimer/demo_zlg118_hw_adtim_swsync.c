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
 * \brief 高级定时器软件软件同步开始、同步停止、同步清零例程，通过 HW层接口实现
 *
 * - 实验现象：
 *   1.串口打印四行数据信息；
 *   2.第1行代表（state）参与软件同步的定时器状态   1：参与同步     0：不参与软件同步
 *     第2行开始（start）时每个定时器的计数值（此行打印数据应该是等差，定时器已启动，
 *                                  分别读取寄存器值的过程中依旧在计数）
 *     第3行停止（stop）时每个定时器的计数值
 *     第4行清零（clear）时每个定时器的计数值
 *
 * \par 源代码
 * \snippet demo_zlg118_hw_adtim_swsync.c src_zlg118_hw_adtim_swsync
 *
 * \internal
 * \par Modification history
 * - 1.00 19-10-10  zp, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg118_hw_adtim_swsync
 * \copydoc demo_zlg118_hw_adtim_swsync.c
 */

/** [src_zlg118_hw_adtim_swsync] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_clk.h"
#include "am_zlg118.h"
#include "hw/amhw_zlg118_adtim.h"

/**
 * \brief 例程入口
 */
void demo_zlg118_hw_adtim_swsync_entry (void *p_tim_cfg)
{
    amhw_zlg118_adtim_basecnt_cfg_t basecnt;
    amhw_zlg118_adtim_sw_sync_t    *p_cfg = (amhw_zlg118_adtim_sw_sync_t *)
                                            p_tim_cfg;

    amhw_zlg118_adtim_t *p_hw_adtim4 = (amhw_zlg118_adtim_t *)ZLG118_TIM4_BASE;
    amhw_zlg118_adtim_t *p_hw_adtim5 = (amhw_zlg118_adtim_t *)ZLG118_TIM5_BASE;
    amhw_zlg118_adtim_t *p_hw_adtim6 = (amhw_zlg118_adtim_t *)ZLG118_TIM6_BASE;

    uint16_t tim4_data = 0;
    uint16_t tim5_data = 0;
    uint16_t tim6_data = 0;

    am_clk_enable(CLK_TIM456);

    /* 设置定时器模式、计数方向、计数时钟分频系数 */
    basecnt.cntmode   = AMHW_ZLG118_ADTIM_CNT_MODE_SAWTOOTH;
    basecnt.cntdir    = AMHW_ZLG118_ADTIM_CNT_DIR_UP;
    basecnt.cntclkdiv = AMHW_ZLG118_ADTIM_PCLK_DIV1;

    if(p_cfg->adtim4 == AM_TRUE) {
        amhw_zlg118_adtim_init(p_hw_adtim4, &basecnt);
    }

    if(p_cfg->adtim5 == AM_TRUE) {
        amhw_zlg118_adtim_init(p_hw_adtim5, &basecnt);
    }

    if(p_cfg->adtim6 == AM_TRUE) {
        amhw_zlg118_adtim_init(p_hw_adtim6, &basecnt);
    }

    while (1) {

        AM_DBG_INFO("state----tim4:%5d, tim5:%5d, tim6:%5d\r\n",
                     p_cfg->adtim4,
                     p_cfg->adtim5,
                     p_cfg->adtim6);

        /* 软件同步启动 */
        amhw_zlg118_adtim_swsyncstart(p_cfg);

        /* 分别获取定时器的计数值  */
        tim4_data = amhw_zlg118_adtim_getcount(p_hw_adtim4);
        tim5_data = amhw_zlg118_adtim_getcount(p_hw_adtim5);
        tim6_data = amhw_zlg118_adtim_getcount(p_hw_adtim6);

        AM_DBG_INFO("start----tim4:%5d, tim5:%5d, tim6:%5d\r\n",
                    tim4_data,
                    tim5_data,
                    tim6_data);

        am_mdelay(1000);

        /* 软件同步停止 */
        amhw_zlg118_adtim_swsyncstop(p_cfg);

        /* 分别获取定时器的计数值  */
        tim4_data = amhw_zlg118_adtim_getcount(p_hw_adtim4);
        tim5_data = amhw_zlg118_adtim_getcount(p_hw_adtim5);
        tim6_data = amhw_zlg118_adtim_getcount(p_hw_adtim6);

        AM_DBG_INFO("stop-----tim4:%5d, tim5:%5d, tim6:%5d\r\n",
                    tim4_data,
                    tim5_data,
                    tim6_data);

        /* 软件同步清零 */
        amhw_zlg118_adtim_swsyncclear(p_cfg);

        /* 分别获取定时器的计数值  */
        tim4_data = amhw_zlg118_adtim_getcount(p_hw_adtim4);
        tim5_data = amhw_zlg118_adtim_getcount(p_hw_adtim5);
        tim6_data = amhw_zlg118_adtim_getcount(p_hw_adtim6);

        AM_DBG_INFO("clear----tim4:%5d, tim5:%5d, tim6:%5d\r\n",
                    tim4_data,
                    tim5_data,
                    tim6_data);

        AM_DBG_INFO("\r\n\r\n");
    }
}
/** [src_zlg118_hw_adtim_swsync] */

/* end of file */
