#include "ametal.h"
#include "am_gpio.h"
#include "am_fm175xx_ant.h"

/**
 * @}
 * .
 */

/**
 * \brief cd4051引脚初始化
 */
void am_cd4051_pin_init (am_antenna_info_t *p_dev)
{
    /* 设置引脚为GPIO功能,输出模式,内部上拉 */
    am_gpio_pin_cfg(p_dev->cd4051_dev_inf.pin_en, AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(p_dev->cd4051_dev_inf.pin_s2, AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(p_dev->cd4051_dev_inf.pin_s1, AM_GPIO_OUTPUT_INIT_HIGH);
    am_gpio_pin_cfg(p_dev->cd4051_dev_inf.pin_s0, AM_GPIO_OUTPUT_INIT_HIGH);

    /* 默认CD4051使能,选择通道0 */
    am_gpio_set(p_dev->cd4051_dev_inf.pin_en, 0);
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s2, 0);
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s1, 0);
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s0, 0);
}

/**
 * \brief cd4051通道选择
 *
 * \param[in] channel_num : 选择的通道号(0~7)
 */
void am_cd4051_channel_selected (am_antenna_info_t *p_dev, uint8_t channel_num)
{
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s2, (channel_num >> 2) & 1);
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s1, (channel_num >> 1) & 1);
    am_gpio_set(p_dev->cd4051_dev_inf.pin_s0, (channel_num >> 0) & 1);
}
