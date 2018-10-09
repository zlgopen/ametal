/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief USART drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-03  bob, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_lpc_usart.h"
#include "hw/amhw_lpc_usart.h"
#include "am_clk.h"

#define __USART_BAUDRATE    115200  /* USART模块默认波特率  */

/*******************************************************************************
* 函数声明
*******************************************************************************/

/** 
 * \brief 串口模式（查询或中断）设置
 */
static int __usart_mode_set (am_lpc_usart_dev_t *p_dev, uint32_t new_mode);

/** 
 * \brief 串口硬件设置
 */
static int __usart_opt_set (am_lpc_usart_dev_t *p_dev, uint32_t opts);

/** 
 * \brief 流控接受器流控状态设置
 */
static int __usart_flow_rxstat_set (am_lpc_usart_dev_t *p_dev, uint32_t ctrl);

/** 
 * \brief 流控模式设置
 */
static int __usart_flow_mode_set (am_lpc_usart_dev_t *p_dev, uint32_t mode);

/** 
 * \brief 流控发送器流控状态获取
 */
static int __usart_flow_txstat_get (am_lpc_usart_dev_t *p_dev);

/* lpc824X UART 设备服务 */
static int __usart_ioctl (void *p_drv,int request, void *p_arg);

static int __usart_tx_startup (void *p_drv);

static int __usart_callback_set (void  *p_drv,
                                 int    callback_type,
                                 void  *pfn_callback,
                                 void  *p_arg);
                                     
static int __usart_poll_getchar (void *p_drv, char *p_char);

static int __usart_poll_putchar (void *p_drv, char outchar);

/** \brief uart 功能函数结构体变量 */
static const struct am_uart_drv_funcs __g_uart_drv_funcs = {
     __usart_ioctl,
     __usart_tx_startup,
     __usart_callback_set,
     __usart_poll_getchar,
     __usart_poll_putchar
};

/*******************************************************************************/

/**
 * \brief 设备控制函数
 *
 * 其中包含设置获取波特率，模式设置（中断/查询），获取支持的模式，硬件选项设置，
 * 流控模式选择，流控设置等特殊功能。
 *
 */
static int __usart_ioctl (void *p_drv, int request, void *p_arg)
{
    int status = AM_OK;

    am_lpc_usart_dev_t   *p_dev      = NULL;
    amhw_lpc_usart_t     *p_hw_usart = NULL;
  
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    
    p_dev      = (am_lpc_usart_dev_t *)p_drv;
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    
    switch (request) {
        
    case AM_UART_BAUD_SET:
    
        /* 等待串口发送空闲 */
        while ((amhw_lpc_usart_stat_get(p_hw_usart) &
                AMHW_LPC_USART_STAT_TXIDLE) == 0);
    
        /* 设置串口波特率 */
        status = amhw_lpc_usart_baudrate_set(
                                       p_hw_usart,
                     am_clk_rate_get(p_dev->p_devinfo->clk_id),
                                       (uint32_t)p_arg);
        if (status > 0) {
            p_dev->baud_rate = status;
            status = AM_OK;
        } else {
            status = -AM_EIO;
        }
        break;

    case AM_UART_BAUD_GET:
        *(int *)p_arg = p_dev->baud_rate;
        break;

    case AM_UART_MODE_SET:
        status = (__usart_mode_set(p_dev, (int)p_arg) == AM_OK)
                 ? AM_OK : -AM_EIO;
        break;

    case AM_UART_MODE_GET:
        *(int *)p_arg = p_dev->channel_mode;
        break;

    case AM_UART_AVAIL_MODES_GET:
        *(int *)p_arg = AM_UART_MODE_INT | AM_UART_MODE_POLL;
        break;

    case AM_UART_OPTS_SET:
        status = (__usart_opt_set(p_dev, (int)p_arg) == AM_OK) ?
                  AM_OK : -AM_EIO;
        break;

    case AM_UART_OPTS_GET:
        *(int *)p_arg = p_dev->options;
        break;
    
    case AM_UART_FLOWMODE_SET:
        __usart_flow_mode_set(p_dev, (int)p_arg);
        break;
    
    case AM_UART_FLOWSTAT_RX_SET:
        __usart_flow_rxstat_set(p_dev, (int)p_arg);
        break;
    
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

/******************************************************************************/
/** 
 * \brief 启动传输（在中断模式下）
 */
static int __usart_tx_startup (void *p_drv)
{
    am_lpc_usart_dev_t *p_dev      = NULL;
    amhw_lpc_usart_t   *p_hw_usart = NULL;
  
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    
    p_dev      = (am_lpc_usart_dev_t *)p_drv;
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;

    /* RS485 mode */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

        /* set to transmit mode */
        p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
    }

    /* 使能发送中断 */
    amhw_lpc_usart_int_enable(p_hw_usart, AMHW_LPC_USART_INT_TXRDY);
    
    /* RS485 mode */
    if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

        /* Enable the Tx Idle interrupt */
        amhw_lpc_usart_int_enable(p_hw_usart, AMHW_LPC_USART_INT_TXIDLE);
    }

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 设置回调函数
 */
static int __usart_callback_set (void  *p_drv,
                                 int    callback_type,
                                 void  *pfn_callback,
                                 void  *p_arg)
{
    
    am_lpc_usart_dev_t *p_dev = NULL;
  
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    
    p_dev = (am_lpc_usart_dev_t *)p_drv;
    
    switch (callback_type) {

    case AM_UART_CALLBACK_TXCHAR_GET:
        p_dev->pfn_txchar_get = (am_uart_txchar_get_t)pfn_callback;
        p_dev->p_txget_arg      = p_arg;
        return (AM_OK);
    
    case AM_UART_CALLBACK_RXCHAR_PUT:
        p_dev->pfn_rxchar_put = (am_uart_rxchar_put_t)pfn_callback;
        p_dev->p_rxput_arg      = p_arg;
        return (AM_OK);
    
    case AM_UART_CALLBACK_ERROR:
        p_dev->pfn_err = (am_uart_err_t)pfn_callback;
        p_dev->p_err_arg = p_arg;
        return (AM_OK);
        
    default:
        return (-AM_ENOTSUP);
    }
}

/******************************************************************************/
/**
 * \brief 发送一个字符（查询模式）
 */
static int __usart_poll_putchar (void *p_drv, char outchar)
{
    am_lpc_usart_dev_t *p_dev      = NULL;
    amhw_lpc_usart_t   *p_hw_usart = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev      = (am_lpc_usart_dev_t *)p_drv;
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
        
    /* 发送非就绪或软流控模式下流控关状态 */
    if ((amhw_lpc_usart_stat_get(p_hw_usart) & 
         AMHW_LPC_USART_STAT_TXRDY) == 0 ||
            (AM_UART_FLOWSTAT_OFF == p_dev->flowctl_tx_stat &&
             AM_UART_FLOWCTL_SW == p_dev->flowctl_mode)) {
        return (-AM_EAGAIN); 
    } else {
        
        if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir != NULL)) {
            /* set to transmit mode */
            p_dev->p_devinfo->pfn_rs485_dir(AM_TRUE);
        }

        /* 发送一个数据 */
        amhw_lpc_usart_txdata_write(p_hw_usart, outchar);

        if (p_dev->rs485_en && p_dev->p_devinfo->pfn_rs485_dir) {

            /* wait for the tx idle */
            while (!(p_hw_usart->stat & AMHW_LPC_USART_STAT_TXIDLE));

            p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
        }
    }

    return (AM_OK);
}

/******************************************************************************/
/**
 * \brief 获取一个字符（查询模式）
 */
static int __usart_poll_getchar (void *p_drv, char *p_char)
{
    uint8_t               *p_inchar   = NULL;
    am_lpc_usart_dev_t *p_dev      = NULL;
    amhw_lpc_usart_t   *p_hw_usart = NULL;

  
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }
    
    p_inchar   = (uint8_t *)p_char;
    p_dev      = (am_lpc_usart_dev_t *)p_drv;
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;

    /* 接收未就绪 */
    if ((amhw_lpc_usart_stat_get(p_hw_usart) &
         AMHW_LPC_USART_STAT_RXRDY) == 0) {
        return (-AM_EAGAIN);
    } else {
        *p_inchar = amhw_lpc_usart_rxdata_read(p_hw_usart);
        
        /* 如果是软流控模式，检查接收的数据是否是XON/XOFF控制信号 */
        if (AM_UART_FLOWCTL_SW == p_dev->flowctl_mode) {
            if (*p_inchar == AM_LPC_UART_XON) {
                p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_ON;
            } else if (*p_inchar == AM_LPC_UART_XOFF) {
                p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_OFF;
            } else {
            }
        }
    }
    
    return (AM_OK);
}

/******************************************************************************/
/**
 * \brief usart 模式设置
 */
static int __usart_mode_set (am_lpc_usart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_lpc_usart_t *p_hw_usart = NULL;
  
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;

    /* 只支持中断模式和查询模式 */
    if ((new_mode != AM_UART_MODE_POLL) && (new_mode != AM_UART_MODE_INT)) {
        return (AM_ERROR);
    }
    
    if (new_mode == AM_UART_MODE_INT) {
            
       /* 使能接收中断 */
       amhw_lpc_usart_int_enable(p_hw_usart, AMHW_LPC_USART_INT_RXRDY);
    } else {
        
       /* 禁能所有中断 */
       amhw_lpc_usart_int_disable(p_hw_usart, AMHW_LPC_USART_INT_ALL);
    }
    
    p_dev->channel_mode = new_mode;

    return AM_OK;
}

/******************************************************************************/
/**
 * \brief usart 硬件设置
 */
static int __usart_opt_set (am_lpc_usart_dev_t *p_dev, uint32_t options)
{
    amhw_lpc_usart_t *p_hw_usart;
    uint32_t      cfg_flags;
    
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    cfg_flags  = p_hw_usart->cfg;
    
    switch (options & AM_UART_CSIZE) {
        
    case AM_UART_CS7:
        AM_BIT_CLR_MASK(cfg_flags, AM_SBF(0x03, 2));
        AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_7BIT);
        break;
        
    case AM_UART_CS8:
        AM_BIT_CLR_MASK(cfg_flags, AM_SBF(0x03, 2));
        AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_8BIT);
        break;
        
    default:
        break;
    }

    /* 停止位设置 */
    if (options & AM_UART_STOPB) {
        AM_BIT_CLR_MASK(cfg_flags, AM_BIT(6));
        AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_STOP_2);
    } else {
        AM_BIT_CLR_MASK(cfg_flags, AM_BIT(6));
        AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_STOP_1);
    }

    /* 奇偶校验位设置 */        
    if (options & AM_UART_PARENB) {
        AM_BIT_CLR_MASK(cfg_flags, AM_SBF(0x03, 4));

        if (options & AM_UART_PARODD) {    
            AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_PARITY_ODD);
        } else {   
            AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_PARITY_EVEN);
        }
    } else {
        AM_BIT_CLR_MASK(cfg_flags, AM_SBF(0x03, 4));
        AM_BIT_SET_MASK(cfg_flags, AMHW_LPC_USART_CFG_PARITY_NO);
    }

    if ((p_hw_usart->cfg & AMHW_LPC_USART_CFG_ENABLE) == 1) {
        /* 等待空闲 */
        while((amhw_lpc_usart_stat_get(p_hw_usart) & 
               AMHW_LPC_USART_STAT_TXIDLE) == 0);
        amhw_lpc_usart_disable(p_hw_usart);
    }
    
    p_hw_usart->cfg = cfg_flags;
    amhw_lpc_usart_enable(p_hw_usart);
    
    p_dev->options = options;
    
    return (AM_OK);
}

/******************************************************************************/
/**
 * \brief 接收器流控状态设置（开或关）
 */
static int __usart_flow_rxstat_set (am_lpc_usart_dev_t *p_dev, uint32_t ctrl)
{
    amhw_lpc_usart_t *p_hw_usart = NULL;
  
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    
    if (AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        return -AM_ENOTSUP;
      
    /* 如果是硬件流控，配置RTS输出引脚电位 */
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            /* 拉低RTS管脚,开流     */
            amhw_lpc_usart_cfg_clr(p_hw_usart, 
                                      AMHW_LPC_USART_CFG_OEPOLHIGH);

        } else {
            /* 拉高RTS管脚,限流     */
            amhw_lpc_usart_cfg_set(p_hw_usart, 
                                      AMHW_LPC_USART_CFG_OEPOLHIGH);
        }
    } else {
        /* 如果是软件流控，发送XON/XOFF流控字符 */
        if (AM_UART_FLOWSTAT_ON == ctrl) {
            amhw_lpc_usart_txdata_write(p_hw_usart, AM_LPC_UART_XON);
        } else {
            amhw_lpc_usart_txdata_write(p_hw_usart, AM_LPC_UART_XOFF);
        } 
    }
    
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 流控模式设置（无流控，软件流控，硬件流控）
 */
static int __usart_flow_mode_set(am_lpc_usart_dev_t *p_dev, uint32_t mode)
{
    amhw_lpc_usart_t *p_hw_usart = NULL;
  
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    p_dev->flowctl_mode = mode;
  
    if(AM_UART_FLOWCTL_NO == p_dev->flowctl_mode) {
        p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_ON;
        amhw_lpc_usart_cfg_clr(p_hw_usart, AMHW_LPC_USART_CFG_HWFLOW);
        amhw_lpc_usart_cfg_clr(p_hw_usart, AMHW_LPC_USART_CFG_OESEL);
    } else if (AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* 硬件流控使能     */
        amhw_lpc_usart_cfg_set(p_hw_usart, AMHW_LPC_USART_CFG_HWFLOW);
        amhw_lpc_usart_cfg_set(p_hw_usart, AMHW_LPC_USART_CFG_OESEL);
    } else {
        amhw_lpc_usart_cfg_clr(p_hw_usart, AMHW_LPC_USART_CFG_HWFLOW);
        amhw_lpc_usart_cfg_clr(p_hw_usart, AMHW_LPC_USART_CFG_OESEL);
    }
    
    return AM_OK;
}

/******************************************************************************/
/**
 * \brief 发送器流控状态获取
 */
static int __usart_flow_txstat_get (am_lpc_usart_dev_t *p_dev)
{
    amhw_lpc_usart_t *p_hw_usart = NULL;
  
    if (NULL == p_dev) {
        return -AM_EINVAL;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    
    if(AM_UART_FLOWCTL_HW == p_dev->flowctl_mode) {

        /* 硬件流控判断cts的状态     */
        if((AMHW_LPC_USART_STAT_CTS &
            amhw_lpc_usart_stat_get(p_hw_usart)) == 0) {
            return (int)AM_UART_FLOWSTAT_ON;
        } else {
            return (int)AM_UART_FLOWSTAT_OFF;
        }
    } else {
        return (int)p_dev->flowctl_tx_stat;
    }
}

/******************************************************************************/
/**
 * \brief 接收中断（接收器接收准备就绪）
 */

static void __usart_irq_rx_handler (am_lpc_usart_dev_t *p_dev)
{
    amhw_lpc_usart_t *p_hw_usart = NULL;
    uint32_t      int_stat   = 0;
    char data;

    if (NULL == p_dev) {
        return ;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    int_stat   = amhw_lpc_usart_intstat_get(p_hw_usart);

    
    /* 检查中断状态寄存器是否有接收中断 */
    if ((int_stat & AMHW_LPC_USART_INTSTAT_RXRDY) != 0) {
        
        data = amhw_lpc_usart_rxdata_read(p_hw_usart);
        
        p_dev->pfn_rxchar_put(p_dev->p_rxput_arg, data);
      
        /* 如果是软件流控模式，检查接受到数据是否是XON/XOFF信号 */
        if (AM_UART_FLOWCTL_SW == p_dev->flowctl_mode) {
            if (data == AM_LPC_UART_XON) {
                p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_ON;
            } else if (data == AM_LPC_UART_XOFF) {
                p_dev->flowctl_tx_stat = AM_UART_FLOWSTAT_OFF;
            } else {
                
            }
        }
    }
    
    return ;
}

/******************************************************************************/
/**
 * \brief 发送中断（发送器发送准备就绪）
 */
static void __usart_irq_tx_handler (am_lpc_usart_dev_t *p_dev)
{

    char     data;
    uint32_t int_stat;

    amhw_lpc_usart_t *p_hw_usart = NULL;
  
    if (NULL == p_dev) {
        return ;
    }
    
    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    int_stat   = amhw_lpc_usart_intstat_get(p_hw_usart);
  
    if (AM_UART_FLOWCTL_SW    == p_dev->flowctl_mode &&
        AM_UART_FLOWSTAT_OFF  == p_dev->flowctl_tx_stat
        ) {

    } else if (((int_stat & AMHW_LPC_USART_INTSTAT_TXRDY)  != 0) ||
               ((int_stat & AMHW_LPC_USART_INTSTAT_TXIDLE) != 0) ) {

        if ((p_dev->pfn_txchar_get(p_dev->p_txget_arg, &data)) == AM_OK) {
            amhw_lpc_usart_txdata_write(p_hw_usart, data);
        } else {

            /* 数据发送完关发送就绪中断 */
            amhw_lpc_usart_int_disable(p_hw_usart, 
                                          AMHW_LPC_USART_INT_TXRDY);


            if (int_stat & AMHW_LPC_USART_INTSTAT_TXIDLE) {

                if ((p_dev->rs485_en) && (p_dev->p_devinfo->pfn_rs485_dir)) {

                    /* set to receive mode */
                    p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
                }

                amhw_lpc_usart_int_disable(p_hw_usart,
                                              AMHW_LPC_USART_INT_TXIDLE);
            }
        }
    }
    
    return ;
}

/******************************************************************************/
/**
 * \brief usart 中断函数
 */
static void __usart_irq_handler (void *p_arg)
{
    am_lpc_usart_dev_t *p_dev      = NULL;
    amhw_lpc_usart_t   *p_hw_usart = NULL;

    uint32_t usart_int_stat;
  
    if (NULL == p_arg) {
        return ;
    }
    
    p_dev          = (am_lpc_usart_dev_t *)p_arg;
    p_hw_usart     = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    usart_int_stat = amhw_lpc_usart_intstat_get(p_hw_usart);
  
    if (usart_int_stat & AMHW_LPC_USART_INTSTAT_RXRDY) {

        __usart_irq_rx_handler(p_dev);

    }

    if (usart_int_stat &
        (AMHW_LPC_USART_INTSTAT_TXRDY | AMHW_LPC_USART_INTSTAT_TXIDLE)) {
                   
        __usart_irq_tx_handler(p_dev);
    }
    
    /* 其他的中断 */
    if ((p_dev->other_int_enable & usart_int_stat) != 0) {

        usart_int_stat &= p_dev->other_int_enable;
        
        if (p_dev->pfn_err != NULL) {
            p_dev->pfn_err(p_dev->p_err_arg,
                           AM_LPC_UART_ERRCODE_USART_OTHER_INT,
                           (void *)usart_int_stat,
                           1);
        }
    }
}

/******************************************************************************/
/**
 * \brief 错误回调函数
 */
static int __usart_dummy_callback (void *p_arg, char *p_outchar)
{
    return (AM_ERROR);
}

/*******************************************************************************
    Public Functions
*******************************************************************************/

/**
 * \brief usart 模块初始化
 */
am_uart_handle_t am_lpc_usart_init (am_lpc_usart_dev_t     *p_dev,
                                    const am_lpc_usart_devinfo_t *p_devinfo)
{
    uint32_t       tmp;

    /* 默认USART配置（8位数据，无奇偶校验，一位停止位） */
    uint32_t       usart_cfg_flags = AMHW_LPC_USART_CFG_8BIT      |
                                     AMHW_LPC_USART_CFG_PARITY_NO |
                                     AMHW_LPC_USART_CFG_STOP_1;

    amhw_lpc_usart_t  *p_hw_usart = NULL;
    
    if ((NULL == p_dev) || (NULL == p_devinfo )) {
        return NULL;
    }


    p_hw_usart               = (amhw_lpc_usart_t *)p_devinfo->usart_regbase;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->uart_serv.p_funcs = (struct am_uart_drv_funcs *)&__g_uart_drv_funcs;
    p_dev->uart_serv.p_drv   = p_dev; 
    p_dev->baud_rate         = __USART_BAUDRATE;
    p_dev->options           = 0;    
    
    p_dev->pfn_txchar_get    = (int (*) (void *, char*))__usart_dummy_callback;
    p_dev->p_txget_arg       = NULL;
    p_dev->pfn_rxchar_put    = (int (*) (void *, char ))__usart_dummy_callback;
    p_dev->p_rxput_arg       = NULL;
    p_dev->pfn_err           = (int (*) (void *, int, void*, int))
                                __usart_dummy_callback;
    p_dev->p_err_arg         = NULL;
    
    p_dev->flowctl_mode      = AM_UART_FLOWCTL_NO;
    p_dev->flowctl_tx_stat   = AM_UART_FLOWSTAT_ON;
    p_dev->other_int_enable  = 0;
    p_dev->rs485_en          = AM_FALSE;

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }
    
    if (p_dev->p_devinfo->pfn_rs485_dir) {
        /* set to receive mode */
        p_dev->p_devinfo->pfn_rs485_dir(AM_FALSE);
    }

    tmp = usart_cfg_flags;
    tmp = (tmp >> 2) & 0x03;
    
    switch (tmp) {
        
    case 0:
        p_dev->options |= AM_UART_CS7;
        break;
        
    case 1:
        p_dev->options |= AM_UART_CS8;
        break;
        
    default:
        p_dev->options |= AM_UART_CS8;
        break;
    }

    tmp = usart_cfg_flags;
    tmp = (tmp >> 4) & 0x03;
    
    if (tmp == 2) {
        p_dev->options |= AM_UART_PARENB;
    } else if (tmp == 3) {
        p_dev->options |= (AM_UART_PARENB | AM_UART_PARODD);
    }
    
    if (usart_cfg_flags & AMHW_LPC_USART_CFG_STOP_2) {
        p_dev->options |= AM_UART_STOPB;
    }
    
    amhw_lpc_usart_config(p_hw_usart, usart_cfg_flags);
    amhw_lpc_usart_control(p_hw_usart, 0);
    
    p_dev->baud_rate = amhw_lpc_usart_baudrate_set(
                                    p_hw_usart,
                  am_clk_rate_get(p_dev->p_devinfo->clk_id),
                                    __USART_BAUDRATE);
  
    __usart_mode_set(p_dev, AM_UART_MODE_POLL); 
    __usart_flow_mode_set (p_dev, AM_UART_FLOWCTL_NO);
    
    am_int_connect(p_dev->p_devinfo->inum, 
                   __usart_irq_handler,
                   (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    
    amhw_lpc_usart_enable(p_hw_usart);

    return &(p_dev->uart_serv);
}

/******************************************************************************/
/**
 * \brief usart 模块解初始化
 */
void am_lpc_usart_deinit (am_uart_handle_t handle)
{
    am_lpc_usart_dev_t *p_dev    = (am_lpc_usart_dev_t *)handle;
    amhw_lpc_usart_t *p_hw_usart = NULL;
    
    if (NULL == p_dev) {
        return ;
    }

    p_hw_usart = (amhw_lpc_usart_t *)p_dev->p_devinfo->usart_regbase;
    
    p_dev->uart_serv.p_funcs = NULL;
    p_dev->uart_serv.p_drv   = NULL;
        
    if (p_dev->channel_mode == AM_UART_MODE_INT) {
        __usart_mode_set(p_dev, AM_UART_MODE_POLL);  
    }

    amhw_lpc_usart_disable(p_hw_usart);
    
    am_int_disable(p_dev->p_devinfo->inum);
    
    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}


/* end of file */
