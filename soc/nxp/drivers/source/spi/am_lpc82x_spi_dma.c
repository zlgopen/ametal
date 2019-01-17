/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief SPI 驱动层实现函数
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-07  tee, first implementation.
 * \endinternal
 */

/*******************************************************************************
includes
*******************************************************************************/

#include <am_lpc82x.h>
#include "am_clk.h"
#include "am_gpio.h"
#include "am_lpc82x_dma.h"
#include "am_lpc82x_spi_dma.h"
#include "hw/amhw_lpc_spi.h"

/*******************************************************************************
  SPI 状态和事件定义
*******************************************************************************/

/**
 * SPI 控制器状态
 */

#define __SPI_ST_IDLE               0                   /* 空闲状态 */
#define __SPI_ST_MSG_START          1                   /* 消息开始 */
#define __SPI_ST_TRANS_START        2                   /* 传输开始 */
#define __SPI_ST_DMA_TRANS_DATA     3                   /* DMA 传输 */

/**
 * SPI 控制器事件
 *
 * 共32位，低16位是事件编号，高16位是事件参数
 */

#define __SPI_EVT_NUM_GET(event)    ((event) & 0xFFFF)
#define __SPI_EVT_PAR_GET(event)    ((event >> 16) & 0xFFFF)
#define __SPI_EVT(evt_num, evt_par) (((evt_num) & 0xFFFF) | ((evt_par) << 16))

#define __SPI_EVT_NONE              __SPI_EVT(0, 0)     /* 无事件 */
#define __SPI_EVT_TRANS_LAUNCH      __SPI_EVT(1, 0)     /* 传输就绪 */
#define __SPI_EVT_DMA_TRANS_DATA    __SPI_EVT(2, 0)     /* DMA传输数据 */

/*******************************************************************************
  模块内变量声明
*******************************************************************************/

static const uint16_t  __const_high = 0xFFFF;     /* SPI接收时MOSI为高电平 */
static const uint16_t  __const_low  = 0x0000;     /* SPI接收时MOSI为低电平 */


/*******************************************************************************
  模块内函数声明
*******************************************************************************/
am_local void __spi_default_cs_ha    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_la    (am_spi_device_t *p_dev, int state);
am_local void __spi_default_cs_dummy (am_spi_device_t *p_dev, int state);

am_local void __spi_cs_on  (am_lpc82x_spi_dma_dev_t *p_this, am_spi_device_t *p_dev);
am_local void __spi_cs_off (am_lpc82x_spi_dma_dev_t *p_this, am_spi_device_t *p_dev);

am_local int  __spi_hard_init (am_lpc82x_spi_dma_dev_t *p_this);
am_local int  __spi_config (am_lpc82x_spi_dma_dev_t *p_this);

am_local int  __spi_dma_trans (am_lpc82x_spi_dma_dev_t *p_dev);
am_local void __spi_dma_isr (void *p_arg, int stat);

am_local int  __spi_mst_sm_event (am_lpc82x_spi_dma_dev_t *p_dev, uint32_t event);
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
void __spi_cs_on (am_lpc82x_spi_dma_dev_t *p_this, am_spi_device_t *p_dev)
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
void __spi_cs_off (am_lpc82x_spi_dma_dev_t   *p_this,
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
int __spi_hard_init (am_lpc82x_spi_dma_dev_t *p_this)
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

/******************************************************************************/
am_local
int __spi_setup (void *p_arg, am_spi_device_t *p_dev)
{
    am_lpc82x_spi_dma_dev_t *p_this   = (am_lpc82x_spi_dma_dev_t *)p_arg;

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


am_local
int __spi_info_get (void *p_arg, am_spi_info_t *p_info)
{
    am_lpc82x_spi_dma_dev_t  *p_this   = (am_lpc82x_spi_dma_dev_t *)p_arg;

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


am_local
int __spi_config (am_lpc82x_spi_dma_dev_t *p_this)
{
    amhw_lpc_spi_t  *p_hw_spi  = (amhw_lpc_spi_t *)(p_this->p_devinfo->spi_regbase);
    am_spi_transfer_t  *p_trans   = p_this->p_cur_trans;

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
    if (p_trans->bits_per_word > 16 || p_trans->bits_per_word < 1) {
        return -AM_EINVAL;
    }

    /* 设置分频值有效性检查 */
    if (p_trans->speed_hz >  am_clk_rate_get(p_this->p_devinfo->clk_id) ||
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

    /* 设置模式(可在这里设置主从机模式) */
    amhw_lpc_spi_cfg_set(p_hw_spi,
                     (mode_flag              |
                         AMHW_LPC_SPI_CFG_ENABLE |
                         AMHW_LPC_SPI_CFG_MASTER));

    /* 设置单帧数据位数 */
    amhw_lpc_spi_data_flen_set(p_hw_spi, p_trans->bits_per_word);

    /* 设置分频值，即SPI速率 */
    div_val = (am_clk_rate_get(p_this->p_devinfo->clk_id) / p_trans->speed_hz);
    amhw_lpc_spi_div_set(p_hw_spi, div_val);

    /* 清除状态位 */
  amhw_lpc_spi_stat_clear(p_hw_spi,
                      AMHW_LPC_SPI_STAT_RXOVERRUN   |
              AMHW_LPC_SPI_STAT_TXUNDERRUN  |
              AMHW_LPC_SPI_STAT_SELNASSERT  |
              AMHW_LPC_SPI_STAT_SELNDEASSERT);

  /* 使能SPI接收发送溢出中断,在从机模式下才会起作用  */
  amhw_lpc_spi_int_enable(p_hw_spi,
                AMHW_LPC_SPI_INTENSET_RXOVERRUN |
                AMHW_LPC_SPI_INTENSET_TXUNDERRUN);

    return AM_OK;
}



am_local
void __spi_dma_isr (void *p_arg, int stat)
{
    am_lpc82x_spi_dma_dev_t  *p_this   = (am_lpc82x_spi_dma_dev_t *)p_arg;
    amhw_lpc_spi_t           *p_hw_spi = (amhw_lpc_spi_t *)(p_this->p_devinfo->spi_regbase);

    /* 如果是中断A发生 */
    if (stat == AM_LPC82X_DMA_STAT_INTA) {

        if (p_this->dma_trans_flg == 1) {
            p_this->dma_trans_num -= 1024;
            p_this->dma_trans_count++;

            /* 接着传输当前trans */
            __spi_mst_sm_event(p_this, __SPI_EVT_DMA_TRANS_DATA);

        /* 表明当前传输已经小于1024个长度，这一次中断产生时，当前的Tran肯定传输完成 */
        } else {

            /* 记录成功传送字节数 */
            if (p_this->p_cur_trans->p_txbuf != NULL) {
                p_this->p_cur_msg->actual_length += (p_this->p_cur_trans->nbytes)
                                *(p_this->p_cur_spi_dev->bits_per_word < 9 ? 1 : 2);
            }

            /* 下一个trans传输就绪 */
            __spi_mst_sm_event(p_this, __SPI_EVT_TRANS_LAUNCH);

            /* 等待SPI数据彻底发送结束 */
             while(!(amhw_lpc_spi_stat_get(p_hw_spi) & AMHW_LPC_SPI_STAT_IDLE));
        }

    } else {
        /* 中断源不匹配 */
    }
}

/**
 * \brief 添加一条 message 到控制器传输列表末尾
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
void __spi_msg_in (am_lpc82x_spi_dma_dev_t *p_dev, struct am_spi_message *p_msg)
{
    am_list_add_tail((struct am_list_head *)(&p_msg->ctlrdata),
                                             &(p_dev->msg_list));
}

/**
 * \brief 从控制器传输列表表头取出一条 message
 * \attention 调用此函数必须锁定控制器
 */
am_static_inline
struct am_spi_message *__spi_msg_out (am_lpc82x_spi_dma_dev_t *p_dev)
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
    am_lpc82x_spi_dma_dev_t *p_this   = (am_lpc82x_spi_dma_dev_t *)p_drv;
    amhw_lpc_spi_t   *p_hw_spi = (amhw_lpc_spi_t *)(p_this->p_devinfo->spi_regbase);

    int key;

    p_this->p_cur_spi_dev  = p_dev;                         /* 将当前设备参数信息存入 */
    p_this->p_cur_msg      = p_msg;                         /* 将当前设备传输消息存入 */

    /* 设备有效性检查 */
    if ((p_drv              == NULL) ||
        (p_dev              == NULL) ||
        (p_msg              == NULL) ){
        return -AM_EINVAL;
    }

    /* 清除传输完成结束位 */
    amhw_lpc_spi_txctl_clear(p_hw_spi, AMHW_LPC_SPI_TXDATCTL_EOT);

    key = am_int_cpu_lock();

    /* 当前正在处理消息，只需要将新的消息加入链表即可 */
    if (p_this->busy) {
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


/**
 * \brief  SPI 使用DMA传输
 */
am_local
int __spi_dma_trans (am_lpc82x_spi_dma_dev_t *p_dev)
{
    am_lpc82x_spi_dma_dev_t             *p_this      = (am_lpc82x_spi_dma_dev_t *)p_dev;
    const am_lpc82x_spi_dma_devinfo_t   *p_devinfo   = p_this->p_devinfo;
    amhw_lpc_spi_t                      *p_hw_spi    = (amhw_lpc_spi_t *)(p_devinfo->spi_regbase);
    am_spi_transfer_t                   *p_cur_trans = p_dev->p_cur_trans;
    am_lpc82x_dma_controller_t          *p_ctr_tx    = p_this->p_ctr_tx;
    am_lpc82x_dma_controller_t          *p_ctr_rx    = p_this->p_ctr_rx;

    const uint16_t                      *p_tx_buf    = NULL; /* 指向用户发送缓冲区指针 */
          uint16_t                      *p_rx_buf    = NULL; /* 指向用户接收缓冲区指针 */

          p_this->rx_trash      = 0;                         /* 丢弃数据的临时接收变量 */

          p_this->p_tx_buf_eot = 0;                          /* 一帧中的最后一个数据 */



    uint8_t    eot_nbyte    = 0;    /* 发送结束字的数据字节数 */
    uint32_t   dma_flags[3] = {0};  /* DMA发送通道描述符 */

    /* 只接收不发送数据 */
    if (p_cur_trans->p_txbuf == NULL) {
        if (p_cur_trans->flags & AM_SPI_READ_MOSI_HIGH) {
            p_tx_buf = &__const_high;                       /* MOSI接收时高电平 */
        } else {
            p_tx_buf = &__const_low;                        /* MOSI接收时低电平 */
        }
        p_this->p_tx_buf_eot = AM_SBF(p_cur_trans->bits_per_word - 1, 24) |
                       *((uint16_t *)p_tx_buf);

    /* 存在发送数据 */
    } else {
        p_tx_buf = (const uint16_t *)p_cur_trans->p_txbuf; /* MOSI发送用户数据 */
        if (p_cur_trans->bits_per_word > 8) {

            /*
             * 这个是用于设置SPI的数据控制寄存器TXDATCTL，低16位是设置发送数据的缓冲区，
             * 高24 ~ 27位是设置传输的数据宽度, 第20位是设置传输的结束(仅用于最后一次传输后结束SPI并释放片选信号)
             *
             */
            p_this->p_tx_buf_eot = AM_SBF(p_cur_trans->bits_per_word - 1, 24) |
                           ((uint16_t *)p_tx_buf)[p_cur_trans->nbytes/2 - 1];
        } else {
            p_this->p_tx_buf_eot = AM_SBF(p_cur_trans->bits_per_word - 1, 24) |
                           ((uint8_t *)p_tx_buf)[p_cur_trans->nbytes - 1];
        }
    }

    /* 配置帧结束的最后一个字符是否结束片选信号（硬件片选） */
    p_this->p_tx_buf_eot |= AMHW_LPC_SPI_TXDATCTL_EOT;

    /* 只发送不接收数据 */
    if (p_cur_trans->p_rxbuf == NULL) {
        p_rx_buf = &p_this->rx_trash ;                           /* 指向变量，丢弃数据 */

      /* 存在接收数据 */
    } else {
       p_rx_buf = p_cur_trans->p_rxbuf;                 /* 指向用户接收缓冲区 */
    }

    /* DMA发送通道配置 */
    dma_flags[0] = AM_LPC82X_DMA_XFER_VALID        |    /* 当前通道描述符有效 */
                   AM_LPC82X_DMA_XFER_RELOAD       |    /* 允许重载通道描述符 */
                   AM_LPC82X_DMA_XFER_SWTRIG       |    /* 软件触发 */
                   AM_LPC82X_DMA_XFER_DSTINC_NOINC;     /* 设置目标地址不递增 */

    /* DMA发送结束字通道配置 */
    dma_flags[1] = AM_LPC82X_DMA_XFER_VALID        |    /* 当前通道描述符有效 */
                   AM_LPC82X_DMA_XFER_SWTRIG       |    /* 软件触发 */
                   AM_LPC82X_DMA_XFER_CLRTRIG      |    /* 清除触发标志 */
                   AM_LPC82X_DMA_XFER_DSTINC_NOINC |    /* 设置目标地址不增加 */
                   AM_LPC82X_DMA_XFER_WIDTH_32BIT  |    /* 数据宽度32位 */
                   AM_LPC82X_DMA_XFER_SRCINC_NOINC;     /* 源地址不递增 */


    /* DMA接收通道配置 */
    dma_flags[2] = AM_LPC82X_DMA_XFER_VALID        |      /* 当前配置描述符有效 */
                   AM_LPC82X_DMA_XFER_SWTRIG       |      /* 软件触发 */
                   AM_LPC82X_DMA_XFER_CLRTRIG      |      /* 清除触发标志 */
                   AM_LPC82X_DMA_XFER_SRCINC_NOINC |      /* 源地址不递增 */
                   AM_LPC82X_DMA_XFER_SETINTA;            /* 使能中断A */

    /* 设置DMA数据位宽 */
    if (p_cur_trans->bits_per_word > 8) {
        dma_flags[0] |= AM_LPC82X_DMA_XFER_WIDTH_16BIT;
        dma_flags[2] |= AM_LPC82X_DMA_XFER_WIDTH_16BIT;
        eot_nbyte     = 2;
    } else {
        dma_flags[0] |= AM_LPC82X_DMA_XFER_WIDTH_8BIT;
        dma_flags[2] |= AM_LPC82X_DMA_XFER_WIDTH_8BIT;
        eot_nbyte     = 1;
    }

    /* 发送缓冲区有效 */
    if (p_cur_trans->p_txbuf != NULL) {
        dma_flags[0] |= AM_LPC82X_DMA_XFER_SRCINC_1X;      /* 源地址递增宽度 */
    } else {
        dma_flags[0] |= AM_LPC82X_DMA_XFER_SRCINC_NOINC;   /* 源地址不递增 */
    }

    /* 接收缓冲区有效 */
    if (p_cur_trans->p_rxbuf != NULL) {
        dma_flags[2] |= AM_LPC82X_DMA_XFER_DSTINC_1X;      /* 目标地址递增宽度 */
    } else {
        dma_flags[2] |= AM_LPC82X_DMA_XFER_DSTINC_NOINC ;            /* 目标地址不递增   */
    }

    /* 当只发生一次传输时 */
    if (p_this->dma_trans_num == 1) {

        /* 建立结束字通道描述符 */
        am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[0],                                  /* 描述符 */
                                        (uint32_t)&p_this->p_tx_buf_eot,                             /* 源端BUFF */
                                        (uint32_t)&(p_hw_spi->txdatctl),                     /* 目标BUFF */
                                         4,                                                  /* TX字节数 */
                                         dma_flags[1]);                                      /* 传输配置 */
        /* RX */
        if (p_cur_trans->p_rxbuf != NULL) {
            /* 建立RX通道描述符 */
            am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                  /* 描述符 */
                                            (uint32_t)&(p_hw_spi->rxdat),                        /* 源端BUFF */
                                            (uint32_t)&p_rx_buf[p_this->dma_trans_count * 512 * eot_nbyte], /* 目标BUFF */
                                            p_this->dma_trans_num * eot_nbyte,                   /* RX字节数 */                                            dma_flags[2]);                                       /* 传输配置 */
        } else {

            /* 建立RX通道描述符 */
            am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                  /* 描述符 */
                                            (uint32_t)&(p_hw_spi->rxdat),                        /* 源端BUFF */
                                            (uint32_t)p_rx_buf,                                  /* 目标BUFF */
                                            p_this->dma_trans_num * eot_nbyte,                   /* TX字节数 */
                                            dma_flags[2]);

        }

        p_this->dma_trans_flg = 0;

    /* 发送多次传输时 */
    } else {

         /* 传输超过1024个长度时 */
         if (p_this->dma_trans_num > 1024) {

             /* TX */
             if (p_cur_trans->p_txbuf != NULL) {
                 /* 建立TX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[0],                                    /* 描述符.  */
                                                  (uint32_t)&p_tx_buf[p_this->dma_trans_count * 512 * eot_nbyte],  /* 源端BUFF */
                                                  (uint32_t)&(p_hw_spi->txdat),                         /* 目标BUFF */
                                                  1024 *  eot_nbyte,                                    /* TX字节数 */
                                                  dma_flags[0]);                                        /* 传输配置 */
             } else {

                 /* 建立TX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[0],                                    /* 描述符.  */
                                                  (uint32_t)p_tx_buf,                                   /* 源端BUFF */
                                                  (uint32_t)&(p_hw_spi->txdat),                         /* 目标BUFF */
                                                  1024 *  eot_nbyte,                                    /* TX字节数 */
                                                  dma_flags[0]);

             }

             /* RX */
             if (p_cur_trans->p_rxbuf != NULL) {

                 /* 建立RX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                    /* 描述符 */
                                                 (uint32_t)&(p_hw_spi->rxdat),                          /* 源端BUFF */
                                                 (uint32_t)&p_rx_buf[p_this->dma_trans_count * 512 * eot_nbyte],   /* 目标BUFF */
                                                  1024 *  eot_nbyte,                                    /* RX字节数 */
                                                  dma_flags[2]);                                        /* 传输配置 */
             } else {
                 /* 建立RX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                      /* 描述符 */
                                                 (uint32_t)&(p_hw_spi->rxdat),                            /* 源端BUFF */
                                                 (uint32_t)p_rx_buf,                                      /* 目标BUFF */
                                                  1024 *  eot_nbyte,                                      /* RX字节数 */
                                                  dma_flags[2]);                                          /* 传输配置 */

             }

             /*　表记传输长度大于1024个长度,此时不需要发传输结束描述符 */
             p_this->dma_trans_flg = 1;

         } else { /* 传输小于或等于1024个长度时  */

             /* TX */
             if (p_cur_trans->p_txbuf != NULL) {
                 /* 建立TX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[0],                                    /* 描述符 */
                                                  (uint32_t)&p_tx_buf[p_this->dma_trans_count * 512 * eot_nbyte],  /* 源端BUFF */
                                                  (uint32_t)&(p_hw_spi->txdat),                         /* 目标BUFF */
                                                  p_this->dma_trans_num * eot_nbyte - eot_nbyte,        /* TX字节数 */
                                                  dma_flags[0]);                                        /* 传输配置 */
             } else {

                 /* 建立TX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[0],                                    /* 描述符.  */
                                                  (uint32_t)p_tx_buf,                                   /* 源端BUFF */
                                                  (uint32_t)&(p_hw_spi->txdat),                         /* 目标BUFF */
                                                  p_this->dma_trans_num * eot_nbyte - eot_nbyte,        /* TX字节数 */
                                                  dma_flags[0]);

             }

             /* 建立结束字通道描述符 */
             am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[1],              /* 描述符 */
                                             (uint32_t)&p_this->p_tx_buf_eot,         /* 源端BUFF */
                                             (uint32_t)&(p_hw_spi->txdatctl), /* 目标BUFF */
                                              4,                              /* TX字节数 */
                                              dma_flags[1]);                  /* 传输配置 */

             /* RX */
             if (p_cur_trans->p_rxbuf != NULL) {


                 /* 建立RX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                    /* 描述符 */
                                                 (uint32_t)&(p_hw_spi->rxdat),                          /* 源端BUFF */
                                                 (uint32_t)&p_rx_buf[p_this->dma_trans_count * 512 * eot_nbyte],   /* 目标BUFF */
                                                 p_this->dma_trans_num * eot_nbyte,                     /* TX字节数 */
                                                 dma_flags[2]);

             } else {
                 /* 建立RX通道描述符 */
                 am_lpc82x_dma_xfer_desc_build(&p_this->g_desc[2],                                      /* 描述符 */
                                                 (uint32_t)&(p_hw_spi->rxdat),                            /* 源端BUFF */
                                                 (uint32_t)p_rx_buf,                                      /* 目标BUFF */
                                                 p_this->dma_trans_num * eot_nbyte,                       /* TX字节数 */
                                                 dma_flags[2]);                                           /* 传输配置 */

             }


             p_this->dma_trans_flg = 0;

         }


         if (p_this->dma_trans_flg == 0) {
             /* DMA连接发送通道 */
             am_lpc82x_dma_xfer_desc_link(&p_this->g_desc[0], &p_this->g_desc[1]);
         }
    }

//    if (amhw_lpc82x_dma_chan_periph_is_enable(AMHW_LPC82X_DMA, p_ctr_tx->chan))


    /* 置位片选信号 */
    __spi_cs_on(p_this, p_this->p_cur_spi_dev);

    /* 启动DMA传输，马上开始传输 */
    am_lpc82x_dma_xfer_desc_startup(p_ctr_rx,
                                    &p_this->g_desc[2],
                                    __spi_dma_isr,
                                    (void *)p_this);

    /* 启动DMA传输，马上开始传输 */
    am_lpc82x_dma_xfer_desc_startup(p_ctr_tx,
                                    &p_this->g_desc[0],
                  __spi_dma_isr,
                                    (void *)p_this);
    return AM_OK;

}

/******************************************************************************/

/*  状态机内部状态切换 */
#define __SPI_NEXT_STATE(s, e) \
    do { \
        p_dev->state = (s); \
        new_event = (e); \
    } while(0)

/**
 * \brief  SPI 使用状态机传输
 */
am_local
int __spi_mst_sm_event (am_lpc82x_spi_dma_dev_t *p_dev, uint32_t event)
{
    amhw_lpc_spi_t *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);

    volatile uint32_t new_event = __SPI_EVT_NONE;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    while (1) {

        if (new_event != __SPI_EVT_NONE) {     /* 检查新事件是否来自内部 */
            event     = new_event;
            new_event = __SPI_EVT_NONE;
        }

        switch (p_dev->state) {

        case __SPI_ST_IDLE:         /* 控制器处于空闲状态 */
        {
            if (event != __SPI_EVT_TRANS_LAUNCH) {
                return -AM_EINVAL;  /* 空闲状态等待的消息必须是启动传输 */
            }
            /* 切换到消息开始状态，不用break */
        }
        /* No break */

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
            } else {
                /* 禁能所有中断 */
                amhw_lpc_spi_int_disable(p_hw_spi,
                                         AMHW_LPC_SPI_INTENCLR_RXRDY      |
                                         AMHW_LPC_SPI_INTENCLR_TXRDY      |
                                         AMHW_LPC_SPI_INTENCLR_RXOVERRUN  |
                                         AMHW_LPC_SPI_INTENCLR_TXUNDERRUN |
                                         AMHW_LPC_SPI_INTENCLR_SELNASSERT |
                                         AMHW_LPC_SPI_INTENCLR_SELNDEASSERT);
                p_dev->busy = AM_FALSE;
            }
            am_int_cpu_unlock(key);

            /* 无需要处理的消息 */
            if (p_cur_msg == NULL) {
                __SPI_NEXT_STATE(__SPI_ST_IDLE, __SPI_EVT_NONE);
                break;
            } else {

                /* 直接进入下一个状态，开始一个传输，此处无需break */
                __SPI_NEXT_STATE(__SPI_ST_TRANS_START, __SPI_EVT_TRANS_LAUNCH);

                event     = new_event;
                new_event = __SPI_EVT_NONE;
            }
        }
        /* No break */

        case __SPI_ST_TRANS_START:  /* 传输开始 */
        {
            am_spi_message_t  *p_cur_msg   = p_dev->p_cur_msg;

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

                /* notify the caller  */
                if (p_cur_msg->pfn_complete != NULL) {
                    p_cur_msg->pfn_complete(p_cur_msg->p_arg);
                }

                /* 片选关闭 */
                __spi_cs_off(p_dev, p_dev->p_cur_spi_dev);

                __SPI_NEXT_STATE(__SPI_ST_MSG_START, __SPI_EVT_TRANS_LAUNCH);

            } else {
                /* 获取到一个传输，正确处理该传输即可 */
                am_spi_transfer_t *p_cur_trans = __spi_trans_out(p_cur_msg);
                p_dev->p_cur_trans             = p_cur_trans;

                /* 配置SPI传输参数 */
                __spi_config(p_dev);

                /* 设置DMA传输的数据长度 */
                if (p_cur_trans->bits_per_word > 8) {
                    p_dev->dma_trans_num = p_cur_trans->nbytes / 2;
                } else {
                    p_dev->dma_trans_num = p_cur_trans->nbytes;
                }

                p_dev->dma_trans_flg = 0;
                p_dev->dma_trans_count = 0;

                /* 下一状态是使用DMA发送数据 */
               __SPI_NEXT_STATE(__SPI_ST_DMA_TRANS_DATA, __SPI_EVT_DMA_TRANS_DATA);
            }
            break;
        }

        case __SPI_ST_DMA_TRANS_DATA:    /* DMA发送数据 */
        {
            if (event != __SPI_EVT_DMA_TRANS_DATA) {
                return -AM_EINVAL;  /* 主机发送状态等待的消息必须是发送数据 */
            }

            if (p_dev->dma_trans_num > 1024) {
                /* 下一状态还是发送状态 */
                __SPI_NEXT_STATE(__SPI_ST_DMA_TRANS_DATA, __SPI_EVT_NONE);
            } else {
                 /* 下一状态是重新获取下一下trans进行传输  */
                __SPI_NEXT_STATE(__SPI_ST_TRANS_START, __SPI_EVT_NONE);
            }


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
am_spi_handle_t am_lpc82x_spi_dma_init (am_lpc82x_spi_dma_dev_t           *p_dev,
                                        const am_lpc82x_spi_dma_devinfo_t *p_devinfo)
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

    p_dev->dma_trans_count = 0;
    p_dev->dma_trans_flg   = 0;
    p_dev->dma_trans_num   = 0;

    p_dev->state            = __SPI_ST_IDLE;     /* 初始化为空闲状态 */

    am_list_head_init(&(p_dev->msg_list));

    if (__spi_hard_init(p_dev) != AM_OK) {
        return NULL;
    }


    /* 配置DMA发送通道 */
    p_dev->p_ctr_tx = am_lpc82x_dma_controller_get(p_devinfo->dma_chan_tx,
                                                   p_devinfo->dma_chan_prio_tx |
                                                   DMA_CHAN_OPT_PERIPH_REQ_EN);
    /* 配置DMA接收通道 */
    p_dev->p_ctr_rx = am_lpc82x_dma_controller_get(p_devinfo->dma_chan_rx,
                                                   p_devinfo->dma_chan_prio_rx |
                                                   DMA_CHAN_OPT_PERIPH_REQ_EN);

    return &(p_dev->spi_serve);
}

/**
 * \brief SPI 去除初始化
 */
void am_lpc82x_spi_dma_deinit (am_spi_handle_t handle)
{
    am_lpc82x_spi_dma_dev_t *p_dev    = (am_lpc82x_spi_dma_dev_t *)handle;
    amhw_lpc_spi_t          *p_hw_spi = (amhw_lpc_spi_t *)(p_dev->p_devinfo->spi_regbase);

    if (NULL == p_dev) {
        return ;
    }

    p_dev->spi_serve.p_funcs = NULL;
    p_dev->spi_serve.p_drv   = NULL;

    /* 禁能 SPI */
    amhw_lpc_spi_disable(p_hw_spi);

    /* 释放DMA控制器 */
    am_lpc82x_dma_controller_release(p_dev->p_ctr_tx);
    am_lpc82x_dma_controller_release(p_dev->p_ctr_rx);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}
