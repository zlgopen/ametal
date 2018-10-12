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
 * \brief book`s digitron0 driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-12  adw, first implementation
 * \endinternal
 */

#ifndef __DIGITRON0_H
#define __DIGITRON0_H

#include <am_types.h>

void digitron_init (void);     // 板级初始化函数，熄灭所有数码管

// 段码传送函数，将段码直接被传送到相应的引脚
// code为传送的段码，bit0~bit7与a ~ dp段对应，为0点亮相应段，为1熄灭相应段
void digitron_segcode_set (uint8_t code);

// 位码传送函数，直接设定指定的数码管的公共端有效，其余公共端无效
// pos的值为0，com0有效；pos的值为1，com1有效
void digitron_com_sel (uint8_t pos);

// 数字显示扫描函数
// pos的值为0，在com0上显示数字；pos的值为1，在com1上显示数字
// num为待显示的数字，有效范围为0 ~ 9
void digitron_disp_num (uint8_t pos, uint8_t num);




#endif /* __DIGITRON0_H */

/* end of file */
