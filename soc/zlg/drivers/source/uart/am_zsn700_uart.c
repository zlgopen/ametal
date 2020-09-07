/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zsn700.cn/
*******************************************************************************/

/**
 * \file
 * \brief UART 驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-03-18  yrz, first implementation
 * \endinternal
 */
#include "am_zsn700_uart.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"
#include "zsn700_pin.h"
#include "zsn700_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/**
 * \brief 串口模式（查询或中断）设置
 */
int __uart_mode_set (am_zsn700_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __uart_opt_set (am_zsn700_uart_dev_t *p_dev, uint32_t opts);

/**
 * \brief 流控接受器流控状态设置
 */
static int __uart_flow_rxstat_set (am_zsn700_uart_dev_t *p_dev, uint32_t ctrl);

/**
 * \brief 流控模式设置
 */
static int __uart_flow_mode_set (am_zsn700_uart_dev_t *p_dev, uint32_t mode);

/**
 * \brief 流控发送器流控状态获取
 */
static int __uart_flow_txstat_get (am_zsn700_uart_dev_t *p_dev);

/* ZSN700 串口驱动函数声明 */
static int __uart_ioctl (void *p_drv, int, void *);

static int __uart_tx_startup (void *p_drv);

static int __uart_callback_set (void *p_drv,
                                int   callback_type,
                                void *pfn_callback,
                                void *p_arg);

static int __uart_poll_getchar (void *p_drv, char *p_char);

static int __uart_poll_putchar (void *p_drv, char outchar);

#if 0
static int __uart_connect (void *p_drv);
#endif

static void __uart0_2_irq_handler (void *p_arg);
static void __uart1_3_irq_handler (void *p_arg);

/** \brief 标准层接口函数实现 */
static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
    __uart_ioctl,
    __uart_tx_startup,
    __uart_callback_set,
    __uart_poll_getchar,
    __uart_poll_putchar,
};

/******************************************************************************/

/**
 * \brief 设备控制函数
 *
 * 其中包含设置获取波特率，模式设置（中断/查询），获取支持的模式，硬件选项设置等功能。
 */
static int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_zsn700_uart_dev_t *p_dev     = (am_zsn700_uart_dev_t *)p_drv;
    amhw_zsn700_uart_t   *p_hw_uart = (amhw_zsn700_uart_t *)
                                      p_dev->p_devinfo->uart_reg_base;

    int status = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        /* 只有在发送空的基础上才允许修改波特率 */
        while (amhw_zsn700_uart_flag_check(
                   p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_EMPTY) == AM_FALSE);

        status = amhw_zsn700_uart_baudrate_set(
                     p_hw_uart, p_dev->clk_rate, (uint32_t)p_arg);

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
        status = (__uart_mode_set(p_dev, (int)p_arg) == AM_OK)
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
        status = (__uart_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
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
        __uart_flow_mode_set(p_dev, (int)p_arg);
    break;

    /* 设置接收器流控状态 */
    case AM_UART_FLOWSTAT_RX_SET:
        __uart_flow_rxstat_set(p_dev, (int)p_arg);
    break;

    /* 获取发送器流控状态 */
    case AM_UART_FLOWSTAT_TX_GET:
        *(int *)p_arg = __uart_flow_txstat_get(p_dev);
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
int __uart_tx_startup (void *p_drv)
{
    char data  = 0;

    am_zsn700_uart_dev_t *p_dev     = (am_zsn700_uart_dev_t *)p_drv;
    amhw_zsn700_uart_t   *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 使能 485 发送控制引脚 */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 等待上一次传输完成 */
    while (amhw_zsn700_uart_flag_check(
               p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_EMPTY) == AM_FALSE);

    /* 获取发送数据并发送 */
    if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
        amhw_zsn700_uart_data_write(p_hw_uart, data);
    }

    /* 使能发送完成中断 */
    amhw_zsn700_uart_int_enable(p_hw_uart, AMHW_ZSN700_UART_INT_TX_COMPLETE);

    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __uart_callback_set (void  *p_drv,
                                int    callback_type,
                                void  *pfn_callback,
                                void  *p_arg)
{
    am_zsn700_uart_dev_t *p_dev = (am_zsn700_uart_dev_t *)p_drv;

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
static int __uart_poll_putchar (void *p_drv, char outchar)
{
    am_zsn700_uart_dev_t *p_dev     = (am_zsn700_uart_dev_t *)p_drv;
    amhw_zsn700_uart_t   *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 发送模块是否空闲, AM_FALSE:忙; TURE: 空闲 */
    if(amhw_zsn700_uart_flag_check(
           p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_EMPTY) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {

            /* 设置 485 为发送模式 */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }

        /* 发送一个字符 */
        amhw_zsn700_uart_data_write(p_hw_uart, outchar);

        /* 等待发送完成 */
        while (amhw_zsn700_uart_flag_check(
                   p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_EMPTY) == AM_FALSE);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
            p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
        }
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 */
static int __uart_poll_getchar (void *p_drv, char *p_char)
{
    am_zsn700_uart_dev_t *p_dev     = (am_zsn700_uart_dev_t *)p_drv;
    amhw_zsn700_uart_t   *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;
    uint8_t           *p_inchar  = (uint8_t *)p_char;

    /* 接收模块是否空闲，AM_FALSE:忙,正在接收; TURE: 已经接收到一个字符 */
    if(amhw_zsn700_uart_flag_check(
           p_hw_uart, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        /* 接收一个字符 */
        *p_inchar = amhw_zsn700_uart_data_read(p_hw_uart);

        amhw_zsn700_uart_flag_clr(p_hw_uart,
                                  AMHW_ZSN700_UART_FLAG_RX_COMPLETE);
    }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 */
int __uart_mode_set (am_zsn700_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_zsn700_uart_t *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {

        if (p_dev->p_devinfo->inum == 6){
            am_int_connect(p_dev->p_devinfo->inum,
                           __uart0_2_irq_handler,
                           (void *)p_dev);
        }else{
            am_int_connect(p_dev->p_devinfo->inum,
                           __uart1_3_irq_handler,
                           (void *)p_dev);
        }

        am_int_enable(p_dev->p_devinfo->inum);

        /* 使能接收完成中断 */
        amhw_zsn700_uart_int_enable(p_hw_uart, AMHW_ZSN700_UART_INT_RX_COMPLETE);
    } else {

        /* 关闭所有串口中断 */
        amhw_zsn700_uart_int_disable(p_hw_uart, AMHW_ZSN700_UART_INT_ALL);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __uart_opt_set (am_zsn700_uart_dev_t *p_dev, uint32_t options)
{
    amhw_zsn700_uart_t *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;
    uint32_t            cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x3ul << 14);
        cfg_flags |= AMHW_ZSN700_UART_STOP_2_0_BIT;
    } else {
        cfg_flags &= ~(0x3ul << 14);
        cfg_flags |= AMHW_ZSN700_UART_STOP_1_0_BIT;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x3ul << 2);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_ZSN700_UART_HW_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_ZSN700_UART_HW_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x3ul << 2);
        cfg_flags |= AMHW_ZSN700_UART_PARITY_NO;
    }

    /* 保存和生效配置 */
    amhw_zsn700_uart_stop_bit_sel(p_hw_uart  , (cfg_flags & (0x3ul << 14)));
    amhw_zsn700_uart_parity_bit_sel(p_hw_uart, (cfg_flags & (0x3ul << 2)));

    p_dev->options = options;

    return (AM_OK);
}
/******************************************************************************/
/**
 * \brief 接收器流控状态设置（开或关）
 */
static int __uart_flow_rxstat_set (am_zsn700_uart_dev_t *p_dev, uint32_t ctrl)
{
    amhw_zsn700_uart_t *p_hw_uart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    if (AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        return -AM_ENOTSUP;

    /* 如果是硬件流控，配置RTS输出引脚电位 */
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            /* 拉低RTS管脚,开流     */
            am_gpio_set(p_dev->p_devinfo->hwflowctl_cfg.rts_pin, 0);

        } else {
            /* 拉高RTS管脚,限流     */
            am_gpio_set(p_dev->p_devinfo->hwflowctl_cfg.rts_pin, 1);
        }
    } else {
        /* 如果是软件流控，发送XON/XOFF流控字符 */
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            amhw_zsn700_uart_data_write(p_hw_uart, AM_ZSN700_UART_XON);
        } else {
            amhw_zsn700_uart_data_write(p_hw_uart, AM_ZSN700_UART_XOFF);
        }
    }

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 流控模式设置（无流控，软件流控，硬件流控）
 */
static int __uart_flow_mode_set(am_zsn700_uart_dev_t *p_dev, uint32_t mode)
{
    amhw_zsn700_uart_t *p_hw_uart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;
    p_dev->flowctl_mode = mode;

    /* 无流控 */
    if(AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_ON;

        /* 硬件流控禁能     */
        amhw_zsn700_uart_disable(p_hw_uart, AMHW_ZSN700_UART_CTS);
        amhw_zsn700_uart_disable(p_hw_uart, AMHW_ZSN700_UART_RTS);

        /* 硬件流控引脚解初始化 */
        if(p_dev->p_devinfo->hwflowctl_cfg.enable == AM_TRUE) {

            /* 解初始化 */
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.rts_pin,
                            PIO_INPUT_PU);
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.cts_pin,
                            PIO_INPUT_PU);
        }

    /* 硬件流控 */
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* 硬件流控引脚初始化 */
        if(p_dev->p_devinfo->hwflowctl_cfg.enable == AM_TRUE) {

            /* 初始化 */
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.rts_pin,
                            PIO_OUT_PP);
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.cts_pin,
                            PIO_INPUT_FLOAT);
        }

        /* 硬件流控使能     */
        amhw_zsn700_uart_enable(p_hw_uart, AMHW_ZSN700_UART_CTS);
        amhw_zsn700_uart_enable(p_hw_uart, AMHW_ZSN700_UART_RTS);
    } else {

        /* 硬件流控禁能     */
        amhw_zsn700_uart_disable(p_hw_uart, AMHW_ZSN700_UART_CTS);
        amhw_zsn700_uart_disable(p_hw_uart, AMHW_ZSN700_UART_RTS);

        /* 硬件流控引脚解初始化 */
        if(p_dev->p_devinfo->hwflowctl_cfg.enable == AM_TRUE) {

            /* 解初始化 */
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.rts_pin,
                            PIO_INPUT_PU);
            am_gpio_pin_cfg(p_dev->p_devinfo->hwflowctl_cfg.cts_pin,
                            PIO_INPUT_PU);
        }

    }

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 发送器流控状态获取
 */
static int __uart_flow_txstat_get (am_zsn700_uart_dev_t *p_dev)
{
    amhw_zsn700_uart_t *p_hw_uart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    if(AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* cts的状态发生变化时，状态位有效 */
        if(amhw_zsn700_uart_flag_check(p_hw_uart,
                                       AMHW_ZSN700_UART_FLAG_CTS_TRIGGER) ==
                                       AM_TRUE) {

            /* 清除cts状态位 */
            amhw_zsn700_uart_flag_clr(p_hw_uart,
                                      AMHW_ZSN700_UART_FLAG_CTS_TRIGGER);

            /* 判断cts当前高低电平 */
            if (am_gpio_get(p_dev->p_devinfo->hwflowctl_cfg.cts_pin) == 0) {
                return (int)AM_UART_FLOWSTAT_ON;
            } else {
                return (int)AM_UART_FLOWSTAT_OFF;
            }
        } else {
            return (int)p_dev->flowctl_tx_stat;
        }
    } else {
        return (int)p_dev->flowctl_tx_stat;
    }
}

/*******************************************************************************
  UART interrupt request handler
*******************************************************************************/

/**
 * \brief 串口接收中断服务
 */
void __uart_irq_rx_handler (am_zsn700_uart_dev_t *p_dev)
{
    amhw_zsn700_uart_t *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;
    char data;

    /* 是否为接收Rx中断 */
    if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE) {

        amhw_zsn700_uart_flag_clr(p_hw_uart, AMHW_ZSN700_UART_FLAG_RX_COMPLETE);

        /* 获取新接收数据 */
        data = amhw_zsn700_uart_data_read(p_hw_uart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 */
void __uart_irq_tx_handler (am_zsn700_uart_dev_t *p_dev)
{
    amhw_zsn700_uart_t *p_hw_uart = (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;

    char data;

    if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE) {

        amhw_zsn700_uart_flag_clr(p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_COMPLETE);

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_zsn700_uart_data_write(p_hw_uart, data);
        } else {

            /* 没有数据传送就关闭发送中断 */
            amhw_zsn700_uart_int_disable(p_hw_uart,
                                         AMHW_ZSN700_UART_FLAG_TX_COMPLETE);

            /* 禁能485发送控制引脚 */
            if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir)) {

                /* 设置 485 为接收模式 */
                p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
            }
        }
    }
}

/**
 * \brief 串口0-2中断服务函数
 */

/**< \brief 处理中断源复用需要全局调用一次设备信息 */
extern am_zsn700_uart_dev_t  *__gp_uart0;
extern am_zsn700_uart_dev_t  *__gp_uart2;
void __uart0_2_irq_handler (void *p_arg)
{
    /* 同一个中断源的中断是否同时产生标志 */
    uint8_t  flag_event    = 0;
    uint32_t uart_int_stat = 0;

    amhw_zsn700_uart_t *p_hw_uart  = NULL;
    am_zsn700_uart_dev_t  *p_dev   = NULL;

    amhw_zsn700_uart_t    *p_hw_uart0 = NULL;
    amhw_zsn700_uart_t    *p_hw_uart2 = NULL;

    am_zsn700_uart_dev_t  *p_dev0     = __gp_uart0;
    if (p_dev0 != NULL){
        p_hw_uart0 = (amhw_zsn700_uart_t *) p_dev0->p_devinfo->uart_reg_base;
    }

    am_zsn700_uart_dev_t  *p_dev2     = __gp_uart2;
    if (p_dev2 != NULL){
        p_hw_uart2 = (amhw_zsn700_uart_t *) p_dev2->p_devinfo->uart_reg_base;
    }

    /* 判断哪个串口使用中断（或者同时使用） */
    if ( amhw_zsn700_uart_int_check (p_hw_uart0) &&
        !amhw_zsn700_uart_int_check (p_hw_uart2)){
        p_hw_uart = p_hw_uart0;
        p_dev = p_dev0;
        flag_event = 0;
    }else if (!amhw_zsn700_uart_int_check (p_hw_uart0) &&
               amhw_zsn700_uart_int_check (p_hw_uart2)){
        p_hw_uart = p_hw_uart2;
        p_dev = p_dev2;
        flag_event = 0;
    }else if (amhw_zsn700_uart_int_check (p_hw_uart0) &&
              amhw_zsn700_uart_int_check (p_hw_uart2)){
        flag_event = 1;
    }

    /* 判断中断类型 */
    if (!flag_event){
        uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart);

        if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE) {

                 __uart_irq_rx_handler(p_dev);

        } else if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE) {

                __uart_irq_tx_handler(p_dev);

        } else {

        }
    }else{
        if (amhw_zsn700_uart_flag_check(
                  p_hw_uart0, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart0);
            __uart_irq_rx_handler(p_dev0);
        }else if (amhw_zsn700_uart_flag_check(
                p_hw_uart0, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart0);
            __uart_irq_tx_handler(p_dev0);
        }else if (amhw_zsn700_uart_flag_check(
                p_hw_uart2, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart2);
            __uart_irq_rx_handler(p_dev2);
        }else if (amhw_zsn700_uart_flag_check(
                  p_hw_uart2, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart2);
            __uart_irq_tx_handler(p_dev2);
        }else{
            ;
        }
    }

    /* 其他中断 */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AM_ZSN700_UART_ERRCODE_UART_OTHER_INT,
                           (void *)p_hw_uart,
                           1);
        }
    }

}

/**
 * \brief 串口1-3中断服务函数
 */
/**< \brief 处理中断源复用需要全局调用一次设备信息 */
extern am_zsn700_uart_dev_t  *__gp_uart1;
extern am_zsn700_uart_dev_t  *__gp_uart3;
void __uart1_3_irq_handler (void *p_arg)
{
    /* 同一个中断源的中断是否同时产生标志 */
    uint8_t  flag_event    = 0;
    uint32_t uart_int_stat = 0;

    amhw_zsn700_uart_t *p_hw_uart  = NULL;
    am_zsn700_uart_dev_t  *p_dev   = NULL;

    amhw_zsn700_uart_t    *p_hw_uart1 = NULL;
    amhw_zsn700_uart_t    *p_hw_uart3 = NULL;

    am_zsn700_uart_dev_t  *p_dev1     = __gp_uart1;
    if (p_dev1 != NULL){
        p_hw_uart1 = (amhw_zsn700_uart_t *) p_dev1->p_devinfo->uart_reg_base;
    }

    am_zsn700_uart_dev_t  *p_dev3     = __gp_uart3;
    if (p_dev3 != NULL){
        p_hw_uart3 = (amhw_zsn700_uart_t *) p_dev3->p_devinfo->uart_reg_base;
    }

    /* 判断哪个串口使用中断（或者同时使用） */
    if ( amhw_zsn700_uart_int_check (p_hw_uart1) &&
        !amhw_zsn700_uart_int_check (p_hw_uart3)){
        p_hw_uart = p_hw_uart1;
        p_dev     = p_dev1;
        flag_event = 0;
    }else if (!amhw_zsn700_uart_int_check (p_hw_uart1) &&
               amhw_zsn700_uart_int_check (p_hw_uart3)){
        p_hw_uart = p_hw_uart3;
        p_dev     = p_dev3;
        flag_event = 0;
    }else if (amhw_zsn700_uart_int_check (p_hw_uart1) &&
              amhw_zsn700_uart_int_check (p_hw_uart3)){
        flag_event = 1;
    }

    /* 判断中断类型 */
    if (!flag_event){
        uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart);

        if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE) {

                 __uart_irq_rx_handler(p_dev);

        } else if (amhw_zsn700_uart_flag_check(
            p_hw_uart, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE) {

                __uart_irq_tx_handler(p_dev);

        } else {

        }
    }else{
        if (amhw_zsn700_uart_flag_check(
                  p_hw_uart1, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart1);
            __uart_irq_rx_handler(p_dev1);
        }else if (amhw_zsn700_uart_flag_check(
                  p_hw_uart3, AMHW_ZSN700_UART_FLAG_RX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart3);
            __uart_irq_rx_handler(p_dev3);
        }else if (amhw_zsn700_uart_flag_check(
                  p_hw_uart1, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart1);
            __uart_irq_tx_handler(p_dev1);
        }else if (amhw_zsn700_uart_flag_check(
                p_hw_uart3, AMHW_ZSN700_UART_FLAG_TX_COMPLETE) == AM_TRUE){
            uart_int_stat = amhw_zsn700_uart_flag_get(p_hw_uart3);
            __uart_irq_tx_handler(p_dev3);
        }else{
            ;
        }
    }

    /* 其他中断 */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AM_ZSN700_UART_ERRCODE_UART_OTHER_INT,
                           (void *)p_hw_uart,
                           1);
        }
    }

}

#if 0
/**
 * \brief UART中断函数连接，仅使用中断模式时需要调用此函数
 */
int __uart_connect (void *p_drv)
{
    am_zsn700_uart_dev_t *p_dev = (am_zsn700_uart_dev_t *)p_drv;

    /* 关联中断向量号，开启中断 */
    am_int_connect(p_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_zsn700_uart_int_enable(p_dev->p_devinfo->uart_reg_base,
                                p_dev->other_int_enable);

    return AM_OK;
}
#endif /* 0 */

/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __uart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief 串口初始化函数
 */
am_uart_handle_t am_zsn700_uart_init (am_zsn700_uart_dev_t           *p_dev,
                                      const am_zsn700_uart_devinfo_t *p_devinfo)
{
    amhw_zsn700_uart_t  *p_hw_uart;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_uart                = (amhw_zsn700_uart_t  *)p_devinfo->uart_reg_base;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv   = p_dev;
    p_dev->baud_rate         = p_devinfo->baud_rate;
    p_dev->options           = 0;

    /* 初始化默认回调函数 */
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__uart_dummy_callback;
    p_dev->txget_arg         = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__uart_dummy_callback;
    p_dev->rxput_arg         = NULL;
    p_dev->pfn_err           =
                     (int (*) (void *, int, void*, int))__uart_dummy_callback;

    p_dev->err_arg           = NULL;

    p_dev->flowctl_mode      = AM_UART_FLOWCTL_NO;
    p_dev->flowctl_tx_stat   = AM_UART_FLOWSTAT_ON;

    p_dev->other_int_enable  = p_devinfo->other_int_enable  &
                               ~(AMHW_ZSN700_UART_INT_TX_COMPLETE |
                                 AMHW_ZSN700_UART_INT_RX_COMPLETE);

    p_dev->rs485_en          = AM_FALSE;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 工作模式2/3，且开启了多机地址自动识别，则做如下配置 */
    if((p_devinfo->work_mode == 2) || (p_devinfo->work_mode == 3)) {

        if((p_devinfo->mut_addr.enable == AM_TRUE)) {

            /* 设置从机地址（针对本机接收其他设备发送的数据而言）*/
            amhw_zsn700_uart_slaver_addr_set(p_hw_uart,
                                             p_devinfo->mut_addr.addr);

            /* 设置地址掩码 */
            amhw_zsn700_uart_slaver_addr_set(p_hw_uart,
                                             p_devinfo->mut_addr.addr_mask);

            /* 多机地址自动识别使能 */
            amhw_zsn700_uart_enable(p_hw_uart, AMHW_ZSN700_UART_MUT_ADR_AUTO);
        }
    }

    /* 工作模式设置 */
    if (p_devinfo->work_mode == AMHW_ZSN700_UART_WORK_MODE_4) {
        amhw_zsn700_uart_single_line_half_enable(p_hw_uart);
        if ((p_devinfo->cfg_flags & (0xc)) == AMHW_ZSN700_UART_PARITY_NO){
            amhw_zsn700_uart_mode_sel(p_hw_uart, AMHW_ZSN700_UART_WORK_MODE_1);
        }else {
            amhw_zsn700_uart_mode_sel(p_hw_uart, AMHW_ZSN700_UART_WORK_MODE_3);
        }
    } else {
        amhw_zsn700_uart_mode_sel(p_hw_uart, p_devinfo->work_mode);
    }
    p_dev->work_mode = p_devinfo->work_mode;

    if(p_devinfo->work_mode == AMHW_ZSN700_UART_WORK_MODE_0) {

        /* 模式0通信时钟分频系数设置无效 */
        amhw_zsn700_uart_clk_div_sel(p_hw_uart,
                                     AMHW_ZSN700_UART_CLK_DIV_MODE0_NO);
    } else {

        /* 设置通信时钟分频系数为最小 */
        amhw_zsn700_uart_clk_div_sel(p_hw_uart,
                                     AMHW_ZSN700_UART_CLK_DIV_MODE2_16);
    }

    /* 获取串口检验方式配置选项 */
    if ((p_devinfo->cfg_flags & (0xc)) == (AMHW_ZSN700_UART_HW_PARITY_ODD)) {
        p_dev->options |= AM_UART_PARENB | AM_UART_PARODD;
    } else if ((p_devinfo->cfg_flags & (0xc)) == (AMHW_ZSN700_UART_HW_PARITY_EVEN)){
        p_dev->options |= AM_UART_PARENB;
    } else {

    }

    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & (AMHW_ZSN700_UART_STOP_2_0_BIT)) {
        p_dev->options |= AM_UART_STOPB;
    } else {

    }

    /* 等待发送空完成 */
    while (amhw_zsn700_uart_flag_check(p_hw_uart,
                                       AMHW_ZSN700_UART_FLAG_TX_EMPTY) ==
                                       AM_FALSE);

    __uart_opt_set (p_dev, p_dev->options);

    /* 流控模式设置（无流控，软件流控，硬件流控）*/
    __uart_flow_mode_set(p_dev, AM_UART_FLOWCTL_NO);

    p_dev->clk_rate = am_clk_rate_get(CLK_PCLK);

    /* 设置波特率 */
    p_dev->baud_rate = amhw_zsn700_uart_baudrate_set(
                                     p_hw_uart,
                                     p_dev->clk_rate,
                                     p_devinfo->baud_rate);
    /* 默认轮询模式 */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* 接收使能
     * Mode0:     0：发送; 1：接收
     * Mode1/2/3: 0：发送; 1： 接收/发送
     */
    amhw_zsn700_uart_enable(p_hw_uart,AMHW_ZSN700_UART_RX);

    if (p_dev->p_devinfo->pfn_rs485_dir) {

        /* 初始化 485 为接收模式 */
        p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
    }

    return &(p_dev->uart_serv);
}

/**
 * \brief 串口去初始化
 */
void am_zsn700_uart_deinit (am_zsn700_uart_dev_t *p_dev)
{
    amhw_zsn700_uart_t *p_hw_uart =
                          (amhw_zsn700_uart_t *)p_dev->p_devinfo->uart_reg_base;
    p_dev->uart_serv.p_funcs   = NULL;
    p_dev->uart_serv.p_drv     = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {

        /* 默认为轮询模式 */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }

    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_RX);
    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_CTS);
    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_RTS);
    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_DMA_TX);
    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_DMA_RX);
    amhw_zsn700_uart_disable(p_hw_uart,AMHW_ZSN700_UART_MUT_ADR_AUTO);

    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
