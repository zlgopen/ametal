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
 * \brief DAC驱动实现
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-26  mkr, first implementation.
 * \endinternal
 */

#include "am_lpc84x_dac.h"
#include "am_int.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __LPC_DAC_HW_DECL(p_hw_dac, p_drv)    \
             amhw_lpc_dac_t *p_hw_dac =       \
            ((am_lpc_dac_dev_t *)p_drv)->p_devinfo->p_hw_dac

#define __LPC_DAC_BITS_GET(p_drv)             \
        (((am_lpc_dac_dev_t *)p_drv)->p_devinfo->bits)

#define __LPC_DAC_VREF_GET(p_drv)             \
        (((am_lpc_dac_dev_t *)p_drv)->p_devinfo->vref)

/*******************************************************************************
* 函数声明
*******************************************************************************/
static uint32_t __lpc_dac_get_bits (void *p_drv);

static uint32_t __lpc_dac_get_vref (void *p_drv);

static int __lpc_dac_val_set (void *p_drv, int chan, uint32_t value);

static int __lpc_dac_enable (void *p_drv, int chan );

static int __lpc_dac_disable (void *p_drv, int chan );

/**
 * \brief DAC服务函数
 */
static const struct am_dac_drv_funcs __g_dac_drvfuncs = {
       __lpc_dac_get_bits,
       __lpc_dac_get_vref,
       __lpc_dac_val_set,
       __lpc_dac_enable,
       __lpc_dac_disable
};

/******************************************************************************/
/**
 * \brief 获取DAC转换精度。
 */
static uint32_t __lpc_dac_get_bits (void *p_drv)
{
    return (uint32_t)__LPC_DAC_BITS_GET(p_drv);
}

/**
 * \brief 获取DAC参考电压。
 */
static uint32_t __lpc_dac_get_vref (void *p_drv)
{
    return (uint32_t)__LPC_DAC_VREF_GET(p_drv);
}

/**
 * \brief 设置通道的DAC转换值。
 */
int __lpc_dac_val_set (void *p_drv, int chan, uint32_t value)
{
    __LPC_DAC_HW_DECL(p_hw_dac, p_drv);
    am_lpc_dac_dev_t *p_dev = (am_lpc_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;       /* 无效的通道值      */
    }

    amhw_lpc84x_dac_cr_set(p_hw_dac, value);

    return AM_OK;
}

/**
 * \brief 启动DAC转换
 */
static int __lpc_dac_enable (void *p_drv, int chan )
{
    __LPC_DAC_HW_DECL(p_hw_dac, p_drv);
    am_lpc_dac_dev_t *p_dev = (am_lpc_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;       /* 无效的通道值       */
    }

    p_dev->chan = chan;

    /* 使能DAC     */
    amhw_lpc84x_dac_dblbuf_set(p_hw_dac, AMHW_LPC84x_DAC_DBLBUF_EN);
    amhw_lpc84x_dac_cnt_set(p_hw_dac, AMHW_LPC84x_DAC_CNT_EN);
    amhw_lpc84x_dac_dma_int_req_set(p_hw_dac, AMHW_LPC84x_DAC_DMA_EN);

    return AM_OK;
}

/**
 * \brief 禁止DAC转换
 */
static int __lpc_dac_disable (void *p_drv, int chan )
{
    __LPC_DAC_HW_DECL(p_hw_dac, p_drv);
    am_lpc_dac_dev_t *p_dev = (am_lpc_dac_dev_t *)p_drv;

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if (chan != 0) {
        return -AM_ENXIO;         /* 无效的通道值 */
    }

    p_dev->chan = chan;

    /* 禁能DAC        */
    amhw_lpc84x_dac_dblbuf_set(p_hw_dac, AMHW_LPC84x_DAC_DBLBUF_DISEN);
    amhw_lpc84x_dac_cnt_set(p_hw_dac, AMHW_LPC84x_DAC_CNT_DISEN);
    return AM_OK;
}


/**
 * \brief DAC初始化
 */
am_dac_handle_t am_lpc_dac_init (am_lpc_dac_dev_t           *p_dev,
                                  const am_lpc_dac_devinfo_t *p_devinfo)
{
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_dev->p_devinfo         = p_devinfo;
    p_dev->dac_serve.p_funcs = &__g_dac_drvfuncs;
    p_dev->dac_serve.p_drv   = p_dev;

    p_dev->chan              = AMHW_LPC_DAC0_CHAN_0;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return (am_dac_handle_t)(&(p_dev->dac_serve));
}

/**
 * \brief DAC去初始化
 */
void am_lpc_dac_deinit (am_dac_handle_t handle)
{
    am_lpc_dac_dev_t *p_dev = (am_lpc_dac_dev_t *)handle;

    p_dev->dac_serve.p_funcs = NULL;
    p_dev->dac_serve.p_drv   = NULL;

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
