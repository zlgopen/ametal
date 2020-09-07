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
 * \brief MiniPort-KEY 例程，通过标准接口实现
 *
 * - 操作步骤
 *   1. 将 MiniPort-KEY 板子直接与 aml165-Core 的 MiniPort接口相连接。
 *
 * - 实验现象：
 *   1. 按下按键时，调试串口打印当前按键信息。
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_KEY_GPIO_ENABLE、
 *    AM_CFG_KEY_ENABLE 和 AM_CFG_SOFTIMER_ENABLE 定义为 1。但这些宏
 *    已经默认配置为 1， 用户不必再次配置。
 *
 * \par 源代码
 * \snippet demo_aml165_core_miniport_key.c src_aml165_core_miniport_key
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_aml165_core_miniport_key
 * \copydoc demo_aml165_core_miniport_key.c
 */

/** [src_aml165_core_miniport_key] */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_vdebug.h"
#include "demo_std_entries.h"
#include "demo_aml165_core_entries.h"

/**
 * \brief 例程入口
 */
void demo_aml165_core_miniport_key_entry (void)
{
    AM_DBG_INFO("demo aml165_core miniport key!\r\n");

    am_miniport_key_inst_init();

    demo_std_4key_entry();
}
/** [src_aml165_core_miniport_key] */

/* end of file */
