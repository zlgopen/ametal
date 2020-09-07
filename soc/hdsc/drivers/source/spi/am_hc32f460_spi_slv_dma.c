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
 * \brief SPI 从机驱动层实现函数(DMA 方式)
 *
 * \internal
 * \par Modification History
 * -1.00 20-04-01 cds, first implementation
 * \endinternal
 */

/*******************************************************************************
includes
*******************************************************************************/

#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_hc32f460.h"
#include "am_hc32f460_dma.h"
#include "am_hc32f460_spi_slv_dma.h"
#include "hw/amhw_hc32f460_spi.h"

/*******************************************************************************
  模块内函数声明
*******************************************************************************/
extern am_hc32f460_dma_dev_t __g_dma1_dev;
extern am_hc32f460_dma_dev_t __g_dma2_dev;

/** \brief SPI 从机硬件初始化 */
static int __spi_slv_hard_init (am_hc32f460_spi_slv_dma_dev_t *p_dev);

/** \brief SPI 从机 DMA 中断处理函数 */
static void __spi_slv_dma_isr (void *p_arg, uint32_t stat);

/** \brief SPI 从机 DMA 传输处理函数 */
static int __spi_slv_transfer_data (am_hc32f460_spi_slv_dma_dev_t *p_dev);

/*******************************************************************************
  SPI 驱动函数声明
*******************************************************************************/

/** \brief 开启 SPI 从机设备  */
static int __spi_slv_setup (void *p_drv, am_spi_slv_device_t *p_dev);
/** \brief 关闭 SPI 从机设备  */
static int __spi_slv_shutdown (void *p_drv, am_spi_slv_device_t *p_dev);

/**
 * \brief SPI 从设备驱动函数定义
 */
static const struct am_spi_slv_drv_funcs __g_spi_slv_drv_funcs = {
    __spi_slv_setup,
    __spi_slv_shutdown,
};

/**
 * \brief SPI 硬件初始化
 */
static int __spi_slv_hard_init (am_hc32f460_spi_slv_dma_dev_t *p_this)
{
    amhw_hc32f460_spi_t *p_hw_spi = (amhw_hc32f460_spi_t *)
                                    (p_this->p_devinfo->spi_reg_base);

    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    amhw_hc32f460_spi_enable(p_hw_spi, AM_FALSE);

    /* 配置SPI通信模式 */
    amhw_hc32f460_spi_trans_mode_sel(p_hw_spi,
                                     AMHW_HC32F460_SPI_TRANS_MODE_FULL_DUPLEX);

    /* 配置SPI工作模式 */
    amhw_hc32f460_spi_work_mode_sel(p_hw_spi,
                                    AMHW_HC32F460_SPI_WORK_MODE_4LINE);

    /* 配置数据长度 */
    amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                      AMHW_HC32F460_SPI_DATA_LENGTH_BIT8);

    /* 配置为从机模式 */
    amhw_hc32f460_spi_mode_sel(p_hw_spi, AMHW_HC32F460_SPI_MODE_SLAVE);

    amhw_hc32f460_spi_frame_number_set(p_hw_spi, AMHW_HC32F460_SPI_FRAME_NUMBER1);

    amhw_hc32f460_spi_first_bit_position_set(p_hw_spi, AMHW_HC32F460_SPI_MSB_FIRST);
    amhw_hc32f460_spi_sck_polarity_set(p_hw_spi, AMHW_HC32F460_SPI_SCK_IDLE_LEVEL_LOW);

    /* 配置SPI模式（时钟相位和极性） */
    amhw_hc32f460_spi_clk_mode_set(p_hw_spi, 0x3 & AM_SPI_SLV_MODE_0);

    return AM_OK;
}

/**
 * \brief 从机配置
 */
static int __spi_slv_cfg (am_hc32f460_spi_slv_dma_dev_t *p_this,
                          am_spi_slv_device_t           *p_slv_dev)
{
    amhw_hc32f460_spi_t         *p_hw_spi =
            (amhw_hc32f460_spi_t *) (p_this->p_devinfo->spi_reg_base);

    /* 默认数据为 8 位，最大不超过 32 位 */
    if (p_slv_dev->bits_per_word == 0) {
        p_slv_dev->bits_per_word = 8;
    } else if (p_slv_dev->bits_per_word > 32) {
        return -AM_ENOTSUP;
    }

    /* 除8位，1~32 数据收发只支持 LSB 模式 */
    if (!(p_slv_dev->mode & AM_SPI_SLV_LSB_FIRST) &&
         (p_slv_dev->bits_per_word != 8) ) {
        return -AM_ENOTSUP;
    }

    /* 配置 LSB 或 MSB */
    if (p_slv_dev->mode & AM_SPI_SLV_LSB_FIRST) {
    	amhw_hc32f460_spi_first_bit_position_set(p_hw_spi, AMHW_HC32F460_SPI_LSB_FIRST);
    } else {
    	amhw_hc32f460_spi_first_bit_position_set(p_hw_spi, AMHW_HC32F460_SPI_MSB_FIRST);
    }

    /* 配置数据长度 */
    switch (p_slv_dev->bits_per_word) {

    case 4:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT4);
        break;
    case 5:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT5);
        break;
    case 6:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT6);
        break;
    case 7:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT7);
        break;
    case 8:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT8);
        break;
    case 9:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT9);
        break;
    case 10:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT10);
        break;
    case 11:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT11);
        break;
    case 12:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT12);
        break;
    case 13:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT13);
        break;
    case 14:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT14);
        break;
    case 15:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT15);
        break;
    case 16:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT16);
        break;
    case 20:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT20);
        break;
    case 24:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT24);
        break;
    case 32:
        amhw_hc32f460_spi_data_length_set(p_hw_spi,
                                          AMHW_HC32F460_SPI_DATA_LENGTH_BIT32);
        break;
    }

    /* 配置SPI模式（时钟相位和极性） */
    amhw_hc32f460_spi_clk_mode_set(p_hw_spi, p_slv_dev->mode);

    return AM_OK;
}

/**
 * \brief 片选中断
 */
void __cs_irq_handle (void *p_arg)
{
    am_hc32f460_spi_slv_dma_dev_t *p_this    = ( am_hc32f460_spi_slv_dma_dev_t *)p_arg;
    amhw_hc32f460_spi_t           *p_hw_spi  = (amhw_hc32f460_spi_t *) (p_this->p_devinfo->spi_reg_base);
    am_spi_slv_device_t           *p_dev     = p_this->p_spi_slv_dev;
    uint8_t                        cs_status = am_gpio_get(p_this->p_devinfo->cs_pin);
    size_t                         count     = 0;
    am_hc32f460_dma_dev_t         *p_dma_dev = &__g_dma1_dev;

    if((cs_status == 1)) { /* 传输结束 */
        count = p_this->sum_nbytes - \
             am_hc32f460_dma_tran_data_get(p_dma_dev, p_this->p_devinfo->dma_chan_rx);

         if(p_dev->p_slv_cb_funcs->pfn_cs_inactive) {
             p_dev->p_slv_cb_funcs->pfn_cs_inactive(p_dev->p_arg, count);
         }

         /* 关闭发送 DMA 传输  */
         am_hc32f460_dma_chan_stop(p_dma_dev, p_this->p_devinfo->dma_chan_tx);

         /* 关闭 SPI，同时可以清空缓存区和移位寄存器中的数据 */
         amhw_hc32f460_spi_enable(p_hw_spi, AM_FALSE);
         amhw_hc32f460_spi_enable(p_hw_spi, AM_TRUE);

         /* 模拟一个 DMA 传输完成，SPI 有 8 字节的 FIFO，没有时钟沿也会有 DMA 请求 */
         am_hc32f460_dma_xfer_desc_build(&(p_this->g_desc[0]),                /* 通道描述符 */
                                          (uint32_t)(&(p_this->dummy_txbuf)), /* 源缓冲区首地址 */
                                          (uint32_t)(&(p_hw_spi->DR)),        /* 目的缓冲区首地址 */
                                          1,                                  /* 传输字节数 */
                                          p_this->dummy_dma_flags);           /* 传输配置 */

         if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                                &(p_this->g_desc[0]),
                                                AMHW_HC32F460_DMA_MER_TO_PER,     /* 内存到外设 */
                                                p_this->p_devinfo->dma_chan_tx) == AM_OK) {
             am_hc32f460_dma_chan_start(p_dma_dev, p_this->p_devinfo->dma_chan_tx);
         }

         /* 清除该字节数 ，关闭 SPI，片选无效时不进行传输 */
         amhw_hc32f460_spi_enable(p_hw_spi, AM_FALSE);

    } else { /* 传输开始 */

        if(p_dev->p_slv_cb_funcs->pfn_cs_active) {
            p_dev->p_slv_cb_funcs->pfn_cs_active(p_dev->p_arg);
        }

        p_this->sum_nbytes = 0;

        /* 配置 DMA 之前必须关闭 DMA 通道 */
        am_hc32f460_dma_chan_stop(p_dma_dev, p_this->p_devinfo->dma_chan_tx);
        am_hc32f460_dma_chan_stop(p_dma_dev, p_this->p_devinfo->dma_chan_rx);

        /* 启动 DMA 传输 */
        __spi_slv_transfer_data(p_this);

        amhw_hc32f460_spi_enable(p_hw_spi, AM_TRUE);
    }
}

/**
 * \brief 使能 CS 引脚外部中断
 */
static int __cs_irq_cfg (am_hc32f460_spi_slv_dma_dev_t *p_this)
{
    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    /* 连接中断回调函数 */
    am_gpio_trigger_connect(p_this->p_devinfo->cs_pin, __cs_irq_handle, p_this);

    /* 配置为双边沿触发 */
    am_gpio_trigger_cfg(p_this->p_devinfo->cs_pin, AM_GPIO_TRIGGER_BOTH_EDGES);

    /* 使能引脚触发 */
    am_gpio_trigger_on(p_this->p_devinfo->cs_pin);

    return AM_OK;
}

/**
 * \brief 片选解初始化
 */
static int __cs_irq_cfg_deinit (am_hc32f460_spi_slv_dma_dev_t *p_this)
{
    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    am_gpio_trigger_disconnect(p_this->p_devinfo->cs_pin,
                               __cs_irq_handle,
                               p_this);

    am_gpio_trigger_off(p_this->p_devinfo->cs_pin);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief 设置 SPI 从设备
 */
static int __spi_slv_setup (void *p_drv, am_spi_slv_device_t *p_dev)
{
    am_hc32f460_spi_slv_dma_dev_t *p_this = (am_hc32f460_spi_slv_dma_dev_t *)p_drv;

    if (p_dev == NULL || p_drv == NULL) {
        return -AM_EINVAL;
    }

    if (p_this->p_spi_slv_dev == NULL) {
        p_this->p_spi_slv_dev = p_dev;
    } else {
        return -AM_ENOTSUP;
    }

    /* 配置从 SPI */
    if (AM_OK != __spi_slv_cfg(p_this, p_dev) ) {
        return -AM_ENOTSUP;
    }

    /* 配置片选中断 */
    __cs_irq_cfg(p_this);

    return AM_OK;
}

/**
 * \brief SPI 从机关闭函数
 */
static int __spi_slv_shutdown(void *p_drv, struct am_spi_slv_device *p_dev)
{
     am_hc32f460_spi_slv_dma_dev_t *p_this;
    
    if ( (p_drv   == NULL) ||
         (p_dev   == NULL)) {
        return -AM_EINVAL;
    }

    p_this = (am_hc32f460_spi_slv_dma_dev_t *)p_drv;

    __cs_irq_cfg_deinit(p_this);

    p_this->p_spi_slv_dev = NULL;

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief SPI 从机 DMA 中断处理
 */
static void __spi_slv_dma_isr (void *p_arg, uint32_t stat)
{
    am_hc32f460_spi_slv_dma_dev_t *p_this = (am_hc32f460_spi_slv_dma_dev_t *)p_arg;

    /* 中断发生 */
    if (stat == AM_HC32F460_DMA_INT_COMPLETE)
    {

        /* 传输完成回调 */
        __spi_slv_transfer_data(p_this);

    } else {

        /* 中断源不匹配 */
    }
}

/**
 * \brief SPI 从机发送数据
 */
static int __spi_slv_transfer_data (am_hc32f460_spi_slv_dma_dev_t *p_dev)
{
    amhw_hc32f460_spi_t *p_hw_spi;
    am_spi_slv_device_t *p_slv_dev;
    am_hc32f460_dma_dev_t *p_dma_dev = &__g_dma1_dev;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_spi = (amhw_hc32f460_spi_t *)(p_dev->p_devinfo->spi_reg_base);

    p_slv_dev = p_dev->p_spi_slv_dev;

    /* 设置字节数有效性检查 */
    if (p_dev->tansfer.bits_per_word > 32) {
        return -AM_EINVAL;
    }

    /* 传输数据回调 */
    if (p_slv_dev->p_slv_cb_funcs->pfn_tansfer_get) {
        if ( p_slv_dev->p_slv_cb_funcs->pfn_tansfer_get(p_slv_dev->p_arg,
                              &(p_dev->tansfer))  != AM_OK ) {
            return AM_OK;
        }
    } else {
        return -AM_ENODEV;
    }

    /* 不需要进行传输 */
    if (p_dev->tansfer.nbytes == 0 ) {
        return AM_OK;
    }

    /* 0 默认为从设备的值 */
    if (p_dev->tansfer.bits_per_word == 0 ) {
        p_dev->tansfer.bits_per_word = p_dev->p_spi_slv_dev->bits_per_word;
    }

    /* 判断是否有效  */
    if (!(p_slv_dev->mode & AM_SPI_SLV_LSB_FIRST) &&
           (p_slv_dev->bits_per_word != 8) ) {
        return -AM_ENOTSUP;
    }

    /* 统计字节数 */
    p_dev->sum_nbytes += p_dev->tansfer.nbytes;


    /* 建立发送通道描述符 */
    if (p_dev->tansfer.p_tx_buf) {

        am_hc32f460_dma_xfer_desc_build(&(p_dev->g_desc[0]),                 /* 通道描述符 */
                                        (uint32_t)(p_dev->tansfer.p_tx_buf), /* 源缓冲区首地址 */
                                        (uint32_t)(&(p_hw_spi->DR)),         /* 目的缓冲区首地址 */
                                         p_dev->tansfer.nbytes ,             /* 传输字节数 */
                                         p_dev->dma_tx_flags);               /* 传输配置 */

    } else {

        am_hc32f460_dma_xfer_desc_build(&(p_dev->g_desc[0]),                /* 通道描述符 */
                                        (uint32_t)(&(p_dev->dummy_txbuf)),  /* 源缓冲区首地址 */
                                        (uint32_t)(&(p_hw_spi->DR)),        /* 目的缓冲区首地址 */
                                        p_dev->tansfer.nbytes,              /* 传输字节数 */
                                        p_dev->dummy_dma_flags);            /* 传输配置 */
    }

    /* 建立接收通道描述符 */
    if ( p_dev->tansfer.p_rx_buf) {
        am_hc32f460_dma_xfer_desc_build(&(p_dev->g_desc[1]),                 /* 通道描述符 */
                                        (uint32_t)(&(p_hw_spi->DR)),         /* 源缓冲区首地址 */
                                        (uint32_t)(p_dev->tansfer.p_rx_buf), /* 目的缓冲区首地址 */
                                         p_dev->tansfer.nbytes,              /* 传输字节数 */
                                         p_dev->dma_rx_flags);               /* 传输配置 */

    } else {
        am_hc32f460_dma_xfer_desc_build(&(p_dev->g_desc[1]),                /* 通道描述符 */
                                        (uint32_t)(&(p_hw_spi->DR)),        /* 源缓冲区首地址 */
                                        (uint32_t)(&(p_dev->dummy_rxbuf)),  /* 目的缓冲区首地址 */
                                        p_dev->tansfer.nbytes ,             /* 传输字节数 */
                                        p_dev->dummy_dma_flags);            /* 传输配置 */

    }

    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                           &(p_dev->g_desc[1]),
                                           AMHW_HC32F460_DMA_PER_TO_MER,         /* 外设到内存 */
                                           p_dev->p_devinfo->dma_chan_rx) == AM_ERROR) {
        return AM_ERROR;
    }

    if (am_hc32f460_dma_xfer_desc_chan_cfg(p_dma_dev,
                                           &(p_dev->g_desc[0]),
                                           AMHW_HC32F460_DMA_MER_TO_PER,        /* 内存到外设 */
                                           p_dev->p_devinfo->dma_chan_tx) == AM_ERROR) {
        return AM_ERROR;
    }

    am_hc32f460_dma_block_data_size(p_dma_dev, p_dev->p_devinfo->dma_chan_rx, 1);
    am_hc32f460_dma_block_data_size(p_dma_dev, p_dev->p_devinfo->dma_chan_tx, 1);


    /* 设置DMA触发源*/
    if((p_dev->p_devinfo->dev_id) == 1) {
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_rx, EVT_SPI1_SRRI);
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_tx, EVT_SPI1_SRTI);
    } else if((p_dev->p_devinfo->dev_id) == 2) {
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_rx, EVT_SPI2_SRRI);
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_tx, EVT_SPI2_SRTI);
    } else if((p_dev->p_devinfo->dev_id) == 3) {
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_rx, EVT_SPI3_SRRI);
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_tx, EVT_SPI3_SRTI);
    } else if((p_dev->p_devinfo->dev_id) == 4) {
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_rx, EVT_SPI4_SRRI);
        am_hc32f460_dma_chan_src_set(p_dma_dev, p_dev->p_devinfo->dma_chan_tx, EVT_SPI4_SRTI);
    }

    am_hc32f460_dma_chan_start(p_dma_dev, p_dev->p_devinfo->dma_chan_rx);
    am_hc32f460_dma_chan_start(p_dma_dev, p_dev->p_devinfo->dma_chan_tx);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief SPI 初始化
 *
 */
am_spi_slv_handle_t
am_hc32f460_spi_slv_dma_init (am_hc32f460_spi_slv_dma_dev_t           *p_dev,
                              const am_hc32f460_spi_slv_dma_devinfo_t *p_devinfo)
{
    if (NULL == p_devinfo || NULL == p_dev ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->spi_slv_serve.p_funcs = (struct am_spi_slv_drv_funcs *)&__g_spi_slv_drv_funcs;
    p_dev->spi_slv_serve.p_drv   = p_dev;

    if (p_devinfo->dma_dev_id == 1) {
        p_dev->p_dma_dev = &__g_dma1_dev;
    } else if (p_devinfo->dma_dev_id == 2) {
        p_dev->p_dma_dev = &__g_dma2_dev;
    }

    p_dev->p_devinfo = p_devinfo;

    p_dev->p_spi_slv_dev    = NULL;

    p_dev->tansfer.nbytes        = 0;
    p_dev->tansfer.bits_per_word = 0;
    p_dev->tansfer.p_rx_buf      = NULL;
    p_dev->tansfer.p_tx_buf      = NULL;
    p_dev->sum_nbytes      = 0;

    p_dev->dummy_txbuf      = 0;

    p_dev->dma_tx_flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
                          AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_SRC_ADD_INCREASING        |  /* 源地址自增 */
                          AMHW_HC32F460_DMA_CHAN_DST_ADD_FIXED;               /* 目标地址固定 */

    p_dev->dma_rx_flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
                          AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
                          AMHW_HC32F460_DMA_CHAN_SRC_ADD_FIXED             |  /* 源地址固定 */
                          AMHW_HC32F460_DMA_CHAN_DST_ADD_INCREASING;          /* 目标地址自增 */

    p_dev->dummy_dma_flags = AMHW_HC32F460_DMA_CHAN_CFG_INT_ENABLE            |  /* 通道中断使能 */
                             AMHW_HC32F460_DMA_CHAN_CFG_SIZE_8BIT             |  /* 数据宽度 1 字节 */
                             AMHW_HC32F460_DMA_CHAN_CFG_LLP_DISABLE           |  /* 连锁传输禁能 */
                             AMHW_HC32F460_DMA_CHAN_CFG_DSTADD_NOTSEQ_DISABLE |  /* 目标地址不连续传输禁能 */
                             AMHW_HC32F460_DMA_CHAN_CFG_SRCADD_NOTSEQ_DISABLE |  /* 源地址不连续传输禁能 */
                             AMHW_HC32F460_DMA_CHAN_CFG_DST_DRPT_DISABLE      |  /* 目标重复传输禁能 */
                             AMHW_HC32F460_DMA_CHAN_CFG_SRC_DRPT_DISABLE      |  /* 源重复传输禁能 */
                             AMHW_HC32F460_DMA_CHAN_SRC_ADD_FIXED             |  /* 源地址递增 */
                             AMHW_HC32F460_DMA_CHAN_DST_ADD_FIXED;               /* 目标地址固定 */


    /* 初始化硬件 */
    if (__spi_slv_hard_init(p_dev) != AM_OK) {
        return NULL;
    }

    /* 连接 DMA 中断服务函数 */
    am_hc32f460_dma_isr_connect(p_dev->p_dma_dev,
                                p_dev->p_devinfo->dma_chan_rx,
                                __spi_slv_dma_isr,
                                (void *)p_dev);

    return &(p_dev->spi_slv_serve);
}

/**
 * \brief SPI 去除初始化
 */
void am_hc32f460_spi_slv_dma_deinit (am_spi_slv_handle_t handle)
{
    am_hc32f460_spi_slv_dma_dev_t *p_dev     = (am_hc32f460_spi_slv_dma_dev_t *)handle;
    amhw_hc32f460_spi_t           *p_hw_spi  = (amhw_hc32f460_spi_t *)(p_dev->p_devinfo->spi_reg_base);
    am_hc32f460_dma_dev_t         *p_dma_dev = (am_hc32f460_dma_dev_t *)&__g_dma1_dev;

    if (NULL == p_dev) {
        return ;
    }

    p_dev->spi_slv_serve.p_funcs = NULL;
    p_dev->spi_slv_serve.p_drv   = NULL;

    /* 禁能 SPI */
    amhw_hc32f460_spi_enable(p_hw_spi, AM_FALSE);

    am_hc32f460_dma_isr_disconnect(p_dma_dev,
                                   p_dev->p_devinfo->dma_chan_rx,
                                   __spi_slv_dma_isr,
                                   (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
