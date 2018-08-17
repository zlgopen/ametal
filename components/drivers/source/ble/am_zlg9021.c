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
 * \brief ZLG9021 driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-06  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_zlg9021.h"
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

/*******************************************************************************
    type defines
*******************************************************************************/


/*******************************************************************************
    global variables
*******************************************************************************/
/* The max data value with a given length */
am_local const int __g_max_val[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
};

/*******************************************************************************
    local defines
*******************************************************************************/

#define __ZLG9021_DEV_DECL(p_this, handle) \
    am_zlg9021_dev_t *p_this = (am_zlg9021_dev_t *)handle

#define __ZLG9021_CMD_PROC_STAT_NONE       0   /* Not in cmd process      */
#define __ZLG9021_CMD_PROC_STAT_START      1   /* Cmd process startted    */
#define __ZLG9021_CMD_PROC_STAT_SEND       2   /* Cmd send complete       */
#define __ZLG9021_CMD_PROC_STAT_RECV_T_1   3   /* receive ack T           */
#define __ZLG9021_CMD_PROC_STAT_RECV_T_2   4   /* receive ack "TT"        */
#define __ZLG9021_CMD_PROC_STAT_RECV_M     5   /* receive ack "TTM"       */
#define __ZLG9021_CMD_PROC_STAT_RECV_COLON 6   /* receive ack "TTM:"      */
#define __ZLG9021_CMD_PROC_STAT_RECV_LINE  7   /* receive first line      */
#define __ZLG9021_CMD_PROC_STAT_RECV_END   8   /* receive first line      */
#define __ZLG9021_CMD_PROC_STAT_RECV_ERR   9   /* some error happen       */

#define __ZLG9021_STAT_REP_NONE            0   /* report none              */
#define __ZLG9021_STAT_REP_HEADER          1   /* report in "TTM:"         */
#define __ZLG9021_STAT_REP_HEADED_END      2   /* report in "TTM:"         */
#define __ZLG9021_STAT_REP_CONNECT         3   /* report the connect state */
#define __ZLG9021_STAT_REP_DISCONNECT      4   /* report the disconnect state */
#define __ZLG9021_STAT_REP_UNKNOWN         5   /* unknown                     */
#define __ZLG9021_STAT_REP_END             6   /* receive complete            */
#define __ZLG9021_STAT_REP_ERR             7   /* some error happen           */

#define __ZLG9021_STAT_REP_STR_HEADER         "TTM:"
#define __ZLG9021_STAT_REP_STR_HEADER_LEN     4

#define __ZLG9021_STAT_REP_STR_CONNECT        "CONNECTED"
#define __ZLG9021_STAT_REP_STR_CONNECT_LEN    9

#define __ZLG9021_STAT_REP_STR_DISCONNECT     "DISCONNECT"
#define __ZLG9021_STAT_REP_STR_DISCONNECT_LEN 10

#define __ZLG9021_STATE_BLE_NONE          0
#define __ZLG9021_STATE_BLE_CONNECTED     1
#define __ZLG9021_STATE_BLE_DISCONNECT    2

#define __ZLG9021_PIN_EN_ACTIVE        0  /* active zlg9021 while low  */
#define __ZLG9021_PIN_EN_INACTIVE      1  /* active zlg9021 while high */

#define __ZLG9021_BRTS_DELAY_FORWARD   20 /* 拉低 BRTS 引脚到串口发送数据的延时 */
#define __ZLG9021_BRTS_DELAY_AFTER     4  /* 串口发送数据到拉高 BRTS 引脚的延时 */

/*******************************************************************************
    Local functions
*******************************************************************************/

am_local void __zlg9021_timeout_cb (void *p_arg)
{
    am_zlg9021_dev_t *p_this = (am_zlg9021_dev_t *)p_arg;

    int key = am_int_cpu_lock();

    if (p_this->stat_rep_type != __ZLG9021_STAT_REP_NONE) {

        if ((p_this->stat_rep_cnt != 0) &&
            (p_this->stat_rep_cnt <= __ZLG9021_STAT_REP_STR_HEADER_LEN)) {

            am_rngbuf_put(&(p_this->rx_rngbuf),
                          __ZLG9021_STAT_REP_STR_HEADER,
                          p_this->stat_rep_cnt);

            am_wait_done(&(p_this->rx_wait));
        }
        p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
        p_this->stat_rep_cnt  = 0;
    }

    am_int_cpu_unlock(key);

    am_softimer_stop(&(p_this->timer));
}

/******************************************************************************/

am_local void __zlg9021_timeout_brts_cb (void *p_arg)
{
    uint32_t key             = am_int_cpu_lock();
    am_zlg9021_dev_t *p_this = (am_zlg9021_dev_t *)p_arg;

    /**
     * 从软件定时器回调运行到这里，中途被其它中断打断并调用了 __zlg9021_cmd_send()
     * 导致定时器被关闭，这说明取消了当前拉高 brts 的行为，并重新启动了串口发送
     * 所以在这里直接返回即可
     */
    if (am_list_empty(&p_this->timer_brts.node)) {
        return;
    }

    /* stop the timer */
    am_softimer_stop(&(p_this->timer_brts));

    if (p_this->is_brts != AM_FALSE) {
        am_gpio_set(p_this->p_devinfo->pin_brts, 1);
        p_this->is_brts = AM_FALSE;

        /* not in send */
        p_this->in_send = AM_FALSE;

        am_int_cpu_unlock(key);
    } else {
        am_int_cpu_unlock(key);

        /* start up to transfer */
        am_uart_tx_startup(p_this->uart_handle);
    }
}

/******************************************************************************/

am_local int __zlg_stat_report_str_proc (am_zlg9021_dev_t *p_this,
                                         const char       *p_str,
                                         int               len,
                                         char              inchar)
{
    int ret = AM_ERROR;

    /* receive complete */
    if (p_this->stat_rep_cnt == (__ZLG9021_STAT_REP_STR_HEADER_LEN + len)) {

        if (inchar == '\r' || inchar == '\0' || inchar == '\n') {

            ret = AM_OK;
            if (inchar == '\n') {
                p_this->stat_rep_type = __ZLG9021_STAT_REP_END;
            }

        } else {    /* error happen */
            p_this->stat_rep_type = __ZLG9021_STAT_REP_END;
        }
    } else {

        if (inchar ==
            p_str[p_this->stat_rep_cnt - __ZLG9021_STAT_REP_STR_HEADER_LEN]) {

            ret = AM_OK;
            p_this->stat_rep_cnt++;

        } else {                 /* error happen */

#if 0
            /* need put the data into rxbuf? */
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          __ZLG9021_STAT_REP_STR_HEADER,
                          __ZLG9021_STAT_REP_STR_HEADER_LEN);

            am_rngbuf_put(&(p_this->rx_rngbuf),
                          p_str,
                          p_this->stat_rep_cnt -
                          __ZLG9021_STAT_REP_STR_HEADER_LEN);

            am_wait_done(&(p_this->rx_wait));
#endif

            p_this->stat_rep_type = __ZLG9021_STAT_REP_ERR;
        }
    }

    return ret;
}

/******************************************************************************/

am_local int __zlg9021_stat_report_proc (am_zlg9021_dev_t *p_this, char inchar)
{
    /* tx rngbuf for temp save cmd ack */
    const char *p_str = __ZLG9021_STAT_REP_STR_HEADER;

    int ret = AM_ERROR;

    switch (p_this->stat_rep_type) {

    case __ZLG9021_STAT_REP_NONE:
        if (inchar == p_str[0]) {          /* receive 'T' */
            ret = AM_OK;
            p_this->stat_rep_type = __ZLG9021_STAT_REP_HEADER;
            p_this->stat_rep_cnt  = 1;
        }
        break;

    case __ZLG9021_STAT_REP_HEADER:
        if (inchar == p_str[p_this->stat_rep_cnt]) {
            ret = AM_OK;
            p_this->stat_rep_cnt++;

            if (p_this->stat_rep_cnt == __ZLG9021_STAT_REP_STR_HEADER_LEN) {
                p_this->stat_rep_type = __ZLG9021_STAT_REP_HEADED_END;
            }
        } else {    /* error happen */

            am_rngbuf_put(&(p_this->rx_rngbuf),
                          __ZLG9021_STAT_REP_STR_HEADER,
                          p_this->stat_rep_cnt);

            am_wait_done(&(p_this->rx_wait));

            p_this->stat_rep_type = __ZLG9021_STAT_REP_ERR;
        }
        break;

    case __ZLG9021_STAT_REP_HEADED_END:

        p_str = __ZLG9021_STAT_REP_STR_CONNECT;

        if (inchar == p_str[0]) {
            ret = AM_OK;
            p_this->stat_rep_type = __ZLG9021_STAT_REP_CONNECT;
            p_this->stat_rep_cnt++;
        }

        p_str = __ZLG9021_STAT_REP_STR_DISCONNECT;
        if (inchar == p_str[0]) {
            ret = AM_OK;
            p_this->stat_rep_type = __ZLG9021_STAT_REP_DISCONNECT;
            p_this->stat_rep_cnt++;
        }

#if 1
        /* some error happen */
        if (ret != AM_OK) {
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          __ZLG9021_STAT_REP_STR_HEADER,
                          p_this->stat_rep_cnt);

            am_wait_done(&(p_this->rx_wait));

            p_this->stat_rep_type = __ZLG9021_STAT_REP_ERR;
        }
#else
        ret = AM_OK;
        if (inchar == '\n') {
            p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
            p_this->stat_rep_cnt  = 0;
        } else {
            p_this->stat_rep_type =  __ZLG9021_STAT_REP_UNKNOWN;
            p_this->stat_rep_cnt++;
        }
#endif
        break;

    case __ZLG9021_STAT_REP_UNKNOWN:  /* until receice '\n' */

        ret = AM_OK;
        if (inchar == '\n') {
            p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
            p_this->stat_rep_cnt  = 0;
        } else {
            p_this->stat_rep_cnt++;
        }

        break;

    case __ZLG9021_STAT_REP_CONNECT:
        ret = __zlg_stat_report_str_proc(p_this,
                                         __ZLG9021_STAT_REP_STR_CONNECT,
                                         __ZLG9021_STAT_REP_STR_CONNECT_LEN,
                                         inchar);

        if (p_this->stat_rep_type == __ZLG9021_STAT_REP_END) {
            p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
            p_this->stat_rep_cnt  = 0;

            /* First Connected */
            if (p_this->connect_state == __ZLG9021_STATE_BLE_NONE) {
                int key = am_int_cpu_lock();
                am_rngbuf_flush(&p_this->rx_rngbuf);
                am_int_cpu_unlock(key);
            }

            /* find the CONNECT event */
            p_this->connect_state = __ZLG9021_STATE_BLE_CONNECTED;

        }
        break;

    case __ZLG9021_STAT_REP_DISCONNECT:
        ret = __zlg_stat_report_str_proc(p_this,
                                         __ZLG9021_STAT_REP_STR_DISCONNECT,
                                         __ZLG9021_STAT_REP_STR_DISCONNECT_LEN,
                                        inchar);

        if (p_this->stat_rep_type == __ZLG9021_STAT_REP_END) {
            p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
            p_this->stat_rep_cnt  = 0;

            /* find the DISCONNECT event */
            p_this->connect_state = __ZLG9021_STATE_BLE_DISCONNECT;
        }

        break;

    default:
        break;

    }

    if (p_this->stat_rep_type == __ZLG9021_STAT_REP_ERR) {
        p_this->stat_rep_type = __ZLG9021_STAT_REP_NONE;
        p_this->stat_rep_cnt  = 0;
    }

    if (ret == AM_OK) {

        /* timeout is fixed to 20ms */
        am_softimer_start(&(p_this->timer), 20);
    }

    return ret;
}

/******************************************************************************/

am_local int __zlg9021_cmd_ack_recv_proc (am_zlg9021_dev_t *p_this, char inchar)
{
    int ret = AM_ERROR;

    switch (p_this->cmd_proc_state) {

    case __ZLG9021_CMD_PROC_STAT_SEND:  /* Cmd send complete       */
         if (inchar == 'T') {
             ret = AM_OK;
             p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
             p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_T_1;
         }
         break;

    case __ZLG9021_CMD_PROC_STAT_RECV_T_1:
        if (inchar == 'T') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_T_2;
        }
        break;

    case __ZLG9021_CMD_PROC_STAT_RECV_T_2:
        if (inchar == 'M') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_M;
        }
        break;

    case __ZLG9021_CMD_PROC_STAT_RECV_M:
        if (inchar == ':') {
            ret = AM_OK;
            p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_COLON;
        }
        break;

    case __ZLG9021_CMD_PROC_STAT_RECV_COLON:

        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;

        if (inchar == '\n') {
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_LINE;
        }

        break;

    case __ZLG9021_CMD_PROC_STAT_RECV_LINE:

        ret = AM_OK;
        p_this->p_cmd_rxbuf[p_this->cmd_rx_len++] = inchar;

        if (inchar == '\n') {
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_END;
        }

        break;
    }

    if (ret != AM_OK) {

        if (p_this->cmd_rx_len != 0) {  /* not empty, error happen */
            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_ERR;
        }
    } else {

        /* full */
        if ((p_this->cmd_rx_len     == p_this->cmd_rxbuf_len) &&
            (p_this->cmd_proc_state != __ZLG9021_CMD_PROC_STAT_RECV_END)) {

            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_RECV_ERR;
        }
    }

    /* receive one ack data */
    am_wait_done(&p_this->ack_wait);

    return ret;
}

/******************************************************************************/

/**
 * \brief the function that to get one char to transmit.
 */
am_local int __zlg9021_uart_txchar_get (void *p_arg, char *p_outchar)
{
    am_zlg9021_dev_t *p_dev       = (am_zlg9021_dev_t *)p_arg;
    am_rngbuf_t       rb          = &(p_dev->tx_rngbuf);
    uint32_t          baudrate    = 0;
    uint32_t          delay_after = 0;
    uint32_t          key         = am_int_cpu_lock();

    /* Success pop one data from ring buffer */
    if (am_rngbuf_getchar(rb, p_outchar) == 1) {
        am_int_cpu_unlock(key);
        return AM_OK;
    }

    if (p_dev->cmd_proc_state == __ZLG9021_CMD_PROC_STAT_START) {

        p_dev->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_SEND;

        /* send complete  */
        am_wait_done(&p_dev->ack_wait);
    }

    /* set the BRTS to High */
    if (p_dev->p_devinfo->pin_brts != -1) {

        if (am_uart_ioctl(p_dev->uart_handle,
                          AM_UART_BAUD_GET,
                         &baudrate) != AM_OK) {
            delay_after = __ZLG9021_BRTS_DELAY_AFTER;
        } else {
            delay_after = 9 * 1000000 / baudrate + 1000;
            delay_after = AM_DIV_ROUND_UP(delay_after, 1000);
        }

        p_dev->is_brts = AM_TRUE;
        am_softimer_start(&(p_dev->timer_brts), delay_after);
    } else {

        /* not in send */
        p_dev->in_send = AM_FALSE;
    }

    am_int_cpu_unlock(key);

    return -AM_EEMPTY;     /* No data to transmit,return -AM_EEMPTY */
}

/******************************************************************************/

/**
 * \brief the function revived one char.
 */
am_local int __zlg9021_uart_rxchar_put (void *p_arg, char inchar)
{
    am_zlg9021_dev_t *p_dev  = (am_zlg9021_dev_t *)p_arg;
    am_rngbuf_t       rx_rb  = &(p_dev->rx_rngbuf);

    if (p_dev->cmd_proc_state >= __ZLG9021_CMD_PROC_STAT_SEND) {

        /* success process by cmd ack */
        if (__zlg9021_cmd_ack_recv_proc(p_dev, inchar) == AM_OK) {
             return AM_OK;
        }
    }

    /* detect the "TTM:DISCONNECT" and "TTM:CONNECT" */
    if (__zlg9021_stat_report_proc(p_dev, inchar) == AM_OK) {
        return AM_OK;
    }

    /* wait done */
    am_wait_done(&p_dev->rx_wait);

    if (am_rngbuf_putchar(rx_rb, inchar) == 1) {
        return AM_OK;
    }

    return -AM_EFULL;          /* No data to receive,return -AM_EFULL */
}

/******************************************************************************/

am_local int __zlg9021_data_send (am_zlg9021_dev_t    *p_this,
                                  const uint8_t       *p_buf,
                                  int                  len)
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

    if (p_this->in_send == AM_FALSE) {
        p_this->in_send = AM_TRUE;

        /* set the BRTS to low */
        if (p_this->p_devinfo->pin_brts != -1) {

            am_gpio_set(p_this->p_devinfo->pin_brts, 0);

            /* send start up after */
            am_softimer_start(&p_this->timer_brts,
                               __ZLG9021_BRTS_DELAY_FORWARD);

        } else {      /* start up transfer now */

            am_uart_tx_startup(p_this->uart_handle);
        }
    } else {

        /**
         * 如果节点在链表中，那么认为定时器还在运行，
         * 如果正在等待拉高 brts，就可以先发送数据，再拉高 brts，提高发送效率
         */
        if ((p_this->p_devinfo->pin_brts != -1) &&
            (AM_FALSE != p_this->is_brts) &&
            (!am_list_empty(&p_this->timer_brts.node))) {

            am_softimer_stop(&p_this->timer_brts);

            p_this->is_brts = AM_FALSE;

            am_uart_tx_startup(p_this->uart_handle);
        }

    }

    return nbytes - len;
}

/******************************************************************************/

am_local int __zlg9021_data_recv (am_zlg9021_dev_t    *p_this,
                                  uint8_t             *p_buf,
                                  int                  len)
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

/******************************************************************************/
am_local int __zlg9021_cmd_char_send(am_zlg9021_dev_t    *p_this,
                                     char                 ch)
{
    return __zlg9021_data_send(p_this, (const uint8_t *)(&ch), 1);
}

/******************************************************************************/
am_local int __zlg9021_cmd_str_send(am_zlg9021_dev_t    *p_this,
                                    const char          *p_str)
{
    return __zlg9021_data_send(p_this, (const uint8_t *)(p_str), strlen(p_str));
}

/******************************************************************************/
/* Send data (type is int) as a string, eg: send 100, will send "100" string */
am_local int __zlg9021_cmd_uint_send (am_zlg9021_dev_t    *p_this,
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
            __zlg9021_cmd_char_send(p_this, c);

        } else if (first_none_zero || (max_len == 0)){

            /* first none zero data is appear */
            __zlg9021_cmd_char_send(p_this, c);
        }

        data = data % __g_max_val[max_len];
    };

    return 0;
}

/******************************************************************************/
/* Send a data (type is int) as a string, eg: send 100, will send "100" string */
am_local int __zlg9021_cmd_int_send (am_zlg9021_dev_t    *p_this,
                                     int                  data,
                                     int                  max_len)
{
    if (data < 0) {
        __zlg9021_cmd_char_send(p_this, '-');
        data = ~data + 1;
    }

    return __zlg9021_cmd_uint_send(p_this, data, max_len);
}
/******************************************************************************/

/* The arg type for cmd send */
#define __ZLG9021_ARG_TYPE_UINT32      1
#define __ZLG9021_ARG_TYPE_INT         2
#define __ZLG9021_ARG_TYPE_STRING      3
#define __ZLG9021_ARG_TYPE_BOOL        4
#define __ZLG9021_ARG_TYPE_FLOAT       5
#define __ZLG9021_ARG_TYPE_CONNECT     6

/* Send a data (type is int) as a string, eg: send 100, will send "100" string */
am_local int __zlg9021_cmd_send (am_zlg9021_dev_t    *p_this,
                                 const char          *p_hdstr,
                                 int                  arg_type,
                                 int                  max_len,
                                 void                *p_arg,
                                 const char          *p_tlstr)
{

    p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_START;

    /* The tx buffer temp for receive cmd ack, only used after send complete */
    p_this->p_cmd_rxbuf   = p_this->p_devinfo->p_uart_txbuf;
    p_this->cmd_rxbuf_len = p_this->p_devinfo->txbuf_size;
    p_this->cmd_rx_len    = 0;

    /* command always start with "TTM: "*/
    __zlg9021_cmd_str_send(p_this, "TTM:");

    if (p_hdstr) {
        __zlg9021_cmd_str_send(p_this, p_hdstr);
    }

    if (p_arg) {

        if (arg_type == __ZLG9021_ARG_TYPE_UINT32) {

            __zlg9021_cmd_uint_send(p_this, (uint32_t)p_arg, max_len);

        } else if (arg_type == __ZLG9021_ARG_TYPE_INT) {

            __zlg9021_cmd_int_send(p_this, (int)p_arg, max_len);

        } else if (arg_type == __ZLG9021_ARG_TYPE_STRING) {

            __zlg9021_cmd_str_send(p_this, (char *)p_arg);

        } else if (arg_type == __ZLG9021_ARG_TYPE_BOOL) {

            if (p_arg) {
                __zlg9021_cmd_str_send(p_this, "TRUE");
            } else {
                __zlg9021_cmd_str_send(p_this, "FALSE");
            }
        }
    }


    if (p_tlstr) {
        __zlg9021_cmd_str_send(p_this, p_tlstr);
    }

    return AM_OK;
}
/******************************************************************************/
am_local int __zlg9021_cmd_result_info_get (am_zlg9021_dev_t    *p_this,
                                            const char          *p_hdstr,
                                            int                  arg_type,
                                            void                *p_arg,
                                            am_bool_t            need_check)
{
    char *p_ack_str = (char *)(p_this->p_cmd_rxbuf);

    p_this->p_cmd_rxbuf[p_this->cmd_rx_len] = '\0';

    AM_DBG_INFO("ack is %d bytes : \r\n%s",
                p_this->cmd_rx_len,
                p_this->p_cmd_rxbuf);

    /* Only the "RST-SYSTEMRESET" needn't check */
    if (need_check) {

        if (p_arg == NULL) {
            if (strstr(p_ack_str,"TTM:ERR") != NULL) {
                return AM_ERROR;
            } else {
                return AM_OK;
            }
        } else {

            char *p_start = NULL;
            char *p_end   = NULL;

            /* error happen */
            if (strstr(p_ack_str,"TTM:ERR") != NULL) {
                return AM_ERROR;
            }

            /* Get the data for p_arg */
            p_start = strstr(p_ack_str, "TTM:");

            if (p_start == NULL) {
                return AM_ERROR;
            }

            p_start += 4;  /* Skip The first "TTM: "*/

            p_start = strstr(p_start, "TTM:");

            if (p_start == NULL) {
                return AM_ERROR;
            }

            p_start += 4;  /* Skip The Second "TTM: "*/

            p_end = p_start;

            while (*p_end != '\0') {
                if ((*p_end == '\r') || (*p_end == '\n')) {
                    *p_end = '\0';
                    break;
                }
                p_end++;
            }

            /* Skip the header in ack, need compare? */
            if (p_hdstr) {
                p_start += strlen(p_hdstr);
            }

            if (p_start >= p_end) {
                return AM_ERROR;
            }

            if (arg_type == __ZLG9021_ARG_TYPE_UINT32) {
                *(uint32_t *)p_arg = strtoul(p_start, &p_end, 10);
                if (p_end == p_start) {
                    return AM_ERROR;
                }
            } else if (arg_type == __ZLG9021_ARG_TYPE_INT) {
                *(int *)p_arg = strtol(p_start, &p_end, 10);
                if (p_end == p_start) {
                    return AM_ERROR;
                }
            } else if (arg_type == __ZLG9021_ARG_TYPE_BOOL) {

                if (strncmp(p_start, "TRUE", 4) == 0) {
                    *(am_bool_t *)p_arg = AM_TRUE;
                } else if (strncmp(p_start, "FALSE", 5) == 0) {
                    *(am_bool_t *)p_arg = AM_FALSE;
                } else {
                    return AM_ERROR;
                }

            } else if (arg_type == __ZLG9021_ARG_TYPE_STRING) {
                strcpy(p_arg, p_start);
            } else if (arg_type == __ZLG9021_ARG_TYPE_FLOAT){

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
            } else if (arg_type == __ZLG9021_ARG_TYPE_CONNECT) {
                if (strncmp(p_start, "CONNECT", 7) == 0) {
                    *(am_bool_t *)p_arg = AM_TRUE;
                } else if (strncmp(p_start, "DISCONNECT", 10) == 0) {
                    *(am_bool_t *)p_arg = AM_FALSE;
                } else {
                    return AM_ERROR;
                }
            } else {
                return AM_ERROR;
            }
        }
    }

    return AM_OK;
}

/******************************************************************************/
am_local int __zlg9021_cmd_result (am_zlg9021_dev_t    *p_this,
                                   const char          *p_hdstr,
                                   int                  arg_type,
                                   void                *p_arg,
                                   am_bool_t            need_check)
{
    int          key;

    /* 等待发送完成 */
    am_wait_on(&p_this->ack_wait);

    am_rngbuf_init(&(p_this->tx_rngbuf),
                    (char *)p_this->p_devinfo->p_uart_txbuf,
                    p_this->p_devinfo->txbuf_size);

    while (1) {

        /* wait 200ms (actual about 30~150)  */
        if (am_wait_on_timeout(&p_this->ack_wait, 200) == AM_OK) {

            key = am_int_cpu_lock();

            /* success process */
            if (p_this->cmd_proc_state == __ZLG9021_CMD_PROC_STAT_RECV_END) {

                p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_NONE;
                am_int_cpu_unlock(key);

                return __zlg9021_cmd_result_info_get(p_this,
                                                     p_hdstr,
                                                     arg_type,
                                                     p_arg,
                                                     need_check);
            }

            if (p_this->cmd_proc_state != __ZLG9021_CMD_PROC_STAT_RECV_ERR) {

                am_int_cpu_unlock(key);
                continue;
            }

            am_int_cpu_unlock(key);
        }

        /* run to here, some error happen */
        key = am_int_cpu_lock();

        /* received "TTM:", same as successful */
        if (p_this->cmd_proc_state >= __ZLG9021_CMD_PROC_STAT_RECV_COLON) {

            p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_NONE;
            am_int_cpu_unlock(key);

            return __zlg9021_cmd_result_info_get(p_this,
                                                 p_hdstr,
                                                 arg_type,
                                                 p_arg,
                                                 need_check);
        }

        /* move the data to rx rngbuf */
        if (p_this->cmd_rx_len != 0) {
            am_rngbuf_put(&(p_this->rx_rngbuf),
                          (const char *)(p_this->p_cmd_rxbuf),
                           p_this->cmd_rx_len);

            am_wait_done(&(p_this->rx_wait));
        }

        p_this->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_NONE;

        am_int_cpu_unlock(key);

        return AM_ERROR;
    }
}

/*******************************************************************************
    public functions
*******************************************************************************/

am_zlg9021_handle_t am_zlg9021_init (am_zlg9021_dev_t           *p_dev,
                                     const am_zlg9021_devinfo_t *p_devinfo,
                                     am_uart_handle_t            uart_handle)
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
    p_dev->cmd_proc_state = __ZLG9021_CMD_PROC_STAT_NONE;
    p_dev->stat_rep_cnt   = 0;
    p_dev->stat_rep_type  = __ZLG9021_STAT_REP_NONE;
    p_dev->connect_state  = __ZLG9021_STATE_BLE_NONE;

    am_wait_init(&(p_dev->rx_wait));
    am_wait_init(&(p_dev->ack_wait));

    /* 模块使能引脚  0：使能模块蓝牙功能 1：模块禁用，并进入极低功耗模式 */
    if (p_devinfo->pin_en != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_en,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_LOW);
    }

    /* 串口发送数据之前拉低此引脚 */
    if (p_devinfo->pin_brts != -1) {
        am_gpio_pin_cfg(p_dev->p_devinfo->pin_brts,
                        AM_GPIO_PUSH_PULL | AM_GPIO_OUTPUT_INIT_HIGH);
        p_dev->is_brts = AM_FALSE;
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

    p_dev->in_send = AM_FALSE;
    am_softimer_init(&(p_dev->timer_brts), __zlg9021_timeout_brts_cb, p_dev);

    am_softimer_init(&(p_dev->timer), __zlg9021_timeout_cb, p_dev);

    am_uart_ioctl(uart_handle, AM_UART_BAUD_SET, (void *)(p_devinfo->baudrate));

    /* Initialize the ring-buffer */
    am_rngbuf_init(&(p_dev->rx_rngbuf),
                   (char *)p_devinfo->p_uart_rxbuf,
                   p_devinfo->rxbuf_size);

    am_rngbuf_init(&(p_dev->tx_rngbuf),
                   (char *)p_devinfo->p_uart_txbuf,
                   p_devinfo->txbuf_size);

    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
        (int (*)(void *))__zlg9021_uart_txchar_get,
                         (void *)(p_dev));

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
        (int (*)(void *))__zlg9021_uart_rxchar_put,
                         (void *)(p_dev));

    return p_dev;
}

/******************************************************************************/
int am_zlg9021_ioctl (am_zlg9021_handle_t  handle,
                      int                  cmd,
                      void                *p_arg)
{
    int ret = -AM_EIO;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    switch(cmd) {

    case  AM_ZLG9021_BLE_CONNECT_INTERVAL_SET:
        ret = __zlg9021_cmd_send(handle,
                                "CIT-",
                                 __ZLG9021_ARG_TYPE_UINT32,
                                 4,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_BLE_CONNECT_INTERVAL_GET:
        ret = __zlg9021_cmd_send(handle, "CIT-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "CIT-",
                                   __ZLG9021_ARG_TYPE_UINT32,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_RENAME:
        ret = __zlg9021_cmd_send(handle,
                                "REN-",
                                 __ZLG9021_ARG_TYPE_STRING,
                                 4,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_RENAME_MAC_STR:
        ret = __zlg9021_cmd_send(handle,
                                "RNI-",
                                 __ZLG9021_ARG_TYPE_STRING,
                                 4,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_NAME_GET:
        ret = __zlg9021_cmd_send(handle, "REN-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "REN-",
                                   __ZLG9021_ARG_TYPE_STRING,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_BAUD_SET:
        ret = __zlg9021_cmd_send(handle,
                                "BPS-",
                                 __ZLG9021_ARG_TYPE_UINT32,
                                 6,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);

        /* change the baudrate */
        am_uart_ioctl(handle->uart_handle, AM_UART_BAUD_SET, (void *)p_arg);

        break;

    case  AM_ZLG9021_BAUD_GET:
        ret = __zlg9021_cmd_send(handle, "BPS-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "BPS-",
                                   __ZLG9021_ARG_TYPE_UINT32,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_MAC_GET:
        ret = __zlg9021_cmd_send(handle, "MAC-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "MAC-",
                                   __ZLG9021_ARG_TYPE_STRING,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_RESET:

        if (handle->p_devinfo->pin_rst != -1) {

            am_gpio_set(handle->p_devinfo->pin_rst, 0);
            am_mdelay(1);
            am_gpio_set(handle->p_devinfo->pin_rst, 1);
            ret = AM_OK;
            break;

        } else {

            ret = __zlg9021_cmd_send(handle, "RST-SYSTEMRESET", 0, 0, NULL, NULL);
            if (ret != AM_OK) {
                return ret;
            }
            ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_FALSE);
            break;
        }

    case  AM_ZLG9021_BLE_ADV_PERIOD_SET:
        ret = __zlg9021_cmd_send(handle,
                                "ADP-(",
                                 __ZLG9021_ARG_TYPE_UINT32,
                                 2,
                                 (void *)(((uint32_t)p_arg) / 100),
                                 ")");
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_BLE_ADV_PERIOD_GET:
        ret = __zlg9021_cmd_send(handle, "ADP-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "ADP-(",
                                   __ZLG9021_ARG_TYPE_UINT32,
                                   p_arg,
                                   AM_TRUE);

        (*(uint32_t *)p_arg) *= 100;
        break;

    case  AM_ZLG9021_TX_POWER_SET:
        ret = __zlg9021_cmd_send(handle,
                                "TPL-(",
                                 __ZLG9021_ARG_TYPE_INT,
                                 2,
                                 p_arg,
                                 ")");
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_TX_POWER_GET:
        ret = __zlg9021_cmd_send(handle, "TPL-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "TPL-(",
                                   __ZLG9021_ARG_TYPE_INT,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_BCTS_DELAY_SET:
        ret = __zlg9021_cmd_send(handle,
                                "CDL-",
                                 __ZLG9021_ARG_TYPE_UINT32,
                                 2,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_BCTS_DELAY_GET:
        ret = __zlg9021_cmd_send(handle, "CDL-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "CDL-",
                                   __ZLG9021_ARG_TYPE_UINT32,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_POWERDOWN:
    {
        am_bool_t en = (am_bool_t)(int)p_arg;         /* powerdown while TRUE */

        if (handle->p_devinfo->pin_en != -1) {
            am_gpio_set(handle->p_devinfo->pin_en,
                        (en == AM_FALSE) ?
                        __ZLG9021_PIN_EN_ACTIVE :   /* exit power down  */
                        __ZLG9021_PIN_EN_INACTIVE); /* enter power down */

            ret = AM_OK;

            break;

        } else {

            if (en) {  /* enter power down */

                ret = __zlg9021_cmd_send(handle, "PWR-POWERDOWN", 0, 0, NULL, NULL);
                if (ret != AM_OK) {
                    return ret;
                }
                ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);

                break;

            } else {  /* exit power down  */

                /* unused en, so let the brts filp-flop */
                if (handle->p_devinfo->pin_brts != -1) {
                    am_gpio_set(handle->p_devinfo->pin_brts, 0);
                    am_gpio_set(handle->p_devinfo->pin_brts, 1);
                    am_gpio_set(handle->p_devinfo->pin_brts, 0);
                    am_gpio_set(handle->p_devinfo->pin_brts, 1);

                    ret = AM_OK;
                    break;
                }

                ret = AM_ERROR;
                break;
            }
        }
    }

    case  AM_ZLG9021_VERSION_GET:
        ret = __zlg9021_cmd_send(handle, "VER-VERSION", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "V",
                                   __ZLG9021_ARG_TYPE_FLOAT,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_CONSTATE_GET:
        ret = __zlg9021_cmd_send(handle, "CON-CONSTATE", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   NULL,
                                   __ZLG9021_ARG_TYPE_CONNECT,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_DISCONNECT:
        ret = __zlg9021_cmd_send(handle, "DSC-DISCON", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_PWD_SET:
        ret = __zlg9021_cmd_send(handle,
                                "PWD-",
                                 __ZLG9021_ARG_TYPE_STRING,
                                 0,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_PWD_GET:
        ret = __zlg9021_cmd_send(handle, "PWD-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "PWD-",
                                   __ZLG9021_ARG_TYPE_STRING,
                                   p_arg,
                                   AM_TRUE);
        break;

    case  AM_ZLG9021_ENC_SET:
        ret = __zlg9021_cmd_send(handle,
                                "ENC-",
                                 __ZLG9021_ARG_TYPE_BOOL,
                                 0,
                                 p_arg,
                                 NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle, NULL, 0, NULL, AM_TRUE);
        break;

    case  AM_ZLG9021_ENC_GET:
        ret = __zlg9021_cmd_send(handle, "ENC-?", 0, 0, NULL, NULL);
        if (ret != AM_OK) {
            return ret;
        }
        ret = __zlg9021_cmd_result(handle,
                                   "ENC-",
                                   __ZLG9021_ARG_TYPE_BOOL,
                                   p_arg,
                                   AM_TRUE);
        break;

    case AM_ZLG9021_NREAD :
        *(int *)p_arg = am_rngbuf_nbytes(&handle->rx_rngbuf);
        ret = AM_OK;
        break;

    case AM_ZLG9021_NWRITE :
        *(int *)p_arg = am_rngbuf_nbytes(&handle->tx_rngbuf);
        ret = AM_OK;
        break;

    case AM_ZLG9021_FLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&handle->rx_rngbuf);
        am_rngbuf_flush(&handle->tx_rngbuf);
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_ZLG9021_WFLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&handle->tx_rngbuf);
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_ZLG9021_RFLUSH :
    {
        int key = am_int_cpu_lock();

        am_rngbuf_flush(&handle->rx_rngbuf);
        am_int_cpu_unlock(key);
        ret = AM_OK;
        break;
    }

    case AM_ZLG9021_TIMEOUT:
        handle->timeout_ms = (uint32_t)p_arg;
        ret = AM_OK;
        break;

    case AM_ZLG9021_RESTORE:
    {
        if (handle->p_devinfo->pin_restore != -1) {
             am_gpio_pin_cfg(handle->p_devinfo->pin_restore,
                             AM_GPIO_OUTPUT_INIT_LOW);

             /* 低电平持续10s */
             am_mdelay((10 + 1) * 000);

             am_gpio_set(handle->p_devinfo->pin_restore, 1);

             ret = AM_OK;

        } else {
            ret = AM_ERROR;
        }
        break;
    }

    default:
        break;
    }

    return ret;
}

/******************************************************************************/
int am_zlg9021_send (am_zlg9021_handle_t  handle,
                     const uint8_t       *p_buf,
                     int                  len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    /* in cmd process, can't send data now */
    if (handle->cmd_proc_state != __ZLG9021_CMD_PROC_STAT_NONE) {
        return -AM_EPERM;
    }

    return __zlg9021_data_send(handle, p_buf, len);
}

/******************************************************************************/
int am_zlg9021_recv (am_zlg9021_handle_t  handle,
                     uint8_t             *p_buf,
                     int                  len)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    return __zlg9021_data_recv(handle, p_buf, len);
}

/* end of file */
