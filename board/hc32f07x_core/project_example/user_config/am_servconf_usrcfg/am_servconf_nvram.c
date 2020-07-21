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
 * \brief NVRAM 标准服务存储段定义
 *
 * 当使用 NVRAM 标准接口进行非易失数据的存储或获取时，如 \sa am_nvram_set() 或
 * \sa am_nvram_get()，接口的详细介绍详见 \sa am_nvram.h
 *
 * 存储段的名字 name 和单元号 unit 均在该文件中定义，便于用户管理系统中所有的非易失
 * 存储段
 *
 * \sa am_nvram_cfg.c
 * \internal
 * \par modification history
 * - 1.00 15-01-19  tee, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_nvram.h"

/**
 * \addtogroup am_if_src_servconf_nvram_cfg
 * \copydoc am_nvram_cfg.c
 * @{
 */

/**
 * \brief NVRAM 存储段列表
 *
 * 系统所有的存储段均定义该数组中，存储段列表结尾应该存在一个空的存储段
 * {NULL, 0, 0, 0, NULL, 0}
 *
 * 一个存储段包含四个信息：
 * 段名，单元号，在存储器中的起始地址，段容量（字节数），实际存储设备的名字
 *
 * 实际存储设备的名字与实际硬件相关，如使用 FM24C02 提供 NVRAM 存储服务，则
 * 名字可能为： "fm24c02" ，实际名字是在相应器件的配置文件中定义的
 * 如定义如下几个存储段：
 *     am_local am_const am_nvram_segment_t __g_nvram_cfg_segs[] = {
 *         {"ip",         0,  0,  4,   "fm24c02"},
 *         {"ip",         1,  4,  4,   "fm24c02"},
 *         {"temp_limit", 0,  8,  4,   "fm24c02"},
 *         {"system",     0,  12, 50,  "fm24c02"},
 *         {"test",       0,  62, 178, "fm24c02"},
 *         {NULL,         0,   0,   0,      NULL}     // 空存储段，必须保留
 */
am_local am_const am_nvram_segment_t __g_nvram_cfg_segs[] = {
    {"microport_eeprom", 0, 0, 256, "microport_eeprom"}, /* MicroPort-EEPROM */
    {NULL,               0, 0, 0,   NULL}                /* 空存储段，必须保留 */
};

/**
 * \brief NVRAM 标准服务初始化
 */
void am_nvram_inst_init (void)
{

    /* 初始化时，指定应用程序的存储段 */
    am_nvram_init(&__g_nvram_cfg_segs[0]);
}

/**
 * @}
 */

/* end of file */
