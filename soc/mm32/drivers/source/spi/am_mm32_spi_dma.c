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
 * \brief SPI 驱动层实现函数
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  ari, first implementation
 * \endinternal
 */

/*******************************************************************************
includes
*******************************************************************************/

#include "ametal.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_clk.h"
#include "am_mm32_dma.h"
#include "hw/amhw_mm32_spi.h"
#include "am_mm32_spi_dma.h"

/*******************************************************************************
  SPI 状态和事件定义
*******************************************************************************/

/** \brief 获取SPI的输入频率 */
#define __SPI_FRQIIN_GET(p_hw_spi)    am_clk_rate_get(p_this->p_devinfo->clk_id)

/** \brief 获取SPI的支持的最大速度 */
#define __SPI_MAXSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 2)

/** \brief 获取SPI支持的最小速度 */
#define __SPI_MINSPEED_GET(p_hw_spi) (__SPI_FRQIIN_GET(p_hw_spi) / 4096)

/**
 * \brief SPI 控制器状态
 */

#define __SPI_ST_IDLE               0                   /**< \brief空闲状态 */
#define __SPI_ST_MSG_START          1                   /**< \brief消息开始 */
#define __SPI_ST_TRANS_START        2                   /**< \brief传输开始 */
#define __SPI_ST_DMA_TRANS_DATA     3                   /**< \briefDMA 传输 */

/**
 * \briefSPI 控制器事件
 *
 */

#define __SPI_EVT_NUM_GET(event)    ((event) & 0xFFFF)
#define __SPI_EVT_PAR_GET(event)    ((event >> 16) & 0xFFFF)
#define __SPI_EVT(evt_num, evt_par) (((evt_num) & 0xFFFF) | ((evt_par) << 16))

#define __SPI_EVT_NONE              __SPI_EVT(0, 0)     /**< \brief 无事件 */
#define __SPI_EVT_TRANS_LAUNCH      __SPI_EVT(1, 0)     /**< \brief 传输就绪 */
#define __SPI_EVT_DMA_TRANS_DATA    __SPI_EVT(2, 0)     /**< \brief DMA传输数据 */

/*******************************************************************************
  模块内函数声明
*******************************************************************************/
am_local void __spi_default_cs_ha    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_la    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_dummy (am_spi_device_t *p_dev, int state);

am_local void __spi_cs_on  (am_mm32_spi_dma_dev_t *p_this, am_spi_device_t *p_dev);
am_local void __spi_cs_off (am_mm32_spi_dma_dev_t *p_this, am_spi_device_t *p_dev);

am_local int  __spi_hard_init (am_mm32_spi_dma_dev_t *p_this);
am_local int  __spi_config (am_mm32_spi_dma_dev_t *p_this);

am_local uint32_t __spi_speed_cfg (am_mm32_spi_dma_dev_t *p_dev, uint32_t target_baud);

am_local int  __spi_mst_sm_event (am_mm32_spi_dma_dev_t *p_dev, uint32_t event);

/*******************************************************************************
  SPI驱动函数声明
*******************************************************************************/
am_local int __spi_info_get  (void *p_arg, am_spi_info_t   *p_info);
am_local int __spi_setup     (void *p_arg, am_spi_device_t *p_dev );
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
void __spi_cs_on (am_mm32_spi_dma_dev_t *p_this, am_spi_device_t *p_dev)
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
void __spi_cs_off (am_mm32_spi_dma_dev_t   *p_this,
                   am_spi_device_t           *p_dev)
{
    if (p_this->p_tgl_dev == p_dev) {
        p_this->p_tgl_dev = NULL;
    }

    p_dev->pfunc_cs(p_dev, 0);
}

/******************************************************************************/

/**
 * \brief 添加一条 message 到控制器传输列表末尾
 * \note调用此函数必须锁定控制器
 */
am_static_inline
void __spi_msg_in (am_mm32_spi_dma_dev_t *p_dev, struct am_spi_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                                             &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 * \note调用此函数必须锁定控制器
 */
am_static_inline
struct am_spi_message *__spi_msg_out (am_mm32_spi_dma_dev_t *p_dev)
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
 * \note 调用此函数必须锁定控制器
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

/******************************************************************************/

am_local
int __spi_setup (void *p_arg, am_spi_device_t *p_dev)
{
    am_mm32_spi_dma_dev_t *p_this = (am_mm32_spi_dma_dev_t *)p_arg;

    uint32_t max_speed, min_speed;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    /* 默认数据为8位，最大不超过16位 */
    if (p_dev->bits_per_word == 0) {
        p_dev->bits_per_word = 8;
    } else if (p_dev->bits_per_word > 32) {
        return -AM_ENOTSUP;
    }

    /* 最大SPI速率不能超过主时钟，最小不能小于主时钟65536分频 */
    max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    min_speed = __SPI_MINSPEED_GET(p_hw_spi);

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

am_local
int __spi_info_get (void *p_arg, am_spi_info_t *p_info)
{
    am_mm32_spi_dma_dev_t *p_this   = (am_mm32_spi_dma_dev_t *)p_arg;

    if (p_info == NULL) {
        return -AM_EINVAL;
    }

    /* 最大速率等于 PCLK */
    p_info->max_speed = __SPI_MAXSPEED_GET(p_hw_spi);
    p_info->min_speed = __SPI_MINSPEED_GET(p_hw_spi);
    p_info->features  = AM_SPI_CS_HIGH   |
                        AM_SPI_LSB_FIRST |
                        AM_SPI_MODE_0    |
                        AM_SPI_MODE_1    |
                        AM_SPI_MODE_2    |
                        AM_SPI_MODE_3;   /* features */
    return AM_OK;
}

/**
 * \brief SPI 硬件初始化
 */
am_local
int __spi_hard_init (am_mm32_spi_dma_dev_t *p_this)
{
    amhw_mm32_spi_t *p_hw_spi = (amhw_mm32_spi_t *)(p_this->p_devinfo->spi_reg_base);

    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    /* 初始化配置SPI */
    amhw_mm32_spi_mode_sel(p_hw_spi, AMHW_MM32_SPI_MODE_MASTER);
    amhw_mm32_spi_dma_enable(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_tx_enable(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_rx_enable(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_module_enable(p_hw_spi, AM_TRUE);

    amhw_mm32_spi_int_open(p_hw_spi, AM_TRUE);
    amhw_mm32_spi_int_enable(p_hw_spi, AMHW_MM32_SPI_INTSTAT_TXEPT_FALG);

    return AM_OK;
}

am_local
void __spi_isr (void *p_arg)
{
    am_mm32_spi_dma_dev_t *p_dev    = (am_mm32_spi_dma_dev_t *)p_arg;
    amhw_mm32_spi_t       *p_hw_spi = (amhw_mm32_spi_t *)(p_dev->p_devinfo->spi_reg_base);

    uint32_t spi_status = (uint32_t)amhw_mm32_spi_reg_cstat_get(p_hw_spi);

    /* 发送完成中断 */
    if((spi_status & AMHW_MM32_SPI_CSTAT_TX_EMPTY)) {
        amhw_mm32_spi_int_flag_clr(p_hw_spi, AMHW_MM32_SPI_INTSTAT_TXEPT_FALG);

        if ((p_dev != NULL) &&
            (p_dev->p_isr_msg != NULL) &&
            (p_dev->p_isr_msg->pfn_complete != NULL)) {
            p_dev->p_isr_msg->pfn_complete(p_dev->p_isr_msg->p_arg);
        }

        /* 片选关闭 */
        __spi_cs_off(p_dev, p_dev->p_cur_spi_dev);

        p_dev->busy = AM_FALSE;
    }
}

am_local
void __dma_isr (void *p_arg, uint32_t stat)
{
    am_mm32_spi_dma_dev_t *p_this   = (am_mm32_spi_dma_dev_t *)p_arg;

    /* 中断发生 */
    if (stat == AM_MM32_DMA_INT_NORMAL)
    {
        /* 记录成功传送字节数 */
        if (p_this->p_cur_trans->p_txbuf != NULL) {
            p_this->p_cur_msg->actual_length += (p_this->p_cur_trans->nbytes)
                            *(p_this->p_cur_spi_dev->bits_per_word < 9 ? 1 : 2);
        }

        /* 传输就绪 */
        __spi_mst_sm_event(p_this, __SPI_EVT_TRANS_LAUNCH);

    } else {
        /* 中断源不匹配 */
    }
}

/**
 * \brief SPI 使用DMA传输
 */
am_local
int __spi_dma_trans (am_mm32_spi_dma_dev_t *p_dev)
{
    am_mm32_spi_dma_dev_t           *p_this      = (am_mm32_spi_dma_dev_t *)p_dev;
    const am_mm32_spi_dma_devinfo_t *p_devinfo   = p_this->p_devinfo;
    amhw_mm32_spi_t                 *p_hw_spi    = (amhw_mm32_spi_t *)(p_devinfo->spi_reg_base);
    am_spi_transfer_t              *p_cur_trans = p_dev->p_cur_trans;

    const void     *p_txbuf     = NULL;
    static uint16_t tx_rx_trans = 0; 
    uint32_t   dma_flags[2]     = {0};  /* DMA发送通道描述符 */

    /* 连接DMA中断服务函数 */
    am_mm32_dma_isr_connect(p_this->p_devinfo->dma_chan_tx, 
                           __dma_isr, 
                           (void *)p_this);

    /* DMA发送通道配置 */
    dma_flags[0] =  AMHW_MM32_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
                    AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT         |  /* 内存数据宽度1字节 */
                    AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT         |  /* 外设数据宽度1字节 */
                    AMHW_MM32_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 外设地址自增 */
                    AMHW_MM32_DMA_CHAN_CIRCULAR_MODE_DISABLE ;  /* 关闭循环模式 */

    /* DMA接收通道配置 */
    dma_flags[1] = AMHW_MM32_DMA_CHAN_PRIORITY_HIGH         |  /* 中断优先级 高 */
                   AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT         |  /* 内存数据宽度1字节 */
                   AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT         |  /* 外设数据宽度1字节 */
                   AMHW_MM32_DMA_CHAN_PER_ADD_INC_DISABLE   |  /* 外设地址自增 */
                   AMHW_MM32_DMA_CHAN_CIRCULAR_MODE_DISABLE ;  /* 关闭循环模式 */

    /* 设置DMA数据位宽 */
    if (p_cur_trans->bits_per_word > 8) {
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT;
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT;
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT;
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT;
    } else {
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT;
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT;
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_MEM_SIZE_8BIT;
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_PER_SIZE_8BIT;
    }

    /* 只发送不接收数据 */
    if (p_cur_trans->p_rxbuf == NULL) {

        /* 内存地址不自增 */
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_MEM_ADD_INC_DISABLE;

        /* 建立接收通道描述符 */
        am_mm32_dma_xfer_desc_build(&(p_this->g_desc[1]),            /* 通道描述符 */
                                    (uint32_t)(&(p_hw_spi->rxreg)), /* 源缓冲区首地址 */
                                    (uint32_t)(&tx_rx_trans),       /* 目的缓冲区首地址 */
                                     p_cur_trans->nbytes,           /* 传输字节数 */
                                     dma_flags[1]);                 /* 传输配置 */

    /* 存在接收数据 */
    } else {

        /* 内存地址自增 */
        dma_flags[1] |= AMHW_MM32_DMA_CHAN_MEM_ADD_INC_ENABLE;

        /* 建立接收通道描述符 */ 
        am_mm32_dma_xfer_desc_build(&(p_this->g_desc[1]),              /* 通道描述符 */
                                    (uint32_t)(&(p_hw_spi->rxreg)),   /* 源缓冲区首地址 */
                                    (uint32_t)(p_cur_trans->p_rxbuf), /* 目的缓冲区首地址 */
                                     p_cur_trans->nbytes,             /* 传输字节数 */
                                     dma_flags[1]);                   /* 传输配置 */
    }

    if (p_cur_trans->p_txbuf == NULL) {

        tx_rx_trans = 0;
        p_txbuf = &tx_rx_trans;

        /* 内存地址不自增 */
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_MEM_ADD_INC_DISABLE;

        /* 建立发送通道描述符 */
        am_mm32_dma_xfer_desc_build(&(p_this->g_desc[0]),             /* 通道描述符 */
                                    (uint32_t)(p_txbuf),             /* 源缓冲区首地址 */
                                    (uint32_t)(&(p_hw_spi->txreg)),  /* 目的缓冲区首地址 */
                                    p_cur_trans->nbytes,             /* 传输字节数 */
                                    dma_flags[0]);                   /* 传输配置 */
    } else {
        p_txbuf = p_cur_trans->p_txbuf;

        /* 内存地址自增 */
        dma_flags[0] |= AMHW_MM32_DMA_CHAN_MEM_ADD_INC_ENABLE;

        /* 建立发送通道描述符 */
        am_mm32_dma_xfer_desc_build(&(p_this->g_desc[0]),             /* 通道描述符 */
                                    (uint32_t)((uint8_t *)p_txbuf),  /* 源缓冲区首地址 */
                                    (uint32_t)(&(p_hw_spi->txreg)),  /* 目的缓冲区首地址 */
                                    p_cur_trans->nbytes,             /* 传输字节数 */
                                    dma_flags[0]);                   /* 传输配置 */
    }

    if (am_mm32_dma_xfer_desc_chan_cfg(&(p_this->g_desc[0]),
                                        AMHW_MM32_DMA_MER_TO_PER,  /* 内存到外设 */
                                        p_this->p_devinfo->dma_chan_tx) == AM_ERROR) {
        return AM_ERROR;
    }

    if (am_mm32_dma_xfer_desc_chan_cfg(&(p_this->g_desc[1]),
                                        AMHW_MM32_DMA_PER_TO_MER,  /* 外设到内存 */
                                        p_this->p_devinfo->dma_chan_rx) == AM_ERROR) {
        return AM_ERROR;
    }

    am_mm32_dma_chan_start(p_this->p_devinfo->dma_chan_rx);
    am_mm32_dma_chan_start(p_this->p_devinfo->dma_chan_tx);

    return AM_OK;
}

am_local
int __spi_config (am_mm32_spi_dma_dev_t *p_this)
{
    const am_mm32_spi_dma_devinfo_t *p_devinfo = p_this->p_devinfo;
    amhw_mm32_spi_t                 *p_hw_spi  = (amhw_mm32_spi_t *)(p_devinfo->spi_reg_base);
    am_spi_transfer_t              *p_trans   = p_this->p_cur_trans;

    uint32_t  mode_flag = 0;

    /* 如果为0，使用默认参数值 */
    if (p_trans->bits_per_word == 0) {
        p_trans->bits_per_word = p_this->p_cur_spi_dev->bits_per_word;
    }

    if (p_trans->speed_hz == 0) {
        p_trans->speed_hz = p_this->p_cur_spi_dev->max_speed_hz;
    }

    /* 设置字节数有效性检查 */
    if (p_trans->bits_per_word > 32 || p_trans->bits_per_word < 1) {
        return -AM_EINVAL;
    }

    /* 设置分频值有效性检查 */
    if (p_trans->speed_hz > __SPI_MAXSPEED_GET(p_hw_spi) ||
        p_trans->speed_hz < __SPI_MINSPEED_GET(p_hw_spi)) {
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

    if (p_this->p_cur_spi_dev->mode & AM_SPI_LSB_FIRST) {
        mode_flag |= AMHW_MM32_SPI_DATA_LSB;
    }

    /* 配置SPI模式（时钟相位和极性） */
    amhw_mm32_spi_clk_mode_set(p_hw_spi, (0x3 & p_this->p_cur_spi_dev->mode) ^ 0x01);

    /* 配置SPI工作特性 */
    amhw_mm32_spi_first_bit_sel(p_hw_spi, (1u << 2) & mode_flag);

    amhw_mm32_spi_data_len_sel(p_hw_spi, AMHW_MM32_SPI_DATA_LEN_8BIT);

    /* 配置为主机模式 */
    amhw_mm32_spi_mode_sel(p_hw_spi, AMHW_MM32_SPI_MODE_MASTER);

    if (p_this->p_cur_spi_dev->bits_per_word > 8) {
        amhw_mm32_spi_valid_data_sel(p_hw_spi, AMHW_MM32_SPI_VALID_DATA_32BIT);
        amhw_mm32_spi_first_bit_sel(p_hw_spi, AMHW_MM32_SPI_DATA_LSB);
        if (p_this->p_cur_spi_dev->bits_per_word == 32) {
            amhw_mm32_spi_extlen_set(p_hw_spi, 0);
        } else {
            amhw_mm32_spi_extlen_set(p_hw_spi, p_this->p_cur_spi_dev->bits_per_word);
        }
    }

    if (p_trans->speed_hz > (72000000 / 5)) {
        /* 高速 */
        amhw_mm32_spi_tx_data_edge_sel(p_hw_spi, AMHW_MM32_SPI_TX_DATA_IMMEDIATELY);
        amhw_mm32_spi_rx_data_edge_sel(p_hw_spi, AMHW_MM32_SPI_RX_DATA_LAST_CLOCK);

    } else {

        /* 低速 */
        amhw_mm32_spi_tx_data_edge_sel(p_hw_spi, AMHW_MM32_SPI_TX_DATA_ONE_CLOCK);
        amhw_mm32_spi_rx_data_edge_sel(p_hw_spi, AMHW_MM32_SPI_RX_DATA_MID_CLOCK);
    }

    /* 设置SPI速率 */
    __spi_speed_cfg(p_this, p_trans->speed_hz);

    return AM_OK;
}

/**
 * \brief SPI 传输数据函数
 */
am_local
int __spi_msg_start (void              *p_drv,
                     am_spi_device_t   *p_dev,
                     am_spi_message_t  *p_msg)
{
    am_mm32_spi_dma_dev_t *p_this = (am_mm32_spi_dma_dev_t *)p_drv;

    int key;

    /* 设备有效性检查 */
    if ((p_drv              == NULL) ||
        (p_dev              == NULL) ||
        (p_msg              == NULL) ){
        return -AM_EINVAL;
    }

    p_msg->p_spi_dev       = p_dev; /* 设备参数信息存入到消息中 */
    p_this->p_cur_msg      = p_msg; /* 将当前设备传输消息存入 */
    p_this->nbytes_to_recv = 0;     /* 待接收字符数清0 */
    p_this->data_ptr       = 0;     /* 已接收字符数清0 */

    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_this->busy == AM_TRUE) {
        __spi_msg_in(p_this, p_msg);
        am_int_cpu_unlock(key);
        return AM_OK;
    } else {
        p_this->busy = AM_TRUE;
        __spi_msg_in(p_this, p_msg);
        p_msg->status = -AM_EISCONN; /* 正在排队中 */
        am_int_cpu_unlock(key);

        /* 启动状态机 */
        return __spi_mst_sm_event(p_this, __SPI_EVT_TRANS_LAUNCH);
    }
}

/******************************************************************************/

/*  状态机内部状态切换 */
#define __SPI_NEXT_STATE(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

/**
 * \brief SPI 使用状态机传输
 */
am_local
int __spi_mst_sm_event (am_mm32_spi_dma_dev_t *p_dev, uint32_t event)
{
    volatile uint32_t new_event = __SPI_EVT_NONE;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    while (1) {

        if (new_event != __SPI_EVT_NONE) {  /* 检查新事件是否来自内部 */
            event     = new_event;
            new_event  = __SPI_EVT_NONE;
        }

        switch (p_dev->state) {

        case __SPI_ST_IDLE:         /* 控制器处于空闲状态 */
        {
            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;  /* 空闲状态等待的消息必须是启动传输 */
            }

            /* 切换到消息开始状态，不用break */
        }
        /* no break */

        case __SPI_ST_MSG_START:    /* 消息开始 */
        {
            am_spi_message_t  *p_cur_msg   = NULL;

            int key;

            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;  /* 消息开始状态等待的消息必须是启动传输 */
            }

            key = am_int_cpu_lock();
            p_cur_msg          = __spi_msg_out(p_dev);
            p_dev->p_cur_msg   = p_cur_msg;

            if (p_cur_msg) {
                p_cur_msg->status = -AM_EINPROGRESS;
            }
            am_int_cpu_unlock(key);

            /* 无需要处理的消息 */
            if (p_cur_msg == NULL) {
                __SPI_NEXT_STATE(__SPI_ST_IDLE, __SPI_EVT_NONE);
                break;
            } else {

                p_dev->p_cur_spi_dev = p_cur_msg->p_spi_dev;

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                __SPI_NEXT_STATE(__SPI_ST_TRANS_START, __SPI_EVT_TRANS_LAUNCH);

                event     = new_event;
                new_event = __SPI_EVT_NONE;
            }
        }
        /* no break */

        case __SPI_ST_TRANS_START:  /* 传输开始 */
        {
            am_spi_message_t *p_cur_msg = p_dev->p_cur_msg;

            p_dev->p_isr_msg = p_cur_msg;

            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;  /* 传输开始状态等待的消息必须是启动传输 */
            }

            /* 当前消息传输完成 */
            if (am_list_empty(&(p_cur_msg->transfers))) {

                p_cur_msg->actual_length = 0;

                /* 消息正在处理中 */
                if (p_cur_msg->status == -AM_EINPROGRESS) {
                    p_cur_msg->status = AM_OK;
                }

                __SPI_NEXT_STATE(__SPI_ST_MSG_START, __SPI_EVT_TRANS_LAUNCH);

            } else {

                /* 获取到一个传输，正确处理该传输即可 */
                am_spi_transfer_t *p_cur_trans = __spi_trans_out(p_cur_msg);
                p_dev->p_cur_trans             = p_cur_trans;

                /* reset current data pointer */
                p_dev->data_ptr       = 0;
                p_dev->nbytes_to_recv = 0;

                /* 配置SPI传输参数 */
                __spi_config(p_dev);

                /* CS选通 */
                __spi_cs_on(p_dev, p_dev->p_cur_spi_dev);

                __SPI_NEXT_STATE(__SPI_ST_DMA_TRANS_DATA, __SPI_EVT_DMA_TRANS_DATA);

            }
            break;
        }

        case __SPI_ST_DMA_TRANS_DATA:    /* DMA发送数据 */
        {
            if (event != __SPI_EVT_DMA_TRANS_DATA) {
                return -AM_EINVAL;  /* 主机发送状态等待的消息必须是发送数据 */
            }

            /* 下一状态还是发送状态 */
            __SPI_NEXT_STATE(__SPI_ST_TRANS_START, __SPI_EVT_NONE);

            /* 使用DMA传输 */
            __spi_dma_trans(p_dev);

            break;
        }

        /*
         * 永远也不该运行到这儿
         */
        default:
            break;
        }

        /* 没有来自内部的信息, 跳出 */
        if (new_event == __SPI_EVT_NONE) {
            break;
        }
    }
    return AM_OK;
}

/******************************************************************************/

/**
 * \brief SPI 初始化
 */
am_spi_handle_t am_mm32_spi_dma_init (am_mm32_spi_dma_dev_t           *p_dev,
                                        const am_mm32_spi_dma_devinfo_t *p_devinfo)
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

    am_int_connect(p_dev->p_devinfo->inum, __spi_isr, (void *)p_dev);
    am_int_enable(p_dev->p_devinfo->inum);

    return &(p_dev->spi_serve);
}

/**
 * \brief SPI 去除初始化
 */
void am_mm32_spi_dma_deinit (am_spi_handle_t handle)
{
    am_mm32_spi_dma_dev_t *p_dev    = (am_mm32_spi_dma_dev_t *)handle;
    amhw_mm32_spi_t       *p_hw_spi = (amhw_mm32_spi_t *)(p_dev->p_devinfo->spi_reg_base);

    if (NULL == p_dev) {
        return ;
    }

    p_dev->spi_serve.p_funcs = NULL;
    p_dev->spi_serve.p_drv   = NULL;

    /* 禁能 SPI */
    amhw_mm32_spi_module_enable(p_hw_spi, AM_FALSE);

    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __spi_isr, (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/**
 * \brief SPI传输速度配置
 *
 * \param[in]  p_dev        : 指向SPI设备的指针
 * \param[in]  target_speed : 目标速度
 *
 * \retval 0 ：不支持目标速度,没有配置成功
 * \return 最终配置的速度，可能与目标速度不同
 *
 * \note 配置成功，会更新SPI设备中的cur_speed和min_diff
 */
am_local
uint32_t __spi_speed_cfg (am_mm32_spi_dma_dev_t *p_dev, uint32_t target_speed)
{

    uint32_t baud_div, best_speed;  /* 计算出的速度 */

    amhw_mm32_spi_t *p_hw_spi = (amhw_mm32_spi_t *)(p_dev->p_devinfo->spi_reg_base);

    baud_div = am_clk_rate_get(p_dev->p_devinfo->clk_id) / target_speed;

    amhw_mm32_spi_baud_rate_set(p_hw_spi, baud_div);

    best_speed = target_speed;

   return best_speed;
}

