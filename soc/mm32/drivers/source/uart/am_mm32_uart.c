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
 * \brief UART 驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  ari, first implementation
 * \endinternal
 */
#include "am_mm32_uart.h"
#include "am_clk.h"
#include "am_int.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

#define  __UART_CLK_RATE   24000000

/**
 * \brief 串口模式（查询或中断）设置
 */
int __uart_mode_set (am_mm32_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __uart_opt_set (am_mm32_uart_dev_t *p_dev, uint32_t opts);

/* ZLG 串口驱动函数声明 */
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

static void __uart_irq_handler (void *p_arg);

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
    am_mm32_uart_dev_t *p_dev     = (am_mm32_uart_dev_t *)p_drv;
    amhw_mm32_uart_t   *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    int status = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        /* 只有在当前传输完成的基础上才允许修改波特率 */
        while (amhw_mm32_uart_status_flag_check(p_hw_uart, AMHW_MM32_UART_TX_COMPLETE_FALG) == AM_FALSE);

        status = amhw_mm32_uart_baudrate_set(p_hw_uart,
                                               am_clk_rate_get(p_dev->p_devinfo->clk_num),
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

    case AM_UART_RS485_SET:
        if (p_dev->rs485_en != (am_bool_t)(int)p_arg) {
            p_dev->rs485_en = (am_bool_t)(int)p_arg;
        }
        break;

    case AM_UART_RS485_GET:
        *(int *)p_arg = p_dev->rs485_en;
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

    am_mm32_uart_dev_t *p_dev     = (am_mm32_uart_dev_t *)p_drv;
    amhw_mm32_uart_t   *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 使能 485 发送控制引脚 */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 等待上一次传输完成 */
    while (amhw_mm32_uart_status_flag_check(p_hw_uart, AMHW_MM32_UART_TX_COMPLETE_FALG) == AM_FALSE);

    /* 获取发送数据并发送 */
    if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
        amhw_mm32_uart_data_write(p_hw_uart, data);
    }

    /* 使能发送中断 */
    amhw_mm32_uart_int_enable(p_hw_uart, AMHW_MM32_UART_INT_TX_EMPTY_ENABLE);
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
    am_mm32_uart_dev_t *p_dev = (am_mm32_uart_dev_t *)p_drv;

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
    am_mm32_uart_dev_t *p_dev     = (am_mm32_uart_dev_t *)p_drv;
    amhw_mm32_uart_t   *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 发送模块是否空闲, AM_FALSE:忙; TURE: 空闲 */
    if(amhw_mm32_uart_status_flag_check(p_hw_uart, AMHW_MM32_UART_TX_EMPTY_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {

            /* 设置 485 为发送模式 */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }

        /* 发送一个字符 */
        amhw_mm32_uart_data_write(p_hw_uart, outchar);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

            /* 等待发送完成 */
            while (!(p_hw_uart->csr & 0x01));

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
    am_mm32_uart_dev_t *p_dev     = (am_mm32_uart_dev_t *)p_drv;
    amhw_mm32_uart_t   *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;
    uint8_t           *p_inchar  = (uint8_t *)p_char;

    /* 接收模块是否空闲，AM_FALSE:忙,正在接收; TURE: 已经接收到一个字符 */
    if(amhw_mm32_uart_status_flag_check(p_hw_uart, AMHW_MM32_UART_RX_VAL_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        /* 接收一个字符 */
        *p_inchar = amhw_mm32_uart_data_read(p_hw_uart);
    }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 */
int __uart_mode_set (am_mm32_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {

        am_int_connect(p_dev->p_devinfo->inum,
                       __uart_irq_handler,
                       (void *)p_dev);
        am_int_enable(p_dev->p_devinfo->inum);

        /* 使能RDRF接收准中断 */
        amhw_mm32_uart_int_enable(p_hw_uart, AMHW_MM32_UART_INT_RX_VAL_ENABLE);
    } else {

        /* 关闭所有串口中断 */
        amhw_mm32_uart_int_disable(p_hw_uart, AMHW_MM32_UART_INT_ALL_ENABLE_MASK);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __uart_opt_set (am_mm32_uart_dev_t *p_dev, uint32_t options)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;
    uint8_t          cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 在改变UART寄存器值前 接收发送禁能 */
    amhw_mm32_uart_disable(p_hw_uart);

    /* 配置数据长度 */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS5:
        cfg_flags |= AMHW_MM32_UART_DATA_5BIT;
        break;

    case AM_UART_CS6:
        cfg_flags |= AMHW_MM32_UART_DATA_6BIT;
        break;

    case AM_UART_CS7:
        cfg_flags |= AMHW_MM32_UART_DATA_7BIT;
        break;

    case AM_UART_CS8:
        cfg_flags |= AMHW_MM32_UART_DATA_8BIT;
        break;

    default:
        break;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_MM32_UART_STOP_2BIT;
    } else {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_MM32_UART_STOP_1BIT;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x3 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_MM32_UART_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_MM32_UART_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x3 << 0);
        cfg_flags |= AMHW_MM32_UART_PARITY_NO;
    }

    /* 保存和生效配置 */
    amhw_mm32_uart_stop_bit_sel(p_hw_uart, (cfg_flags & 0x4));
    amhw_mm32_uart_data_length(p_hw_uart, (cfg_flags & 0x30));
    amhw_mm32_uart_parity_bit_sel(p_hw_uart, (cfg_flags & 0x3));

    amhw_mm32_uart_enable(p_hw_uart);

    p_dev->options = options;

    return (AM_OK);
}

/*******************************************************************************
  UART interrupt request handler
*******************************************************************************/

/**
 * \brief 串口接收中断服务
 */
void __uart_irq_rx_handler (am_mm32_uart_dev_t *p_dev)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;
    char data;

    /* 是否为接收Rx中断 */
    if (amhw_mm32_uart_int_flag_check(p_hw_uart,AMHW_MM32_UART_INT_RX_VAL_FLAG) == AM_TRUE) {

        amhw_mm32_uart_int_flag_clr(p_hw_uart, AMHW_MM32_UART_INT_RX_VAL_FLAG_CLR);

        /* 获取新接收数据 */
        data = amhw_mm32_uart_data_read(p_hw_uart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 */
void __uart_irq_tx_handler (am_mm32_uart_dev_t *p_dev)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    char data;

    if (amhw_mm32_uart_int_flag_check(p_hw_uart,AMHW_MM32_UART_INT_TX_EMPTY_FLAG) == AM_TRUE) {

        amhw_mm32_uart_int_flag_clr(p_hw_uart, AMHW_MM32_UART_INT_TX_EMPTY_FLAG_CLR);

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_mm32_uart_data_write(p_hw_uart, data);
        } else {

            /* 没有数据传送就关闭发送中断 */
            amhw_mm32_uart_int_disable(p_hw_uart, AMHW_MM32_UART_INT_TX_EMPTY_ENABLE);

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
void __uart_irq_handler (void *p_arg)
{
    am_mm32_uart_dev_t  *p_dev     = (am_mm32_uart_dev_t *)p_arg;
    amhw_mm32_uart_t    *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;

    uint32_t uart_int_stat        = amhw_mm32_uart_int_flag_get(p_hw_uart);

    if (amhw_mm32_uart_int_flag_check(p_hw_uart,AMHW_MM32_UART_INT_RX_VAL_FLAG) == AM_TRUE) {
         __uart_irq_rx_handler(p_dev);
    } else if (amhw_mm32_uart_int_flag_check(p_hw_uart,AMHW_MM32_UART_INT_TX_EMPTY_FLAG) == AM_TRUE) {
        __uart_irq_tx_handler(p_dev);
    } else {

    }

    /* 其他中断 */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AM_MM32_UART_ERRCODE_UART_OTHER_INT,
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
    am_mm32_uart_dev_t *p_dev = (am_mm32_uart_dev_t *)p_drv;

    /* 关联中断向量号，开启中断 */
    am_int_connect(p_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_mm32_uart_int_enable(p_dev->p_devinfo->uart_reg_base,
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
am_uart_handle_t am_mm32_uart_init (am_mm32_uart_dev_t           *p_dev,
                                      const am_mm32_uart_devinfo_t *p_devinfo)
{
    amhw_mm32_uart_t  *p_hw_uart;
    uint32_t          tmp;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_uart                = (amhw_mm32_uart_t  *)p_devinfo->uart_reg_base;
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

    p_dev->other_int_enable  = p_devinfo->other_int_enable  &
                               ~(AMHW_MM32_UART_INT_TX_EMPTY_ENABLE |
                                 AMHW_MM32_UART_INT_RX_VAL_ENABLE);
    p_dev->rs485_en          = AM_FALSE;

    /* 获取串口数据长度配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 4) & 0x03;

    switch (tmp) {

    case 0:
        p_dev->options |= AM_UART_CS5;
        break;

    case 1:
        p_dev->options |= AM_UART_CS6;
        break;
    case 2:
        p_dev->options |= AM_UART_CS7;
        break;
    case 3:
        p_dev->options |= AM_UART_CS8;
        break;

    default:
        p_dev->options |= AM_UART_CS8;
        break;
    }

    /* 开启UART时钟 */
    am_clk_enable(p_devinfo->clk_num);

    /* 获取串口检验方式配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 1) {
        p_dev->options |= AM_UART_PARENB | AM_UART_PARODD;
    } else {
    }

    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & (AMHW_MM32_UART_STOP_2BIT)) {
        p_dev->options |= AM_UART_STOPB;
    } else {

    }

    /* 等待上一次传输完成 */
    while (amhw_mm32_uart_status_flag_check(p_hw_uart, AMHW_MM32_UART_TX_COMPLETE_FALG) == AM_FALSE);

    __uart_opt_set (p_dev, p_dev->options);

    /* 设置波特率 */
    p_dev->baud_rate = amhw_mm32_uart_baudrate_set(
                                     p_hw_uart,
                                     am_clk_rate_get(p_dev->p_devinfo->clk_num),
                                     p_devinfo->baud_rate);
    /* 默认轮询模式 */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* uart使能 */

    amhw_mm32_uart_rx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_tx_enable(p_hw_uart,AM_TRUE);
    amhw_mm32_uart_enable(p_hw_uart);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    if (p_dev->p_devinfo->pfn_rs485_dir) {

        /* 初始化 485 为接收模式 */
        p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
    }

    return &(p_dev->uart_serv);
}

/**
 * \brief 串口去初始化
 */
void am_mm32_uart_deinit (am_mm32_uart_dev_t *p_dev)
{
    amhw_mm32_uart_t *p_hw_uart = (amhw_mm32_uart_t *)p_dev->p_devinfo->uart_reg_base;
    p_dev->uart_serv.p_funcs   = NULL;
    p_dev->uart_serv.p_drv     = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {

        /* 默认为轮询模式 */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }

    /* 关闭串口 */
    amhw_mm32_uart_disable(p_hw_uart);

    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
