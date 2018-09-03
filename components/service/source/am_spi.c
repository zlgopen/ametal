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
 * \brief SPI 读/写函数实现
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#include "am_spi.h"
#include "am_wait.h"

/**
 * \brief SPI消息完成回调函数
 */
static void __message_complete (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;
     
    /* 结束等待序列 */
    am_wait_done(p_wait);
}

/**
 * \brief 先写后读
 */
int am_spi_write_then_read (am_spi_device_t *p_dev,
                            const uint8_t   *p_txbuf,
                            size_t           n_tx,
                            uint8_t         *p_rxbuf,
                            size_t           n_rx)
{
    am_spi_transfer_t trans[2];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    if(p_txbuf == NULL && p_rxbuf == NULL) {
        return AM_OK;
    }
    
    am_wait_init(&msg_wait);
    
    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);
    
    if ((p_txbuf != NULL) && (n_tx != 0)) {
        
        am_spi_mktrans(&trans[0], 
                        p_txbuf,
                        NULL,
                        n_tx,
                        0,
                        0,
                        0,
                        0,
                        0);
        
        am_spi_trans_add_tail(&spi_msg, &trans[0]);
    }
    
    if ((p_rxbuf != NULL) && (n_rx != 0)) {
        
        am_spi_mktrans(&trans[1], 
                        NULL,
                        p_rxbuf,
                        n_rx,
                        0,
                        0,
                        0,
                        0,
                        0);
        
        am_spi_trans_add_tail(&spi_msg, &trans[1]);
    }
    
    am_spi_msg_start(p_dev, &spi_msg);
    
    am_wait_on(&msg_wait);

    return spi_msg.status;
}

/**
 * \brief 执行两次写操作
 */
int am_spi_write_then_write (am_spi_device_t *p_dev,
                             const uint8_t   *p_txbuf0,
                             size_t           n_tx0,
                             const uint8_t   *p_txbuf1,
                             size_t           n_tx1)
{
    am_spi_transfer_t trans[2];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;
    
    if (p_dev == NULL) {
        return -AM_EINVAL;
    }
    
    if(p_txbuf0 == NULL && p_txbuf1 == NULL) {
        return AM_OK;
    }
    
    am_wait_init(&msg_wait);
    
    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);
    
    if ((p_txbuf0 != NULL) && (n_tx0 != 0)) {
        
        am_spi_mktrans(&trans[0], 
                        p_txbuf0,
                        NULL,
                        n_tx0,
                        0,
                        0,
                        0,
                        0,
                        0);
        
        am_spi_trans_add_tail(&spi_msg, &trans[0]);
    }
    
    if ((p_txbuf1 != NULL) && (n_tx1 != 0)) {
        
        am_spi_mktrans(&trans[1], 
                        p_txbuf1,
                        NULL,
                        n_tx1,
                        0,
                        0,
                        0,
                        0,
                        0);
        
        am_spi_trans_add_tail(&spi_msg, &trans[1]);
    }
    
    am_spi_msg_start(p_dev, &spi_msg);
    
    am_wait_on(&msg_wait);

    return spi_msg.status;
}

/* end of file */
