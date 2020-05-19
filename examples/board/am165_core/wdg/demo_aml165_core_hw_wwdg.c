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
 * \brief WWDG 例程，通过 HW 层接口实现
 * 
 * - 实验现象：
 *   1. 修改宏定义 __WWDG_FEED_TIME_MS 的值，超过 __WWDG_TIMEOUT_MS，芯片复位；
 *   2. 修改宏定义 __WWDG_FEED_TIME_MS 的值，小于 __WWDG_TIMEOUT_MS，程序正常运行。
 *
 * \par 源代码
 * \snippet demo_aml165_core_hw_wwdg.c src_aml165_core_hw_wwdg
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-26  sdy, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_aml165_core_hw_wwdg
 * \copydoc demo_aml165_core_hw_wwdg.c
 */
 
/** [src_aml165_core_hw_wwdg] */
#include "ametal.h"
#include "am_clk.h"
#include "am_vdebug.h"
#include "am_zml165.h"
#include "demo_zlg_entries.h"
#include "am_zml165_clk.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 看门狗超时时间 
 */
#define __WWDG_TIMEOUT_MS       50

/**
 * \brief 看门狗喂狗时间，若喂狗时间超过WWDG_TIMEOUT_MS的值
 */
#define __WWDG_FEED_TIME_MS     25

/**
 * \brief 例程入口
 */
void demo_aml165_core_hw_wwdg_entry (void)
{
    AM_DBG_INFO("demo aml165_core hw wwdg!\r\n");

    am_clk_enable(CLK_WWDG);
    am_zml165_clk_reset(CLK_WWDG);

    demo_zlg_hw_wwdg_entry(ZML165_WWDG,
                           am_clk_rate_get(CLK_WWDG),
                           __WWDG_TIMEOUT_MS,
                           __WWDG_FEED_TIME_MS);
}
/** [src_aml165_core_hw_wwdg] */

/* end of file */
