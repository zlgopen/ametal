/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2012 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SPI drivers implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 19-11-01  htf, first implementation.
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_lpc_spi_poll.h"

#define __SPI_ST_IDLE  0
/*******************************************************************************
  模块内函数声明
*******************************************************************************/
am_local void __spi_default_cs_ha    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_la    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_dummy (am_spi_device_t *p_dev, int state);

am_local void __spi_cs_on  (am_lpc_spi_poll_dev_t *p_this, am_spi_device_t *p_dev);
am_local void __spi_cs_off (am_lpc_spi_poll_dev_t *p_this, am_spi_device_t *p_dev);

am_local void __spi_write_data (am_lpc_spi_poll_dev_t *p_dev);
am_local void __spi_read_data (am_lpc_spi_poll_dev_t *p_dev);

am_local int  __spi_hard_init (am_lpc_spi_poll_dev_t *p_this);
am_local int  __spi_config (am_lpc_spi_poll_dev_t *p_this);

am_local int  __spi_mst_sm_event (am_lpc_spi_poll_dev_t *p_dev);
/*******************************************************************************
  SPI驱动函数声明
*******************************************************************************/
am_local int __spi_info_get (void *p_arg, am_spi_info_t   *p_info);
am_local int __spi_setup    (void *p_arg, am_spi_device_t *p_dev );
am_local int __spi_msg_start (void              *p_drv,
                              am_spi_device_t   *p_dev,
                              am_spi_message_t  *p_msg);

/**
 * \brief SPI 驱动函数
 */
am_local am_const struct am_spi_drv_funcs __g_spi_drv_funcs = {
    __spi_info_get,
    __spi_setup,
    __spi_msg_start,
};

/******************************************************************************/

/**
 * \brief 默认CS脚控制函数，高电平有效
 */
am_local
void __spi_default_cs_ha (am_spi_device_t *p_dev, int state)
{
    am_gpio_set(p_dev->cs_pin, state ? 1 : 0);
}

/**
 * \brief 默认CS脚控制函数，低电平有效
 */
am_local
void __spi_default_cs_la (am_spi_device_t *p_dev, int state)
{
    am_gpio_set(p_dev->cs_pin, state ? 0 : 1);
}

/**
 * \brief 默认CS脚控制函数，由硬件自行控制
 */
am_local
void __spi_default_cs_dummy (am_spi_device_t *p_dev, int state)
{
    return;
}

/**
 * \brief CS引脚激活
 */
am_local
void __spi_cs_on (am_lpc_spi_poll_dev_t *p_this, am_spi_device_t *p_dev)
{
    /* if last device toggled after message */
    if (p_this->p_tgl_dev != NULL) {

        /* last message on defferent device */
        if (p_this->p_tgl_dev != p_dev) {
            p_this->p_tgl_dev->pfunc_cs(p_this->p_tgl_dev, 0);
        }
        p_this->p_tgl_dev = NULL;
    }

    p_dev->pfunc_cs(p_dev, 1);
}

/**
 * \brief CS引脚去活
 */
am_local
void __spi_cs_off (am_lpc_spi_poll_dev_t   *p_this,
                   am_spi_device_t  *p_dev)
{
    if (p_this->p_tgl_dev == p_dev) {
        p_this->p_tgl_dev = NULL;
    }

    p_dev->pfunc_cs(p_dev, 0);
}

/******************************************************************************/

/**
 * \brief SPI 硬件初始化
 */
am_local
int __spi_hard_init (am_lpc_spi_poll_dev_t *p_this)
{
    amhw_lpc_spi_t *p_hw_spi = (amhw_lpc_spi_t *)(p_this->p_devinfo->spi_regbase);

    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    /* 等待SPI空闲，方可设置配置寄存器 */
    while ((amhw_lpc_spi_stat_get(p_hw_spi) & AMHW_LPC_SPI_STAT_IDLE) == 0);
    amhw_lpc_spi_cfg_set(p_hw_spi,
                         (AMHW_LPC_SPI_CFG_ENABLE |
                         AMHW_LPC_SPI_CFG_MASTER));

    /* 初始化配置SPI */
    return AM_OK;
}

am_local
int __spi_info_get (void *p_arg, am_spi_info_t *p_info)
{
    am_lpc_spi_poll_dev_t  *p_this   = (am_lpc_spi_poll_dev_t *)p_arg;

    if (p_info == NULL) {
        return -AM_EINVAL;
    }

    /* 最大速率等于 PCLK */
    p_info->max_speed = am_clk_rate_get(p_this->p_devinfo->clk_id);
    p_info->min_speed = p_info->max_speed / 65536;
    p_info->features  = AM_SPI_CPHA  |
                        AM_SPI_CPOL  |
                        AM_SPI_NO_CS |
                        AM_SPI_LOOP  |
                        AM_SPI_CS_HIGH;   /* features */
    return AM_OK;
}

/******************************************************************************/
am_local
int __spi_config (am_lpc_spi_poll_dev_t *p_this)
{
    const am_lpc_spi_poll_devinfo_t *p_devinfo = p_this->p_devinfo;
    amhw_lpc_spi_t                 *p_hw_spi  = (amhw_lpc_spi_t *)(p_devinfo->spi_regbase);
    am_spi_transfer_t              *p_trans   = p_this->p_cur_trans;

    uint32_t                       mode_flag = 0;
    uint32_t                       div_val;

    /* 如果为0，使用默认参数值 */
    if (p_trans->bits_per_word == 0) {
        p_trans->bits_per_word = p_this->p_cur_spi_dev->bits_per_word;
    }

    if (p_trans->speed_hz == 0) {
        p_trans->speed_hz = p_this->p_cur_spi_dev->max_speed_hz;
    }

    /* 设置字节数有效性检查 */
    if (p_trans->bits_per_word > 16 || p_trans->bits_per_word < 4) {
        return -AM_EINVAL;
    }

    /* 设置分频值有效性检查 */
    if (p_trans->speed_hz > am_clk_rate_get(p_this->p_devinfo->clk_id) ||
        p_trans->speed_hz < (am_clk_rate_get(p_this->p_devinfo->clk_id) / 65536)) {
        return -AM_EINVAL;
    }

    /* 发送和接收缓冲区有效性检查 */
    if ((p_trans->p_txbuf == NULL) && (p_trans->p_rxbuf == NULL)) {
        return -AM_EINVAL;
    }

    /* 发送字节数检查 */
    if (p_trans->nbytes == 0) {
        return -AM_ELOW;
    }

    /**
     * 配置当前设备模式
     */
    mode_flag = 0;

    if (p_this->p_cur_spi_dev->mode & AM_SPI_CPHA) {
        mode_flag |= AMHW_LPC_SPI_CFG_CHANGE;
    }
    if (p_this->p_cur_spi_dev->mode & AM_SPI_CPOL) {
        mode_flag |= AMHW_LPC_SPI_CFG_STAT_HIGH;
    }
    if (p_this->p_cur_spi_dev->mode & AM_SPI_CS_HIGH) {
        mode_flag |= AMHW_LPC_SPI_CFG_SPOL0_HIGH;
    }
    if (p_this->p_cur_spi_dev->mode & AM_SPI_LSB_FIRST) {
        mode_flag |= AMHW_LPC_SPI_CFG_LSB;
    }
    if (p_this->p_cur_spi_dev->mode & AM_SPI_LOOP) {
        mode_flag |= AMHW_LPC_SPI_CFG_LOOP;
    }

    /* 配置时先停止主机  */
    amhw_lpc_spi_disable(p_hw_spi);

    /* 设置模式(可在这里设置主从机模式) */
    amhw_lpc_spi_cfg_set(p_hw_spi, (mode_flag               |
                                    AMHW_LPC_SPI_CFG_ENABLE |
                                    AMHW_LPC_SPI_CFG_MASTER));

    /* 设置单帧数据位数 */
    amhw_lpc_spi_data_flen_set (p_hw_spi, p_trans->bits_per_word);

    /* 设置分频值，即SPI速率 */
    div_val = (am_clk_rate_get(p_this->p_devinfo->clk_id) / p_trans->speed_hz);
    amhw_lpc_spi_div_set(p_hw_spi, div_val);

    /* 清除状态位 */
    amhw_lpc_spi_stat_clear(p_hw_spi, AMHW_LPC_SPI_STAT_RXOVERRUN   |
                                      AMHW_LPC_SPI_STAT_TXUNDERRUN  |
                                      AMHW_LPC_SPI_STAT_SELNASSERT  |
                                      AMHW_LPC_SPI_STAT_SELNDEASSERT);

    /* 使能SPI接收发送溢出中断 */
    amhw_lpc_spi_int_enable(p_hw_spi,
                            AMHW_LPC_SPI_INTENSET_RXOVERRUN |
                            AMHW_LPC_SPI_INTENSET_TXUNDERRUN);

    return AM_OK;
}

/******************************************************************************/
am_local
int __spi_setup (void *p_arg, am_spi_device_t *p_dev)
{
    am_lpc_spi_poll_dev_t *p_this   = (am_lpc_spi_poll_dev_t *)p_arg;

    uint32_t max_speed, min_speed;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 默认数据为8位，最大不超过16位 */
    if (p_dev->bits_per_word == 0) {
        p_dev->bits_per_word = 8;
    } else if (p_dev->bits_per_word > 16) {
        return -AM_ENOTSUP;
    }

    /* 最大SPI速率不能超过主时钟，最小不能小于主时钟65536分频 */
    max_speed = am_clk_rate_get(p_this->p_devinfo->clk_id);
    min_speed = max_speed / 65536;

    if (p_dev->max_speed_hz > max_speed) {
        p_dev->max_speed_hz = max_speed;
    } else if (p_dev->max_speed_hz < min_speed) {
        return -AM_ENOTSUP;
    }

    /* 无片选函数 */
    if (p_dev->mode & AM_SPI_NO_CS) {
        p_dev->pfunc_cs = __spi_default_cs_dummy;

    /* 有片选函数 */
    }  else {

        /* 不提供则默认片选函数 */
        if (p_dev->pfunc_cs == NULL) {

            /* 片选高电平有效 */
            if (p_dev->mode & AM_SPI_CS_HIGH) {
                am_gpio_pin_cfg(p_dev->cs_pin, AM_GPIO_OUTPUT_INIT_LOW);
                p_dev->pfunc_cs = __spi_default_cs_ha;

            /* 片选低电平有效 */
            } else {
                am_gpio_pin_cfg(p_dev->cs_pin, AM_GPIO_OUTPUT_INIT_HIGH);
                p_dev->pfunc_cs = __spi_default_cs_la;
            }
        }
    }

    /* 解除片选信号 */
    __spi_cs_off(p_this, p_dev);

    return AM_OK;
}

/******************************************************************************/

am_local
void __spi_write_data (am_lpc_spi_poll_dev_t *p_dev)
{
    amhw_lpc_spi_t *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);
    am_spi_transfer_t *p_trans  = p_dev->p_cur_trans;

    /* tx_buf 有效 */
    if (p_trans->p_txbuf != NULL) {
        if ((p_dev->p_cur_spi_dev->bits_per_word) <= 8) {
            /** \brief 待发送数据的基址+偏移 */
            uint8_t *ptr = (uint8_t *)(p_trans->p_txbuf) + p_dev->data_ptr;
            amhw_lpc_spi_txdat_write(p_hw_spi, *ptr);
        } else {
            uint16_t *ptr = (uint16_t *)((uint8_t *)(p_trans->p_txbuf)
                                                     + p_dev->data_ptr);
        }
    } else {
        /** \brief 待发送数据无效 直接发0, 由于是全双工关系，这在读的时候必须要发数据，芯片才回你一个要读的数据  */
        amhw_lpc_spi_txdat_write(p_hw_spi, 0x0000);
    }
    /** \brief 需接收数据的byte数 */
    p_dev->nbytes_to_recv = p_dev->p_cur_spi_dev->bits_per_word < 9 ? 1 : 2;
    p_dev->p_cur_msg->actual_length += p_dev->nbytes_to_recv;
}

am_local
void __spi_read_data (am_lpc_spi_poll_dev_t *p_dev)
{
    amhw_lpc_spi_t *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);
    am_spi_transfer_t *p_trans  = p_dev->p_cur_trans;
    uint8_t *p_buf8  = (uint8_t *)p_trans->p_rxbuf + p_dev->data_ptr;

    /* rx_buf 有效 */
    if (p_trans->p_rxbuf != NULL && p_dev->nbytes_to_recv) {
        if ((p_dev->p_cur_spi_dev->bits_per_word) <= 8) {
             *p_buf8 = amhw_lpc_spi_rxdata_8bit_read(p_hw_spi);
        } else {
             *(uint16_t *)(p_buf8) = amhw_lpc_spi_rxdata_16bit_read(p_hw_spi);
        }
    /* rx_buf 无效或者不需要接收数据 */
    } else {
        if ((p_dev->p_cur_spi_dev->bits_per_word) <= 8) {
            (void)amhw_lpc_spi_rxdata_8bit_read(p_hw_spi);

        } else {
            (void)amhw_lpc_spi_rxdata_16bit_read(p_hw_spi);
        }
    }

    /* 已经发送或接收的数据byte数 */
    p_dev->data_ptr += p_dev->nbytes_to_recv;
    p_dev->nbytes_to_recv = 0;
}

/**
 * \brief 从message列表表头取出一条 transfer
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_spi_transfer *__spi_trans_out (am_spi_message_t *msg)
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
 * \brief SPI 传输数据函数
 */
am_local
int __spi_msg_start (void              *p_drv,
                     am_spi_device_t   *p_dev,
                     am_spi_message_t  *p_msg)
{
    am_lpc_spi_poll_dev_t *p_this   = (am_lpc_spi_poll_dev_t *)p_drv;
    amhw_lpc_spi_t      *p_hw_spi  = (amhw_lpc_spi_t *)(p_this->p_devinfo->spi_regbase);

    p_this->p_cur_spi_dev  = p_dev;                         /* 将当前设备参数信息存入 */
    p_this->p_cur_msg      = p_msg;                         /* 将当前设备传输消息存入 */
    p_this->nbytes_to_recv = 0;                             /* 待接收字符数清0 */
    p_this->data_ptr       = 0;                             /* 已接收字符数清0 */


    /* 设备有效性检查 */
    if ((p_drv              == NULL) ||
        (p_dev              == NULL) ||
        (p_msg              == NULL) ){
        return -AM_EINVAL;
    }

    /* 清除传输完成结束位 */
    amhw_lpc_spi_txctl_clear(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);

    p_this->busy = AM_TRUE;
    p_msg->status = -AM_EISCONN; /* 正在排队中 */

    /* 启动状态机 */
    return __spi_mst_sm_event(p_this);
}


/******************************************************************************/

/******************************************************************************/
/**
 * \brief  SPI 使用状态机传输
 */
am_local
int __spi_mst_sm_event (am_lpc_spi_poll_dev_t *p_dev)
{
    amhw_lpc_spi_t *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);
    am_spi_message_t  *p_cur_msg   = NULL;
    p_cur_msg = p_dev->p_cur_msg;

    while (!(am_list_empty(&(p_cur_msg->transfers)))) {

        am_spi_transfer_t *p_cur_trans = __spi_trans_out(p_cur_msg);
        p_dev->p_cur_trans             = p_cur_trans;

        /* reset current data pointer */
        p_dev->data_ptr                = 0;
        p_dev->nbytes_to_recv          = 0;

        /* 如果该传输指定SPI字节数传输  则进入该判断 用户自定义设置  */
        if(p_dev->p_cur_trans->bits_per_word != 0){

            amhw_lpc_spi_data_flen_set (p_hw_spi, p_dev->p_cur_spi_dev->bits_per_word);
            p_dev->bef_bits_per_word = p_dev->p_cur_trans->bits_per_word;
        /* 如果该传输默认SPI字节数传输  且上次传输保存的SPI字节数与 SPI 设备不同  则进行重新设置  */
        }else if(p_dev->p_cur_trans->bits_per_word == 0 &&
               p_dev->bef_bits_per_word != p_dev->p_cur_spi_dev->bits_per_word){

            p_dev->bef_bits_per_word = p_dev->p_cur_spi_dev->bits_per_word;
            /* 判断使用何种传输方式   改变时进行重新配置 */
            amhw_lpc_spi_data_flen_set (p_hw_spi, p_dev->p_cur_spi_dev->bits_per_word);
            p_dev->bef_bits_per_word = p_dev->p_cur_spi_dev->bits_per_word;
        }
        /* 如果该传输指定SPI速度传输  则进入该判断 用户自定义设置  */
        if(p_dev->p_cur_trans->speed_hz != 0){
            __spi_config(p_dev);
            p_dev->bef_speed_hz = p_dev->p_cur_trans->speed_hz;
        /* 如果该传输默认SPI速度传输  且上次传输保存的SPI字节数与 SPI 设备不同  则进行重新设置  */
        }else if(p_dev->p_cur_trans->speed_hz == 0 &&
                p_dev->bef_speed_hz != p_dev->p_cur_spi_dev->max_speed_hz ){
            /* 配置SPI传输参数    改变时进行重新配置     */
            __spi_config(p_dev);
            p_dev->bef_speed_hz = p_dev->p_cur_spi_dev->max_speed_hz;
        }

        /* CS选通 */
        __spi_cs_on(p_dev, p_dev->p_cur_spi_dev);

        while(p_dev->data_ptr < p_cur_trans->nbytes){

            if ((amhw_lpc_spi_stat_get(p_hw_spi) & AMHW_LPC_SPI_STAT_TXRDY)!= 0) {

                if (p_dev->p_cur_spi_dev->bits_per_word <= 8) {
                    if ((p_dev->data_ptr == (p_cur_trans->nbytes - 1))) {
                        amhw_lpc_spi_txctl_set(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);
                    } else {
                        amhw_lpc_spi_txctl_clear(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);
                    }
                } else {
                    if ((p_dev->data_ptr == (p_cur_trans->nbytes - 2))) {
                        amhw_lpc_spi_txctl_set(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);
                    } else {
                        amhw_lpc_spi_txctl_clear(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);
                    }
                }
                /* SPI写数据 */
                __spi_write_data(p_dev);

                while(p_dev->nbytes_to_recv != 0){
                    /* SPI读取数据 */
                    __spi_read_data(p_dev);
                }
            }
        }
    }

    /* notify the caller  */
    if (p_cur_msg->pfn_complete != NULL) {
        p_cur_msg->pfn_complete(p_cur_msg->p_arg);
    }

    /* 片选关闭 */
    __spi_cs_off(p_dev, p_dev->p_cur_spi_dev);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief SPI 初始化
 */
am_spi_handle_t am_lpc_spi_poll_init (am_lpc_spi_poll_dev_t            *p_dev,
                                      const am_lpc_spi_poll_devinfo_t  *p_devinfo)
{
    if (NULL == p_devinfo || NULL == p_dev ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->spi_serve.p_funcs = (struct am_spi_drv_funcs *)&__g_spi_drv_funcs;
    p_dev->spi_serve.p_drv   = p_dev;

    p_dev->p_devinfo = p_devinfo;

    p_dev->p_cur_spi_dev    = NULL;
    p_dev->p_tgl_dev        = NULL;
    p_dev->busy             = AM_FALSE;
    p_dev->p_cur_msg        = NULL;
    p_dev->p_cur_trans      = NULL;
    p_dev->data_ptr         = 0;
    p_dev->nbytes_to_recv   = 0;
    p_dev->state            = __SPI_ST_IDLE;     /* 初始化为空闲状态 */

    am_list_head_init(&(p_dev->msg_list));

    if (__spi_hard_init(p_dev) != AM_OK) {
        return NULL;
    }

    return &(p_dev->spi_serve);
}

/**
 * \brief SPI 去除初始化
 */
void am_lpc_spi_poll_deinit (am_spi_handle_t handle)
{
    am_lpc_spi_poll_dev_t *p_dev    = (am_lpc_spi_poll_dev_t *)handle;
    amhw_lpc_spi_t       *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);

    if (NULL == p_dev) {
        return ;
    }

    p_dev->spi_serve.p_funcs = NULL;
    p_dev->spi_serve.p_drv   = NULL;

    /* 禁能 SPI */
    amhw_lpc_spi_disable(p_hw_spi);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}
/* end of file */
