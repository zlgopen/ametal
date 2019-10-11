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
 * \brief ZSN603(I2C模式)配置文件
 *
 * \internal
 * \par Modification history
 * - 1.00 19-04-22  htf, first implementation.
 * \endinternal
 */
#include <am_hwconf_zsn603_i2c.h>
#include "ametal.h"
#include "am_i2c.h"
#include "zsn603.h"
#include "zlg116_pin.h"
#include "am_zsn603.h"
#include "zsn603_platform.h"
#include "am_zlg116_inst_init.h"

/**< \brief ZSN603(I2C模式) 设备实例 */
static   zsn603_i2c_dev_t          __g_zsn603_i2c_dev;
/**< \brief ZSN603(I2C模式) 设备信息 */
static const zsn603_i2c_devinfo_t  __g_i2c_info = {
    0xb2,
    {
        -1,
        PIOA_12,
        am_zlg116_i2c1_inst_init,
        am_zlg116_i2c1_inst_deinit
    }
};
/** \brief ZSN603实例初始化，获得ZSN603标准服务句柄(i2c模式) */
zsn603_handle_t  am_zsn603_i2c_inst_init (void)
{
    return zsn603_i2c_init(&__g_zsn603_i2c_dev, &__g_i2c_info);
}

