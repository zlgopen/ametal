/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief ZM516X 模块 GPIO 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. ZM516X 模块初始化并配置成功后 LED0 长亮，如果初始化失败，LED0 闪烁；
 *   2. 串口打印读取到的模块配置信息；
 *   3. 读取 IO1(JOIN) 引脚的电平并将相同的电平输出到 IO2(DETECH) 引脚。
 *
 * \note
 *    1. LED0 需要短接 J9 跳线帽，才能被 PIO0_8 控制；
 *    2. 如需观察串口打印的调试信息，需要将 PIO0_0 引脚连接 PC 串口的 TXD，
 *       PIO0_4 引脚连接 PC 串口的 RXD；
 *    3. ZigBee 模块内连接关系如下：
 * <pre>
 *           PIO0_26  <-->  ZigBee_TX
 *           PIO0_27  <-->  ZigBee_RX
 *           PIO0_28  <-->  ZigBee_RST
 * </pre>
 *        如果需要使用 ZigBee，这些 IO 口不能用作其它用途。
 *
 * \par 源代码
 * \snippet demo_zm516x_gpio.c src_zm516x_gpio
 *
 * \internal
 * \par Modification History
 * - 1.00 18-01-15  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_zm516x_gpio
 * \copydoc demo_zm516x_gpio.c
 */

/** [src_zm516x_gpio] */
#include "ametal.h"
#include "am_zm516x.h"
#include "am_led.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_board.h"
#include <string.h>

#define    __TEST_CFG_COMMAND    (1)    /**< \brief 是否测试永久配置命令 */
#define    __TEST_CMD_COMMAND    (1)    /**< \brief 是否测试临时配置命令 */

/**
 * \brief 闪烁 LED0
 */
am_local void flash_led (void)
{
    while (1) {
        am_led_on(LED0);
        am_mdelay(50);

        am_led_off(LED0);
        am_mdelay(500);
    }
}

/**
 * \brief 打印 ZM516X 模块的配置信息
 */
am_local void __cfg_info_print (am_zm516x_cfg_info_t *p_zm516x_cfg_info)
{
    AM_DBG_INFO("\r\n__cfg_info_print\r\n");
    AM_DBG_INFO("dev_name:%s\r\n"
                "dev_pwd:%s\r\n"
                "dev_mode:%d\r\n"
                "chan:%d\r\n"
                "panid:0x%02x%02x\r\n"
                "my_addr:0x%02x%02x\r\n"
                "dst_addr:0x%02x%02x\r\n"
                "power_level:%d\r\n"
                "retry_num:%d\r\n"
                "tran_timeout:%d\r\n"
                "send_mode:%d\r\n",
                p_zm516x_cfg_info->dev_name,
                p_zm516x_cfg_info->dev_pwd,
                p_zm516x_cfg_info->dev_mode,
                p_zm516x_cfg_info->chan,
                p_zm516x_cfg_info->panid[0],
                p_zm516x_cfg_info->panid[1],
                p_zm516x_cfg_info->my_addr[0],
                p_zm516x_cfg_info->my_addr[1],
                p_zm516x_cfg_info->dst_addr[0],
                p_zm516x_cfg_info->dst_addr[1],
                p_zm516x_cfg_info->power_level,
                p_zm516x_cfg_info->retry_num,
                p_zm516x_cfg_info->tran_timeout,
                p_zm516x_cfg_info->send_mode);
    AM_DBG_INFO("\r\n");
}

#if (__TEST_CFG_COMMAND == 1)

/**
 * \brief 打印 PWM 信息
 */
am_local void __pwm_info_print (am_zm516x_pwm_t *p_pwm_info, uint8_t num)
{
    uint8_t  i    = 0;
    uint32_t temp = 0;

    AM_DBG_INFO("\r\n__pwm_info_print\r\n");
    for (i = 0; i < num; i++) {
        memcpy(&temp, &((p_pwm_info + i)->freq), sizeof(uint32_t));

        AM_DBG_INFO("freq:%d\r\n"
                    "duty_cycle:%d\r\n",
                    temp,
                    (p_pwm_info + i)->duty_cycle);
    }
    AM_DBG_INFO("\r\n");
}
#endif /* (__TEST_CFG_COMMAND == 1) */

/**
 * \brief 例程入口
 */
void demo_zm516x_gpio_entry (am_zm516x_handle_t zm516x_handle)
{
    am_zm516x_cfg_info_t zm516x_cfg_info;
    am_zm516x_addr_t     zb_addr;

#if (__TEST_CFG_COMMAND == 1)
    am_zm516x_pwm_t      pwm[4];
    uint8_t              i              = 0;
    uint8_t              dir            = 0;
    uint16_t             period         = 0;
    am_bool_t            is_dormant     = 0;
    uint32_t             temp           = 0;
#endif /* (__TEST_CFG_COMMAND == 1) */

#if (__TEST_CMD_COMMAND == 1)
    uint8_t              gpio_dir       = 0;
    uint8_t              gpio_value     = 0;
    uint16_t             adc_value      = 0;
#endif /* (__TEST_CMD_COMMAND == 1) */

    /* 恢复 ZM516X 模块出厂设置（永久命令：DA），设置成功需复位 */
    if (am_zm516x_default_set(zm516x_handle) != AM_OK) {
        AM_DBG_INFO("am_zm516x_default_set failed\r\n");
        flash_led();
    }

    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

    /* 获取 ZigBee 模块的配置信息（永久命令：D1） */
    if (am_zm516x_cfg_info_get(zm516x_handle, &zm516x_cfg_info) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_info_get failed\r\n");
        flash_led();
    }
    __cfg_info_print(&zm516x_cfg_info);

    /* 设置目标地址 */
    zb_addr.p_addr    = zm516x_cfg_info.my_addr;
    zb_addr.addr_size = 2;

#if (__TEST_CFG_COMMAND == 1)
    gpio_dir = 0x05;

    /* 设置指定地址 ZigBee 模块的 GPIO 输入输出方向（永久命令：E1） */
    if (am_zm516x_cfg_gpio_dir_set(zm516x_handle,
                                  &zb_addr,
                                   gpio_dir) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_gpio_dir_set failed\r\n");
        flash_led();
    }

    /* 获取指定地址 ZigBee 模块的 GPIO 输入输出方向（永久命令：E1） */
    if (am_zm516x_cfg_gpio_dir_get(zm516x_handle,
                                  &zb_addr,
                                  &gpio_dir) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_gpio_dir_get failed\r\n");
        flash_led();
    }

    dir = 0x0a;
    period = 100;
    is_dormant = AM_FALSE;

    /* IO/AD 采集设置（永久命令：E2），设置成功需复位 */
    if (am_zm516x_cfg_io_adc_upload_set(zm516x_handle,
                                       &zb_addr,
                                        dir,
                                        period,
                                        is_dormant) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_io_adc_upload_set failed\r\n");
        flash_led();
    }

    /* 设置成功，复位 ZM516X 模块（永久命令：D9） */
    am_zm516x_reset(zm516x_handle);
    am_mdelay(10);

    /* IO/AD 采集配置获取（永久命令：E2） */
    if (am_zm516x_cfg_io_adc_upload_get(zm516x_handle,
                                       &zb_addr,
                                       &dir,
                                       &period,
                                       &is_dormant) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_io_adc_upload_set failed\r\n");
        flash_led();
    }
    AM_DBG_INFO("io_adc_upload: dir = 0x%02x, period = %d, is_dormant = %s\r\n",
                dir,
                period,
                (is_dormant != AM_FALSE) ? "AM_TRUE" : "AM_FALSE");

    gpio_value = 0x05;

    /* 设置指定地址 ZigBee 模块的 GPIO 输出值（永久命令：E3） */
    if (am_zm516x_cfg_gpio_set(zm516x_handle,
                              &zb_addr,
                               gpio_value) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_gpio_set failed\r\n");
        flash_led();
    }

    /* 获取指定地址 ZigBee 模块的 GPIO 输入值（永久命令：E3） */
    if (am_zm516x_cfg_gpio_get(zm516x_handle,
                              &zb_addr,
                              &gpio_value) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_gpio_get failed\r\n");
        flash_led();
    }
    AM_DBG_INFO("gpio_value: 0x%02x\r\n", gpio_value);

    temp = 1000;
    for (i = 0; i < 4; i++) {
        memcpy(&pwm[i].freq, &temp, 4);
        pwm[i].duty_cycle = 50;
    }

    /* 设置指定地址 ZigBee 模块的 PWM 输出值（永久命令：E4） */
    if (am_zm516x_cfg_pwm_set(zm516x_handle,
                             &zb_addr,
                             &pwm[0]) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_pwm_set failed\r\n");
        flash_led();
    }

    /* 获取指定地址 ZigBee 模块的 PWM 输出值配置（永久命令：E4） */
    if (am_zm516x_cfg_pwm_get(zm516x_handle,
                             &zb_addr,
                             &pwm[0]) != AM_OK) {
        AM_DBG_INFO("am_zm516x_cfg_pwm_get failed\r\n");
        flash_led();
    }
    __pwm_info_print(pwm, 4);
#endif /* (__TEST_CFG_COMMAND == 1) */

    AM_DBG_INFO("\r\n");

#if (__TEST_CMD_COMMAND == 1)
    gpio_dir = 0x08;

    /* 设置指定地址 ZM516X 模块的 GPIO 输入输出方向（临时命令：D4） */
    if (am_zm516x_gpio_dir(zm516x_handle,
                          &zb_addr,
                           gpio_dir) != AM_OK) {
        AM_DBG_INFO("am_zm516x_gpio_dir failed\r\n");
        flash_led();
    }

    /* 读取指定地址 ZM516X 模块的 AD 转换值（临时命令：D7） */
    if (am_zm516x_ad_get(zm516x_handle,
                        &zb_addr,
                         0,
                        &adc_value) != AM_OK) {
        AM_DBG_INFO("am_zm516x_ad_get failed\r\n");
        flash_led();
    }
    AM_DBG_INFO("adc_value: %d voltage: %dmV\r\n",
                adc_value,
                adc_value * 2470 / 1024);
#endif /* (__TEST_CMD_COMMAND == 1) */

    /* 配置完成，点亮 LED0 */
    am_led_on(LED0);

    AM_FOREVER {

#if (__TEST_CMD_COMMAND == 1)

        /* 读取指定地址 ZM516X 模块的 GPIO 输入值（临时命令：D5） */
        if (am_zm516x_gpio_get(zm516x_handle,
                              &zb_addr,
                              &gpio_value) != AM_OK) {
            AM_DBG_INFO("am_zm516x_gpio_get failed\r\n");
            flash_led();
        }
        AM_DBG_INFO("gpio_value: 0x%02x\r\n", gpio_value);

        /* 设置指定地址 ZM516X 模块的 GPIO 输出值（临时命令：D6） */
        if (am_zm516x_gpio_set(zm516x_handle,
                              &zb_addr,
                               (gpio_value & 0x04) << 1) != AM_OK) {
            AM_DBG_INFO("am_zm516x_gpio_set failed\r\n");
            flash_led();
        }
#endif /* (__TEST_CMD_COMMAND == 1) */

        am_mdelay(100);
    }
}
/** [src_zm516x_gpio] */

/* end of file */
