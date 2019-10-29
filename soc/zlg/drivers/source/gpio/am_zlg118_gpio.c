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
 * - 1.00 19-09-06  zp, first implementation
 * \endinternal
 */

#include "am_zlg118_gpio.h"
#include "am_gpio_util.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_bitops.h"
#include "am_zlg118_gpio_util.h"
#include "zlg118_pin.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define AM_ZLG118_GPIO_INVALID_PIN_MAP    0xFF

const am_zlg118_gpio_devinfo_t *p_gpio_devinfo = NULL;

/******************************************************************************
  全局变量
******************************************************************************/

/** \bruef 指向GPIO设备的指针 */
am_zlg118_gpio_dev_t *__gp_gpio_dev;

/*******************************************************************************
  公共函数
*******************************************************************************/

static int __am_zlg118_peripheral_afio_clear(int pin)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (NULL == p_gpio_devinfo) {
        return -AM_ENXIO;
    }

    if (p_gpio_devinfo->p_afio[pin] == AMHW_ZLG118_AFIO_NO) {
        return -AM_ENXIO;
    }

    amhw_zlg118_gpio_afio_set(p_hw_gpio, AMHW_ZLG118_AFIO_NO, pin);

    if(amhw_zlg118_gpio_afio_get(p_hw_gpio, pin) == AMHW_ZLG118_AFIO_NO)
    {
        p_gpio_devinfo->p_afio[pin] = AMHW_ZLG118_AFIO_NO;
    }

    return AM_OK;
}

/**
 * \brief 引脚功能配置
 *
 * \param[in] pin   : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] flags : 引脚功能
 *
 * \return AM_OK ：配置成功
 */
int am_gpio_pin_cfg (int pin, uint32_t flags)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;
    amhw_zlg118_gpiomode_t  pin_mode   = AMHW_ZLG118_GPIO_MODE_IPU;
    uint32_t                func = 0, mode = 0;

    /* IO口方向 */
    uint8_t dir = 0;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (AM_FALSE == __gp_gpio_dev->valid_flg) {
        return -AM_ENODEV;
    }

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    func = AM_GPIO_COM_FUNC_GET(flags);
    mode = AM_GPIO_COM_MODE_GET(flags);

    __am_zlg118_peripheral_afio_clear(pin);

    /* 标准层相关 */
    if (0 != func) {

        /* 使用标准层前先退回引脚为GPIO模式 */
        __am_zlg118_peripheral_afio_clear(pin);

        switch (func) {

        case AM_GPIO_INPUT_VAL:

            /* 设置方向为输入 */
            amhw_zlg118_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH_VAL:

            /* 推挽输出 */
            amhw_zlg118_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_ZLG118_GPIO_MODE_OUT_PP,
                                          pin);

            /* 输出高电平 */
            amhw_zlg118_gpio_pin_out_high(p_hw_gpio, pin);

            break;

        case AM_GPIO_OUTPUT_INIT_LOW_VAL:

            /* 推挽输出 */
            amhw_zlg118_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_ZLG118_GPIO_MODE_OUT_PP,
                                          pin);

            /* 输出低电平 */
            amhw_zlg118_gpio_pin_out_low(p_hw_gpio, pin);

            break;

        default:
            return -AM_ENOTSUP;
        }
    }

    if (mode != 0x00) {

        /* 获取输入输出方向 */
        dir = amhw_zlg118_gpio_pin_dir_get(p_hw_gpio, pin);

        switch (mode) {

        case AM_GPIO_PULL_UP_VAL:
            if (dir == 1) {

                /* 上拉输入模式  */
                pin_mode = AMHW_ZLG118_GPIO_MODE_IPU;

            } else {

                /* 输出没有上拉模式，设置为推挽模式 */
                pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_PP;
            }
            break;

        case AM_GPIO_PULL_DOWN_VAL:
            if (dir == 1) {

                /* 下拉输入模式  */
                pin_mode = AMHW_ZLG118_GPIO_MODE_IPD;

            } else {

                /* 输出没有下拉模式，设置为推挽模式 */
                pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_PP;
            }
            break;

        case AM_GPIO_FLOAT_VAL:
            if (dir == 1) {
                pin_mode = AMHW_ZLG118_GPIO_MODE_IN_FLOATING;
            } else {

                /* 输出没有浮空模式，设置为开漏模式 */
                pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_OD;
            }
            break;

        case AM_GPIO_OPEN_DRAIN_VAL:
            if (dir == 1) {
                /* 输入没有开漏模式，设置为浮空 */
                pin_mode = AMHW_ZLG118_GPIO_MODE_IN_FLOATING;
            } else {
                pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_OD;
            }
            break;

        case AM_GPIO_PUSH_PULL_VAL:
            if (dir == 1) {
                /* 输入没有推挽模式，设置为上拉 */
                pin_mode = AMHW_ZLG118_GPIO_MODE_IPU;
            } else {
                pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_PP;
            }
            break;

        default:
            return -AM_ENOTSUP;
        }

        amhw_zlg118_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /* 平台相关 */
    if (0 != (flags & AM_ZLG118_GPIO_MODE)) {

        /* 输入输出相应模式设置 */

        switch (AM_ZLG118_GPIO_MODE_GET(flags)) {

        case 0:
            /* 上拉输入 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_IPU;
            break;

        case 1:
            /* 下拉输入 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_IPD;
            break;

        case 2:
            /* 模拟输入 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_AIN;
            break;

        case 3:
            /* 浮空输入 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_IN_FLOATING;
            break;

        case 4:
            /* 推挽输出  */
            pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_PP;
            break;

        case 5:
            /* 开漏输出 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_OUT_OD;
            break;

        case 6:
            /* 模拟输出 */
            pin_mode = AMHW_ZLG118_GPIO_MODE_AOUT;
            break;

        default:
            return -AM_ENOTSUP;
        }

        amhw_zlg118_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /* 重映像设置 */
    if (0 != (flags & AM_ZLG118_GPIO_REMAP)) {

        __am_zlg118_peripheral_afio_clear(pin);

        amhw_zlg118_gpio_afio_set(
            p_hw_gpio,
            (amhw_zlg118_gpio_afio_t)AM_ZLG118_GPIO_REMAP_MODE_GET(flags),
            pin);
        p_gpio_devinfo->p_afio[pin] =
            (amhw_zlg118_gpio_afio_t)AM_ZLG118_GPIO_REMAP_MODE_GET(flags);
    }

    return AM_OK;
}

/**
 * \brief 获取GPIO引脚状态
 * \param[in] pin : 引脚编号，值为 PIO* (#PIOA_0)
 * \return 引脚状态
 */
int am_gpio_get (int pin)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (0 == amhw_zlg118_gpio_pin_dir_get(p_hw_gpio, pin)) {
        return amhw_zlg118_gpio_pin_output_get(p_hw_gpio, pin);
    } else {
        return amhw_zlg118_gpio_pin_input_get(p_hw_gpio, pin);
    }
}

/**
 * \brief 设置引脚输出状态
 *
 * \param[in] pin   : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] value : 引脚状态，参见
 *                    \ref grp_am_gpio_pin_level
 *
 * \retval  AM_OK   : 操作成功
 */
int am_gpio_set (int pin, int value)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (value == 0) {
        amhw_zlg118_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_zlg118_gpio_pin_out_high(p_hw_gpio, pin);
    }

    return AM_OK;
}

/**
 * \brief 翻转GPIO引脚输出状态
 * \param[in] pin : 引脚编号，值为 PIO* (#PIOA_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_toggle (int pin)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    uint8_t t = 0;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    t= amhw_zlg118_gpio_pin_output_get(p_hw_gpio, pin);

    if(1 == t) {
        amhw_zlg118_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_zlg118_gpio_pin_out_high(p_hw_gpio, pin);
    }

    return AM_OK;
}

/**
 * \brief 端口A的中断函数
 */
static void __port_a_int_isr (void * p_arg)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;
    uint8_t      pin       = 0;

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;


    /* 有中断触发 */
    for(pin = 0; pin < 16; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }
}

/**
 * \brief 端口B的中断函数
 */
static void __port_b_int_isr (void * p_arg)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;
    uint8_t      pin       = 0;

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;


    /* 有中断触发 */
    for(pin = 16; pin < 32; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }
}

/**
 * \brief 端口C/E的中断函数
 */
static void __port_c_e_int_isr (void * p_arg)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;
    uint8_t      pin       = 0;

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    /* 有中断触发 */
    for(pin = 32; pin < 48; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }

    /* 有中断触发 */
    for(pin = 64; pin < 80; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }
}

/**
 * \brief 端口D/F的中断函数
 */
static void __port_d_f_int_isr (void * p_arg)
{
    amhw_zlg118_gpio_t     *p_hw_gpio  = NULL;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;
    uint8_t      pin       = 0;

    p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    /* 有中断触发 */
    for(pin = 48; pin < 64; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }

    /* 有中断触发 */
    for(pin = 80; pin < 91; pin++) {
        if (1 == amhw_zlg118_gpio_pin_int_flag_get(p_hw_gpio, pin)) {

            /* 获取有关回调函数及参数 */
            pfn_isr   = p_gpio_devinfo->p_triginfo[pin].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[pin].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg_tmp);
            }
            /* 清中断标志 */
            amhw_zlg118_gpio_pin_int_flag_clr(p_hw_gpio, pin);
        }
    }
}

/**
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_zlg118_gpio_init (am_zlg118_gpio_dev_t           *p_dev,
                         const am_zlg118_gpio_devinfo_t *p_devinfo)
{
    uint8_t i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return -AM_EINVAL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;
    p_gpio_devinfo   = p_devinfo;

    if ((p_devinfo->p_afio == NULL) || (p_devinfo->inum_pin == NULL) ||
        (p_devinfo->p_infomap == NULL) || (p_devinfo->p_triginfo == NULL)) {
        p_dev->valid_flg = AM_FALSE;
        return -AM_EINVAL;
    }

    for (i = 0 ; i < p_devinfo->pin_count ; i++) {
        __am_zlg118_peripheral_afio_clear(i);
    }

    for (i = 0 ; i < p_devinfo->exti_num_max ; i++) {
        p_devinfo->p_infomap[i] = AM_ZLG118_GPIO_INVALID_PIN_MAP;
        p_devinfo->p_triginfo[i].p_arg = NULL;
        p_devinfo->p_triginfo[i].pfn_callback = NULL;
    }

    am_int_connect(p_devinfo->inum_pin[0], __port_a_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[1], __port_b_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[2], __port_c_e_int_isr, NULL);
    am_int_connect(p_devinfo->inum_pin[3], __port_d_f_int_isr, NULL);

    am_int_enable(p_devinfo->inum_pin[0]);
    am_int_enable(p_devinfo->inum_pin[1]);
    am_int_enable(p_devinfo->inum_pin[2]);
    am_int_enable(p_devinfo->inum_pin[3]);

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
void am_zlg118_gpio_deinit (void)
{
    uint8_t i;

    if (p_gpio_devinfo->pfn_plfm_deinit) {
        p_gpio_devinfo->pfn_plfm_deinit();
    }

    if (__gp_gpio_dev->valid_flg == AM_FALSE) {
        return;
    }

    for (i = 0 ; i < p_gpio_devinfo->pin_count ; i++) {
        __am_zlg118_peripheral_afio_clear(i);
    }

    for (i = 0 ; i < p_gpio_devinfo->exti_num_max ; i++) {
        p_gpio_devinfo->p_infomap[i] = AM_ZLG118_GPIO_INVALID_PIN_MAP;
        p_gpio_devinfo->p_triginfo[i].p_arg = NULL;
        p_gpio_devinfo->p_triginfo[i].pfn_callback = NULL;
    }

    am_int_disable(p_gpio_devinfo->inum_pin[0]);
    am_int_disable(p_gpio_devinfo->inum_pin[1]);
    am_int_disable(p_gpio_devinfo->inum_pin[2]);
    am_int_disable(p_gpio_devinfo->inum_pin[3]);

    am_int_disconnect(p_gpio_devinfo->inum_pin[0], __port_a_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[1], __port_b_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[2], __port_c_e_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[3], __port_d_f_int_isr, NULL);

    __gp_gpio_dev = NULL;
}

/**
 * \brief 配置GPIO引脚触发功能
 *
 * \param[in] pin  : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] flag : 配置参数，参见
 *                   \ref grp_am_gpio_pin_trigger_function
 *
 * \retval AM_OK   : 配置成功
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{
    uint8_t                 slot       = pin;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    if (p_gpio_devinfo->p_infomap[slot] != pin) {
        return -AM_EINVAL;
    }

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:
        break;

    case AM_GPIO_TRIGGER_HIGH:
        __gp_gpio_dev->int_type = AM_GPIO_TRIGGER_HIGH;
        break;

    case AM_GPIO_TRIGGER_LOW:
        __gp_gpio_dev->int_type = AM_GPIO_TRIGGER_LOW;
        break;

    case AM_GPIO_TRIGGER_RISE:
        __gp_gpio_dev->int_type = AM_GPIO_TRIGGER_RISE;
        break;

    case AM_GPIO_TRIGGER_FALL:
        __gp_gpio_dev->int_type = AM_GPIO_TRIGGER_FALL;
        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:
        __gp_gpio_dev->int_type = AM_GPIO_TRIGGER_BOTH_EDGES;
        break;

    default:
        return -AM_ENOTSUP;
    }

    return AM_OK;
}

/**
 * \brief 连接回调函数到引脚
 *
 * \param[in] pin          : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] pfn_callback : 回调函数指针
 * \param[in] p_arg        : 回调函数的入口参数
 *
 * \retval  AM_OK          : 操作成功
 */
int am_gpio_trigger_connect(int           pin,
                            am_pfnvoid_t  pfn_callback,
                            void         *p_arg)
{
    uint8_t slot = pin;
    int     key;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (AM_FALSE == __gp_gpio_dev->valid_flg) {
        return -AM_ENODEV;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    key = am_int_cpu_lock();

    if (p_gpio_devinfo->p_infomap[slot] == AM_ZLG118_GPIO_INVALID_PIN_MAP) {

        p_gpio_devinfo->p_infomap[slot] = pin;
        p_gpio_devinfo->p_triginfo[slot].p_arg = p_arg;
        p_gpio_devinfo->p_triginfo[slot].pfn_callback = pfn_callback;

    } else {

        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief 删除连接到引脚的回调函数
 *
 * \param[in] pin          : 引脚编号，值为 PIO* (#PIOA_0)
 * \param[in] pfn_callback : 回调函数指针
 * \param[in] p_arg        : 回调函数的入口参数
 *
 * \retval  AM_OK          : 操作成功
 */
int am_gpio_trigger_disconnect(int           pin,
                               am_pfnvoid_t  pfn_callback,
                               void         *p_arg)
{
    uint8_t                  slot        = pin;
    int key;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    key = am_int_cpu_lock();

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        p_gpio_devinfo->p_infomap[slot] = AM_ZLG118_GPIO_INVALID_PIN_MAP;
        p_gpio_devinfo->p_triginfo[slot].p_arg = NULL;
        p_gpio_devinfo->p_triginfo[slot].pfn_callback = NULL;

    } else {
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief 使能引脚触发中断
 * \param[in] pin : 引脚编号，值为 PIO* (#PIOA_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_trigger_on(int pin)
{
    uint8_t             slot       = pin;
    amhw_zlg118_gpio_t *p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        switch (__gp_gpio_dev->int_type) {

            case AM_GPIO_TRIGGER_HIGH:
                amhw_zlg118_gpio_pin_high_int_enable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_low_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_raising_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_falling_int_disable(p_hw_gpio,pin);
                break;

            case AM_GPIO_TRIGGER_LOW:
                amhw_zlg118_gpio_pin_high_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_low_int_enable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_raising_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_falling_int_disable(p_hw_gpio,pin);
                break;

            case AM_GPIO_TRIGGER_RISE:
                amhw_zlg118_gpio_pin_high_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_low_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_raising_int_enable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_falling_int_disable(p_hw_gpio,pin);
                break;

            case AM_GPIO_TRIGGER_FALL:
                amhw_zlg118_gpio_pin_high_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_low_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_raising_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_falling_int_enable(p_hw_gpio,pin);
                break;

            case AM_GPIO_TRIGGER_BOTH_EDGES:
                amhw_zlg118_gpio_pin_high_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_low_int_disable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_raising_int_enable(p_hw_gpio,pin);
                amhw_zlg118_gpio_pin_falling_int_enable(p_hw_gpio,pin);
                break;
        }
    } else {
        return -AM_ENXIO;
    }

    return AM_OK;
}

/**
 * \brief 禁能引脚触发中断
 * \param[in] pin : 引脚编号，值为 PIO* (#PIOA_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_trigger_off(int pin)
{
    uint8_t             slot       = pin;
    amhw_zlg118_gpio_t *p_hw_gpio  = (amhw_zlg118_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    /* 关闭某一引脚的所有触发中断 */
    if (p_gpio_devinfo->p_infomap[slot] == pin) {
        amhw_zlg118_gpio_pin_high_int_disable(p_hw_gpio,pin);
        amhw_zlg118_gpio_pin_low_int_disable(p_hw_gpio,pin);
        amhw_zlg118_gpio_pin_raising_int_disable(p_hw_gpio,pin);
        amhw_zlg118_gpio_pin_falling_int_disable(p_hw_gpio,pin);
    } else {
        return -AM_ENXIO;
    }

    return AM_OK;
}

/* end of file */
