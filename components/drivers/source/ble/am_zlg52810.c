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
 * \brief ZLG52810 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 19-01-23  htf, first implementation.
 * \endinternal
 */


#include "ametal.h"
#include "am_zlg52810.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_uart.h"
#include "am_uart_rngbuf.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_wait.h"
#include "am_delay.h"
#include "am_list.h"
#include "string.h"

/* \brief   命令接收解析状态机  */
#define __ZLG52810_CMD_PROC_STAT_NONE          0   /* Not in cmd process      */
#define __ZLG52810_CMD_PROC_STAT_START         1   /* Cmd process startted    */
#define __ZLG52810_CMD_PROC_STAT_SEND          2   /* Cmd send complete       */
#define __ZLG52810_CMD_PROC_STAT_RECV_A        3   /* receive ack "A"         */
#define __ZLG52810_CMD_PROC_STAT_RECV_AT       4   /* receive ack "AT"        */
#define __ZLG52810_CMD_PROC_STAT_RECV_AT_1     5   /* receive ack "AT+"       */
#define __ZLG52810_CMD_PROC_STAT_RECV_AT_OK    6   /* receive ack "AT+OK"     */
#define __ZLG52810_CMD_PROC_STAT_RECV_AT_ERR   7   /* receive ack "AT+ERR"    */
#define __ZLG52810_CMD_PROC_STAT_RECV_END      8   /* receive first line      */
#define __ZLG52810_CMD_PROC_STAT_RECV_OVERFLOW 9   /* receive overflow recbuff*/
#define __ZLG52810_CMD_PROC_STAT_RECV_ERR      10  /* some error happen       */

/* \brief BLE 连接状态 */
#define __ZLG52810_STATE_BLE_NONE              0   /* 无蓝牙连接                   */
#define __ZLG52810_STATE_BLE_CONNECTED         1   /* 模块与蓝牙连接中       */
#define __ZLG52810_STATE_BLE_DISCONNECT        2   /* 模块与蓝牙断开连接   */

/* \brief 参数类型标识 */
#define __ZLG52810_ARG_TYPE_UINT32             1   /* 传入参数为uint32_t 类型       */
#define __ZLG52810_ARG_TYPE_STRING             2   /* 传入参数为char * 类型       */
#define __ZLG52810_ARG_TYPE_FLOAT              3   /* 传入参数为float 类型       */

/* The max data value with a given length */
am_local const int __g_max_val[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
};

am_local int __zlg52810_data_send (am_zlg52810_dev_t    *p_this,
                                   const uint8_t        *p_buf,
                                   int                   len)
{
    am_rngbuf_t rb     = &(p_this->tx_rngbuf);
    uint32_t    nbytes = len;

    uint32_t rb_ct, write_ct;

    if (am_rngbuf_isfull(rb) == AM_TRUE) {
        return -AM_ENOSPC;
    }

    rb_ct = am_rngbuf_freebytes(rb);
    write_ct = (rb_ct > len) ? len : rb_ct;

    am_rngbuf_put(rb, (const char *)p_buf, write_ct);

    p_buf += write_ct;
    len   -= write_ct;

    am_uart_tx_startup(p_this->uart_handle);

    return nbytes - len;
}

am_local int __zlg52810_cmd_str_send(am_zlg52810_dev_t    *p_this,
                                     const char           *p_str)
{
    return __zlg52810_data_send(p_this, (const uint8_t *)(p_str), strlen(p_str));
}

am_local int __zlg52810_cmd_char_send(am_zlg52810_dev_t    *p_this,
                                     char                 ch)
{
    return __zlg52810_data_send(p_this, (const uint8_t *)(&ch), 1);
}

am_local int __zlg52810_cmd_uint_send (am_zlg52810_dev_t    *p_this,
                                      unsigned int         data,
                                      int                  max_len)
{
    char c;
    int  first_none_zero = 0;  /* whether appear first none zero data */

    data = data % __g_max_val[max_len];

    while (max_len != 0) {

        max_len--;

        c = data / __g_max_val[max_len] + '0';

        if (c != '0') {

            first_none_zero = 1;
            __zlg52810_cmd_char_send(p_this, c);

        } else if (first_none_zero || (max_len == 0)){

            /* first none zero data is appear */
            __zlg52810_cmd_char_send(p_this, c);
        }

        data = data % __g_max_val[max_len];
    };

    return 0;
}


/* Send a data (type is int) as a string, eg: send 100, will send "100" string */
am_local int __zlg52810_cmd_send (am_zlg52810_dev_t    *p_this,
                                  const char           *p_hdstr,
                                  int                   arg_type,
                                  int                   max_len,
                                  void                 *p_arg )
{

    p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_START;

    /* The tx buffer temp for receive cmd ack, only used after send complete */
    p_this->p_cmd_rxbuf   = p_this->p_devinfo->p_uart_txbuf;
    p_this->cmd_rxbuf_len = p_this->p_devinfo->txbuf_size;
    p_this->cmd_rx_len    = 0;

    /* command always start with "AT+ "*/
    __zlg52810_cmd_str_send(p_this, "AT+");

    if (p_hdstr) {
        __zlg52810_cmd_str_send(p_this, p_hdstr);
    }

    if (p_arg) {

        if (arg_type == __ZLG52810_ARG_TYPE_UINT32) {

            __zlg52810_cmd_uint_send(p_this, (uint32_t)p_arg, max_len);

        } else if (arg_type == __ZLG52810_ARG_TYPE_STRING) {

            __zlg52810_cmd_str_send(p_this, (char *)p_arg);

        }
    }
    return AM_OK;
}


am_local int __zlg52810_cmd_result_info_get (am_zlg52810_dev_t    *p_this,
                                             int                   arg_type,
                                             void                 *p_arg,
                                             am_bool_t             need_check)
{
    char *p_ack_str = (char *)(p_this->p_cmd_rxbuf);
    char *p_start = NULL;
    char *p_end   = NULL;

    p_this->p_cmd_rxbuf[p_this->cmd_rx_len] = '\0';

    AM_DBG_INFO("ack is %d bytes : \r\n%s",
                p_this->cmd_rx_len,
                p_this->p_cmd_rxbuf);

    /* error happen */
    if (strstr(p_ack_str,"AT+ERR:7") != NULL) {
        return AM_ZLG52810_PARAMETER_INVALID;
    }else if(strstr(p_ack_str,"AT+ERR:9") != NULL) {
        return AM_ZLG52810_COMMAND_INVALID;
    }else if(strstr(p_ack_str, "AT+OK:") == NULL){
        return AM_ZLG52810_RECVIVE_INVALID;
    }else{
        /* Get the data for p_arg */
        p_start = strstr(p_ack_str, "AT+OK:");

        if (p_start == NULL) {
            return AM_ERROR;
        }

        p_start += 6;  /* Skip The Second "AT+OK:"*/

        p_end = p_start;

        while (*p_end != '\0') {
            if ((*p_end == '\r') || (*p_end == '\n')) {
                *p_end = '\0';
                break;
            }
            p_end++;
        }

        if (p_start >= p_end) {
            return AM_ERROR;
        }

        if (arg_type == __ZLG52810_ARG_TYPE_UINT32) {
            if(need_check == AM_TRUE){
                if( (uint32_t )p_arg != strtoul(p_start, &p_end, 10)){
                return AM_ERROR;
                }
            }else {
                *(uint32_t *)p_arg = strtoul(p_start, &p_end, 10);
            }

        } else if (arg_type == __ZLG52810_ARG_TYPE_STRING) {
            if(need_check == AM_TRUE){
                if(strcmp(p_arg, p_start) != 0){
                return AM_ERROR;
                }
            }else{
                strcpy(p_arg, p_start);
            }
        } else if (arg_type == __ZLG52810_ARG_TYPE_FLOAT){

            /* only for version "1.00" -> 100 */
            if (p_start[1] != '.') {
                return AM_ERROR;
            }
            p_start[1] = p_start[2];
            p_start[2] = p_start[3];
            p_start[3] = '\0';

            *(uint32_t *)p_arg = strtoul(p_start, &p_end, 10);
            if (p_end == p_start) {
                return AM_ERROR;
            }
        }else{
            return AM_ERROR;
        }
    }
    return AM_OK;
}


am_local int __zlg52810_cmd_result (am_zlg52810_dev_t    *p_this,
                                    int                   arg_type,
                                    void                 *p_arg,
                                    am_bool_t             need_check)
{
    int          key;

    /* 等待发送完成 */
    am_wait_on(&p_this->ack_wait);

    am_rngbuf_init(&(p_this->tx_rngbuf),
                    (char *)p_this->p_devinfo->p_uart_txbuf,
                    p_this->p_devinfo->txbuf_size);

    while (1) {

        /* wait 200ms (actual about 30~150)  */
        if (am_wait_on_timeout(&p_this->ack_wait, 2000) == AM_OK) {

            key = am_int_cpu_lock();

            /* success process */
            if (p_this->cmd_proc_state == __ZLG52810_CMD_PROC_STAT_RECV_END) {

                p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_NONE;
                am_int_cpu_unlock(key);

                return __zlg52810_cmd_result_info_get(p_this,
                                                      arg_type,
                                                      p_arg,
                                                      need_check);
            }

            if (p_this->cmd_proc_state != __ZLG52810_CMD_PROC_STAT_RECV_ERR) {

                am_int_cpu_unlock(key);
                continue;
            }

            am_int_cpu_unlock(key);
        }

        /* run to here, some error happen */
        key = am_int_cpu_lock();

        /* move the data to rx rngbuf */
        if (p_this->cmd_rx_len != 0) {
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          (const char *)(p_this->p_cmd_rxbuf),
                           p_this->cmd_rx_len);

            am_wait_done(&(p_this->rx_wait));
        }
        if(p_this->p_devinfo->pin_cts != -1 && p_this->flowc_state == AM_ZLG52810_FLOWC_DISABLE){
            am_gpio_set(p_this->p_devinfo->pin_cts, AM_GPIO_OUTPUT_INIT_LOW);
            p_this->flowc_state = AM_ZLG52810_FLOWC_EN_NOFLOW;
        }

        p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_NONE;

        am_int_cpu_unlock(key);

        return AM_ERROR;
    }
}

/**
 * \brief the function that to get one char to transmit.
 */
am_local int __zlg52810_uart_txchar_get (void *p_arg, char *p_outchar)
{
    am_zlg52810_dev_t *p_dev       = (am_zlg52810_dev_t *)p_arg;
    am_rngbuf_t        rb          = &(p_dev->tx_rngbuf);
    uint32_t           key         = am_int_cpu_lock();

    /* 若开启流控功能  并存在流控引脚  判断模块是否能接受数据*/
    if(p_dev->flowc_state != AM_ZLG52810_FLOWC_DISABLE &&
            p_dev->p_devinfo->pin_rts != -1){
        if(am_gpio_get(p_dev->p_devinfo->pin_rts == 0)){
            /* Success trans one data from ring buffer */
            if (am_rngbuf_getchar(rb, p_outchar) == 1) {
                am_int_cpu_unlock(key);
                return AM_OK;
            }
        }else{
            return -AM_EEMPTY;
        }
    }else{
        /* 若未开启流控功能  直接发送数据*/
        if (am_rngbuf_getchar(rb, p_outchar) == 1) {
            am_int_cpu_unlock(key);
            return AM_OK;
        }
    }

    if (p_dev->cmd_proc_state == __ZLG52810_CMD_PROC_STAT_START) {

        p_dev->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_SEND;

        /* send complete  */
        am_wait_done(&p_dev->ack_wait);
    }

    am_int_cpu_unlock(key);

    return -AM_EEMPTY;     /* No data to transmit,return -AM_EEMPTY */
}

int i = 0;
am_local int __zlg52810_cmd_ack_recv_proc (am_zlg52810_dev_t *p_this, char inchar)
{
    int ret = AM_ERROR;

    switch (p_this->cmd_proc_state) {

    case __ZLG52810_CMD_PROC_STAT_SEND:  /* Cmd send complete       */
        memset(p_this->p_cmd_rxbuf, 0 , 10);
         if (inchar == 'A') {
             ret = AM_OK;
             p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
             p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_RECV_A;
         }
         break;

    case __ZLG52810_CMD_PROC_STAT_RECV_A:
        if (inchar == 'T') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_RECV_AT;
        }
        break;

    case __ZLG52810_CMD_PROC_STAT_RECV_AT:
        if (inchar == '+') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_RECV_AT_1;
        }
        break;

    case __ZLG52810_CMD_PROC_STAT_RECV_AT_1:

        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
        am_softimer_stop(&p_this->timer);
        am_softimer_start(&p_this->timer, 3);
        break;
    }
    /* 接收错误  接收帧头错误将会进入该判断 */
    if (ret == AM_OK){
        /* full */
        if ((p_this->cmd_rx_len     == p_this->cmd_rxbuf_len) &&
            (p_this->cmd_proc_state != __ZLG52810_CMD_PROC_STAT_RECV_END)) {

            if(p_this->p_devinfo->pin_cts != -1 &&
                    p_this->flowc_state == AM_ZLG52810_FLOWC_EN_NOFLOW){
                /*此处开启MCU流控*/
                am_gpio_set(p_this->p_devinfo->pin_cts, AM_GPIO_OUTPUT_INIT_LOW);
                p_this->flowc_state = AM_ZLG52810_FLOWC_EN_FLOW;
            }else{
                /*未开启流控功能  接收命令溢出接收缓冲区*/
                p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_RECV_OVERFLOW;
            }
        }
    }

    /* receive one ack data */
    am_wait_done(&p_this->ack_wait);

    return ret;
}

am_local int __zlg52810_data_recv (am_zlg52810_dev_t    *p_this,
                                   uint8_t              *p_buf,
                                   int                   len)
{
    am_rngbuf_t rb = &(p_this->rx_rngbuf);

    uint32_t rb_ct, read_ct;
    uint32_t nbytes = 0;

    while (len > 0) {

        if (am_rngbuf_isempty(rb) == AM_TRUE) {

            if (p_this->timeout_ms == (uint32_t)AM_WAIT_FOREVER) {

                am_wait_on(&p_this->rx_wait);

            } else if (p_this->timeout_ms != AM_NO_WAIT) {

                if (am_wait_on_timeout(&p_this->rx_wait,
                                       p_this->timeout_ms) != AM_OK) {
                    return nbytes;
                }

            } else {
                return nbytes;
            }
        }

        rb_ct   = am_rngbuf_nbytes(rb);

        read_ct = (rb_ct > len) ? len : rb_ct;

        am_rngbuf_get(rb, (char *)p_buf, read_ct);

        p_buf   += read_ct;
        len     -= read_ct;
        nbytes  += read_ct;
    }

    return nbytes;
}

/**
 * \brief the function revived one char.
 */
am_local int __zlg52810_uart_rxchar_put (void *p_arg, char inchar)
{
    am_zlg52810_dev_t *p_dev  = (am_zlg52810_dev_t *)p_arg;
    am_rngbuf_t        rx_rb  = &(p_dev->rx_rngbuf);

    if (p_dev->cmd_proc_state >= __ZLG52810_CMD_PROC_STAT_SEND) {
        /* success process by cmd ack */
        if (__zlg52810_cmd_ack_recv_proc(p_dev, inchar) == AM_OK) {
             return AM_OK;
        }
    }

    /* wait done */
    am_wait_done(&p_dev->rx_wait);

    if (am_rngbuf_putchar(rx_rb, inchar) == 1) {
        return AM_OK;
    }

    return -AM_EFULL;          /* No data to receive,return -AM_EFULL */
}


void __timer_cb_function(void *p_arg){
    am_zlg52810_dev_t *p_this = (am_zlg52810_dev_t *)p_arg;
    am_softimer_stop(&p_this->timer);
    p_this->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_RECV_END;
    am_wait_done(&p_this->ack_wait);
}
/**
 * \brief zlg52810  init
 */
am_zlg52810_handle_t am_zlg52810_init (am_zlg52810_dev_t           *p_dev,
                                       const am_zlg52810_devinfo_t *p_devinfo,
                                       am_uart_handle_t             uart_handle)
{

    if ((p_dev                   == NULL) ||
        (p_devinfo               == NULL) ||
        (p_devinfo->p_uart_rxbuf == NULL) ||
        (p_devinfo->p_uart_txbuf == NULL)) {

        return NULL;
    }

    p_dev->uart_handle    = uart_handle;
    p_dev->p_devinfo      = p_devinfo;
    p_dev->timeout_ms     = (uint32_t)AM_WAIT_FOREVER;
    p_dev->cmd_proc_state = __ZLG52810_CMD_PROC_STAT_NONE;
    p_dev->stat_rep_cnt   = 0;
    p_dev->connect_state  = __ZLG52810_STATE_BLE_NONE;
    p_dev->flowc_state    = AM_ZLG52810_FLOWC_DISABLE;

    am_wait_init(&(p_dev->rx_wait));
    am_wait_init(&(p_dev->ack_wait));

    /* 流控控制引脚初始化 */
    if (p_devinfo->pin_rts != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_rts,AM_GPIO_INPUT);
    }
    if (p_devinfo->pin_cts != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_cts,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_LOW);
    }

    /* 恢复出厂设置引脚 */
    if (p_devinfo->pin_restore != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_restore,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_HIGH);
    }

    /* 低电平复位引脚 */
    if (p_devinfo->pin_rst != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_rst,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_HIGH);
    }

    /* 低电平复位引脚 */
    if (p_devinfo->pin_wakeup != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_rst,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_HIGH);
    }

    am_uart_ioctl(uart_handle, AM_UART_BAUD_SET, (void *)(p_devinfo->baudrate));

    /* Initialize the ring-buffer */
    am_rngbuf_init(&(p_dev->rx_rngbuf),
                   (char *)p_devinfo->p_uart_rxbuf,
                   p_devinfo->rxbuf_size);

    am_rngbuf_init(&(p_dev->tx_rngbuf),
                   (char *)p_devinfo->p_uart_txbuf,
                   p_devinfo->txbuf_size);

    am_softimer_init(&p_dev->timer, __timer_cb_function , (void *)p_dev);

    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
        (int (*)(void *))__zlg52810_uart_txchar_get,
                         (void *)(p_dev));

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
        (int (*)(void *))__zlg52810_uart_rxchar_put,
                         (void *)(p_dev));

    return p_dev;
}

/**
 * \brief zlg52810 send data
 */
int am_zlg52810_send (am_zlg52810_handle_t  handle,
                      const uint8_t        *p_buf,
                      int                   len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    /* in cmd process, can't send data now */
    if (handle->cmd_proc_state != __ZLG52810_CMD_PROC_STAT_NONE) {
        return -AM_EPERM;
    }

    return __zlg52810_data_send(handle, p_buf, len);
}

/**
 * \brief zlg52810 recvice data
 */
int am_zlg52810_recv (am_zlg52810_handle_t  handle,
                      uint8_t              *p_buf,
                      int                   len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg52810_data_recv(handle, p_buf, len);
}

/**
 * \brief zlg52810  do command
 */
int am_zlg52810_ioctl (am_zlg52810_handle_t  handle,
                       int                   cmd,
                       void                 *p_arg)
{
    int ret = -AM_EIO;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch (cmd) {
        case AM_ZLG52180_ACOMA_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "ACOMA:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;

        case AM_ZLG52810_ACOMA_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "ACOMA:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_ACOUP_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "ACOUP:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_ACOUP_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "ACOUP:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_ADDR_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "ADDR:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_ADTY_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "ADTY:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_ADTY_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "ADTY:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_ADVI_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "ADVI:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_ADVI_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "ADVI:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_BAUD_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "BAUD:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_BAUD_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "BAUD:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_CONS_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "CONS:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);

            if(p_arg == 0 && ret == AM_OK){
                handle->connect_state = __ZLG52810_STATE_BLE_DISCONNECT;
            }else if((uint32_t)p_arg == 1 && ret == AM_OK){
                handle->connect_state = __ZLG52810_STATE_BLE_CONNECTED;
            }
            break;

        case AM_ZLG52810_DISCONNECT:
            ret = __zlg52810_cmd_send(handle,
                                     "CONS:0",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        0,
                                        AM_TRUE);
            if(p_arg == 0 && ret == AM_OK){
                handle->connect_state = __ZLG52810_STATE_BLE_DISCONNECT;
            }else if((uint32_t)p_arg == 1 && ret == AM_OK){
                handle->connect_state = __ZLG52810_STATE_BLE_CONNECTED;
            }
            break;
        case AM_ZLG52810_COPS_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "COPS:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_COPS_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "COPS:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_ERASE :
            ret = __zlg52810_cmd_send(handle,
                                     "ERASE:1",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        (void *)1,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_FARESET:
            ret = __zlg52810_cmd_send(handle,
                                     "FARESET:1",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            break;
        case AM_ZLG52810_FLC_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "FLC:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_FLC_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "FLC:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_LESC_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "LESC:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_LESC_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "FLC:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_LOWL_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "LOWL:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_LOWL_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "LOWL:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_MFSD_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "MFSD:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_MFSD_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "MFSD:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_NAME_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "NAME:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_NAME_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "NAME:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_PASS_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "PASS:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_PASS_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "PASS:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_POWE_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "POWE:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_POWE_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "POWD:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_RESET:
            ret = __zlg52810_cmd_send(handle,
                                     "RESET:1",
                                      0,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            break;
        case AM_ZLG52810_RSPI_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "RSPI:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_RSPI_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "RSPI:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_UTDT_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "UTDT:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_UTDT_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "UTDT:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_SECL_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "SECL:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_SECL_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "SECL:",
                                      __ZLG52810_ARG_TYPE_UINT32,
                                      1,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_UINT32,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_UUTX_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "UUTX:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_UUTX_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "UUTX:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_UURX_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "UURX:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_UURX_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "UURX:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_UUSE_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "UUSE:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_UUSE_SET:
            ret = __zlg52810_cmd_send(handle,
                                     "UUSE:",
                                      __ZLG52810_ARG_TYPE_STRING,
                                      0,
                                      p_arg);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_TRUE);
            break;
        case AM_ZLG52810_VERS_GET:
            ret = __zlg52810_cmd_send(handle,
                                     "VERS:?",
                                      0,
                                      0,
                                      NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg52810_cmd_result(handle,
                                        __ZLG52810_ARG_TYPE_STRING,
                                        p_arg,
                                        AM_FALSE);
            break;
        case AM_ZLG52810_NREAD :
            *(int *)p_arg = am_rngbuf_nbytes(&handle->rx_rngbuf);
            ret = AM_OK;
            break;

        case AM_ZLG52810_NWRITE :
            *(int *)p_arg = am_rngbuf_nbytes(&handle->tx_rngbuf);
            ret = AM_OK;
            break;

        case AM_ZLG52810_FLUSH :
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->rx_rngbuf);
            am_rngbuf_flush(&handle->tx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_ZLG52810_WFLUSH :
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->tx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_ZLG52810_RFLUSH :
        {
            int key = am_int_cpu_lock();

            am_rngbuf_flush(&handle->rx_rngbuf);
            am_int_cpu_unlock(key);
            ret = AM_OK;
            break;
        }

        case AM_ZLG52810_TIMEOUT:
            handle->timeout_ms = (uint32_t)p_arg;
            ret = AM_OK;
            break;
        default:
            break;
    }
    return ret;
}

/**
 * \brief  zlg52810  low power mode wake-up
 */
int am_zlg52810_lpower_wakeup(am_zlg52810_handle_t  handle)
{
    const am_zlg52810_devinfo_t *p_info = handle->p_devinfo;

    if(p_info->pin_wakeup != -1){
        am_gpio_set(p_info->pin_wakeup, AM_GPIO_OUTPUT_INIT_HIGH);
        am_mdelay(1);
        am_gpio_set(p_info->pin_wakeup, AM_GPIO_OUTPUT_INIT_LOW);
        return AM_OK;
    }else{
        return AM_FALSE;
    }
}

/**
 * \brief  zlg52810  recover factory setting
 */
int am_zlg52810_fareset(am_zlg52810_handle_t  handle)
{
    const am_zlg52810_devinfo_t *p_info = handle->p_devinfo;

    if(p_info->pin_restore != -1){
        am_gpio_set(p_info->pin_restore, AM_GPIO_OUTPUT_INIT_LOW);
        am_mdelay(6000);
        return AM_OK;
    }else{
        return AM_FALSE;
    }
}

/**
 * \brief  zlg52810  reset
 */
int am_zlg52810_reset(am_zlg52810_handle_t  handle)
{
    const am_zlg52810_devinfo_t *p_info = handle->p_devinfo;

    if(p_info->pin_rst != -1){
        am_gpio_set(p_info->pin_restore, AM_GPIO_OUTPUT_INIT_LOW);
        am_mdelay(20);
        am_gpio_set(p_info->pin_restore, AM_GPIO_OUTPUT_INIT_HIGH);
        return AM_OK;
    }else{
        return AM_FALSE;
    }
}
