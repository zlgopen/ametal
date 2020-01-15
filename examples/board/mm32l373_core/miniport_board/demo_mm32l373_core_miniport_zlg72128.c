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
 * \brief MiniPort-ZLG72128 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 将 MiniPort-ZLG72128 板子与 AM217BLE 的 MiniPort 相连接。
 *
 * - 实验现象：
 *   1. 按下一个按键，两位数码管显示按键编号的二进制；
 *   2. 偶数编号时，数码管闪烁，奇数编号时，数码管不闪烁。
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *    AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *    已经默认配置为 1， 用户不必再次配置。
 *
 * \par 源代码
 * \snippet demo_miniport_zlg72128.c src_miniport_zlg72128
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-03  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_miniport_zlg72128
 * \copydoc demo_miniport_zlg72128.c
 */

/** [src_miniport_zlg72128] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_mm32l373_inst_init.h"
#include "demo_std_entries.h"
#include "demo_mm32l373_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_mm32l373_core_miniport_zlg72128_entry (void)
{
    AM_DBG_INFO("demo mm32l373_core miniport zlg72128!\r\n");

    am_miniport_zlg72128_inst_init();

    demo_std_key_digitron_rngbuf_entry(0);
}
/** [src_miniport_zlg72128] */

/* end of file */
