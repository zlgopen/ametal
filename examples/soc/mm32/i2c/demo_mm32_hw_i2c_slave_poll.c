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
 * \snippet demo_mm32l073_hw_i2c_slave_poll.c src_mm32l073_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  sdy, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_mm32l073_hw_i2c_slave_poll
 * \copydoc demo_mm32l073_hw_i2c_slave_poll.c
 */

/** [src_mm32l073_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_mm32_i2c.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** 主机操作从机地址定义 */
#define OPERATE_ADDR      0x50

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
static int __i2c_slave_init (amhw_mm32_i2c_t *p_hw_i2c, uint8_t dev_addr)
{
    if (p_hw_i2c == NULL) {
        return -AM_EINVAL;
    }

    /* 关闭 I2C 控制器，配置参数 */
    amhw_mm32_i2c_disable(p_hw_i2c);

    amhw_mm32_i2c_rx_tl_set(p_hw_i2c, 0);
    amhw_mm32_i2c_tx_tl_set(p_hw_i2c, 1);

    amhw_mm32_i2c_slave_addr_set (p_hw_i2c, dev_addr);

    p_hw_i2c->ic_con = AMHW_MM32_I2C_SLAVE |
                       AMHW_MM32_I2C_7BITADDR_SLAVE;

    amhw_mm32_i2c_intr_mask_clear (p_hw_i2c, 0xfff);

    amhw_mm32_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief I2C 状态机函数
 *
 * \param[in] p_hw_i2c 指向 I2C 寄存器块的指针
 *
 * \return 无
 */

static void __i2c_slave_event (amhw_mm32_i2c_t *p_hw_i2c)
{
    static uint8_t status = __I2C_SLV_ST_IDLE;

    switch (status) {

    /* 空闲状态 */
    case __I2C_SLV_ST_IDLE:
        if (p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_START_DET) {
            amhw_mm32_i2c_clr_start_det_get(p_hw_i2c);
        } else {
            break;
        }
        /* no break */

     /* 地址匹配状态 */
    case __I2C_SLV_ST_ADDR_MATCH:

        if( p_hw_i2c->ic_status & AMHW_MM32_STATUS_FLAG_SLV_ACTIVITY) {
            status = __I2C_SLV_ST_SUBADDR_GET;
            sub_addr = 0;
        } else {
            status = __I2C_SLV_ST_IDLE;
        }
        break;

    /* 获取子地址状态  */
    case __I2C_SLV_ST_SUBADDR_GET:
        if ( is_sub_addr ==  AM_TRUE ) {
            if( p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_RX_FULL ) {
                sub_addr =  amhw_mm32_i2c_dat_read (p_hw_i2c);
                is_sub_addr = AM_FALSE;
            }
        } else {
            if (p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_RD_REQ ) {
                amhw_mm32_i2c_clr_rd_req_get(p_hw_i2c);
                status = __I2C_SLV_ST_TXBYTE_GET;
                is_sub_addr = AM_TRUE;
            } else if (p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_RX_FULL ){
                status = __I2C_SLV_ST_RXBYTE_PUT;
                is_sub_addr = AM_TRUE;
            }
        }
        break;

    /* 获取发送的数据状态  */
    case __I2C_SLV_ST_TXBYTE_GET:
        if (p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_TX_EMPTY ) {
            amhw_mm32_i2c_dat_write (p_hw_i2c, eeprom[sub_addr++]);
            sub_addr %= EEPROM_BUFFER_LEN;
        }

        if(p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_STOP_DET) {
            amhw_mm32_i2c_clr_stop_det_get(p_hw_i2c);
            status = __I2C_SLV_ST_STOP;
        }

        break;

    /* 接受数据 状态  */
    case __I2C_SLV_ST_RXBYTE_PUT:
        if ( p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_RX_FULL ) {
            eeprom[sub_addr++] = amhw_mm32_i2c_dat_read (p_hw_i2c);
            sub_addr %= EEPROM_BUFFER_LEN;
        }
        if(p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_STOP_DET) {
            amhw_mm32_i2c_clr_stop_det_get(p_hw_i2c);
            status = __I2C_SLV_ST_STOP;
        }
        break;

    /* 停止状态 */
    case __I2C_SLV_ST_STOP:

        /** 若主机不需接受数据时 ，缓存区可能还有数据，发送终止将置1,及时清除 */
        if ( p_hw_i2c->ic_raw_intr_stat & AMHW_MM32_INT_FLAG_TX_ABRT ) {
            amhw_mm32_i2c_clr_tx_abrt_get(p_hw_i2c);
        }
        amhw_mm32_i2c_intr_mask_clear(p_hw_i2c, AMHW_MM32_INT_FLAG_TX_EMPTY);

        status = __I2C_SLV_ST_IDLE;
        break;

    default:
        break;
    }
}

/**
 * \brief 例程入口
 */
void demo_mm32_hw_i2c_slave_poll_entry (amhw_mm32_i2c_t *p_hw_i2c)
{

    __i2c_slave_init(p_hw_i2c, OPERATE_ADDR); /* I2C 从机初始化配置 */

    while(1) {
        __i2c_slave_event(p_hw_i2c);
    }
}
/** [src_mm32l073_hw_i2c_slave_poll] */

/* end of file */
