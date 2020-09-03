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
#include "am_stm32f103rbt6_usart.h"
#include "am_delay.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_gpio.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

#define  __UART_CLK_RATE   24000000

/**
 * \brief 串口模式（查询或中断）设置
 */
int __usart_mode_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __usart_opt_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t opts);

/**
 * \brief 流控接受器流控状态设置
 */
static int __usart_flow_rxstat_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t ctrl);

/**
 * \brief 流控模式设置
 */
static int __usart_flow_mode_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t mode);

/**
 * \brief 流控发送器流控状态获取
 */
static int __usart_flow_txstat_get (am_stm32f103rbt6_usart_dev_t *p_dev);

/* STM32F103RBT6 串口驱动函数声明 */
static int __usart_ioctl (void *p_drv, int, void *);

static int __usart_tx_startup (void *p_drv);

static int __usart_callback_set (void *p_drv,
                                int   callback_type,
                                void *pfn_callback,
                                void *p_arg);

static int __usart_poll_getchar (void *p_drv, char *p_char);

static int __usart_poll_putchar (void *p_drv, char outchar);

#if 0
static int __usart_connect (void *p_drv);
#endif

static void __usart_irq_handler (void *p_arg);

/** \brief 标准层接口函数实现 */
static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
    __usart_ioctl,
    __usart_tx_startup,
    __usart_callback_set,
    __usart_poll_getchar,
    __usart_poll_putchar,
};

/******************************************************************************/

/**
 * \brief 设备控制函数
 *
 * 其中包含设置获取波特率，模式设置（中断/查询），获取支持的模式，硬件选项设置等功能。
 */
static int __usart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_stm32f103rbt6_usart_dev_t *p_dev     = (am_stm32f103rbt6_usart_dev_t *)p_drv;
    amhw_stm32f103rbt6_usart_t   *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    int status = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        /* 只有在当前传输完成的基础上才允许修改波特率 */
        while (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart, AMHW_STM32F103RBT6_USART_TX_COMPLETE_FLAG) == AM_FALSE);

        status = amhw_stm32f103rbt6_usart_baudrate_set(p_hw_usart,
                                               am_clk_rate_get(p_dev->p_devinfo->clk_num),
                                               (uint32_t)p_arg);

        if (status > 0) {

            /* 寄存器需要重新使能 UART 才能生效 */
            amhw_stm32f103rbt6_usart_disable(p_hw_usart);
            amhw_stm32f103rbt6_usart_enable(p_hw_usart);

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
        status = (__usart_mode_set(p_dev, (int)p_arg) == AM_OK)
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
        status = (__usart_opt_set(p_dev, (int)p_arg) == AM_OK) ? AM_OK : -AM_EIO;
        break;

    /* 串口选项获取 */
    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->options;
        break;

    /* 设置流控模式 */
    case AM_UART_FLOWMODE_SET:
        __usart_flow_mode_set(p_dev, (int)p_arg);
        break;

    /* 设置接收器流控状态 */
    case AM_UART_FLOWSTAT_RX_SET:
        __usart_flow_rxstat_set(p_dev, (int)p_arg);
        break;

    /* 获取发送器流控状态 */
    case AM_UART_FLOWSTAT_TX_GET:
        *(int *)p_arg = __usart_flow_txstat_get(p_dev);
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
int __usart_tx_startup (void *p_drv)
{
    char data  = 0;

    am_stm32f103rbt6_usart_dev_t *p_dev     = (am_stm32f103rbt6_usart_dev_t *)p_drv;
    amhw_stm32f103rbt6_usart_t   *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    /* 使能 485 发送控制引脚 */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 等待上一次传输完成 */
    while (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart, AMHW_STM32F103RBT6_USART_TX_COMPLETE_FLAG) == AM_FALSE);

    /* 获取发送数据并发送 */
    if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
        amhw_stm32f103rbt6_usart_data_write(p_hw_usart, data);
    }

    /* 使能发送中断 */
    amhw_stm32f103rbt6_usart_int_enable(p_hw_usart, AMHW_STM32F103RBT6_USART_INT_TX_EMPTY_ENABLE);
    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __usart_callback_set (void  *p_drv,
                                 int    callback_type,
                                 void  *pfn_callback,
                                 void  *p_arg)
{
    am_stm32f103rbt6_usart_dev_t *p_dev = (am_stm32f103rbt6_usart_dev_t *)p_drv;

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
static int __usart_poll_putchar (void *p_drv, char outchar)
{
    am_stm32f103rbt6_usart_dev_t *p_dev     = (am_stm32f103rbt6_usart_dev_t *)p_drv;
    amhw_stm32f103rbt6_usart_t   *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    /* 发送模块是否空闲, AM_FALSE:忙; TURE: 空闲 */
    if(amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart, AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {

            /* 设置 485 为发送模式 */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }

        while(amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,
                                                  AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG)
                                                  == AM_FALSE);

        /* 发送一个字符 */
        amhw_stm32f103rbt6_usart_data_write(p_hw_usart, outchar);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

            /* 等待发送完成 */
            while (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart, AMHW_STM32F103RBT6_USART_TX_COMPLETE_FLAG) == AM_FALSE);

            p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
        }
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 */
static int __usart_poll_getchar (void *p_drv, char *p_char)
{
    am_stm32f103rbt6_usart_dev_t *p_dev     = (am_stm32f103rbt6_usart_dev_t *)p_drv;
    amhw_stm32f103rbt6_usart_t   *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;
    uint8_t           *p_inchar  = (uint8_t *)p_char;

    /* 接收模块是否空闲，AM_FALSE:忙,正在接收; TURE: 已经接收到一个字符 */
    if(amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart, AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG) == AM_FALSE) {
        return (-AM_EAGAIN);
    } else {

        /* 接收一个字符 */
        *p_inchar = amhw_stm32f103rbt6_usart_data_read(p_hw_usart);
    }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 */
int __usart_mode_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    /* 仅支持以下模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }

    if (new_mode == AM_UART_MODE_INT) {

        am_int_connect(p_dev->p_devinfo->inum,
                       __usart_irq_handler,
                       (void *)p_dev);
        am_int_enable(p_dev->p_devinfo->inum);

        /* 使能接收准中断 */
        amhw_stm32f103rbt6_usart_int_enable(p_hw_usart, AMHW_STM32F103RBT6_USART_INT_RX_NOT_EMPTY_ENABLE);
    } else {

        /* 关闭所有串口中断 */
        amhw_stm32f103rbt6_usart_int_disable(p_hw_usart, AMHW_STM32F103RBT6_USART_INT_ALL_ENABLE_MASK);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __usart_opt_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t options)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;
    uint8_t             cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_STM32F103RBT6_USART_STOP_20_BIT;
    } else {
        cfg_flags &= ~(0x1 << 2);
        cfg_flags |= AMHW_STM32F103RBT6_USART_STOP_10_BIT;
    }

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {
        cfg_flags &= ~(0x3 << 0);

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_STM32F103RBT6_USART_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_STM32F103RBT6_USART_PARITY_EVEN;
        }
    } else {
        cfg_flags &= ~(0x3 << 0);
        cfg_flags |= AMHW_STM32F103RBT6_USART_PARITY_NO;
    }

    /* 保存和生效配置 */
    amhw_stm32f103rbt6_usart_stop_bit_sel(p_hw_usart, (cfg_flags & 0x4));
    amhw_stm32f103rbt6_usart_parity_bit_sel(p_hw_usart, (cfg_flags & 0x3));

    p_dev->options = options;

    return (AM_OK);
}

/******************************************************************************/
/**
 * \brief 接收器流控状态设置（开或关）
 */
static int __usart_flow_rxstat_set (am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t ctrl)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    if (AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        return -AM_ENOTSUP;

    /* 如果是硬件流控，配置RTS输出引脚电位 */
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            /* 拉低RTS管脚,开流     */
            am_gpio_set(p_dev->p_devinfo->gpio_rts[0], 0);

        } else {
            /* 拉高RTS管脚,限流     */
            am_gpio_set(p_dev->p_devinfo->gpio_rts[0], 1);
        }
    } else {
        /* 如果是软件流控，发送XON/XOFF流控字符 */
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            amhw_stm32f103rbt6_usart_data_write(p_hw_usart, AM_STM32F103RBT6_USART_XON);
        } else {
            amhw_stm32f103rbt6_usart_data_write(p_hw_usart, AM_STM32F103RBT6_USART_XOFF);
        }
    }

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 流控模式设置（无流控，软件流控，硬件流控）
 */
static int __usart_flow_mode_set(am_stm32f103rbt6_usart_dev_t *p_dev, uint32_t mode)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;
    p_dev->flowctl_mode = mode;

    if(AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_ON;
        amhw_stm32f103rbt6_usart_rts_disable(p_hw_usart);
        amhw_stm32f103rbt6_usart_cts_disable(p_hw_usart);
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_rts[0], AM_GPIO_INPUT);
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_cts[0], AM_GPIO_INPUT);
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* 硬件流控使能     */
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_rts[0], p_dev->p_devinfo->gpio_rts[1]);
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_cts[0], p_dev->p_devinfo->gpio_cts[1]);
        amhw_stm32f103rbt6_usart_rts_enable(p_hw_usart);
        amhw_stm32f103rbt6_usart_cts_enable(p_hw_usart);
    } else {
        amhw_stm32f103rbt6_usart_rts_disable(p_hw_usart);
        amhw_stm32f103rbt6_usart_cts_disable(p_hw_usart);
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_rts[0], AM_GPIO_INPUT);
        am_gpio_pin_cfg(p_dev->p_devinfo->gpio_cts[0], AM_GPIO_INPUT);
    }

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 发送器流控状态获取
 */
static int __usart_flow_txstat_get (am_stm32f103rbt6_usart_dev_t *p_dev)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = NULL;

    if (NULL == p_dev) {
        return -AM_EINVAL;
    }

    p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    if(AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* cts的状态发生变化时，状态位有效 */
        if(amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,AMHW_STM32F103RBT6_USART_CTS_FLAG) == AM_TRUE) {

            /* 清除cts状态位 */
            amhw_stm32f103rbt6_usart_status_flag_clr(p_hw_usart,AMHW_STM32F103RBT6_USART_CTS_FLAG);

            /* 判断cts当前高低电平 */
            if (am_gpio_get(p_dev->p_devinfo->gpio_cts[0]) == 0) {
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
  USART interrupt request handler
*******************************************************************************/

/**
 * \brief 串口接收中断服务
 */
void __usart_irq_rx_handler (am_stm32f103rbt6_usart_dev_t *p_dev)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;
    char data;

    /* 是否为接收Rx中断 */
    if (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG) == AM_TRUE) {

        amhw_stm32f103rbt6_usart_status_flag_clr(p_hw_usart, AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG);

        /* 获取新接收数据 */
        data = amhw_stm32f103rbt6_usart_data_read(p_hw_usart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 */
void __usart_irq_tx_handler (am_stm32f103rbt6_usart_dev_t *p_dev)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    char data;

    if (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG) == AM_TRUE) {

        amhw_stm32f103rbt6_usart_status_flag_clr(p_hw_usart, AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG);
        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_stm32f103rbt6_usart_data_write(p_hw_usart, data);
        } else {

            /* 没有数据传送就关闭发送中断 */
            amhw_stm32f103rbt6_usart_int_disable(p_hw_usart, AMHW_STM32F103RBT6_USART_INT_TX_EMPTY_ENABLE);

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
void __usart_irq_handler (void *p_arg)
{
    am_stm32f103rbt6_usart_dev_t  *p_dev     = (am_stm32f103rbt6_usart_dev_t *)p_arg;
    amhw_stm32f103rbt6_usart_t    *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;

    uint32_t usart_int_stat        = p_hw_usart->sr;

    if (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG) == AM_TRUE) {
         __usart_irq_rx_handler(p_dev);
    } else if (amhw_stm32f103rbt6_usart_status_flag_check(p_hw_usart,AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG) == AM_TRUE) {
        __usart_irq_tx_handler(p_dev);
    } else {

    }

    /* 其他中断 */
    if ((p_dev->other_int_enable & usart_int_stat) != 0) {

        usart_int_stat &= p_dev->other_int_enable;

        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->err_arg,
                           AM_STM32F103RBT6_USART_ERRCODE_USART_OTHER_INT,
                           (void *)p_hw_usart,
                           1);
        }
    }

}

#if 0
/**
 * \brief UART中断函数连接，仅使用中断模式时需要调用此函数
 */
int __usart_connect (void *p_drv)
{
    am_stm32f103rbt6_usart_dev_t *p_dev = (am_stm32f103rbt6_usart_dev_t *)p_drv;

    /* 关联中断向量号，开启中断 */
    am_int_connect(p_dev->p_devinfo->inum, __usart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_stm32f103rbt6_usart_int_enable(p_dev->p_devinfo->usart_reg_base,
                                p_dev->other_int_enable);

    return AM_OK;
}
#endif /* 0 */

/**
 * \brief 默认回调函数
 *
 * \returns AW_ERROR
 */
static int __usart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/**
 * \brief 串口初始化函数
 */
am_uart_handle_t am_stm32f103rbt6_usart_init (am_stm32f103rbt6_usart_dev_t           *p_dev,
                                       const am_stm32f103rbt6_usart_devinfo_t *p_devinfo)
{
    amhw_stm32f103rbt6_usart_t  *p_hw_usart;
    uint32_t              tmp;

    if (p_devinfo == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_usart                = (amhw_stm32f103rbt6_usart_t  *)p_devinfo->usart_reg_base;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv   = p_dev;
    p_dev->baud_rate         = p_devinfo->baud_rate;
    p_dev->options           = 0;

    /* 初始化默认回调函数 */
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__usart_dummy_callback;
    p_dev->txget_arg         = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__usart_dummy_callback;
    p_dev->rxput_arg         = NULL;
    p_dev->pfn_err           = (int (*) (void *, int, void*, int))
                               __usart_dummy_callback;

    p_dev->err_arg           = NULL;

    p_dev->flowctl_mode      = AM_UART_FLOWCTL_NO;
    p_dev->flowctl_tx_stat   = AM_UART_FLOWSTAT_ON;

    p_dev->other_int_enable  = p_devinfo->other_int_enable  &
                               ~(AMHW_STM32F103RBT6_USART_TX_EMPTY_FLAG |
                                 AMHW_STM32F103RBT6_USART_RX_NOT_EMPTY_FLAG);
    p_dev->rs485_en          = AM_FALSE;

    /* 开启UART时钟 */
    am_clk_enable(p_devinfo->clk_num);

    /* 在改变UART寄存器值前禁能 */
    amhw_stm32f103rbt6_usart_disable(p_hw_usart);

    /* 获取串口数据长度配置选项,并进行设置 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 4) & 0x01;

    switch (tmp) {

    case 0:
        amhw_stm32f103rbt6_usart_word_length_sel(p_hw_usart, AMHW_STM32F103RBT6_USART_DATA_8BIT);
        break;

    default:
        amhw_stm32f103rbt6_usart_word_length_sel(p_hw_usart, AMHW_STM32F103RBT6_USART_DATA_8BIT);
        break;
    }

    /* 获取串口检验方式配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 0) & 0x03;

    if (tmp == 3) {
        p_dev->options |= AM_UART_PARENB | AM_UART_PARODD;
    } else {
    }

    /* 获取串口停止位配置选项 */
    tmp = p_devinfo->cfg_flags;
    tmp = (tmp >> 2) & 0x03;
    if (tmp == 3) {
        p_dev->options |= AM_UART_STOPB;
    } else {

    }

    __usart_opt_set(p_dev, p_dev->options);

    /* 设置波特率 */
    p_dev->baud_rate = amhw_stm32f103rbt6_usart_baudrate_set(
        p_hw_usart,
        am_clk_rate_get(p_dev->p_devinfo->clk_num),
        p_devinfo->baud_rate);

    /* 默认轮询模式 */
    __usart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* 流控模式设置（无流控，软件流控，硬件流控）*/
    __usart_flow_mode_set(p_dev, AM_UART_FLOWCTL_NO);

    /* usart使能 */
    amhw_stm32f103rbt6_usart_receiver_enable(p_hw_usart);
    amhw_stm32f103rbt6_usart_transmitter_enable(p_hw_usart);
    amhw_stm32f103rbt6_usart_enable(p_hw_usart);

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
void am_stm32f103rbt6_usart_deinit (am_stm32f103rbt6_usart_dev_t *p_dev)
{
    amhw_stm32f103rbt6_usart_t *p_hw_usart = (amhw_stm32f103rbt6_usart_t *)p_dev->p_devinfo->usart_reg_base;
    p_dev->uart_serv.p_funcs   = NULL;
    p_dev->uart_serv.p_drv     = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {

        /* 默认为轮询模式 */
        __usart_mode_set(p_dev, AM_UART_MODE_POLL);
    }

    /* 关闭串口 */
    amhw_stm32f103rbt6_usart_disable(p_hw_usart);

    am_int_disable(p_dev->p_devinfo->inum);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
