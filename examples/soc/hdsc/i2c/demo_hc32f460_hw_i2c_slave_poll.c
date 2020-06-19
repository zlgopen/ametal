/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2019 Guangzhou ZHIYUAN Electronics Co., Ltd.
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
 *   1. PIOA_11 引脚连接 I2C 主机的 SCL 引脚；
 *   2. PIOA_12 引脚连接 I2C 主机的 SDA 引脚。
 *
 * \note
 *    当前 I2C 的 SCL 引脚使用的是 PIOA_11，SDA 引脚使用的是 PIOA_12，
 *    可根据实际情况更换引脚。
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_i2c_slave_poll.c src_hc32f460_hw_i2c_slave_poll
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-16  licl, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_i2c_slave_poll
 * \copydoc demo_hc32f460_hw_i2c_slave_poll.c
 */

/** [src_hc32f460_hw_i2c_slave_poll] */
#include "ametal.h"
#include "am_vdebug.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "am_int.h"
#include "hc32f460_clk.h"
#include "am_hc32f460_clk.h"

/*******************************************************************************
  宏定义
*******************************************************************************/

/** 主机操作从机地址定义 */
#define BUFF_DATA_LEN                   32u
#define TIMEOUT                         ((uint32_t)0x10000)

static uint8_t u8RxBuf[BUFF_DATA_LEN];

static uint8_t slave_revc_data(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t *pu8RxData)
{
    uint8_t i = 0u;

    while(1)
    {
        /* Detect the stop signal on the bus */
        if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_STOPF))
        {
            amhw_hc32f460_i2c_clear_status(p_hw_i2c, I2C_SR_STOPF);
            return AM_OK;
        }

        /* Wait for the Rx full flag set */
        if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_RFULLF))
        {
            /* Read the data from buffer */
            pu8RxData[i++] = amhw_hc32f460_i2c_dat_read(p_hw_i2c);
        }
    }
}

static uint8_t slave_write_data(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t *pTxData, uint32_t u32Size)
{
    uint32_t u32TimeOut = TIMEOUT;

    while(u32Size--)
    {
        /* Wait tx buffer empty */
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TEMPTYF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        /* Send one byte data */
        amhw_hc32f460_i2c_dat_write(p_hw_i2c, *pTxData++);

        /* Check ACK */
        u32TimeOut = TIMEOUT;
        while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_NACKDETECTF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }
    }

    return AM_OK;
}

/**
 * \brief 例程入口
 */
void demo_hc32f460_hw_i2c_slave_poll_entry (void *p_hw_i2c_addr, 
                                       uint32_t clk_id, 
                                       int      slave_speed,
                                       int      slave_addr)
{
    stc_i2c_init_t stcI2cInit;
    uint32_t pclk3Freq;

    amhw_hc32f460_i2c_t *p_hw_i2c = (amhw_hc32f460_i2c_t *)p_hw_i2c_addr;

    am_clk_enable(clk_id);

    amhw_hc32f460_i2c_deinit(p_hw_i2c);
    
    /* Get system clock frequency */
    pclk3Freq =  am_clk_rate_get(clk_id);
    
    stcI2cInit.enI2cMode = I2cSlave;
    stcI2cInit.u32Pclk3 = pclk3Freq;
    stcI2cInit.u32Baudrate = slave_speed;
    stcI2cInit.u32SclTime = 0ul;
    amhw_hc32f460_i2c_init(p_hw_i2c, &stcI2cInit);
    amhw_hc32f460_i2c_enable(p_hw_i2c);

    amhw_hc32f460_i2c_slave_addr0_config(p_hw_i2c, I2C_ENABLE, Adr7bit, slave_addr);

    while(1) {
        /* Wait salve address matched*/
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_SLADDR0F))
        {
            ;
        }

        amhw_hc32f460_i2c_clear_status(p_hw_i2c, I2C_CLR_SLADDR0FCLR);

        if(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TRA))
        {
            /* Slave receive data*/
            slave_revc_data(p_hw_i2c, u8RxBuf);
        }
        else
        {
            /* Slave send data*/
            slave_write_data(p_hw_i2c, u8RxBuf, BUFF_DATA_LEN);
            break;
        }
    }
}
/** [src_hc32f460_hw_i2c_slave_poll] */

/* end of file */
