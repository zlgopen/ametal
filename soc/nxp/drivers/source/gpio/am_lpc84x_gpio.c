/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief GPIO implementation
 * 
 * - GPIO引脚能够通过软件配置为输入或者输出；
 * - 复位后，所有GPIO引脚默认是输入且关闭中断；
 * - 引脚寄存器允许引脚单独设置。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-11-27  mem,modified.
 * - 1.01 15-07-28  zxl,modified.
 * - 1.00 15-07-10  zxl, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "am_int.h"
#include "am_gpio.h"
#include "am_gpio_util.h"
#include "am_lpc84x.h"
#include "am_lpc84x_gpio.h"
#include "am_lpc84x_gpio_util.h"
#include "hw/amhw_lpc84x_syscon.h"
#include "hw/amhw_lpc84x_gpio.h"
#include "hw/amhw_lpc84x_iocon.h"
#include "hw/amhw_lpc82x_pint.h"
#include "hw/amhw_lpc84x_swm.h"
#include "lpc84x_pin.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define __GPIO_INVALID_PIN_MAP    0xFF

/** \brief 未配置可配置功能标识 */
#define __GPIO_NO_MOVED_FUNC      0xFF

/** \brief 定义指向GPIO设备信息的指针 */
#define __GPIO_DEVINFO_DECL(p_devinfo, p_dev)  \
        const am_lpc84x_gpio_devinfo_t *p_devinfo = p_dev->p_devinfo

/******************************************************************************
  全局变量
******************************************************************************/

/** \bruef 指向GPIO设备的指针 */
am_lpc84x_gpio_dev_t *__gp_gpio_dev = NULL;

/*******************************************************************************
  公共函数
*******************************************************************************/

/**
 * \brief 引脚功能配置
 *
 * \param[in] pin   : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] flags : 功能码，为am_gpio.h,lpc8xx_pin.h的宏定义
 *
 */
int am_gpio_pin_cfg (int pin, uint32_t flags)
{
    uint32_t func, mode;
    uint32_t iocon_cfg;
    
    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    amhw_lpc84x_gpio_t             *p_hw_gpio  = NULL;
    amhw_lpc84x_iocon_t            *p_hw_iocon = NULL;
    amhw_lpc84x_swm_t              *p_hw_swm   = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    p_devinfo  = __gp_gpio_dev->p_devinfo;
    p_hw_gpio  = (amhw_lpc84x_gpio_t*)p_devinfo->gpio_regbase;
    p_hw_iocon = (amhw_lpc84x_iocon_t*)p_devinfo->iocon_regbase;
    p_hw_swm   = (amhw_lpc84x_swm_t*)p_devinfo->swm_regbase;
            
    /* 读取原IOCON数据 */
    iocon_cfg = amhw_lpc84x_iocon_pio_get(p_hw_iocon, pin);
    
    /* 读取标准层定义的信息 */
    func = AM_GPIO_COM_FUNC_GET(flags);
    mode = AM_GPIO_COM_MODE_GET(flags);

    /**
     * \brief 共有功能配置（优先级高）
     */
    if (func != 0x00) {
        
        /* 不再设置LPC8xx特有功能 */
        AM_BIT_CLR_MASK(flags, AM_LPC84X_GPIO_FUNC_MASK);
            
        /* 将引脚至于GPIO状态 */
        
        /* 如果之前有已配置的可转移功能，先取消 */
        if (__gp_gpio_dev->pin_moved_func[pin] != __GPIO_NO_MOVED_FUNC) {

            amhw_lpc84x_swm_moved_func_cfg(p_hw_swm,
                                          (amhw_lpc84x_swm_moved_func_t)\
                                          (__gp_gpio_dev->pin_moved_func[pin]),
                                          __GPIO_NO_MOVED_FUNC);

            /* 记录，当前引脚没有设置可配置功能 */
            __gp_gpio_dev->pin_moved_func[pin] = __GPIO_NO_MOVED_FUNC;
        }
        
        amhw_lpc84x_swm_pin_fixed_func_disable(p_hw_swm, pin);

        switch (func) {

        case AM_GPIO_INPUT:
            amhw_lpc84x_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH:
            amhw_lpc84x_gpio_pin_out_high(p_hw_gpio, pin);
            amhw_lpc84x_gpio_pin_dir_output(p_hw_gpio, pin);
            
            break;

        case AM_GPIO_OUTPUT_INIT_LOW:
            amhw_lpc84x_gpio_pin_out_low(p_hw_gpio, pin);
            amhw_lpc84x_gpio_pin_dir_output(p_hw_gpio, pin);

            break;

        default:
            break;
        }
    }

    if (mode != 0x00) {
        
        if (mode < AM_GPIO_OPEN_DRAIN_VAL) {
            
            /* 忽略中继模式 */
            AM_BIT_CLR_MASK(flags, AM_LPC84X_GPIO_REP_MASK);
            
            /* 清除开漏模式 */
            AM_BIT_CLR(iocon_cfg, 10);
            
            AM_BIT_CLR_MASK(iocon_cfg, AM_SBF(0x3, 3));
            iocon_cfg |= (((~mode) & 0x3) << 3);

          /* 设置开漏模式 */
        } else if (mode == AM_GPIO_OPEN_DRAIN_VAL) {
            AM_BIT_SET(iocon_cfg, 10);

          /* 其它模式不支持  */
        } else if (mode > AM_GPIO_OPEN_DRAIN_VAL) {
            return -AM_ENOTSUP;
        }
    }

    /**
     * \brief 平台功能配置
     */

    /* 需要设置引脚功能 */
    if ((flags & AM_LPC84X_GPIO_FUNC_MASK) != 0) {
        
        /* 如果之前有已配置的可转移功能，先取消 */
        if (__gp_gpio_dev->pin_moved_func[pin] != __GPIO_NO_MOVED_FUNC) {
            amhw_lpc84x_swm_moved_func_cfg(
                                       p_hw_swm,
                                       (amhw_lpc84x_swm_moved_func_t)(
                                       __gp_gpio_dev->pin_moved_func[pin]),
                                       __GPIO_NO_MOVED_FUNC);    
            
            __gp_gpio_dev->pin_moved_func[pin] = __GPIO_NO_MOVED_FUNC;
        }
        
        amhw_lpc84x_swm_pin_fixed_func_disable(p_hw_swm, pin);

        /* 特殊功能配置 */
        if (AM_LPC84X_GPIO_FUNC_GET(flags) >
            AM_LPC84X_GPIO_FUNC_GET(AM_LPC84X_GPIO_FUNC_GPIO)) {

            amhw_lpc84x_swm_pin_fixed_func_enable(
                p_hw_swm, 
                pin, 
                AM_LPC84X_GPIO_FUNC_CODE(AM_LPC84X_GPIO_FUNC_GET(flags)));

        /* 可转移功能 */
        }else if (AM_LPC84X_GPIO_FUNC_GET(flags) <
                  AM_LPC84X_GPIO_FUNC_GET(AM_LPC84X_GPIO_FUNC_GPIO)) {
            
            /* 存储当前引脚配置的功能 */
            __gp_gpio_dev->pin_moved_func[pin] = AM_LPC84X_GPIO_FUNC_GET(flags);

            amhw_lpc84x_swm_moved_func_cfg(
                p_hw_swm,
                (amhw_lpc84x_swm_moved_func_t)AM_LPC84X_GPIO_FUNC_GET(flags),
                pin);
        }
    }

    /* 需要设置为中继模式 */
    if ((flags & AM_LPC84X_GPIO_REP_MASK) != 0) {
             AM_BIT_SET_MASK(iocon_cfg, AM_SBF(0x3, 3));
    }

    /* 需要设置引脚迟滞 */
    if ((flags & AM_LPC84X_GPIO_HYS_MASK) != 0) {
        AM_BIT_CLR(iocon_cfg, 5);
        AM_BIT_SET_MASK(iocon_cfg, AM_SBF(AM_LPC84X_GPIO_HYS_GET(flags), 5));
    }

    /* 需要设置引脚输入反转 */
    if ((flags & AM_LPC84X_GPIO_INV_MASK) != 0) {
        AM_BIT_CLR(iocon_cfg, 6);
        AM_BIT_SET_MASK(iocon_cfg, AM_SBF(AM_LPC84X_GPIO_INV_GET(flags), 6));
    }

    /* 需要设置引脚I2C模式  */
    if ((flags & AM_LPC84X_GPIO_I2C_MASK) != 0) {
        AM_BIT_CLR_MASK(iocon_cfg, AM_SBF(0x3, 8));
        AM_BIT_SET_MASK(iocon_cfg ,AM_SBF(AM_LPC84X_GPIO_I2C_GET(flags), 8));
    }

    /* 需要设置引脚滤波周期 */
    if ((flags & AM_LPC84X_GPIO_FIL_CYC_MASK) != 0) {
        AM_BIT_CLR_MASK(iocon_cfg, AM_SBF(0x7, 11));
        AM_BIT_SET_MASK(iocon_cfg,
                        AM_SBF(AM_LPC84X_GPIO_FIL_CYC_GET(flags),
                        11));
    }

    /* 需要设置引脚滤波分频 */
    if ((flags & AM_LPC84X_GPIO_FIL_DIV_MASK) != 0) {
        AM_BIT_CLR_MASK(iocon_cfg, AM_SBF(0xF, 13));
        AM_BIT_SET_MASK(iocon_cfg, 
                        AM_SBF(AM_LPC84X_GPIO_FIL_DIV_GET(flags),
                        13));
    }

    /* 配置ICON信息 */
    amhw_lpc84x_iocon_pio_cfg(p_hw_iocon, pin, iocon_cfg);

    return AM_OK;
}


/** \brief 获取引脚状态 */
int am_gpio_get (int pin)
{
    amhw_lpc84x_gpio_t *p_hw_gpio  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    p_hw_gpio = (amhw_lpc84x_gpio_t*)__gp_gpio_dev->p_devinfo->gpio_regbase;
    
    return amhw_lpc84x_gpio_pin_level_get(p_hw_gpio, pin);
}

/** \brief 设置输出引脚状态 */
int am_gpio_set (int pin, int value)
{
    amhw_lpc84x_gpio_t *p_hw_gpio  = NULL;
        
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio = (amhw_lpc84x_gpio_t*)__gp_gpio_dev->p_devinfo->gpio_regbase;
    amhw_lpc84x_gpio_pin_out_set(p_hw_gpio, pin, value);

    return AM_OK;
}

/** \brief 引脚输出状态翻转 */
int am_gpio_toggle (int pin)
{
    amhw_lpc84x_gpio_t *p_hw_gpio  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio = (amhw_lpc84x_gpio_t*)__gp_gpio_dev->p_devinfo->gpio_regbase;
    amhw_lpc84x_gpio_pin_out_tog(p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief 引脚触发形式选择。
 */
int am_gpio_trigger_cfg (int pin, uint32_t flag)
{   
    int i;
    int slot = -1;
    
    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    amhw_lpc82x_pint_t             *p_hw_pint  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_devinfo = __gp_gpio_dev->p_devinfo;
    p_hw_pint = (amhw_lpc82x_pint_t *)p_devinfo->pint_regbase;
    
    for (i = 0; i < p_devinfo->pint_count; i++) {
        if ((p_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        return -AM_ENXIO;
    }

    /* 清零该引脚的上升和下降沿检测 */
    amhw_lpc82x_pint_edge_dec_clr(p_hw_pint, slot);

    switch (flag) {
        
    case AM_GPIO_TRIGGER_OFF:          /* 关闭触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                          AMHW_LPC82X_PINT_TRIGGER_OFF);
        break;

    case AM_GPIO_TRIGGER_HIGH:         /* 高电平触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                          AMHW_LPC82X_PINT_TRIGGER_HIGH);
        break;

    case AM_GPIO_TRIGGER_LOW:          /* 低电平触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                          AMHW_LPC82X_PINT_TRIGGER_LOW);
        break;

    case AM_GPIO_TRIGGER_RISE:         /* 上升沿触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                          AMHW_LPC82X_PINT_TRIGGER_RISE);
        break;

    case AM_GPIO_TRIGGER_FALL:         /* 下降沿触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                          AMHW_LPC82X_PINT_TRIGGER_FALL);
        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:   /* 双边沿触发 */
        amhw_lpc82x_pint_trigger_set(p_hw_pint,
                                          slot, 
                                     AMHW_LPC82X_PINT_TRIGGER_BOTH_EDGES);
        break;

    default:
        break;
    }

    return AM_OK;
}

/**
 * \brief GPIO 中断服务响应函数。
 * \param[in] p_arg : 当前中断映射编号。
 */
static void __gpio_eint_isr (void *p_arg)
{
    int          slot      = (int)p_arg;
    am_pfnvoid_t pfn_isr   = NULL;
    
    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    amhw_lpc82x_pint_t             *p_hw_pint  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return;
    }
    
    p_devinfo = __gp_gpio_dev->p_devinfo;
    p_hw_pint = (amhw_lpc82x_pint_t *)p_devinfo->pint_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr = p_devinfo->p_triginfo[slot].pfn_callback;
    p_arg   = p_devinfo->p_triginfo[slot].p_arg;

    if (pfn_isr != NULL) {
        pfn_isr(p_arg);
    }

    /* 如果是边沿敏感触发，则清边沿监测 */
    if ((p_hw_pint->isel & AM_BIT(slot)) == 0) {
        p_hw_pint->ist = AM_BIT(slot);
    }
}

/**
 * \brief 连接引脚中断回调函数
 */
int am_gpio_trigger_connect (int           pin,
                             am_pfnvoid_t  pfn_callback,
                             void         *p_arg)
{
    int          key;
    int          i;
    int          slot      = -1;
    int          err       = AM_OK;
    am_bool_t    already   = AM_FALSE;

    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    p_devinfo = __gp_gpio_dev->p_devinfo;
    
    /* 关闭CPU中断 */
    key = am_int_cpu_lock();
    
    for (i = 0; i < p_devinfo->pint_count; i++) {
        if ((p_devinfo->p_infomap)[i] == __GPIO_INVALID_PIN_MAP) {
            slot = i;
            break;
        }
        if (p_devinfo->p_infomap[i] == pin) {
            already = AM_TRUE;
            break;
        }
    }

    if (already) {

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    if (-1 != slot) {
        err = am_int_connect((p_devinfo->inum_pin)[slot],
                             (am_pfnvoid_t)__gpio_eint_isr,
                             (void *)slot);

        /*  保存触发引脚及回调函数 */
        if (AM_OK == err) {
            (p_devinfo->p_infomap)[slot]               = pin;
            (p_devinfo->p_triginfo)[slot].pfn_callback = pfn_callback;
            (p_devinfo->p_triginfo)[slot].p_arg        = p_arg;
        }

        amhw_lpc84x_syscon_pint_sel(slot, pin);

        /* 打开CPU中断 */
        am_int_cpu_unlock(key);
    }

    /* 打开CPU中断 */
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
    int key;
    int i;
    int slot = -1;
    
    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
        
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_devinfo = __gp_gpio_dev->p_devinfo;
    
    /* 关闭CPU中断 */  
    key = am_int_cpu_lock();
    
    for (i = 0; i < p_devinfo->pint_count; i++) {
        if ((p_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    if (slot == -1 || 
       ((p_devinfo->p_triginfo)[slot].pfn_callback != pfn_callback)) {

        /* 打开CPU中断 */ 
        am_int_cpu_unlock(key);
        return -AM_EINVAL;
    }

    am_int_disconnect((p_devinfo->inum_pin)[slot],
                      (am_pfnvoid_t)__gpio_eint_isr,
                      (void *)slot);

    (p_devinfo->p_infomap)[slot]               = __GPIO_INVALID_PIN_MAP;
    (p_devinfo->p_triginfo)[slot].pfn_callback = NULL;

    /* 打开CPU中断 */ 
    am_int_cpu_unlock(key);

    return AM_OK;
}

/**
 * \brief 使能引脚中断
 */
int am_gpio_trigger_on (int pin)
{
    int key;
    int i;
    int slot = -1;
    
    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    p_devinfo = __gp_gpio_dev->p_devinfo;
    
    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    for (i = 0; i < p_devinfo->pint_count; i++) {
        if ((p_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* 打开CPU中断t */ 
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    return am_int_enable((p_devinfo->inum_pin)[slot]);
}

/**
 * \brief 禁能引脚中断
 */
int am_gpio_trigger_off (int pin)
{
    int key;
    int i;
    int slot = -1;

    const am_lpc84x_gpio_devinfo_t *p_devinfo  = NULL;
    
    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }
    
    p_devinfo = __gp_gpio_dev->p_devinfo;

    /* 关闭CPU中断 */ 
    key = am_int_cpu_lock();
    
    for (i = 0; i < p_devinfo->pint_count; i++) {
        if ((p_devinfo->p_infomap)[i] == pin) {
            slot = i;
            break;
        }
    }

    /* 打开CPU中断 */
    am_int_cpu_unlock(key);

    if (slot == -1) {
        return -AM_ENXIO;
    }

    return am_int_disable((p_devinfo->inum_pin)[slot]);
}

/**
 * \brief GPIO初始化
 */
int am_lpc84x_gpio_init (am_lpc84x_gpio_dev_t           *p_dev,
                         const am_lpc84x_gpio_devinfo_t *p_devinfo)
{
    int i;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }
    
    if (__gp_gpio_dev != NULL) {
        return -AM_EPERM;
    }
    
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_gpio_dev     = p_dev;
    p_dev->valid_flg  = AM_TRUE;

    if ((p_devinfo->p_triginfo == NULL) ||
        (p_devinfo->p_infomap == NULL)) {
        p_dev->valid_flg = AM_FALSE;
    }

    if (p_dev->valid_flg) {
        for (i = 0; i < p_devinfo->pint_count; i++) {
            (p_devinfo->p_infomap)[i]               = __GPIO_INVALID_PIN_MAP;
            (p_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    /* 初始化引脚功能表 */
    for (i = 0; i < LPC84X_PIN_NUM; i++) {
        p_dev->pin_moved_func[i] = __GPIO_NO_MOVED_FUNC;
    }

    return AM_OK;
}

/**
 * \brief GPIO去初始化
 */
void am_lpc84x_gpio_deinit (void)
{
    int i;
    const am_lpc84x_gpio_devinfo_t *p_devinfo = NULL;
        
    if (__gp_gpio_dev == NULL) {
        return;
    }
    
    p_devinfo = __gp_gpio_dev->p_devinfo;
    
    if (__gp_gpio_dev->valid_flg) {
        for (i = 0; i < p_devinfo->pint_count; i++) {
            (p_devinfo->p_infomap)[i]               = 0;
            (p_devinfo->p_triginfo)[i].pfn_callback = NULL;
        }
    }

    for (i = 0; i < p_devinfo->pint_count; i++) {
        am_int_disable((p_devinfo->inum_pin)[i]);
    }
    
    for (i = 0; i < LPC84X_PIN_NUM; i++) {
        __gp_gpio_dev->pin_moved_func[i] = __GPIO_NO_MOVED_FUNC;
    }

    /* 恢复开关矩阵寄存器数据 */
    amhw_lpc84x_swm_reset(LPC84X_SWM);
    
    if (__gp_gpio_dev->p_devinfo->pfn_plfm_deinit) {
        __gp_gpio_dev->p_devinfo->pfn_plfm_deinit();
    }
    __gp_gpio_dev = NULL;
}

/* end of file */
