/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief ISO7816 协议
 *
 * \internal
 * \par Modification History
 * - 1.00 19-01-23  htf, first implementation.
 * \endinternal
 */

#include "string.h"
#include "am_iso7816.h"


#define __DEBUG_TD  1   /* 当收到TD(1)，多接收两个字节 */


/**
 * \name 设备状态
 * @{
 */

#define __AMDR_DEV_STATE_OFF   0x00
#define __AMDR_DEV_STATE_ON    0xFF

/** @} */

/*******************************************************************************
  本地函数声明
*******************************************************************************/

am_local int __iso7816_cold_reset (void        *p_drv,
                                   uint8_t     *p_atr,
                                   uint32_t    *p_nbytes,
                                   am_bool_t    pps);

am_local int __iso7816_warm_reset (void        *p_drv,
                                   uint8_t     *p_atr,
                                   uint32_t    *p_nbytes);

am_local int __iso7816_deactivate (void        *p_drv);

am_local int __iso7816_transfer (void          *p_drv,
                                 uint8_t        type,
                                 const void    *p_tx_buf,
                                 uint32_t       tx_nbytes,
                                 void          *p_rx_buf,
                                 uint32_t      *p_rx_nbytes);

am_local int __iso7816_pps_set (void     *p_drv,
                                uint8_t  *p_pps,
                                uint32_t  nbytes);

/*******************************************************************************
  全局变量
*******************************************************************************/

/**
 * \brief 时钟率转换因子
 */
am_local uint16_t __g_ic_f[16] = {
     372,  372,  558,  744,
    1116, 1488, 1860,    0,
       0,  512,  768, 1024,
    1536, 2048,    0,    0
};

/**
 * \brief 波特率校正参数
 */
am_local uint16_t __g_ic_d[16] = {
     0,  1,  2,  4,
     8, 16, 32,  0,
    12, 20,  0,  0,
     0,  0,  0,  0
};

/**
 * \brief ISO7816-3命令驱动函数
 */
const struct am_iso7816_drv_funcs __g_ic_funcs = {
    __iso7816_cold_reset,
    __iso7816_warm_reset,
    __iso7816_transfer,
    __iso7816_deactivate,
//    __amdr_touch_card_pps_set,
};

/*******************************************************************************
  本地函数
*******************************************************************************/

/**
 * \brief 计算LRC校验值
 *
 * \param[in] p_buf  : 存放数据的缓冲区
 * \param[in] nbytes : 数据的字节数
 *
 * \return LRC校验值
 */
am_local
uint32_t __iso7816_lrc_calc (const void *p_buf, uint32_t nbytes)
{
    uint32_t  i;
    uint32_t  lrc = 0;
    uint8_t  *p   = (uint8_t *)p_buf;

    if (p_buf == NULL || nbytes == 0) {
        return 0xffffffff;
    }

    for(i = 0; i < nbytes; i++) {
        lrc ^= p[i];
    }

    return lrc;
}

/**
 * \brief 断电
 *
 * \param[in] p_dev     : 接触式IC设备结构体指针
 *
 * \retval AM_OK        : 成功
 */
am_local
int __iso7816_cold_power_off (am_iso7816_dev_t  *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_set(p_dev->p_info->rst_pin, AM_GPIO_LEVEL_LOW);
    am_udelay(1000000 * 50 / p_dev->clkout);
    am_gpio_pin_cfg(p_dev->p_info->pwm_pin, AM_GPIO_OUTPUT_INIT_LOW);
    am_gpio_pin_cfg(p_dev->p_info->io_pin, AM_GPIO_OUTPUT_INIT_LOW);

    if (p_dev->p_info->is_high) {
        am_gpio_set(p_dev->p_info->vicc_pin, AM_GPIO_LEVEL_LOW);
    } else {
        am_gpio_set(p_dev->p_info->vicc_pin, AM_GPIO_LEVEL_HIGH);
    }

    am_udelay(1000000 * 50 / p_dev->clkout);

    p_dev->status = __AMDR_DEV_STATE_OFF;

    return AM_OK;
}

/**
 * \brief 加电
 *
 * \param[in] p_dev     : 接触式IC设备结构体指针
 *
 * \retval AM_OK        : 成功
 */
am_local
int __iso7816_cold_power_on (am_iso7816_dev_t  *p_dev)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_set(p_dev->p_info->rst_pin, AM_GPIO_LEVEL_LOW);

    if (p_dev->p_info->is_high) {
        am_gpio_set(p_dev->p_info->vicc_pin, AM_GPIO_LEVEL_HIGH);
    } else {
        am_gpio_set(p_dev->p_info->vicc_pin, AM_GPIO_LEVEL_LOW);
    }

    am_gpio_pin_cfg(p_dev->p_info->pwm_pin, p_dev->p_info->pwm_mode);
    am_pwm_config(p_dev->pwm_handle,
                  p_dev->p_info->pwm_chan,
                  1000000000 / p_dev->clkout / 2,
                  1000000000 / p_dev->clkout);
    am_pwm_enable(p_dev->pwm_handle, p_dev->p_info->pwm_chan);
    am_udelay(1000000 * 200 / p_dev->clkout);

    am_gpio_pin_cfg(p_dev->p_info->io_pin, p_dev->p_info->rx_mode);
    am_udelay(1000000 * 200 / p_dev->clkout);

    am_gpio_set(p_dev->p_info->rst_pin, AM_GPIO_LEVEL_HIGH);

    p_dev->status = __AMDR_DEV_STATE_ON;

    return AM_OK;
}

/**
 * \brief 接收字节
 *
 * \param[in] p_dev     : 接触式IC卡座结构体指针
 * \param[in] p_rx_buf  : 接收缓存区
 * \param[in] nbytes    : 需要接收的字节
 *
 * \retval AM_OK        : 成功
 *         其它         : 失败
 */
am_local
int __iso7816_data_recv (am_iso7816_dev_t *p_dev,
                         uint8_t          *p_rx_buf,
                         uint32_t          nbytes)
{
    uint8_t  count = 0;
    uint32_t timeout;

    if (p_dev == NULL || p_rx_buf == NULL) {
        return -AM_EINVAL;
    }

    p_dev->is_timeout = AM_FALSE;

    timeout = p_dev->etu * 1000000 / p_dev->clkout / 5;
    if (timeout < 1) {
        timeout = 1;
    }

    /* 初始化等待事件 */
    am_softimer_start(&p_dev->timeout_timer, timeout);

    while(count < nbytes && p_dev->is_timeout == AM_FALSE) {
        if (am_uart_poll_getchar(p_dev->uart_handle, (char *)&p_rx_buf[count])
                == AM_OK) {
            count ++;
            am_softimer_stop(&p_dev->timeout_timer);
            am_softimer_start(&p_dev->timeout_timer, timeout);
        }
    }

    if (p_dev->is_timeout) {
        p_dev->is_timeout = AM_FALSE;
        return -AM_ETIME;
    } else {
        return AM_OK;
    }
}

/**
 * \brief 发送字节
 *
 * \param[in] p_dev    : 接触式IC卡座结构体指针
 * \param[in] p_tx_buf : 发送缓冲区
 * \param[in] nbytes   : 需要发送的字节数
 *
 * \retval 成功发送的字节数
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_data_send (am_iso7816_dev_t  *p_dev,
                         uint8_t           *p_tx_buf,
                         uint32_t           nbytes)
{
    if (p_dev == NULL || p_tx_buf == NULL || nbytes == 0) {
        return -AM_EINVAL;
    }

    return am_uart_poll_send(p_dev->uart_handle, p_tx_buf, nbytes);
}

/**
 * \brief 输入/输出模式设置
 *
 * \param[in] p_dev    : 接触式IC卡座结构体指针
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_io_set (am_iso7816_dev_t *p_dev)
{
    uint32_t arg;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    if (p_dev->msb_first == 0) {
        arg = (AM_UART_PARENB | AM_UART_STOPB | AM_UART_CS8);   /* 偶校验、两个停止位、8个数据位 */
    } else {
        arg = (AM_UART_PARODD | AM_UART_STOPB | AM_UART_CS8);   /* 奇校验、两个停止位、8个数据位 */
    }

    return am_uart_ioctl(p_dev->uart_handle, AM_UART_OPTS_SET, (void *)arg);
}

/**
 * \brief 接触式IC卡接口时间参数设置
 *
 * \param[in] p_dev     : 接触式IC设备结构体指针
 *
 * \retval AM_OK        : 成功
 *         其它         : 失败
 */
am_local
int __iso7816_time_set (am_iso7816_dev_t *p_dev)
{
    uint32_t dn, fn;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    dn = __g_ic_d[p_dev->fd & 0x0f];
    fn = __g_ic_f[p_dev->fd >> 4];

    if (dn == 0 || fn == 0) {
        return -AM_ENOTSUP;
    }

    p_dev->etu = fn / dn;

    /* 最小时钟支持16 */
    if (p_dev->etu < 16) {
        return -AM_ENOTSUP;
    }

    am_uart_ioctl(p_dev->uart_handle,
                  AM_UART_BAUD_SET,
                  (void *)(p_dev->clkout / (fn / dn)));

    return AM_OK;
}

/**
 * \brief T=0 传输协议
 *
 * \param[in] p_slot      : 接触式IC卡卡座结构体指针
 * \param[in] p_tx_buf    : 发送缓存区
 * \param[in] tx_nbytes   : 需要发送的字节数
 * \param[in] p_rx_buf    : 接收缓存区
 * \param[in] p_rx_nbytes : 需要接收的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_t0_transfer (am_iso7816_dev_t      *p_slot,
                           const void            *p_tx_buf,
                           uint32_t               tx_nbytes,
                           void                  *p_rx_buf,
                           uint32_t              *p_rx_nbytes)
{
    int            ret;
    uint8_t        cmd_header[5];
    uint8_t        ins, lc, le, ack, temp_le;
    uint8_t       *p_send_buf;
    uint8_t       *p_recv_buf;
    uint8_t        error_times = 0;

    if (p_slot    == NULL ||
        p_tx_buf  == NULL ||
        p_rx_buf  == NULL ||
        tx_nbytes  < 5) {
        return -AM_EINVAL;
    }

    p_send_buf = (uint8_t *)p_tx_buf;
    p_recv_buf = (uint8_t *)p_rx_buf;

    *p_rx_nbytes = 0;
    ins          = p_send_buf[1];
    lc           = 0;
    le           = 0;

    for (uint8_t i = 0; i < 5; i++) {
        cmd_header[i] = p_send_buf[i];
    }

    if (tx_nbytes > 5) {    /* 有数据域的情况, 情况3、4 */
        lc = p_send_buf[4];
        if (tx_nbytes > (lc + 5)) { /* APDU命令情况3,lc Data */
            le = p_send_buf[tx_nbytes - 1]; /* APDU命令情况4,Lc Data Le */
        }
    } else if (5 == tx_nbytes) {
        le = p_send_buf[4]; /* APDU命令情况2, Data Le */
    } else {
        cmd_header[4] = 0;  /* APDU命令情况1 */
    }
    temp_le = le;

    /* 发送命令头 */
    __iso7816_data_send(p_slot, cmd_header, 5);

    while (1) {

        /* 接收响应 */
        ret = __iso7816_data_recv(p_slot, &ack, 1);
        if (ret != AM_OK) {
            if (ret == -AM_ETIME) {
                ack = 0x00;
            } else {
                return ret;
            }
        }

        if (ack == 0x60) {
            continue;
        } else if (ack == 0x00) {  /* 接收错误，重发命令 */
            error_times++;
            if (error_times >= 5) {
                return -AM_ETIME;
            }
            __iso7816_data_send(p_slot, cmd_header, 5);
        } else if (ack == ins) {   /* NULL，表示继续等待过程字节 */
            if (lc != 0) {
                am_udelay(1000000 * 16 / (p_slot->clkout / p_slot->etu));
                __iso7816_data_send(p_slot, &p_send_buf[5], lc);
                lc = 0;
            } else {
                ret = __iso7816_data_recv(p_slot,
                                         &p_recv_buf[*p_rx_nbytes],
                                          temp_le);
                if (ret != AM_OK) {
                    return ret;
                }
                if (0 != le) {
                    *p_rx_nbytes += (temp_le < le) ? temp_le : le;
                } else {
                    *p_rx_nbytes += temp_le;
                }
            }
        } else if (ack == 0x61) {   /* 正常处理，还有XX字节可接收 */
            ret = __iso7816_data_recv(p_slot, &temp_le, 1);
            if (ret != AM_OK) {
                return ret;
            }

            /*
             * 发GetRespons命令
             */
            ins            = 0xC0;  /* APDU_INS_GET_RESPONSE; */
            cmd_header[0]  = 0x00;  /* APDU_CLA_ISO; */
            cmd_header[1]  = 0xC0;  /* 取响应数据 APDU_INS_GET_RESPONSE; */
            cmd_header[2]  = 0;
            cmd_header[3]  = 0;

            /* 取较小长度(Le = 0 为最大长度) */
            if (0 != le) {
                temp_le = (temp_le < le) ? temp_le : le;
            }
            cmd_header[4]  = temp_le;
            am_udelay(1000000 * 16 / (p_slot->clkout / p_slot->etu));

            /* 发送命令头 */  //00 C0 00 00 17
            __iso7816_data_send(p_slot, cmd_header, 5);
        } else if (ack == 0x6C) {   /* 命令情形2, Le长度错误，实际长度由下字节指出 */
            ret = __iso7816_data_recv(p_slot, &temp_le, 1);
            if (ret != AM_OK) {
                return ret;
            }
            cmd_header[4] = temp_le;
            am_udelay(1000000 * 16 / (p_slot->clkout / p_slot->etu));

            /* 发送命令头 */
            __iso7816_data_send(p_slot, cmd_header, 5);
        } else if (ack == 0x91 || ack == 0x9E || ack == 0x9F) {

            /* 正常处理，还有XX字节可接收 */
            ret = __iso7816_data_recv(p_slot, &temp_le, 1);
            if (ret != AM_OK) {
                return ret;
            }

            /*
             * 发GetRespons命令
             */
            ins            = 0xC0;  /* APDU_INS_GET_RESPONSE; */
            cmd_header[0]  = 0xA0;  /* APDU_CLA_ISO; */
            cmd_header[1]  = 0xC0;  /* 取响应数据 APDU_INS_GET_RESPONSE; */
            cmd_header[2]  = 0;
            cmd_header[3]  = 0;

            /* 取较小长度(Le = 0 为最大长度) */
            if (0 != le) {
                temp_le = (temp_le < le) ? temp_le : le;
            }
            cmd_header[4]  = temp_le;
            am_udelay(1000000 * 16 / (p_slot->clkout / p_slot->etu));

            /* 发送命令头 */
            __iso7816_data_send(p_slot, cmd_header, 5);
        } else {
            p_recv_buf[*p_rx_nbytes] = ack;
            ret = __iso7816_data_recv(p_slot, &ack, 1);
            if (ret != AM_OK) {
                return ret;
            }
            *p_rx_nbytes += 1;
            p_recv_buf[*p_rx_nbytes] = ack;
            *p_rx_nbytes += 1;
            break;
        }
    }
    am_udelay(1000000 * 5 / (p_slot->clkout / p_slot->etu));

    return AM_OK;
}

/**
 * \brief 接触式IC卡接口参数设置
 *
 * \param[in] p_dev : 接触式IC设备结构体指针
 * \param[in] fd    : 时钟频率转换因子和波特率校正参数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_param_init (am_iso7816_dev_t *p_dev,
                          uint8_t           fd)
{
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_dev->msb_first  = 0;    /* 正向约定 */
    p_dev->t_type     = 0;    /* T = 0 */
    p_dev->fd         = fd;
    p_dev->extra_time = 0;    /* 字节额外保护时间为0 */
    p_dev->wi         = 10;   /* (T = 0)字符间最大时间间隔 960* WI * (Fi / f)*/
    p_dev->ifsc       = 32;   /* (T = 1)最大接收信息长度 */
    p_dev->b_cwi      = 0x45; /* (T = 1)字符等待时间分组等待时间(PBOC定义的最大值) */
    p_dev->tc3        = 0;

    return __iso7816_time_set(p_dev);
}

/**
 * \brief  接触式卡命令传输
 * \param[in] p_drv       : ISO7816 句柄
 * \param[in] type        : ISO7816 协议卡类型 可选参数: AM_ISO7816_TOUCH_CARD_TYPE_T0
 *                                                       AM_ISO7816_TOUCH_CARD_TYPE_T1
 * \param[in] p_tx_buf    : 待发送命令缓冲区
 * \param[in] tx_nbytes   : 待发送命令字节数
 * \param[in] p_rx_buf    : 接收数据缓冲区
 * \param[in] p_rx_nbytes : 接收到的数据长度
 *
 * \retval AM_OK : 操作成功
 *         其它  : 失败
 */
am_local
int __iso7816_transfer (void       *p_drv,
                        uint8_t     type,
                        const void *p_tx_buf,
                        uint32_t    tx_nbytes,
                        void       *p_rx_buf,
                        uint32_t   *p_rx_nbytes)
{
    am_iso7816_dev_t  *p_dev;
    int                ret;

    if (p_drv == NULL) {
        *p_rx_nbytes = 0;
        return -AM_EINVAL;
    }

    p_dev = (am_iso7816_dev_t *)p_drv;

    if (type > 15) {
        type = p_dev->t_type;
    }

    switch (type) {

    case 0 :
        ret = __iso7816_t0_transfer(p_dev,
                                    p_tx_buf,
                                    tx_nbytes,
                                    p_rx_buf,
                                    p_rx_nbytes);
        break;

//    case 1 :  T=1 卡命令通信未调试 （ 没有T=1的卡）


    default :
        ret = -AM_ENOTSUP;
        break;
    }

    return ret;
}

/**
 * \biref 接触式IC卡停活
 *
 * \param[in]  p_drv : ISO7816 句柄
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_deactivate (void *p_drv)
{
    am_iso7816_dev_t  *p_dev;

    if (p_drv == NULL) {
        return -AM_EINVAL;
    }

    p_dev = (am_iso7816_dev_t *)p_drv;

    __iso7816_cold_power_off(p_dev);

    return AM_OK;
}

/**
 * \brief 接触式IC卡PPS请求和响应设置
 *
 * \param[in] p_drv  : ISO7816 句柄
 * \param[in] p_pps  : 指向存放协议参数的指针
 * \param[in] nbytes : 协议参数的字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_pps_set (void    *p_drv,
                       uint8_t *p_pps,
                       uint32_t nbytes)
{
    am_iso7816_dev_t   *p_dev;
    int                 ret;
    uint8_t             tx_buf[6];
    uint8_t             rx_buf[6];
    uint8_t             index;

    if (p_drv == NULL || p_pps == NULL || nbytes == 0) {
        return -AM_EINVAL;
    }

    p_dev = (am_iso7816_dev_t *)p_drv;
    if (p_dev->status != __AMDR_DEV_STATE_ON) {
        return -AM_ENXIO;
    }

    tx_buf[0] = 0xFF;    /* PPS请求和相应代码固定为0xFF */
    if (p_dev->t_type == 0) {
        tx_buf[1] = p_pps[0] & 0x70;
    } else {

        /* T= 1，除T=0和T=1之外的协议按T=1处理 */
        tx_buf[1] = (p_pps[0] & 0x70) | 0x01;
    }

    index = 2;
    if (p_pps[0] & 0x10) {
        tx_buf[index++] = p_pps[1];
    }
    if (p_pps[0] & 0x20) {
        tx_buf[index++] = p_pps[2];
    }
    if (p_pps[0] & 0x40) {
        tx_buf[index++] = p_pps[3];
    }

    tx_buf[index] = __iso7816_lrc_calc(tx_buf, index);
    index++;

    /* 发送PPS请求 */
    __iso7816_data_send(p_dev, tx_buf, index);

    am_udelay(100);
    ret = __iso7816_data_recv(p_dev, rx_buf, index);
    if (ret != AM_OK) {
        return ret;
    }

    if (tx_buf[0] == rx_buf[0] || (rx_buf[1] & 0x0f) == (p_pps[0] & 0x0f)) {
        if (p_pps[0] & 0x10) {  /* PPS1存在 */
            if (rx_buf[1] & 0x10) { /* 响应PPS1请求 */
                if (rx_buf[2] == p_pps[1]) {
                    p_dev->fd = p_pps[1];
                    __iso7816_time_set(p_dev);
                } else {
                    return -AM_EAGAIN;
                }
            }
        }

        if (p_pps[0] & 0x20) {  /* PPS2存在 */
            if (rx_buf[1] & 0x20) { /* 响应PPS2请求 */
                if (rx_buf[3] == p_pps[2]) {
                    if (p_pps[2] == 0xFF) {
                        p_dev->extra_time = 0;
                    } else {
                        p_dev->extra_time = p_pps[2];
                    }
                } else {
                    return -AM_EAGAIN;
                }
            }
        }

        if (p_pps[0] & 0x40) {  /* PPS3存在 */
            if (rx_buf[1] & 0x40) { /* 响应PPS3请求，不支持PPS3 */
                if (rx_buf[4] != p_pps[3]) {
                    return -AM_EAGAIN;
                }
            }
        }
    } else {
        return -AM_EAGAIN;
    }

    return AM_OK;
}

/**
 * \brief 接触式IC卡热复位
 *
 * \param[in] p_drv     : ISO7816 服务句柄
 * \param[in] fd        : 初始传输时钟频率变换因子和比特率调整因子
 * \param[in] p_art     : 复位信息缓冲区
 * \param[in] p_nbytes  : 复位信息字节数
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_warm_reset (void     *p_drv,
                          uint8_t  *p_atr,
                          uint32_t *p_nbytes)
{
    am_iso7816_dev_t      *p_dev;
    int                    ret     = 0;
    uint32_t               bytes   = 0;
    uint32_t               i, td_x = 0;
    am_bool_t              end, td;

    if (p_drv == NULL || p_atr == NULL || p_nbytes == NULL) {
        return -AM_EINVAL;
    }

    p_dev = (am_iso7816_dev_t *)p_drv;

    ret = __iso7816_param_init(p_dev, 0x11);
    if (ret != AM_OK) {
        *p_nbytes = 0;
        return ret;
    }
    am_gpio_set(p_dev->p_info->rst_pin, AM_GPIO_LEVEL_LOW);

    /* 至少延时40000时钟周期 */
    for (i = 0; i < 100; i++) {
        am_udelay(1000000 * 400 / p_dev->clkout);
    }

    am_gpio_set(p_dev->p_info->rst_pin, AM_GPIO_LEVEL_HIGH);

    p_atr[0] = 0x00;
    while (p_atr[0] != 0x3b) {
        ret = __iso7816_data_recv(p_dev, &p_atr[0], 1);
        if (ret != AM_OK) {
            *p_nbytes = 0;
            __iso7816_cold_power_off(p_dev);
            return ret;
        }
    }

    i      = 1;       /* 接收完第一个字节 */
    bytes  = 1;
    end    = AM_FALSE;
    td     = AM_TRUE;    /* 下一个要接收的为TD(i+1)或T0 */

    while (bytes -- ) {

        ret = __iso7816_data_recv(p_dev, &p_atr[i], 1);
        if (ret != AM_OK) {
            *p_nbytes = 0;
            return ret;
        }

        if (bytes == 0 && end == AM_FALSE) {
            if (td == AM_TRUE) {    /* 接收的字符为TD(i+1)或T0 */
                if (p_atr[i] & 0x10) {  /* TA(i+1)存在 */
                    bytes++;
                }
                if (p_atr[i] & 0x20) {  /* TB(i+1)存在 */
                    bytes++;
                }
                if (p_atr[i] & 0x40) {  /* TC(i+1)存在 */
                    bytes++;
                }
                if (p_atr[i] & 0x80) {  /* TD(i+1)存在 */
                    bytes++;
                    if (i == 1) {
                        td_x = bytes + 1;   /* 记录TD(1)的位置 */
                    }
                } else {
                    td = AM_FALSE;
                }

                /* TD(1)的值 */
                if (i == td_x) {
                    p_dev->t_type = p_atr[i] & 0x0f;
                }
            } else {    /* 接收剩余的信息 */
                bytes += p_atr[1] & 0x0f;

#if __DEBUG_TD
                if (td_x > 0) {
                    bytes += 2;
                }
#endif

                if (p_atr[1] & 0x80) {
                    if (p_dev->t_type == 1) {
                        bytes++;
                    }
                }
                end = AM_TRUE;
            }
        }
        i ++;
    }

    /*
     * 先对T0字节做解析
     */
    *p_nbytes = i;
    i         = 1;
    td_x      = i;

    if (p_atr[td_x] & 0x10) {   /* TA(1)存在，更新通信速率 */
        ++i;
        p_dev->fd       = p_atr[i];

        /* 可能是协商模式，需要执行PPS */
        p_dev->auto_pps = AM_TRUE;
    } else {
        p_dev->auto_pps = AM_FALSE;
    }

    if (p_atr[td_x] & 0x20) {   /* 对TB(1)不做处理 */
        ++i;
    }

    if (p_atr[td_x] & 0x40) {   /* TC(1)存在，更新额外保护时间 */
        ++i;
        p_dev->extra_time = p_atr[i];
        if (p_dev->extra_time == 0xFF) {
            p_dev->extra_time = 0;
        }
    }

    if (!(p_atr[td_x] & 0x80)) {    /* TD(1)不存在 */
//        if (p_dev->auto_pps == AM_OK) {
            p_dev->fd = 0x11;
//        }
    } else {
        ++i;
        td_x = i;
        p_dev->t_type = p_atr[i] & 0x0f;

        if (p_atr[td_x] & 0x10) {   /* TA(2)存在 */
            ++i;
            p_dev->auto_pps = AM_FALSE; /* 专用模式，不需要执行PPS */
            if (p_atr[i] & 0x10) {

                /*
                 * TA(2) bit4 == 0，使用TA(1)指定的FD，否则，使用默认FD
                 */
                p_dev->fd = 0x11;
            }
        } else {    /* TA(2)不存在，需要使用协商模式，修改参数 */
            p_dev->auto_pps = AM_TRUE;
            p_dev->fd       = 0x11;
        }

        if (p_atr[td_x] & 0x20) {   /* TB2存在(定义编程电压PI2，TB2存在，TB1定义的编程PI1无效) */
            ++i;
        }

        if (p_atr[td_x] & 0x40) {   /* TC(2)存在 */
            ++i;
            p_dev->wi = p_atr[i];
        }

        if (p_atr[td_x] & 0x80) {   /* TD(2)存在 */
            ++i;
            td_x = i;
            if (p_atr[td_x] & 0x10) {   /* TA(3)存在，定义信息字段长度 */
                ++i;
                p_dev->ifsc = p_atr[i];
            }
            if (p_atr[td_x] & 0x20) {   /* TB(3)存在，字符等待时间 */
                ++i;
                p_dev->b_cwi = p_atr[i];
            }
            if (p_atr[td_x] & 0x40) {   /* TC(3)存在，帧尾校验模式 */
                ++i;
                p_dev->tc3   = p_atr[i];
            }
        }
    }

    if (p_dev->t_type != 0) {

        /* 计算TCK */
        if (__iso7816_lrc_calc(&p_atr[1], *p_nbytes - 1)) { /* LRC的结果要为0，才是正确的数据 */
            *p_nbytes = 0;
            return -AM_EBADMSG;
        }
    }

    ret = __iso7816_time_set(p_dev);
    if (ret != AM_OK) {
        *p_nbytes = 0;
        return ret;
    }

    ret = __iso7816_io_set(p_dev);

    am_udelay(1000000 * 5 / (p_dev->clkout / p_dev->etu));

    return ret;
}

/**
 * \brief 接触式IC卡冷复位
 *
 * \param[in] p_drv     : ISO7816 服务句柄
 * \param[in] fd        : 初始传输时钟频率变换因子和比特率调整因子
 * \param[in] p_art     : 复位信息缓冲区
 * \param[in] p_nbytes  : 复位信息字节数
 * \param[in] pps       : PPS请求使能
 *
 * \retval AM_OK      : 成功
 * \retval -AM_EINVAL : 参数错误
 */
am_local
int __iso7816_cold_reset (void       *p_drv,
                          uint8_t    *p_atr,
                          uint32_t   *p_nbytes,
                          am_bool_t   pps)
{
    int8_t              ret;
    am_iso7816_dev_t   *p_dev;
    uint8_t             temp_buf[4];

    if (p_drv == NULL || p_atr == NULL || p_nbytes == NULL) {
        return -AM_EINVAL;
    }
    p_dev = (am_iso7816_dev_t *)p_drv;

    __iso7816_cold_power_off(p_dev);
    __iso7816_cold_power_on(p_dev);

    ret = __iso7816_warm_reset(p_dev, p_atr, p_nbytes);
    if (ret != AM_OK) {
        *p_nbytes = 0;
        return ret;
    }

    if (pps != AM_TRUE || p_dev->auto_pps != AM_TRUE) {
        /* 不需要自动处理PPS，则直接返回 */
        return AM_OK;
    }

    am_udelay(1000000 * 22 / (p_dev->clkout / p_dev->etu));
    temp_buf[0] = 0x10;
    temp_buf[1] = p_atr[2];  /* TA(1) */

    ret = __iso7816_pps_set(p_drv, temp_buf, 4);
    am_udelay(1000000 * 22 / (p_dev->clkout / p_dev->etu));

    return ret;
}

/**
 * \brief 软件定时器回调函数
 *
 * \param[in] p_arg     : 回调函数参数
 */
am_local
void __timeout_cb (void *p_arg)
{
    am_bool_t *p_timeout = (uint8_t *)p_arg;

    *p_timeout = AM_TRUE;
}

/*******************************************************************************
  外部函数
*******************************************************************************/

/**
 * \brief ISO7816接触式卡协议初始化
 *
 * \param[in] p_dev         : 接触式IC卡卡座结构体指针
 * \param[in] uart_handle   : 串口服务句柄
 * \param[in] p_info        : 设备信息结构体指针
 *
 * \retval ISO7816 句柄
 */
am_iso7816_handle_t am_iso7816_init (am_iso7816_dev_t              *p_dev,
                                     am_uart_handle_t               uart_handle,
                                     am_pwm_handle_t                pwm_handle,
                                     const am_iso7816_devinfo_t    *p_info)
{
    if (p_dev == NULL || p_info == NULL) {
        return NULL;
    }

    if (uart_handle == NULL || pwm_handle == NULL) {
        return NULL;
    }

    p_dev->clkout      = p_info->clk;
    p_dev->is_timeout  = AM_FALSE;
    p_dev->p_info      = p_info;
    p_dev->pwm_handle  = pwm_handle;
    p_dev->uart_handle = uart_handle;

    if (p_info->is_high) {
        am_gpio_pin_cfg(p_info->vicc_pin, AM_GPIO_OUTPUT_INIT_LOW);
    } else {
        am_gpio_pin_cfg(p_info->vicc_pin, AM_GPIO_OUTPUT_INIT_HIGH);
    }

    am_gpio_pin_cfg(p_info->rst_pin, AM_GPIO_OUTPUT_INIT_LOW);

    p_dev->status = __AMDR_DEV_STATE_OFF;

    am_softimer_init(&p_dev->timeout_timer,
                     __timeout_cb,
                     (void *)&p_dev->is_timeout);

    p_dev->serv.p_funcs = (struct am_iso7816_drv_funcs *)&__g_ic_funcs;
    p_dev->serv.p_drv   = (void *)p_dev;

    return (am_iso7816_handle_t)&p_dev->serv;
}

/* end of file */
