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
 * \brief USART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---USART test in interrupt mode:"；
 *   2. 指定串口输出接收到的字符。
 *
 * \note
 *   1.若该串口与调试串口一致，需关闭调试串口以免影响效果
 *
 * \par 源代码
 * \snippet demo_hk32f103rbt6_hw_usart_int.c src_hk32f103rbt6_hw_usart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 17-01-20  ari, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hk32f103rbt6_hw_usart_int
 * \copydoc demo_hk32f103rbt6_hw_usart_int.c
 */

/** [src_hk32f103rbt6_hw_usart_int] */
#include "amhw_hk32f103rbt6_usart.h"
#include "ametal.h"
#include "am_int.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define USART_BAUDRATE      (115200)           /**< \brief 波特率 */

/*******************************************************************************
  全局变量
*******************************************************************************/

static const uint8_t hw_polling_str[] =
                              {"HW example---USART test in interrupt mode:\r\n"};

static const uint8_t   *gp_tx_buf   = NULL; /**< \brief 发送缓冲区 */
static volatile uint8_t g_tx_index  = 0;    /**< \brief 当前发送数据索引 */
static volatile uint8_t g_tx_nbytes = 0;    /**< \brief 需要发送的总字节数 */

/**
 * \brief 串口发送数据（中断模式下，该函数不会阻塞）
 *
 * \param[in] p_hw_usart指向需控制的串口设备结构体
 * \param[in] p_buf    指向发送数据的缓冲区
 * \param[in] len      发送的字节数
 *
 * \retval 1 发送就绪
 * \retval 0 发送模块忙碌，无法发送
 *
 * \note 发送缓冲区最好是全局的，保证在所有数据发送完成前，缓冲区没有被释放掉
 */
static uint8_t usart_int_send (amhw_hk32f103rbt6_usart_t *p_hw_usart,
                               const uint8_t       *p_buf,
                               uint32_t             len)
{
    if (g_tx_index >= g_tx_nbytes)  {   /* 上次发送已经发送完成 */
        gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区 */
        g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节 */

        /* 必须先写一个一个字节的数据才以产生发送中断 */
        while(amhw_hk32f103rbt6_usart_status_flag_check(p_hw_usart,
        									      AMHW_HK32F103RBT6_USART_TX_COMPLETE_FLAG)
                                                  == AM_FALSE);

        amhw_hk32f103rbt6_usart_data_write(p_hw_usart, p_buf[g_tx_index++]);

        amhw_hk32f103rbt6_usart_int_enable(p_hw_usart,AMHW_HK32F103RBT6_USART_INT_TX_EMPTY_ENABLE);

        return 1;
    }

    return 0;
}

/**
 * \brief 串口中断服务函数
 *
 * \param[in] p_arg 指向需控制的串口设备结构体 ，在 am_int_connect() 注册
 */
static void usart_hw_irq_handler (void *p_arg)
{
    amhw_hk32f103rbt6_usart_t *p_hw_usart = (amhw_hk32f103rbt6_usart_t *)p_arg;
    uint8_t          data;

    if (amhw_hk32f103rbt6_usart_status_flag_check(
            p_hw_usart, AMHW_HK32F103RBT6_USART_RX_NOT_EMPTY_FLAG) == AM_TRUE) {
        amhw_hk32f103rbt6_usart_status_flag_clr(p_hw_usart, AMHW_HK32F103RBT6_USART_RX_NOT_EMPTY_FLAG);

        /* 获取新接收数据，并发送出去 */

        data = amhw_hk32f103rbt6_usart_data_read(p_hw_usart);
        amhw_hk32f103rbt6_usart_data_write(p_hw_usart, data);

    } else if (amhw_hk32f103rbt6_usart_status_flag_check(p_hw_usart,
    		                                AMHW_HK32F103RBT6_USART_TX_EMPTY_FLAG) == AM_TRUE) {
        amhw_hk32f103rbt6_usart_status_flag_clr(p_hw_usart,
        		                   AMHW_HK32F103RBT6_USART_TX_EMPTY_FLAG);
        /* 发送中断 */
        if(g_tx_index < g_tx_nbytes) {

            /* 缓冲区还有待发送数据 */
            amhw_hk32f103rbt6_usart_data_write(p_hw_usart, gp_tx_buf[g_tx_index++]);

        } else {

            /* 缓冲区没有发送数据，关闭发送中断 */
        	amhw_hk32f103rbt6_usart_int_disable(p_hw_usart, AMHW_HK32F103RBT6_USART_INT_TX_EMPTY_ENABLE);
        }
    }
}

/**
 * \brief USART hw 中断收发初始化
 */
void usart_int_init (amhw_hk32f103rbt6_usart_t *p_hw_usart,
                     void (* pfn_init)(void),
                     uint32_t         clk_rate,
                     unsigned long    usart_base,
                     unsigned char    inum_usart)
{
    uint8_t inum = 0;

    amhw_hk32f103rbt6_usart_stop_bit_sel(p_hw_usart, AMHW_HK32F103RBT6_USART_STOP_10_BIT);
    amhw_hk32f103rbt6_usart_word_length_sel(p_hw_usart, AMHW_HK32F103RBT6_USART_DATA_8BIT);
    amhw_hk32f103rbt6_usart_parity_bit_sel(p_hw_usart,  AMHW_HK32F103RBT6_USART_PARITY_NO);

    /* 设置串口波特率 */
    amhw_hk32f103rbt6_usart_baudrate_set(p_hw_usart, clk_rate, USART_BAUDRATE);

    /* 关闭所有串口中断 */
    amhw_hk32f103rbt6_usart_int_disable(p_hw_usart, AMHW_HK32F103RBT6_USART_INT_ALL_ENABLE_MASK);

    /* 计算出不同串口对应的中断向量号 */
    if ((uint32_t)p_hw_usart - usart_base) {
        inum = inum_usart + 1;
    } else {
        inum = inum_usart;
    }

    /* 使能串口 */
    amhw_hk32f103rbt6_usart_receiver_enable(p_hw_usart);
    amhw_hk32f103rbt6_usart_transmitter_enable(p_hw_usart);
    amhw_hk32f103rbt6_usart_enable(p_hw_usart);

    /* 使能接收准中断 */
    amhw_hk32f103rbt6_usart_int_enable(p_hw_usart,AMHW_HK32F103RBT6_USART_INT_RX_NOT_EMPTY_ENABLE);

    /* 关联中断向量号，开启中断 */
    am_int_connect(inum, usart_hw_irq_handler, (void *)p_hw_usart);
    am_int_enable(inum);
    if(pfn_init != NULL){
        pfn_init();
    }
}

/**
 * \brief 例程入口
 */
void demo_hk32f103rbt6_hw_usart_int_entry (amhw_hk32f103rbt6_usart_t *p_hw_usart,
                                     void (* pfn_init)(void),
                                     uint32_t             clk_rate,
                                     unsigned long        usart_base,
                                     unsigned char        inum_usart)
{
    if (pfn_init == NULL) {
        return;
    }
    usart_int_init(p_hw_usart, pfn_init, clk_rate , usart_base, inum_usart);

    usart_int_send(p_hw_usart, hw_polling_str, sizeof(hw_polling_str));

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hk32f103rbt6_hw_usart_int] */

/* end of file */
