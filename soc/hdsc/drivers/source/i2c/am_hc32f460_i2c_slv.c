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
 * \brief I2C从机驱动，服务I2C从机标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-25
 * \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_hc32f460_i2c_slv.h"
#include "hw/amhw_hc32f460_i2c.h"
#include "hc32f460_irq_handle.h"
#include "hc32f460_intctrl.h"
#include "am_vdebug.h"
/*******************************************************************************
  函数声明
*******************************************************************************/

/** \brief I2C硬件初始化 */
static int __i2c_slv_hard_init(am_hc32f460_i2c_slv_dev_t *p_dev);
/** \brief I2C中断处理函数 */
static void __i2c_slv_irq_handler (void *p_arg);
/** \brief 开启I2C从机设备 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev);
/** \brief 关闭 I2C从机设备 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev);
/** \brief 获取可用的从机设备个数 */
static int __i2c_slv_num_get(void *p_drv);

/**
 * \brief I2C从设备 驱动函数定义
 */
static am_const struct am_i2c_slv_drv_funcs __g_i2c_slv_drv_funcs = {
    __i2c_slv_setup,
    __i2c_slv_shutdown,
    __i2c_slv_num_get,
};

/**
 *  \brief I2C硬件初始化
 *  \note  1: 设置从设备的地址 及地址位数
 *         2：开启中断
 */
static int __i2c_slv_hard_init (am_hc32f460_i2c_slv_dev_t *p_dev)
{
    amhw_hc32f460_i2c_t *p_hw_i2c  = NULL;
    stc_i2c_init_t       stcI2cInit;
    uint32_t             pclk3Freq;

    if (p_dev == NULL) {
        return -AM_EINVAL;
    }

    p_hw_i2c = (amhw_hc32f460_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    amhw_hc32f460_i2c_deinit(p_hw_i2c);
    
    /* Get system clock frequency */
    pclk3Freq =  am_clk_rate_get(p_dev->p_devinfo->clk_num);
    
    stcI2cInit.enI2cMode = I2cSlave;
    stcI2cInit.u32Pclk3 = pclk3Freq;
    stcI2cInit.u32Baudrate = p_dev->p_devinfo->speed;
    stcI2cInit.u32SclTime = 0ul;
    amhw_hc32f460_i2c_init(p_hw_i2c, &stcI2cInit);
    amhw_hc32f460_i2c_enable(p_hw_i2c);

    return AM_OK;
}

/**
 * \brief 开始从机设备
 */
static int __i2c_slv_setup (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    int                        i;
    uint8_t                    zlg_i2c_slv_dev_num ;
    am_hc32f460_i2c_slv_dev_t *p_dev      = NULL;
    amhw_hc32f460_i2c_t       *p_hw_i2c   = NULL;

    
    if ((p_drv == NULL) || (p_i2c_slv_dev == NULL)) {
        return -AM_EINVAL;
    }

    p_dev    = (am_hc32f460_i2c_slv_dev_t *)p_drv;
    p_hw_i2c = (amhw_hc32f460_i2c_t *)(p_dev->p_devinfo->i2c_regbase);

    /* 获取总的从设备个数 */
    zlg_i2c_slv_dev_num = p_dev->zlg_i2c_slv_dev_num;

    for ( i = 0; i < zlg_i2c_slv_dev_num; i++) {
        if( NULL == p_dev->p_i2c_slv_dev[i]) {
            p_dev->p_i2c_slv_dev[i] = p_i2c_slv_dev;
            break ;
        }
    }

    /* 从机设备都用光了，无法开启该从机设备 */
    if ( i == zlg_i2c_slv_dev_num) {
        return -AM_EAGAIN;
    }

    /**
     * \brief 根据  i 存放从机设备地址。
     *         i = 0 ,存放到第1个从地址
     *         i = 1 ,存放到第2从地址 ，以此类推
     *
     * \note ZLG只有一个从机地址 ,直接存放即可 。
     */
    amhw_hc32f460_i2c_slave_addr0_config(p_hw_i2c,
                                         I2C_ENABLE,
                                         Adr7bit,
                                         p_i2c_slv_dev->dev_addr);
    
    /* Config slave address match interrupt function*/
    amhw_hc32f460_i2c_intr_enable(p_hw_i2c, I2C_CR2_SLADDR0EN);
    am_int_enable(p_dev->p_devinfo->inum);

    return AM_OK;
}

/**
 * \brief 关闭 从机设备
 */
static int __i2c_slv_shutdown (void *p_drv, am_i2c_slv_device_t *p_i2c_slv_dev)
{
    am_hc32f460_i2c_slv_dev_t *p_dev    = (am_hc32f460_i2c_slv_dev_t *)p_drv;
    amhw_hc32f460_i2c_t       *p_hw_i2c_slv = NULL;

    if ( (p_dev              == NULL) ||
         (p_i2c_slv_dev      == NULL)) {
        return -AM_EINVAL;
    }
    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *) p_dev->p_devinfo->i2c_regbase;

    /**
     * \brief 失能对应从地址 ,根据从机地址可以确定该关闭哪一个从机设备
     *
     * \note ZLG只能生成一个从设备 ，直接关闭失能I2C即可
     */
    p_dev->p_i2c_slv_dev[0] = NULL;
    am_int_disable(p_dev->p_devinfo->inum);
    amhw_hc32f460_i2c_disable (p_hw_i2c_slv);

    return AM_OK;
}

/**
 * \brief 获取可用从机设备个数
 */
static int __i2c_slv_num_get (void *p_drv)
{
    int     i;
    int     count = 0;
    uint8_t zlg_i2c_slv_dev_num ;
    am_hc32f460_i2c_slv_dev_t *p_dev = (am_hc32f460_i2c_slv_dev_t *)p_drv;

    if ((p_dev == NULL)) {
        return -AM_EINVAL;
    }
    /* 获取总的从设备个数 */
    zlg_i2c_slv_dev_num = p_dev->zlg_i2c_slv_dev_num;

    for (i = 0; i < zlg_i2c_slv_dev_num; i++) {
        if (NULL == p_dev->p_i2c_slv_dev[i]) {
            count++;
        }
    }
    return count;
}

/**
 * \brief 软件定时器回调函数
 *
 * \note 在开始信号之后 ，若主机超过1秒还未发停止信号 ，则将从机进行复位
 */
static void __i2c_slv_timing_callback (void *p_arg)
{
    am_hc32f460_i2c_slv_dev_t  *p_dev     = ( am_hc32f460_i2c_slv_dev_t *) p_arg;
    amhw_hc32f460_i2c_t        *p_hw_i2c  = NULL;
    p_hw_i2c  = (amhw_hc32f460_i2c_t *) (p_dev->p_devinfo->i2c_regbase);

    /* 平台初始化之后将 寄存器的值复位 */
    if (p_dev->p_devinfo->pfn_plfm_init) {
        p_dev->p_devinfo->pfn_plfm_init();
    }

    /* 关闭I2C控制器，配置参数 */
    amhw_hc32f460_i2c_disable (p_hw_i2c);

    /** 判断是否 需要响应 广播地址 */
    if ( p_dev->p_i2c_slv_dev[0]->dev_flags & AM_I2C_SLV_GEN_CALL_ACK) {
        amhw_hc32f460_i2c_gen_call_ack(p_hw_i2c);
    } else {
        amhw_hc32f460_i2c_gen_call_nack(p_hw_i2c);
    }

    amhw_hc32f460_i2c_slave_addr0_config(p_hw_i2c,
                                         I2C_ENABLE,
                                         Adr7bit,
                                         p_dev->p_i2c_slv_dev[0]->dev_addr);

    amhw_hc32f460_i2c_enable(p_hw_i2c);

    __i2c_slv_hard_init(p_dev);

    /** 停止定时 */
    am_softimer_stop(&p_dev->softimer);
}
#define TEST_DATA_LEN                   256u
uint8_t u8RxBuf[TEST_DATA_LEN];
uint32_t u32DataInOffset = 0ul;
uint32_t u32DataOutOffset = 0ul;
uint8_t u8FinishFlag = 0u;

/**
 * \brief I2C 中断处理函数
 *
 * \param[in] p_arg : 指向I2C设备结构体的指针
 *
 * \return 无
 */
static void __i2c_slv_irq_handler (void *p_arg)
{
    static uint8_t             rx_len = 0;
    uint8_t                    rx_data;
    uint8_t                    tx_data;
    amhw_hc32f460_i2c_t       *p_hw_i2c_slv;
    am_hc32f460_i2c_slv_dev_t *p_dev         = (am_hc32f460_i2c_slv_dev_t *)p_arg;
    am_i2c_slv_device_t       *p_i2c_slv_dev = p_dev->p_i2c_slv_dev[0];

    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    /* If NACK interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_NACKDETECTF))
    {
       /* clear NACK flag*/
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_NACKFCLR);

       if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
           p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read完 */
       } 
       
       /* Stop tx or rx process*/
       if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TRA))
       {
           /* Config tx buffer empty interrupt function disable*/
           amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv,
                                          I2C_CR2_TEMPTYIE | I2C_CR2_TENDIE);

           /* Read DRR register to release SCL*/
           amhw_hc32f460_i2c_dat_read(p_hw_i2c_slv);
       }
       else
       {
           /* Config rx buffer full interrupt function disable */
           amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv, I2C_CR2_RFULLIE);
       }
    }

    /* If stop interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_STOPF))
    {
       /* Disable all interrupt enable flag except SLADDR0IE*/
       amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv,
                                      I2C_CR2_TEMPTYIE |
                                      I2C_CR2_TENDIE   |
                                      I2C_CR2_RFULLIE  |
                                      I2C_CR2_STOPIE   |
                                      I2C_CR2_NACKIE);

       /* Clear STOPF flag */
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_STOPFCLR);
       
       if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
           p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read开始 */
       }
       rx_len = 0;
    }


    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_RFULLF))
    {
        // TODO: I2C_ReadData
#if 1       
        rx_data = amhw_hc32f460_i2c_dat_read(p_hw_i2c_slv);//接收数据
            /* 接受回调 */
            if (p_dev->is_gen_call == AM_TRUE) {
                /** 如果是广播呼叫 */
                if(p_i2c_slv_dev->p_cb_funs->pfn_gen_call) {
                    p_i2c_slv_dev->p_cb_funs->pfn_gen_call(p_i2c_slv_dev->p_arg, rx_data);
                }
            } else {
                if(p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put) {
                    p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put(p_i2c_slv_dev->p_arg, rx_data);
                }
            }
        rx_len++;
#endif        
    }

    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TEMPTYF))
    {   
        // TODO: I2C_wirteData
        if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get) {
            p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get(p_i2c_slv_dev->p_arg, &tx_data);
        }
        amhw_hc32f460_i2c_dat_write(p_hw_i2c_slv,tx_data);
    }


    
    /* If start interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_SLADDR0F))
    {
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_SLADDR0FCLR);
       /* Enable Tx or Rx function*/
       if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TRA))
       {
           /* Config tx buffer empty interrupt function*/
           amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv, I2C_CR2_TEMPTYIE | I2C_CR2_TENDIE);
           /* Write the first data to DTR immediately */
           // TODO: I2C_writeData           
           if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get) {
               p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get(p_i2c_slv_dev->p_arg, &tx_data);
           }
           amhw_hc32f460_i2c_dat_write(p_hw_i2c_slv,tx_data);
       }
       else
       {
           /* Config rx buffer full interrupt function*/
           amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv, I2C_CR2_RFULLIE);
       }

       /* Enable stop and NACK interrupt */
       amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv, I2C_CR2_STOPIE | I2C_CR2_NACKIE);
    }
   
}

static uint8_t           rx_len = 0;

void I2C_EEI_Callback(void *p_arg)
{
    uint8_t                    tx_data = 0;
    amhw_hc32f460_i2c_t       *p_hw_i2c_slv;
    am_hc32f460_i2c_slv_dev_t *p_dev = (am_hc32f460_i2c_slv_dev_t *)p_arg;
    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t     *p_i2c_slv_dev  = p_dev->p_i2c_slv_dev[0];

    /* If start interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_SLADDR0F))
    {
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_SLADDR0FCLR);
       /* Enable Tx or Rx function*/
       if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TRA))
       {
           /* Config tx buffer empty interrupt function*/
           amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv,
                                         I2C_CR2_TEMPTYIE | I2C_CR2_TENDIE);
           /* Write the first data to DTR immediately */
           // TODO: I2C_writeData           

           if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get) {
               p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get(p_i2c_slv_dev->p_arg, &tx_data);               
               amhw_hc32f460_i2c_dat_write(p_hw_i2c_slv,tx_data);
           }
       }
       else
       {
           /* Config rx buffer full interrupt function*/
           amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv, I2C_CR2_RFULLIE);
       }

       /* Enable stop and NACK interrupt */
       amhw_hc32f460_i2c_intr_enable(p_hw_i2c_slv, I2C_CR2_STOPIE | I2C_CR2_NACKIE);
    }

   /* If NACK interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_NACKDETECTF))
    {
       /* clear NACK flag*/
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_NACKFCLR);

       if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
           p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read完 */
       } 
       
       /* Stop tx or rx process*/
       if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TRA))
       {
           /* Config tx buffer empty interrupt function disable*/
           amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv,
                                          I2C_CR2_TEMPTYIE | I2C_CR2_TENDIE);

           /* Read DRR register to release SCL*/
           amhw_hc32f460_i2c_dat_read(p_hw_i2c_slv);
       }
       else
       {
           /* Config rx buffer full interrupt function disable */
           amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv, I2C_CR2_RFULLIE);
       }
    }

    /* If stop interrupt occurred */
    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_STOPF))
    {
       /* Disable all interrupt enable flag except SLADDR0IE*/
       amhw_hc32f460_i2c_intr_disable(p_hw_i2c_slv,
                                      I2C_CR2_TEMPTYIE |
                                      I2C_CR2_TENDIE   |
                                      I2C_CR2_RFULLIE  |
                                      I2C_CR2_STOPIE   |
                                      I2C_CR2_NACKIE);

       /* Clear STOPF flag */
       amhw_hc32f460_i2c_clear_status(p_hw_i2c_slv, I2C_CLR_STOPFCLR);
       
       if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_tran_stop) {
           p_i2c_slv_dev->p_cb_funs->pfn_tran_stop(p_i2c_slv_dev->p_arg); /* 主机read开始 */
       }
       rx_len = 0;
    }

}

void I2C_RXI_Callback(void *p_arg)
{
    uint8_t                  rx_data = 0;    
    amhw_hc32f460_i2c_t       *p_hw_i2c_slv;
    am_hc32f460_i2c_slv_dev_t    *p_dev          = (am_hc32f460_i2c_slv_dev_t *)p_arg;
    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t     *p_i2c_slv_dev  = p_dev->p_i2c_slv_dev[0];

    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_RFULLF))
    {
        // TODO: I2C_ReadData
        rx_data = amhw_hc32f460_i2c_dat_read(p_hw_i2c_slv);//接收数据
            /* 接受回调 */
            if (p_dev->is_gen_call == AM_TRUE) {
                /** 如果是广播呼叫 */
                if(p_i2c_slv_dev->p_cb_funs->pfn_gen_call) {
                    p_i2c_slv_dev->p_cb_funs->pfn_gen_call(p_i2c_slv_dev->p_arg, rx_data);
                }
            } else {
                if(p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put) {
                    p_i2c_slv_dev->p_cb_funs->pfn_rxbyte_put(p_i2c_slv_dev->p_arg, rx_data);
//                    AM_DBG_INFO("I2C_RXI_Callback pfn_rxbyte_put rx_data = %d\r\n", rx_data);
                }
            }
        rx_len++;
    }
}

void I2C_TEI_Callback(void *p_arg)
{
    uint8_t                  tx_data = 0;
    amhw_hc32f460_i2c_t       *p_hw_i2c_slv;
    am_hc32f460_i2c_slv_dev_t    *p_dev          = (am_hc32f460_i2c_slv_dev_t *)p_arg;
    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;
    am_i2c_slv_device_t     *p_i2c_slv_dev  = p_dev->p_i2c_slv_dev[0];

    if(I2C_SET == amhw_hc32f460_i2c_get_status(p_hw_i2c_slv, I2C_SR_TEMPTYF))
    {   
        // TODO: I2C_wirteData
        if (NULL != p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get) {
            p_i2c_slv_dev->p_cb_funs->pfn_txbyte_get(p_i2c_slv_dev->p_arg, &tx_data);            
            amhw_hc32f460_i2c_dat_write(p_hw_i2c_slv,tx_data);    
        }
    }
}


/**
 * \brief I2C初始化
 *
 * \note 硬件初始化 通过用户调用开始从设备来初始化
 */
am_i2c_slv_handle_t am_hc32f460_i2c_slv_init (
        am_hc32f460_i2c_slv_dev_t           *p_dev,
        const am_hc32f460_i2c_slv_devinfo_t *p_devinfo)
{
    if (p_dev == NULL || p_devinfo == NULL ) {
        return NULL;
    }

    p_dev->i2c_slv_serv.p_funcs = \
            (struct am_i2c_slv_drv_funcs *)&__g_i2c_slv_drv_funcs;
    p_dev->i2c_slv_serv.p_drv   = p_dev;
    p_dev->p_devinfo            = p_devinfo;
    p_dev->zlg_i2c_slv_dev_num  = 1;  /* 只能生成一个从机设备 */
    p_dev->is_gen_call          = AM_FALSE;
    p_dev->p_i2c_slv_dev[0]     = NULL;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    /* 初始化软件定时器 */
    am_softimer_init(&p_dev->softimer, __i2c_slv_timing_callback, p_dev);

    __i2c_slv_hard_init(p_dev);

    /* 连接中断 */
    am_int_connect(p_dev->p_devinfo->inum, IRQ141_Handler, NULL);
    if (HC32F460_I2C1_BASE == p_dev->p_devinfo->i2c_regbase)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                              I2C1_RXI |
                                              I2C1_TXI |
                                              I2C1_TEI |
                                              I2C1_EEI);
    }

    if (HC32F460_I2C2_BASE == p_dev->p_devinfo->i2c_regbase)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                              I2C2_RXI |
                                              I2C2_TXI |
                                              I2C2_TEI |
                                              I2C2_EEI);
    }
    
    if (HC32F460_I2C3_BASE == p_dev->p_devinfo->i2c_regbase)
    {
        amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                              I2C3_RXI |
                                              I2C3_TXI |
                                              I2C3_TEI |
                                              I2C3_EEI);
    } 
#if 0
#define INT_I2C1_EE1            423u

    stc_intc_sel_field_t *stcIntSel;
    
    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * 1));
    stcIntSel->INTSEL = INT_I2C1_EE1;
    am_int_connect(1, I2C_EEI_Callback, p_dev);
//    am_int_enable(1);

#define  INT_I2C1_RXI            420u
    
    
    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * 3));
    stcIntSel->INTSEL = INT_I2C1_RXI;
    am_int_connect(3, I2C_RXI_Callback, p_dev);
//    am_int_enable(3);

#define  INT_I2C1_TEI            422u
            
    
    stcIntSel = (stc_intc_sel_field_t *)((uint32_t)(0x40051000UL + 0x5c) + (4u * 2));
    stcIntSel->INTSEL = INT_I2C1_TEI;
    am_int_connect(2, I2C_TEI_Callback, p_dev);
//    am_int_enable(2);
#endif

    return &(p_dev->i2c_slv_serv);
}

/**
 * \brief I2C解初始化
 */
void am_hc32f460_i2c_slv_deinit (am_i2c_slv_handle_t handle)
{
    amhw_hc32f460_i2c_t    *p_hw_i2c_slv = NULL;
    am_hc32f460_i2c_slv_dev_t *p_dev        = NULL;

    if (NULL == handle) {
        return ;
    }

    p_dev        = (am_hc32f460_i2c_slv_dev_t *)handle;
    p_hw_i2c_slv = (amhw_hc32f460_i2c_t *)p_dev->p_devinfo->i2c_regbase;

    amhw_hc32f460_i2c_disable (p_hw_i2c_slv);

    /* 去除中断连接 */
//    am_int_disable(p_dev->p_devinfo->inum);

//    am_int_disconnect(p_dev->p_devinfo->inum,
//                      IRQ141_Handler,
//                      (void *)p_dev);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void I2c1RxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c1RxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c1TxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c1TxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c1TxEmpty_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c1TxEmpty_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c1Err_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c1Err_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c2RxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c2RxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c2TxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c2TxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c2TxEmpty_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c2TxEmpty_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c2Err_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c2Err_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c3RxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c3RxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c3TxEnd_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c3TxEnd_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c3TxEmpty_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c3TxEmpty_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}

void I2c3Err_IrqHandler(void *p_arg)
{
//    AM_DBG_INFO("I2c3Err_IrqHandler!\r\n");    
    __i2c_slv_irq_handler(p_arg);
}


/* end of file */
