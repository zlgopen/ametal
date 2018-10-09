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
 * \brief 妯℃嫙姣旇緝鍣ㄤ緥绋嬶紙涓柇妯″紡锛夛紝閫氳繃 HW 灞傛帴鍙ｅ疄鐜�
 *
 * - 鎿嶄綔姝ラ锛�
 *   1. PIO0_1 杩炴帴妯℃嫙杈撳叆 2锛�
 *   2. PIO0_14 杩炴帴妯℃嫙杈撳叆 3銆�
 *
 * - 瀹為獙鐜拌薄锛�
 *   1. 褰撹緭鍏� 2 涓庤緭鍏� 3 姣旇緝缁撴灉鍙戠敓鍙樺寲鐨勬椂鍊欎細杈撳嚭鎻愮ず瀛楃锛�
 *   2. 杈撳叆 2 澶т簬杈撳叆 3 鏃讹紝涓插彛浼氳緭鍑烘瘮杈冪粨鏋� "result: PIO0_1_ACMP_I2 > PIO0_14_ACMP_I3"锛�
 *   3. 杈撳叆 2 灏忎簬杈撳叆 3 鏃讹紝涓插彛浼氳緭鍑烘瘮杈冪粨鏋� "result: PIO0_1_ACMP_I2 < PIO0_14_ACMP_I3"銆�
 *
 * \note
 *    濡傞渶瑙傚療涓插彛鎵撳嵃鐨勮皟璇曚俊鎭紝闇�瑕佸皢 PIO0_0 寮曡剼杩炴帴 PC 涓插彛鐨� TXD锛�
 *    PIO0_4 寮曡剼杩炴帴 PC 涓插彛鐨� RXD銆�
 *
 * \par 婧愪唬鐮�
 * \snippet demo_am824_hw_acmp_int.c src_am824_hw_acmp_int
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-05  hgo, modified
 * - 1.00 15-07-21  bob, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am824_hw_acmp_int
 * \copydoc demo_am824_hw_acmp_int.c
 */

/** [src_am824_hw_acmp_int] */
#include "ametal.h"
#include "am_int.h"
#include "am_vdebug.h"
#include "hw/amhw_lpc82x_clk.h"
#include "hw/amhw_lpc82x_syscon.h"
#include "demo_nxp_entries.h"

/**
 * \brief ACMP 骞冲彴鍒濆鍖�
 */
am_local void __plfm_acmp_init()
{
    amhw_lpc82x_clk_periph_enable(AMHW_LPC82X_CLK_ACMP);
    amhw_lpc82x_syscon_powerup(AMHW_LPC82X_SYSCON_PD_ACMP);

    /* 浣胯兘绠¤剼  PIO0_1 涓烘瘮杈冨櫒杈撳叆 2 鍔熻兘锛堟瘮杈冨櫒杈撳叆 2 绠¤剼鍥哄畾涓� PIO0_1锛� */
    am_gpio_pin_cfg (PIO0_1, PIO0_1_ACMP_I2);

    /* 浣胯兘绠¤剼  PIO0_14 涓烘瘮杈冨櫒杈撳叆 3 鍔熻兘锛堟瘮杈冨櫒杈撳叆 3 绠¤剼鍥哄畾涓� PIO0_14锛� */
    am_gpio_pin_cfg (PIO0_14, PIO0_14_ACMP_I3);

    /* 浣胯兘绠¤剼 PIO0_18 涓烘瘮杈冨櫒杈撳嚭鍔熻兘 */
    am_gpio_pin_cfg (PIO0_18, PIO_FUNC_ACMP_O);
}


/**
 * \brief 渚嬬▼鍏ュ彛
 */
void demo_am824_core_hw_acmp_int_entry (void)
{
    uint32_t flags = 0;

    am_kprintf("demo am824_core hw acmp int!\r\n");
    /* 骞冲彴鍒濆鍖� */
    __plfm_acmp_init();

    flags = AMHW_LPC82X_ACMP_CTRL_VP_CMP2      | /* 杈撳叆 2 涓哄悓鐩哥杈撳叆 */
            AMHW_LPC82X_ACMP_CTRL_VM_CMP3      | /* 杈撳叆 3 涓哄弽鐩哥杈撳叆 */
            AMHW_LPC82X_ACMP_CTRL_EDGESEL_BOTH | /* 閰嶇疆鍙岃竟娌胯Е鍙戜腑鏂�  */                           
            AMHW_LPC82X_ACMP_CTRL_HYS_20MV;      /* 杩熸粸鐢靛帇涓� 20 mV    */


    demo_lpc824_hw_acmp_int_entry(LPC82X_ACMP, flags, INUM_ACMP, PIO0_18);
}
/** [src_am824_hw_acmp_int] */

/* end of file */
