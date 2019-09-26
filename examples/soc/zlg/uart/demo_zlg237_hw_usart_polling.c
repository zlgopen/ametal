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
 * \brief USART 轮询方式例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---USART test in polling mode:"；
 *   2. 指定串口输出接收到的字符串。
 *
 * \par 源代码
 * \snippet demo_zlg237_hw_usart_polling.c src_zlg237_hw_usart_polling
 *
 * \internal
 * \par Modification History
 * - 1.00 17-04-17  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_zlg237_hw_usart_polling
 * \copydoc demo_zlg237_hw_usart_polling.c
 */

/** [src_zlg237_hw_usart_polling] */
#include <amhw_zlg237_usart.h>
#include "ametal.h"
//#include "am_zlg116.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define USART_BAUDRATE     (115200)              /** <\brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/
static const uint8_t usart_hw_str[] =
                                {"HW example---USART test in polling mode:\r\n"};

/**
 * \brief USART HW 查询收发初始化
 */
static void usart_polling_init (amhw_zlg237_usart_t *p_hw_usart, uint32_t clk_rate)
{

    /* 设置串口波特率 */
    amhw_zlg237_usart_baudrate_set(p_hw_usart, clk_rate, USART_BAUDRATE);

    amhw_zlg237_usart_stop_bit_sel(p_hw_usart, AMHW_ZLG237_USART_STOP_10_BIT);
    amhw_zlg237_usart_word_length_sel(p_hw_usart, AMHW_ZLG237_USART_DATA_8BIT);
    amhw_zlg237_usart_parity_bit_sel(p_hw_usart,  AMHW_ZLG237_USART_PARITY_NO);

    /* 使能串口 */
    amhw_zlg237_usart_receiver_enable(p_hw_usart);
    amhw_zlg237_usart_transmitter_enable(p_hw_usart);
    amhw_zlg237_usart_enable(p_hw_usart);
}

/**
 * \brief 例程入口
 */
void demo_zlg237_hw_usart_polling_entry (amhw_zlg237_usart_t *p_hw_usart,
                                         uint32_t             clk_rate)
{

    /* 数据缓冲区 */
    uint8_t buf_src[256];

    /* USART查询初始化 */
    usart_polling_init(p_hw_usart, clk_rate);

    amhw_zlg237_usart_poll_send(p_hw_usart, usart_hw_str, sizeof(usart_hw_str));

    while (1) {

        /* 接收字符 */
        amhw_zlg237_usart_poll_receive(p_hw_usart, buf_src, 1);

        /* 发送刚刚接收的字符 */
        amhw_zlg237_usart_poll_send(p_hw_usart, buf_src, 1);
    }
}
/** [src_zlg237_hw_usart_polling] */

/* end of file */
