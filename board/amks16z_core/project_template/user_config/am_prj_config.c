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
 * \brief KL26工程配置文件
 * \sa am_prj_config.c
 * 
 * \internal
 * \par Modification history
 * - 1.00 16-09-13 nwt, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_prj_config.h"
#include "am_kl26_inst_init.h"
#include "am_softimer.h"
#include "hw/amhw_kl26_sim.h"
#include "am_board.h"
#include "am_service_inst_init.h"

/** \brief am_main外部函数 */
extern int am_main (void);

/** \brief main函数入口 */
int main (void)
{
	extern int is_wakeup_reset(void);
#if 0
    am_timer_handle_t tmp2_tmrhdl;
    am_timer_info_t   tmp2_tmrinfo;
#endif

    /* 关闭COP看门狗 */
#if (AM_CFG_SIM_COP == 0)
    amhw_kl26_sim_cop_cfg(KL26_SIM_COP_MODE_NORMAL,
                          KL26_SIM_COP_CLK_INTERNAL,
                          KL26_SIM_COP_DISABLED);
#endif

    /* 系统时钟初始化 */
#if (AM_CFG_CLK_ENABLE == 1)
    am_kl26_clk_inst_init();
#endif

    /* 中断初始化 */
#if (AM_CFG_INT_ENABLE == 1)
    am_kl26_nvic_inst_init();
#endif

    /* GPIO初始化 */
#if (AM_CFG_GPIO_ENABLE == 1)
    am_kl26_gpio_inst_init();
#endif

#if (AM_CFG_DMA_ENABLE == 1)
    am_kl26_dma_inst_init();
#endif

#if (AM_CFG_NVRAM_ENABLE == 1)
    am_nvram_inst_init();
#endif /* (AM_CFG_NVRAM_ENABLE == 1) */

    /* 板级初始化 */
    am_board_init();
#if 0
    /* ADC 硬件触发时需要开启定时器，默认配置TMP2作为ADC硬件触发源 */
    tmp2_tmrhdl = am_kl26_tpm2_timing_inst_init();
    am_timer_info_get(tmp2_tmrhdl, &tmp2_tmrinfo);

    /* 20Hz硬件触发采样 */
    am_timer_enable(tmp2_tmrhdl, 0, tmp2_tmrinfo.clk_frequency / 50);
#endif

    return am_main();
}

/**
 * @}
 */

/* end of file */
