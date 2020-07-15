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
 * \brief LPUART 驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-26  zp, first implementation
 * \endinternal
 */
#include "am_hc32_lpuart.h"
#include "am_clk.h"
#include "hw/amhw_hc32_rcc.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_uart.h"
#include "hc32_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/**
 * \brief 串口模式（查询或中断）设置
 */
int __lpuart_mode_set (am_hc32_lpuart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __lpuart_opt_set (am_hc32_lpuart_dev_t *p_dev, uint32_t opts);

/* HC32 串口驱动函数声明 */
static int __lpuart_ioctl (void *p_drv, int, void *);

static int __lpuart_tx_startup (void *p_drv);

static int __lpuart_callback_set (void *p_drv,
                                int   callback_type,
                                void *pfn_callback,
                                void *p_arg);

static int __lpuart_poll_getchar (void *p_drv, char *p_char);

static int __lpuart_poll_putchar (void *p_drv, char outchar);

static void __lpuart_irq_handler (void *p_arg);

/** \brief 标准层接口函数实现 */
static const struct am_uart_drv_funcs __g_lpuart_drv_funcs = {
    __lpuart_ioctl,
    __lpuart_tx_startup,
    __lpuart_callback_set,
    __lpuart_poll_getchar,
    __lpuart_poll_putchar,
};

/******************************************************************************/

/**
 * \brief 设备控制函数
 *
 * 其中包含设置获取波特率，模式设置（中断/查询），获取支持的模式，硬件选项设置等功能。
 */
static int __lpuart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_hc32_lpuart_dev_t *p_dev       = (am_hc32_lpuart_dev_t *)p_drv;
    amhw_hc32_lpuart_t   *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    int status = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        /* 只有在发送空的基础上才允许修改波特率 */
        while (amhw_hc32_lpuart_flag_check(
                   p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_EMPTY) == AM_FALSE);

        status = amhw_hc32_lpuart_baudrate_set(p_hw_lpuart,
                                               p_dev->clk_rate,
                                               (uint32_t)p_arg);

        if (status > 0) {
            p_dev->baud_rate = status;
            status = AM_OK;
        } else {
            status = -AM_EIO;
        }
        break;

    /* 波特率获取 */
    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->baud_rate;
        break;

    /* 模式设置 */
    case AM_UART_MODE_SET:
        status = (__lpuart_mode_set(p_dev, (int)p_arg) == AM_OK)
                 ? AM_OK : -AM_EIO;
        break;

    /* 模式获取 */
    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->channel_mode;
        break;

    /* 获取串口可设置的模式 */
    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

    /* 串口选项设置 */
    case AM_UART_OPTS_SET:
        status = (__lpuart_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

    /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->options;
        break;

    /* 485模式设置 */
    case AM_UART_RS485_SET:
        if (p_dev->rs485_en != (am_bool_t)(int)p_arg) {
            p_dev->rs485_en = (am_bool_t)(int)p_arg;
        }
        break;

    /* 485模式状态获取 */
    case AM_UART_RS485_GET:
        *(int *)p_arg = p_dev->rs485_en;
        break;

    /* 设置流控模式 */
    case AM_UART_FLOWMODE_SET:
        status = -AM_ENOTSUP;
    break;

    /* 设置接收器流控状态 */
    case AM_UART_FLOWSTAT_RX_SET:
        status = -AM_ENOTSUP;
    break;

    /* 获取发送器流控状态 */
    case AM_UART_FLOWSTAT_TX_GET:
        status = -AM_ENOTSUP;
    break;

    default:
        status = -AM_EIO;
        break;
    }

    return (status);
}

/**
 * \brief 启动串口发送(仅限中断模式)
 */
int __lpuart_tx_startup (void *p_drv)
{
    char data  = 0;

    am_hc32_lpuart_dev_t *p_dev       = (am_hc32_lpuart_dev_t *)p_drv;
    amhw_hc32_lpuart_t   *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    /* 使能 485 发送控制引脚 */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 等待上一次传输完成 */
    while (amhw_hc32_lpuart_flag_check(p_hw_lpuart,
                                       AMHW_HC32_LPUART_FLAG_TX_EMPTY) ==
                                       AM_FALSE);

    /* 获取发送数据并发送 */
    if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
        amhw_hc32_lpuart_data_write(p_hw_lpuart, data);
    }

    /* 使能发送完成中断 */
    amhw_hc32_lpuart_int_enable(p_hw_lpuart,
                                AMHW_HC32_LPUART_INT_TX_COMPLETE);

    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __lpuart_callback_set (void  *p_drv,
                                  int    callback_type,
                                  void  *pfn_callback,
                                  void  *p_arg)
{
    am_hc32_lpuart_dev_t *p_dev = (am_hc32_lpuart_dev_t *)p_drv;

    switch (callback_type) {

    /* 设置发送回调函数中的获取发送字符回调函数 */
    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get = (am_uart_txchar_get_t)pfn_callback;
        p_dev->txget_arg      = p_arg;
        return (AM_OK);

    /* 设置接收回调函数中的存放接收字符回调函数 */
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put = (am_uart_rxchar_put_t)pfn_callback;
        p_dev->rxput_arg      = p_arg;
        return (AM_OK);

    /* 设置串口异常回调函数 */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (am_uart_err_t)pfn_callback;
        p_dev->err_arg = p_arg;
        return (AM_OK);

    default:
        return (-AM_ENOTSUP);
    }
}

/**
 * \brief 轮询模式下发送一个字符
 */
static int __lpuart_poll_putchar (void *p_drv, char outchar)
{
    am_hc32_lpuart_dev_t *p_dev     = (am_hc32_lpuart_dev_t *)p_drv;
    amhw_hc32_lpuart_t   *p_hw_lpuart = (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    /* 发送模块是否空闲, AM_FALSE:忙; TURE: 空闲 */
    if(amhw_hc32_lpuart_flag_check(
           p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_EMPTY) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {

            /* 设置 485 为发送模式 */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }

        /* 发送一个字符 */
        amhw_hc32_lpuart_data_write(p_hw_lpuart, outchar);

        /* 等待发送完成 */
        while (amhw_hc32_lpuart_flag_check(
                   p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_EMPTY) == AM_FALSE);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
            p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
        }
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 */
static int __lpuart_poll_getchar (void *p_drv, char *p_char)
{
    am_hc32_lpuart_dev_t *p_dev       = (am_hc32_lpuart_dev_t *)p_drv;
    amhw_hc32_lpuart_t   *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    uint8_t *p_inchar  = (uint8_t *)p_char;

    /* 接收模块是否空闲，AM_FALSE:忙,正在接收; TURE: 已经接收到一个字符 */
    if(amhw_hc32_lpuart_flag_check(
           p_hw_lpuart, AMHW_HC32_LPUART_FLAG_RX_COMPLETE) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        /* 接收一个字符 */
        *p_inchar = amhw_hc32_lpuart_data_read(p_hw_lpuart);

        amhw_hc32_lpuart_flag_clr(p_hw_lpuart,
                                  AMHW_HC32_LPUART_FLAG_RX_COMPLETE);
    }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 */
int __lpuart_mode_set (am_hc32_lpuart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_hc32_lpuart_t *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {

        am_int_connect(p_dev->p_devinfo->inum,
                       __lpuart_irq_handler,
                       (void *)p_dev);

        am_int_enable(p_dev->p_devinfo->inum);

        /* 使能接收完成中断 */
        amhw_hc32_lpuart_int_enable(p_hw_lpuart,
                                    AMHW_HC32_LPUART_INT_RX_COMPLETE);
    } else {

        /* 关闭所有串口中断 */
        amhw_hc32_lpuart_int_disable(p_hw_lpuart, AMHW_HC32_LPUART_INT_ALL);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __lpuart_opt_set (am_hc32_lpuart_dev_t *p_dev, uint32_t options)
{
    amhw_hc32_lpuart_t *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;
    uint32_t            cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x3ul << 14);
        cfg_flags |= AMHW_HC32_LPUART_STOP_2_0_BIT;
    } else {
        cfg_flags &= ~(0x3ul << 14);
        cfg_flags |= AMHW_HC32_LPUART_STOP_1_0_BIT;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x3ul << 2);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_HC32_LPUART_HW_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_HC32_LPUART_HW_PARITY_EVEN;
        }
        amhw_hc32_lpuart_mode_sel(p_hw_lpuart, AMHW_HC32_LPUART_WORK_MODE_3);
    } else {
        cfg_flags &= ~(0x3ul << 2);
        cfg_flags |= AMHW_HC32_LPUART_PARITY_NO;
        amhw_hc32_lpuart_mode_sel(p_hw_lpuart, AMHW_HC32_LPUART_WORK_MODE_1);
    }

    /* 保存和生效配置 */
    amhw_hc32_lpuart_stop_bit_sel(p_hw_lpuart  , (cfg_flags & (0x3ul << 14)));
    amhw_hc32_lpuart_parity_bit_sel(p_hw_lpuart, (cfg_flags & (0x3ul << 2)));

    p_dev->options = options;

    return (AM_OK);
}

/*******************************************************************************
  LPUART interrupt request handler
*******************************************************************************/

/**
 * \brief 串口接收中断服务
 */
void __lpuart_irq_rx_handler (am_hc32_lpuart_dev_t *p_dev)
{
    amhw_hc32_lpuart_t *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;
    char data;

    /* 是否为接收Rx中断 */
    if (amhw_hc32_lpuart_flag_check(
            p_hw_lpuart, AMHW_HC32_LPUART_FLAG_RX_COMPLETE) == AM_TRUE) {

        amhw_hc32_lpuart_flag_clr(p_hw_lpuart,
                                  AMHW_HC32_LPUART_FLAG_RX_COMPLETE);

        /* 获取新接收数据 */
        data = amhw_hc32_lpuart_data_read(p_hw_lpuart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 */
void __lpuart_irq_tx_handler (am_hc32_lpuart_dev_t *p_dev)
{
    amhw_hc32_lpuart_t *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    char data;

    if (amhw_hc32_lpuart_flag_check(
            p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_COMPLETE) == AM_TRUE) {

        amhw_hc32_lpuart_flag_clr(p_hw_lpuart,
                                  AMHW_HC32_LPUART_FLAG_TX_COMPLETE);

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_hc32_lpuart_data_write(p_hw_lpuart, data);
        } else {

            /* 没有数据传送就关闭发送中断 */
            amhw_hc32_lpuart_int_disable(p_hw_lpuart,
                                         AMHW_HC32_LPUART_FLAG_TX_COMPLETE);

            /* 禁能485发送控制引脚 */
            if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir)) {

                /* 设置 485 为接收模式 */
                p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
            }
        }
    }
}

/**
 * \brief 串口中断服务函数
 */
void __lpuart_irq_handler (void *p_arg)
{
    am_hc32_lpuart_dev_t  *p_dev       = (am_hc32_lpuart_dev_t *)p_arg;
    amhw_hc32_lpuart_t    *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;

    uint32_t lpuart_int_stat = amhw_hc32_lpuart_flag_get(p_hw_lpuart);

    if (amhw_hc32_lpuart_flag_check(
            p_hw_lpuart, AMHW_HC32_LPUART_FLAG_RX_COMPLETE) == AM_TRUE) {

         __lpuart_irq_rx_handler(p_dev);

    } else if (amhw_hc32_lpuart_flag_check(
                   p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_COMPLETE) == AM_TRUE) {

        __lpuart_irq_tx_handler(p_dev);

    } else {

    }

    /* 其他中断 */
    if ((p_dev->other_int_enable & lpuart_int_stat) != 0) {

        lpuart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AM_HC32_LPUART_ERRCODE_LPUART_OTHER_INT,
                           (void *)p_hw_lpuart,
                           1);
        }
    }

}

/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __lpuart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief 串口初始化函数
 */
am_uart_handle_t am_hc32_lpuart_init (
                     am_hc32_lpuart_dev_t           *p_dev,
                     const am_hc32_lpuart_devinfo_t *p_devinfo)
{
    amhw_hc32_lpuart_t  *p_hw_lpuart;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_lpuart                =
            (amhw_hc32_lpuart_t  *)p_devinfo->lpuart_reg_base;
    p_dev->p_devinfo           = p_devinfo;
    p_dev->lpuart_serv.p_funcs = (struct am_uart_drv_funcs *)
                                  &__g_lpuart_drv_funcs;
    p_dev->lpuart_serv.p_drv   = p_dev;
    p_dev->baud_rate           = p_devinfo->baud_rate;
    p_dev->options             = 0;

    /* 初始化默认回调函数 */
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__lpuart_dummy_callback;
    p_dev->txget_arg         = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__lpuart_dummy_callback;
    p_dev->rxput_arg         = NULL;
    p_dev->pfn_err           =
                     (int (*) (void *, int, void*, int))__lpuart_dummy_callback;

    p_dev->err_arg           = NULL;

    p_dev->other_int_enable  = p_devinfo->other_int_enable  &
                               ~(AMHW_HC32_LPUART_INT_TX_COMPLETE |
                                 AMHW_HC32_LPUART_INT_RX_COMPLETE);
    p_dev->rs485_en          = AM_FALSE;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 设置通信时钟源 */
    amhw_hc32_lpuart_clk_src_sel(p_hw_lpuart, p_devinfo->sclk_src);

    if(p_devinfo->sclk_src == AMHW_HC32_LPUART_SCLK_SRC_XTL) {

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

        /* 串口时钟获取 */
        p_dev->clk_rate = 32768;

    } else if(p_devinfo->sclk_src == AMHW_HC32_LPUART_SCLK_SRC_RCL) {

        /* 设置RCL时钟频率校准值 （38.4KHz）*/
        amhw_hc32_rcc_rcl_trim_set(*((uint16_t*)0x00100C20));

        amhw_hc32_rcc_rcl_waittime_set(AMHW_HC32_RCL_WAITTIME_256);

        amhw_hc32_rcc_set_start(0x5A5A);
        amhw_hc32_rcc_set_start(0xA5A5);

        amhw_hc32_rcc_rcl_enable();

        /* 等待稳定*/
        while(amhw_hc32_rcc_rcl_state_get() == AM_FALSE);

        /* 串口时钟获取 */
        p_dev->clk_rate = 38400;
    } else {
        p_dev->clk_rate = am_clk_rate_get(CLK_PCLK);
    }

    /* 工作模式设置 */
    if (AM_TRUE == p_devinfo->asyn_hd) {
        amhw_hc32_lpuart_single_line_half_enable(p_hw_lpuart);
    }

    /* 获取串口检验方式配置选项 */
    if ((p_devinfo->cfg_flags & (0xc)) == (AMHW_HC32_LPUART_HW_PARITY_ODD)) {
        p_dev->options |= AM_UART_PARENB | AM_UART_PARODD;
    } else if((p_devinfo->cfg_flags & (0xc)) == (AMHW_HC32_LPUART_HW_PARITY_EVEN)){
        p_dev->options |= AM_UART_PARENB;
    } else {

    }

    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & (AMHW_HC32_LPUART_STOP_2_0_BIT)) {
        p_dev->options |= AM_UART_STOPB;
    } else {

    }

    /* 等待发送空完成 */
    while (amhw_hc32_lpuart_flag_check(
               p_hw_lpuart, AMHW_HC32_LPUART_FLAG_TX_EMPTY) == AM_FALSE);

    __lpuart_opt_set (p_dev, p_dev->options);

    /* 模式0情况下，波特率固定为 PCLK/12 */
    if(amhw_hc32_lpuart_mode_get(p_hw_lpuart) == AMHW_HC32_LPUART_WORK_MODE_0) {

        /* 模式0通信时钟分频系数设置无效 */
        amhw_hc32_lpuart_clk_div_sel(p_hw_lpuart,
                                       AMHW_HC32_LPUART_SCLK_DIV_MODE0_NO);

    /* 模式2情况下，波特率 = 时钟 / 分频系数 */
    } else {

        /* 设置通信时钟分频系数为最小 */
        amhw_hc32_lpuart_clk_div_sel(p_hw_lpuart,
                                       AMHW_HC32_LPUART_SCLK_DIV_MODE2_8);
    }
    
    /* 设置波特率 */
    p_dev->baud_rate = amhw_hc32_lpuart_baudrate_set(p_hw_lpuart,
                                                     p_dev->clk_rate,
                                                     p_devinfo->baud_rate);
    /* 默认轮询模式 */
    __lpuart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_hc32_lpuart_enable(p_hw_lpuart, AMHW_HC32_LPUART_RX);

    if (p_dev->p_devinfo->pfn_rs485_dir) {

        /* 初始化 485 为接收模式 */
        p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
    }

    return &(p_dev->lpuart_serv);
}

/**
 * \brief 串口去初始化
 */
void am_hc32_lpuart_deinit (am_hc32_lpuart_dev_t *p_dev)
{
    amhw_hc32_lpuart_t *p_hw_lpuart =
            (amhw_hc32_lpuart_t *)p_dev->p_devinfo->lpuart_reg_base;
    p_dev->lpuart_serv.p_funcs   = NULL;
    p_dev->lpuart_serv.p_drv     = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {

        /* 默认为轮询模式 */
        __lpuart_mode_set(p_dev, AM_UART_MODE_POLL);
    }

    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_RX);
    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_CTS);
    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_RTS);
    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_DMA_TX);
    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_DMA_RX);
    amhw_hc32_lpuart_disable(p_hw_lpuart,AMHW_HC32_LPUART_MUT_ADR_AUTO);

    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
