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
 * \brief ZLG116 用户配置文件
 * \sa am_prj_config.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-07 nwt, first implementation.
 * \endinternal
 */

#include <am_aml166_inst_init.h>
#include "ametal.h"
#include "am_prj_config.h"
#include "am_board.h"
#include "am_service_inst_init.h"
#include "zlg116_periph_map.h"
#include "string.h"

extern int am_main (void);

#define RAM_START_ADDR   0x20000000
#define APP_START_ADDR   0x08003C00
#define G_VECTORS_SIZE   48 * 4

extern int am_main (void);

int main (void)
{

#if (AM_CFG_INT_ENABLE == 1)
    am_zlg116_nvic_inst_init();
#endif /* (AM_CFG_INT_ENABLE == 1) */

#if (AM_CFG_CLK_ENABLE == 1)
    am_zlg116_clk_inst_init();
#endif /* (AM_CFG_CLK_ENABLE == 1) */

#if (AM_CFG_GPIO_ENABLE == 1)
    am_zlg116_gpio_inst_init();
#endif /* (AM_CFG_GPIO_ENABLE == 1) */

#if (AM_CFG_DMA_ENABLE == 1)
    am_zlg116_dma_inst_init();
#endif /* (AM_CFG_DMA_ENABLE == 1) */

#if (AM_CFG_NVRAM_ENABLE == 1)
    am_nvram_inst_init();
#endif /* (AM_CFG_NVRAM_ENABLE == 1) */

    /* 板级初始化 */
    am_board_init();

#if (AM_CFG_MEM_MODE_RAM_ENABLE == 1)
    memcpy((void*)RAM_START_ADDR,
           (void*)APP_START_ADDR,
           G_VECTORS_SIZE);

    ZLG116_SYSCFG->cfgr |= (0x3);
    __enable_irq();
#endif /* (AM_CFG_MEM_MODE_RAM_ENABLE == 1) */

    return am_main();
}

/* end of file */
