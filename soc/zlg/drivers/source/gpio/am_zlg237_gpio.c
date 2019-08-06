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
 * - 1.00 19-07-04  zc,first implementation
 * \endinternal
 */

#include "am_zlg237_gpio.h"
#include "am_gpio_util.h"
#include "am_gpio.h"
#include "am_int.h"
#include "am_bitops.h"
#include "am_zlg237_gpio_util.h"
#include "amhw_zlg237_afio.h"
#include "amhw_zlg237_exti.h"
#include "zlg237_pin.h"

/*******************************************************************************
* 私有定义
*******************************************************************************/

/** \brief 中断未连接标识 */
#define AM_ZLG237_GPIO_INVALID_PIN_MAP    0xFF

/** \brief 定义指向GPIO设备信息的指针 */
#define __GPIO_DEVINFO_DECL(p_gpio_devinfo, p_dev)  \
        const am_zlg237_gpio_devinfo_t *p_gpio_devinfo = p_dev->p_devinfo

/******************************************************************************
  全局变量
******************************************************************************/

/** \bruef 指向GPIO设备的指针 */
am_zlg237_gpio_dev_t *__gp_gpio_dev;

/*******************************************************************************
  公共函数 关闭外设重映射
*******************************************************************************/

static int __am_zlg237_peripheral_remap_clear(int pin)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_zlg237_afio_remap_peripheral_t peripheral;
    amhw_zlg237_afio_t                  *p_hw_afio;
    uint8_t i = 0;

    if (NULL == p_gpio_devinfo) {
        return -AM_ENXIO;
    }

    p_hw_afio  = (amhw_zlg237_afio_t *)p_gpio_devinfo->afio_regbase;
    peripheral = p_gpio_devinfo->p_remap[pin];

    if (peripheral == AMHW_ZLG237_NO_REMAP) {
        return -AM_ENXIO;
    }

    /* JATG / SWD 引脚选择性关闭 */
    if (peripheral != AMHW_ZLG237_SWJ_CFG) {

        /*
         * 将外设在GPIO中重映像的改组引脚全部重置为AMHW_ZLG237_NO_REMAP
         * 表明改组引脚现在已没有重映像
         */

        for (i = 0 ; i < p_gpio_devinfo->pin_count ; i++) {
            if (p_gpio_devinfo->p_remap[i] == peripheral) {
                p_gpio_devinfo->p_remap[i] = AMHW_ZLG237_NO_REMAP;
            }
        }

    } else {
        /*
         * JTMS/SWDIO -- PA13   JTCK/SWCLK    -- PA14
         * JTDI       -- PA15   JTDO/TRACESWO -- PB3
         * JNTRST     -- PB4
         *
         * 0  --  无可用引脚
         * 1  --  PB4
         * 2  --  PB3/PB4/PA15
         * 4  --  PB3/PB4/PA15/PA14/PA13
         */
        if ((PIOA_13 == pin) || (PIOA_14 == pin)) {

            amhw_zlg237_afio_swj_cfg_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_SWJ_CFG_4);

            /* 清除引脚重映像信息 */
            for (i = PIOA_13 ; i < PIOB_5 ; i++) {
                if (p_gpio_devinfo->p_remap[i] == AMHW_ZLG237_SWJ_CFG) {
                    p_gpio_devinfo->p_remap[i] = AMHW_ZLG237_NO_REMAP;
                }
            }

            /* 如果使用A15/B3 且模式不是位于7 */
        } else if (((PIOA_15 == pin) || (PIOB_3 == pin)) &&
                 (p_gpio_devinfo->p_remap[PIOA_13] != AMHW_ZLG237_NO_REMAP)){

            amhw_zlg237_afio_swj_cfg_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_SWJ_CFG_2);

            /* 清除引脚重映像信息 */
            p_gpio_devinfo->p_remap[PIOB_3]  = AMHW_ZLG237_NO_REMAP;
            p_gpio_devinfo->p_remap[PIOB_4]  = AMHW_ZLG237_NO_REMAP;
            p_gpio_devinfo->p_remap[PIOA_15] = AMHW_ZLG237_NO_REMAP;

        } else if ((PIOB_4 == pin) &&
                 (p_gpio_devinfo->p_remap[PIOB_3] != AMHW_ZLG237_NO_REMAP)) {

            amhw_zlg237_afio_swj_cfg_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_SWJ_CFG_1);

            /* 清除引脚重映像信息 */
            p_gpio_devinfo->p_remap[PIOB_4]  = AMHW_ZLG237_NO_REMAP;
        }
    }

    /* 关闭外设重映像 */
    switch (peripheral) {

    case AMHW_ZLG237_SPI1_REMAP:
        amhw_zlg237_afio_spi1_pin_remap_disable(p_hw_afio);
        break;

    case AMHW_ZLG237_I2C1_REMAP:
        amhw_zlg237_afio_i2c1_pin_remap_disable(p_hw_afio);
        break;

    case AMHW_ZLG237_UART1_REMAP:
        amhw_zlg237_afio_uart_pin_remap_disable(p_hw_afio, 1);
        break;

    case AMHW_ZLG237_UART2_REMAP:
        amhw_zlg237_afio_uart_pin_remap_disable(p_hw_afio, 2);
        break;

    case AMHW_ZLG237_UART3_REMAP:
        amhw_zlg237_afio_uart3_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_UART3_REMAP_NO);
        break;

    case AMHW_ZLG237_TIM1_REMAP:
        amhw_zlg237_afio_tim1_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_TIM1_REMAP_NO);
        break;

    case AMHW_ZLG237_TIM2_REMAP:
        amhw_zlg237_afio_tim2_remap_mode_set(p_hw_afio, AMHW_ZLG237_AFIO_TIM2_REMAP_NO);
        break;

    case AMHW_ZLG237_TIM3_REMAP:
        amhw_zlg237_afio_tim3_remap_mode_set(p_hw_afio ,AMHW_ZLG237_AFIO_TIM3_REMAP_NO);
        break;

    case AMHW_ZLG237_TIM4_REMAP:
        amhw_zlg237_afio_tim4_pin_remap_disable(p_hw_afio);
        break;

    case AMHW_ZLG237_CAN_REMAP:
        amhw_zlg237_afio_can_remap_mode_set(p_hw_afio ,AMHW_ZLG237_AFIO_CAN_RX_PA11_TX_PA12);
        break;

    case AMHW_ZLG237_PD01_REMAP:
        amhw_zlg237_afio_pd01_pin_remap_disable(p_hw_afio);
        break;

    case AMHW_ZLG237_TIM5_CH4_REMAP:
        amhw_zlg237_afio_tim5_ch4_pin_remap_disable(p_hw_afio);
        break;

    case AMHW_ZLG237_ADC1_ETRGINJ_REMAP:
        amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGINJ);
        break;

    case AMHW_ZLG237_ADC1_ETRGREG_REMAP:
        amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGREG);
        break;

    case AMHW_ZLG237_ADC2_ETRGINJ_REMAP:
        amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGINJ);
        break;

    case AMHW_ZLG237_ADC2_ETRGREG_REMAP:
        amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGREG);
        break;

    case AMHW_ZLG237_SWJ_CFG:

        break;

    default:
        return -AM_ENOTSUP;
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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_zlg237_gpio_t     *p_hw_gpio  = NULL;
    amhw_zlg237_afio_t     *p_hw_afio  = NULL;
    amhw_zlg237_gpiomode_t  pin_mode   = AMHW_ZLG237_GPIO_MODE_AIN;
    uint32_t                func = 0, mode = 0;
    /* IO口方向 */
    uint8_t dir = 0;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (AM_FALSE == __gp_gpio_dev->valid_flg) {
        return -AM_ENODEV;
    }

    p_hw_gpio  = (amhw_zlg237_gpio_t *)p_gpio_devinfo->gpio_regbase;
    p_hw_afio  = (amhw_zlg237_afio_t *)p_gpio_devinfo->afio_regbase;

    func = AM_GPIO_COM_FUNC_GET(flags);
    mode = AM_GPIO_COM_MODE_GET(flags);

    /* 标准层相关 */
    if (0 != func) {

        /* 使用标准层前先退回引脚为GPIO模式 */
        __am_zlg237_peripheral_remap_clear(pin);

        switch (func) {

        case AM_GPIO_INPUT_VAL:
            /* 设置方向为输入 */
            amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);
            break;

        case AM_GPIO_OUTPUT_INIT_HIGH_VAL:
            /* 设置输出模式，如没有设置则默认为50Mhz */
            if (0 == (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                                AMHW_ZLG237_GPIO_SPEED_50MHz,
                                                pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            }
            /* 输出高电平 */
            amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
            /* 推挽输出 */
            amhw_zlg237_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_ZLG237_GPIO_MODE_OUT_PP,
                                          pin);
            break;

        case AM_GPIO_OUTPUT_INIT_LOW_VAL:
            /* 设置输出模式，如没有设置则默认为50Mhz */
            if (0 == (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                                AMHW_ZLG237_GPIO_SPEED_50MHz,
                                                pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            }
            /* 输出低电平 */
            amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
            /* 推挽输出 */
            amhw_zlg237_gpio_pin_mode_set(p_hw_gpio,
                                          AMHW_ZLG237_GPIO_MODE_OUT_PP,
                                          pin);
            break;

        default:
            return -AM_ENOTSUP;
        }
    }

    if (mode != 0x00) {

        /* 获取输入输出方向 */
        dir = amhw_zlg237_gpio_pin_dir_get(p_hw_gpio, pin);

        switch (mode) {

        case AM_GPIO_PULL_UP_VAL:
            if (dir != 1) {

                /* 使OD位置为1, 表明是上拉 */
                amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);

                pin_mode = AMHW_ZLG237_GPIO_MODE_IPU;
            } else {

                /* 输出没有上拉模式，设置为推挽模式 */
                pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_PP;
            }
            break;

        case AM_GPIO_PULL_DOWN_VAL:
            if (dir != 1) {

                /* 使OD位置为0, 表明是下拉 */
                amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
                pin_mode = AMHW_ZLG237_GPIO_MODE_IPD;
            } else {

                /* 输出没有下拉模式，设置为推挽模式 */
                pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_PP;
            }
            break;

        case AM_GPIO_FLOAT_VAL:
            if (dir != 1) {
                pin_mode = AMHW_ZLG237_GPIO_MODE_IN_FLOATING;
            } else {

                /* 输出没有浮空模式，设置为开漏模式 */
                pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_OD;
            }
            break;

        case AM_GPIO_OPEN_DRAIN_VAL:
            if (dir == 0) {
                /* 输入没有开漏模式，设置为浮空 */
                pin_mode = AMHW_ZLG237_GPIO_MODE_IN_FLOATING;
            } else {
                pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_OD;
            }
            break;

        case AM_GPIO_PUSH_PULL_VAL:
            if (dir == 0) {
                /* 输入没有推挽模式，设置为上拉 */
                pin_mode = AMHW_ZLG237_GPIO_MODE_IPU;
            } else {
                pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_PP;
            }
            break;

        default:
            return -AM_ENOTSUP;
        }

        amhw_zlg237_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /* 平台相关 */
    if (0 != (flags & AM_ZLG237_GPIO_MODE)) {

        /* 输入输出相应模式设置 */

        switch (AM_ZLG237_GPIO_MODE_GET(flags)) {

        case 0:
            /* 设置为输入 */
            amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);
            pin_mode = AMHW_ZLG237_GPIO_MODE_AIN;
            break;

        case 1:
            /* 设置为输入 */
            amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);
            pin_mode = AMHW_ZLG237_GPIO_MODE_IN_FLOATING;
            break;

        case 2:
            /* 使OD位置为0, 表明是下拉 */
            amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
            /* 设置为输入 */
            amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);
            pin_mode = AMHW_ZLG237_GPIO_MODE_IPD;
            break;

        case 3:
            /* 使OD位置为1, 表明是上拉 */
            amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
            /* 设置为输入 */
            amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);
            pin_mode = AMHW_ZLG237_GPIO_MODE_IPU;
            break;

        case 4:
            /* 设置为输出并设置响应频率，默认为50Mhz */
            if (0 != (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                                AMHW_ZLG237_GPIO_SPEED_50MHz,
                                                pin);
            }
            pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_PP;
            break;

        case 5:
            /* 设置为输出并设置响应频率，默认为50Mhz */
            if (0 != (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                                AMHW_ZLG237_GPIO_SPEED_50MHz,
                                                pin);
            }
            pin_mode = AMHW_ZLG237_GPIO_MODE_OUT_OD;
            break;

        case 6:
            /* 设置为输出并设置响应频率，默认为50Mhz */
            if (0 != (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                AMHW_ZLG237_GPIO_SPEED_50MHz, pin);
            }
            pin_mode = AMHW_ZLG237_GPIO_MODE_AF_PP;
            break;

        case 7:
            /* 设置为输出并设置响应频率，默认为50Mhz */
            if (0 != (flags & AM_ZLG237_GPIO_OUTRES_RATE)) {
                amhw_zlg237_gpio_pin_dir_output(
                    p_hw_gpio,
                    (amhw_zlg237_gpio_speed_mode_t)AM_ZLG237_GPIO_OUTRES_RATE_GET(flags),
                    pin);
            } else {
                amhw_zlg237_gpio_pin_dir_output(p_hw_gpio,
                                AMHW_ZLG237_GPIO_SPEED_50MHz, pin);
            }
            pin_mode = AMHW_ZLG237_GPIO_MODE_AF_OD;
            break;

        default:
            return -AM_ENOTSUP;
        }

        amhw_zlg237_gpio_pin_mode_set(p_hw_gpio, pin_mode, pin);
    }

    /* 重映像设置 */
    if (0 != (flags & AM_ZLG237_GPIO_REMAP)) {

        /* 新重映像与当前重映像不等， NO -> 外设重映像 ； 外设重映像 -> NO ； 外设1 -> 外设2 */
        if (AM_ZLG237_GPIO_REMAP_PERIPHERAL_GET(flags) !=
                                               p_gpio_devinfo->p_remap[pin]) {

            /* NO -> 外设重映像，当前引脚无重映像可不做任何处理 */
            if (p_gpio_devinfo->p_remap[pin] == AMHW_ZLG237_NO_REMAP) {

                /* 保存当前引脚重映像信息 */
                p_gpio_devinfo->p_remap[pin] =
                                     (amhw_zlg237_afio_remap_peripheral_t)
                                     AM_ZLG237_GPIO_REMAP_PERIPHERAL_GET(flags);

            } else if (AM_ZLG237_GPIO_REMAP_PERIPHERAL_GET(flags) == AMHW_ZLG237_NO_REMAP) {

                /*
                 * 外设重映像 -> NO
                 * 清除当前引脚对应外设的重映像模式，该外设将不能正常工作，需要重新配置
                 * 清楚外设重映像模式需要将所影响的所有GPIO重映像信息重写
                 */

                /* 清除外设的重映像，并对所影响的GPIO初始化 */
                __am_zlg237_peripheral_remap_clear(pin);

                /* 保存当前引脚重映像信息 */
                p_gpio_devinfo->p_remap[pin] = AMHW_ZLG237_NO_REMAP;

                return AM_OK;
            } else {

                /*
                 * 外设1 -> 外设2
                 * 清除当前引脚对应外设的重映像模式，设置新的重映像
                 * 清楚外设重映像模式需要将所影响的所有GPIO重映像信息重写
                 */

                /* 清除外设的重映像，并对所影响的GPIO初始化 */
                __am_zlg237_peripheral_remap_clear(pin);

                /* 保存当前引脚重映像信息 */
                p_gpio_devinfo->p_remap[pin] =
                                     (amhw_zlg237_afio_remap_peripheral_t)
                                     AM_ZLG237_GPIO_REMAP_PERIPHERAL_GET(flags);
            }
        } else {

            /* 新重映像与当前重映像相等，按外设重新写入相应对应重映像前需要清楚前前重映像影响的GPIO */
            if (p_gpio_devinfo->p_remap[pin] != AMHW_ZLG237_NO_REMAP) {

                __am_zlg237_peripheral_remap_clear(pin);
            }
        }

        /* 按外设设置重映像模式 */
        switch (AM_ZLG237_GPIO_REMAP_PERIPHERAL_GET(flags)) {

        case AMHW_ZLG237_SPI1_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_spi1_pin_remap_enable(p_hw_afio);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_spi1_pin_remap_disable(p_hw_afio);
            }
            break;

        case AMHW_ZLG237_I2C1_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_i2c1_pin_remap_enable(p_hw_afio);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_i2c1_pin_remap_disable(p_hw_afio);
            }
            break;

        case AMHW_ZLG237_UART1_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_uart_pin_remap_enable(p_hw_afio, 1);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_uart_pin_remap_disable(p_hw_afio, 1);
            }
            break;

        case AMHW_ZLG237_UART2_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_uart_pin_remap_enable(p_hw_afio, 2);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_uart_pin_remap_disable(p_hw_afio, 2);
            }
            break;

        case AMHW_ZLG237_UART3_REMAP:
            amhw_zlg237_afio_uart3_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_uart3_remap_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_TIM1_REMAP:
            amhw_zlg237_afio_tim1_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_tim1_remap_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_TIM2_REMAP:
            amhw_zlg237_afio_tim2_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_tim2_remap_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_TIM3_REMAP:
            amhw_zlg237_afio_tim3_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_tim3_remap_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_TIM4_REMAP:
            amhw_zlg237_afio_tim4_pin_remap_enable(p_hw_afio);
            break;

        case AMHW_ZLG237_CAN_REMAP:
            amhw_zlg237_afio_can_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_can_remap_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_PD01_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_pd01_pin_remap_enable(p_hw_afio);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_pd01_pin_remap_disable(p_hw_afio);
            }
            break;

        case AMHW_ZLG237_TIM5_CH4_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_tim5_ch4_pin_remap_enable(p_hw_afio);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_tim5_ch4_pin_remap_disable(p_hw_afio);
            }
            break;

        case AMHW_ZLG237_ADC1_ETRGINJ_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_enable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGINJ);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGINJ);
            }
            break;

        case AMHW_ZLG237_ADC1_ETRGREG_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_enable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGREG);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC1_ETRGREG);
            }
            break;

        case AMHW_ZLG237_ADC2_ETRGINJ_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_enable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGINJ);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGINJ);
            }
            break;

        case AMHW_ZLG237_ADC2_ETRGREG_REMAP:
            if (1 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_enable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGREG);
            } else if (0 == AM_ZLG237_GPIO_REMAP_MODE_GET(flags)) {

                amhw_zlg237_afio_adc_pin_remap_disable(p_hw_afio ,AMHW_ZLG237_AFIO_MAPR_ADC2_ETRGREG);
            }
            break;

        case AMHW_ZLG237_SWJ_CFG:
            amhw_zlg237_afio_swj_cfg_remap_mode_set(
                p_hw_afio,
                (amhw_zlg237_afio_swj_cfg_mode_t)AM_ZLG237_GPIO_REMAP_MODE_GET(flags));
            break;

        case AMHW_ZLG237_NO_REMAP:
            break;

        default:
            return -AM_ENOTSUP;
        }
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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg237_gpio_t     *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg237_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (amhw_zlg237_gpio_pin_dir_get(p_hw_gpio, pin)) {
        return amhw_zlg237_gpio_pin_output_get(p_hw_gpio, pin);
    } else {
        return amhw_zlg237_gpio_pin_input_get(p_hw_gpio, pin);
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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg237_gpio_t     *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg237_gpio_t *)p_gpio_devinfo->gpio_regbase;

    if (value == 0) {
        amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
    } else {
        amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg237_gpio_t     *p_hw_gpio  = NULL;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio  = (amhw_zlg237_gpio_t *)p_gpio_devinfo->gpio_regbase;

    amhw_zlg237_gpio_pin_out_toggle(p_hw_gpio, pin);

    return AM_OK;
}

/**
 * \brief 外部中断线0中断函数
 */
static void __port_exit0_int_isr (void * p_arg)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t  slot = 0;

    am_pfnvoid_t pfn_isr   = NULL;
    void        *p_arg_tmp = NULL;

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t   *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zlg237_exti_pending_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zlg237_exti_pending_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zlg237_exti_pending_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zlg237_exti_pending_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    /* 获取有关回调函数及参数 */
    pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
    p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

    if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot))) {
        if (pfn_isr != NULL) {
            pfn_isr(p_arg_tmp);
        }
    }

    /* 清中断标志 */
    amhw_zlg237_exti_pending_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t   *)p_gpio_devinfo->exti_regbase;

    for (slot = 5 ; slot < 10 ; slot++) {
        if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                     (amhw_zlg237_line_num_t)(1ul << slot))) {

            pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

            if (NULL != pfn_isr) {
                pfn_isr(p_arg_tmp);
            }

            amhw_zlg237_exti_pending_clear(p_hw_exti,
                                         (amhw_zlg237_line_num_t)(1ul << slot));

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

    amhw_zlg237_exti_t   *p_hw_exti   = NULL;

    p_hw_exti   = (amhw_zlg237_exti_t   *)p_gpio_devinfo->exti_regbase;

    for (slot = 10 ; slot < 16 ; slot++) {
        if (amhw_zlg237_exti_pr_read(p_hw_exti,
                                     (amhw_zlg237_line_num_t)(1ul << slot))) {

            pfn_isr   = p_gpio_devinfo->p_triginfo[slot].pfn_callback;
            p_arg_tmp = p_gpio_devinfo->p_triginfo[slot].p_arg;

            if (NULL != pfn_isr) {
                pfn_isr(p_arg_tmp);
            }

            amhw_zlg237_exti_pending_clear(p_hw_exti,
                                         (amhw_zlg237_line_num_t)(1ul << slot));

            break;
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
int am_zlg237_gpio_init (am_zlg237_gpio_dev_t           *p_dev,
                         const am_zlg237_gpio_devinfo_t *p_devinfo)
{
    uint8_t i = 0;

    if (NULL == p_dev || NULL == p_devinfo) {
        return -AM_EINVAL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_dev->p_devinfo = p_devinfo;

    if ((p_devinfo->p_remap == NULL) || (p_devinfo->inum_pin == NULL) ||
        (p_devinfo->p_infomap == NULL) || (p_devinfo->p_triginfo == NULL)) {
        p_dev->valid_flg = AM_FALSE;
        return -AM_EINVAL;
    }

    for (i = 0 ; i < p_devinfo->pin_count ; i++) {
        if ((i == PIOA_13) || (i == PIOA_14) || (i == PIOA_15) ||
             (i == PIOB_3) || (i == PIOB_4)) {
            p_devinfo->p_remap[i] = AMHW_ZLG237_SWJ_CFG;
        } else {
            p_devinfo->p_remap[i] = AMHW_ZLG237_NO_REMAP;
        }
    }

    for (i = 0 ; i < p_devinfo->exti_num_max ; i++) {
        p_devinfo->p_infomap[i] = AM_ZLG237_GPIO_INVALID_PIN_MAP;
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
void am_zlg237_gpio_deinit (void)
{
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    uint8_t i;

    if (p_gpio_devinfo->pfn_plfm_deinit) {
        p_gpio_devinfo->pfn_plfm_deinit();
    }

    if (__gp_gpio_dev->valid_flg == AM_FALSE) {
        return;
    }

    for (i = 0 ; i < p_gpio_devinfo->pin_count ; i++) {
        if ((i == PIOA_13) || (i == PIOA_14) || (i == PIOA_15) ||
             (i == PIOB_3) || (i == PIOB_4)) {
            p_gpio_devinfo->p_remap[i] = AMHW_ZLG237_SWJ_CFG;
        } else {
            p_gpio_devinfo->p_remap[i] = AMHW_ZLG237_NO_REMAP;
        }
    }

    for (i = 0 ; i < p_gpio_devinfo->exti_num_max ; i++) {
        p_gpio_devinfo->p_infomap[i] = AM_ZLG237_GPIO_INVALID_PIN_MAP;
        p_gpio_devinfo->p_triginfo[i].p_arg = NULL;
        p_gpio_devinfo->p_triginfo[i].pfn_callback = NULL;
    }

    am_int_disable(p_gpio_devinfo->inum_pin[0]);
    am_int_disable(p_gpio_devinfo->inum_pin[1]);
    am_int_disable(p_gpio_devinfo->inum_pin[2]);
    am_int_disable(p_gpio_devinfo->inum_pin[3]);
    am_int_disable(p_gpio_devinfo->inum_pin[4]);
    am_int_disable(p_gpio_devinfo->inum_pin[5]);
    am_int_disable(p_gpio_devinfo->inum_pin[6]);

    am_int_disconnect(p_gpio_devinfo->inum_pin[0], __port_exit0_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[1], __port_exit1_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[2], __port_exit2_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[3], __port_exit3_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[4], __port_exit4_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[5], __port_exit9_5_int_isr, NULL);
    am_int_disconnect(p_gpio_devinfo->inum_pin[6], __port_exit15_10_int_isr, NULL);

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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);

    amhw_zlg237_gpio_t     *p_hw_gpio  = NULL;
    amhw_zlg237_exti_t     *p_hw_exti  = NULL;
    amhw_zlg237_afio_t     *p_hw_afio  = NULL;
    uint8_t                 slot       = pin & 0x0f;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_gpio = (amhw_zlg237_gpio_t *)p_gpio_devinfo->gpio_regbase;
    p_hw_exti = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;
    p_hw_afio = (amhw_zlg237_afio_t *)p_gpio_devinfo->afio_regbase;

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    if (p_gpio_devinfo->p_infomap[slot] != pin) {
        return -AM_EINVAL;
    }

    /* 使用GPIO功能 */
    __am_zlg237_peripheral_remap_clear(pin);

    /* 设置管脚为输入方向 */
    amhw_zlg237_gpio_pin_dir_input(p_hw_gpio, pin);

    /* 管脚为上拉/下拉模式 */
    amhw_zlg237_gpio_pin_mode_set(p_hw_gpio, AMHW_ZLG237_GPIO_MODE_IPD, pin);
    /*
     * 中断线0只能连一个端口编号为0的GPIO引脚，比如说，PIOA_0连接到了中断线0，
     * PIOB_0、PIOC_0、PIOD_0就不能连到中断线0, 但此时PIOA_1可以连接到中断线1。
     */
    amhw_zlg237_afio_exti_pin_set(p_hw_afio, pin);

    /* 清除中断线配置 */
    amhw_zlg237_exti_imr_clear(p_hw_exti,
                               (amhw_zlg237_line_num_t)(1ul << slot));
    amhw_zlg237_exti_emr_clear(p_hw_exti,
                               (amhw_zlg237_line_num_t)(1ul << slot));

    /* 清除中断线触发方式配置 */
    amhw_zlg237_exti_rtsr_clear(p_hw_exti,
                                (amhw_zlg237_line_num_t)(1ul << slot));
    amhw_zlg237_exti_ftsr_clear(p_hw_exti,
                                (amhw_zlg237_line_num_t)(1ul << slot));

    switch (flag) {

    case AM_GPIO_TRIGGER_OFF:
        break;

    case AM_GPIO_TRIGGER_RISE:
        /* 使OD位置为0, 表明是下拉 */
        amhw_zlg237_gpio_pin_out_low(p_hw_gpio, pin);
        amhw_zlg237_exti_rtsr_set(p_hw_exti,
                                  (amhw_zlg237_line_num_t)(1ul << slot));
        break;

    case AM_GPIO_TRIGGER_FALL:
        /* 使OD位置为1, 表明是上拉 */
        amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
        /* falling  edge */
        amhw_zlg237_exti_ftsr_set(p_hw_exti,
                                  (amhw_zlg237_line_num_t)(1ul << slot));
        break;

    case AM_GPIO_TRIGGER_BOTH_EDGES:
        /* 使OD位置为1, 表明是上拉 */
        amhw_zlg237_gpio_pin_out_high(p_hw_gpio, pin);
        /* falling  edge */
        amhw_zlg237_exti_ftsr_set(p_hw_exti,
                                  (amhw_zlg237_line_num_t)(1ul << slot));
        amhw_zlg237_exti_rtsr_set(p_hw_exti,
                                  (amhw_zlg237_line_num_t)(1ul << slot));
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

    if (p_gpio_devinfo->p_infomap[slot] == AM_ZLG237_GPIO_INVALID_PIN_MAP) {

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

        p_gpio_devinfo->p_infomap[slot] = AM_ZLG237_GPIO_INVALID_PIN_MAP;
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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_zlg237_exti_t *p_hw_exti  = NULL;
    uint8_t             slot       = pin & 0x0f;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    if (slot > (p_gpio_devinfo->exti_num_max - 1)) {
        return -AM_ENOSPC;
    }

    p_hw_exti = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        amhw_zlg237_exti_pending_clear(p_hw_exti,
                                       (amhw_zlg237_line_num_t)(1ul << slot));
        amhw_zlg237_exti_imr_set(p_hw_exti,
                                 (amhw_zlg237_line_num_t)(1ul << slot));

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
    __GPIO_DEVINFO_DECL(p_gpio_devinfo, __gp_gpio_dev);
    amhw_zlg237_exti_t *p_hw_exti  = NULL;
    uint8_t             slot       = pin & 0x0f;

    if (__gp_gpio_dev == NULL) {
        return -AM_ENXIO;
    }

    p_hw_exti = (amhw_zlg237_exti_t *)p_gpio_devinfo->exti_regbase;

    if (p_gpio_devinfo->p_infomap[slot] == pin) {

        amhw_zlg237_exti_imr_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));
        amhw_zlg237_exti_emr_clear(p_hw_exti,
                                   (amhw_zlg237_line_num_t)(1ul << slot));

    } else {
        return -AM_ENXIO;
    }

    return AM_OK;
}

/* end of file */
