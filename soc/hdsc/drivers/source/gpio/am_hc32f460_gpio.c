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

#include <am_hc32f460_gpio.h>
#include <am_hc32f460_gpio_util.h>
#include "am_gpio_util.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_bitops.h"
#include "hc32f460_irq_handle.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define AM_HC32F460_GPIO_INVALID_PIN_MAP    0xFF

/** \brief 定义指向GPIO设备信息的指针 */
#define __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev)  \
        const am_hc32f460_gpio_devinfo_t *p_gpio_devinfo = p_dev->p_devinfo

/******************************************************************************
  全局变量
******************************************************************************/

/** \bruef 指向GPIO设备的指针 */
am_hc32f460_gpio_dev_t *__gp_gpio_dev;

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
    amhw_hc32f460_gpio_t *p_hw_gpio  = NULL;
    amhw_hc32f460_gpio_mode_t pin_mode   = AMHW_HC32F460_GPIO_MODE_IPU;

    uint8_t pin_pos = 0u;
    en_portx_t en_portx = AMHW_HC32F460_GPIO_PORT_A;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    __pin_calc(pin, &en_portx, &pin_pos);
    p_hw_gpio = (amhw_hc32f460_gpio_t*)p_gpio_devinfo->gpio_regbase;

    func      = AM_GPIO_COM_FUNC_GET(flags);
    mode      = AM_GPIO_COM_MODE_GET(flags);

    /* 标准层相关 */
    if (func != AM_GPIO_FUNC_INVALID_VAL) {
        /* 使用标准层前先退回引脚为GPIO模式 */
        if (amhw_hc32f460_gpio_pin_afr_get(p_hw_gpio, pin) != AMHW_HC32F460_AFIO_GPIO) {

            /* 复用功能回到默认状态，表明使用GPIO功能 */
            amhw_hc32f460_gpio_pin_afr_set(p_hw_gpio, AMHW_HC32F460_AFIO_GPIO, pin);

        }

        switch (func) {

        case AM_GPIO_INPUT_VAL:
            amhw_hc32f460_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH_VAL:
            /* 输出高电平 */
            amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, pin);
            /* 推挽输出 */
            amhw_hc32f460_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_HC32F460_GPIO_MODE_OUT_PP,
                                          pin);
            break;

        case AM_GPIO_OUTPUT_INIT_LOW_VAL:
            /* 输出低电平 */
            amhw_hc32f460_gpio_pin_out_low(p_hw_gpio, pin);
            /* 推挽输出 */
            amhw_hc32f460_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_HC32F460_GPIO_MODE_OUT_PP,
                                          pin);

            break;

        default:
            return -AM_ENOTSUP;
        }
    }
    
    /* 获取输入输出方向 */
    
    if (mode != AM_GPIO_MODE_INVALID_VAL) {
        dir = amhw_hc32f460_gpio_pin_dir_get(p_hw_gpio, pin);

        switch (mode) {              
        case AM_GPIO_PULL_UP_VAL:
            if (dir != 1) {
                /* 上拉输入模式  */
                pin_mode = AMHW_HC32F460_GPIO_MODE_IPU;
            } else {
                /* 输出没有上拉模式，设置为推挽模式 */
                pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_PP;
            }
            break;

        case AM_GPIO_PULL_DOWN_VAL:  //HC32F46X cannot Support 
            break;

        case AM_GPIO_FLOAT_VAL:    //HC32F46X cannot Support 
            if (dir != 1) {
                pin_mode = AMHW_HC32F460_GPIO_MODE_IN_FLOATING;
            } else {
                /* 输出没有浮空模式，设置为开漏模式 */
                pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_OD;
            }
            break;

        case AM_GPIO_OPEN_DRAIN_VAL:
            if (dir != 1) {
                /* 输入没有开漏模式，设置为浮空 */
                pin_mode = AMHW_HC32F460_GPIO_MODE_IN_FLOATING;
            } else {
                pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_OD;
            }

            break;

        case AM_GPIO_PUSH_PULL_VAL:
            if (dir != 1) {
                /* 输入没有推挽模式，设置为上拉 */
                pin_mode = AMHW_HC32F460_GPIO_MODE_IPU;
            } else {
                pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_PP;
            }
            break;

        default:
            return -AM_ENOTSUP;
        }        
        amhw_hc32f460_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /*
     * 平台相关配置
     */

    /* 管脚的模式 */
    if ((flags & AM_HC32F460_GPIO_MODE) != 0) {

       switch (AM_HC32F460_GPIO_MODE_GET(flags)) {

       case 0:
           /* 模拟输入 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_AIN;
           break;

       case 1:
           /* 浮空输入 */       
           pin_mode = AMHW_HC32F460_GPIO_MODE_IN_FLOATING;       
           break;

       case 2:  
           /* 下拉输入模式  */
           pin_mode = AMHW_HC32F460_GPIO_MODE_IPD;                             
           break;

       case 3:
           /* 上拉输入模式  */
           pin_mode = AMHW_HC32F460_GPIO_MODE_IPU;                  
           break;

       case 4:
           /* 通用推挽输出模式 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_PP;                  
           break;

       case 5:
           /* 开漏输出模式 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_OUT_OD;
           break;

       case 6:   
           /* 复用推挽输出模式 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_AOUT;
           break;

       case 7:  
           /* 复用开漏输出模式 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_AF_PP;
           break;
           
       case 8:  
           /* 复用开漏输出模式 */
           pin_mode = AMHW_HC32F460_GPIO_MODE_AF_OD;
           break;

       default:
           return -AM_ENOTSUP;
       }       
       amhw_hc32f460_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /* 设置管脚的输出速率 */
    if ((flags & AM_HC32F460_GPIO_OUTRES_RATE) != 0) {
        amhw_hc32f460_gpio_pin_driver_capability(p_hw_gpio,(amhw_hc32f460_gpio_speed_mode_t)(AM_HC32F460_GPIO_OUTRES_RATE_GET(flags)), pin);
    }

    /* 设置管脚的复用功能 */
    if ((flags & AM_HC32F460_GPIO_FUNEN) != 0) {
        amhw_hc32f460_gpio_pin_afr_set(p_hw_gpio, AM_HC32F460_GPIO_FUNC_GET(flags), pin);
    }
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

    amhw_hc32f460_gpio_t *p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;

    return amhw_hc32f460_gpio_pin_get(p_hw_gpio, pin);
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

    amhw_hc32f460_gpio_t *p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (value == 0) {
        amhw_hc32f460_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, pin);

    }
    return AM_OK;
}

/**
 * \brief 引脚输出状态反转
 */
int am_gpio_toggle (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_hc32f460_gpio_t *p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;

    amhw_hc32f460_gpio_pin_toggle(p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief 引脚触发形式设置
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{
    uint8_t pin_pos = 0u;
    en_portx_t en_portx = AMHW_HC32F460_GPIO_PORT_A;

    __pin_calc(pin, &en_portx, &pin_pos);

    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_hc32f460_gpio_t     *p_hw_gpio  = NULL;

    uint8_t                 slot       = pin & 0x0f;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;


    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    if (p_gpio_devinfo->p_infomap[slot] != pin) {
        return -AM_EINVAL;
    }

    /* 设置管脚为输入方向 */
    amhw_hc32f460_gpio_pin_dir_input(p_hw_gpio, pin);

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:
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
 * \brief 使能引脚中断。
 */
int am_gpio_trigger_on (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    uint8_t               slot       = pin & 0x0f;
    amhw_hc32f460_gpio_t *p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        /* 使能引脚外部中断输入许可 */
        amhw_hc32f460_gpio_pin_ext_int_enable(p_hw_gpio, pin);

        switch (__gp_gpio_dev->int_type) {

            case AM_GPIO_TRIGGER_OFF:
            break;

            case AM_GPIO_TRIGGER_LOW:
                /* 配置为上拉 */
                amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, pin);
                amhw_hc32f460_intc_pin_ext_int_trigger_cfg(slot, AMHW_HC32F460_INTC_PIN_EXT_INT_LOW);
                break;

            case AM_GPIO_TRIGGER_RISE:
                /* 配置为下拉 */
                amhw_hc32f460_gpio_pin_out_low(p_hw_gpio, pin);
                /* rising  edge */
                amhw_hc32f460_intc_pin_ext_int_trigger_cfg(slot, AMHW_HC32F460_INTC_PIN_EXT_INT_RISE);
                break;

            case AM_GPIO_TRIGGER_FALL:
                /* 配置为上拉 */
                amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, pin);
                /* falling  edge */
                amhw_hc32f460_intc_pin_ext_int_trigger_cfg(slot, AMHW_HC32F460_INTC_PIN_EXT_INT_FALL);
                break;
            case AM_GPIO_TRIGGER_BOTH_EDGES:
                /* 配置为上拉 */
                amhw_hc32f460_gpio_pin_out_high(p_hw_gpio, pin);
                /* both  edge */
                amhw_hc32f460_intc_pin_ext_int_trigger_cfg(slot, AMHW_HC32F460_INTC_PIN_EXT_INT_BOTHEDGE);
                break;
        }
    } else {
        return -AM_ENXIO;
    }

    return AM_OK;
}

/**
 * \brief 禁能引脚中断。
 */
int am_gpio_trigger_off (int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_hc32f460_gpio_t *p_hw_gpio = (amhw_hc32f460_gpio_t *)p_gpio_devinfo->gpio_regbase;

    amhw_hc32f460_gpio_pin_ext_int_disable(p_hw_gpio, pin);
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
    uint8_t                  slot        = pin & 0x0f;
    int key;

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

    if (p_gpio_devinfo->p_infomap[slot] == AM_HC32F460_GPIO_INVALID_PIN_MAP) {

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
 * \brief 删除引脚中断回调函数连接
 */
int am_gpio_trigger_disconnect (int           pin,
                                am_pfnvoid_t  pfn_callback,
                                void         *p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t                  slot        = pin & 0x0f;
    int key;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    key = am_int_cpu_lock();

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        p_gpio_devinfo->p_infomap[slot] = AM_HC32F460_GPIO_INVALID_PIN_MAP;
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
 * \brief GPIO初始化
 *
 * \param[in] p_dev     : 指向GPIO设备的指针
 * \param[in] p_devinfo : 指向GPIO设备信息的指针
 *
 * \retval AM_OK : 操作成功
 */
int am_hc32f460_gpio_init (am_hc32f460_gpio_dev_t           *p_dev,
                           const am_hc32f460_gpio_devinfo_t *p_devinfo)
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

    for (i = 0 ; i < p_devinfo->exti_num_max ; i++) {
        p_devinfo->p_infomap[i] = AM_HC32F460_GPIO_INVALID_PIN_MAP;
        p_devinfo->p_triginfo[i].p_arg = NULL;
        p_devinfo->p_triginfo[i].pfn_callback = NULL;
    }

    am_int_connect(p_devinfo->inum_pin[0], IRQ128_Handler, NULL);
    amhw_hc32f460_intc_int_vssel_bits_set(p_devinfo->inum_pin[0], 0xFFFF);
    am_int_enable(p_devinfo->inum_pin[0]);

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
void am_hc32f460_gpio_deinit (void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    int i = 0;

    if (__gp_gpio_dev->valid_flg) {
        for (i = 0; i < p_gpio_devinfo->exti_num_max; i++) {
            (p_gpio_devinfo->p_infomap)[i]               = AM_HC32F460_GPIO_INVALID_PIN_MAP;
            (p_gpio_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    am_int_disable(p_gpio_devinfo->inum_pin[0]);
    am_int_disconnect(p_gpio_devinfo->inum_pin[0], IRQ128_Handler, NULL);


    if (__gp_gpio_dev->p_devinfo->pfn_plfm_deinit) {
        __gp_gpio_dev->p_devinfo->pfn_plfm_deinit();
    }
}

void Extint00_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[0].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[0].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint01_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[1].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[1].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint02_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[2].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[2].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint03_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[3].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[3].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint04_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[4].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[4].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint05_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[5].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[5].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint06_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[6].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[6].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}


void Extint07_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[7].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[7].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint08_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[8].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[8].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint09_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[9].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[9].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint10_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[10].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[10].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}


void Extint11_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[11].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[11].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}

void Extint12_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[12].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[12].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}


void Extint13_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[13].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[13].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}


void Extint14_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[14].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[14].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}


void Extint15_IrqHandler(void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[15].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[15].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg_tmp);
    }
}
/* end of file */
