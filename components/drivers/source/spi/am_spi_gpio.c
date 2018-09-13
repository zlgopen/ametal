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
 * \brief GPIO 模拟SPI 驱动
 *
 * \internal
 * \par Modification history
 * - 1.00 18-04-10  vir, first implementation.
 * \endinternal
 */

#include "am_spi_gpio.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_delay.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
/**
 * \brief 状态定义
 */
#define __SPI_GPIO_ST_IDLE             (0x10u)          /* 空闲状态 */
#define __SPI_GPIO_ST_MSG_START        (0x11u)          /* 消息传输开始状态 */
#define __SPI_GPIO_ST_TRANS_START      (0x12u)          /* 获取传输开始状态 */
#define __SPI_GPIO_ST_TX_RX_DATA       (0x13u)          /* 传输数据 */

/**
 * \brief 事件定义
 */
#define __SPI_GPIO_EVT_NONE            (0xFFu)          /* 无事件 */
#define __SPI_GPIO_EVT_MSG_LAUNCH      (0xFEu)          /* 开始处理一个新的消息 */
#define __SPI_GPIO_EVT_TRANS_LAUNCH    (0xFDu)          /* 开始处理一个新的传输 */

/**
 * \brief 变量未使用到
 */
#define __VAR_NO_USE(a)      (void)(a)

/* 获取当前消息 */
#define __spi_gpio_cur_msg(p_dev) \
    ((p_dev)->p_cur_msg)

/* 判断传输体是否为空 */
#define __spi_gpio_trans_empty(p_dev) \
                  am_list_empty(&(p_cur_msg->transfers))

/* 获取当前传输体 */
#define __spi_gpio_cur_trans(p_dev) \
    ((p_dev)->p_cur_trans)


/******************************************************************************/
am_local int __spi_gpio_info_get(void *p_drv, am_spi_info_t *p_info);
am_local int __spi_gpio_set_up(void *p_drv, struct am_spi_device *p_dev);
am_local int __spi_gpio_msg_start (void              *p_drv,
                                   am_spi_device_t   *p_dev,
                                   am_spi_message_t  *p_msg);

am_local int __spi_mst_sm_event(am_spi_gpio_dev_t *p_dev, uint32_t event);

am_local struct am_spi_drv_funcs   __g_spi_gpio_drv_funcs = {
        __spi_gpio_info_get,
        __spi_gpio_set_up,
        __spi_gpio_msg_start,
 };


/******************************************************************************
 SPI 协议实现
*******************************************************************************/

/**
 * \biref 延时
 */
am_local
void __spi_gpio_delay(am_spi_gpio_dev_t *p_this)
{
    volatile unsigned int i = p_this->p_devinfo->speed_exp;

    while (i--);
}

/**
 * \brief 设置SCK的空闲状态
 */
am_static_inline
void __spi_gpio_sck_idle_state_set( am_spi_gpio_dev_t *p_this )
{
    p_this->sck_state = (p_this->p_cur_spi_dev->mode & AM_SPI_CPOL)? 1:0;
    am_gpio_set(p_this->p_devinfo->sck_pin, p_this->sck_state);
}

/**
 * \brief 翻转时钟SCK
 */
am_static_inline
void __spi_gpio_sck_toggle(am_spi_gpio_dev_t *p_this)
{
    p_this->sck_state = !p_this->sck_state;
    am_gpio_set(p_this->p_devinfo->sck_pin, p_this->sck_state);
}


/**
 * \brief SPI 读写1~32位数据
 */
am_local uint32_t __gpio_spi_rw_data (am_spi_gpio_dev_t *p_this,
                                      uint32_t           w_data,
                                      uint8_t            bits_per_word)
{
    am_spi_device_t *p_dev                 = p_this->p_cur_spi_dev;
    const am_spi_gpio_devinfo_t *p_devinfo = p_this->p_devinfo;

    uint32_t         r_data = 0;
    uint8_t          i      = 0;
    uint8_t          bit    = 0;

    if (p_dev->mode & AM_SPI_LSB_FIRST) {
        bit = 0;
    } else {
        bit = bits_per_word - 1;
    }

    /* 根据时钟相位输入第一个周期 */
    am_gpio_set(p_devinfo->mosi_pin, AM_BIT_GET(w_data, bit));

    if (p_dev->mode & AM_SPI_CPHA) {
        __spi_gpio_sck_toggle(p_this);
    }
    __spi_gpio_delay(p_this);

    if ( p_dev->mode & AM_SPI_3WIRE ) {
        am_gpio_pin_cfg(p_devinfo->mosi_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
        AM_BIT_MODIFY(r_data, bit, am_gpio_get(p_devinfo->mosi_pin));
        am_gpio_pin_cfg(p_devinfo->mosi_pin,
           AM_BIT_GET(w_data, bit) ? AM_GPIO_OUTPUT_INIT_HIGH :AM_GPIO_OUTPUT_INIT_LOW);
    } else {
        AM_BIT_MODIFY(r_data, bit, am_gpio_get(p_devinfo->miso_pin));
    }

    __spi_gpio_sck_toggle(p_this);
    __spi_gpio_delay(p_this);

    for (i=0; i< (p_dev->bits_per_word -1); i++) {

        if (p_dev->mode & AM_SPI_LSB_FIRST) {
            bit++;
        } else {
            bit--;
        }

        am_gpio_set(p_devinfo->mosi_pin, AM_BIT_GET(w_data, bit));

        __spi_gpio_sck_toggle(p_this);
        __spi_gpio_delay(p_this);

        if ( p_dev->mode & AM_SPI_3WIRE ) {
            am_gpio_pin_cfg(p_devinfo->mosi_pin, AM_GPIO_INPUT | AM_GPIO_PULLUP);
            AM_BIT_MODIFY(r_data, bit, am_gpio_get(p_devinfo->mosi_pin));
            am_gpio_pin_cfg(p_devinfo->mosi_pin, AM_GPIO_OUTPUT_INIT_HIGH);
        } else {
            AM_BIT_MODIFY(r_data, bit, am_gpio_get(p_devinfo->miso_pin));
        }

        __spi_gpio_sck_toggle(p_this);
        __spi_gpio_delay(p_this);
    }
    __spi_gpio_sck_idle_state_set(p_this);

    return r_data;
}

/*
 * \brief 传输体数据处理
 */
am_local
void __spi_gpio_trans_data_start(am_spi_gpio_dev_t *p_this,
                                 am_spi_transfer_t *p_trans)
{
    uint32_t  len = 0;

    if( p_trans->bits_per_word > 32) {
        return;
    }

    if(p_trans->bits_per_word == 0) {
        p_trans->bits_per_word = p_this->p_cur_spi_dev->bits_per_word;
    }
    switch( AM_ROUND_UP(p_trans->bits_per_word,8)) {

    /* 处理 1 ~ 8 位数据 */
    case 8:
    {
        uint8_t  *p_tx    = NULL;
        uint8_t  *p_rx    = NULL;
        uint8_t   tx_data = 0;
        uint8_t   rx_data = 0;

        p_tx  = (uint8_t *)p_trans->p_txbuf;
        p_rx  = (uint8_t *)p_trans->p_rxbuf;
        len   = p_trans->nbytes / 1;

        if( p_trans->flags & AM_SPI_READ_MOSI_HIGH ) {
            tx_data = 0XFF;
        }

        while (len--) {
            if (p_tx) {
                tx_data = *p_tx;
                p_tx++;
            }

            rx_data = __gpio_spi_rw_data(p_this, tx_data, p_trans->bits_per_word);

            if (p_rx) {
                *p_rx = rx_data;
                p_rx++;
            }
        }
        break;
    }

    /* 处理 9 ~ 16 位数据 */
    case 16:
    {
        uint16_t  *p_tx    = NULL;
        uint16_t  *p_rx    = NULL;
        uint16_t   tx_data = 0;
        uint16_t   rx_data = 0;

        p_tx  = (uint16_t *)p_trans->p_txbuf;
        p_rx  = (uint16_t *)p_trans->p_rxbuf;
        len   = p_trans->nbytes / 2;

        if( p_trans->flags & AM_SPI_READ_MOSI_HIGH ) {
            tx_data = 0XFFFF;
        }

        while (len--) {
            if (p_tx) {
                tx_data = *p_tx;
                p_tx++;
            }

            rx_data = __gpio_spi_rw_data(p_this, tx_data, p_trans->bits_per_word);

            if (p_rx) {
                *p_rx = rx_data;
                p_rx++;
            }
        }
        break;
    }

    /* 处理 17 ~ 32 位数据 */
    case 24:
    case 32:
    {
        uint32_t  *p_tx    = NULL;
        uint32_t  *p_rx    = NULL;
        uint32_t   tx_data = 0;
        uint32_t   rx_data = 0;

        p_tx  = (uint32_t *)p_trans->p_txbuf;
        p_rx  = (uint32_t *)p_trans->p_rxbuf;
        len   = p_trans->nbytes / 4;

        if( p_trans->flags & AM_SPI_READ_MOSI_HIGH ) {
            tx_data = 0XFFFF;
        }

        while (len--) {
            if (p_tx) {
                tx_data = *p_tx;
                p_tx++;
            }

            rx_data = __gpio_spi_rw_data(p_this, tx_data, p_trans->bits_per_word);

            if (p_rx) {
                *p_rx = rx_data;
                p_rx++;
            }
        }
        break;
    }

    default:
        break;
    }
}

/******************************************************************************/

/**
 * \brief 硬件初始化
 */
am_local void __spi_gpio_hw_init(const am_spi_gpio_devinfo_t *p_devinfo)
{
   am_gpio_pin_cfg(p_devinfo->sck_pin,
                         AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PUSH_PULL);

   if (p_devinfo->miso_pin != -1) {
       am_gpio_pin_cfg(p_devinfo->miso_pin,
                         AM_GPIO_INPUT | AM_GPIO_PULLUP);
   }

   if(p_devinfo->mosi_pin != -1) {
       am_gpio_pin_cfg(p_devinfo->mosi_pin,
                          AM_GPIO_OUTPUT_INIT_HIGH | AM_GPIO_PUSH_PULL );
   }
}

/**
* \brief 添加一条 message 到控制器传输列表末尾
*
* \attention 调用此函数必须锁定控制器
*/
am_static_inline
void __spi_gpio_msg_in (am_spi_gpio_dev_t *p_dev, struct am_spi_message *p_msg)
{
 am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                 &(p_dev->msg_list));
}

/**
* \brief 从控制器传输列表表头取出一条 message
*
* \attention 调用此函数必须锁定控制器
*/
am_static_inline
struct am_spi_message *__spi_gpio_msg_out (am_spi_gpio_dev_t *p_dev)
{
 if (am_list_empty_careful(&(p_dev->msg_list))) {
     return NULL;
 } else {
     struct am_list_head *p_node = p_dev->msg_list.next;
     am_list_del(p_node);
     return am_list_entry(p_node, struct am_spi_message, ctlrdata);
 }
}

/**
 * \brief 从message列表表头取出一条 transfer
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_spi_transfer *__spi_gpio_trans_out (am_spi_message_t *msg)
{
    if (am_list_empty_careful(&(msg->transfers))) {
        return NULL;
    } else {
        struct am_list_head *p_node = msg->transfers.next;
        am_list_del(p_node);
        return am_list_entry(p_node, struct am_spi_transfer, trans_node);
    }
}
/**
* \brief 默认CS脚控制函数，高电平有效
*/
am_local
void __spi_gpio_default_cs_ha (am_spi_device_t *p_dev, int state)
{
 am_gpio_set(p_dev->cs_pin, state ? 1 : 0);
}

/**
* \brief 默认CS脚控制函数，低电平有效
*/
am_local
void __spi_gpio_default_cs_la (am_spi_device_t *p_dev, int state)
{
 am_gpio_set(p_dev->cs_pin, state ? 0 : 1);
}

/**
* \brief 默认CS脚控制函数，由硬件自行控制
*/
am_local
void __spi_gpio_default_cs_dummy (am_spi_device_t *p_dev, int state)
{
 return;
}

/**
* \brief CS引脚激活
*/
am_local
void __spi_gpio_cs_on (am_spi_gpio_dev_t *p_this, am_spi_device_t *p_dev)
{
 p_dev->pfunc_cs(p_dev, 1);
}

/**
 * \brief CS引脚去活
 */
am_local
void __spi_gpio_cs_off (am_spi_gpio_dev_t *p_this, am_spi_device_t   *p_dev)
{
    p_dev->pfunc_cs(p_dev, 0);
}

/**
 * \brief SPI配置
 */
void __spi_gpio_config(am_spi_gpio_dev_t *p_this, am_spi_transfer_t *p_trans)
{
    __spi_gpio_sck_idle_state_set(p_this);

}


/******************************************************************************/
/**
 * \brief 获取SPI控制器信息
 */
am_local int __spi_gpio_info_get(void *p_drv, am_spi_info_t *p_info)
{
    if (p_info == NULL) {
        return -AM_EINVAL;
    }

    /* 最大速率等于 PCLK */
    p_info->max_speed = 0;
    p_info->min_speed = 0;
    p_info->features  = AM_SPI_CS_HIGH   |
                        AM_SPI_LSB_FIRST |
                        AM_SPI_MODE_0    |
                        AM_SPI_MODE_1    |
                        AM_SPI_MODE_2    |
                        AM_SPI_MODE_3    |
                        AM_SPI_3WIRE     |
                        AM_SPI_NO_CS     ;   /* features */
    return AM_OK;
}

/**
 * \brief 设置SPI从机设备
 */
am_local int __spi_gpio_set_up(void *p_drv, struct am_spi_device *p_dev)
{
    am_spi_gpio_dev_t *p_this = (am_spi_gpio_dev_t *)p_drv;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 默认数据为8位，最大不超过16位 */
    if (p_dev->bits_per_word == 0) {
        p_dev->bits_per_word = 8;
    } else if (p_dev->bits_per_word > 32) {
        return -AM_ENOTSUP;
    }

    /* 无片选函数 */
    if (p_dev->mode & AM_SPI_NO_CS) {
        p_dev->pfunc_cs = __spi_gpio_default_cs_dummy;

    /* 有片选函数 */
    }  else {

        /* 不提供则默认片选函数 */
        if (p_dev->pfunc_cs == NULL) {

            /* 片选高电平有效 */
            if (p_dev->mode & AM_SPI_CS_HIGH) {
                am_gpio_pin_cfg(p_dev->cs_pin, AM_GPIO_OUTPUT_INIT_LOW);
                p_dev->pfunc_cs = __spi_gpio_default_cs_ha;

            /* 片选低电平有效 */
            } else {
                am_gpio_pin_cfg(p_dev->cs_pin, AM_GPIO_OUTPUT_INIT_HIGH);
                p_dev->pfunc_cs = __spi_gpio_default_cs_la;
            }
        }
    }

    /* 解除片选信号 */
    __spi_gpio_cs_off(p_this, p_dev);

    return AM_OK;
}

/**
 * \brief 消息开始
 */
am_local int __spi_gpio_msg_start (void              *p_drv,
                                   am_spi_device_t   *p_dev,
                                   am_spi_message_t  *p_msg)
{
    am_spi_gpio_dev_t *p_this    = (am_spi_gpio_dev_t *)p_drv;
    int key;

    if ( (p_this             == NULL) ||
         (p_dev              == NULL) ||
         (p_msg              == NULL)) {

        return -AM_EINVAL;
    }

    p_msg->p_spi_dev = p_dev; /* 保存消息对应的设备*/

    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_this->busy == AM_TRUE) {

        __spi_gpio_msg_in(p_this, p_msg);

        am_int_cpu_unlock(key);

        return AM_OK;

    } else {
        p_this->busy = AM_TRUE;
        __spi_gpio_msg_in(p_this, p_msg);
        p_msg->status = -AM_EISCONN; /* 正在排队中 */
        am_int_cpu_unlock(key);

        return __spi_mst_sm_event(p_this, __SPI_GPIO_EVT_MSG_LAUNCH);
    }
}

/******************************************************************************/

#define __spi_gpio_next_state(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

/**
 * \brief  SPI_GPIO 状态机处理
 */
am_local int __spi_mst_sm_event(am_spi_gpio_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t new_event = __SPI_GPIO_EVT_NONE;
    
    __VAR_NO_USE(event);
 
    while (1) {

        /* 检查是否有新的事件在状态机内部产生 */
        if (new_event != __SPI_GPIO_EVT_NONE) {
            event     = new_event;
            new_event = __SPI_GPIO_EVT_NONE;
        }

        switch (p_dev->state) {

        /*
         * 空闲状态和开始消息传输状态要处理的事情是一样，事件只应是：
         * __SPI_EVT_TRANS_LAUNCH
         */
        case __SPI_GPIO_ST_IDLE:
        case __SPI_GPIO_ST_MSG_START:
        {
            am_spi_message_t *p_cur_msg = NULL;

            int key;

            key = am_int_cpu_lock();
            p_cur_msg        = __spi_gpio_msg_out(p_dev);
            p_dev->p_cur_msg     = p_cur_msg;

            if (p_cur_msg) {
                p_cur_msg->status = -AM_EINPROGRESS;
            } else {
                p_dev->busy = AM_FALSE;
            }

            am_int_cpu_unlock(key);

            /* 无需要处理的消息 */
            if (p_cur_msg == NULL) {

                __spi_gpio_next_state(__SPI_GPIO_ST_IDLE, __SPI_GPIO_EVT_NONE);

                break;
            } else {

                p_dev->p_cur_spi_dev = p_cur_msg->p_spi_dev;

                p_cur_msg->actual_length = 0;

                __spi_gpio_next_state(__SPI_GPIO_ST_TRANS_START,
                                      __SPI_GPIO_EVT_TRANS_LAUNCH);

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                event     = new_event;
                new_event = __SPI_GPIO_EVT_NONE;

            }
        }
        /* no break */

        /*　传输开始 */
        case __SPI_GPIO_ST_TRANS_START:
        {
            struct am_spi_message *p_cur_msg =  __spi_gpio_cur_msg(p_dev);

            /* 当前消息传输完成 */
            if (__spi_gpio_trans_empty(p_dev)) {


                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                /* 回调消息完成函数  */
                if (p_cur_msg->pfn_complete != NULL) {
                    p_cur_msg->pfn_complete(p_cur_msg->p_arg);
                }

                /* 片选关闭 */
                __spi_gpio_cs_off(p_dev, p_dev->p_cur_spi_dev);

                __spi_gpio_next_state(__SPI_GPIO_ST_MSG_START,
                                            __SPI_GPIO_EVT_MSG_LAUNCH);

            } else {                    /* 获取到一个传输，正确处理该传输即可 */

                am_spi_transfer_t *p_cur_trans = __spi_gpio_trans_out(p_cur_msg);
                p_dev->p_cur_trans              = p_cur_trans;

                /* 配置SPI 传输参数 */
                __spi_gpio_config(p_dev, p_dev->p_cur_trans);

                /* CS选通 */
                __spi_gpio_cs_on(p_dev, p_dev->p_cur_spi_dev);

                __spi_gpio_next_state(__SPI_GPIO_ST_TX_RX_DATA, __SPI_GPIO_EVT_TRANS_LAUNCH);

            }
            break;
        }

        /* 发送并接受数据状态 */
        case __SPI_GPIO_ST_TX_RX_DATA:
        {
            struct am_spi_transfer *p_cur_trans = __spi_gpio_cur_trans(p_dev);

            /* 开始传输数据 */
            __spi_gpio_trans_data_start(p_dev, p_cur_trans);

            if( p_cur_trans->delay_usecs != 0) {
                am_udelay(p_cur_trans->delay_usecs);
            }

            /* 影响片选 */
            if( 1 == p_cur_trans->cs_change) {
                __spi_gpio_cs_off(p_dev, p_dev->p_cur_spi_dev);
            }

            p_dev->p_cur_msg->actual_length += p_dev->p_cur_trans->nbytes;

            __spi_gpio_next_state(__SPI_GPIO_ST_TRANS_START, __SPI_GPIO_EVT_TRANS_LAUNCH);

            break;
        }


        default:
            break;
        }

        /* 无事件退出 */
        if (new_event == __SPI_GPIO_EVT_NONE) {
            break;
        }
    }

    return AM_OK;
}

/******************************************************************************/

am_spi_handle_t am_spi_gpio_init (am_spi_gpio_dev_t           *p_dev,
                                  const am_spi_gpio_devinfo_t *p_devinfo)
{
    p_dev->p_devinfo = p_devinfo;

    p_dev->spi_serv.p_drv = p_dev;
    p_dev->spi_serv.p_funcs = (struct am_spi_drv_funcs *)&__g_spi_gpio_drv_funcs;

    p_dev->p_cur_trans = NULL;
    p_dev->p_cur_msg   = NULL;
    p_dev->sck_state   = 0;
    p_dev->busy        = AM_FALSE;
    p_dev->state       = __SPI_GPIO_ST_IDLE;

    am_list_head_init(&(p_dev->msg_list));

    __spi_gpio_hw_init(p_devinfo);

    return &(p_dev->spi_serv);
}

void am_spi_gpio_deinit (am_spi_handle_t handle)
{
}

/* end of file */
