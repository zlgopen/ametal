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
 * \brief SPI 主机例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 将本主机的 SPI接口与从机SPI接口对应连接 。
 *   2. 先复位主机，再复位从机，再运行主机程序。
 *   3. 特别注意，主机的整个复位动作会产生一个等效的片选信号，导致从机错误识别上传一次历史数据。
 *      请注意步骤2操作顺序，以避免从机错误数据上传。
 *
 * - 实验现象：
 *   1. 主机发送的数据不断累加1，从机发回的数据保持不变。
 *   2. 调试串口打印通过SPI接收和发送的具体数据。
 *
 * \par 源代码
 * \snippet demo_stm32f103rbt6_hw_spi_master.c src_stm32f103rbt6_hw_spi_master
 *
 * \internal
 * \par History
 * - 1.00 19-07-23  ari, first implementation
 * \endinternal
 */ 
 
/**
 * \addtogroup demo_if_stm32f103rbt6_hw_spi_master
 * \copydoc demo_stm32f103rbt6_hw_spi_master.c
 */
 
/** [src_stm32f103rbt6_hw_spi_master] */
#include "ametal.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_stm32f103rbt6_spi.h"

/**
 * \name SPI传输结构体配置参数
 */

#define SPI_CFG_MODE_0      0           /**< \brief 模式0 */
#define SPI_CFG_MODE_1      1           /**< \brief 模式1 */
#define SPI_CFG_MODE_2      2           /**< \brief 模式2 */
#define SPI_CFG_MODE_3      3           /**< \brief 模式3 */

/** @} */

/** 
 * \brief SPI传输结构体
 */
typedef struct spi_transfer {
    const void  *p_txbuf;           /**< \brief 指向发送数据缓存的指针 */
    void        *p_rxbuf;           /**< \brief 指向接收数据缓存的指针 */
    uint32_t     nbytes;            /**< \brief 数据长度 */
    uint32_t     flags;             /**< \brief SPI传输控制标志位 */
    uint32_t     spi_clk;           /**< \brief SPI时钟频率 */
} spi_transfer_t;

spi_transfer_t g_spi_transfer;      /* 定义一个SPI传输结构体实例 */

static void spi_master_cfg (amhw_stm32f103rbt6_spi_t *p_hw_spi)
{
    /* 关闭spi外设*/
    amhw_stm32f103rbt6_spi_disable(p_hw_spi);

    /* NSS采用软件管理*/
    amhw_stm32f103rbt6_spi_ssm_enable(p_hw_spi);

    /* 关闭硬件SPI的NSS引脚输出*/
    amhw_stm32f103rbt6_spi_ssout_disable(p_hw_spi);

    /* 软件管理下，NSS内部为高，与引脚无关。特别注意，无论硬件、软件管理，配置成主机模式，NSS必须为高*/
    amhw_stm32f103rbt6_spi_ssi_set(p_hw_spi,AMHW_STM32F103RBT6_SPI_SSI_TO_NSS_ENABLE);

    /* 设置为主机模式*/
    amhw_stm32f103rbt6_spi_master_salver_set(p_hw_spi, AMHW_STM32F103RBT6_SPI_MASTER);

    /* 配置时钟相位和极性 */
    amhw_stm32f103rbt6_spi_clk_mode_set(p_hw_spi, SPI_CFG_MODE_1 );

    /* 帧格式设置，先发送MSB*/
    amhw_stm32f103rbt6_spi_lsbfirst_set(p_hw_spi, AMHW_STM32F103RBT6_SPI_LSB_FIRST_SEND_MSB);

    /* 分频系数为128*/
    amhw_stm32f103rbt6_spi_baudratefre_set(p_hw_spi,
                                    AMHW_STM32F103RBT6_SPI_BAUDRATE_PRESCALER_128);

    /* 数据长度为8*/
    amhw_stm32f103rbt6_spi_data_length_set(p_hw_spi, AMHW_STM32F103RBT6_SPI_DATA_8BIT);

    /* SPI使能*/
    amhw_stm32f103rbt6_spi_enable(p_hw_spi);

}

/**
 * \brief SPI回环传输测试
 */
void spi_loop_trans (amhw_stm32f103rbt6_spi_t *p_hw_spi,
                     spi_transfer_t    *p_trans,
                     int32_t            cs_pin,
					 uint32_t           cs_mdelay)
{
    uint32_t pos = 0;

    /* 片选引脚输出低电平 */
    am_gpio_set(cs_pin, AM_GPIO_LEVEL_LOW);

    /* CS选通后，至少需要等待100ms，等待从机(单片机)完成相关配置才能正确传输数据，否则部分数据会丢失、错乱*/
    /* 假如是其他从机单位，响应较快，可考虑取消此处延时*/
    if(cs_mdelay != 0) {
    	am_mdelay(cs_mdelay);
    }

    while(pos < p_trans->nbytes) {

        /* 等待可以发送 */
        while (amhw_stm32f103rbt6_spi_status_flag_check (
            p_hw_spi, AMHW_STM32F103RBT6_SPI_TX_EMPTY_FLAG) == AM_FALSE);

        amhw_stm32f103rbt6_spi_tx_put(p_hw_spi,*((uint8_t*)(p_trans->p_txbuf) + pos));

        /* 等待可以接收 */
        while (amhw_stm32f103rbt6_spi_status_flag_check (
            p_hw_spi,AMHW_STM32F103RBT6_SPI_RX_NOT_EMPTY_FLAG) == AM_FALSE) {
        	;
        }

        *(uint8_t*)((uint8_t*)p_trans->p_rxbuf + pos) =
            amhw_stm32f103rbt6_spi_rx_get(p_hw_spi);
        pos += 1;
    }

    /* 片选引脚输出高电平 */
    am_gpio_set(cs_pin, AM_GPIO_LEVEL_HIGH);
}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_spi_master_entry (amhw_stm32f103rbt6_spi_t *p_hw_spi,
                                      int32_t            cs_pin,
                                      uint32_t           clk_rate,
                                      uint32_t           cs_mdelay)
{
    uint8_t         i = 0, t = 0;
    uint8_t         spi_send_buf[21] = {0};
    uint8_t         spi_recv_buf[21] = {0};
    const uint32_t  length     = 20;
    spi_transfer_t *p_trans    = &g_spi_transfer;

    p_trans->p_txbuf = spi_send_buf;
    p_trans->p_rxbuf = spi_recv_buf;
    p_trans->nbytes  = length;
    p_trans->spi_clk = clk_rate;

    /* 构造发送数据*/
    for (i = 0; i < length; i++) {
        spi_send_buf[i] = i +'A';
    }

    /* SPI主机模式配置 */
    spi_master_cfg(p_hw_spi);

    AM_FOREVER {

        /* 数据传输 */
        spi_loop_trans(p_hw_spi, p_trans, cs_pin, cs_mdelay);

        am_kprintf("spi_recv_buf: %s\r\n",spi_recv_buf);
        am_kprintf("spi_send_buf: %s\r\n",spi_send_buf);

        /* 清空接收缓存区 */
        for (i = 0; i < length; i++) {
            spi_recv_buf[i] = '\0';
        }

        /* 构造新的发送数据*/
        for (i = 0; i < length; i++) {
            spi_send_buf[i] = i +'A' + t;
        }

        t++;

        if (t == 50) {
            t = 0;
        }
        am_mdelay(1000);
    }
}
/** [src_stm32f103rbt6_hw_spi_master] */

/* end of file */
