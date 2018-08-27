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
 * \brief I2C implementation
 * 
 * \internal
 * \par Modification history
 * - 1.02 15-10-27  tee, add the concept of message.
 * - 1.01 15-08-19  tee, modified some interface.
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#include "am_i2c.h"
#include "am_wait.h"

/******************************************************************************/

static void __i2c_callback (void *p_arg)
{
    /* 结束等待序列 */
    am_wait_done((am_wait_t *)p_arg);
}

/******************************************************************************/

static int __i2c_rw_sync (am_i2c_device_t *p_dev,
                          uint32_t         sub_addr,
                          uint8_t         *p_buf,
                          uint32_t         nbytes,
                          am_bool_t        is_read)
{
    uint16_t subaddr_len = AM_I2C_SUBADDR_LEN_GET(p_dev->dev_flags);
    
    am_i2c_transfer_t trans[3];
    am_wait_t         trans_wait;
    am_i2c_message_t  msg;
    uint8_t           subaddr_buf[2];  /* 用于存放2bytes期间内子地址 */
    int               ret;
    
    am_wait_init(&trans_wait);
    
    /* if no sub address just send the data */
    if (subaddr_len == 0) {
        
        am_i2c_mktrans(&trans[0],
                       p_dev->dev_addr,
                       p_dev->dev_flags |
                       ((is_read == AM_TRUE) ? AM_I2C_M_RD : AM_I2C_M_WR),
                       p_buf, 
                       nbytes);
        
        am_i2c_mkmsg( &msg, &trans[0], 1, __i2c_callback, &trans_wait);
                       
                       
    } else {
        
        /* one byte sub address */
        if (subaddr_len == 1) {
             subaddr_buf[0] = (uint8_t)sub_addr;

        /* two byte byte address */
        } else if (subaddr_len == 2) {

            if (p_dev->dev_flags & AM_I2C_SUBADDR_LSB_FIRST) {
                subaddr_buf[0] = (uint8_t)(sub_addr & 0xFF);
                subaddr_buf[1] = (uint8_t)(sub_addr >> 8);
            } else {
                subaddr_buf[0] = (uint8_t)(sub_addr >> 8);
                subaddr_buf[1] = (uint8_t)(sub_addr & 0xFF);
            }

        /* this case can't happen */
        } else {
            return -AM_ENOTSUP;
        }
        

        am_i2c_mktrans(&trans[0],
                       p_dev->dev_addr,
                       p_dev->dev_flags | AM_I2C_M_WR,
                       &subaddr_buf[0], 
                       subaddr_len);
        
        am_i2c_mktrans(&trans[1],
                       p_dev->dev_addr,
                       p_dev->dev_flags | 
                       ((is_read == AM_TRUE) ? \
                       AM_I2C_M_RD : (AM_I2C_M_WR | AM_I2C_M_NOSTART)),
                       p_buf, 
                       nbytes);
        
        am_i2c_mkmsg( &msg, &trans[0], 2, __i2c_callback, &trans_wait);
    }
    
    ret = am_i2c_msg_start(p_dev->handle, &msg);
    
    if (ret != AM_OK) {
        return ret;
    }
    
    am_wait_on(&trans_wait);
    
    return msg.status;
}

/******************************************************************************/

/**
 * \brief I2C write operate
 */
int am_i2c_write (am_i2c_device_t *p_dev,
                  uint32_t         sub_addr,
                  const uint8_t   *p_buf, 
                  uint32_t         nbytes)
{
    return __i2c_rw_sync(p_dev,
                         sub_addr,
                         (void *)p_buf,
                         nbytes,
                         AM_FALSE);
}

/******************************************************************************/

/**
 * \brief I2C read operate
 */
int am_i2c_read (am_i2c_device_t *p_dev,
                 uint32_t         sub_addr,
                 uint8_t         *p_buf, 
                 uint32_t         nbytes)
{
    return __i2c_rw_sync(p_dev,
                         sub_addr,
                         p_buf,
                         nbytes,
                         AM_TRUE);
}

/* end of file */
