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
 * \brief I2C 从机例程(此例程可以用来模拟 EEPROM)，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. PIOB_6 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOB_7 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOB_6，SDA 引脚使用的是 PIOB_7，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_zlg116_hw_i2c_slave_poll.c src_zlg116_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 19-07-26  zc, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg116_hw_i2c_slave_poll
 * \copydoc demo_zlg116_hw_i2c_slave_poll.c
 */

/** [src_zlg116_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_stm32f103rbt6_clk.h"
#include "am_stm32f103rbt6.h"
#include "hw/amhw_stm32f103rbt6_i2c.h"
#include "am_delay.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** 主机操作从机地址定义 */
#define OPERATE_ADDR      (0XB2 >> 1)

#define EEPROM_BUFFER_LEN (0x20)

#define __I2C_SLV_ST_IDLE         0      /*< \brief 空闲状态 */
#define __I2C_SLV_ST_ADDR_MATCH   1      /*< \brief 地址匹配状态 */
#define __I2C_SLV_ST_SUBADDR_GET  2      /*< \brief 获取子地址状态 */
#define __I2C_SLV_ST_TXBYTE_GET   3      /*< \brief 获取发送的数据状态 */
#define __I2C_SLV_ST_RXBYTE_PUT   4      /*< \brief 接受数据 状态*/
#define __I2C_SLV_ST_STOP         5      /*< \brief 停止状态 */

/*******************************************************************************
  全局变量
*******************************************************************************/

static am_bool_t is_sub_addr = AM_FALSE;      /**< I2C 从机地址标志 */
static uint8_t   sub_addr;                    /**< I2C 从机地址 */
static uint8_t   eeprom[EEPROM_BUFFER_LEN];   /**< 缓冲区 */

/**
 * \brief I2C 从机初始化配置函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] dev_addr 主机操作从机的地址参数
 *
 * \retval AM_OK 初始化配置完成
 */
static int __i2c_slave_init (amhw_stm32f103rbt6_i2c_t *p_hw_i2c, uint8_t dev_addr)
{
     uint32_t pclk1 = 0;
     uint16_t tmpreg = 0, i2c_freq_value = 0;
     uint16_t result = 0x04;
     uint32_t speed = 100000;
     uint16_t i2c_dutycycle = AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2;
     uint16_t i2c_mode      = AMHW_STM32F103RBT6_I2C_MODE_I2C;
     uint16_t i2c_ack       = AMHW_STM32F103RBT6_I2C_ACK_ENABLE;

     uint16_t i2c_acknowledgedaddress = AMHW_STM32F103RBT6_I2C_ACK_ADDRESS_7_BIT;
     uint16_t i2c_ownaddress1 = dev_addr;

     if (p_hw_i2c == NULL) {
         return -AM_EINVAL;
     }

     /** <breif 配置 CR2 */
     tmpreg = p_hw_i2c->i2c_cr2;

     /** <brief 清零 freq[5:0] */
     tmpreg &= 0xFFC0;

     /** <brief 根据 pclk1  设置freq时钟 */
     pclk1 = am_clk_rate_get(CLK_APB1);

     i2c_freq_value = (uint16_t)(pclk1/1000000);
     tmpreg |= i2c_freq_value;

     /** <brief 写入CR2 */
     p_hw_i2c->i2c_cr2 = tmpreg;

     /** <brief 配置 CCR */

     /**
      *  \brief 禁能 I2C  配置 TRISE 清零 F/S DUTY CCR[11:0]
      */
     amhw_stm32f103rbt6_i2c_disable(p_hw_i2c);
     tmpreg = 0;

     /** <brief 最高为 100khz */
     if (speed <= 100000) {
         /** <brief 标准模式速度计算 */
         result = (uint16_t)(pclk1 / (speed << 1));

         if (result < 0x04) {

             /** <brief 设置最小允许值 */
             result = 0x04;
         }
         tmpreg |= result;
         /** \brief 设置标准模式下最大的 rise值     */
         p_hw_i2c->i2c_trise = i2c_freq_value + 1;
     } else {

         /** <brief 最高为 400khz */
         if (i2c_dutycycle == AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2 ) {
             result = (uint16_t)(pclk1 / (speed *3));
         } else {
             result = (uint16_t)(pclk1 / (speed * 25));
             result |= AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_16_9;
         }

         /** < 测试 CCR[11:0] 是否小于 0x01 */
         if ((result & 0x0FFF) == 0) {

             /** <brief 设置最小的 CCR值 */
             result |= (uint16_t)0x0001;
         }

         /** <brief 设置speed值  和  F/S为 快速模式 */
         tmpreg |= (uint16_t)(result | (uint16_t)0x8000);
         /** <brief 设置快速模式的最大rise时间 */
         p_hw_i2c->i2c_trise = (uint16_t) (((i2c_freq_value *(uint16_t)300) /
                                           (uint16_t)1000) + (uint16_t)1) ;
     }

     /** <brief 写入 ccr */
     p_hw_i2c->i2c_ccr = tmpreg;

     /** <brief 启动I2C外设 */
     amhw_stm32f103rbt6_i2c_enable(p_hw_i2c);

     /** <brief 配置 CR1 */

     tmpreg = p_hw_i2c->i2c_cr1;

     /** \brief 清零 ACK,SMBTYPE,SMBUS */
     tmpreg &= 0xFBF5;

     /**
      *  <brief 配置 I2C 模式 和 acknowledgement
      *  根据I2C的模式值 设置 SMBTYPE 和 SMBUS
      *  根据I2C的ACK值 设置ACK位
      */
     tmpreg |= (uint16_t)((uint16_t)i2c_mode | i2c_ack);

     /** \breif 写入CR1 */
     p_hw_i2c->i2c_cr1 = tmpreg;

     /** \breif I2C OAR1 配置 */
     p_hw_i2c->i2c_oar1 = (i2c_acknowledgedaddress |
                           (i2c_ownaddress1 <<1 ) );

     /* 默认不使用 10bit地址*/
     amhw_stm32f103rbt6_i2c_endual(p_hw_i2c,DISABLE);
     amhw_stm32f103rbt6_i2c_iten_mode_set(p_hw_i2c,
                                   I2C_CR2_ITERREN |
                                   I2C_CR2_ITEVTEN |
                                   I2C_CR2_ITBUFEN,
                                   DISABLE);

     /** 启动 I2C外设  */
     amhw_stm32f103rbt6_i2c_enable(p_hw_i2c);

     return AM_OK;
}

/**
 * \brief I2C 状态机函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 *
 * \return 无
 */

static void __i2c_slave_event (amhw_stm32f103rbt6_i2c_t *p_hw_i2c)
{
    static uint8_t status = __I2C_SLV_ST_IDLE;

    switch (status) {

    /* 空闲状态 */
    case __I2C_SLV_ST_IDLE:
        sub_addr = 0;

        /* no break*/

     /* 地址匹配状态 */
    case __I2C_SLV_ST_ADDR_MATCH:
        while(1) {
            if (p_hw_i2c->i2c_sr1 & 0x0002) {
                amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
                amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);
                break;
            }
        }

        status = __I2C_SLV_ST_SUBADDR_GET;
        /* no break*/

    /* 获取子地址状态  */
    case __I2C_SLV_ST_SUBADDR_GET:
        if (is_sub_addr == AM_FALSE) {
            am_udelay(100);
            sub_addr =  amhw_stm32f103rbt6_i2c_recv_data (p_hw_i2c);
            is_sub_addr = AM_TRUE;
            status = __I2C_SLV_ST_ADDR_MATCH;
        } else {
            if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                    AMHW_STM32F103RBT6_I2C_FLAG_TRA)) {
                status = __I2C_SLV_ST_TXBYTE_GET;
                is_sub_addr = AM_FALSE;
            } else {
                status = __I2C_SLV_ST_RXBYTE_PUT;
                is_sub_addr = AM_FALSE;
            }
        }

        break;

    /* 获取发送的数据状态  */
    case __I2C_SLV_ST_TXBYTE_GET:
        if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                AMHW_STM32F103RBT6_I2C_FLAG_TXE)) {

            amhw_stm32f103rbt6_i2c_send_data (p_hw_i2c, eeprom[sub_addr++]);
            sub_addr %= EEPROM_BUFFER_LEN;
        }
        if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                   AMHW_STM32F103RBT6_I2C_FLAG_AF)) {

            amhw_stm32f103rbt6_i2c_clearflag(p_hw_i2c, AMHW_STM32F103RBT6_I2C_FLAG_AF);
            status = __I2C_SLV_ST_IDLE;
                }

        break;

    /* 接受数据 状态  */
    case __I2C_SLV_ST_RXBYTE_PUT:
        if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                AMHW_STM32F103RBT6_I2C_FLAG_RXNE)) {

            eeprom[sub_addr++] = amhw_stm32f103rbt6_i2c_recv_data (p_hw_i2c);
            sub_addr %= EEPROM_BUFFER_LEN;
        }

        if (amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                AMHW_STM32F103RBT6_I2C_FLAG_STOPF)) {

            amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
            p_hw_i2c->i2c_cr1 &=0xffff;
            status = __I2C_SLV_ST_IDLE;
        }

        break;

    default:
        break;
    }
}

/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_i2c_slave_poll_entry (amhw_stm32f103rbt6_i2c_t *p_hw_i2c)
{

    __i2c_slave_init(p_hw_i2c, OPERATE_ADDR); /* I2C 从机初始化配置 */

    while(1) {
        __i2c_slave_event(p_hw_i2c);
    }
}
/** [src_zlg116_hw_i2c_slave_poll] */

/* end of file */
