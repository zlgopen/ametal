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
#include "am_hc32f460_uart.h"
#include "am_clk.h"
#include "am_int.h"
#include "hw/amhw_hc32f460_uart.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"

/*******************************************************************************
 * Functions declaration
 *******************************************************************************/
#if 1
void __uart_irq_rx_handler(void *p_arg);
void __uart_irq_tx_handler(void *p_arg);
void __uart_irq_tci_handler(void *p_arg);
#endif

/**
 * \brief 串口模式（查询或中断）设置
 */
int __uart_mode_set(am_hc32f460_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __uart_opt_set(am_hc32f460_uart_dev_t *p_dev, uint32_t opts);

/* ZLG 串口驱动函数声明 */
static int __uart_ioctl(void *p_drv, int, void *);

static int __uart_tx_startup(void *p_drv);

static int __uart_callback_set(void *p_drv,
                               int   callback_type,
                               void *pfn_callback,
                               void *p_arg);

static int __uart_poll_getchar(void *p_drv, char *p_char);

static int __uart_poll_putchar(void *p_drv, char outchar);


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
static int __uart_ioctl(void *p_drv, int request, void *p_arg)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *) p_drv;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;

    int status = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        /* 只有在当前传输完成的基础上才允许修改波特率 */
        while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
        AMHW_HC32F460_UART_TX_COMPLETE_FALG) == AM_FALSE)
            ;
        status = amhw_hc32f460_uart_baudrate_set(p_hw_uart,
                am_clk_rate_get(p_dev->p_devinfo->clk_num), (uint32_t) p_arg);

        if (status > 0) {
            p_dev->baud_rate = status;
            status = AM_OK;
        }
        else {
            status = -AM_EIO;
        }

        break;

        /* 波特率获取 */
    case AM_UART_BAUD_GET:
        *(int *) p_arg = p_dev->baud_rate;
        break;

        /* 模式设置 */
    case AM_UART_MODE_SET:
        status = (__uart_mode_set(p_dev, (int) p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

        /* 模式获取 */
    case AM_UART_MODE_GET:
        *(int *) p_arg = p_dev->channel_mode;
        break;

        /* 获取串口可设置的模式 */
    case AM_UART_AVAIL_MODES_GET:
        *(int *) p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

        /* 串口选项设置 */
    case AM_UART_OPTS_SET:
        status = (__uart_opt_set(p_dev, (int) p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

        /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *) p_arg = p_dev->options;
        break;

    case AM_UART_RS485_SET:
        if (p_dev->rs485_en != (am_bool_t) (int) p_arg) {
            p_dev->rs485_en = (am_bool_t) (int) p_arg;
        }
        break;

    case AM_UART_RS485_GET:
        *(int *) p_arg = p_dev->rs485_en;
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
int __uart_tx_startup(void *p_drv)
{
	char data  = 0;
	uint32_t u32Addr;
	__IO stc_usart_cr1_field_t CR1_f;

    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *) p_drv;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;

    /* 使能 485 发送控制引脚 */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 等待上一次传输完成 */
    while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
            AMHW_HC32F460_UART_TX_EMPTY_FLAG) == AM_FALSE);

    /* 获取发送数据并发送 */
    if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
        amhw_hc32f460_uart_data_write(p_hw_uart, data);
    }

    /* 使能发送数据寄存器空中断并打开发送功能  */
    CR1_f = p_hw_uart->CR1_f;
    CR1_f.TE = 0x1;
    CR1_f.TXEIE = 0x1;
    u32Addr = (uint32_t)&CR1_f;
    p_hw_uart->CR1 = *(__IO uint32_t *)u32Addr;


    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __uart_callback_set(void *p_drv, int callback_type,
        void *pfn_callback, void *p_arg)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *) p_drv;

    switch (callback_type) {

    /* 设置发送回调函数中的获取发送字符回调函数 */
    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get = (am_uart_txchar_get_t) pfn_callback;
        p_dev->txget_arg = p_arg;
        return (AM_OK);

        /* 设置接收回调函数中的存放接收字符回调函数 */
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put = (am_uart_rxchar_put_t) pfn_callback;
        p_dev->rxput_arg = p_arg;
        return (AM_OK);

        /* 设置串口异常回调函数 */
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (am_uart_err_t) pfn_callback;
        p_dev->err_arg = p_arg;
        return (AM_OK);

    default:
        return (-AM_ENOTSUP);
    }
}

/**
 * \brief 轮询模式下发送一个字符
 */
static int __uart_poll_putchar(void *p_drv, char outchar)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *) p_drv;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;

    /* 发送模块是否空闲, AM_FALSE:忙; TURE: 空闲 */
    if (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
    AMHW_HC32F460_UART_TX_EMPTY_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    }
    else {
        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {
            /* 设置 485 为发送模式 */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }
        amhw_hc32f460_uart_tx_enable(p_hw_uart, AM_TRUE);

        /* 发送一个字符 */
        amhw_hc32f460_uart_data_write(p_hw_uart, outchar);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

            /* 等待发送完成 */
            while (!(p_hw_uart->SR_f.TC & 0x01))
                ;

            p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
        }
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 */
static int __uart_poll_getchar(void *p_drv, char *p_char)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *) p_drv;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;
    uint8_t *p_inchar = (uint8_t *) p_char;

    /* 接收模块是否空闲，AM_FALSE:忙,正在接收; TURE: 已经接收到一个字符 */
    if (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
    AMHW_HC32F460_UART_RX_VAL_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    }
    else {
        /* 接收一个字符 */
        *p_inchar = amhw_hc32f460_uart_data_read(p_hw_uart);
    }

    return (AM_OK);
}


void __uart_irq_tci_handler(void *p_arg) {
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *)p_arg;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;

    /* 关闭发送完成中断 */
    amhw_hc32f460_uart_int_disable(p_hw_uart,
                                   AMHW_HC32F460_UART_INT_TX_COMPLETE_ENABLE);

    /* 关闭发送功能 */
    amhw_hc32f460_uart_tx_enable(p_hw_uart, AM_FALSE);
}

/**
 * \brief 配置串口模式
 */
int __uart_mode_set(am_hc32f460_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {
#if 1
        uint8_t uart_id = p_dev->p_devinfo->dev_id;

        uint32_t enIRQ_EI  = p_dev->p_devinfo->inum_ei;
        uint32_t enIRQ_RI  = p_dev->p_devinfo->inum_ri;
        uint32_t enIRQ_TI  = p_dev->p_devinfo->inum_ti;
        uint32_t enIRQ_TCI = p_dev->p_devinfo->inum_tci;
        uint32_t enIRQ_RTO = p_dev->p_devinfo->inum_rto;

        uint32_t INT_USART_EI    = 278u + 5 * (uart_id - 1);
        uint32_t INT_USART_RI    = 279u + 5 * (uart_id - 1);
        uint32_t INT_USART_TI    = 280u + 5 * (uart_id - 1);
        uint32_t INT_USART_TCI   = 281u + 5 * (uart_id - 1);
        uint32_t INT_USART_RTO   = 282u + 5 * (uart_id - 1);

        stc_intc_sel_field_t *stcIntSel;

        stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_RI));
        stcIntSel->INTSEL = INT_USART_RI;
        am_int_connect(enIRQ_RI, __uart_irq_rx_handler, p_dev);
        am_int_enable(enIRQ_RI);

        stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_EI));
        stcIntSel->INTSEL = INT_USART_EI;
        am_int_connect(enIRQ_EI, NULL, NULL);
        am_int_enable(enIRQ_EI);

        stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_TI));
        stcIntSel->INTSEL = INT_USART_TI;
        am_int_connect(enIRQ_TI, __uart_irq_tx_handler, p_dev);
        am_int_enable(enIRQ_TI);

        stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_TCI));
        stcIntSel->INTSEL = INT_USART_TCI;
        am_int_connect(enIRQ_TCI, __uart_irq_tci_handler, p_dev);
        am_int_enable(enIRQ_TCI);

        stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * enIRQ_RTO));
        stcIntSel->INTSEL = INT_USART_RTO;
        am_int_connect(enIRQ_RTO, NULL, NULL);
        am_int_enable(enIRQ_RTO);
#endif


        /* 使能接收完成中断 */
        amhw_hc32f460_uart_int_enable(p_hw_uart,AMHW_HC32F460_UART_INT_RX_VAL_ENABLE);
    }
    else {

        /* 关闭所有串口中断 */
        amhw_hc32f460_uart_int_disable(p_hw_uart,
                AMHW_HC32F460_UART_INT_TIME_OUT_ENABLE    |\
                AMHW_HC32F460_UART_INT_RX_VAL_ENABLE      |\
                AMHW_HC32F460_UART_INT_TX_COMPLETE_ENABLE |\
                AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __uart_opt_set(am_hc32f460_uart_dev_t *p_dev, uint32_t options)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;
    uint8_t cfg_flags = 0;

    /* 在改变UART寄存器值前 接收发送禁能 */
    /* Set default value */
    p_hw_uart->CR1 = (uint32_t) 0xFFFFFFF3ul;
    p_hw_uart->CR1 = (uint32_t) 0x80000000ul;
    p_hw_uart->CR2 = (uint32_t) 0x00000000ul;
    p_hw_uart->CR3 = (uint32_t) 0x00000000ul;
    p_hw_uart->BRR = (uint32_t) 0x0000FFFFul;
    p_hw_uart->PR  = (uint32_t) 0x00000000ul;

    /* 配置数据长度 */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS5:
        cfg_flags |= AMHW_HC32F460_UART_DATA_5BIT;
        break;

    case AM_UART_CS6:
        cfg_flags |= AMHW_HC32F460_UART_DATA_6BIT;
        break;

    case AM_UART_CS7:
        cfg_flags |= AMHW_HC32F460_UART_DATA_7BIT;
        break;

    case AM_UART_CS8:
        cfg_flags |= AMHW_HC32F460_UART_DATA_8BIT;
        break;

    default:
        break;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_HC32F460_UART_STOP_2BIT;
    }
    else {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_HC32F460_UART_STOP_1BIT;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x3 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_HC32F460_UART_PARITY_ODD;
        }
        else {
            cfg_flags |= AMHW_HC32F460_UART_PARITY_EVEN;
        }
    }
    else {
        cfg_flags &= ~(0x3 << 0);
        cfg_flags |= AMHW_HC32F460_UART_PARITY_NO;
    }

    /* 保存和生效配置 */

    if (AMHW_HC32F460_UART_STOP_1BIT
            == (cfg_flags & AMHW_HC32F460_UART_STOP_1BIT)) {
        amhw_hc32f460_uart_stop_bit_sel(p_hw_uart, 0);
    }
    else if (AMHW_HC32F460_UART_STOP_2BIT
            == (cfg_flags & AMHW_HC32F460_UART_STOP_2BIT)) {
        amhw_hc32f460_uart_stop_bit_sel(p_hw_uart, 1);
    }

    if (AMHW_HC32F460_UART_DATA_8BIT
            == (cfg_flags & AMHW_HC32F460_UART_DATA_8BIT)) {
        /* hc32f460 only support 8(0) or 9(1) bits */
        amhw_hc32f460_uart_data_length(p_hw_uart, 0);
    }
    else if (AMHW_HC32F460_UART_DATA_9BIT
            == (cfg_flags & AMHW_HC32F460_UART_DATA_9BIT)) {
        /* hc32f460 only support 8(0) or 9(1) bits */
        amhw_hc32f460_uart_data_length(p_hw_uart, 1);
    }

    switch ((cfg_flags & 0x3)) {
    case AMHW_HC32F460_UART_PARITY_NO:
        p_hw_uart->CR1_f.PCE = (uint32_t) 0ul;
        break;
    case AMHW_HC32F460_UART_PARITY_EVEN:
        p_hw_uart->CR1_f.PS = (uint32_t) 0ul;
        p_hw_uart->CR1_f.PCE = (uint32_t) 1ul;
        break;
    case AMHW_HC32F460_UART_PARITY_ODD:
        p_hw_uart->CR1_f.PS = (uint32_t) 1ul;
        p_hw_uart->CR1_f.PCE = (uint32_t) 1ul;
        break;
    default:
        break;
    }

    p_hw_uart->CR3_f.CTSE  = (uint32_t) (0);  /* 0-RTS功能 1-CTS功能 */
    p_hw_uart->CR1_f.SBS   = (uint32_t) (1);  /* 1-接收数据RX管脚下降沿作为起始位*/
    p_hw_uart->CR1_f.OVER8 = (uint32_t) (1);  /* 1-8位UART过采样模式*/
    p_dev->options = options;
    return (AM_OK);
}

/*******************************************************************************
 UART interrupt request handler
 *******************************************************************************/

/**
 * \brief 串口接收中断服务
 */
void __uart_irq_rx_handler(void *p_arg)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *)p_arg;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;
    char data;

    data = amhw_hc32f460_uart_data_read(p_hw_uart);

    /* 存放新接收数据 */
    p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
}

/**
 * \brief 串口发送中断服务
 */
void __uart_irq_tx_handler(void *p_arg)
{
    am_hc32f460_uart_dev_t *p_dev = (am_hc32f460_uart_dev_t *)p_arg;
    amhw_hc32f460_uart_t *p_hw_uart =
            (amhw_hc32f460_uart_t *) p_dev->p_devinfo->uart_reg_base;
    char data;

    if (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
            AMHW_HC32F460_UART_TX_EMPTY_FLAG) == AM_TRUE) {


        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {

            amhw_hc32f460_uart_data_write(p_hw_uart, data);

        } else {
                /* 没有数据传送就关闭发送中断 */
                amhw_hc32f460_uart_int_disable(p_hw_uart,
                        AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE);

                /* 禁能485发送控制引脚 */
                if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir)) {

                    /* 设置 485 为接收模式 */
                    p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
                }
        }
    }
}


/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __uart_dummy_callback(void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief 串口初始化函数
 */
am_uart_handle_t am_hc32f460_uart_init(am_hc32f460_uart_dev_t *p_dev,
        const am_hc32f460_uart_devinfo_t *p_devinfo)
{
    amhw_hc32f460_uart_t *p_hw_uart;
    uint32_t tmp;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_uart = (amhw_hc32f460_uart_t *) p_devinfo->uart_reg_base;
    p_dev->p_devinfo = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *) &__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv = p_dev;
    p_dev->baud_rate = p_devinfo->baud_rate;
    p_dev->options = 0;

    /* 初始化默认回调函数 */
    p_dev->pfn_txchar_get = (int (*)(void *, char*)) __uart_dummy_callback;
    p_dev->txget_arg = NULL;
    p_dev->pfn_rxchar_put = (int (*)(void *, char)) __uart_dummy_callback;
    p_dev->rxput_arg = NULL;
    p_dev->pfn_err = (int (*)(void *, int, void*, int)) __uart_dummy_callback;
    p_dev->err_arg = NULL;

    p_dev->other_int_enable  = p_devinfo->other_int_enable  &
                               ~(AMHW_HC32F460_UART_INT_TX_EMPTY_ENABLE |
                                 AMHW_HC32F460_UART_INT_RX_VAL_ENABLE   |
                                 AMHW_HC32F460_UART_INT_TX_COMPLETE_ENABLE);

    p_dev->rs485_en = AM_FALSE;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }


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

    /* 获取串口检验方式配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 1) {
        p_dev->options |= AM_UART_PARENB | AM_UART_PARODD;
    }
    else {
    }

    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & (AMHW_HC32F460_UART_STOP_2BIT)) {
        p_dev->options |= AM_UART_STOPB;
    }
    else {

    }

    /* 等待上一次传输完成 */
    while (amhw_hc32f460_uart_status_flag_check(p_hw_uart,
            AMHW_HC32F460_UART_TX_COMPLETE_FALG) == AM_FALSE)
        ;

    __uart_opt_set(p_dev, p_dev->options);


    /* 设置波特率 */
    p_dev->baud_rate = amhw_hc32f460_uart_baudrate_set(p_hw_uart,
            am_clk_rate_get(p_dev->p_devinfo->clk_num), p_devinfo->baud_rate);
    /* 默认轮询模式 */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* uart 接收功能使能 */
    amhw_hc32f460_uart_rx_enable(p_hw_uart, AM_TRUE);
    amhw_hc32f460_uart_tx_enable(p_hw_uart, AM_TRUE);


    if (p_dev->p_devinfo->pfn_rs485_dir) {

        /* 初始化 485 为接收模式 */
        p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
    }

    return (am_uart_handle_t)&(p_dev->uart_serv);
}

/**
 * \brief 串口去初始化
 */
void am_hc32f460_uart_deinit(am_hc32f460_uart_dev_t *p_dev)
{
    p_dev->uart_serv.p_funcs = NULL;
    p_dev->uart_serv.p_drv = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {

        /* 默认为轮询模式 */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }

    am_int_disable(p_dev->p_devinfo->inum_ei);
    am_int_disable(p_dev->p_devinfo->inum_ri);
    am_int_disable(p_dev->p_devinfo->inum_ti);
    am_int_disable(p_dev->p_devinfo->inum_tci);
    am_int_disable(p_dev->p_devinfo->inum_rto);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
