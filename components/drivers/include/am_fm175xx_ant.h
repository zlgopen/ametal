#ifndef __AM_CD4051_H
#define __AM_CD4051_H

#include "ametal.h"

#define  AM_FM175XX_ANTENNA_ONE       1  /**< \brief 1可控天线设备 FM17510 */
#define  AM_FM175XX_ANTENNA_TWO       2  /**< \brief 2可控天线设备 FM17550 */
#define  AM_FM175XX_ANTENNA_EIGHT     8  /**< \brief 8可控天线设备 FM175xx + CD4051*/

typedef struct am_cd4051_device_info{
    int pin_en;
    int pin_s0;
    int pin_s1;
    int pin_s2;
}am_cd4051_device_info_t;

typedef struct am_antenna_info{
    am_cd4051_device_info_t  cd4051_dev_inf;
    int                      antenna_num;
}am_antenna_info_t;

/**
 * \brief cd4051引脚初始化
 */
void am_cd4051_pin_init (am_antenna_info_t *p_dev);

/**
 * \brief cd4051通道选择
 *
 * \param[in] channel_num : 选择的通道号(0~7)
 */
void am_cd4051_channel_selected (am_antenna_info_t *p_dev, uint8_t channel_num);


#endif
