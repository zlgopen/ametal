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
 * \brief HDIV 例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 串口打印除数，被除数及结果
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOA_10 引脚连接 PC 串口的 TXD，
 *       PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_hc32f07x_hw_hdiv.c src_hc32f07x_hw_hdiv
 *
 * \internal
 * \par Modification History
 * - 1.00 15-7-13  sss, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f07x_hw_hdiv
 * \copydoc demo_hc32f07x_hw_hdiv.c
 */

/** [src_hc32f07x_hw_hdiv] */
#include "hw/amhw_hc32f07x_hdiv.h"
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "am_hc32f07x.h"
#include "am_clk.h"
#include "demo_amf07x_core_entries.h"

amhw_hc32f07x_hdiv_unsigned_t hdiv_unsigned;
amhw_hc32f07x_hdiv_signed_t   hdiv_signed;
/**
 * \brief 例程入口
 */
void demo_hc32f07x_core_hw_hdiv_entry (void)
{
    int ret = 0;

    AM_DBG_INFO("demo amf07x_core hw hdiv!\r\n");

    am_clk_enable(CLK_HDIV);

    hdiv_unsigned.undividend = 0x7250a3fb;
    hdiv_unsigned.undivisor  = 0x257d;

    ret = am_hc32f07x_hdiv_unsigned((amhw_hc32f07x_hdiv_t *)HC32F07X_HDIV_BASE,
                                     &hdiv_unsigned);
    AM_DBG_INFO("\r\nthis is unsigned hdiv!\r\n");
    if(ret == AM_OK){
        AM_DBG_INFO("the dividend is  : %d!\r\n", (int)hdiv_unsigned.undividend);
        AM_DBG_INFO("the divisor is   : %d!\r\n", (int)hdiv_unsigned.undivisor);
        AM_DBG_INFO("the qutient is   : %d!\r\n", (int)hdiv_unsigned.quotient );
        AM_DBG_INFO("the remainder is : %d!\r\n", (int)hdiv_unsigned.remainder );
    }else {
        AM_DBG_INFO("the unsigned hdiv failed!\r\n");
    }

    hdiv_signed.dividend = -0x7250a3fb;
    hdiv_signed.divisor  = 0x257d;

    ret = am_hc32f07x_hdiv_signed((amhw_hc32f07x_hdiv_t *)HC32F07X_HDIV_BASE,
                                   &hdiv_signed);
    AM_DBG_INFO("\r\nthis is signed hdiv:\r\n");
    if(ret == AM_OK){
        AM_DBG_INFO("the dividend is  : %d!\r\n", (int)hdiv_signed.dividend);
        AM_DBG_INFO("the divisor is   : %d!\r\n", (int)hdiv_signed.divisor);
        AM_DBG_INFO("the qutient is   : %d!\r\n", (int)hdiv_signed.quotient );
        AM_DBG_INFO("the remainder is : %d!\r\n", (int)hdiv_signed.remainder );
    }else {
        AM_DBG_INFO("the signed hdiv failed!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32f07x_hw_hdiv] */

/* end of file */
