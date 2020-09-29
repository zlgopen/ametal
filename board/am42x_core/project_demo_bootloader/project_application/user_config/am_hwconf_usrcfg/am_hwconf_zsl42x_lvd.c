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
 * \brief ZLG LVD 用户配置文件
 * \sa am_hwconf_zsl42x_lvd.c
 *
 * \internal
 * \par Modification history
 * - 1.00 19-09-27
 * \endinternal
 */

#include "am_clk.h"
#include "am_gpio.h"
#include "am_zsl42x.h"
#include "am_zsl42x_lvd.h"
#include "hw/amhw_zsl42x_lvd.h"
#include "zsl42x_pin.h"

/**
 * \addtogroup am_if_src_hwconf_zsl42x_lvd
 * \copydoc am_hwconf_zsl42x_lvd.c
 * @{
 */

/**
 * \brief LVD 平台初始化
 */
void __zsl42x_plfm_lvd_init (void)
{
    /* 开启LVD时钟 */
    am_clk_enable (CLK_VC_LVD);
}

/**
 * \brief LVD 平台去初始化
 */
void __zsl42x_plfm_lvd_deinit (void)
{
    /* 关闭DAC时钟 */
    am_clk_disable(CLK_VC_LVD);
}

/** \brief LVD 设备信息 */
static const am_zsl42x_lvd_devinfo_t __g_lvd_devinfo =
{
    /**< \brief 指向LVD寄存器块的指针 */
    ZSL42x_LVD_BASE,

    /**< \brief LVD中断编号 */
    INUM_LVD,

    /**
     * \brief LVD触发条件
     *
     * \note 八种触发条件，如需要选择多个触发条件，
     *       例：AMHW_ZSL42x_LVD_FTEN | AMHW_ZSL42x_LVD_RTEN。
     */
    AMHW_ZSL42x_LVD_RTEN,

    /**
     * \brief LVD数字滤波时间,用户配置值参考枚举定义：LVD 数字滤波时间
     */
    AMHW_ZSL42x_LVD_DEB_TIME_7P2_MS,

    /**
     * \brief LVD阈值电压,用户配置值参考枚举定义：LVD 阈值电压
     */
    AMHW_ZSL42x_LVD_VIDS_2P5_V,

    /**
     * \brief LVD监测来源,用户配置值参考宏定义：LVD 监测来源选择掩码
     */
    AMHW_ZSL42x_LVD_SRC_AVCC,

    /**
     * \brief LVD触发动作,用户配置值参考宏定义：LVD 触发动作选择掩码
     *
     * \note 此配置位只能选择为中断或者复位触发动作
     */
    AMHW_ZSL42x_LVD_TRI_ACT_SYSTEM_RESET,

    /**< \brief LVD平台初始化函数 */
    __zsl42x_plfm_lvd_init,

    /**< \brief LVD平台解初始化函数  */
    __zsl42x_plfm_lvd_deinit,
};

/** \brief LVD设备实例 */
static am_zsl42x_lvd_dev_t __g_lvd_dev;

/** \brief LVD 实例初始化，获得LVD标准服务句柄 */
am_zsl42x_lvd_handle_t am_zsl42x_lvd_inst_init (void)
{
    return am_zsl42x_lvd_init(&__g_lvd_dev, &__g_lvd_devinfo);
}

/** \brief LVD 实例解初始化 */
void am_zsl42x_lvd_inst_deinit (am_zsl42x_lvd_handle_t handle)
{
    am_zsl42x_lvd_deinit(handle);
}

/**
 * @}
 */

/* end of file */
