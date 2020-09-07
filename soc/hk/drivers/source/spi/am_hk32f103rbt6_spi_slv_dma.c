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
 * -1.00 19-07-18 fra, first implementation
 * \endinternal
 */

/*******************************************************************************
includes
*******************************************************************************/

#include "ametal.h"
#include "am_int.h"
#include "am_clk.h"
#include "am_gpio.h"
#include "am_hk32f103rbt6_dma.h"
#include "am_hk32f103rbt6_spi_slv_dma.h"
#include "hw/amhw_hk32f103rbt6_spi.h"

/*******************************************************************************
  模块内函数声明
*******************************************************************************/

/** \brief SPI 从机硬件初始化 */
static int __spi_slv_hard_init (am_hk32f103rbt6_spi_slv_dma_dev_t *p_dev);

/** \brief SPI 从机 DMA 中断处理函数 */
static void __spi_slv_dma_isr (void *p_arg, uint32_t stat);

/** \brief SPI 从机 DMA 传输处理函数 */
static int __spi_slv_transfer_data (am_hk32f103rbt6_spi_slv_dma_dev_t *p_dev);

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
static int __spi_slv_hard_init (am_hk32f103rbt6_spi_slv_dma_dev_t *p_this)
{
    amhw_hk32f103rbt6_spi_t *p_hw_spi = (amhw_hk32f103rbt6_spi_t *)
                                  (p_this->p_devinfo->spi_reg_base);

    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    amhw_hk32f103rbt6_spi_disable(p_hw_spi);

    /* 配置SPI模式（时钟相位和极性） */
    amhw_hk32f103rbt6_spi_clk_mode_set(p_hw_spi, 0x3 & p_this->p_spi_slv_dev->mode);

    /* 设置帧格式，先发送MSB */
    amhw_hk32f103rbt6_spi_lsbfirst_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_LSB_FIRST_SEND_MSB);

    /* 设置数据长度 */
    amhw_hk32f103rbt6_spi_data_length_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_DATA_8BIT);

    /* 配置为从机模式 */
    amhw_hk32f103rbt6_spi_master_salver_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_SLAVER);

    /* 使能为 DMA 方式 */
    amhw_hk32f103rbt6_spi_dma_tx_enable(p_hw_spi);
    amhw_hk32f103rbt6_spi_dma_rx_enable(p_hw_spi);

    /* 使能SPI */
    amhw_hk32f103rbt6_spi_enable(p_hw_spi);

    return AM_OK;
}

/**
 * \brief 从机配置
 */
static int __spi_slv_cfg (am_hk32f103rbt6_spi_slv_dma_dev_t *p_this,
                          am_spi_slv_device_t         *p_slv_dev)
{
    amhw_hk32f103rbt6_spi_t *p_hw_spi = (amhw_hk32f103rbt6_spi_t *)
                                  (p_this->p_devinfo->spi_reg_base);


    amhw_hk32f103rbt6_spi_disable(p_hw_spi);

    /* 默认数据为 8 位，最大不超过 32 位 */
    if (p_slv_dev->bits_per_word == 0) {
        p_slv_dev->bits_per_word = 8;
    } else if (p_slv_dev->bits_per_word > 32) {
        return -AM_ENOTSUP;
    }

    /* 配置SPI数据长度和帧格式 */
    if (p_slv_dev->bits_per_word == 8) {
        amhw_hk32f103rbt6_spi_data_length_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_DATA_8BIT);
    }
    else if (p_slv_dev->bits_per_word == 16) {
        amhw_hk32f103rbt6_spi_data_length_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_DATA_16BIT);
    }
    else if ((p_slv_dev->bits_per_word >= 1) ||
             (p_slv_dev->bits_per_word <= 32)) {
        amhw_hk32f103rbt6_spi_flex_length_set(p_hw_spi,p_slv_dev->bits_per_word);
        amhw_hk32f103rbt6_spi_flexlength_enable(p_hw_spi);
    }
    else {
         amhw_hk32f103rbt6_spi_data_length_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_DATA_8BIT);
    }

    /* 配置SPI模式（时钟相位和极性） */
    amhw_hk32f103rbt6_spi_clk_mode_set(p_hw_spi, 0x3 & p_slv_dev->mode);

    /* 设置帧格式，先发送MSB */
    amhw_hk32f103rbt6_spi_lsbfirst_set(p_hw_spi, AMHW_HK32F103RBT6_SPI_LSB_FIRST_SEND_MSB);

    return AM_OK;
}

/**
 * \brief 片选中断
 */
void __cs_irq_handle (void *p_arg)
{

    am_hk32f103rbt6_spi_slv_dma_dev_t *p_this     = ( am_hk32f103rbt6_spi_slv_dma_dev_t *)
                                              p_arg;
    amhw_hk32f103rbt6_spi_t           *p_hw_spi   = (amhw_hk32f103rbt6_spi_t *)
                                              (p_this->p_devinfo->spi_reg_base);
    am_spi_slv_device_t         *p_dev      = p_this->p_spi_slv_dev;
    uint8_t                      cs_status  = am_gpio_get(
                                                  p_this->p_devinfo->cs_pin);
    size_t                       count      = 0;
    static uint8_t               start_flag = 0;

    /* 本次边沿触发后，检测到的CS电平为高，且开始标志有效的情况下，才进行传输结束的操作*/
    if( (cs_status == 1) && (start_flag == 1)) { /* 传输结束 */

        start_flag = 0;

        count      = p_this->sum_nbytes -
                     am_hk32f103rbt6_dma_tran_data_get(p_this->p_devinfo->dma_chan_rx);


         amhw_hk32f103rbt6_spi_disable(p_hw_spi);

        if(p_dev->p_slv_cb_funcs->pfn_cs_inactive) {
            p_dev->p_slv_cb_funcs->pfn_cs_inactive(p_dev->p_arg, count);
        }

        /* 关闭 SPI，同时可以清空缓存区和移位寄存器中的数据 */

        /* 关闭发送 DMA 传输  */
        am_hk32f103rbt6_dma_chan_stop( p_this->p_devinfo->dma_chan_tx);

        /* 关闭 SPI，同时可以清空缓存区和移位寄存器中的数据 */
        amhw_hk32f103rbt6_spi_disable(p_hw_spi);

        amhw_hk32f103rbt6_spi_enable(p_hw_spi);

        /* 模拟一个 DMA 传输完成，SPI 有 8 字节的 FIFO，没有时钟沿也会有 DMA 请求 */
        am_hk32f103rbt6_dma_xfer_desc_build(&(p_this->g_desc[0]),        /* 通道描述符 */
                                    (uint32_t)(&(p_this->dummy_txbuf)),
                                    (uint32_t)(&(p_hw_spi->dr)),/* 目的缓冲区首地址 */
                                     1,                         /* 传输字节数 */
                                     p_this->dummy_dma_flags);  /* 传输配置 */

        if (am_hk32f103rbt6_dma_xfer_desc_chan_cfg(&(p_this->g_desc[0]),
                                          AMHW_HK32F103RBT6_DMA_MER_TO_PER,  /* 内存到外设 */
                                          p_this->p_devinfo->dma_chan_tx) ==
                                          AM_OK) {
            am_hk32f103rbt6_dma_chan_start(p_this->p_devinfo->dma_chan_tx);
        }

        /* 清除该字节数 ，关闭 SPI，片选无效时不进行传输 */
        amhw_hk32f103rbt6_spi_disable(p_hw_spi);

    } else { /* 传输开始 */

        start_flag = 1;

        if(p_dev->p_slv_cb_funcs->pfn_cs_active) {
            p_dev->p_slv_cb_funcs->pfn_cs_active(p_dev->p_arg);
        }

        p_this->sum_nbytes = 0;

        /* 配置 DMA 之前必须关闭 DMA 通道 */
        am_hk32f103rbt6_dma_chan_stop(p_this->p_devinfo->dma_chan_tx);
        am_hk32f103rbt6_dma_chan_stop(p_this->p_devinfo->dma_chan_rx);

        /* 启动 DMA 传输 */
        __spi_slv_transfer_data(p_this);

        amhw_hk32f103rbt6_spi_enable(p_hw_spi);
    }
}

/**
 * \brief 使能 CS 引脚外部中断
 */
static int __cs_irq_cfg (am_hk32f103rbt6_spi_slv_dma_dev_t *p_this)
{
    if (p_this == NULL) {
        return -AM_EINVAL;
    }

    /* 连接终端回调函数 */
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
static int __cs_irq_cfg_deinit (am_hk32f103rbt6_spi_slv_dma_dev_t *p_this)
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
    am_hk32f103rbt6_spi_slv_dma_dev_t *p_this = (am_hk32f103rbt6_spi_slv_dma_dev_t *)p_drv;

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
     am_hk32f103rbt6_spi_slv_dma_dev_t *p_this;
    
    if ( (p_drv   == NULL) ||
         (p_dev   == NULL)) {
        return -AM_EINVAL;
    }

    p_this = (am_hk32f103rbt6_spi_slv_dma_dev_t *)p_drv;

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
    am_hk32f103rbt6_spi_slv_dma_dev_t *p_this = (am_hk32f103rbt6_spi_slv_dma_dev_t *)p_arg;

    /* 中断发生 */
    if (stat == AM_HK32F103RBT6_DMA_INT_NORMAL)
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
static int __spi_slv_transfer_data (am_hk32f103rbt6_spi_slv_dma_dev_t *p_dev)
{
    amhw_hk32f103rbt6_spi_t      *p_hw_spi;
    am_spi_slv_device_t *p_slv_dev;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_spi = (amhw_hk32f103rbt6_spi_t *)(p_dev->p_devinfo->spi_reg_base);

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

        am_hk32f103rbt6_dma_xfer_desc_build(&(p_dev->g_desc[0]),                  /* 通道描述符 */
                                    (uint32_t)(p_dev->tansfer.p_tx_buf), /* 源缓冲区首地址 */
                                    (uint32_t)(&(p_hw_spi->dr)),         /* 目的缓冲区首地址 */
                                     p_dev->tansfer.nbytes ,             /* 传输字节数 */
                                     p_dev->dma_flags);                  /* 传输配置 */

    } else {

        am_hk32f103rbt6_dma_xfer_desc_build(&(p_dev->g_desc[0]),                  /* 通道描述符 */
                                    (uint32_t)(&(p_dev->dummy_txbuf)),   /* 源缓冲区首地址 */
                                    (uint32_t)(&(p_hw_spi->dr)),         /* 目的缓冲区首地址 */
                                     p_dev->tansfer.nbytes,              /* 传输字节数 */
                                     p_dev->dummy_dma_flags);            /* 传输配置 */
    }

    /* 建立接收通道描述符 */
    if ( p_dev->tansfer.p_rx_buf) {
        am_hk32f103rbt6_dma_xfer_desc_build(&(p_dev->g_desc[1]),                  /* 通道描述符 */
                                    (uint32_t)(&(p_hw_spi->dr)),         /* 源缓冲区首地址 */
                                    (uint32_t)(p_dev->tansfer.p_rx_buf), /* 目的缓冲区首地址 */
                                     p_dev->tansfer.nbytes,              /* 传输字节数 */
                                     p_dev->dma_flags);                  /* 传输配置 */

    } else {
        am_hk32f103rbt6_dma_xfer_desc_build(&(p_dev->g_desc[1]),                  /* 通道描述符 */
                                    (uint32_t)(&(p_hw_spi->dr)),         /* 源缓冲区首地址 */
                                    (uint32_t)(&(p_dev->dummy_rxbuf)),   /* 目的缓冲区首地址 */
                                     p_dev->tansfer.nbytes ,             /* 传输字节数 */
                                     p_dev->dummy_dma_flags);            /* 传输配置 */

    }

    if (am_hk32f103rbt6_dma_xfer_desc_chan_cfg(&(p_dev->g_desc[1]),
                                        AMHW_HK32F103RBT6_DMA_PER_TO_MER,         /* 外设到内存 */
                                        p_dev->p_devinfo->dma_chan_rx) == AM_ERROR) {
        return AM_ERROR;
    }

    if (am_hk32f103rbt6_dma_xfer_desc_chan_cfg(&(p_dev->g_desc[0]),
                                      AMHW_HK32F103RBT6_DMA_MER_TO_PER,        /* 内存到外设 */
                                      p_dev->p_devinfo->dma_chan_tx) == AM_ERROR) {
        return AM_ERROR;
    }

    am_hk32f103rbt6_dma_chan_start(p_dev->p_devinfo->dma_chan_rx);
    am_hk32f103rbt6_dma_chan_start(p_dev->p_devinfo->dma_chan_tx);

    return AM_OK;
}

/******************************************************************************/

/**
 * \brief SPI 初始化
 */
am_spi_slv_handle_t
am_hk32f103rbt6_spi_slv_dma_init (am_hk32f103rbt6_spi_slv_dma_dev_t           *p_dev,
                            const am_hk32f103rbt6_spi_slv_dma_devinfo_t *p_devinfo)
{
    if (NULL == p_devinfo || NULL == p_dev ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->spi_slv_serve.p_funcs = (struct am_spi_slv_drv_funcs *)&__g_spi_slv_drv_funcs;
    p_dev->spi_slv_serve.p_drv   = p_dev;

    p_dev->p_devinfo             = p_devinfo;

    p_dev->p_spi_slv_dev         = NULL;

    p_dev->tansfer.nbytes        = 0;
    p_dev->tansfer.bits_per_word = 0;
    p_dev->tansfer.p_rx_buf      = NULL;
    p_dev->tansfer.p_tx_buf      = NULL;
    p_dev->sum_nbytes            = 0;

    p_dev->dummy_txbuf           = 0;

    p_dev->dma_flags       = AMHW_HK32F103RBT6_DMA_CHAN_PRIORITY_HIGH       |  /* 中断优先级高 */
                             AMHW_HK32F103RBT6_DMA_CHAN_MEM_SIZE_8BIT       |  /* 内存数据宽度 1 字节 */
                             AMHW_HK32F103RBT6_DMA_CHAN_PER_SIZE_8BIT       |  /* 外设数据宽度 1 字节 */
                             AMHW_HK32F103RBT6_DMA_CHAN_MEM_ADD_INC_ENABLE  |  /* 内存地址自增 */
                             AMHW_HK32F103RBT6_DMA_CHAN_PER_ADD_INC_DISABLE |  /* 外设地址不自增 */
                             AMHW_HK32F103RBT6_DMA_CHAN_CIRCULAR_MODE_DISABLE; /* 关闭循环模式 */


    p_dev->dummy_dma_flags = AMHW_HK32F103RBT6_DMA_CHAN_PRIORITY_HIGH       |  /* 中断优先级高 */
                             AMHW_HK32F103RBT6_DMA_CHAN_MEM_SIZE_8BIT       |  /* 内存数据宽度 1 字节 */
                             AMHW_HK32F103RBT6_DMA_CHAN_PER_SIZE_8BIT       |  /* 外设数据宽度 1 字节 */
                             AMHW_HK32F103RBT6_DMA_CHAN_MEM_ADD_INC_DISABLE |  /* 内存地址不自增 */
                             AMHW_HK32F103RBT6_DMA_CHAN_PER_ADD_INC_DISABLE |  /* 外设地址不自增 */
                             AMHW_HK32F103RBT6_DMA_CHAN_CIRCULAR_MODE_DISABLE; /* 关闭循环模式 */
    /* 初始化硬件 */
    if (__spi_slv_hard_init(p_dev) != AM_OK) {
        return NULL;
    }

    /* 连接 DMA 中断服务函数 */
    am_hk32f103rbt6_dma_isr_connect(p_dev->p_devinfo->dma_chan_rx,
                           __spi_slv_dma_isr,
                           (void *)p_dev);

    return &(p_dev->spi_slv_serve);
}

/**
 * \brief SPI 去除初始化
 */
void am_hk32f103rbt6_spi_slv_dma_deinit (am_spi_slv_handle_t handle)
{
    am_hk32f103rbt6_spi_slv_dma_dev_t *p_dev    = (am_hk32f103rbt6_spi_slv_dma_dev_t *)handle;
    amhw_hk32f103rbt6_spi_t           *p_hw_spi = (amhw_hk32f103rbt6_spi_t *)(p_dev->p_devinfo->spi_reg_base);

    if (NULL == p_dev) {
        return ;
    }

    p_dev->spi_slv_serve.p_funcs = NULL;
    p_dev->spi_slv_serve.p_drv   = NULL;

    /* 禁能 SPI */
    amhw_hk32f103rbt6_spi_disable(p_hw_spi);

    am_hk32f103rbt6_dma_isr_disconnect(p_dev->p_devinfo->dma_chan_rx,
                              __spi_slv_dma_isr,
                              (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
