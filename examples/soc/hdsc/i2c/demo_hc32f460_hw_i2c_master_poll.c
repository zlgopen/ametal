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
 * \brief I2C 主机轮询例程，通过 HW 层接口实现
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
#define BUFF_DATA_LEN                   8u
#define TIMEOUT                         ((uint32_t)0x10000)

static uint8_t u8TxBuf[BUFF_DATA_LEN];
static uint8_t u8RxBuf[BUFF_DATA_LEN];

static uint8_t __master_start_or_restart(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t u8Start)
{
    uint32_t u32TimeOut = TIMEOUT;
    en_i2c_flag_status_t enFlagBusy = I2C_RESET;
    en_i2c_flag_status_t enFlagStartf = I2C_RESET;

    /* generate start or restart signal */
    if(!u8Start)
    {
        /* Wait I2C bus idle */
        while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        amhw_hc32f460_i2c_generate_start(p_hw_i2c , I2C_ENABLE);
    }
    else
    {
        /* Clear start status flag */
        amhw_hc32f460_i2c_clear_status(p_hw_i2c, I2C_CLR_STARTFCLR);
        /* Send restart condition */
        amhw_hc32f460_i2c_generate_restart(p_hw_i2c , I2C_ENABLE);
    }

    /* Judge if start success*/
    u32TimeOut = TIMEOUT;
    while(1)
    {
        enFlagBusy = amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY);
        enFlagStartf = amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_STARTF);
        if(enFlagBusy && enFlagStartf)
        {
            break;
        }
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

uint8_t __master_stop(amhw_hc32f460_i2c_t *p_hw_i2c)
{
    uint32_t u32TimeOut;

    /* Wait I2C bus busy */
    u32TimeOut = TIMEOUT;
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_BUSY))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    amhw_hc32f460_i2c_generate_stop(p_hw_i2c, I2C_ENABLE);

    /* Wait STOPF */
    u32TimeOut = TIMEOUT;
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_STOPF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }
    return AM_TRUE;
}

static uint8_t __master_send_addr(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t u8Adr)
{
    uint32_t u32TimeOut = TIMEOUT;

    /* Wait tx buffer empty */
    while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TEMPTYF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    /* Send I2C address */
    amhw_hc32f460_i2c_send_data(p_hw_i2c, u8Adr);

    if(!(u8Adr & 0x01u))     
    {
        /* If in master transfer process, Need wait transfer end*/
        uint32_t u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TENDF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }
    }

    /* Check ACK */
    u32TimeOut = TIMEOUT;
    while(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_NACKDETECTF))
    {
        if(0ul == (u32TimeOut--))
        {
            return AM_FALSE;
        }
    }

    return AM_TRUE;
}

static uint8_t __master_read_data(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t *pRxData, uint32_t u32Size)
{
    uint32_t u32TimeOut = TIMEOUT;

    for(uint32_t i=0ul; i<u32Size; i++)
    {
        /* if the last byte receive, need config NACK*/
        if(i == (u32Size - 1ul))
        {
            amhw_hc32f460_i2c_nack_config(p_hw_i2c, I2C_ENABLE);
        }

        /* Wait receive full flag*/
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_RFULLF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

        /* read data from register*/
        *pRxData++ = amhw_hc32f460_i2c_dat_read(p_hw_i2c);
    }

    return AM_OK;
}

static uint8_t __master_write_data(amhw_hc32f460_i2c_t *p_hw_i2c, uint8_t *pTxData, uint32_t u32Size)
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
        amhw_hc32f460_i2c_send_data(p_hw_i2c, *pTxData++);

        /* Wait transfer end*/
        u32TimeOut = TIMEOUT;
        while(I2C_RESET == amhw_hc32f460_i2c_get_status(p_hw_i2c, I2C_SR_TENDF))
        {
            if(0ul == (u32TimeOut--))
            {
                return AM_FALSE;
            }
        }

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
void demo_hc32f460_hw_i2c_master_poll_entry (void *p_hw_i2c_addr, 
                                       uint32_t clk_id, 
                                       int      master_speed,
                                       int      master_addr)
{
    stc_i2c_init_t stcI2cInit;
    uint32_t pclk3Freq;    
    uint32_t i;
    amhw_hc32f460_i2c_t *p_hw_i2c = (amhw_hc32f460_i2c_t *)p_hw_i2c_addr;

    for(i = 0ul; i < BUFF_DATA_LEN; i++)
    {
        u8TxBuf[i] = (uint8_t)(i+10ul);
    }

    am_clk_enable(clk_id);

    amhw_hc32f460_i2c_deinit(p_hw_i2c);
    
    /* Get system clock frequency */
    pclk3Freq =  am_clk_rate_get(clk_id);
    
    stcI2cInit.enI2cMode = I2cMaster;
    stcI2cInit.u32Pclk3 = pclk3Freq;
    stcI2cInit.u32Baudrate = master_speed;
    stcI2cInit.u32SclTime = 0ul;
    amhw_hc32f460_i2c_init(p_hw_i2c, &stcI2cInit);
    amhw_hc32f460_i2c_enable(p_hw_i2c);

    /* I2C master data write*/
    __master_start_or_restart(p_hw_i2c, GENERATE_START);
    __master_send_addr(p_hw_i2c, (uint8_t)(master_addr << 1u) | ADDRESS_W);
    __master_write_data(p_hw_i2c, u8TxBuf, BUFF_DATA_LEN);
    __master_stop(p_hw_i2c);

    /* I2C master data read*/
    __master_start_or_restart(p_hw_i2c, GENERATE_START);
    __master_send_addr(p_hw_i2c, (uint8_t)(master_addr << 1u) | ADDRESS_R);
    __master_read_data(p_hw_i2c, u8RxBuf, BUFF_DATA_LEN);
    __master_stop(p_hw_i2c);

    /* Compare the data */
    for(i = 0ul; i < BUFF_DATA_LEN; i++)
    {
        if(u8TxBuf[i] != u8RxBuf[i])
        {           
            /* Data write error*/
            am_kprintf("u8TxBuf[%d] != u8RxBuf[%d] error!\r\n", i, i);
            break;
        }
    }
    if (BUFF_DATA_LEN == i)
    {
        am_kprintf("i2c master success!\r\n");
    }
    
}
/** [src_hc32f460_hw_i2c_slave_poll] */

/* end of file */
