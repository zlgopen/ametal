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
 * \brief GPIO驱动，服务GPIO标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 19-02-22  ipk,first implementation
 * \endinternal
 */

#include "am_zmf159_gpio.h"
#include "am_gpio_util.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_bitops.h"
#include "hw/amhw_zmf159_exti.h"
#include "zmf159_pin.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define AM_ZMF159_GPIO_INVALID_PIN_MAP    0xFF

/** \brief 定义指向GPIO设备信息的指针 */
#define __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev)  \
        const am_zmf159_gpio_devinfo_t *p_gpio_devinfo = p_dev->p_devinfo

/******************************************************************************
  全局变量
******************************************************************************/

/** \bruef 指向GPIO设备的指针 */
am_zmf159_gpio_dev_t *__gp_gpio_dev;

/**
 * \brief 外部中断线0中断函数
 */
static void __port_exit0_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 0;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t   *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                 (amhw_zmf159_exti_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zmf159_exti_pending_clear(p_hw_exti,
                                   (amhw_zmf159_exti_line_num_t)(1ul << slot));
}

/**
 * \brief 外部中断线1中断函数
 */
static void __port_exit1_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 1;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                 (amhw_zmf159_exti_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zmf159_exti_pending_clear(p_hw_exti,
                                   (amhw_zmf159_exti_line_num_t)(1ul << slot));
}

/**
 * \brief 外部中断线2中断函数
 */
static void __port_exit2_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 2;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                 (amhw_zmf159_exti_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zmf159_exti_pending_clear(p_hw_exti,
                                   (amhw_zmf159_exti_line_num_t)(1ul << slot));
}

/**
 * \brief 外部中断线2中断函数
 */
static void __port_exit3_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 3;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                 (amhw_zmf159_exti_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zmf159_exti_pending_clear(p_hw_exti,
                                   (amhw_zmf159_exti_line_num_t)(1ul << slot));
}

/**
 * \brief 外部中断线4中断函数
 */
static void __port_exit4_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 4;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                 (amhw_zmf159_exti_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zmf159_exti_pending_clear(p_hw_exti,
                                   (amhw_zmf159_exti_line_num_t)(1ul << slot));
}

/**
 * \brief 外部中断线9_5中断函数
 */
static void __port_exit9_5_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    am_pfnvoid_t pfn_isr    = NULL;
    void         *p_arg_tmp = NULL;
    uint8_t      slot       = 5;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t   *)p_gpio_devinfo->exti_regbase;

    for (slot = 5 ; slot < 10 ; slot++) {
        if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                     (amhw_zmf159_exti_line_num_t)(1ul << slot))) {

            pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

            if (NULL != pfn_isr) {
                pfn_isr(p_arg_tmp);
            }

            amhw_zmf159_exti_pending_clear(p_hw_exti,
                                         (amhw_zmf159_exti_line_num_t)(1ul << slot));

            break;
        }
    }
}

/**
 * \brief 外部中断线5_10中断函数
 */
static void __port_exit15_10_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    am_pfnvoid_t pfn_isr    = NULL;
    void         *p_arg_tmp = NULL;
    uint8_t      slot       = 10;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zmf159_exti_t   *)p_gpio_devinfo->exti_regbase;

    for (slot = 10 ; slot < 16 ; slot++) {
        if (amhw_zmf159_exti_pr_read(p_hw_exti,
                                     (amhw_zmf159_exti_line_num_t)(1ul << slot))) {

            pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

            if (NULL != pfn_isr) {
                pfn_isr(p_arg_tmp);
            }

            amhw_zmf159_exti_pending_clear(p_hw_exti,
                                         (amhw_zmf159_exti_line_num_t)(1ul << slot));

            break;
        }
    }
}

/*******************************************************************************
  公共函数
*******************************************************************************/

/**
 * \brief 引脚功能配置
 *
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] flags     : 引脚功能
 *
 * \return AM_OK     ：配置成功
 */
int am_gpio_pin_cfg (int pin, uint32_t flags)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    /* IO口方向 */
    uint8_t dir = 0;

    uint32_t func = 0, mode = 0;

    uint32_t iocon_cfg = 0;

    amhw_zlg_gpio_t *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio = (amhw_zlg_gpio_t*)p_gpio_devinfo->gpio_regbase;

    func      = AM_GPIO_COM_FUNC_GET(flags);
    mode      = AM_GPIO_COM_MODE_GET(flags) << 3;

    /*
     * 共有功能配置
     */
    if (func != 0x00) {

        /* 表明不需要再设置方向 */
        flags &= ~AM_ZLG116_GPIO_DIR;

        /* 表明不需要再设置引脚复用功能，仅使用GPIO功能 */
        flags &= ~AM_ZMF159_GPIO_FUNEN;

        if (amhw_zlg_gpio_pin_afr_get(p_hw_gpio, pin) != AMHW_ZLG_GPIO_AF_DEFAULT) {

            /* 复用功能回到默认状态，表明使用GPIO功能 */
            amhw_zlg_gpio_pin_afr_set(p_hw_gpio, AMHW_ZLG_GPIO_AF_DEFAULT, pin);

        }

        switch (func) {

        case AM_GPIO_INPUT:
            AM_BITS_SET(iocon_cfg, 0, 2, 0X00);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH:

            amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);

            /* 输出时，若未配置管脚速率则默认为 10MHz */
            if ((flags & AM_ZMF159_GPIO_OUTRES_RATE) == 0) {
                AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
            }

            dir = 1;

            break;

        case AM_GPIO_OUTPUT_INIT_LOW:
            amhw_zlg_gpio_pin_out_low(p_hw_gpio, pin);

            /* 输出时，若未配置管脚速率则默认为 10MHz */
            if ((flags & AM_ZMF159_GPIO_OUTRES_RATE) == 0) {
                AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
            }

            dir = 1;

            break;

        default:
            break;
        }
    }

    if (mode != 0x00) {

        switch (mode) {

        case AM_GPIO_PULLUP:
            if (dir != 1) {

                /* 使OD位置为1, 表明是上拉 */
                amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);

                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IPU);
            } else {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_PP);
            }
            break;

        case AM_GPIO_PULLDOWN:
            if (dir != 1) {

                /* 使OD位置为0, 表明是下拉 */
                amhw_zlg_gpio_pin_out_low(p_hw_gpio, pin);
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IPD);
            } else {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_PP);
            }
            break;

        case AM_GPIO_FLOAT:
            if (dir != 1) {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IN_FLOATING);
            } else {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_OD);
            }
            break;

        case AM_GPIO_OPEN_DRAIN:
            if (dir == 0) {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IN_FLOATING);
            } else {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_OD);
            }
            break;

        case AM_GPIO_PUSH_PULL:
            if (dir == 0) {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IPU);
            } else {
                AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_PP);
            }
            break;

        default:
            return -AM_ENOTSUP;
        }
    }

    /*
     * 平台相关配置
     */

    /* 管脚的模式 */
    if ((flags & AM_ZMF159_GPIO_MODE) != 0) {

       switch (AM_ZMF159_GPIO_MODE_GET(flags)) {

       case 0:
           AM_BITS_SET(iocon_cfg, 0, 2, 0x00);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_AIN);

           break;

       case 1:
           AM_BITS_SET(iocon_cfg, 0, 2, 0x00);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IN_FLOATING);

           break;

       case 2:

           /* 使OD位置为0, 表明是下拉 */
           amhw_zlg_gpio_pin_out_low(p_hw_gpio, pin);
           AM_BITS_SET(iocon_cfg, 0, 2, 0x00);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IPD);

           break;

       case 3:

           /* 使OD位置为1, 表明是上拉 */
           amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);
           AM_BITS_SET(iocon_cfg, 0, 2, 0x00);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_IPU);

           break;

       case 4:
           AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_PP);

           break;

       case 5:
           AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_OUT_OD);

           break;

       case 6:
           AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_AF_PP);

           break;

       case 7:
           AM_BITS_SET(iocon_cfg, 0, 2, AMHW_ZLG_GPIO_SPEED_10MHz);
           AM_BITS_SET(iocon_cfg, 2, 2, AMHW_ZLG_GPIO_MODE_AF_OD);
           break;

       default:
           return -AM_ENOTSUP;
       }
    }

    /* 设置管脚的输出速率 */
    if ((flags & AM_ZMF159_GPIO_OUTRES_RATE) != 0) {

        AM_BITS_SET(iocon_cfg, 0, 2, AM_ZMF159_GPIO_OUTRES_RATE_GET(flags));

    }

    /* 设置管脚的复用功能 */
    if ((flags & AM_ZMF159_GPIO_FUNEN) != 0) {
        amhw_zlg_gpio_pin_afr_set(p_hw_gpio, AM_ZMF159_GPIO_FUNC_GET(flags), pin);
    }

    /* 设置复用功能时管脚模式 */
    amhw_zlg_gpio_pin_set(p_hw_gpio, iocon_cfg, pin);

    return AM_OK;
}

/**
 * \brief 获取引脚状态
 *
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 *
 * \return 无
 */
int am_gpio_get (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg_gpio_t *p_hw_gpio = (amhw_zlg_gpio_t *)p_gpio_devinfo->gpio_regbase;

    return amhw_zlg_gpio_pin_get(p_hw_gpio, pin);
}

/**
 * \brief 设置输出引脚状态
 *
 * \param[in] pin       : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] value     : 引脚值
 *
 * \return AM_OK     ：设置成功
 */
int am_gpio_set (int pin, int value)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg_gpio_t *p_hw_gpio = (amhw_zlg_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (value == 0) {
        amhw_zlg_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);

    }
    return AM_OK;
}

/**
 * \brief 引脚输出状态反转
 */
int am_gpio_toggle (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg_gpio_t *p_hw_gpio = (amhw_zlg_gpio_t *)p_gpio_devinfo->gpio_regbase;

    amhw_zlg_gpio_pin_toggle(p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief 引脚触发形式设置
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int  port = 0;
    int  slot = -1;

    amhw_zlg_gpio_t   *p_hw_gpio   = NULL;
    amhw_zlg_syscfg_t *p_hw_syscfg = NULL;
    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    port = pin >> 4;       /* 获得引脚对应的端口号 */

    if (port > 3) {        /* 端口不存在 */
        return -AM_EINVAL;
    }

    p_hw_gpio   = (amhw_zlg_gpio_t   *)p_gpio_devinfo->gpio_regbase;
    p_hw_syscfg = (amhw_zlg_syscfg_t *)p_gpio_devinfo->syscfg_regbase;
    p_hw_exti   = (amhw_zmf159_exti_t   *)p_gpio_devinfo->exti_regbase;

    /* 还没有开始配置管脚 */
    if ((p_gpio_devinfo->p_infomap)[pin & 0x0f] == pin) {
        slot = pin & 0x0f;
    } else {
        return -AM_ENXIO;
    }

    /* 复用功能回到默认状态，表明使用GPIO功能 */
    amhw_zlg_gpio_pin_afr_set(p_hw_gpio, AMHW_ZLG_GPIO_AF_DEFAULT, pin);

    /* 可以添加该中断引脚配置为GPIO功能的函数代码 */

    /* 设置管脚为输入方向 */
    amhw_zlg_gpio_pin_dir_input(p_hw_gpio, pin);

    /* 管脚为上拉/下拉模式 */
    amhw_zlg_gpio_pin_mode_set(p_hw_gpio, AMHW_ZLG_GPIO_MODE_IPD, pin);

    /*
     * 中断线0只能连一个端口编号为0的GPIO引脚，比如说，PIOA_0连接到了中断线0，
     * PIOB_0、PIOC_0、PIOD_0就不能连到中断线0, 但此时PIOA_1可以连接到中断线1。
     */
    amhw_zlg_syscfg_exti_line_config(p_hw_syscfg, port, slot);

    /* 清除中断线配置 */
    amhw_zmf159_exti_imr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
    amhw_zmf159_exti_emr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

    /* 清除中断线触发方式配置 */
    amhw_zmf159_exti_rtsr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
    amhw_zmf159_exti_ftsr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:           /* 关闭触发 */
        amhw_zmf159_exti_imr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
        amhw_zmf159_exti_emr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
        break;

    case AM_GPIO_TRIGGER_HIGH:          /* 高电平触发 */
        return -AM_ENOTSUP;

    case AM_GPIO_TRIGGER_LOW:           /* 低电平触发 */
        return -AM_ENOTSUP;

    case AM_GPIO_TRIGGER_RISE:          /* 上升沿触发 */

        /* 使OD位置为0, 表明是下拉 */
        amhw_zlg_gpio_pin_out_low(p_hw_gpio, pin);

        /* rising  edge */
        amhw_zmf159_exti_rtsr_set(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

        break;

    case AM_GPIO_TRIGGER_FALL:          /* 下降沿触发 */

        /* 使OD位置为1, 表明是上拉 */
        amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);

        /* falling  edge */
        amhw_zmf159_exti_ftsr_set(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:    /* 双边沿触发 */

        /* 使OD位置为1, 表明是上拉 */
        amhw_zlg_gpio_pin_out_high(p_hw_gpio, pin);

        /* rising falling edge */
        amhw_zmf159_exti_rtsr_set(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
        amhw_zmf159_exti_ftsr_set(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

        break;

    default:
        break;
    }

    return AM_OK;
}

/**
 * \brief 使能引脚中断。
 */
int am_gpio_trigger_on (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int slot = -1;
    int port = 0;

    int err = 0;

    uint32_t extipinsource = 0;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    port = pin >> 4;       /* 获得引脚对应的端口号 */

    if (port > 3) {        /* 端口不存在 */
        return -AM_EINVAL;
    }

    extipinsource = pin & 0x0f;

    /* 还没有开始配置管脚 */
    if ((p_gpio_devinfo->p_infomap)[extipinsource] == pin) {
        slot = extipinsource;
    } else {
        return -AM_ENXIO;
    }

    p_hw_exti   = (amhw_zmf159_exti_t   *)p_gpio_devinfo->exti_regbase;

    /* 清引脚中断线标志位 */
    amhw_zmf159_exti_pending_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

    /* 设置开放中断线请求 */
    amhw_zmf159_exti_imr_set(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

    return err;
}

/**
 * \brief 禁能引脚中断。
 */
int am_gpio_trigger_off (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int slot = -1;
    int port = 0;

    uint32_t extipinsource = 0;

    amhw_zmf159_exti_t   *p_hw_exti   = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    port = pin >> 4;       /* 获得引脚对应的端口号 */

    if (port > 3) {        /* 端口不存在 */
        return -AM_EINVAL;
    }

    extipinsource = pin & 0x0f;

    /* 还没有开始配置管脚 */
    if ((p_gpio_devinfo->p_infomap)[extipinsource] == pin) {
        slot = extipinsource;
    } else {
        return -AM_ENXIO;
    }

    p_hw_exti = (amhw_zmf159_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 屏蔽中断线请求 */
    amhw_zmf159_exti_imr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));
    amhw_zmf159_exti_emr_clear(p_hw_exti, (amhw_zmf159_exti_line_num_t)(1UL << slot));

    return AM_OK;
}

/**
 * \brief 连接引脚中断回调函数
 */
int am_gpio_trigger_connect (int           pin,
                             am_pfnvoid_t  pfn_callback,
                             void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int      key;

    int      slot      = -1;
    int      port      = 0;

    uint32_t extipinsource = 0;

    am_bool_t already = AM_FALSE;

    extipinsource = pin & 0x0f;

    port = pin >> 4;

    if (port > 3) {
        return -AM_EINVAL;
    }

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    if ((p_gpio_devinfo->p_infomap)[extipinsource] == AM_ZMF159_GPIO_INVALID_PIN_MAP) {
        slot = extipinsource;
    }  else {
        already = AM_TRUE;
    }

    if (already) {

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    if (-1 != slot) {

        /*  保存触发引脚及回调函数 */
        (p_gpio_devinfo->p_infomap)[slot]               = pin;
        (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = pfn_callback;
        (p_gpio_devinfo->p_triginfo)[slot].p_arg        = p_arg;

        /* 打开中断 */
        am_int_cpu_unlock(key);

    }

     return AM_OK;
}

/**
 * \brief 删除引脚中断回调函数连接
 */
int am_gpio_trigger_disconnect (int           pin,
                                am_pfnvoid_t  pfn_callback,
                                void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int key;

    int port = 0;

    int slot = -1;

    uint32_t extipinsource = 0;

    port = pin >> 4;

    if (port > 3) {
     return -AM_EINVAL;
    }

    extipinsource = pin & 0x0f;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    if ((p_gpio_devinfo->p_infomap)[extipinsource] == pin) {
      slot = extipinsource;
    }

    if (slot == -1) {

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);

        return -AM_EINVAL;
    }

    (p_gpio_devinfo->p_infomap)[slot]               = AM_ZMF159_GPIO_INVALID_PIN_MAP;
    (p_gpio_devinfo->p_triginfo)[slot].pfn_callback = NULL;

    /* 打开CPU中断 */
    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_zmf159_gpio_init (am_zmf159_gpio_dev_t           *p_dev,
                         const am_zmf159_gpio_devinfo_t *p_devinfo)
{
    uint8_t i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return -AM_EINVAL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    if ((p_devinfo->inum_pin == NULL)  ||
        (p_devinfo->p_infomap == NULL) ||
        (p_devinfo->p_triginfo == NULL)) {
        p_dev->valid_flg = AM_FALSE;
        return -AM_EINVAL;
    }

//    for (i = 0 ; i < p_devinfo->pin_count ; i++) {
//        if ((i == PIOA_13) || (i == PIOA_14) || (i == PIOA_15) ||
//             (i == PIOB_3) || (i == PIOB_4)) {
//            p_devinfo->p_remap[i] = AMHW_ZLG217_SWJ_CFG;
//        } else {
//            p_devinfo->p_remap[i] = AMHW_ZLG217_NO_REMAP;
//        }
//    }

    for (i = 0 ; i < p_devinfo->exti_num_max ; i++) {
        p_devinfo->p_infomap[i] = AM_ZMF159_GPIO_INVALID_PIN_MAP;
        p_devinfo->p_triginfo[i].p_arg = NULL;
        p_devinfo->p_triginfo[i].pfn_callback = NULL;
    }

    am_int_connect(p_devinfo->inum_pin[0], __port_exit0_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[1], __port_exit1_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[2], __port_exit2_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[3], __port_exit3_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[4], __port_exit4_int_isr, NULL);

    am_int_connect(p_devinfo->inum_pin[5], __port_exit9_5_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[6], __port_exit15_10_int_isr, NULL);

    am_int_enable(p_devinfo->inum_pin[0]);
    am_int_enable(p_devinfo->inum_pin[1]);
    am_int_enable(p_devinfo->inum_pin[2]);
    am_int_enable(p_devinfo->inum_pin[3]);
    am_int_enable(p_devinfo->inum_pin[4]);
    am_int_enable(p_devinfo->inum_pin[5]);
    am_int_enable(p_devinfo->inum_pin[6]);

    p_dev->valid_flg = AM_TRUE;

    __gp_gpio_dev = p_dev;

    return AM_OK;
}

/**
 * \brief GPIO去初始化
 *
 * \param[in] 无
 *
 * \return 无
 */
void am_zmf159_gpio_deinit (void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int i = 0;

    if (__gp_gpio_dev->valid_flg) {
        for (i = 0; i < p_gpio_devinfo->exti_num_max; i++) {
            (p_gpio_devinfo->p_infomap)[i]               = AM_ZMF159_GPIO_INVALID_PIN_MAP;
            (p_gpio_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    am_int_disable((p_gpio_devinfo->inum_pin)[0]);
    am_int_disable((p_gpio_devinfo->inum_pin)[1]);
    am_int_disable((p_gpio_devinfo->inum_pin)[2]);

    am_int_disconnect(p_gpio_devinfo->inum_pin[0], __port_exit0_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[1], __port_exit1_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[2], __port_exit2_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[3], __port_exit3_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[4], __port_exit4_int_isr, NULL);

    am_int_disconnect(p_gpio_devinfo->inum_pin[5], __port_exit9_5_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[6], __port_exit15_10_int_isr, NULL);

    if (__gp_gpio_dev->p_devinfo->pfn_plfm_deinit) {
        __gp_gpio_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
