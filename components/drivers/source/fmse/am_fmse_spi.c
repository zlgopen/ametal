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
 * \brief 安全芯片FMSE-A03 SPI 驱动文件
 *
 * \internal
 * \par Modification history
 * - 1.00 20-07-02  yrz, first implementation.
 * \endinternal
 */
#include "am_fmse_a03.h"
#include "am_fmse_spi.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_wait.h"

/*******************************************************************************
 * 宏定义
 ******************************************************************************/
#define SPI_CMD_SEND_DATA		0x02
#define SPI_CMD_RECV_DATA		0x03
#define SPI_CMD_CHK_STATE		0x05

#define SPI_CMD_DELAY			16

#define SPI_MIN_LEN				2
#define SPI_MAX_LEN				1024

/*******************************************************************************
 * 本地函数声明
 ******************************************************************************/
static uint8_t __fmse_spi_transceive (void     *p_drv,
                                      uint8_t  *sbuf,
                                      uint16_t  slen,
                                      uint8_t  *rbuf,
                                      uint16_t *rlen, 
                                      uint16_t  poll_inv,
                                      uint32_t  poll_timeout);
/*******************************************************************************
  Local functions
*******************************************************************************/
const struct am_fmse_drv_funcs __fmse_spi_drv_funcs = {
	__fmse_spi_transceive
};

/**
 * \brief SPI 消息完成回调函数
 */
static void __message_complete (void *p_arg)
{
    am_wait_t *p_wait = (am_wait_t *)p_arg;

    /* 结束等待序列 */
    am_wait_done(p_wait);
}

/**
 * \brief 安全芯片状态获取
 *
 * \param[in] p_drv   : SPI驱动的 FMSE设备
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_state_get (void *p_drv)
{
    am_spi_device_t *p_dev = (am_spi_device_t*)(p_drv);

    am_spi_transfer_t trans[2];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;    

    uint8_t cmd   = SPI_CMD_CHK_STATE;
    uint8_t temp  = 0xff;
    uint8_t state;

    am_wait_init(&msg_wait);

    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);
    /* send cmd */
    am_spi_mktrans(&trans[0],
                   &cmd,
                    NULL,
                    1,
                    0,
                    0,
                    SPI_CMD_DELAY,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[0]);

    /* recieve length */
    am_spi_mktrans(&trans[1],
                   &temp,
                   &state,
                    1,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[1]);

    am_spi_msg_start(p_dev, &spi_msg);

    am_wait_on(&msg_wait);

    return state;
}

/**
 * \brief 安全芯片状态查询
 *
 * \param[in] p_drv   : SPI驱动的 FMSE设备
 * \param[in] inv     : 轮询间隔
 * \param[in] timeout : 超时时间
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_chk_state(void *p_drv, uint16_t inv, uint32_t timeout)
{
    am_spi_device_t *p_dev = (am_spi_device_t*)(p_drv);

    uint8_t state = 1; 

    /* 轮询间隔强制为1~9毫秒 */
    inv = ((inv == 0) || (inv > 9)) ? 1 : inv;

    /* chk state and wait se ok */
    do {
        state = __fmse_spi_state_get(p_dev);
        
        if(state) {
            am_mdelay(inv);
            timeout -= inv;
        } else {
            break;
        }
    } while (timeout > 10);    /* 预留10ms余量 */

    return state;
}

/**
 * \brief SPI帧数据发送
 *
 * \param[in] p_drv   : SPI驱动的 FMSE设备
 * \param[in] inv     : 轮询间隔
 * \param[in] timeout : 超时时间
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_frame_send (void *p_drv, uint8_t *sbuf, uint16_t slen)
{
    am_spi_device_t *p_dev = (am_spi_device_t*)(p_drv);

    am_spi_transfer_t trans[4];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;  

    const uint8_t cmd = SPI_CMD_SEND_DATA;
    uint16_t i;
    uint8_t  lrc;
    uint8_t  len[2];

    /* length */
    len[0] = slen >> 8;
    len[1] = slen;

    /* calc lrc */
    lrc = 0xFF^len[0]^len[1];

    /* send frame */
    for(i = 0; i < slen; i++) {
        lrc ^= sbuf[i];
    }

    am_wait_init(&msg_wait);

    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);

    /* send cmd */
    am_spi_mktrans(&trans[0],
                   &cmd,
                    NULL,
                    1,
                    0,
                    0,
                    SPI_CMD_DELAY,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[0]);

    /* send length */
    am_spi_mktrans(&trans[1],
                   &len[0],
                    NULL,
                    2,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[1]);

    /* send data */
    am_spi_mktrans(&trans[2],
                   &sbuf[0],
                    NULL,
                    slen,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[2]);

    /* send lrc */
    am_spi_mktrans(&trans[3],
                   &lrc,
                    NULL,
                    1,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[3]);

    am_spi_msg_start(p_dev, &spi_msg);

    am_wait_on(&msg_wait);

    return 0;
}

/**
 * \brief SPI帧数据发送
 *
 * \param[in] p_drv   : SPI驱动的 FMSE设备
 * \param[in] inv     : 轮询间隔
 * \param[in] timeout : 超时时间
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_frame_recv (void     *p_drv,
                                      uint8_t  *rbuf,
                                      uint16_t *rlen)
{
    am_spi_device_t *p_dev = (am_spi_device_t*)(p_drv);

    am_spi_transfer_t trans[4];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;    

    const uint8_t cmd = SPI_CMD_RECV_DATA;
    uint16_t i;
    uint8_t  temp;
    uint8_t  lrc;
    uint8_t  len[2];

    am_wait_init(&msg_wait);

    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);

    /* send cmd */
    am_spi_mktrans(&trans[0],
                   &cmd,
                    NULL,
                    1,
                    0,
                    0,
                    SPI_CMD_DELAY,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[0]);

    /* recieve length */
    am_spi_mktrans(&trans[1],
                    NULL,
                   &len[0],
                    2,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[1]);

    /* recieve data */
    am_spi_mktrans(&trans[2],
                    NULL,
                   &rbuf[0],
                   *rlen,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[2]);

    /* recieve lrc */
    am_spi_mktrans(&trans[3],
                    NULL,
                   &temp,
                    1,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[3]);

    am_spi_msg_start(p_dev, &spi_msg);

    am_wait_on(&msg_wait);

    /* 帧长度 */
    *rlen = len[0] << 8 | len[1];

    /* 计算校验 */
    lrc = 0xFF^len[0]^len[1];

    for (i = 0; i < *rlen; i++) {
        lrc ^= rbuf[i];
    }

    lrc ^= temp;

    if (lrc) {
        *rlen = 0;
        return 14;
    }

    return 0;
}

/**
 * \brief SPI帧长度获取
 *
 * \param[in] p_drv   : SPI驱动的 FMSE设备
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_frame_length_get (void     *p_drv,
                                            uint16_t *rlen)
{
    am_spi_device_t *p_dev = (am_spi_device_t*)(p_drv);

    am_spi_transfer_t trans[2];
    am_wait_t         msg_wait;
    am_spi_message_t  spi_msg;    

    const uint8_t cmd = SPI_CMD_RECV_DATA;
    uint8_t len[2];

    am_wait_init(&msg_wait);

    am_spi_msg_init(&spi_msg,
                    __message_complete,
                    (void *)&msg_wait);

    /* send cmd */
    am_spi_mktrans(&trans[0],
                &cmd,
                    NULL,
                    1,
                    0,
                    0,
                    SPI_CMD_DELAY,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[0]);

    /* recieve length */
    am_spi_mktrans(&trans[1],
                    NULL,
                   &len[0],
                    2,
                    0,
                    0,
                    0,
                    0,
                    0);

    am_spi_trans_add_tail(&spi_msg, &trans[1]);
    
    am_spi_msg_start(p_dev, &spi_msg);
    
    am_wait_on(&msg_wait);
    
    *rlen = len[0] << 8 | len[1];

    return 0;
}

/**
 * \brief 安全芯片发送接收帧
 *
 * \param[in] p_drv        : I2C驱动的 FMSE设备
 * \param[in] sbuf         : 待发送数据缓冲区
 * \param[in] slen         : 待发送数据长度
 * \param[in] rbuf         : 接收数据缓冲区
 * \param[in] rlen         : 接收数据长度
 * \param[in] poll_inv     : 帧间延时
 * \param[in] poll_timeout : 超时时间
 *
 * \return 状态值
 */
static uint8_t __fmse_spi_transceive (void     *p_drv,
                                      uint8_t  *sbuf,
                                      uint16_t  slen,
                                      uint8_t  *rbuf,
                                      uint16_t *rlen, 
                                      uint16_t  poll_inv,
                                      uint32_t  poll_timeout)
{
    am_fmse_spi_dev_t *p_this = (am_fmse_spi_dev_t*)p_drv;
    am_spi_device_t   *p_dev = (am_spi_device_t*)(&p_this->spi_dev);

    uint8_t state;

    if(p_dev == NULL) {
        return 11;
    }

    __fmse_spi_frame_send(p_dev, sbuf, slen);

    /* chk se state */
    if (__fmse_spi_chk_state(p_dev, poll_inv, poll_timeout)) {
        return 12;
    }

    __fmse_spi_frame_length_get(p_dev, rlen);

    if (*rlen < SPI_MIN_LEN || *rlen > SPI_MAX_LEN) {
        *rlen = 0;
        return 13;
    }

    state = __fmse_spi_frame_recv(p_dev, rbuf, rlen);

    return state;
}

/**
 * \brief SPI驱动 FMSE的设备初始化
 *
 * \param[in] p_dev     : SPI驱动的 FMSE设备
 * \param[in] p_devinfo : FMSE设备信息
 * \param[in] handle    : SPI标准服务句柄
 *
 * \return FMSE标准服务句柄，若为NULL，表明初始化失败
 */
am_fmse_handle_t am_fmse_spi_init (am_fmse_spi_dev_t           *p_dev,
                                   const am_fmse_spi_devinfo_t *p_devinfo,
                                   am_spi_handle_t              handle)
{
    if ((p_dev == NULL) || (handle == NULL) || (p_devinfo == NULL)) {
        return NULL;
    }

    p_dev->fmse_dev.p_drv   = p_dev;
    p_dev->fmse_dev.p_funcs = &__fmse_spi_drv_funcs;
    p_dev->dev_info         = p_devinfo;

    am_spi_mkdev(&(p_dev->spi_dev),
                  handle,
                  8,
                  AM_SPI_MODE_3,
                  p_devinfo->speed,
                  p_devinfo->cs_pin,
                  NULL);

    if (am_spi_setup(&(p_dev->spi_dev)) < 0) {
        return NULL;
    }

    return (&(p_dev->fmse_dev));
}

/**
 * \brief FMSE的设备去初始化
 *
 * \param[in] handle : FMSE初始化获取的标准服务句柄
 *
 * \return 
 */
am_err_t am_fmse_spi_deinit (am_fmse_handle_t handle)
{
    am_fmse_spi_dev_t *p_dev = handle->p_drv;

    if (handle == NULL) {
        return -AM_EINVAL;
    }

    p_dev->fmse_dev.p_drv   = NULL;
    p_dev->fmse_dev.p_funcs = NULL;
    p_dev->dev_info         = NULL;

    return AM_OK;
}

/* end of file */
