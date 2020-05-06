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
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 *
 * - 实验现象：
 *   1. 指定串口输出 "HW example---LPUART test in interrupt mode:"；
 *   2. 指定串口输出接收到的字符。
 *
 * \note
 *   1.若该串口与调试串口一致，需关闭调试串口以免影响效果
 *
 * \par 源代码
 * \snippet demo_hc32_hw_lpuart_int.c src_hc32_hw_lpuart_int
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */

 /**
 * \addtogroup demo_if_hc32_hw_lpuart_int
 * \copydoc demo_hc32_hw_lpuart_int.c
 */

/** [src_hc32_hw_lpuart_int] */
#include "ametal.h"
#include "am_int.h"
#include "hw/amhw_hc32_lpuart.h"
#include "hw/amhw_hc32_rcc.h"

/*******************************************************************************
  全局变量
*******************************************************************************/
static const uint8_t hw_polling_str[] =
                         {"HW example---LPUART test in interrupt mode:\r\n"};

static const uint8_t   *gp_tx_buf   = NULL; /**< \brief 发送缓冲区 */
static volatile uint8_t g_tx_index  = 0;    /**< \brief 当前发送数据索引 */
static volatile uint8_t g_tx_nbytes = 0;    /**< \brief 需要发送的总字节数 */

static uint32_t lpuart_baudrate = 0;

/**
 * \brief 串口发送数据（中断模式下，该函数不会阻塞）
 *
 * \param[in] p_hw_lpuart指向需控制的串口设备结构体
 * \param[in] p_buf    指向发送数据的缓冲区
 * \param[in] len      发送的字节数
 *
 * \retval 1 发送就绪
 * \retval 0 发送模块忙碌，无法发送
 *
 * \note 发送缓冲区最好是全局的，保证在所有数据发送完成前，缓冲区没有被释放掉
 */
static uint8_t lpuart_int_send (amhw_hc32_lpuart_t *p_hw_lpuart,
                                const uint8_t   *p_buf,
                                uint32_t         len)
{
    if (g_tx_index >= g_tx_nbytes)  {   /* 上次发送已经发送完成 */
        gp_tx_buf   = p_buf;            /* 重新定义发送缓冲器区 */
        g_tx_index  = 0;                /* 从缓存区起始字节开始发送 */
        g_tx_nbytes = len;              /* 发送g_tx_nbytes个字节 */

        /* 必须先写一个一个字节的数据才以产生发送中断 */
        while(amhw_hc32_lpuart_flag_check(
                  p_hw_lpuart,
                  AMHW_HC32_LPUART_FLAG_TX_EMPTY) ==
                  AM_FALSE);

        amhw_hc32_lpuart_data_write(p_hw_lpuart, p_buf[g_tx_index++]);

        amhw_hc32_lpuart_int_enable(p_hw_lpuart,
                                      AMHW_HC32_LPUART_FLAG_TX_COMPLETE);

        return 1;
    }

    return 0;
}

/**
 * \brief 串口中断服务函数
 *
 * \param[in] p_arg 指向需控制的串口设备结构体 ，在 am_int_connect() 注册
 */
static void lpuart_hw_irq_handler (void *p_arg)
{
    amhw_hc32_lpuart_t *p_hw_lpuart = (amhw_hc32_lpuart_t *)p_arg;
    uint8_t             data;

    if (amhw_hc32_lpuart_flag_check(p_hw_lpuart,
                                      AMHW_HC32_LPUART_FLAG_RX_COMPLETE) ==
                                      AM_TRUE) {

        amhw_hc32_lpuart_flag_clr(p_hw_lpuart,
                                    AMHW_HC32_LPUART_FLAG_RX_COMPLETE);

        /* 获取新接收数据，并发送出去 */
        data = amhw_hc32_lpuart_data_read(p_hw_lpuart);
        amhw_hc32_lpuart_data_write(p_hw_lpuart, data);

    } else  if (amhw_hc32_lpuart_flag_check(
                    p_hw_lpuart,
                    AMHW_HC32_LPUART_FLAG_TX_COMPLETE) ==
                    AM_TRUE) {

        amhw_hc32_lpuart_flag_clr(p_hw_lpuart,
                                    AMHW_HC32_LPUART_FLAG_TX_COMPLETE);

        /* 发送中断 */
        if(g_tx_index < g_tx_nbytes) {

            /* 缓冲区还有待发送数据 */
            amhw_hc32_lpuart_data_write(p_hw_lpuart, gp_tx_buf[g_tx_index++]);

        } else {

            /* 缓冲区没有发送数据，关闭发送中断 */
            amhw_hc32_lpuart_int_disable(p_hw_lpuart,
                                           AMHW_HC32_LPUART_INT_TX_COMPLETE);
        }
    }
}

/**
 * \brief LPUART hw 中断收发初始化
 */
void lpuart_int_init (amhw_hc32_lpuart_t   *p_hw_lpuart,
                      void                (*pfn_init) (void),
                      uint8_t               clk_src,
                      uint32_t              clk_rate,
                      unsigned char         inum_lpuart)
{
    /* 工作模式设置  */
    amhw_hc32_lpuart_mode_sel(p_hw_lpuart, AMHW_HC32_LPUART_WORK_MODE_1);

    /* 设置通信时钟源 */
    amhw_hc32_lpuart_clk_src_sel(p_hw_lpuart,
                                   (amhw_hc32_lpuart_sclk_src_t)clk_src);

    if(clk_src == AMHW_HC32_LPUART_SCLK_SRC_XTL) {

        /* 外部晶振驱动能力选择 */
        amhw_hc32_rcc_xtl_xtal_driver_set(AMHW_HC32_XTL_XTAL_DRIVER_DEFAULT);

        /* XTL晶振振荡幅度的调整 */
        amhw_hc32_rcc_xtl_xtal_amplitude_set(
            AMHW_HC32_XTL_XTAL_AMPLITUDE_BIG_DEFAULT);

        /* 外部高速时钟XTL稳定时间选择 */
        amhw_hc32_rcc_xtl_waittime_set(AMHW_HC32_XTL_WAITTIME_16384);

        amhw_hc32_rcc_set_start(0x5A5A);
        amhw_hc32_rcc_set_start(0xA5A5);

        amhw_hc32_rcc_xtl_enable();

        /* 等待稳定*/
        while(amhw_hc32_rcc_xtl_state_get() == AM_FALSE);

    } else if(clk_src == AMHW_HC32_LPUART_SCLK_SRC_RCL) {

        /* 设置RCL时钟频率校准值 （38.4KHz）*/
        amhw_hc32_rcc_rcl_trim_set(*((uint16_t*)0x00100C20));

        amhw_hc32_rcc_rcl_waittime_set(AMHW_HC32_RCL_WAITTIME_256);

        amhw_hc32_rcc_set_start(0x5A5A);
        amhw_hc32_rcc_set_start(0xA5A5);

        amhw_hc32_rcc_rcl_enable();

        /* 等待稳定*/
        while(amhw_hc32_rcc_rcl_state_get() == AM_FALSE);
    }

    /* 分频系数设置  */
    amhw_hc32_lpuart_clk_div_sel(p_hw_lpuart,
                                   AMHW_HC32_LPUART_SCLK_DIV_MODE13_4);

    /* 停止位设置 */
    amhw_hc32_lpuart_stop_bit_sel(p_hw_lpuart,
                                    AMHW_HC32_LPUART_STOP_1_0_BIT);

    /* 校验设置 */
    amhw_hc32_lpuart_parity_bit_sel(p_hw_lpuart,
                                      AMHW_HC32_LPUART_PARITY_NO);

    /* 波特率设置 */
    amhw_hc32_lpuart_baudrate_set( p_hw_lpuart, clk_rate, lpuart_baudrate);

    /* 接收完成中断使能 */
    amhw_hc32_lpuart_int_enable(p_hw_lpuart,
                                  AMHW_HC32_LPUART_INT_RX_COMPLETE);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_hc32_lpuart_enable(p_hw_lpuart,AMHW_HC32_LPUART_RX);

    /* 关联中断向量号，开启中断 */
    am_int_connect(inum_lpuart, lpuart_hw_irq_handler, (void *)p_hw_lpuart);
    am_int_enable(inum_lpuart);
    if(pfn_init != NULL){
        pfn_init();
    }
}

/**
 * \brief LPUART 中断发送例程，通过 HW 层接口实现
 */
void demo_hc32_hw_lpuart_int_entry (void             *p_hw_lpuart,
                                    void            (*pfn_init) (void),
                                    uint8_t           sclk_src,
                                    uint32_t          clk_rate,
                                    uint32_t          baudrate,
                                    unsigned char     inum_lpuart)
{
    amhw_hc32_lpuart_t *p_lpuart = (amhw_hc32_lpuart_t *)p_hw_lpuart;

    lpuart_baudrate = baudrate;

    if (pfn_init == NULL) {
        return;
    }

    lpuart_int_init(p_lpuart,
                    pfn_init,
                    sclk_src,
                    clk_rate,
                    inum_lpuart);

    lpuart_int_send(p_lpuart, hw_polling_str, sizeof(hw_polling_str));

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_hc32_hw_lpuart_int] */

/* end of file */
