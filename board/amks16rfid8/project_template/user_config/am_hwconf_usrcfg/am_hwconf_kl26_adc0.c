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
 * \brief FSL ADC 用户配置文件。
 * \sa am_kl26_hwconfig_adc0.c
 *
 * \internal
 * \par Modification history
 * - 1.00 16-10-09  mkr, first implementation.
 * \endinternal
 */
#include "hw/amhw_fsl_adc.h"
#include "hw/amhw_kl26_sim.h"
#include "am_fsl_adc.h"
#include "am_kl26.h"
#include "am_gpio.h"
#include "kl26_pin.h"

/**
 * \brief ADC 模式设置，默认开启软件触发
 *
 * \note 一次只能开启其中一种模式，需要使用哪种模式，只需将对应ADC模式宏注释符去掉即可。
 *       如果使用硬件触发模式，则应该在平台初始化函数中选择硬件触发源，还必须初始化相应的硬件。
 * 		  如果同时开启了两种模式则认为是软件触发模式，或者两种模式都没开，则认为是软件触发模式。
 */
#define AM_FSL_ADC_SW_TRG           /**< \brief ADC 软件触发  */
//#define AM_FSL_ADC_HW_TRG           /**< \brief ADC 硬件触发 */

/**
 * \name ADC 功能设置
 * @{
 */
//#define AM_FSL_ADC_FUN_CMP_EN       /**< \brief 使能比较功能 */
//#define AM_FSL_ADC_FUN_AVG_EN       /**< \brief 使能硬件平均功能  */
//#define AM_FSL_ADC_FUN_DIF_EN       /**< \brief 使能差分功能 */

/** @} */

/**
 * \addtogroup am_kl26_if_hwconfig_src_adc0
 * \copydoc am_kl26_hwconfig_adc0.c
 * @{
 */

/**
 * \brief ADC 平台初始化。
 *
 * \note 如果某个引脚支持差分模式，那么除了配置引脚功能外，还要选择使用A通道还是B通道。
 */
static void __fsl_plfm_adc0_init (void)
{
    /* 使能ADC时钟。对ADC相关寄存器的操作一定要在使能ADC时钟之后 */
    amhw_kl26_sim_periph_clock_enable(KL26_SIM_SCGC_ADC0);

#ifdef AM_FSL_ADC_FUN_DIF_EN
    /* 配置PIOE_20为ADC0_DP0输入 */
    am_gpio_pin_cfg(PIOE_20, PIOE_20_ADC0_DP0);

    /* 配置PIOE_21为ADC0_DM0输入 */
    am_gpio_pin_cfg(PIOE_21, PIOE_21_ADC0_DM0);
#else
    /* 配置PIOE_29为ADC0_SE0输入 */
    am_gpio_pin_cfg(PIOE_29, PIOE_29_ADC0_SE4B);

	/**\brief 选择通道B */
	amhw_fsl_adc_seq_set(KL26_ADC0, AMHW_FSL_ADC_MUXSEL_B);

#endif
    /* 对ADC进行校准                */
    amhw_fsl_adc_calibrate(KL26_ADC0, CLK_BUS);

    /**
     * \note
     * 根据需要设置采样的通道，
     * ADC通道4~7分别对应两种类型的通道，分别为a和b，
     * 根据需要设置下面的muxsel，系统默认情况下使用的是a通道，
     * 当需要使用b通道时，需要在下面加入以下语句，开启b通道
     * 如需要使用PIOD_6通道作为ADC通道7的B通道使用时，配置配置如下：
     *
     * am_gpio_pin_cfg (PIOD_6, PIOD_6_ADC0_SE7B);
     *
     * amhw_fsl_adc_seq_set((amhw_fsl_adc_t *)KL26_ADC0, AMHW_FSL_ADC_MUXSEL_B);
     */

#if defined(AM_FSL_ADC_HW_TRG)

    /**
     * 使能ADC硬件可选触发源，选择TPM2溢出触发
     * 使用定时器需要进行初始化设置，因为使用TPM2作为软件定时器，
     * 在初始化软件定时器时就完成了TPM2定时器的初始化
     */
    amhw_kl26_sim_adc0_alter_trigger_enable();
    amhw_kl26_sim_adc0_trigger_set(KL26_SIM_ADC0_TRIGGER_TPM2);

#endif

}

/**
 * \brief ADC 平台去初始化。
 */
static void __plfm_adc0_deinit (void)
{
    /* 关闭ADC模块上电                */
    amhw_kl26_sim_periph_clock_disable(KL26_SIM_SCGC_ADC0);
}

/**
 * \brief ADC 设备信息结构体
 * @{
 */

#ifdef AM_FSL_ADC_FUN_AVG_EN
/** \brief ADC 硬件平均功能配置信息结构体 */
static const am_fsl_adc_avg_info_t __g_adc0_avg_info = {
    AM_FSL_ADC_AVG_EN,          /* 开启硬件平均配置 */
    AMHW_FSL_ADC_AVGS_32,       /* 每次平均32个采样值 */
};
#endif

#ifdef AM_FSL_ADC_FUN_CMP_EN
/** \brief ADC 比较功能配置信息结构体 */
static const am_fsl_adc_cmp_info_t __g_adc0_cmp_info = {
    AM_FSL_ADC_CMP_EN,          /* 开启比较 */
    AM_FSL_ADC_CMP_MODE_6,      /* 比较条件为 "res <= a" 或者  "res >= b" */
    ((1 << 16) - 1) / 3,         /* 参考的电压的1/3点 */
    (((1 << 16) - 1) << 1) / 3,  /* 参考的电压的2/3点 */
};
#endif

/** \brief ADC 设备信息  */
static const am_fsl_adc_devinfo_t __g_adc0_devinfo =
{
    KL26_ADC0,                /**< \brief 指向ADC0寄存器块的指针 */
    2500,                          /**< \brief ADC参考电压，单位：mV  */
    16,                            /**< \brief ADC采样精度(位数)   */

	/* 选择差分或者单端模式 */
#ifdef AM_FSL_ADC_DIF_EN
	AM_FSL_ADC_DIFF_EN,            /**< \brief ADC0差分模式        */
#else
    AM_FSL_ADC_DIFF_DIS,           /**< \brief ADC0单端模式        */
#endif

    INUM_ADC0,                     /**< \brief ADC中断向量号  */

	/* 选择软件或者硬件触发模式 */
#ifdef AM_FSL_ADC_SW_TRG
    AM_FSL_ADC_MODE_TRG_SF,        /**< \brief ADC软件触发模式      */
#elif defined(AM_FSL_ADC_HW_TRG)
	AM_FSL_ADC_MODE_TRG_HW,        /**< \brief ADC硬件触发模式      */
#else
	AM_FSL_ADC_MODE_TRG_SF,
#endif

    AMHW_FSL_ADC_SEQ_A,            /**< \brief ADC序列索引            */
    DMA_CHAN_0,                    /**< \brief DMA通道0传输，值为-1时不使用DMA */
    CLK_BUS,                       /**< \brief ADC的时钟源 （一般都是BUS） */

	/* 选择是否使用硬件平均功能 */
#ifdef AM_FSL_ADC_FUN_AVG_EN
	&__g_adc0_avg_info,
#else
    NULL,                          /**< \brief 不使用硬件平均功能       */
#endif

	/* 选择是否使用比较功能 */
#ifdef AM_FSL_ADC_FUN_CMP_EN
	&__g_adc0_cmp_info,
#else
    NULL,                          /**< \brief 不使用比较功能       */
#endif
    __fsl_plfm_adc0_init,

    __plfm_adc0_deinit,

};


/** @} */

/** \brief ADC0设备实例 */
static am_fsl_adc_dev_t __g_adc0_dev;

/**
 * \brief ADC0 实例初始化，获得ADC标准服务句柄
 *
 * \return 无
 */
am_adc_handle_t am_kl26_adc0_inst_init (void)
{
    return am_fsl_adc_init(&__g_adc0_dev, &__g_adc0_devinfo);
}

/**
 * \brief ADC0 实例解初始化
 * \param[in] handle : ADC0句柄值
 *
 * \return 无
 */
void am_kl26_adc0_inst_deinit (am_adc_handle_t handle)
{
    am_fsl_adc_deinit(handle);
}


/**
 * @}
 */

/* end of file */
