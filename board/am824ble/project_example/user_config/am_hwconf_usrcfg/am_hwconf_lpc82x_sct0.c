/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief LPC82X SCT 用户配置文件
 * \sa am_hwconf_lpc82x_sct0.c
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-03  hgo, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_lpc82x.h"
#include "am_lpc_sct.h"
#include "hw/amhw_lpc_sct.h"
#include "hw/amhw_lpc82x_clk.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_sct0
 * \copydoc am_hwconf_lpc82x_sct0.c
 * @{
 */

/**
 * \brief SCT 事件 ISR 信息数量
 *
 * 用户可以根据应用中实际所需要用到的事件中断个数，最大可以支持 8 个，通过修改该宏
 * 值，以达到不浪费内存的目的
 */
#define __SCT_EVT_ISRINFO_COUNT    8

/** \brief SCT 输出通道数量 */
#define __SCT_OUTPUT_COUNT    6

/** \brief SCT DAM 请求通道数量 */
#define __SCT_DMA_REQ_COUNT    2

/** \brief 存储用户中断回调信息 */
am_local
struct am_lpc_sct_evt_isr_info __sct_evt_isr_info[__SCT_EVT_ISRINFO_COUNT];

/**
 * \brief 存储中断号与其对应在 __sct_evt_isr_info 中断回调信息中位置的映射，数组
 *        大小一般与 SCT 所支持的最大事件个数与 __SCT_EVT_ISRINFO_COUNT 相等。
 */
am_local uint8_t __sct_evt_isr_map[__SCT_EVT_ISRINFO_COUNT];

/**
 * \brief SCT0 平台初始化
 */
am_local void __lpc82x_sct0_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
}

/**
 * \brief SCT0 平台解初始化
 */
am_local void __lpc82x_sct0_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SCT);
}

/** \brief SCT 设备信息 */
am_local am_const am_lpc_sct_devinfo_t __g_lpc82x_sct0_devinfo = {
    LPC82X_SCT0_BASE,             /* SCT0 寄存器块基地址 */
    INUM_SCT0,                    /* SCT0 中断号 */
    CLK_SCT,                      /* SCT0 时钟号 */
    __SCT_EVT_ISRINFO_COUNT,      /* 总中断数量 */
    __sct_evt_isr_map,            /* ISR 信息映射内存(大小与 evt_isr_cnt 一致) */
    __SCT_EVT_ISRINFO_COUNT,      /* ISR 信息数量 */
    __sct_evt_isr_info,           /* ISR 信息内存(大小与 isrinfo_cnt 一致) */

    __SCT_OUTPUT_COUNT,           /* 8 个通道输出 */
    __SCT_DMA_REQ_COUNT,          /* 2 个 DMA 请求通道 */
    __lpc82x_sct0_plfm_init,      /* 平台初始化函数 */
    __lpc82x_sct0_plfm_deinit,    /* 平台解初始化函数 */
};

/** \brief SCT设备实例 */
am_local am_lpc_sct_dev_t __g_lpc82x_sct0_dev;

/**
 * \brief SCT0 实例初始化
 */
am_lpc_sct_handle_t am_lpc82x_sct0_inst_init (void)
{
    return am_lpc_sct_init(&__g_lpc82x_sct0_dev, &__g_lpc82x_sct0_devinfo);
}

/**
 * \brief SCT0 实例解初始化
 */
void am_lpc82x_sct0_inst_deinit (am_lpc_sct_handle_t handle)
{
    am_lpc_sct_deinit(handle);
}

/**
 * @}
 */

/* end of file */
