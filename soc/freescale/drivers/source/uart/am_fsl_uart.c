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
 * \par Modification history.
 * - 1.04 18-12-28 zcb, modified errors in data width and parity type settings.
 * - 1.03 18-03-16 sdq, fix the bug when uart bits set.
 * - 1.02 18-01-22 sdq, am_fsl_uart_deinit call to pfn_plfm_deinit is added.
 *                      call to amhw_fsl_uart_disable instead of
 *                      amhw_fsl_uart_enable when closing serial port.
 * - 1.01 16-09-16 nwt, fix the bug when uart tx/rx data by polling.
 * - 1.00 15-10-20 xym, first implementation.
 * \endinternal
 */
#include "am_fsl_uart.h"
#include "am_int.h"
#include "am_clk.h"

/*******************************************************************************
* Functions declaration
*******************************************************************************/

/**
 * \brief 串口模式（查询或中断）设置
 */
int __uart_mode_set (am_fsl_uart_dev_t *p_dev, uint32_t new_mode);

/**
 * \brief 串口硬件设置
 */
int __uart_opt_set (am_fsl_uart_dev_t *p_dev, uint32_t opts);


/* Kl26 串口驱动函数声明 */
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
 *
 */
static int __uart_ioctl (void *p_drv, int request, void *p_arg)
{
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    int              status    = AM_OK;

    switch (request) {

    /* 波特率设置 */
    case AM_UART_BAUD_SET:

        if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
            status = amhw_fsl_uart_ver0_baudrate_set(p_dev->p_devinfo->p_hw_uart,
                                     am_clk_rate_get(p_dev->p_devinfo->clk_id),
                                     (uint32_t)p_arg);
        }else{
            status = amhw_fsl_uart_ver1_baudrate_set(p_dev->p_devinfo->p_hw_uart,
                                      am_clk_rate_get(p_dev->p_devinfo->clk_id),
                                     (uint32_t)p_arg);
        }


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
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t     *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    
    /* 使能485发送控制引脚 */
    if (p_dev->p_devinfo->uart_int_485_send) {
        p_dev->p_devinfo->uart_int_485_send(AM_TRUE);
    }

    /* 使能发送中断  使能发送完成中断 */
    amhw_fsl_uart_int_enable(p_hw_uart,
                             AMHW_FSL_UART_INT_C2_TIE | \
                             AMHW_FSL_UART_INT_C2_TCIE);


    return AM_OK;
}

/**
 * \brief 配置中断服务回调函数
 */
static int __uart_callback_set ( void  *p_drv,
                                 int    callback_type,
                                 void  *pfn_callback,
                                 void  *p_arg)
{
    am_fsl_uart_dev_t *p_dev = (am_fsl_uart_dev_t *)p_drv;

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
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t   *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    uint32_t  idle_stat;
    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
         idle_stat = (uint32_t)(amhw_fsl_uart_ver0_intstat_get(p_hw_uart)>>7);
    }else{
         idle_stat = (uint32_t)(amhw_fsl_uart_ver1_intstat_get(p_hw_uart)>>7);
    }

    /* 发送模块是否空闲, 0:忙; 1: 空闲 */
    if(((uint8_t)idle_stat & amhw_fsl_uart_stat1_tdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
    } else {
        /* 变为发送模式 */
        if (p_dev->p_devinfo->uart_int_485_send != NULL) {
            /* set to transmit mode */
            p_dev->p_devinfo->uart_int_485_send(AM_TRUE);
        }
        /* 发送一个字符 */
        amhw_fsl_uart_txdata_write(p_hw_uart, outchar);

        /* 等待可以发送 */
        if (p_dev->p_devinfo->uart_int_485_send) {
            /* 如果用485发送数据则必须等待当前字节发送完成才能改变数据方向  */
            while(!amhw_fsl_uart_stat1_tc_get(p_hw_uart));
            /* 变为接收模式 */
            p_dev->p_devinfo->uart_int_485_send(AM_FALSE);
        } else {
            while(!amhw_fsl_uart_stat1_tdre_get(p_hw_uart));
        }
    }

    return (AM_OK);
}

/**
 * \brief 轮询模式下接收字符
 */
static int __uart_poll_getchar (void *p_drv, char *p_char)
{
    am_fsl_uart_dev_t *p_dev     = (am_fsl_uart_dev_t *)p_drv;
    amhw_fsl_uart_t   *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t            *p_inchar  = (uint8_t *)p_char;

    uint32_t  idle_stat;
    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
         idle_stat = (uint16_t)(amhw_fsl_uart_ver0_intstat_get(p_hw_uart)>>5);
    }else{
         idle_stat = (uint16_t)(amhw_fsl_uart_ver1_intstat_get(p_hw_uart)>>5);
    }
    /* 接收模块是否空闲，0:忙,正在接收; 1: 已经接收到一个字符  */
    if(((uint8_t)idle_stat & amhw_fsl_uart_stat1_rdre_get(p_hw_uart)) == 0) {
        return (-AM_EAGAIN);
    }else {
        /* 接收一个字符 */
        *p_inchar = amhw_fsl_uart_rxdata_read(p_hw_uart);
    }

    return (AM_OK);
}

/**
 * \brief 配置串口模式
 */
int __uart_mode_set (am_fsl_uart_dev_t *p_dev, uint32_t new_mode)
{
    amhw_fsl_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

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
        amhw_fsl_uart_int_enable(p_hw_uart, AMHW_FSL_UART_INT_C2_IRIE);
    } else {
        /* 关闭所有串口中断 */
        amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_ALL);
    }

    p_dev->channel_mode = new_mode;

    return (AM_OK);
}

/**
 * \brief 串口选项配置
 */
int __uart_opt_set (am_fsl_uart_dev_t *p_dev, uint32_t options)
{
    amhw_fsl_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint8_t          cfg_flags = 0;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    /* 在改变UART寄存器值前 接收发送禁能 */
    amhw_fsl_uart_disable(p_hw_uart);

    /* 配置检验方式 */
    if (options & AM_UART_PARENB) {

        if (options & AM_UART_PARODD) {
            cfg_flags |= AMHW_FSL_UART_C1_PARITY_ODD;
        } else {
            cfg_flags |= AMHW_FSL_UART_C1_PARITY_EVEN;
        }
    } else {
        cfg_flags |= AMHW_FSL_UART_C1_PARITY_NO;
    }

    /* 配置数据长度 */
    switch (options & AM_UART_CSIZE) {

    case AM_UART_CS7:
        if (AMHW_FSL_UART_C1_PARITY_NO ==
            AMHW_FSL_UART_C1_PARITY_GET(cfg_flags)) {
            /* 不支持设置7bit 无校验模式 */
            amhw_fsl_uart_enable(p_hw_uart);
            return -AM_ENOTSUP;
        }
        /* 7bit+校验需要设置为8bit宽度 */
        cfg_flags |= AMHW_FSL_UART_C1_M_8BIT;
        break;

    case AM_UART_CS8:
        if (AMHW_FSL_UART_C1_PARITY_NO ==
            AMHW_FSL_UART_C1_PARITY_GET(cfg_flags)) {
            /* 设置8bit无校验需要设置为8bit宽度 */
            cfg_flags |= AMHW_FSL_UART_C1_M_8BIT;
        } else {
            /* 设置8bit+校验需要设置为9bit宽度 */
            cfg_flags |= AMHW_FSL_UART_C1_M_9BIT;
        }
        break;

    default:
        /* 不支持设置其他地址宽度bit */
        amhw_fsl_uart_enable(p_hw_uart);
        return -AM_ENOTSUP;
    }

    amhw_fsl_uart_data_mode_set(p_hw_uart, AMHW_FSL_UART_C1_M_GET(cfg_flags));
    amhw_fsl_uart_parity_set(p_hw_uart, AMHW_FSL_UART_C1_PARITY_GET(cfg_flags));

    /* 配置停止位 */
    if (options & AM_UART_STOPB) {
        amhw_fsl_uart_stop_bit_set (p_hw_uart, AMHW_FSL_UART_BDH_SBNS_STOP_2);
    } else {
        amhw_fsl_uart_stop_bit_set (p_hw_uart, AMHW_FSL_UART_BDH_SBNS_STOP_1);
    }

    amhw_fsl_uart_enable(p_hw_uart);

    p_dev->options = options;

    return (AM_OK);
}


/******************************************************************************
  UART interrupt request handler
******************************************************************************/
/**
 * \brief 串口接收中断服务
 */
void __uart_irq_rx_handler (am_fsl_uart_dev_t *p_dev)
{
    amhw_fsl_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t         int_stat;
    char             data;

    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
        int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    }else{
        int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }

    /* 是否为接收Rx中断 */
    if ((int_stat & AMHW_FSL_UART_INTSTAT_S1_RDRF) != 0) {

        /* 获取新接收数据 */
        data = amhw_fsl_uart_rxdata_read(p_hw_uart);

        /* 存放新接收数据 */
        p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
    }
}

/**
 * \brief 串口发送中断服务
 */
void __uart_irq_tx_handler (am_fsl_uart_dev_t *p_dev)
{
    amhw_fsl_uart_t *p_hw_uart = p_dev->p_devinfo->p_hw_uart;
    uint32_t         int_stat;
    char             data;

    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
        int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    }else{
        int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }

    if (((int_stat & AMHW_FSL_UART_INTSTAT_S1_TDRE) != 0) || /* 是否为发送Tx中断 */
        ((int_stat & AMHW_FSL_UART_INTSTAT_S1_TC) != 0)) {

        /* 获取发送数据并发送 */
        if ((p_dev->pfn_txchar_get(p_dev->txget_arg, &data)) == AM_OK) {
            amhw_fsl_uart_txdata_write(p_hw_uart, data);
        } else {
            /* 没有数据传送就关闭发送中断 */
            amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_C2_TIE);
            
            /* 禁止能485发送控制引脚 */
            if (int_stat & AMHW_FSL_UART_INTSTAT_S1_TC) {

                if (p_dev->p_devinfo->uart_int_485_send) {

                    p_dev->p_devinfo->uart_int_485_send(AM_FALSE);
                }
                amhw_fsl_uart_int_disable(p_hw_uart, AMHW_FSL_UART_INT_C2_TCIE);
            }
        }
    }
}

/**
 * \brief 串口中断服务函数
 */
void __uart_irq_handler (void *p_arg)
{
    am_fsl_uart_dev_t  *p_dev     = (am_fsl_uart_dev_t *)p_arg;
    amhw_fsl_uart_t    *p_hw_uart = p_dev->p_devinfo->p_hw_uart;

    uint32_t  uart_int_stat;
    if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
        uart_int_stat = amhw_fsl_uart_ver0_intstat_get(p_hw_uart);
    }else{
        uart_int_stat = amhw_fsl_uart_ver1_intstat_get(p_hw_uart);
    }

    if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_RDRF) {
         __uart_irq_rx_handler(p_dev);
    } else if (uart_int_stat & AMHW_FSL_UART_INTSTAT_S1_TDRE) {
        __uart_irq_tx_handler(p_dev);
    } else {

    }
    /* 其他中断 */
    if ((p_dev->other_int_enable & uart_int_stat) != 0) {

        uart_int_stat &= p_dev->other_int_enable;

        if(uart_int_stat == AMHW_FSL_UART_INT_C3_OVR){
            /* 若为OVR中断  则表示当前程序处理不及时，新接收到的数据无存放空间
             * 需要根据串口对其进行处理，若不处理，则会导致响应的串口无法继续进行接收
             */
            if(p_dev->p_devinfo->ver == AM_FSL_UART_VER0){
                /* 清除OVR状态位 */
                p_hw_uart->stat1 |= AMHW_FSL_UART_INT_C3_OVR;
            }else{
                /* 获取新接收数据 */
                uint8_t data = amhw_fsl_uart_rxdata_read(p_hw_uart);
                /* 存放新接收数据 */
                p_dev->pfn_rxchar_put(p_dev->rxput_arg, data);
            }
        }else{
            if (p_dev->pfn_err != NULL) {
                p_dev->pfn_err(p_dev->err_arg,
                               AM_FSL_UART_ERRCODE_UART_OTHER_INT,
                               (void *)uart_int_stat,
                               1);
            }
        }
    }

}

#if 0
/**
 * \brief UART中断函数连接，仅使用中断模式时需要调用此函数
 */
int __uart_connect (void *p_drv)
{
    am_fsl_uart_dev_t *p_dev = (am_fsl_uart_dev_t *)p_drv;

    /* 关联中断向量号，开启中断 */
    am_int_connect(p_dev->p_devinfo->inum, __uart_irq_handler, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);
    amhw_fsl_uart_int_enable(p_dev->p_devinfo->p_hw_uart,
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
am_uart_handle_t am_fsl_uart_init (am_fsl_uart_dev_t           *p_dev,
                                    const am_fsl_uart_devinfo_t *p_devinfo)
{
    amhw_fsl_uart_t  *p_hw_uart;

    if (p_devinfo == NULL || p_devinfo->p_hw_uart == NULL) {
        return NULL;
    }

    /* 获取配置参数 */
    p_hw_uart                = p_devinfo->p_hw_uart;
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

    p_dev->other_int_enable  = (p_devinfo->other_int_enable |
                                AMHW_FSL_UART_INT_C3_OVR)   &
                              ~(AMHW_FSL_UART_INT_C2_TCIE   |
                                AMHW_FSL_UART_INT_C2_IRIE   |
                                AMHW_FSL_UART_INT_C2_IDIE   |
                                AMHW_FSL_UART_INT_C2_TIE);

    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 获取串口数据长度配置选项 */
    switch (AMHW_FSL_UART_C1_M_GET(p_devinfo->cfg_flags)) {

    case AMHW_FSL_UART_C1_M_8BIT:
        if (AMHW_FSL_UART_C1_PARITY_NO ==
            AMHW_FSL_UART_C1_PARITY_GET(p_devinfo->cfg_flags)) {
            p_dev->options |= AM_UART_CS8;
        } else {
            p_dev->options |= AM_UART_CS7;
        }
        break;

    case AMHW_FSL_UART_C1_M_9BIT:
        if (AMHW_FSL_UART_C1_PARITY_NO ==
            AMHW_FSL_UART_C1_PARITY_GET(p_devinfo->cfg_flags)) {
            return NULL;
        }
        p_dev->options |= AM_UART_CS8;
        break;

    default:
        p_dev->options |= AM_UART_CS8;
        break;
    }

    /* 获取串口检验方式配置选项 */
    switch (AMHW_FSL_UART_C1_PARITY_GET(p_devinfo->cfg_flags)) {

    case AMHW_FSL_UART_C1_PARITY_EVEN:
        p_dev->options |= AM_UART_PARENB;
        break;

    case AMHW_FSL_UART_C1_PARITY_ODD:
        p_dev->options |= (AM_UART_PARENB | AM_UART_PARODD);
        break;

    default:
        break;
    }

    /* 获取串口停止位配置选项 */
    if (p_devinfo->cfg_flags & AMHW_FSL_UART_BDH_SBNS_STOP_2) {
        p_dev->options |= AM_UART_STOPB;
    }

    __uart_opt_set(p_dev, p_dev->options);

    /* 设置波特率 */
    if(p_devinfo->ver == AM_FSL_UART_VER0){
        p_dev->baud_rate  = amhw_fsl_uart_ver0_baudrate_set(p_hw_uart,
                am_clk_rate_get(p_dev->p_devinfo->clk_id),
                p_devinfo->baud_rate);
    }else{
        p_dev->baud_rate  = amhw_fsl_uart_ver1_baudrate_set(p_hw_uart,
                am_clk_rate_get(p_dev->p_devinfo->clk_id),
                p_devinfo->baud_rate);
    }

    /* 默认轮询模式  */
    __uart_mode_set(p_dev, AM_UART_MODE_POLL);

    /* uart使能 */
    amhw_fsl_uart_enable(p_hw_uart);

    return &(p_dev->uart_serv);
}

/**
 * \brief 串口去初始化
 */
void am_fsl_uart_deinit (am_fsl_uart_dev_t *p_dev)
{
    amhw_fsl_uart_t *p_hw_uart   = p_dev->p_devinfo->p_hw_uart;
    p_dev->uart_serv.p_funcs = NULL;
    p_dev->uart_serv.p_drv   = NULL;

    if (p_dev->channel_mode == AM_UART_MODE_INT) {
        /* 默认为轮询模式 */
        __uart_mode_set(p_dev, AM_UART_MODE_POLL);
    }
    /* 关闭串口 */
    amhw_fsl_uart_disable(p_hw_uart);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }

    am_int_disable(p_dev->p_devinfo->inum);
}

/* end of file */
