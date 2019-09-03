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
 * \brief MiniPort-View + MiniPort-KEY 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 先将 MiniPort-KEY 板子直接与 aml165-Core 的 MiniPort 相连接；
 *   2. 将 MiniPort-View 板子与 MiniPort-KEY 接口相连接。
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
 * \snippet demo_aml165_core_miniport_key_digitron.c src_aml165_core_miniport_key_digitron
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-03  nwt, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_miniport_key_digitron
 * \copydoc demo_aml165_core_miniport_key_digitron.c
 */

/** [src_aml165_core_miniport_key_digitron] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_miniport_key_digitron_entry (void)
{
    AM_DBG_INFO("demo aml165_core miniport key digitron!\r\n");

    am_miniport_view_key_inst_init();

    demo_std_key_digitron_entry(0);
}
/** [src_aml165_core_miniport_key_digitron] */

/* end of file */
