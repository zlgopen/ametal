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
 * \brief
 * - 实验现象：
 *   1. 连接好串口，串口将打印提示消息。
 *   2. 按照提示消息进行操作，即可完成电压校准，完成后会输出校准函数
 *   3. 校准系数会自动保存至flash中。
 *   4. 将会调用固定电压测试例程，对固定电压进行测试后通过函数修正后进行打印输出。
 *
 * \par 源代码
 * \snippet demo_aml166_core_adc24_vol_para_adjuet.c src_aml166_core_adc24_vol_para_adjuet
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-17  sdq, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_aml166_core_adc24_vol_para_adjuet
 * \copydoc demo_aml166_core_adc24_vol_para_adjuet.c
 */

/** [src_aml166_core_adc24_vol_para_adjuet] */

#include "am_aml166_inst_init.h"
#include "am_hwconf_zml166_adc.h"
#include "am_zml166_adc.h"
#include "string.h"
#include "demo_components_entries.h"
#include "zlg116_periph_map.h"
#include "am_zlg_flash.h"
#include "demo_aml166_core_entries.h"
#include "demo_zlg_entries.h"

/**
 * \brief ZML166 24位ADC 校准参数获取例程
 */
void demo_aml166_core_zml166_adc_vol_para_adjuet (void)
{
    float mem_data[2] = {0};
    float para[16]    = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    am_zml166_adc_handle_t  handle       = am_zml166_adc_inst_init();
    am_uart_handle_t        uart_handle  = am_zlg116_uart1_inst_init();

    /* 进行电压系数修调  */
    demo_zml166_adc_vol_para_adjuet_entry(handle, uart_handle, para);

    /* 对flash进行初始化 */
    am_zlg_flash_init(ZLG116_FLASH);

    /* 重写电压校准系数前，先保存PT100校准系数 */
    memcpy((void *)mem_data, (uint32_t *)PT100_PARA_SAVE_ADDRESS, 4 * 2);

    am_zlg_flash_sector_erase(ZLG116_FLASH, (1024 * FLASH_BLOCK_NUM));

    am_zlg_flash_sector_program(ZLG116_FLASH,
                               PT100_PARA_SAVE_ADDRESS,
                   (uint32_t *)mem_data,
                               2);

    am_zlg_flash_sector_program(ZLG116_FLASH,
                              (1024 * FLASH_BLOCK_NUM),
                   (uint32_t *)para,
                               16);

    demo_zml166_adc_vol_measure_entry((void *)handle,
                                              para,
                                              AM_ZML166_ADC_PGA_1);
}


/** [src_aml166_core_adc24_vol_para_adjuet] */

/* end of file */
