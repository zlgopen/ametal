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
 * \brief TRNG 随机数产生例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   TRNG 模块上电产生一次随机数与非上电产生随机数对比，如果不同则TRNG随机数产生正确，打印
 *   “Trng generate tset success!\r\n”,反之打印 “Trng generate tset failed!\r\n”。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_zsn700_hw_trng_dac.c src_zsn700_hw_trng_generate
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-12  ly, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zsn700_hw_trng_generate
 * \copydoc demo_zsn700_hw_trng_generate.c
 */

/** [src_zsn700_hw_trng_generate] */
#include "ametal.h"
#include "am_zsn700.h"
#include "am_int.h"
#include "am_delay.h"
#include "am_board.h"
#include "am_zsn700_trng.h"
#include "hw/amhw_zsn700_trng.h"

/*******************************************************************************
  全局变量
*******************************************************************************/
static amhw_zsn700_trng_t       *gp_hw_trng   = NULL;  /**< \brief trng 外设 */

/**< \brief 上电产生随机数缓存区 */
static uint32_t rng_data1[2] = {0};

/**< \brief 非上电产生随机数缓存区 */
static uint32_t rng_data2[2] = {0};

/**
 * \brief 随机数产生函数
 */
static void rng_generate (uint8_t env_flag)
{

    /* 打开随机源电路 */
    amhw_zsn700_trng_cir_enable (gp_hw_trng);

    if (env_flag == AM_ZSN700_TRNG_POWER_ON){
        /* 选择重新装载初始值 */
        amhw_zsn700_trng_load_set (gp_hw_trng, AM_TRUE);
    }else{
        /* 选择不重新装载初始值 */
        amhw_zsn700_trng_load_set (gp_hw_trng, AM_FALSE);
    }

    /* 选择反馈方式 */
    amhw_zsn700_trng_load_set (gp_hw_trng, AM_TRUE);

    /* 选择移位次数 */
    amhw_zsn700_trng_load_set (gp_hw_trng, AMHW_ZSN700_TRNG_CNT_256);

    /* 随机数生成启动 */
    amhw_zsn700_trng_run (gp_hw_trng);

    /* 等待生成完成 */
    while (amhw_zsn700_trng_stu_check (gp_hw_trng)){

        ;
    }

    if (env_flag == AM_ZSN700_TRNG_POWER_ON){
        /* 选择不重新装载初始值 */
        amhw_zsn700_trng_load_set (gp_hw_trng, AM_FALSE);
    }else{
        ;
    }

    /* 选择反馈方式 */
    amhw_zsn700_trng_load_set (gp_hw_trng, AM_FALSE);

    /* 选择移位次数 */
    amhw_zsn700_trng_load_set (gp_hw_trng, AMHW_ZSN700_TRNG_CNT_64);

    /* 随机数生成启动 */
    amhw_zsn700_trng_run (gp_hw_trng);

    /* 等待生成完成 */
    while (amhw_zsn700_trng_stu_check (gp_hw_trng)){

        ;
    }

    if (env_flag == AM_ZSN700_TRNG_POWER_ON){
        /* 读取64位真随机数 */
        rng_data1[0] = amhw_zl118_trng_low32_data_read (gp_hw_trng);
        rng_data1[1] = amhw_zl118_trng_high32_data_read (gp_hw_trng);
    }else{
        /* 读取64位真随机数 */
        rng_data2[0] = amhw_zl118_trng_low32_data_read (gp_hw_trng);
        rng_data2[1] = amhw_zl118_trng_high32_data_read (gp_hw_trng);
    }

    /* 关闭随机源电路 */
    amhw_zsn700_trng_cir_disable (gp_hw_trng);
}

/**
 * \brief 例程入口
 */
void demo_zsn700_hw_trng_generate_entry (void *p_hw_trng)
{
    gp_hw_trng  = (amhw_zsn700_trng_t *)p_hw_trng;

    /* 第一次上电产生随机数 */
    rng_generate (AM_ZSN700_TRNG_POWER_ON);

    /* 随机数不应该为全0或全1 */
    if ( (rng_data1[0] != 0) &&
         (rng_data1[1] != 0) &&
         (rng_data1[0] != 0xFFFFFFFF) &&
         (rng_data1[1] != 0xFFFFFFFF)){
        ;
    }
    else{
        am_kprintf ("Trng generate tset failed!\r\n");
    }

    /* 非上电产生随机数 */
    rng_generate (AM_ZSN700_TRNG_POWER_NON);

    if ((rng_data1[0] != rng_data2[0]) && (rng_data1[1] != rng_data2[1])){

        am_kprintf ("Trng generate tset success!\r\n");
    }else{

        am_kprintf ("Trng generate tset failed!\r\n");
    }

    AM_FOREVER {
        ;
    }
}

/* end of file */

