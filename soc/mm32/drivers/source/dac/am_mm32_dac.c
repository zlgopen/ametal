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
 * - 1.00 17-8-22  fra, first implementation
 * \endinternal
 */

#include "am_mm32_dac.h"
#include "am_int.h"
#include "hw/amhw_mm32_dac.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

#define __MM32_DAC_BITS_GET(p_drv)             \
        (((am_mm32_dac_dev_t *)p_drv)->p_devinfo->bits)

#define __MM32_DAC_VREF_GET(p_drv)             \
        (((am_mm32_dac_dev_t *)p_drv)->p_devinfo->vref)

/*******************************************************************************
* 函数声明
*******************************************************************************/
static uint32_t __mm32_dac_get_bits (void *p_drv);

static uint32_t __mm32_dac_get_vref (void *p_drv);

static int __mm32_dac_val_set (void *p_drv, int chan, uint32_t value);

static int __mm32_dac_enable (void *p_drv, int chan );

static int __mm32_dac_disable (void *p_drv, int chan );

/**
 * \brief DAC服务函数
 */
static const struct am_dac_drv_funcs __g_dac_drvfuncs = {
       __mm32_dac_get_bits,
       __mm32_dac_get_vref,
       __mm32_dac_val_set,
       __mm32_dac_enable,
       __mm32_dac_disable
};

/******************************************************************************/

/**
 * \brief 获取DAC转换精度。
 */
static uint32_t __mm32_dac_get_bits (void *p_drv)
{
    return (uint32_t)__MM32_DAC_BITS_GET(p_drv);
}

/**
 * \brief 获取DAC参考电压。
 */
static uint32_t __mm32_dac_get_vref (void *p_drv)
{
    return (uint32_t)__MM32_DAC_VREF_GET(p_drv);
}

/**
 * \brief 设置通道的DAC转换值。
 */
static int __mm32_dac_val_set (void *p_drv, int chan, uint32_t value)
{
    am_mm32_dac_dev_t *p_dev    = (am_mm32_dac_dev_t *)p_drv;
    amhw_mm32_dac_t   *p_hw_dac = (amhw_mm32_dac_t *)(p_dev->p_devinfo->dac_reg_base);

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if ((chan != AMHW_MM32_DAC_CHAN_1) && (chan != AMHW_MM32_DAC_CHAN_2)) {
        return -AM_ENXIO;       /* 无效的通道值 */
    }

    p_dev->chan = chan;

    if (chan == AMHW_MM32_DAC_CHAN_1) {
        if (p_dev->p_devinfo->bits == 8) {

            amhw_mm32_dac_chan1_8bit_right_aligned_data(p_hw_dac, value);
        } else {
            amhw_mm32_dac_chan1_12bit_right_aligned_data(p_hw_dac, value);
        }
    } else {
        if (p_dev->p_devinfo->bits == 8) {
            amhw_mm32_dac_chan2_8bit_right_aligned_data(p_hw_dac, value);
        }else {
            amhw_mm32_dac_chan2_12bit_right_aligned_data(p_hw_dac, value);
        }
    }

    return AM_OK;
}

/**
 * \brief 启动DAC转换
 */
static int __mm32_dac_enable (void *p_drv, int chan)
{

    am_mm32_dac_dev_t *p_dev    = (am_mm32_dac_dev_t *)p_drv;
    amhw_mm32_dac_t   *p_hw_dac = (amhw_mm32_dac_t *)(p_dev->p_devinfo->dac_reg_base);

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if ((chan != AMHW_MM32_DAC_CHAN_1) && (chan != AMHW_MM32_DAC_CHAN_2)) {
        return -AM_ENXIO;       /* 无效的通道值 */
    }

    p_dev->chan = chan;

    if (chan == AMHW_MM32_DAC_CHAN_1) {
        amhw_mm32_dac_chan1_enable(p_hw_dac);
    } else {
        amhw_mm32_dac_chan2_enable(p_hw_dac);
    }

    return AM_OK;
}

/**
 * \brief 禁止DAC转换
 */
static int __mm32_dac_disable (void *p_drv, int chan )
{

    am_mm32_dac_dev_t *p_dev    = (am_mm32_dac_dev_t *)p_drv;
    amhw_mm32_dac_t   *p_hw_dac = (amhw_mm32_dac_t *)(p_dev->p_devinfo->dac_reg_base);

    if (p_dev == NULL ) {
        return -AM_EINVAL;
    }

    if ((chan != AMHW_MM32_DAC_CHAN_1) && (chan != AMHW_MM32_DAC_CHAN_2)) {
        return -AM_ENXIO;       /* 无效的通道值 */
    }

    p_dev->chan = chan;

    if (chan == AMHW_MM32_DAC_CHAN_1) {
        amhw_mm32_dac_chan1_disable(p_hw_dac);
    } else {
        amhw_mm32_dac_chan2_disable(p_hw_dac);
    }

    return AM_OK;
}

/**
 * \brief DAC初始化
 */
am_dac_handle_t am_mm32_dac_init (am_mm32_dac_dev_t           *p_dev,
                                 const am_mm32_dac_devinfo_t *p_devinfo)
{
    amhw_mm32_dac_t *p_hw_dac;
    
    if ((p_devinfo == NULL) || (p_dev == NULL)) {
        return NULL;
    }

    p_hw_dac = (amhw_mm32_dac_t *)(p_dev->p_devinfo->dac_reg_base);

    p_dev->p_devinfo         = p_devinfo;
    p_dev->dac_serve.p_funcs = &__g_dac_drvfuncs;
    p_dev->dac_serve.p_drv   = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    amhw_mm32_dac_disable(p_hw_dac);

    return (am_dac_handle_t)(&(p_dev->dac_serve));
}

/**
 * \brief DAC去初始化
 */
void am_mm32_dac_deinit (am_dac_handle_t handle)
{
    am_mm32_dac_dev_t *p_dev  = (am_mm32_dac_dev_t *)handle;
    amhw_mm32_dac_t *p_hw_dac = (amhw_mm32_dac_t *)(p_dev->p_devinfo->dac_reg_base);

    p_dev->dac_serve.p_funcs = NULL;
    p_dev->dac_serve.p_drv   = NULL;

    amhw_mm32_dac_disable(p_hw_dac);

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
