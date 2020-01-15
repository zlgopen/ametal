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
 * \brief MM32L373 用户配置文件
 * \sa am_prj_config.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  sss, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_prj_config.h"
#include "am_mm32l373_inst_init.h"
#include "am_board.h"
#include "am_service_inst_init.h"

extern int am_main (void);

int main (void)
{

#if (AM_CFG_CLK_ENABLE == 1)
    am_mm32l373_clk_inst_init();
#endif /* (AM_CFG_CLK_ENABLE == 1) */

#if (AM_CFG_INT_ENABLE == 1)
    am_mm32l373_nvic_inst_init();
#endif /* (AM_CFG_INT_ENABLE == 1) */

#if (AM_CFG_GPIO_ENABLE == 1)
    am_mm32l373_gpio_inst_init();
#endif /* (AM_CFG_GPIO_ENABLE == 1) */

#if (AM_CFG_DMA_ENABLE == 1)
    am_mm32l373_dma_inst_init();
#endif /* (AM_CFG_DMA_ENABLE == 1) */

#if (AM_CFG_NVRAM_ENABLE == 1)
    am_nvram_inst_init();
#endif /* (AM_CFG_NVRAM_ENABLE == 1) */

    /* 板级初始化 */
    am_board_init();

    return am_main();
}

/* end of file */
