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
 * \brief book`s digitron2 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __DIGITRON2_H
#define __DIGITRON2_H

#include <am_types.h>

void digitron2_init(void);                                // 板级初始化函数
// 带软件定时器的板级初始化函数，该初始化函数可替代digitron_init()初始化
// 即可在模块内部使用软件定时器实现自动扫描显示。在扫描显示时，直接从缓冲区中获取需要显
// 示的内容，可以使用digitron_disp_code_set()和digitron_disp_num_set()函数设置缓冲区的内容
// 进而相当于设置了数码管显示的内容
void digitron2_init_with_softimer(void);                  // 板级初始化函数（含软件定时器）

// 动态显示扫描函数
// 如果使用digitron_init()进行数码管初始化，则必须在应用程序中以5ms的时间间隔调用该函数
// 如果使用digitron_init_with_softimer()进行数码管初始化，则自动扫描，无需调用该函数进行扫描
void digitron2_disp_scan(void);                           // 动态扫描显示函数

// 设置数码管显示的段码值
// pos的值为0，则设置com0显示的段码；pos的值为1，则设置com1显示的段码
// code：需要传送的段码，bit0~bit7与a ~ dp段对应，为0则点亮相应段，为1则熄灭相应段
void digitron2_disp_code_set(uint8_t pos, uint8_t code);  // 传送段码到显示缓冲区函数

// 设置数码管显示的字符
// pos的值为0，则设置com0显示的字符；pos的值为1，则设置com1显示的字符
//ch：需要显示的字符，有效字符有：123456789-ABCDEFabcdefORPNorpn。
void digitron2_disp_char_set(uint8_t pos, char ch);       // 传送字符到显示缓冲区函数

// 获取ch参数指定字符的段码，返回对应的段码值。
// 支持的字符有：123456789-ABCDEFabcdefORPNorpn。
uint8_t digitron2_char_decode(char ch);                   // 获取待显示字符的段码函数

// 设置数码管的闪烁属性
// pos的值为0，则设置com0的闪烁属性；pos的值为1，则设置com1的闪烁属性
// isblink为AM_TRUE，闪烁；isblink为AM_FALSE，正常显示，不闪烁
void digitron2_disp_blink_set(uint8_t pos, am_bool_t isblink);    // 设定闪烁位函数


#endif /* __DIGITRON2_H */

/* end of file */
