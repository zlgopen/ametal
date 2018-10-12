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
 * \brief book`s digitron1 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __DIGITRON1_H
#define __DIGITRON1_H

#include <am_types.h>

void digitron_init1 (void);        // 板级初始化函数

// 带软件定时器的板级初始化函数，该初始化函数可替代digitron_init()初始化
// 即可在模块内部使用软件定时器实现自动扫描显示。在扫描显示时，直接从缓冲区中获取需要显
// 示的内容，可以使用digitron_disp_code_set()和digitron_disp_num_set()函数设置缓冲区的内容，
// 进而相当于设置了数码管显示的内容
void digitron_init_with_softimer (void);

// 动态显示扫描函数
// 如果使用digitron_init()进行数码管初始化，则必须在应用程序中以5ms的时间间隔调用该函数
// 如果使用digitron_init_with_softimer()进行数码管初始化，则自动扫描，无需调用该函数进行扫描
void digitron_disp_scan (void);


// 当74HC595驱动数码管时，即用：
// digitron_hc595_init()替代digitron_init()
// digitron_hc595_disp_scan()替代digitron_disp_scan()
void digitron_hc595_init(void);						// 板级初始化
void digitron_hc595_disp_scan(void);			//动态扫描显示，每5ms扫描一次
void 	digitron_hc595_init_with_softimer(void);			// 带软件定时器的板级初始化函数

// 设置数码管显示的段码值
// pos的值为0，则设置com0显示的段码；pos的值为1，则设置com1显示的段码
// code为待传送的段码，bit0~bit7与a ~ dp段对应，为0则点亮相应段，为1则熄灭相应段
void digitron_disp_code_set (uint8_t pos, uint8_t code);

// 设置数码管显示的数字
// pos的值为0，则设置com0显示的数字；pos的值为1，则设置com1显示的数字
// num为待显示的数字，有效范围为0 ~ 9
void digitron_disp_num_set (uint8_t pos, uint8_t num);

//获取num参数指定数字的段码，返回对应的段码值，num有效范围为0 ~ 9
uint8_t digitron_num_decode (uint8_t num);

// 设置数码管的闪烁属性
// pos的值为0，则设置com0的闪烁属性；pos的值为1，则设置com1的闪烁属性
// isblink为AM_TRUE，闪烁；isblink为AM_FALSE，正常显示，不闪烁
void digitron_disp_blink_set (uint8_t pos, am_bool_t isblink);


#endif /* __DIGITRON1_H */

/* end of file */
