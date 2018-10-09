/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2014 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ADC drivers implementation
 * 
 * \internal
 * \par Modification history
 * - 1.02 18-04-24  pea, update standard interface
 * - 1.01 15-01-15  hbt, add __adc_global_read
 * - 1.00 14-12-01  asd, first implementation
 * \endinternal
 */

#include "am_lpc_adc_int.h"
#include "am_int.h"
#include "am_clk.h"


/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __ADC_HW_DECL(p_hw_adc, p_drv)    \
        amhw_lpc_adc_t *p_hw_adc =               \
            (amhw_lpc_adc_t *)((am_lpc_adc_dev_t *)p_drv)->p_devinfo->adc_regbase

#define __ADC_BITS_GET(p_drv)             \
        (((am_lpc_adc_dev_t *)p_drv)->p_devinfo->bits)

#define __ADC_VREF_GET(p_drv)             \
        (((am_lpc_adc_dev_t *)p_drv)->p_devinfo->vref)

/*******************************************************************************
* 函数声明
*******************************************************************************/

static int __adc_start (void                   *p_drv,
                        int                     chan,
                        am_adc_buf_desc_t      *p_desc,
                        uint32_t                desc_num,
                        uint32_t                count,
                        uint32_t                flags,
                        am_adc_seq_cb_t         pfn_callback,
                        void                   *p_arg);

static int __adc_stop (void *p_drv, int chan);

static int __adc_get_rate (void *p_drv, int chan, uint32_t *p_rate);

static int __adc_set_rate (void *p_drv, int chan, uint32_t rate);

static uint32_t __adc_get_bits(void *p_drv, int chan);

static uint32_t __adc_get_vref(void *p_drv, int chan);

/**
 * \brief ADC服务函数
 */
static const struct am_adc_drv_funcs __g_adc_drvfuncs = {
    __adc_start,
    __adc_stop,
    __adc_get_rate,
    __adc_set_rate,
    __adc_get_bits, 
    __adc_get_vref
};

/******************************************************************************/

static void __adc_int(void *p_arg)
{
    am_lpc_adc_dev_t *p_dev = (am_lpc_adc_dev_t *)p_arg;
    uint16_t         *p_buf = NULL;

    __ADC_HW_DECL(p_hw_adc, p_arg);

    uint32_t dat;

    /* 获取存放数据缓冲区的指针 */
    p_buf = (uint16_t *)p_dev->p_desc[p_dev->desc_pos].p_buf;
    if (p_buf == NULL) {
        return;
    }

    dat = amhw_lpc_adc_glo_data_get(p_hw_adc, AMHW_LPC_ADC_SEQ_A);

    /* 对齐格式 */
    if (AM_ADC_DATA_ALIGN_RIGHT != p_dev->flags) {
        p_buf[p_dev->index] = (dat & (0xFFF << (12 - __ADC_BITS_GET(p_arg))));
    } else {
        p_buf[p_dev->index] = (dat & 0xFFF0) >> (4 + (12 - __ADC_BITS_GET(p_arg)));
    }
    (p_dev->index)++;
    /* 完成一个描述数缓冲区的接收*/
    if (p_dev->index >= p_dev->p_desc[p_dev->desc_pos].length) {
        p_dev->index = 0;

        if (NULL != p_dev->p_desc[p_dev->desc_pos].pfn_complete) {
            p_dev->p_desc[p_dev->desc_pos].pfn_complete(p_dev->p_desc[p_dev->desc_pos].p_arg, AM_OK);
        }

        p_dev->desc_pos++;
        if (p_dev->desc_pos == p_dev->desc_num) { /* 已经接收完成 */
            p_dev->desc_pos = 0;

            if (NULL != p_dev->pfn_callback) {
                p_dev->pfn_callback(p_dev->p_arg, AM_OK);
            }

            p_dev->count_pos++; /* 转换序列标准 */

            if (p_dev->count != 0 && p_dev->count_pos == p_dev->count) {
                p_dev->count_pos = 0;
                __adc_stop(p_dev, p_dev->chan);  /* 关闭模块 */
                return ; /* 返回 */
            }
        }
    }

    /* 启动ADC序列转换 */
    amhw_lpc_adc_seq_start(p_hw_adc, AMHW_LPC_ADC_SEQ_A);
}

/**
 * \brief ADC初始化
 */
am_adc_handle_t am_lpc_adc_int_init (am_lpc_adc_dev_t           *p_dev,
                                      const am_lpc_adc_devinfo_t *p_devinfo)
{
    amhw_lpc_adc_t *p_hw_adc = NULL;

    uint32_t cfg_ctrl;
    
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }
    
    p_dev->p_devinfo         = p_devinfo;
    p_dev->adc_serve.p_funcs = &__g_adc_drvfuncs;
    p_dev->adc_serve.p_drv   = p_dev; 
    
    p_dev->pfn_callback      = NULL;
    p_dev->chan              = 0;

    p_hw_adc                 = (amhw_lpc_adc_t *)p_devinfo->adc_regbase;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    amhw_lpc_adc_startup(p_hw_adc);
    
    cfg_ctrl = AMHW_LPC_ADC_CTRL_CLK_DIV(p_devinfo->clk_div)    |
               AMHW_LPC_ADC_CTRL_MODE_SYNC                      |
               AMHW_LPC_ADC_CTRL_RESOL_NBIT(p_devinfo->bits)    |
               AMHW_LPC_ADC_CTRL_SAMPLE_TIME(0)                 |
               AMHW_LPC_ADC_CTRL_BYPASS_CALIB;

  
    /* ADC控制寄存器配置 */
    amhw_lpc_adc_config(p_hw_adc, cfg_ctrl);
    
    /* 使能序列A中断 */
    amhw_lpc_adc_int_enable(p_hw_adc, AMHW_LPC_ADC_INTEN_SEQA_ENABLE);
    
    am_int_connect(p_devinfo->intnums[0], __adc_int, p_dev);
    am_int_enable(p_devinfo->intnums[0]);
    
    return (am_adc_handle_t)(&(p_dev->adc_serve));
}

/**
 * \brief ADC去初始化
 */
void am_lpc_adc_int_deinit (am_adc_handle_t handle)
{
    am_lpc_adc_dev_t *p_dev = (am_lpc_adc_dev_t *)handle;

    int i = 0;

    if (p_dev == NULL) {
    	return;
    }
    
    for (i = 0; i < p_dev->p_devinfo->intcnts; i++) {
        am_int_disable(p_dev->p_devinfo->intnums[i]);
    }
    
    amhw_lpc_adc_seq_disable((amhw_lpc_adc_t *)p_dev->p_devinfo->adc_regbase,
                                  AMHW_LPC_ADC_SEQ_A);

    am_int_disconnect(p_dev->p_devinfo->intnums[0], __adc_int, p_dev);
    am_int_disable(p_dev->p_devinfo->intnums[0]);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/**
 * \brief 启动ADC转换
 */
static int __adc_start (void                   *p_drv,
                        int                     chan,
                        am_adc_buf_desc_t      *p_desc,
                        uint32_t                desc_num,
                        uint32_t                count,
                        uint32_t                flags,
                        am_adc_seq_cb_t         pfn_callback,
                        void                   *p_arg)
{
    am_lpc_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv || p_desc == NULL) {
        return -AM_EINVAL;
    }

    if (chan >= 12) {
        return -AM_ENXIO;   /* 无效的通道值 */
    }

    p_dev = (am_lpc_adc_dev_t *)p_drv;

    __ADC_HW_DECL(p_hw_adc, p_drv);

    p_dev->p_desc       = p_desc;
    p_dev->chan         = chan;
    p_dev->desc_num     = desc_num;
    p_dev->count        = count;
    p_dev->flags        = flags;
    p_dev->pfn_callback = pfn_callback;
    p_dev->p_arg        = p_arg;
    p_dev->index        = 0;
    p_dev->count_pos    = 0;

    amhw_lpc_adc_startup(p_hw_adc);

    /* 序列A禁能 */
    amhw_lpc_adc_seq_disable(p_hw_adc, AMHW_LPC_ADC_SEQ_A);
    amhw_lpc_adc_seq_config(p_hw_adc, AMHW_LPC_ADC_SEQ_A, 0);

    amhw_lpc_adc_seq_config(p_hw_adc,
                                 AMHW_LPC_ADC_SEQ_A,
                                 AMHW_LPC_ADC_SEQ_CTRL_TRIG_SOFT    |
                                 AMHW_LPC_ADC_SEQ_CTRL_TRIG_POL_POS |
                                 AMHW_LPC_ADC_SEQ_CTRL_MODE_EOC     |
                                (1UL << chan));

    amhw_lpc_adc_seq_enable(p_hw_adc, AMHW_LPC_ADC_SEQ_A);
    amhw_lpc_adc_seq_start(p_hw_adc, AMHW_LPC_ADC_SEQ_A);

    return AM_OK;
}

static int __adc_stop (void *p_drv, int chan)
{
    am_lpc_adc_dev_t *p_dev = NULL;

    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev = (am_lpc_adc_dev_t *)p_drv;

    amhw_lpc_adc_seq_disable((amhw_lpc_adc_t *)p_dev->p_devinfo->adc_regbase,
                                  AMHW_LPC_ADC_SEQ_A);

    return AM_OK;
}

static int __adc_get_rate (void *p_drv, int chan, uint32_t *p_rate)
{
    amhw_lpc_adc_t   *p_hw_adc = NULL;
    am_lpc_adc_dev_t *p_dev = NULL;
    uint32_t               adc_pclk;
    uint32_t               adc_clk_div;

    if (NULL == p_drv || NULL == p_rate) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc_adc_t *)(p_dev->p_devinfo->adc_regbase);

    adc_pclk    = am_clk_rate_get(p_dev->p_devinfo->clk_id);
    adc_clk_div = amhw_lpc_adc_ctrl_get(p_hw_adc) & 0xFF;

    /* LPC5411x ADC采样率*/
    *p_rate = adc_pclk / adc_clk_div;

    return AM_OK;
}

static int __adc_set_rate (void *p_drv, int chan, uint32_t rate)
{
    amhw_lpc_adc_t   *p_hw_adc = NULL;
    am_lpc_adc_dev_t *p_dev = NULL;
    uint32_t               adc_pclk;
    uint32_t               adc_clk_div;


    if (NULL == p_drv) {
        return -AM_EINVAL;
    }

    p_dev    = (am_lpc_adc_dev_t *)p_drv;
    p_hw_adc = (amhw_lpc_adc_t *)(p_dev->p_devinfo->adc_regbase);

    adc_pclk  = am_clk_rate_get(p_dev->p_devinfo->clk_id);

    adc_clk_div = adc_pclk / rate;

    /* 输入时钟太高时，设置到极限 */
    if (adc_clk_div != 0) {

        if ((adc_pclk % rate) == 0) {
            adc_clk_div = adc_clk_div > 256 ? 255 : adc_clk_div - 1;
        } else {
            adc_clk_div = adc_clk_div >= 255 ? 255 : adc_clk_div ;
        }
    }

    p_hw_adc->ctrl &= (~(uint32_t)0xFF);
    p_hw_adc->ctrl |= AMHW_LPC_ADC_CTRL_CLK_DIV(adc_clk_div);

    return AM_OK;
}

/**
 * \brief 获取ADC转换精度。
 */
static uint32_t __adc_get_bits (void *p_drv , int chan)
{
    return (uint32_t)__ADC_BITS_GET(p_drv);
}

/**
 * \brief 获取ADC参考电压。
 */
static uint32_t __adc_get_vref (void *p_drv , int chan)
{
    if (NULL == p_drv) {
        return 0;   /* 资源已经释放，参考电压未知 */
    }

    return (uint32_t)__ADC_VREF_GET(p_drv);
}
/* end of file */
