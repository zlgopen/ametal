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
 * \brief 在I2C轮询模式下从LM75传感器读取温度值演示例程，通过HW层的接口实现
 *
 * - 操作步骤:
 *       1. 将J11和J13用跳线帽连接；
 *       2. 将程序下载至板子。
 *
 * - 实验现象:
 *       从LM75读回来的温度值向串口打印。
 *
 * \par 源代码
 * \snippet demo_fsl_hw_i2c_master_poll.c src_fsl_hw_i2c_master_poll
 * 
 * \internal
 * \par History
 * - 1.00 16-09-22  ari, first implementation.
 * \endinternal
 */
 
/**
 * \addtogroup demo_fsl_if_hw_i2c_master_poll
 * \copydoc demo_fsl_hw_i2c_master_poll.c
 */
 
/** [src_fsl_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_list.h"
#include "am_fsl_i2c.h"
#include "hw/amhw_fsl_i2c.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_gpio.h"
#include "am_board.h"
#include "demo_fsl_entrys.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define I2C_M_7BIT        0x0000u          /**< \brief 7-bits 设备地址           */
#define I2C_M_10BIT       0x0001u          /**< \brief 10-bit 设备地址           */
#define I2C_M_WR          0x0000u          /**< \brief 写操作                    */
#define I2C_M_RD          0x0002u          /**< \brief 读操作                    */
#define I2C_M_NOSTART     0x0010u          /**< \brief 无需重新启动标识          */
                      
//#define I2C_MASTER        KL26_I2C1   /**< \brief I2C主机控制器通道号定义   */
#define I2C_SPEED         100000           /**< \brief I2C控制器速度参数定义     */
#define LM75_ADDR         0x48             /**< \brief AAM75设备地址定义        */


/**
 * \brief I2C传输结构体定义，主要用于I2C轮询模式下。
 */ 
typedef struct i2c_transfer {
    /** \brief I2C设备地址          */ 
    volatile uint16_t   addr;      
    /** \brief I2C 设备特性以及transfer需要的特殊标识 */
    volatile uint16_t   flags;     
    /** \brief I2C 传输数据缓存指针 */
    volatile void      *p_buf;     
    /** \brief I2C 数据缓存长度     */
    volatile uint16_t   length;    
} i2c_transfer_t;

/**
 * \brief I2C时钟配置结构体
 */
typedef struct __i2c_div_table {
    uint8_t   icr;         /**< \brief I2C 乘数因子        */
    uint16_t  scl_div;     /**< \brief I2C 分频因子        */
} __i2c_div_table_t;

/**
 * \brief I2C传输结构体参数设置函数
 *
 * \param[in] p_trans : 指向I2C传输结构体的指针
 * \param[in] addr    : 设备地址参数
 * \param[in] flags   : 传输控制标识位参数 
 * \param[in] p_buf   : 指向发送或者接收数据缓存的指针
 * \param[in] length  : 缓存的字节长度参数
 *
 * \retval AM_OK : 传输结构体设置完成
 */
static int __i2c_mktrans (i2c_transfer_t    *p_trans,
                          uint16_t           addr,  
                          uint16_t           flags, 
                          void              *p_buf, 
                          uint16_t           length)
{
    if(p_trans == NULL) {
        return -AM_EINVAL;
    }
    
    p_trans->addr   = addr;
    p_trans->flags  = flags;
    p_trans->p_buf  = p_buf;
    p_trans->length = length;
    
    return AM_OK;
}

/**
 * \brief I2C 波特率设置函数
 */
static int __i2c_speed_set(amhw_fsl_i2c_t *p_hw_i2c,
                           uint32_t        i2c_clk,
                           uint32_t        speed)
{

    uint8_t  mult , i , multiplier;
    uint32_t rate , abs_error;
    uint32_t min_error = 0xffffffff;
    uint32_t best_mult = 0;
    uint32_t best_icr  = 0;

    if (p_hw_i2c  == NULL) {
        return -AM_EINVAL;
    }

    /* I2C 时钟分频因子列表  */
    const __i2c_div_table_t div_table[50] = {
        { 0x00, 20 },   { 0x01, 22 },   { 0x02, 24 },   { 0x03, 26 },
        { 0x04, 28 },   { 0x05, 30 },   { 0x09, 32 },   { 0x06, 34 },
        { 0x0a, 36 },   { 0x07, 40 },   { 0x0c, 44 },   { 0x10, 48 },
        { 0x11, 56 },   { 0x12, 64 },   { 0x0f, 68 },   { 0x13, 72 },
        { 0x14, 80 },   { 0x15, 88 },   { 0x19, 96 },   { 0x16, 104 },
        { 0x1a, 112 },  { 0x17, 128 },  { 0x1c, 144 },  { 0x1d, 160 },
        { 0x1e, 192 },  { 0x22, 224 },  { 0x1f, 240 },  { 0x23, 256 },
        { 0x24, 288 },  { 0x25, 320 },  { 0x26, 384 },  { 0x2a, 448 },
        { 0x27, 480 },  { 0x2b, 512 },  { 0x2c, 576 },  { 0x2d, 640 },
        { 0x2e, 768 },  { 0x32, 896 },  { 0x2f, 960 },  { 0x33, 1024 },
        { 0x34, 1152 }, { 0x35, 1280 }, { 0x36, 1536 }, { 0x3a, 1792 },
        { 0x37, 1920 }, { 0x3b, 2048 }, { 0x3c, 2304 }, { 0x3d, 2560 },
        { 0x3e, 3072 }, { 0x3f, 3840 }
    };

    if(speed > 1000000) {
        return  -AM_EINVAL;
    }
    for (mult = 0 ; mult <= 2 ; mult++) {
        multiplier = 1 << mult;
        for (i = 0 ; i < 50 ; i++) {
            rate = i2c_clk / (multiplier * div_table[i].scl_div);
            abs_error = speed > rate ? speed - rate : rate - speed;
            if (abs_error < min_error) {
                best_mult = mult;
                best_icr  =  div_table[i].icr;
                min_error = abs_error;
            }
            if (min_error == 0) {
                break;
            }
        }
    }

    /* 写入分频因子，设置波特率  */
    amhw_fsl_i2c_clk_div_set(p_hw_i2c , (best_mult << 6)|best_icr);
    return AM_OK;
}

/**
 * \brief I2C主机启动函数
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] p_trans  : 指向I2C传输结构体的指针
 *
 * \return 无
 */
static void __i2c_mst_start (amhw_fsl_i2c_t     *p_hw_i2c,
                             i2c_transfer_t      *p_trans)
{
    uint16_t dev_addr = (p_trans->addr) << 1;
    
    if(!(p_trans->flags & I2C_M_NOSTART)) {
        
        if (p_trans->flags & I2C_M_RD) {
            dev_addr |= 0x1;            /* 设置bit0为1，表明当前为读操作 */
            amhw_fsl_i2c_restart_signal_send(p_hw_i2c);

        } else {
            dev_addr |= 0x0;            /* 设置bit0为0，表明当前为写操作 */
            amhw_fsl_i2c_start_signal_send(p_hw_i2c);
        }

        amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_SEND);

        /* 发送设备地址 */
        amhw_fsl_i2c_data_write(p_hw_i2c, dev_addr);

    }
    while (!(p_hw_i2c->status & AMHW_FSL_I2C_STAT_IICIF )) ;
    amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);
}

/**
 * \brief I2C主机发送地址/数据
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] p_trans  : 指向I2C传输结构体的指针
 *
 * \retval AM_OK : 发送完成
 */
static int __i2c_mst_send (amhw_fsl_i2c_t    *p_hw_i2c,
                           i2c_transfer_t    *p_trans)
                      
{
    uint8_t i;
    amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_SEND);
    for (i = 0;i < p_trans->length; i++) {

        amhw_fsl_i2c_data_write(p_hw_i2c, ((uint8_t *)(p_trans->p_buf))[i]);
        while (!(p_hw_i2c->status & AMHW_FSL_I2C_STAT_IICIF )) ;
        amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);

    }

    return AM_OK;
}

/**
 * \brief  I2C主机接收地址/数据
 *
 * \param[in] p_hw_i2c : 指向I2C寄存器块的指针
 * \param[in] p_trans  : 指向I2C传输结构体的指针
 *
 * \retval AM_OK : 接收完成
 */
static int __i2c_mst_recv (amhw_fsl_i2c_t *p_hw_i2c,
                           i2c_transfer_t    *p_trans)
{
    uint8_t i;
    amhw_fsl_i2c_transmode_set(p_hw_i2c, AMHW_FSL_I2C_TRANSMODE_RECV );
    for (i = 0; i < p_trans->length; i++) {

        /* 接收数据 */
        ((uint8_t *)(p_trans->p_buf))[i] = amhw_fsl_i2c_data_read(p_hw_i2c);
        while (!(p_hw_i2c->status & AMHW_FSL_I2C_STAT_IICIF )) ;
        amhw_fsl_i2c_stat_clr(p_hw_i2c , AMHW_FSL_I2C_STAT_IICIF);

    }
    amhw_fsl_i2c_stop_signal_send(p_hw_i2c);

    /* p_buf[0] 中读取的是上次写入的 设备地址（AM75_ADDR） 停止后再读一次 */
    ((uint8_t *)(p_trans->p_buf))[0] = amhw_fsl_i2c_data_read(p_hw_i2c);
    return AM_OK;    
}

/**
 * \brief 测试函数入口
 */
void demo_fsl_hw_i2c_master_poll_entry (amhw_fsl_i2c_t *p_hw_i2c,
                                        uint32_t        i2c_clk)
{
    i2c_transfer_t    i2c1_transfer;
    uint8_t           lm75_data[2];
    uint8_t           test_addr   = 0x00;
    
    am_kprintf("I2C operate LM75 demo in polling mode .\r\n");

    amhw_fsl_i2c_disable(p_hw_i2c);
       /* I2C 波特率设置  */
    __i2c_speed_set(p_hw_i2c, i2c_clk, I2C_SPEED);
    amhw_fsl_i2c_enable(p_hw_i2c);

    while (1) {
         /* 设置transfer结构体参数，写入地址 */
        __i2c_mktrans(&i2c1_transfer,
                      LM75_ADDR,
                      (I2C_M_7BIT | I2C_M_WR),
                      (uint8_t *)&test_addr,
                      1);

        __i2c_mst_start(p_hw_i2c, &i2c1_transfer);
        __i2c_mst_send (p_hw_i2c, &i2c1_transfer);

        /* 设置transfer结构体参数，写入数据 */
        __i2c_mktrans(&i2c1_transfer,
                      LM75_ADDR,
                      (I2C_M_7BIT | I2C_M_RD),
                      (uint8_t *)&lm75_data[0],
                      2);

        __i2c_mst_start(p_hw_i2c, &i2c1_transfer);
        __i2c_mst_recv(p_hw_i2c, &i2c1_transfer);

        uint16_t temp;
        lm75_data[0] &= 0xE0;
        temp = lm75_data[1] << 8 | lm75_data[0];
        AM_DBG_INFO("Current temperature is %d°C\r\n", temp / 256 );
        am_mdelay(500);
    }

}

/** [src_fsl_hw_i2c_master_poll] */

/* end of file */
