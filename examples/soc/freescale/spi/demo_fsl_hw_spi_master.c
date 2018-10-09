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
 * \brief SPI主机演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *   1. 将SPI的PIOC_6和PIOC_7用线相连，模拟从机设备，回环测试；
 *
 * - 实验现象:
 *   1. 主机通过MOSI发送数据，发出的数据从MOSI读回；
 *   2. 比较发出和读回的数据，如果相同LED0一直常亮,否则以200ms时间间隔闪烁；
 *
 * \par 源代码
 * \snippet demo_kl26_hw_spi_master.c src_kl26_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 16-09-27  sdy, first implementation.
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_kl26_if_hw_spi_master
 * \copydoc demo_kl26_hw_spi_master.c
 */
 
/** [src_kl26_hw_spi_master] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_gpio.h"
#include "hw/amhw_fsl_spi.h"
#include "am_board.h"

/**
 * \name SPI传输结构体配置参数
 */

#define SPI_CFG_LSB         AM_BIT(0)   /**< \brief 低位优先发送         */
#define SPI_CFG_16BIT       AM_BIT(1)   /**< \brief 传输数据宽度为16位   */

#define SPI_CFG_MODE_0      (0)                     /**< \brief 模式0    */
#define SPI_CFG_MODE_1      (0         | AM_BIT(2)) /**< \brief 模式1    */
#define SPI_CFG_MODE_2      (AM_BIT(3) | 0)         /**< \brief 模式2    */
#define SPI_CFG_MODE_3      (AM_BIT(2) | AM_BIT(3)) /**< \brief 模式3    */

/** @} */

/** 
 * \brief SPI传输结构体
 */
typedef struct spi_transfer {
    const void  *p_txbuf;           /**< \brief 指向发送数据缓存的指针   */
    void        *p_rxbuf;           /**< \brief 指向接收数据缓存的指针    */
    uint32_t     nbytes;            /**< \brief 数据长度                  */
    uint32_t     flags;             /**< \brief SPI传输控制标志位         */
} spi_transfer_t;


/**
 * \brief SPI传输结构体参数设置
 *
 * \param[in] p_trans : 指向SPI传输结构体
 * \param[in] p_txbuf : 指向发送数据缓存的指针
 * \param[in] p_rxbuf : 指向接收数据缓存的指针
 * \param[in] nbytes  : 当前传输数据长度
 * \param[in] flags   : SPI传输控制标志位
 *
 * \retval  AM_OK     : 设置完成
 */
static int __spi_mktrans (spi_transfer_t *p_trans, 
                          const void     *p_txbuf,
                          void           *p_rxbuf,
                          uint32_t        nbytes,
                          uint16_t        flags)
                    
{
    if (p_trans == NULL) {
        return -AM_EINVAL;
    }
    
    p_trans->p_txbuf  = p_txbuf;
    p_trans->p_rxbuf  = p_rxbuf;
    p_trans->nbytes   = nbytes;
    p_trans->flags    = flags;

    return AM_OK;
}

/**
 * \brief SPI速度配置
 *
 * \param[in] p_hw_spi     : 指向SPI寄存器块的指针
 * \param[in] target_speed : 目标速度
 * \param[in] spi_freq     : SPI输入时钟频率
 *
 * \return  无
 */
static void __spi_speed_cfg (amhw_fsl_spi_t *p_hw_spi,
                             uint32_t        target_speed,
                             uint32_t        spi_freq)
{

    uint32_t real_speed;              /* 计算出的速度    */
    uint32_t pdiv, best_pdiv;         /* 预分频值        */
    uint32_t div, best_div;           /* 分配值          */
    uint32_t diff, min_diff;          /* 速度差值        */

    min_diff = 0xFFFFFFFFU;

    /* 设置为最大的分频值,速度为最小 */
    best_pdiv  = 7;
    best_div   = 8;

    /* 查找最合适的速度 */
    for (pdiv = 0; (pdiv <= 7) && min_diff; pdiv++)
    {
       for (div = 0; (div <= 8) && min_diff; div++)
       {
           /* all_div = (pdiv+1) * 2^(div+1) */
           real_speed = (spi_freq / ((pdiv + 1) << (div+1)));

           /* 实际速度应小于目标速度 */
           if (target_speed >= real_speed)
           {
               diff = target_speed-real_speed;

               if (min_diff > diff)
               {
                   /* 最合适的速度 */
                   min_diff   = diff;
                   best_pdiv  = pdiv;
                   best_div   = div;
               }
           }
       }
    }

   amhw_fsl_spi_div_cfg(p_hw_spi, best_pdiv, best_div);
}

static void __spi_master_cfg (amhw_fsl_spi_t *p_hw_spi, uint32_t flags)
{
    if (flags & SPI_CFG_LSB) {
        amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_LSB);
    } else {
        amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_MSB);
    }

    if (flags & SPI_CFG_16BIT) {
        amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_16BIT_WIDTH);
    } else {
        amhw_fsl_spi_feature_cfg(p_hw_spi, AMHW_FSL_SPI_CFG_8BIT_WIDTH);
    }

    /* 配置时钟相位和极性 */
    amhw_fsl_spi_mode_cfg(p_hw_spi, flags);
}

/**
 * \brief SPI片选控制
 *
 * \param[in] p_hw_spi     : 指向SPI寄存器块的指针
 * \param[in] pin          : CS引脚号
 * \param[in] active_level : 引脚有效状态电平
 * \param[in] state        : 状态
 *
 * \return  无
 */
static void __spi_master_cs_ctr (amhw_fsl_spi_t *p_hw_spi,
                                 int             pin,
                                 uint8_t         active_level,
                                 am_bool_t       state)
{
    /* 片选有效 */
    if (state) {
        am_gpio_set(pin, active_level);
    
    /* 片选无效 */
    } else {
        am_gpio_set(pin, !active_level);
    }
}

/**
 * \brief SPI回环传输测试
 */
static void __spi_loop_trans (amhw_fsl_spi_t  *p_hw_spi,
                              spi_transfer_t  *p_trans,
                              int              pin_cs)
{
    uint32_t pos = 0;

    am_bool_t  width_16bit  = (p_trans->flags & SPI_CFG_16BIT) ? AM_TRUE : AM_FALSE;
    
    __spi_master_cfg(p_hw_spi, p_trans->flags);

    __spi_master_cs_ctr(p_hw_spi, pin_cs, AM_GPIO_LEVEL_LOW, AM_TRUE);

    while(pos < p_trans->nbytes) {

        /* 等待可以发送 */
        while ((amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_T_EMPTY) == 0);

        if (width_16bit) {
            amhw_fsl_spi_data16_wirte(p_hw_spi, *(uint16_t*)((uint32_t)p_trans->p_txbuf + pos));
        } else {
            amhw_fsl_spi_data8_wirte(p_hw_spi, *(uint8_t*)((uint32_t)p_trans->p_txbuf + pos));
        }

        /* 等待可以接收 */
        while ((amhw_fsl_spi_stat_get(p_hw_spi) & AMHW_FSL_SPI_STAT_R_FULL) == 0);

        if (width_16bit) {
            *(uint16_t*)((uint32_t)p_trans->p_rxbuf + pos) = amhw_fsl_spi_data16_read(p_hw_spi);
            pos +=2;
        } else {
            *(uint8_t*)((uint32_t)p_trans->p_rxbuf + pos) = amhw_fsl_spi_data8_read(p_hw_spi);
            pos += 1;
        }
    }
    
    /* 处理片选 */
    __spi_master_cs_ctr(p_hw_spi, pin_cs, AM_GPIO_LEVEL_LOW, AM_FALSE);
}

/**
 * \brief 例程入口
 */
void demo_fsl_hw_spi_master_entry (amhw_fsl_spi_t *p_hw_spi,
                                   int             pin_cs,
                                   uint32_t        spi_freq)
{
    spi_transfer_t spi0_transfer;     /* 定义一个SPI传输结构体实例          */

    const uint32_t length =32;
    uint16_t       spi_send_buf[16];
    uint16_t       spi_recv_buf[16];

    uint8_t i;

    am_bool_t error_ocur = AM_FALSE;

    amhw_fsl_spi_workmode_cfg(p_hw_spi, AMHW_FSL_SPI_WMODE_MASTER);
    __spi_speed_cfg(p_hw_spi, 3000000, spi_freq);
    amhw_fsl_spi_enable(p_hw_spi);

    /* 构造数据 */
    for (i = 0; i < (length / 2); i++) {
        spi_send_buf[i] = i;
    }
    __spi_mktrans(&spi0_transfer,
                  &spi_send_buf,
                  &spi_recv_buf,
                  length,
                  (SPI_CFG_MODE_0 | SPI_CFG_16BIT));

    /* 数据传输 */
    __spi_loop_trans(p_hw_spi, &spi0_transfer, pin_cs);

    /* 数据检验 */
    for (i = 0; (i < length / 2); i++) {
        if (spi_recv_buf[i] != spi_send_buf[i]) {
            error_ocur = AM_TRUE;
            break;
        }
        am_kprintf("recv = %d\r\n", spi_recv_buf[i]);
    }

    while (1) {
        if (error_ocur) {
            am_led_on(LED0);
            am_mdelay(200);
            am_led_off(LED0);
            am_mdelay(200);
        } else {
            am_led_on(LED0);
        }
    }
}

/** [src_fsl_hw_spi_master] */

/* end of file */
