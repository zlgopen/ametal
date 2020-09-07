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
 * \brief aes 系统复位例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *
 *   AES对数据加密然后解密，如果解密出来数据与加密前数据一样，则串口打印“Aes ency-decy success!\r\n”。
 *   反之打印"Aes ency-decy failed!\r\n"。
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *    2. 如果调试串口使用与本例程相同，则不应在后续继续使用调试信息输出函数
 *      （如：AM_DBG_INFO()）。
 *
 * \par 源代码
 * \snippet demo_hc32f460_drv_aes_ency_decy.c src_hc32f460_drv_aes_ency_decy
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-26  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_drv_aes_ency_decy
 * \copydoc demo_hc32f460_drv_aes_ency_decy.c
 */

/** [src_hc32f460_drv_aes_ency_decy] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_board.h"
#include "am_hc32f460_aes.h"
#include "hw/amhw_hc32f460_aes.h"

/*******************************************************************************
  全局变量
*******************************************************************************/

/**< \brief 存放加密后的数据 */
static uint32_t ency_data[4] = {0};

/**< \brief 存放解密后的数据 */
static uint32_t decy_data[4]  = {0};


/**
 * \brief AES 加密解密例程，通过 HW 层接口实现
 */
void demo_hc32f460_drv_aes_ency_decy_entry (am_aes_handle_t  handle,
                                            uint32_t        *p_aes_data,
                                            uint32_t        *p_key_data)
{
	uint32_t i = 0;

    am_hc32f460_aes_encrypt(handle, p_aes_data, p_key_data, ency_data);

    am_hc32f460_aes_decrypt(handle, ency_data, p_key_data, decy_data);

    /* 判断加密解密后的明文与初始明文是否一样 */
    for (i = 0; i < 4; i++){

        if (decy_data[i] != p_aes_data[i]){
            am_kprintf ("Aes ency-decy failed!\r\n");
            return ;
        }

        if (i == 3){
            am_kprintf ("Aes ency-decy success!\r\n");
        }
    }
}

/* end of file */
