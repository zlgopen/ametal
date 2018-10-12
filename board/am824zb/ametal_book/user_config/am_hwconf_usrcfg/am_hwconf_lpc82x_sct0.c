/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
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
 * - 1.00 15-12-03  hgo, first implementation.
 * \endinternal
 */
#include "am_lpc_sct.h"
#include "ametal.h"
#include "am_lpc82x.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc_sct.h"

/**
 * \addtogroup am_if_src_hwconf_lpc82x_sct0
 * \copydoc am_hwconf_lpc82x_sct0.c
 * @{
 */

/**
 * \brief SCT事件ISR 信息数量
 *        用户可以根据应用中实际所需要用到的事件中断个数，最大可以支持8个
 *        来修改该宏值，以达到不必要的内存浪费的目的。
 */
#define __SCT_EVT_ISRINFO_COUNT    8

/**
 * \brief SCT输出通道数量
 */
#define __SCT_OUTPUT_COUNT    6

/**
 * \brief SCT DAM请求通道数量
 */
#define __SCT_DMA_REQ_COUNT    2

/**
 * \brief 存储用户中断回调信息
 */
static struct am_lpc_sct_evt_isr_info __sct_evt_isr_info[__SCT_EVT_ISRINFO_COUNT];

/**
 * \brief 存储中断编号与其对应在__sct_evt_isr_info中断回调信息中
 *        位置的映射，数组大小一般与SCT所支持的最大事件个数与  __SCT_EVT_ISRINFO_COUNT 相等。
 */
static uint8_t __sct_evt_isr_map[__SCT_EVT_ISRINFO_COUNT];

/** \brief SCT0 平台初始化 */
static void __lpc82x_sct0_plfm_init (void)
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_SCT);
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
}

/** \brief 解除SCT0平台初始化 */
static void __lpc82x_sct0_plfm_deinit (void)
{
    amhw_lpc82x_syscon_periph_reset(AMHW_LPC82X_RESET_SCT);
    amhw_lpc82x_clk_periph_disable(AMHW_LPC82X_CLK_SCT);
}

/** \brief SCT 设备信息 */
static const am_lpc_sct_devinfo_t  __g_sct0_devinfo = {
    LPC82X_SCT0_BASE,               /**< \brief SCT0寄存器块的基地址 */
    INUM_SCT0,                      /**< \brief SCT0中断编号 */
    CLK_SCT,                        /**< \brief SCT0时钟编号 */
    __SCT_EVT_ISRINFO_COUNT,        /**< \brief 总中断数量 */
    __sct_evt_isr_map,              /**< \brief ISR信息映射内存(大小与 evt_isr_cnt一致) */
    __SCT_EVT_ISRINFO_COUNT,        /**< \brief ISR信息数量 */
    __sct_evt_isr_info,             /**< \brief ISR信息内存(大小与 isrinfo_cnt一致) */

    __SCT_OUTPUT_COUNT,             /**< \brief 8个通道输出 */
    __SCT_DMA_REQ_COUNT,            /**< \brief 2个DMA请求通道 */
    __lpc82x_sct0_plfm_init,        /**< \brief 平台初始化函数 */
    __lpc82x_sct0_plfm_deinit,      /**< \brief 平台解初始化函数 */
};

/** \brief SCT设备定义 */
static am_lpc_sct_dev_t  __g_sct0_dev;

/** \brief SCT0 实例初始化，获得SCT标准服务句柄 */
am_lpc_sct_handle_t am_lpc82x_sct0_inst_init (void)
{
    return am_lpc_sct_init(&__g_sct0_dev, &__g_sct0_devinfo);
}

/** \brief SCT0 实例解初始化 */
void am_lpc82x_sct0_inst_deinit (am_lpc_sct_handle_t handle)
{
    am_lpc_sct_deinit(handle);
}
/**
 * @}
 */

/* end of file */
