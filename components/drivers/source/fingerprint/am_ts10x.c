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
 * \brief ts10x driver implement
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-06  tee, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_ts10x.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_uart.h"
#include "am_uart_rngbuf.h"
#include "string.h"
#include "am_vdebug.h"
#include "am_int.h"
#include "am_wait.h"
#include "am_softimer.h"

/*******************************************************************************
    type defines
*******************************************************************************/
typedef struct {
    const void * (*pfn_process) (am_ts10x_dev_t *p_dev, uint8_t inchar);
    const void * (*pfn_timeout) (am_ts10x_dev_t *p_dev);
} __ts10x_recv_state_t;

typedef struct {
    const void * (*pfn_process) (am_ts10x_dev_t *p_dev, uint8_t *, int *);
} __ts10x_send_state_t;

/*******************************************************************************
    local defines
*******************************************************************************/

#define __TS10X_FEATURE_LEN                494

#define __TS10X_UART_BAUDRATE              115200

/* The timeout set to 10s */
#define __TS10X_TIMEOUT_CHK_PERIOD         10
#define __TS10X_TIMEOUT_TIME_RECVED        20
#define __TS10X_TIMEOUT_TIME_COLLECT       15000
#define __TS10X_TIMEOUT_TIME_DEFAULT       1000


#define __TS10X_TIMEOUT_VAL_CLEAR(p_dev)    \
        do {                                \
            int key = am_int_cpu_lock();    \
            p_dev->timeout_val = 0;         \
            am_int_cpu_unlock(key);         \
        } while(0);

#define __TS10X_STATE_NONE              0
#define __TS10X_STATE_CMD_SEND          1
#define __TS10X_STATE_ACK_RECV_FIRST    2  /* The first segment of ack  */
#define __TS10X_STATE_ACK_RECV_SECOND   3  /* The second segment of ack */
#define __TS10X_STATE_ACK_RECV_COMPLETE 4  /* ack receive complete      */
#define __TS10X_STATE_ACK_RECV_ERROR    5  /* ack receive error         */

#define __TS10X_PACKET_LEN          (0x08)

#define __TS10X_PACKET_HEAD         (0xF5)
#define __TS10X_PACKET_TAIL         (0xF5)

#define __TS10X_PACKET_TYPE_REGISTER_1                       0x01
#define __TS10X_PACKET_TYPE_REGISTER_2                       0x02
#define __TS10X_PACKET_TYPE_REGISTER_3                       0x03

#define __TS10X_PACKET_TYPE_USER_ONE_DEL                     0x04
#define __TS10X_PACKET_TYPE_USER_ALL_DEL                     0x05

#define __TS10X_PACKET_TYPE_USER_COUNT_GET                   0x09
#define __TS10X_PACKET_TYPE_USER_ROLE_GET                    0x0A
#define __TS10X_PACKET_TYPE_COMPARE_1_WITH_1                 0x0B
#define __TS10X_PACKET_TYPE_COMPARE_1_WITH_N                 0x0C
#define __TS10X_PACKET_TYPE_UNUSED_USER_ID_GET               0x0D

#define __TS10X_PACKET_TYPE_BAUDRATE_SET                     0x21

#define __TS10X_PACKET_TYPE_IMAGE_FEATURE_GET                0x23
#define __TS10X_PACKET_TYPE_IMAGE_DATA_GET                   0x24

#define __TS10X_PACKET_TYPE_VERSION_GET                      0x26

#define __TS10X_PACKET_TYPE_COMPARE_LEVEL_SET                0x28

#define __TS10X_PACKET_TYPE_USER_ALL_ID_ROLE_GET             0x2B

#define __TS10X_PACKET_TYPE_REGISTER_MODE                    0x2D

#define __TS10X_PACKET_TYPE_USER_FEATURE_GET                 0x31

#define __TS10X_PACKET_TYPE_USER_ADD_WITH_FEATURE            0x41
#define __TS10X_PACKET_TYPE_COMPARE_FEATURE_WITH_ONE_USER    0x42
#define __TS10X_PACKET_TYPE_COMPARE_FEATURE_WITH_ALL_USER    0x43
#define __TS10X_PACKET_TYPE_COMPARE_COLLECT_WITH_FEATURE     0x44

/*******************************************************************************
    local function declare
*******************************************************************************/

am_local const void * __ts10x_send_first_head (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_first_type (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_first_data (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_first_chk (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_first_tail (am_ts10x_dev_t *, uint8_t *, int *);

am_local const void * __ts10x_send_sec_head (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_sec_data (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_sec_chk (am_ts10x_dev_t *, uint8_t *, int *);
am_local const void * __ts10x_send_sec_tail (am_ts10x_dev_t *, uint8_t *, int *);

am_local const void * __ts10x_send_idle (am_ts10x_dev_t *, uint8_t *, int *);


/* the head represent should receive the head data in the next input char */
am_local const void * __ts10x_recv_first_head (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_first_type (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_first_data (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_first_chk (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_first_tail (am_ts10x_dev_t *, uint8_t );

am_local const void * __ts10x_recv_sec_head (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_sec_data (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_sec_chk (am_ts10x_dev_t *, uint8_t );
am_local const void * __ts10x_recv_sec_tail (am_ts10x_dev_t *, uint8_t );

am_local const void * __ts10x_recv_idle (am_ts10x_dev_t *, uint8_t );

am_local const void * __ts10x_recv_timeout_at_first (am_ts10x_dev_t *p_dev);
am_local const void * __ts10x_recv_timeout_at_sec (am_ts10x_dev_t *p_dev);
am_local const void * __ts10x_recv_timeout_at_idle (am_ts10x_dev_t *p_dev);

am_local void __ts10x_proc_complete_at_first (am_ts10x_dev_t *, int );
am_local void __ts10x_proc_complete_at_sec (am_ts10x_dev_t *, int );

am_local void __ts10x_proc_complete_proc (am_ts10x_dev_t *p_dev, int status);

am_local int __ts10x_cmd_send_start (am_ts10x_dev_t *p_dev);
/*******************************************************************************
    global receive state defines
*******************************************************************************/
am_local const __ts10x_send_state_t  __g_ts10x_send_first_head_state = {
        __ts10x_send_first_head,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_first_type_state = {
        __ts10x_send_first_type,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_first_data_state = {
        __ts10x_send_first_data,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_first_chk_state = {
        __ts10x_send_first_chk,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_first_tail_state = {
        __ts10x_send_first_tail,
};

/******************************************************************************/

am_local const __ts10x_send_state_t  __g_ts10x_send_sec_head_state = {
        __ts10x_send_sec_head,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_sec_data_state = {
        __ts10x_send_sec_data,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_sec_chk_state = {
        __ts10x_send_sec_chk,
};

am_local const __ts10x_send_state_t  __g_ts10x_send_sec_tail_state = {
        __ts10x_send_sec_tail,
};

/******************************************************************************/

am_local const __ts10x_send_state_t  __g_ts10x_send_idle_state = {
        __ts10x_send_idle,
};

/******************************************************************************/
am_local const __ts10x_recv_state_t  __g_ts10x_recv_first_head_state = {
        __ts10x_recv_first_head,
        __ts10x_recv_timeout_at_first
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_first_type_state = {
        __ts10x_recv_first_type,
        __ts10x_recv_timeout_at_first
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_first_data_state = {
        __ts10x_recv_first_data,
        __ts10x_recv_timeout_at_first
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_first_chk_state = {
        __ts10x_recv_first_chk,
        __ts10x_recv_timeout_at_first
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_first_tail_state = {
        __ts10x_recv_first_tail,
        __ts10x_recv_timeout_at_first
};

/******************************************************************************/

am_local const __ts10x_recv_state_t  __g_ts10x_recv_sec_head_state = {
        __ts10x_recv_sec_head,
        __ts10x_recv_timeout_at_sec
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_sec_data_state = {
        __ts10x_recv_sec_data,
        __ts10x_recv_timeout_at_sec
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_sec_chk_state = {
        __ts10x_recv_sec_chk,
        __ts10x_recv_timeout_at_sec
};

am_local const __ts10x_recv_state_t  __g_ts10x_recv_sec_tail_state = {
        __ts10x_recv_sec_tail,
        __ts10x_recv_timeout_at_sec
};

/******************************************************************************/

am_local const __ts10x_recv_state_t  __g_ts10x_recv_idle_state = {
        __ts10x_recv_idle,
        __ts10x_recv_timeout_at_idle
};
/*******************************************************************************
    global variables
*******************************************************************************/

/*******************************************************************************
    The CMD Send local state functions
*******************************************************************************/

am_local const void * __ts10x_send_first_head (am_ts10x_dev_t *p_dev,
                                               uint8_t           *p_outchar,
                                               int            *p_ret)
{
    *p_outchar = __TS10X_PACKET_HEAD;
    *p_ret     = AM_OK;

    return &__g_ts10x_send_first_type_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_first_type (am_ts10x_dev_t *p_dev,
                                               uint8_t           *p_outchar,
                                               int            *p_ret)
{
    *p_outchar        = p_dev->cmd_ack_type;
    *p_ret            = AM_OK;
    p_dev->data_index = 0;
    p_dev->chk_value  = p_dev->cmd_ack_type;

    return &__g_ts10x_send_first_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_first_data (am_ts10x_dev_t *p_dev,
                                               uint8_t           *p_outchar,
                                               int            *p_ret)
{
    *p_outchar = p_dev->data[p_dev->data_index++];
    *p_ret     = AM_OK;

    p_dev->chk_value ^= (*p_outchar);

    /* For Send the data length exclude head, type, check, tail */
    if (p_dev->data_index == __TS10X_PACKET_LEN - 4) {
        return &__g_ts10x_send_first_chk_state;
    }

    /* The data send not complete, the state not change */
    return &__g_ts10x_send_first_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_first_chk (am_ts10x_dev_t *p_dev,
                                              uint8_t           *p_outchar,
                                              int            *p_ret)
{
    *p_outchar = p_dev->chk_value;
    *p_ret     = AM_OK;

    /* The data send not complete, the state not change */
    return &__g_ts10x_send_first_tail_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_first_tail (am_ts10x_dev_t *p_dev,
                                               uint8_t           *p_outchar,
                                               int            *p_ret)
{
    *p_outchar = __TS10X_PACKET_TAIL;
    *p_ret     = AM_OK;

    /* need send second packet */
    if (p_dev->is_send_sec) {
        return &__g_ts10x_send_sec_head_state;
    }
    /* ready to receive the head of first packet */
    p_dev->p_rx_state = &__g_ts10x_recv_first_head_state;

    /* The timeout set to 10s */
    p_dev->timeout_expect_val = p_dev->timeout_cmd_start;
    p_dev->timeout_val        = 0;

    am_softimer_start(&(p_dev->timer), __TS10X_TIMEOUT_CHK_PERIOD);

    /* The data send complete, the state change to IDLE */
    return &__g_ts10x_send_idle_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_sec_head (am_ts10x_dev_t *p_dev,
                                             uint8_t           *p_outchar,
                                             int            *p_ret)
{
    *p_outchar = __TS10X_PACKET_HEAD;
    *p_ret     = AM_OK;

    p_dev->data_index    = 0;
    p_dev->sec_buf_index = 0;
    p_dev->chk_value     = 0;

    return &__g_ts10x_send_sec_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_sec_data (am_ts10x_dev_t *p_dev,
                                             uint8_t           *p_outchar,
                                             int            *p_ret)
{
    const uint8_t *p_buf = p_dev->send_sec_buf_info[p_dev->sec_buf_index].p_buf;
    size_t         size  = p_dev->send_sec_buf_info[p_dev->sec_buf_index].expect_len;

    *p_outchar = p_buf[p_dev->data_index++];
    *p_ret     = AM_OK;

    p_dev->chk_value ^= (*p_outchar);

    if (size == p_dev->data_index) {

        p_dev->data_index = 0;
        p_dev->sec_buf_index++;

        /* all buf send complete */
        if (p_dev->sec_buf_index == p_dev->send_sec_num) {
            return &__g_ts10x_send_sec_chk_state;
        }
    }

    return &__g_ts10x_send_sec_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_sec_chk (am_ts10x_dev_t *p_dev,
                                            uint8_t           *p_outchar,
                                            int            *p_ret)
{
    *p_outchar = p_dev->chk_value;
    *p_ret     = AM_OK;

    /* The data send not complete, the state not change */
    return &__g_ts10x_send_sec_tail_state;
}

/******************************************************************************/
am_local const void * __ts10x_send_sec_tail (am_ts10x_dev_t *p_dev,
                                             uint8_t           *p_outchar,
                                             int            *p_ret)
{
    *p_outchar = __TS10X_PACKET_TAIL;
    *p_ret     = AM_OK;

    /* ready to receive the head of first packet */
    p_dev->p_rx_state = &__g_ts10x_recv_first_head_state;

    /* The timeout set to 10s */
    p_dev->timeout_expect_val = p_dev->timeout_cmd_start;
    p_dev->timeout_val        = 0;

    am_softimer_start(&(p_dev->timer), __TS10X_TIMEOUT_CHK_PERIOD);

    /* The data send complete, the state change to IDLE */
    return &__g_ts10x_send_idle_state;
}

/******************************************************************************/

am_local const void * __ts10x_send_idle  (am_ts10x_dev_t *p_dev,
                                          uint8_t           *p_outchar,
                                          int            *p_ret)
{
    /* Cmd Send Complete or cmd send is not start up */
    *p_ret = -AM_EEMPTY;

    /* the state change do not change */
    return &__g_ts10x_send_idle_state;
}

/*******************************************************************************
    The ACK Receive local state functions
*******************************************************************************/
am_local const void * __ts10x_recv_first_head (am_ts10x_dev_t *p_dev,
                                               uint8_t         inchar)
{
    if (inchar != __TS10X_PACKET_HEAD) {

        /* some error happen */
        __ts10x_proc_complete_at_first(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }

    p_dev->timeout_expect_val = __TS10X_TIMEOUT_TIME_RECVED;

    return &__g_ts10x_recv_first_type_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_first_type (am_ts10x_dev_t *p_dev,
                                               uint8_t            inchar)
{

    /* Don't match the type field */
#if 0
    if (inchar != p_dev->cmd_ack_type) {

        /* some error happen */
        __ts10x_proc_complete_at_first(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }
#else
    p_dev->cmd_ack_type = inchar;
#endif

    p_dev->data_index = 0;
    p_dev->chk_value  = inchar;     /* the check initial value */

    return &__g_ts10x_recv_first_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_first_data (am_ts10x_dev_t *p_dev,
                                               uint8_t            inchar)
{
    /* save data to buffer, the buffer can not smaller than 8 */
    p_dev->data[p_dev->data_index++] = inchar;

    /* calculate the check value */
    p_dev->chk_value ^= inchar;

    /* For receive the data length exclude head, type, check, tail */
    if (p_dev->data_index == __TS10X_PACKET_LEN - 4) {
        return &__g_ts10x_recv_first_chk_state;
    }

    /* The state is not change */
    return &__g_ts10x_recv_first_data_state;

}

/******************************************************************************/
am_local const void * __ts10x_recv_first_chk (am_ts10x_dev_t *p_dev,
                                              uint8_t            inchar)
{
    if (inchar != p_dev->chk_value) {

        /* some error happen */
        __ts10x_proc_complete_at_first(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }

    return &__g_ts10x_recv_first_tail_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_first_tail (am_ts10x_dev_t *p_dev,
                                               uint8_t            inchar)
{
    if (inchar != __TS10X_PACKET_TAIL) {

        /* some error happen */
        __ts10x_proc_complete_at_first(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }

    if (p_dev->is_recv_sec) {

        /* calculate the real data length */
        p_dev->max_length = (p_dev->data[0] << 8) | p_dev->data[1];

        if (p_dev->max_length != 0) {

            return &__g_ts10x_recv_sec_head_state;
        }
    }

    /* The packet receive successful */
    __ts10x_proc_complete_at_first(p_dev, AM_TS10X_SUCCESS);

    return &__g_ts10x_recv_idle_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_sec_head (am_ts10x_dev_t *p_dev,
                                             uint8_t            inchar)
{
    if (inchar != __TS10X_PACKET_HEAD) {

        /* some error happen */
        __ts10x_proc_complete_at_sec(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }
    p_dev->data_index    = 0;
    p_dev->chk_value     = 0;
    p_dev->sec_buf_index = 0;

    return &__g_ts10x_recv_sec_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_sec_data (am_ts10x_dev_t *p_dev,
                                             uint8_t            inchar)
{
    /* save the data into multi segment */
    uint8_t *p_buf = p_dev->recv_sec_buf_info[p_dev->sec_buf_index].p_buf;
    size_t   size  = p_dev->recv_sec_buf_info[p_dev->sec_buf_index].expect_len;

    if (p_buf) {
         p_buf[p_dev->data_index++] = inchar;
    } else {
         p_dev->data_index++;  /* the data be discarded */
    }

    p_dev->chk_value ^= inchar;
    p_dev->max_length--;

    if (p_dev->max_length == 0) {          /* receive complete */
        return &__g_ts10x_recv_sec_chk_state;
    } else {

        if (p_dev->data_index == size) {  /* The buf segment receive complete */

            p_dev->data_index = 0;
            p_dev->sec_buf_index++;
            if (p_dev->sec_buf_index == p_dev->recv_sec_num) {

                /* The buf is full, can't save more data */
                __ts10x_proc_complete_at_sec(p_dev, -AM_ENOMEM);

                /* change to IDLE_STATE */
                return &__g_ts10x_recv_idle_state;
            }
        }
    }

    /* The state is not change */
    return &__g_ts10x_recv_sec_data_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_sec_chk (am_ts10x_dev_t *p_dev,
                                            uint8_t            inchar)
{
    if (inchar != p_dev->chk_value) {

        /* some error happen */
        __ts10x_proc_complete_at_sec(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }

    return &__g_ts10x_recv_sec_tail_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_sec_tail (am_ts10x_dev_t *p_dev,
                                             uint8_t            inchar)
{
    if (inchar != __TS10X_PACKET_TAIL) {

        /* some error happen */
        __ts10x_proc_complete_at_sec(p_dev, AM_TS10X_FAIL);

        /* change to IDLE_STATE */
        return &__g_ts10x_recv_idle_state;
    }

    /* The packet receive successful */
    __ts10x_proc_complete_at_sec(p_dev, AM_TS10X_SUCCESS);

    return &__g_ts10x_recv_idle_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_idle (am_ts10x_dev_t *p_dev, uint8_t inchar)
{
    /* the state is not change */
    return &__g_ts10x_recv_idle_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_timeout_at_first (am_ts10x_dev_t *p_dev)
{
    /* timeout */
    __ts10x_proc_complete_at_first(p_dev, -AM_ETIME);

    /* change to IDLE_STATE */
    return &__g_ts10x_recv_idle_state;
}

/******************************************************************************/

am_local const void * __ts10x_recv_timeout_at_sec (am_ts10x_dev_t *p_dev)
{
    /* timeout */
    __ts10x_proc_complete_at_sec(p_dev, -AM_ETIME);

    /* change to IDLE_STATE */
    return &__g_ts10x_recv_idle_state;
}

/******************************************************************************/
am_local const void * __ts10x_recv_timeout_at_idle (am_ts10x_dev_t *p_dev)
{
    /* the state is not change */
     return &__g_ts10x_recv_idle_state;
}

/*******************************************************************************
    The ACK receive Timeout Process functions
*******************************************************************************/

am_local void __ts10x_timeout_callback (void *p_arg)
{
    am_ts10x_dev_t *p_dev = (am_ts10x_dev_t *)p_arg;

    p_dev->timeout_val += __TS10X_TIMEOUT_CHK_PERIOD;

    if (p_dev->timeout_val >= p_dev->timeout_expect_val) {

        p_dev->timeout_val = 0;

        p_dev->p_rx_state = ((__ts10x_recv_state_t *)(p_dev->p_rx_state))
                             ->pfn_timeout(p_arg);

        if (p_dev->cmd_ack_complete) {
            p_dev->cmd_ack_complete = AM_FALSE;
            __ts10x_proc_complete_proc(p_dev, p_dev->complete_status);
        }

    }
}

/*******************************************************************************
    The INT PIN callback
*******************************************************************************/

am_local void __ts10x_int_pin_callback (void *p_arg)
{
    am_ts10x_dev_t *p_dev = (am_ts10x_dev_t *)p_arg;

    if (p_dev->pfn_int_pin_cb) {
        p_dev->pfn_int_pin_cb(p_dev->p_int_pin_arg);
    }
}


/*******************************************************************************
    The CMD/ACK Process complete functions
*******************************************************************************/

am_local void __ts10x_proc_complete_at_first (am_ts10x_dev_t *p_dev, int status)
{
    p_dev->cmd_ack_complete = AM_TRUE;
    p_dev->complete_status  = status;
}

/******************************************************************************/
am_local void __ts10x_proc_complete_at_sec (am_ts10x_dev_t *p_dev, int status)
{
    p_dev->cmd_ack_complete = AM_TRUE;
    p_dev->complete_status  = status;
}

/******************************************************************************/
am_local void __ts10x_proc_complete_proc (am_ts10x_dev_t *p_dev, int status)
{
    /* anyway, stop the timer used for timeout */
    am_softimer_stop(&(p_dev->timer));

    if (p_dev->pfn_private_cb) {

        /* The p_dev->pfn_complete should be called internal if need */
        p_dev->pfn_private_cb(p_dev, status);
    } else {
        /* The Q3 is ACK status */
        status = ((status == AM_TS10X_SUCCESS) ? p_dev->data[2]: status);

        if (p_dev->pfn_process_cb) {
            p_dev->pfn_process_cb(p_dev->p_process_arg, status);
        }
    }
}

/******************************************************************************/
am_local void __ts10x_user_count_get_complete (am_ts10x_dev_t *p_dev,
                                               int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.user_count_get.p_user_count) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.user_count_get.p_user_count) =
                (p_dev->data[0] << 8) | (p_dev->data[1]);
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_user_role_get_complete (am_ts10x_dev_t *p_dev,
                                              int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.user_role_get.p_user_role) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.user_role_get.p_user_role) = p_dev->data[2];
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_user_compare_1_with_n (am_ts10x_dev_t *p_dev,
                                             int             status)
{
    uint16_t user_id;

    if (status == AM_TS10X_SUCCESS) {

        user_id = (p_dev->data[0] << 8) | (p_dev->data[1]);

        /* process according to the cmd */
        if (p_dev->private_data.compare_1_n.p_user_role) {
            *(p_dev->private_data.compare_1_n.p_user_role) = p_dev->data[2];
        }

        /* process according to the cmd */
        if (p_dev->private_data.compare_1_n.p_user_id) {
            *(p_dev->private_data.compare_1_n.p_user_id) = user_id;
        }

        if (user_id == 0) {                  /* failed!      */
            status = AM_TS10X_NOUSER;        /* no this user */
        }
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_user_id_unused_get_complete (am_ts10x_dev_t *p_dev,
                                                   int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.unused_user_id_get.p_user_id) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.unused_user_id_get.p_user_id) = \
                (p_dev->data[0] << 8) | (p_dev->data[1]);
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_baudrate_set_complete (am_ts10x_dev_t *p_dev,
                                             int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* note: timeout same to successful! */
    if ((status == -AM_ETIME) || (status == AM_TS10X_SUCCESS)) {
        am_uart_ioctl(p_dev->uart_handle,
                      AM_UART_BAUD_SET,
                      (void *)(p_dev->private_data.baudrate_set.baudrate));

        status = AM_TS10X_SUCCESS;
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_image_data_get_complete (am_ts10x_dev_t *p_dev,
                                               int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    if (p_dev->private_data.image_data_get.p_actual_len) {
        *(p_dev->private_data.image_data_get.p_actual_len) = \
                (p_dev->data[0] << 8) | p_dev->data[1];
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_version_info_get_complete (am_ts10x_dev_t *p_dev,
                                                 int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.version_info_get.p_info_len) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.version_info_get.p_info_len) = \
                (p_dev->data[0] << 8) | p_dev->data[1];
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}
/******************************************************************************/
am_local void __ts10x_compare_level_set_complete (am_ts10x_dev_t *p_dev,
                                                  int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.compare_level_set.p_old_level) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.compare_level_set.p_old_level) = p_dev->data[1];
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_user_all_id_role_get_complete (am_ts10x_dev_t *p_dev,
                                                     int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.user_all_id_role_get.p_user_count) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.user_all_id_role_get.p_user_count) = \
                (((p_dev->data[0] << 8) | (p_dev->data[1])) - 2) / 3;
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_register_mode_get_complete (am_ts10x_dev_t *p_dev,
                                                  int             status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    /* process according to the cmd */
    if (p_dev->private_data.register_mode_get.p_allow_same) {

        /* only the last buffer segment length is not known  */
        *(p_dev->private_data.register_mode_get.p_allow_same) = \
                (p_dev->data[1]) ? AM_FALSE : AM_TRUE;
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_compare_feature_with_all_user_complete (
        am_ts10x_dev_t *p_dev,
        int             status)
{
    uint16_t user_id;

    if (status == AM_TS10X_SUCCESS) {

        user_id = (p_dev->data[0] << 8) | (p_dev->data[1]);

        if (user_id == 0) {                  /* failed! */
            status = p_dev->data[2];
        } else {                             /* successful */

            /* process according to the cmd */
            if (p_dev->private_data.compare_feature_with_all_user.p_user_id) {
                *(p_dev->private_data.compare_feature_with_all_user.p_user_id) \
                        = user_id;
            }
        }
    }

    if (p_dev->pfn_process_cb) {
        p_dev->pfn_process_cb(p_dev->p_process_arg, status);
    }
}

/******************************************************************************/
am_local void __ts10x_register_complete (am_ts10x_dev_t *p_dev, int status)
{
    if (status == AM_TS10X_SUCCESS) {
        status = p_dev->data[2];
    }

    if (status != AM_TS10X_SUCCESS) {  /*failed */

        if (p_dev->pfn_process_cb) {
            p_dev->pfn_process_cb(p_dev->p_process_arg, status);
        }

    } else {

        /* complete */
        if (p_dev->cmd_ack_type == __TS10X_PACKET_TYPE_REGISTER_3) {

            uint16_t user_id = (p_dev->data[0] << 8) | (p_dev->data[1]);

            if (p_dev->private_data.register_data.p_user_id) {
                *(p_dev->private_data.register_data.p_user_id) = user_id;
            }

            if (p_dev->pfn_process_cb) {
                p_dev->pfn_process_cb(p_dev->p_process_arg, status);
            }
        } else {

            /* inform the user the register is in progress */
            if (p_dev->pfn_process_cb) {
                p_dev->pfn_process_cb(p_dev->p_process_arg, -AM_EINPROGRESS);
            }

            /* continue do something, eg: send cmd, receive ack. */
            if (p_dev->private_data.register_data.pfn_continue) {
                p_dev->private_data.register_data.pfn_continue(p_dev);
            }
        }
    }
}

/******************************************************************************/
am_local void __ts10x_register_1c3r_continue (am_ts10x_dev_t *p_dev)
{
    /* continue receive next packet */
    p_dev->p_rx_state = &__g_ts10x_recv_first_head_state;

    /* The timeout set to 10s */
    p_dev->timeout_expect_val = p_dev->timeout_cmd_start;
    p_dev->timeout_val        = 0;

    am_softimer_start(&(p_dev->timer), __TS10X_TIMEOUT_CHK_PERIOD);
}

/******************************************************************************/
am_local void __ts10x_register_2c2r_continue (am_ts10x_dev_t *p_dev)
{
    /* start send the second packet (cmd is 0x03) */
    p_dev->cmd_ack_type = __TS10X_PACKET_TYPE_REGISTER_3;
    p_dev->data[0]      = 0x00;
    p_dev->data[1]      = 0x00;
    p_dev->data[2]      = 0x00;
    p_dev->data[3]      = 0x00;

    p_dev->p_tx_state = &__g_ts10x_send_first_head_state;

    am_uart_tx_startup(p_dev->uart_handle);
}

/******************************************************************************/
am_local void __ts10x_register_3c3r_continue (am_ts10x_dev_t *p_dev)
{
    /* start send the next packet, send 0x02 and 0x03 */
    p_dev->cmd_ack_type = p_dev->cmd_ack_type + 1;
    p_dev->data[0]      = 0x00;
    p_dev->data[1]      = 0x00;
    p_dev->data[2]      = 0x00;
    p_dev->data[3]      = 0x00;

    p_dev->p_tx_state = &__g_ts10x_send_first_head_state;

    am_uart_tx_startup(p_dev->uart_handle);
}

/******************************************************************************/
am_local void __ts10x_register_mofn_continue (am_ts10x_dev_t *p_dev)
{
    /* start send the next packet, send 0x01 cmd */
    p_dev->cmd_ack_type = __TS10X_PACKET_TYPE_REGISTER_1;
    p_dev->data[0]      = 0x00;
    p_dev->data[1]      = 0x00;
    p_dev->data[2]      = p_dev->private_data.register_data.user_role;
    p_dev->data[3]      = 0x00;

    p_dev->p_tx_state = &__g_ts10x_send_first_head_state;

    am_uart_tx_startup(p_dev->uart_handle);
}

/*******************************************************************************
    The UART txchar_get and rxchar_put functions
*******************************************************************************/

/* the function that to get one char to transmit */
am_local int __ts10x_uart_txchar_get (void *p_arg, char *p_outchar)
{
    am_ts10x_dev_t *p_dev = (am_ts10x_dev_t *)p_arg;
    int             ret;

    p_dev->p_tx_state = ((__ts10x_send_state_t *)(p_dev->p_tx_state))
                        ->pfn_process(p_arg, (uint8_t *)p_outchar, &ret);

    return ret;
}

/******************************************************************************/

/* the function revived one char */
am_local int __zlg101_uart_rxchar_put (void *p_arg, char inchar)
{
    am_ts10x_dev_t *p_dev = (am_ts10x_dev_t *)p_arg;

    __TS10X_TIMEOUT_VAL_CLEAR(p_dev);

    p_dev->p_rx_state = ((__ts10x_recv_state_t *)(p_dev->p_rx_state))
                         ->pfn_process(p_arg, (uint8_t)inchar);

    if (p_dev->cmd_ack_complete) {
        p_dev->cmd_ack_complete = AM_FALSE;
        __ts10x_proc_complete_proc(p_dev, p_dev->complete_status);
    }

    return AM_OK;
}

/******************************************************************************/

am_local int __ts10x_cmd_send_start (am_ts10x_dev_t *p_dev)
{
    p_dev->p_tx_state = &__g_ts10x_send_first_head_state;

    am_uart_tx_startup(p_dev->uart_handle);

    /* cmd is in process, call callback function while complete */
    return -AM_EINPROGRESS;
}

/*******************************************************************************
    public functions
*******************************************************************************/

am_ts10x_handle_t am_ts10x_init (am_ts10x_dev_t           *p_dev,
                                 int                       int_pin,
                                 am_uart_handle_t          uart_handle)
{
    if ((p_dev == NULL) || (uart_handle  == NULL)) {
        return NULL;
    }

    p_dev->uart_handle = uart_handle;
    p_dev->p_rx_state  = &__g_ts10x_recv_idle_state;
    p_dev->p_tx_state  = &__g_ts10x_send_idle_state;

    p_dev->pfn_int_pin_cb = NULL;
    p_dev->p_int_pin_arg  = NULL;
    p_dev->pfn_process_cb = NULL;
    p_dev->p_process_arg  = NULL;
    p_dev->int_pin        = int_pin;

    p_dev->cmd_ack_complete = AM_FALSE;

    am_uart_ioctl(uart_handle,
                  AM_UART_BAUD_SET,
                  (void *)__TS10X_UART_BAUDRATE);

    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_INT);

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_TXCHAR_GET,
        (int (*)(void *))__ts10x_uart_txchar_get,
                         (void *)(p_dev));

    am_uart_callback_set(uart_handle,
                         AM_UART_CALLBACK_RXCHAR_PUT,
        (int (*)(void *))__zlg101_uart_rxchar_put,
                         (void *)(p_dev));

    am_softimer_init (&(p_dev->timer), __ts10x_timeout_callback, (void *)p_dev);

    am_gpio_trigger_connect(int_pin, __ts10x_int_pin_callback, (void *)p_dev);

    am_gpio_trigger_cfg(int_pin, AM_GPIO_TRIGGER_RISE);
    am_gpio_trigger_on(int_pin);

    return p_dev;
}

/******************************************************************************/
int am_ts10x_deinit (am_ts10x_handle_t handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_trigger_off(handle->int_pin);
    am_gpio_trigger_disconnect(handle->int_pin,
                               __ts10x_int_pin_callback,
                               (void *)handle);

    am_softimer_stop(&(handle->timer));

    return AM_OK;
}

/******************************************************************************/
int am_ts10x_process_cb_set (am_ts10x_handle_t        handle,
                             am_ts10x_process_cb_t    pfn_process_cb,
                             void                    *p_arg)
{
    int key;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();
    handle->pfn_process_cb = pfn_process_cb;
    handle->p_process_arg  = p_arg;
    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_ts10x_int_pin_cb_set (am_ts10x_handle_t  handle,
                             am_pfnvoid_t       pfn_int_pin_cb,
                             void              *p_arg)
{
    int key;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();
    handle->pfn_int_pin_cb = pfn_int_pin_cb;
    handle->p_int_pin_arg  = p_arg;
    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_ts10x_user_one_del (am_ts10x_handle_t  handle, uint16_t user_id)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_ONE_DEL;
    handle->data[0]      = (user_id >> 8) & 0xFF;  /* Higher 8 Bits */
    handle->data[1]      = user_id & 0xFF;         /* Lower 8 Bits  */
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_user_all_del (am_ts10x_handle_t  handle)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_ALL_DEL;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_user_count_get (am_ts10x_handle_t handle, uint16_t *p_user_count)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_COUNT_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_user_count_get_complete;

    handle->private_data.user_count_get.p_user_count = p_user_count;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_user_role_get (am_ts10x_handle_t handle,
                            uint16_t          user_id,
                            uint8_t          *p_user_role)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_ROLE_GET;
    handle->data[0]      = (user_id >> 8) & 0xFF;  /* Higher 8 Bits */
    handle->data[1]      = user_id & 0xFF;         /* Lower 8 Bits  */
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_user_role_get_complete;

    handle->private_data.user_role_get.p_user_role = p_user_role;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_collect_with_one_user (am_ts10x_handle_t handle,
                                            uint16_t          user_id)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_1_WITH_1;
    handle->data[0]      = (user_id >> 8) & 0xFF;  /* Higher 8 Bits */
    handle->data[1]      = user_id & 0xFF;         /* Lower 8 Bits  */
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_collect_with_all_user (am_ts10x_handle_t  handle,
                                            uint16_t          *p_user_id,
                                            uint8_t           *p_user_role)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_1_WITH_N;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_user_compare_1_with_n;

    handle->private_data.compare_1_n.p_user_role = (volatile uint8_t *)p_user_role;
    handle->private_data.compare_1_n.p_user_id   = (volatile uint16_t *)p_user_id;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_user_id_unused_get (am_ts10x_handle_t  handle,
                                 uint16_t          *p_user_id)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_UNUSED_USER_ID_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_user_id_unused_get_complete;

    handle->private_data.unused_user_id_get.p_user_id = p_user_id;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_baudrate_set (am_ts10x_handle_t  handle,
                           uint32_t           baudrate)
{
    static const uint32_t baudrate_support[] = {9600, 19200, 38400, 57600, 115200};

    int i;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    for (i = 0; i < AM_NELEMENTS(baudrate_support); i++) {
        if (baudrate_support[i] == baudrate) {
            break;
        }
    }

    if (i == AM_NELEMENTS(baudrate_support)) {
        return -AM_EINVAL;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_BAUDRATE_SET;
    handle->data[0]      = 0x00;
    handle->data[1]      = i + 1;                    /* New baudrate ID  */
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_baudrate_set_complete;

    handle->private_data.baudrate_set.baudrate = baudrate_support[i];

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_collect_image_feature_get (am_ts10x_handle_t  handle,
                                        uint8_t           *p_data_buf,
                                        size_t             buf_len)
{
    if ((handle     == NULL) ||
        (p_data_buf == NULL) ||
        (buf_len    < __TS10X_FEATURE_LEN)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_IMAGE_FEATURE_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_TRUE;
    handle->recv_sec_num = 2;    /* only 2 buffer for information */
    handle->send_sec_num = 0;

    /* discard 3 bytes */
    handle->recv_sec_buf_info[0].p_buf      = NULL;
    handle->recv_sec_buf_info[0].expect_len = 3;

    /* store feature data */
    handle->recv_sec_buf_info[1].p_buf      = p_data_buf;
    handle->recv_sec_buf_info[1].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_collect_image_data_get (am_ts10x_handle_t  handle,
                                     uint8_t           *p_data_buf,
                                     size_t             buf_len,
                                     size_t            *p_actual_len)
{
    if ((handle == NULL) || (p_data_buf == NULL) || (buf_len == 0)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_IMAGE_DATA_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_TRUE;
    handle->recv_sec_num = 1;    /* only 1 buffer for image data */
    handle->send_sec_num = 0;

    /* image data buf */
    handle->recv_sec_buf_info[0].p_buf      = p_data_buf;
    handle->recv_sec_buf_info[0].expect_len = buf_len;

    handle->pfn_private_cb  = __ts10x_image_data_get_complete;

    handle->private_data.image_data_get.p_actual_len = p_actual_len;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_version_info_get (am_ts10x_handle_t  handle,
                               uint8_t           *p_buf,
                               size_t             buf_size,
                               size_t            *p_info_size)
{
    if ((handle == NULL) || (p_buf == NULL) || (buf_size == 0)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_VERSION_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_TRUE;
    handle->recv_sec_num = 1;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->recv_sec_buf_info[0].p_buf      = p_buf;
    handle->recv_sec_buf_info[0].expect_len = buf_size;

    handle->pfn_private_cb  = __ts10x_version_info_get_complete;

    /* The private data for get the actual length */
    handle->private_data.version_info_get.p_info_len = p_info_size;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_level_set (am_ts10x_handle_t  handle,
                                uint8_t            new_level,
                                uint8_t           *p_old_level)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_LEVEL_SET;
    handle->data[0]      = 0x00;
    handle->data[1]      = new_level;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only one buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_compare_level_set_complete;

    handle->private_data.compare_level_set.p_old_level = p_old_level;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);

}

/******************************************************************************/
int am_ts10x_user_all_id_role_get (am_ts10x_handle_t   handle,
                                   uint16_t           *p_user_count,
                                   uint8_t            *p_buf,
                                   size_t              buf_len)
{
    if ((handle == NULL) || (p_buf == NULL) || (buf_len == 0)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_ALL_ID_ROLE_GET;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0x00;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_TRUE;
    handle->recv_sec_num = 2;    /* only 2 buffer for information */
    handle->send_sec_num = 0;

    /* user number */
    handle->recv_sec_buf_info[0].p_buf      = \
            &(handle->private_data.user_all_id_role_get.user_number[0]);

    handle->recv_sec_buf_info[0].expect_len = 2;

    /* store feature data */
    handle->recv_sec_buf_info[1].p_buf      = p_buf;
    handle->recv_sec_buf_info[1].expect_len = buf_len;

    handle->pfn_private_cb  = __ts10x_user_all_id_role_get_complete;

    handle->private_data.user_all_id_role_get.p_user_count = p_user_count;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);

}
/******************************************************************************/
int am_ts10x_register_mode_set (am_ts10x_handle_t   handle,
                                am_bool_t           allow_same)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_REGISTER_MODE;
    handle->data[0]      = 0x00;
    handle->data[1]      = (allow_same == AM_FALSE) ? 1 : 0;
    handle->data[2]      = 0x00;              /* set */
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_register_mode_get (am_ts10x_handle_t   handle,
                                am_bool_t          *p_allow_same)
{
    if (handle == NULL) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_REGISTER_MODE;
    handle->data[0]      = 0x00;
    handle->data[1]      = 0;
    handle->data[2]      = 0x01;              /* get */
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_register_mode_get_complete;

    handle->private_data.register_mode_get.p_allow_same = p_allow_same;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);

}

/******************************************************************************/
int am_ts10x_user_feature_get (am_ts10x_handle_t   handle,
                               uint16_t            user_id,
                               uint8_t            *p_user_role,
                               uint8_t            *p_buf,
                               size_t              buf_len)
{
    if ((handle == NULL) || (p_buf == NULL) || (buf_len < __TS10X_FEATURE_LEN)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_FEATURE_GET;
    handle->data[0]      = (user_id >> 8) & 0xFF;
    handle->data[1]      = user_id & 0xFF;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_TRUE;
    handle->recv_sec_num = 3;    /* only 3 buffer for information */
    handle->send_sec_num = 0;

    /* The user id discard  */
    handle->recv_sec_buf_info[0].p_buf      = NULL;
    handle->recv_sec_buf_info[0].expect_len = 2;

    /* user role */
    handle->recv_sec_buf_info[1].p_buf      = p_user_role;
    handle->recv_sec_buf_info[1].expect_len = 1;

    /* store feature data */
    handle->recv_sec_buf_info[2].p_buf      = p_buf;
    handle->recv_sec_buf_info[2].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);

}

/******************************************************************************/
int am_ts10x_user_add_with_feature (am_ts10x_handle_t  handle,
                                    uint16_t           user_id,
                                    uint8_t            user_role,
                                    const uint8_t     *p_feature,
                                    size_t             feature_len)
{
    size_t   cmd_data_len;
    uint8_t *p_buf;

    if ((handle    == NULL)    ||
        (p_feature == NULL) ||
        (feature_len < __TS10X_FEATURE_LEN)) {

        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    cmd_data_len = 2 + 1 + __TS10X_FEATURE_LEN;

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_USER_ADD_WITH_FEATURE;
    handle->data[0]      = (cmd_data_len >> 8) & 0xFF;
    handle->data[1]      = cmd_data_len & 0xFF;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_TRUE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only 3 buffer for information */
    handle->send_sec_num = 2;

    p_buf = handle->private_data.add_user_with_feature.user_id_role;

    p_buf[0] = (user_id >> 8) & 0xFF;
    p_buf[1] = (user_id & 0xFF);
    p_buf[2] = user_role;

    /* The user id and role  */
    handle->send_sec_buf_info[0].p_buf      = p_buf;
    handle->send_sec_buf_info[0].expect_len = 3;

    /* the feature data */
    handle->send_sec_buf_info[1].p_buf      = p_feature;
    handle->send_sec_buf_info[1].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_feature_with_one_user (am_ts10x_handle_t   handle,
                                            const uint8_t      *p_feature,
                                            size_t              feature_len,
                                            uint16_t            user_id,
                                            uint8_t             user_role)
{
    size_t   cmd_data_len;
    uint8_t *p_buf;

    if ((handle    == NULL)    ||
        (p_feature == NULL) ||
        (feature_len < __TS10X_FEATURE_LEN)) {

        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    cmd_data_len = 2 + 1 + __TS10X_FEATURE_LEN;

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_FEATURE_WITH_ONE_USER;
    handle->data[0]      = (cmd_data_len >> 8) & 0xFF;
    handle->data[1]      = cmd_data_len & 0xFF;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_TRUE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only 3 buffer for information */
    handle->send_sec_num = 2;

    p_buf = handle->private_data.compare_feature_with_one_user.user_id_role;

    p_buf[0] = (user_id >> 8) & 0xFF;
    p_buf[1] = (user_id & 0xFF);
    p_buf[2] = user_role;

    /* The user id and role  */
    handle->send_sec_buf_info[0].p_buf      = p_buf;
    handle->send_sec_buf_info[0].expect_len = 3;

    /* the feature data */
    handle->send_sec_buf_info[1].p_buf      = p_feature;
    handle->send_sec_buf_info[1].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_feature_with_all_user (am_ts10x_handle_t  handle,
                                            const uint8_t     *p_feature,
                                            size_t             feature_len,
                                            uint16_t          *p_user_id)
{
    size_t   cmd_data_len;
    uint8_t *p_buf;

    if ((handle    == NULL)    ||
        (p_feature == NULL) ||
        (feature_len < __TS10X_FEATURE_LEN)) {

        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    cmd_data_len = 3 + __TS10X_FEATURE_LEN;

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_FEATURE_WITH_ALL_USER;
    handle->data[0]      = (cmd_data_len >> 8) & 0xFF;
    handle->data[1]      = cmd_data_len & 0xFF;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_TRUE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only 3 buffer for information */
    handle->send_sec_num = 2;

    p_buf = handle->private_data.zero_data.data;

    p_buf[0] = 0;
    p_buf[1] = 0;
    p_buf[2] = 0;

    /* The user id and role  */
    handle->send_sec_buf_info[0].p_buf      = p_buf;
    handle->send_sec_buf_info[0].expect_len = 3;

    /* the feature data */
    handle->send_sec_buf_info[1].p_buf      = p_feature;
    handle->send_sec_buf_info[1].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = __ts10x_compare_feature_with_all_user_complete;

    handle->private_data.compare_feature_with_all_user.p_user_id = p_user_id;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_DEFAULT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_compare_collect_with_feature (am_ts10x_handle_t  handle,
                                           const uint8_t     *p_feature,
                                           size_t             feature_len)
{
    size_t   cmd_data_len;
    uint8_t *p_buf;

    if ((handle    == NULL)    ||
        (p_feature == NULL) ||
        (feature_len < __TS10X_FEATURE_LEN)) {

        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    cmd_data_len = 3 + __TS10X_FEATURE_LEN;

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_COMPARE_COLLECT_WITH_FEATURE;
    handle->data[0]      = (cmd_data_len >> 8) & 0xFF;
    handle->data[1]      = cmd_data_len & 0xFF;
    handle->data[2]      = 0x00;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_TRUE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only 3 buffer for information */
    handle->send_sec_num = 2;

    p_buf = handle->private_data.zero_data.data;

    p_buf[0] = 0;
    p_buf[1] = 0;
    p_buf[2] = 0;

    /* The user id and role  */
    handle->send_sec_buf_info[0].p_buf      = p_buf;
    handle->send_sec_buf_info[0].expect_len = 3;

    /* the feature data */
    handle->send_sec_buf_info[1].p_buf      = p_feature;
    handle->send_sec_buf_info[1].expect_len = __TS10X_FEATURE_LEN;

    handle->pfn_private_cb  = NULL;

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    return __ts10x_cmd_send_start(handle);
}

/******************************************************************************/
int am_ts10x_user_register (am_ts10x_handle_t  handle,
                            uint16_t          *p_user_id,
                            uint8_t            user_role,
                            uint8_t            reg_method)
{
    if ((handle == NULL) || (p_user_id == NULL)) {
        return -AM_EINVAL;
    }

    if ((handle->p_tx_state  != &__g_ts10x_send_idle_state) ||
        (handle->p_rx_state  != &__g_ts10x_recv_idle_state)) {

        return -AM_EBUSY;
    }

    /* make the cmd/ack information */
    handle->cmd_ack_type = __TS10X_PACKET_TYPE_REGISTER_1;
    handle->data[0]      = (*p_user_id >> 8) & 0xFF;
    handle->data[1]      = *p_user_id & 0xFF;
    handle->data[2]      = user_role;
    handle->data[3]      = 0x00;

    handle->is_send_sec  = AM_FALSE;
    handle->is_recv_sec  = AM_FALSE;
    handle->recv_sec_num = 0;    /* only 3 buffer for information */
    handle->send_sec_num = 0;

    handle->pfn_private_cb  = __ts10x_register_complete;

    if (reg_method == AM_TS10X_USER_REG_METHOD_1C3R) {

        handle->cmd_ack_type   = __TS10X_PACKET_TYPE_REGISTER_1;

        handle->private_data.register_data.pfn_continue = \
                __ts10x_register_1c3r_continue;

    } else if (reg_method == AM_TS10X_USER_REG_METHOD_2C2R) {

        handle->cmd_ack_type   = __TS10X_PACKET_TYPE_REGISTER_2;

        handle->private_data.register_data.pfn_continue = \
                __ts10x_register_2c2r_continue;

    }  else if (reg_method == AM_TS10X_USER_REG_METHOD_3C3R) {

        handle->cmd_ack_type   = __TS10X_PACKET_TYPE_REGISTER_1;

        handle->private_data.register_data.pfn_continue = \
                __ts10x_register_3c3r_continue;

    } else if (reg_method == AM_TS10X_USER_REG_METHOD_MOFN) {

        handle->cmd_ack_type   = __TS10X_PACKET_TYPE_REGISTER_1;

        handle->private_data.register_data.pfn_continue = \
                __ts10x_register_mofn_continue;

    } else {
         return -AM_EINVAL;
    }

    handle->timeout_cmd_start = __TS10X_TIMEOUT_TIME_COLLECT;

    handle->private_data.register_data.p_user_id = p_user_id;
    handle->private_data.register_data.user_role = user_role;

    return __ts10x_cmd_send_start(handle);
}

/* end of file */
