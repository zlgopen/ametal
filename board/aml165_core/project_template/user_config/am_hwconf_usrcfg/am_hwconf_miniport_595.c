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
 * \brief Miniport-595 配置文件
 * \sa am_hwconf_miniport_595.c
 *
 * \internal
 * \par Modification history
 * - 1.00 17-07-13  tee, first implementation.
 * \endinternal
 */
#include <am_aml165_inst_init.h>
#include "ametal.h"
#include "am_hc595_gpio.h"
#include "am_hc595_spi.h"
#include "zml165_pin.h"

/**
 * \addtogroup am_if_src_hwconf_miniport_595
 * \copydoc am_hwconf_miniport_595.c
 * @{
 */

/*******************************************************************************
   Public functions
*******************************************************************************/
static  am_hc595_spi_info_t __hc595_spi_info = {
    PIOA_4,                                        /* 数据锁存引脚 */
    -1,                                            /* 未使用oe引脚 */
    300000,                                        /* 时钟300KHZ */
    AM_TRUE                                        /* 数据低位先发送 */
};

/* MiniPort-595 实例初始化 */
am_hc595_handle_t am_miniport_595_inst_init (void)
{
#if 0
    static am_hc595_gpio_dev_t         miniport_595;
    static const am_hc595_gpio_info_t  miniport_595_info = {
        PIOA_7,
        PIOA_5,
        PIOA_4,
        -1,                     /* OE固定为低电平，未使用 */
        AM_TRUE                 /* 低位先发送 */
    };

    return  am_hc595_gpio_init(&miniport_595, &miniport_595_info);

#else

    static am_hc595_spi_dev_t    miniport_595;

    return  am_hc595_spi_init(&miniport_595,
                              &__hc595_spi_info,
                               am_zml165_spi1_int_inst_init());
#endif

}

/**
 * @}
 */

/* end of file */
