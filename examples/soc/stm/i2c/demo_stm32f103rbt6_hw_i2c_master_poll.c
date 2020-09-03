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
 * \brief I2C 轮询模式下操作 EEPROM 例程，通过 HW 层接口实现
 *
 * - 操作步骤：
 *   1. 连接 EEPROM 的 I2C 引脚到相应引脚。
 *
 * - 实验现象：
 *   1. 写数据到 EEPROM；
 *   2. 读出刚才写入的数据；
 *   3. 调试串口打印测试结果。
 *
 * \par 源代码
 * \snippet demo_zlg116_hw_i2c_master_poll.c src_zlg116_hw_i2c_master_poll
 *
 * \internal
 * \par History
 * - 1.00 19-07-26  zc, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zlg116_hw_i2c_master_poll
 * \copydoc demo_zlg116_hw_i2c_master_poll.c
 */

/** [src_zlg116_hw_i2c_master_poll] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "hw/amhw_stm32f103rbt6_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/
#define I2C_M_7BIT    0x0000u          /**< \brief 7-bits 设备地址 */
#define I2C_M_10BIT   0x0001u          /**< \brief 10-bit 设备地址 */
#define I2C_M_WR      0x0000u          /**< \brief 写操作 */
#define I2C_M_RD      0x0002u          /**< \brief 读操作 */
#define I2C_M_NOSTART 0x0010u          /**< \brief 无需重新启动标识 */

#define I2C_SPEED     100000            /**< \brief I2C 控制器速度参数定义 */
#define EEPROM_ADDR   (0xb2 >> 1)       /**< \brief EEPROM 设备地址定义 */
#define TEST_LEN      0Xb                 /**< \brief 操作 EEPROM 的页大小 */

/**
 * \brief I2C 传输结构体定义，主要用于 I2C 轮询模式下
 */
typedef struct i2c_transfer {

    /** \brief I2C设备地址 */
    volatile uint16_t   addr;

    /** \brief I2C 设备特性以及transfer需要的特殊标识 */
    volatile uint16_t   flags;

    /** \brief I2C 传输数据缓存指针 */
    volatile void      *p_buf;

    /** \brief I2C 数据缓存长度 */
    volatile uint16_t   length;
} i2c_transfer_t;

/** \brief I2C 传输结构体 */
static i2c_transfer_t __g_i2c1_transfer;

/**
 * \brief I2C 传输结构体参数设置函数
 *
 * \param[in] p_trans 指向 I2C 传输结构体的指针
 * \param[in] addr    设备地址参数
 * \param[in] flags   传输控制标识位参数
 * \param[in] p_buf   指向发送或者接收数据缓存的指针
 * \param[in] length  缓存的字节长度参数
 *
 * \retval AM_OK 传输结构体设置完成
 */
static int __i2c_mktrans (i2c_transfer_t *p_trans,
                          uint16_t        addr,
                          uint16_t        flags,
                          void           *p_buf,
                          uint16_t        length)
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
 * \brief I2C 主机发送地址/数据
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] p_trans  指向I2C传输结构体的指针
 * \param[in] stop     是否发送停止信号
 *                         AM_TRUE  : 是
 *                         AM_FALSE : 否
 *
 * \retval AM_OK 发送完成
 */
static int __i2c_mst_send (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                           i2c_transfer_t    *p_trans,
                           am_bool_t          stop)

{
    uint8_t i;

    for (i = 0; i < p_trans->length; i++) {

        while (!amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                AMHW_STM32F103RBT6_I2C_FLAG_TXE));

        if (i == (p_trans->length - 1) && stop ) {
            amhw_stm32f103rbt6_i2c_send_data(p_hw_i2c,
                    ((uint8_t *)(p_trans->p_buf))[i]);

        } else {
            amhw_stm32f103rbt6_i2c_send_data(p_hw_i2c,
                    ((uint8_t *)(p_trans->p_buf))[i]);
        }

    }

    if (stop ) {

        while (!amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
            AMHW_STM32F103RBT6_I2C_FLAG_BTF));

            /* 设置停止位  */
           amhw_stm32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
    }

    return AM_OK;
}

/**
 * \brief I2C 主机接收地址/数据
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] p_trans  指向I2C传输结构体的指针
 * \param[in] stop     是否发送停止信号
 *                         AM_TRUE  : 是
 *                         AM_FALSE : 否
 *
 * \retval AM_OK 接收完成
 */
static int __i2c_mst_recv (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                           i2c_transfer_t    *p_trans,
                           am_bool_t          stop)
{
    uint8_t i;

    for (i = 0; i < p_trans->length; i++) {
        while (!amhw_stm32f103rbt6_i2c_checkflagstaus(p_hw_i2c,
                AMHW_STM32F103RBT6_I2C_FLAG_RXNE));

        /* 接收数据 */
        ((uint8_t *)(p_trans->p_buf))[i] = amhw_stm32f103rbt6_i2c_recv_data(p_hw_i2c);

        if (i == (p_trans->length - 2) && stop) {

            /* 产生停止信号后  iic 回到从机模式 */
            amhw_stm32f103rbt6_i2c_genstop(p_hw_i2c, ENABLE);
            amhw_stm32f103rbt6_i2c_ack_en(p_hw_i2c, DISABLE);
        }
    }

    return AM_OK;
}

/**
 * \brief I2C 主机初始化配置
 *
 * \param[in] p_hw_i2c 指向I2C寄存器块的指针
 * \param[in] speed    主机速度参数
 *
 * \retval  AM_OK     配置完成
 * \retval -AM_EINVAL 参数无效
 */
static int __i2c_mst_init (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                           uint32_t           speed,
                           uint32_t           clk_rate)
{

    uint32_t pclk1 = 0;
    uint16_t tmpreg = 0, i2c_freq_value = 0;
    uint16_t result = 0x04;

    uint16_t i2c_dutycycle = AMHW_STM32F103RBT6_I2C_DUTY_CYCLE_2;
    uint16_t i2c_mode      = AMHW_STM32F103RBT6_I2C_MODE_I2C;
    uint16_t i2c_ack       = AMHW_STM32F103RBT6_I2C_ACK_ENABLE;

    uint16_t i2c_acknowledgedaddress = AMHW_STM32F103RBT6_I2C_ACK_ADDRESS_7_BIT;
    uint16_t i2c_ownaddress1 = 0x01;

    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /** <breif 配置 CR2 */
    tmpreg = p_hw_i2c->i2c_cr2;

    /** <brief 清零 freq[5:0] */
    tmpreg &= 0xFFC0;

    /** <brief 根据 pclk1  设置freq时钟 */
    pclk1 = clk_rate;

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

        /** \brief 写入CR1 */
        p_hw_i2c->i2c_cr1 = tmpreg;

        /** \brief I2C OAR1 配置 */
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
 * \brief I2C 主机启动函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 * \param[in] p_trans  指向 I2C 传输结构体的指针
 *
 * \return 无
 */
static void __i2c_mst_start (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                             i2c_transfer_t    *p_trans,
                             uint32_t           clk_rate)
{
    if(!(p_trans->flags & I2C_M_NOSTART)) {

        __i2c_mst_init (p_hw_i2c, I2C_SPEED, clk_rate);
    }

    /* 产生起始条件 切换至主模式 */
    amhw_stm32f103rbt6_i2c_genstrat(p_hw_i2c, ENABLE);

    /* EV5事件 ： 等待SB */
    while (!amhw_stm32f103rbt6_i2c_checkevent(p_hw_i2c,
            I2C_EVENT_MASTER_MODE_SELECT ));

    /* 清除SB标志位 */
    amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);

    /* 发送从机地址 */
    if ( p_trans->flags & I2C_M_RD) {
        /* 读*/
        amhw_stm32f103rbt6_i2c_send7bit_address(
            p_hw_i2c, p_trans->addr, 1);

    } else {
        /* 写 */
        amhw_stm32f103rbt6_i2c_send7bit_address(
            p_hw_i2c, p_trans->addr, 0);
    }

    /* 等待EV6事件标志位 */
    while (1) {
        if (p_hw_i2c->i2c_sr1 & 0x0002) {

            /* 清除addr标志位 */
            amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr1);
            amhw_stm32f103rbt6_i2c_read_reg(p_hw_i2c->i2c_sr2);
            break;
        }
    }
}
/**
 * \brief 例程入口
 */
void demo_stm32f103rbt6_hw_i2c_master_poll_entry (amhw_stm32f103rbt6_i2c_t *p_hw_i2c,
                                           uint32_t           clk_rate)
{
    i2c_transfer_t *p_trans              = &__g_i2c1_transfer;
    uint8_t         eeprom_buf[16]       = {0};
    uint8_t         test_addr[2]         = {0x02};
    uint8_t         i;

    __i2c_mst_init(p_hw_i2c, I2C_SPEED, clk_rate);   /* I2C 主机初始化配置 */

     /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_WR | I2C_M_NOSTART),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans, clk_rate);
    __i2c_mst_send (p_hw_i2c, p_trans, AM_FALSE);

    for (i = 0;i < TEST_LEN; i++) {
        eeprom_buf[i] = i + 0xc;
    }

     /* 设置 transfer 结构体参数，写入数据 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_NOSTART | I2C_M_WR),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mst_start(p_hw_i2c, p_trans, clk_rate);
    __i2c_mst_send(p_hw_i2c, p_trans, AM_TRUE);

    am_mdelay(10);

    /* 设置 transfer 结构体参数，写入地址 */
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_WR | I2C_M_NOSTART),
                  (uint8_t *)test_addr,
                  1);

    __i2c_mst_start(p_hw_i2c, p_trans, clk_rate);
    __i2c_mst_send (p_hw_i2c, p_trans, AM_FALSE);


   /* 设置 transfer 结构体参数，读取数据 */
    for (i = 0; i < TEST_LEN; i++) {
        eeprom_buf[i] = 0;
    }
    __i2c_mktrans(p_trans,
                  EEPROM_ADDR,
                  (I2C_M_7BIT | I2C_M_RD | I2C_M_NOSTART),
                  (uint8_t *)eeprom_buf,
                  TEST_LEN);

    __i2c_mst_start(p_hw_i2c, p_trans, clk_rate);
    __i2c_mst_recv(p_hw_i2c, p_trans, AM_TRUE);

    am_mdelay(10);

    /* 校验写入和读取的数据是否一致 */
    for (i = 0;i < TEST_LEN; i++) {
        am_kprintf("Read EEPROM the %2dth data is  : 0x%02x\r\n", i, eeprom_buf[i]);

        /* 校验失败 */
        if(eeprom_buf[i] != (i + 0xc)) {
            am_kprintf("verify failed!\r\n");
            break;
        }
    }

    if (TEST_LEN == i) {
        am_kprintf("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_zlg116_hw_i2c_master_poll] */

/* end of file */
