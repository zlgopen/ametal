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
 * \brief CS1237软件包实现
 *
 * \internal
 * \par Modification history
 * - 1.01 18-04-12  zcb, del am_adc_val_t
 * - 1.00 17-09-15  lqy, first implementation.
 * \endinternal
 */

#include "am_int.h"
#include "am_cs1237.h"
#include "am_gpio.h"
#include "am_delay.h"
#include "am_vdebug.h"
/**
 * \brief CS1237读指令和写指令宏定义
 */
#define __AM_CS1237_WRITE_CMD             (0x65 << 1)      /**< \brief 写指令 */
#define __AM_CS1237_READ_CMD              (0x56 << 1)      /**< \brief 读指令 */

/*****************************************************************************
 * 静态声明
 ****************************************************************************/

 /** \brief CS1237 ADC读函数 */
static int __am_cs1237_dat_read(am_cs1237_adc_dev_t *p_dev, uint32_t *val);

 /** \brief CS1237开启中断 */
 static void __am_cs1237_int_open(am_cs1237_adc_dev_t  *p_dev);

 /** \brief CS1237关闭中断 */
 static void __am_cs1237_int_close(am_cs1237_adc_dev_t  *p_dev);

 /** \brief CS1237 adc时序 */
  void __am_cs1237_ad_clk (am_cs1237_adc_dev_t  *p_dev);

 /** \brief CS1237 ADC中断处理 */
 static void __am_cs1237_trigger_handle(void *p_arg);

 /** \brief ADC 配置寄存器写函数 */
// static
 int __am_cs1237_config_reg_write (am_cs1237_adc_dev_t  *p_dev,
                                   int8_t           config_reg);

 /** \brief 启动ADC转换 */
 static int __pfn_adc_start (void                   *p_drv,
                            int                     chan,
                            am_adc_buf_desc_t      *p_desc,
                            uint32_t                desc_num,
                            uint32_t                count,
                            uint32_t                flags,
                            am_adc_seq_cb_t         pfn_callback,
                            void                   *p_arg);

 /** \brief 停止转换 */
 static int __pfn_adc_stop (void *p_drv, int chan);

 /** \brief 获取ADC的采样率    */
 static int __pfn_adc_rate_get (void       *p_drv,
                               int         chan,
                               uint32_t   *p_rate);

 /** \brief 设置ADC的采样率，实际采样率可能存在差异 */
 static int __pfn_adc_rate_set (void     *p_drv,
                               int       chan,
                               uint32_t  rate);

 /** \brief 获取ADC转换精度 */
 static uint32_t __pfn_bits_get (void *p_drv, int chan);

 /** \brief 获取ADC参考电压 */
 static uint32_t __pfn_vref_get (void *p_drv, int chan);

 /**
  * \brief ADC服务函数
  */
 static const struct am_adc_drv_funcs __g_adc_drvfuncs = {
     __pfn_adc_start,
     __pfn_adc_stop,
     __pfn_adc_rate_get,
     __pfn_adc_rate_set,
     __pfn_bits_get,
     __pfn_vref_get
 };



/**
 * \brief CS1237 SPI 时钟时序
 */
 void __am_cs1237_ad_clk (am_cs1237_adc_dev_t  *p_dev)
{
    int key;

    if (p_dev == NULL) {
        return ;
    }

    key = am_int_cpu_lock();

    am_gpio_set(p_dev->p_devinfo->clk,1);

    if(p_dev->p_devinfo->high_clk_delay){
        am_udelay(p_dev->p_devinfo->high_clk_delay);
    }

    am_gpio_set(p_dev->p_devinfo->clk,0);

    am_int_cpu_unlock(key);

    if(p_dev->p_devinfo->low_clk_delay){
        am_udelay(p_dev->p_devinfo->low_clk_delay);
    }
}

/**
 * \brief CS1237 读中断处理
 */
static void __am_cs1237_trigger_handle(void *p_arg)
{
    am_cs1237_adc_dev_t *p_dev = (am_cs1237_adc_dev_t *)p_arg;
    /* 当前转换的序列描述符 */
    am_adc_buf_desc_t *p_desc     = &(p_dev->p_desc[p_dev->desc_index]);
    uint32_t           adc_dat    = 0;
    uint32_t          *buf        = NULL;

    int ret = AM_OK;

    /* 取消外部中断 */
    __am_cs1237_int_close(p_dev);

    ret = __am_cs1237_dat_read(p_dev, &adc_dat);

    /* 开启外部中断 */
    __am_cs1237_int_open(p_dev);

    if (AM_OK != ret) {
        return;
    }

    if(p_dev->count==0){

        if (NULL != p_dev->triginfo.pfn_callback) {
            p_dev->triginfo.pfn_callback(p_dev->triginfo.p_arg, adc_dat);

        }
        return ;

    } else {
        /* 判断是否当前数据是有效的 */
        if (p_dev->conv_cnt < p_desc->length) {

            buf = (uint32_t *)p_desc->p_buf;

            /* 保存数据 */
            if (AM_ADC_DATA_ALIGN_RIGHT == p_dev->flags) {
                buf[p_dev->conv_cnt] = adc_dat & 0x00ffffff;
            } else {
                buf[p_dev->conv_cnt] = adc_dat << 0x8;
            }

            p_dev->conv_cnt++;

            /* 判断当前序列描述符是否已经完成转换 */
            if (p_dev->conv_cnt >= p_desc->length) {

                p_dev->conv_cnt = 0;
                if (NULL != p_desc->pfn_complete) {
                    p_desc->pfn_complete(p_desc->p_arg, AM_OK);
                }

                p_dev->desc_index++;

                /* 判断整个序列描述符是否完成一轮转换 */
                if (p_dev->desc_index == p_dev->desc_num) {

                    p_dev->desc_index = 0;
                    if (NULL != p_dev->pfn_callback) {
                        p_dev->pfn_callback(p_dev->p_arg, AM_OK);
                    }

                    p_dev->seq_cnt++; /* 整个序列转换完成 */
                    if (p_dev->count != 0 && p_dev->seq_cnt == p_dev->count) {
                        p_dev->seq_cnt = 0;

                        __pfn_adc_stop (p_dev, p_dev->ch);  /* 关闭模块 */

                        return ; /* 返回 */
                    }
                }
            }
        } else {
            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_ERROR);
            }
            __pfn_adc_stop (p_dev, p_dev->ch);
        }
    }
}

/**
 * \brief CS1237 中断关闭
 */
static void __am_cs1237_int_close(am_cs1237_adc_dev_t  *p_dev)
{
    if (NULL == p_dev) {
        return;
    }

    am_gpio_trigger_off(p_dev->p_devinfo->out_pin);

    am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                    AM_GPIO_INPUT | AM_GPIO_PULLUP);

    p_dev->is_int = AM_FALSE;
}

/**
 * \brief CS1237 中断开启
 */
static void __am_cs1237_int_open(am_cs1237_adc_dev_t  *p_dev)
{
    if (NULL == p_dev) {
        return;
    }

    am_gpio_trigger_connect(p_dev->p_devinfo->out_pin,
                            __am_cs1237_trigger_handle,
                            (void *)p_dev);

    am_gpio_trigger_cfg(p_dev->p_devinfo->out_pin,
                        AM_GPIO_TRIGGER_FALL);

    am_gpio_trigger_on(p_dev->p_devinfo->out_pin);

    p_dev->is_int = AM_TRUE;
}

 /**
  * \brief cs1237 读数据函数
  */
static int __am_cs1237_dat_read(am_cs1237_adc_dev_t *p_dev, uint32_t *val)
{
    uint8_t i = 0;

    if (NULL == p_dev || NULL == val) {
        return -AM_EINVAL;
    }

    /* out_pin输入上拉模式 */
    am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                    AM_GPIO_INPUT | AM_GPIO_PULLUP);

    if (am_gpio_get(p_dev->p_devinfo->out_pin)) {
        /* adc数据还未转换完成 */
        return AM_ERROR;
    }

//    *val = 0;

    /* 循环读取24bit数据 */
    for (i = 0 ; i < 24 ; i++) {

        __am_cs1237_ad_clk(p_dev);
        if (am_gpio_get(p_dev->p_devinfo->out_pin)) {

            *val |= 0x1 << (23-i);
        }
    }

    __am_cs1237_ad_clk(p_dev);
    __am_cs1237_ad_clk(p_dev);

    /* 27个时钟拉高数据线 */
    am_gpio_pin_cfg(p_dev->p_devinfo->in_pin,
                    AM_GPIO_OUTPUT_INIT_HIGH);
    __am_cs1237_ad_clk(p_dev);

    return AM_OK;
}

/**
 * \brief ADC 配置寄存器写函数(非阻塞)
 */
//static
int __am_cs1237_config_reg_write (am_cs1237_adc_dev_t  *p_dev,
                                  int8_t           config_reg)
{
    uint8_t   i      = 0;
    am_bool_t is_int = AM_FALSE;

    if (AM_TRUE == p_dev->is_int) {
        __am_cs1237_int_close(p_dev);
        is_int = AM_TRUE;
    } else {
        am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                        AM_GPIO_INPUT | AM_GPIO_PULLUP);
    }

    /* 等待数据转换完成 */
    if (am_gpio_get(p_dev->p_devinfo->out_pin)) {
        /* adc数据还未转换完成 */
        return AM_ERROR;
    }

    /*发送29个CLK*/
    for (i = 0; i < 29; i++) {
        __am_cs1237_ad_clk(p_dev);
    }

    am_gpio_pin_cfg(p_dev->p_devinfo->in_pin,
                    AM_GPIO_OUTPUT_INIT_HIGH);

    /* 写命令 */
    for (i = 0 ; i < 7 ; i++) {

        if(__AM_CS1237_WRITE_CMD & (1 << (7-i))) {
            am_gpio_set(p_dev->p_devinfo->in_pin,1);
        } else {
            am_gpio_set(p_dev->p_devinfo->in_pin,0);
        }
        __am_cs1237_ad_clk(p_dev);
    }

    __am_cs1237_ad_clk(p_dev);

    /* 写配置 */
    for (i = 0; i < 8; i++) {

        if(config_reg & (1 << (7-i))) {
            am_gpio_set(p_dev->p_devinfo->in_pin,1);
        } else {
            am_gpio_set(p_dev->p_devinfo->in_pin,0);
        }
        __am_cs1237_ad_clk(p_dev);
    }

    __am_cs1237_ad_clk(p_dev);

    am_gpio_set(p_dev->p_devinfo->in_pin, 1);

    if (AM_TRUE == is_int) {
        __am_cs1237_int_open(p_dev);
    }

    return AM_OK;
}

/** \brief 启动ADC转换 */
static int __pfn_adc_start (void                   *p_drv,
                           int                     chan,
                           am_adc_buf_desc_t      *p_desc,
                           uint32_t                desc_num,
                           uint32_t                count,
                           uint32_t                flags,
                           am_adc_seq_cb_t         pfn_callback,
                           void                   *p_arg)
{
    am_cs1237_adc_dev_t *p_dev;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev  = (am_cs1237_adc_dev_t *)p_drv;

    p_dev->ch           = chan;
    p_dev->p_desc       = p_desc;
    p_dev->desc_num     = desc_num;
    p_dev->count        = count;
    p_dev->flags        = flags;
    p_dev->pfn_callback = pfn_callback;
    p_dev->p_arg        = p_arg;

    am_cs1237_ch_set(p_dev,p_dev->ch);
    /* 开启外部中断 */
    am_cs1237_read_int_enable(p_dev);

    return AM_OK;
}

/** \brief 停止转换 */
static int __pfn_adc_stop (void *p_drv, int chan)
{
    am_cs1237_adc_dev_t *p_dev;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_cs1237_adc_dev_t *)p_drv;

    am_cs1237_read_int_disable(p_dev);

    return AM_OK;
}

/** \brief 获取ADC的采样率    */
static int __pfn_adc_rate_get (void       *p_drv,
                              int         chan,
                              uint32_t   *p_rate)
{
    am_cs1237_adc_dev_t *p_dev;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_cs1237_adc_dev_t *)p_drv;
    
    *p_rate = am_cs1237_out_speed_get(p_dev);

    return AM_OK;
}

/** \brief 设置ADC的采样率，实际采样率可能存在差异 */
static int __pfn_adc_rate_set (void     *p_drv,
                              int       chan,
                              uint32_t  rate)
{
    am_cs1237_adc_dev_t *p_dev;
    
    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_cs1237_adc_dev_t *)p_drv;

    /* 10 40 640 1280 */
    if ( rate <= 25 ) {
        rate = AM_CS1237_SPEED_10HZ;
    } else if ( rate <= 340 ) {
        rate = AM_CS1237_SPEED_40HZ;
    } else if ( rate <= 960){
        rate = AM_CS1237_SPEED_640HZ;
    } else {
        rate = AM_CS1237_SPEED_1280HZ;
    }
    am_cs1237_out_speed_set(p_dev,rate);

    return AM_OK;
}

/** \brief 获取ADC转换精度 */
static uint32_t __pfn_bits_get (void *p_drv, int chan)
{

    if (NULL == p_drv) {
        return (uint32_t)(-AM_EINVAL);
    }

    return 24;
}

/** \brief 获取ADC参考电压 */
static uint32_t __pfn_vref_get (void *p_drv, int chan)
{
    am_cs1237_adc_dev_t *p_dev;
    
    if (NULL == p_drv) {
            return (uint32_t)(-AM_EINVAL);
        }

    p_dev = (am_cs1237_adc_dev_t *)p_drv;

    return p_dev->p_devinfo->vref;
}

/**
 * \brief ADC 配置寄存器读函数
 */
uint8_t am_cs1237_config_reg_read(am_cs1237_adc_dev_t  *p_dev)
{
    uint8_t i          = 0;
    uint8_t config_reg = 0;

    am_bool_t is_int = AM_FALSE;

    if (AM_TRUE == p_dev->is_int) {
        __am_cs1237_int_close(p_dev);
        is_int = AM_TRUE;
    } else {
        am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                        AM_GPIO_INPUT | AM_GPIO_PULLUP);
    }

    while (am_gpio_get(p_dev->p_devinfo->out_pin));

    for (i = 0 ; i < 29 ; i++) {
        __am_cs1237_ad_clk(p_dev);
    }

    am_gpio_pin_cfg(p_dev->p_devinfo->in_pin,
                    AM_GPIO_OUTPUT_INIT_HIGH);

    /*30-36CLK,MCU data_in引脚发送读命令字*/
    for (i = 0; i < 7; i++) {

        if (__AM_CS1237_READ_CMD & (1 << (7-i))) {
            am_gpio_set(p_dev->p_devinfo->in_pin,1);
        } else {
            am_gpio_set(p_dev->p_devinfo->in_pin,0);
        }
        __am_cs1237_ad_clk(p_dev);
    }

    __am_cs1237_ad_clk(p_dev);

    am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                    AM_GPIO_INPUT | AM_GPIO_PULLUP);

    for (i = 0; i < 8; i++) {

        __am_cs1237_ad_clk(p_dev);

        if(am_gpio_get(p_dev->p_devinfo->out_pin)) {

            config_reg |= (1 << (7-i));
        }
    }

    __am_cs1237_ad_clk(p_dev);

    am_gpio_pin_cfg(p_dev->p_devinfo->in_pin,
                    AM_GPIO_OUTPUT_INIT_HIGH);

    if (AM_TRUE == is_int) {
        __am_cs1237_int_open(p_dev);
    }

    return config_reg;
}

/**
 * \brief 初始化函数
 */
am_cs1237_adc_handle_t am_cs1237_init(am_cs1237_adc_dev_t            *p_dev,
                                  const am_cs1237_adc_devinfo_t  *p_devinfo)
{
    uint8_t config_reg = 0;

    /* 验证参数有效性 */
    if ((p_dev == NULL) || (p_devinfo == NULL)) {
        return NULL;
    }

    p_dev->ch = AM_CS1237_CHANNEL_A;
    p_dev->pga = AM_CS1237_PGA_1;
    p_dev->out_speed = AM_CS1237_SPEED_10HZ;
    p_dev->ref  = p_dev->p_devinfo->refo_off;
    p_dev->adc_serve.p_funcs = &__g_adc_drvfuncs;
    p_dev->adc_serve.p_drv   = p_dev;
    p_dev->pfn_callback      = NULL;
    p_dev->p_desc            = NULL;
    p_dev->p_arg             = NULL;
    p_dev->desc_num          = 0;
    p_dev->flags             = 0;
    p_dev->count             = 0;
    p_dev->seq_cnt           = 0;
    p_dev->desc_index        = 0;
    p_dev->conv_cnt          = 0;
    p_dev->p_devinfo         = p_devinfo;
    p_dev->is_int            = AM_FALSE;

    /*MCU data_clk 配置为输出，起始电平为低*/
    am_gpio_pin_cfg(p_dev->p_devinfo->clk,
                    AM_GPIO_OUTPUT_INIT_LOW);

    /*MCU data_in 配置为输出，起始电平为高*/
    am_gpio_pin_cfg(p_dev->p_devinfo->in_pin,
                    AM_GPIO_OUTPUT_INIT_HIGH);

    /*MCU data_out配置为上拉输入*/
    am_gpio_pin_cfg(p_dev->p_devinfo->out_pin,
                    AM_GPIO_INPUT | AM_GPIO_PULLUP);

   /*初始化config寄存器*/
    config_reg |= p_dev->ch;
    config_reg |= p_dev->pga << 2;
    config_reg |= p_dev->out_speed << 4;
    config_reg |= p_dev->ref <<6;

    while ( AM_OK != __am_cs1237_config_reg_write(p_dev, config_reg) );

    return p_dev;
}

/**
 * \brief 解初始化函数
 */
void am_cs1237_deinit (am_cs1237_adc_handle_t handle)
{
    handle->adc_serve.p_funcs = NULL;
    handle->adc_serve.p_drv   = NULL;
    handle->pfn_callback      = NULL;
    handle->p_desc            = NULL;
    handle->p_arg             = NULL;
    handle->p_devinfo = NULL;

    return ;
}

/**
 * \brief CS1237 配置寄存器pga写
 */
int am_cs1237_pga_set(am_cs1237_adc_dev_t  *p_dev, uint32_t pga)
{
    uint8_t config_reg = 0;

    config_reg |= p_dev->ch;
    config_reg |= pga << 2;
    config_reg |= p_dev->out_speed << 4;
    config_reg |= p_dev->ref <<6;

    if ( AM_OK != __am_cs1237_config_reg_write(p_dev, config_reg) ) {
        return AM_ERROR;
    }
    p_dev->pga = pga;

    return AM_OK;
}

/**
 * \brief CS1237 配置寄存器Ref 配置使能  0 使用内部     1 使用外部
 */
int am_cs1237_ref_set(am_cs1237_adc_dev_t  *p_dev, uint8_t flag)
{
    uint8_t config_reg = 0;

    config_reg |= p_dev->ch;
    config_reg |= p_dev->pga << 2;
    config_reg |= p_dev->out_speed << 4;
    config_reg |= flag <<6;

    if ( AM_OK != __am_cs1237_config_reg_write(p_dev, config_reg) ) {
        return AM_ERROR;
    }
    p_dev->ref = flag;

    return AM_OK;
}

/**
 * \brief CS1237 pga放大倍数读
 */
uint32_t am_cs1237_pga_get(am_cs1237_adc_dev_t  *p_dev)
{
    switch(p_dev->pga){

    case AM_CS1237_PGA_1:
         return 1;

    case AM_CS1237_PGA_2:
        return 2;

    case AM_CS1237_PGA_64:
        return 64;

    default :
        return 128;
    }
}

/**
 * \brief CS1237 配置寄存器ch写
 */
int am_cs1237_ch_set(am_cs1237_adc_dev_t  *p_dev, uint32_t ch)
{
    uint8_t config_reg = 0;

    config_reg |= ch;
    config_reg |= p_dev->pga << 2;
    config_reg |= p_dev->out_speed << 4;
    config_reg |= p_dev->ref << 6;

    if ( AM_OK != __am_cs1237_config_reg_write(p_dev, config_reg) ) {
        return AM_ERROR;
    }

    p_dev->ch = ch;

    return AM_OK;
}


/**
 * \brief CS1237 配置寄存器ch读
 */
uint8_t am_cs1237_ch_get(am_cs1237_adc_dev_t  *p_dev)
{
    uint8_t ch_reg = 0;

    ch_reg = p_dev->ch & 0x03;

    return ch_reg;
}
/**
 * \brief CS1237 配置寄存器speed写
 */
int am_cs1237_out_speed_set(am_cs1237_adc_dev_t  *p_dev, uint32_t speed)
{
    uint8_t config_reg = 0;


    config_reg |= p_dev->ch;
    config_reg |= p_dev->pga << 2;
    config_reg |= speed << 4;
    config_reg |= p_dev->ref << 6;

    if ( AM_OK != __am_cs1237_config_reg_write(p_dev, config_reg) ) {
        return AM_ERROR;
    }

    p_dev->out_speed = speed;
    return AM_OK;
}

/**
 * \brief CS1237 adc输出速率读
 */
uint32_t am_cs1237_out_speed_get(am_cs1237_adc_dev_t  *p_dev)
{
    switch(p_dev->out_speed){

    case AM_CS1237_SPEED_10HZ:
        return 10;

    case AM_CS1237_SPEED_40HZ:
        return 40;

    case AM_CS1237_SPEED_640HZ:
        return 640;

    default:
        return 1280;
    }
}

/**
 * \brief CS1237 允许读
 */
int am_cs1237_read_int_enable(am_cs1237_adc_dev_t  *p_dev)
{
    __am_cs1237_int_open(p_dev);

    return AM_OK;
}

/**
 * \brief CS1237 禁止读
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  AM_OK     : 操作成功
 */
int am_cs1237_read_int_disable(am_cs1237_adc_dev_t  *p_dev)
{
    __am_cs1237_int_close(p_dev);

    return AM_OK;
}


/**
 * \brief CS1237 轮循读adc采集值
 */
int am_cs1237_read_polling(am_cs1237_adc_dev_t *p_dev, uint32_t *val)
{
    return __am_cs1237_dat_read(p_dev, val);
}

/**
 * \brief CS1237 低功耗模式
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  无
 */
void am_cs1237_power_down_enter(am_cs1237_adc_dev_t  *p_dev)
{
    am_gpio_set(p_dev->p_devinfo->clk,1);

    if(p_dev->p_devinfo->powerdown_delay){
        am_udelay(p_dev->p_devinfo->powerdown_delay);
    }

    return ;
}

/**
 * \brief CS1237 低功耗模式
 *
 * \param[in] p_dev   : CS1237操作句柄
 *
 * \retval  无
 */
void am_cs1237_power_down_out(am_cs1237_adc_dev_t  *p_dev)
{
    am_gpio_set(p_dev->p_devinfo->clk,0);

    return ;
}

/**
 * \brief CS1237 连接中断连接函数
 */
void am_cs1237_int_connect(am_cs1237_adc_dev_t      *p_dev ,
                           am_cs1237_code_read_cb_t  p_fun,
                           void                     *p_arg)
{
    p_dev->triginfo.pfn_callback = p_fun;
    p_dev->triginfo.p_arg = p_arg;
}

/**
 * \brief CS1237删除中断连接函数
 */
void am_cs1237_int_disconnect(am_cs1237_adc_dev_t      *p_dev ,
                              am_cs1237_code_read_cb_t  p_fun,
                              void                     *p_arg)
{
    p_dev->triginfo.pfn_callback = NULL;
    p_dev->triginfo.p_arg = NULL;
}

am_adc_handle_t am_cs1237_standard_adc_handle_get(am_cs1237_adc_dev_t *p_dev)
{
    return (am_adc_handle_t)(&(p_dev->adc_serve));
}
/* end of file */
