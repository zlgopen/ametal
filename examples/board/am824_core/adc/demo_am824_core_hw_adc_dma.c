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
 * \brief ADC 渚嬬▼锛岃浆鎹㈢粨鏋滄暟鎹�氳繃 DMA 浼犺緭锛岄�氳繃 HW 灞傛帴鍙ｅ疄鐜�
 *
 * - 鎿嶄綔姝ラ锛�
 *   1. 鐭帴 J12 璺崇嚎甯斤紝浣垮緱鍙傝�冪數鍘嬩负 2.5V锛�
 *   2. PIO0_7(ADC 閫氶亾 0) 杩炴帴妯℃嫙杈撳叆銆�
 *
 * - 瀹為獙鐜拌薄锛�
 *   1. ADC 杞崲瀹屾垚 100 娆★紝涓插彛鎵撳嵃鍑� 100 涓噰鏍风數鍘嬪�笺��
 *
 * \note
 *    濡傞渶瑙傚療涓插彛鎵撳嵃鐨勮皟璇曚俊鎭紝闇�瑕佸皢 PIO0_0 寮曡剼杩炴帴 PC 涓插彛鐨� TXD锛�
 *    PIO0_4 寮曡剼杩炴帴 PC 涓插彛鐨� RXD銆�
 *
 * \par 婧愪唬鐮�
 * \snippet demo_am824_hw_adc_dma.c src_am824_hw_adc_dma
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-24  hgo, modified
 * - 1.00 15-07-23  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_adc_dma
 * \copydoc demo_am824_hw_adc_dma.c
 */

/** [src_am824_hw_adc_dma] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_board.h"
#include "hw/amhw_lpc82x_clk.h"
#include "demo_nxp_entries.h"

/**
 * \brief 渚嬬▼鍏ュ彛
 */
void demo_am824_core_hw_adc_dma_entry (void)
{
    am_kprintf("demo am824_core hw adc dma!\r\n");

    /* 骞冲彴鍒濆鍖� */
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_ADC0);
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_ADC0);

    /* 閰嶇疆閫氶亾锛屼娇鐢ㄩ�氶亾 0锛屽紩鑴氶厤缃负娑堟瀬妯″紡 INACTIVE */
    am_gpio_pin_cfg(PIO0_7, PIO0_7_ADC_0 | PIO0_7_INACTIVE);

    demo_lpc824_hw_adc_dma_entry(LPC82X_ADC0, 0, 2500);
}
/** [src_am824_hw_adc_dma] */

/* end of file */
