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
 * \brief PMU驱动接口声明
 *
 *  1. 系统模式切换；
 *  2. 掉电检测配置；
 *  3. LLWU唤醒源配置及LLWU中断连接；
 *
 * \internal
 * \par Modification history
 * - 1.00 16-09-14  ari, first implementation.
 * \endinternal
 */

#ifndef __AM_KL26_PMU_H
#define __AM_KL26_PMU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"
#include "hw/amhw_fsl_smc.h"
#include "hw/amhw_fsl_pmc.h"
#include "hw/amhw_fsl_smc.h"
#include "hw/amhw_fsl_rcm.h"
#include "hw/amhw_fsl_llwu.h"


/**
 * \addtogroup am_kl26_if_pmu
 * \copydoc am_kl26_pmu.h
 * @{
 */

/**
 * \name PMU设备初始化参数
 *  
 * 用于初始化设备信息中flags参数
 * @{
 */

/*
 *  bit[ 0 ] : 复位引脚stop模式LPO滤波使能
 *  bit[1-5] : 复位引脚run和wait模式滤波时钟个数（BUS时钟）（0-32）
 *  bit[ 6 ] : VLPx模式是否使能能带隙
 *  bit[ 7 ] : 是否使能能带隙缓冲
 *  bit[ 8 ] : VLSS0模式是否禁能POR
 */

/** \brief 复位引脚stop模式LPO滤波使能 */
#define AM_KL26_PMU_ARG_REST_PIN_STOP_FILT_ENABLE    AM_BIT(0)

/** \brief 复位引脚run模式滤波时钟配置 */
#define AM_KL26_PMU_ARG_REST_PIN_RUN_FILT_C(count)   AM_SBF((count),1

/** \brief VLPx模式能带隙使能 */
#define AM_KL26_PMU_ARG_VLPx_BANDGAP_ENABLE          AM_BIT(6)

/** \brief 能带隙缓冲是否使能 */
#define AM_KL26_PMU_ARG_BANDGAP_BUF_ENABLE           AM_BIT(7)

/** \brief VLSS0模式禁能POR，降低功耗 */
#define AM_KL26_PMU_ARG_VLLS0_POR_DISABLE            AM_BIT(8)

/** \brief 是否清除LLWU唤醒复位的ACK标志(清除后引脚释放) */
#define AM_KL26_PMU_ARG_VLLSx_ISOACK_CLEAR           AM_BIT(9)

/** @}*/

/**
 * \brief PMU 设备信息结构体
 */
typedef struct am_kl26_pmu_devinfo {

    amhw_fsl_smc_t  *p_hw_smc;   /**< \brief 指向SMC寄存器块的指针  */
    amhw_fsl_pmc_t  *p_hw_pmc;   /**< \brief 指向PMC寄存器块的指针  */

    amhw_fsl_rcm_t  *p_hw_rcm;   /**< \brief 指向RCM寄存器块的指针  */
    amhw_fsl_llwu_t *p_hw_llwu;  /**< \brief 指向LLWU寄存器块的指针 */
    uint32_t          flags;      /**< \brief 初始化配置参数         */

    /** \brief 平台初始化函数，配置引脚等工作 */
    void  (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void  (*pfn_plfm_deinit)(void);
} am_kl26_pmu_devinfo_t;

/**
 * \brief PMU 设备结构体
 */
typedef struct am_kl26_pmu_dev {

    /** \brief 指向PMU设备信息指针 */
    const am_kl26_pmu_devinfo_t *p_devinfo;

    /** \brief 报警中断回调函数指针 */
    am_pfnvoid_t                 p_warn_func;

    /** \brief 报警中断回调函数参数 */
    void                        *p_warn_arg;
} am_kl26_pmu_dev_t;

/**
 * \brief PMU 系统模式
 *
 *        RUN <--> VPLR时钟模式切换：PEE <--> BLPE
 *                                FEI <--> BLPI
 */
typedef enum am_kl26_pmu_mode {
    AM_KL26_PMU_MODE_RUN = 0,       /**< \brief 运行模式          */
    AM_KL26_PMU_MODE_VLPR,          /**< \brief 超低功耗运行模式  */
    AM_KL26_PMU_MODE_WAIT,          /**< \brief 等待模式          */
    AM_KL26_PMU_MODE_VLPW,          /**< \brief 超低功耗等待模式  */
    AM_KL26_PMU_MODE_STOP2,         /**< \brief 停止模式2 */
    AM_KL26_PMU_MODE_STOP1,         /**< \brief 停止模式1 */
    AM_KL26_PMU_MODE_STOP,          /**< \brief 正常停止模式      */
    AM_KL26_PMU_MODE_VLPS,          /**< \brief 超低功耗停止模式  */
    AM_KL26_PMU_MODE_LLS,           /**< \brief 低漏电停止模式    */
    AM_KL26_PMU_MODE_VLLS3,         /**< \brief 超低漏电停止模式3 */
    AM_KL26_PMU_MODE_VLLS1,         /**< \brief 超低漏电停止模式1 */
    AM_KL26_PMU_MODE_VLLS0          /**< \brief 超低漏电停止模式0 */
} am_kl26_pmu_mode_t;

/** \brief 掉电报警电压阈值（报警电压大于复位电压） */
typedef enum amhw_kl26_pmu_warn_v_sel {
    AM_KL26_PMU_WARN_V1 = 0,              /**< \brief 选择报警阈值1(1.8v)\(2.7V) */
    AM_KL26_PMU_WARN_V2,                  /**< \brief 选择报警阈值2(1.9V)\(2.8V) */
    AM_KL26_PMU_WARN_V3,                  /**< \brief 选择报警阈值3(2.0V)\(2.9V) */
    AM_KL26_PMU_WARN_V4                   /**< \brief 选择报警阈值4(2.1V)\(3.0V) */
} amhw_kl26_pmu_warn_v_sel_t;

/** \brief 掉电复位电压阈值 */
typedef enum am_kl26_pmu_rest_v_sel {
    AM_KL26_PMU_REST_VL = 0,              /**< \brief 选择低阈值检测(1.6V ) */
    AM_KL26_PMU_REST_VH                   /**< \brief 选择高阈值检测(2.56V) */
} am_kl26_pmu_rest_v_sel_t;

/** \brief PMU 掉电信息配置 */
typedef struct am_kl26_pmu_brownout_info {
    amhw_kl26_pmu_warn_v_sel_t  warn_v;      /**< \brief 报警电压选择 */
    am_kl26_pmu_rest_v_sel_t    rest_v;      /**< \brief 复位电压选择 */
    am_pfnvoid_t                p_callfunc;  /**< \brief 报警回调函数 */
    void                       *p_arg;       /**< \brief 回调参数     */
    am_bool_t                      enable;      /**< \brief 是否使能     */
} am_kl26_pmu_brownout_info_t;

/** \brief LLWU唤醒源信息
 *  
 * 应使用函数am_kl26_pmu_llwu_src_x_mkinfo()初始化该结构体
 */
typedef struct am_kl26_pmu_llwu_src_info {
    uint32_t src;         /**< \brief 唤醒源         */
    uint32_t extra_info;  /**< \brief 唤醒源附加信息 */
} am_kl26_pmu_llwu_src_info_t;

/**
 * \brief PMU初始化
 *
 * \param[in] p_dev     : 指向PMU设备结构体的指针
 * \param[in] p_devinfo : 指向PMU设备信息结构体常量的指针
 *
 * \retval AM_OK      : 初始化成功
 * \retval -AM_EINVAL : 参数错误
 */
int am_kl26_pmu_init (am_kl26_pmu_dev_t *p_dev, const am_kl26_pmu_devinfo_t *p_devinfo);

/**
 * \brief PMU去初始化
 * \param[in] p_dev     : 指向PMU设备结构体的指针
 * \return 无
 */
void am_kl26_pmu_deinit (am_kl26_pmu_dev_t *p_dev);

/**
 * \brief 配置系统模式
 *
 *  1.模式切换是有条件的，如：VLPR到WAIT，应先切换到RUN模式，再切换到WAIT模式\n
 *  2.如果模式切换等待或停止模式，将在此函数内部停止，非复位唤醒后会退出
 *    并返回AM_OK;\n
 *  3.如果模式切换不成功或参数错误，该函数会返回-AM_EINVAL或AM_ERROR，
 *    调试时可用断言判定；\n
 *  4.VLLSx模式唤醒后将执行复位操作。
 *
 * \param[in] mode : 系统模式定义
 *
 * \retval AM_OK      ：模式切换有效
 * \retval AM_ERROR   ：模式切换错误
 * \retval -AM_EINVAL ：模式参数错误
 */
int am_kl26_pmu_mode_into (am_kl26_pmu_mode_t mode);

/**
 * \brief 获取系统模式
 * \retval AM_KL26_PMU_MODE_RUN  : 运行模式
 * \retval AM_KL26_PMU_MODE_VLPR : 低功耗运行模式
 */
am_kl26_pmu_mode_t am_kl26_pmu_mode_get (void);

/**
 * \brief 配置掉电检测信息
 *
 * \param[in] p_info : 指向掉电信息结构体指针
 *
 * \retval AM_OK      : 配置成功
 * \retval -AM_EINVAL : 参数错误
 * \retval AM_ERROR   : 配置失败
 *
 * \note VLPx,LLS和VLLSx模式不支持掉电检测，返回AM_ERROR
 */
int am_kl26_pmu_brownout_cfg (am_kl26_pmu_brownout_info_t *p_info);

/**
 * \brief 构造掉电检测信息结构体
 *
 * \param[in] p_info : 指向掉电信息结构体指针
 * \param[in] warn_v : 报警电压
 * \param[in] rest_v : 复位电压（报警电压高于复位电压）
 * \param[in] p_func : 报警回调函数
 * \param[in] p_arg  : 回调函数参数
 * \param[in] enable : 是否使能
 *      \arg TURE  : 开启报警中断和复位使能
 *      \arg AM_FALSE : 不使用掉电检测功能
 *
 * \retval AM_OK 构造成功
 */
am_static_inline
int am_kl26_pmu_brownout_mkinfo (am_kl26_pmu_brownout_info_t *p_info,
                                 amhw_kl26_pmu_warn_v_sel_t   warn_v,
                                 am_kl26_pmu_rest_v_sel_t     rest_v,
                                 am_pfnvoid_t                 p_func,
                                 void                        *p_arg,
                                 am_bool_t                       enable)
{
    p_info->warn_v     = warn_v;
    p_info->rest_v     = rest_v;
    p_info->p_callfunc = p_func;
    p_info->p_arg      = p_arg;
    p_info->enable     = enable;

    return AM_OK;
}

/**
 * \brief 配置LLWU唤醒源信息
 * \param[in] p_info : 指向唤醒信息结构体指针
 * \retval AM_OK 构造成功
 */
int am_kl26_pmu_llwu_src_cfg (am_kl26_pmu_llwu_src_info_t *p_info);
        
/**
 * \brief 构造引脚的唤醒信息结构体
 *
 * \param[in] p_info : 指向唤醒信息结构体指针
 * \param[in] pin    : 唤醒引脚
 * \param[in] arg    : 引脚参数
 *
 * \retval AM_OK 构造成功
 *
 * \note 唤醒引脚应配置为输入模式才能产生唤醒
 */
am_static_inline
int am_kl26_pmu_llwu_src_pin_mkinfo (am_kl26_pmu_llwu_src_info_t *p_info,
                                     amhw_fsl_llwu_pin_t         pin,
                                     amhw_fsl_llwu_pin_arg_t     arg)
{
    p_info->src        = pin | AM_SBF(0x00, 24);
    p_info->extra_info = arg;

    return AM_OK;
}

/**
 * \brief 构造外设模块的唤醒信息结构体
 *
 * \param[in] p_info : 指向唤醒信息结构体指针
 * \param[in] module : 外设模块
 * \param[in] arg    : 模块参数
 *
 * \retval AM_OK 构造成功
 */
am_static_inline
int am_kl26_pmu_llwu_src_module_mkinfo (am_kl26_pmu_llwu_src_info_t *p_info,
                                        amhw_fsl_llwu_module_t      module,
                                        amhw_fsl_llwu_module_arg_t  arg)
{
    p_info->src        = module | AM_SBF(0x01, 24);
    p_info->extra_info = arg;

    return AM_OK;
}

/**
 * \brief 构造滤波通道的唤醒信息结构体
 *
 * \param[in] p_info : 指向唤醒信息结构体指针
 * \param[in] chan   : 滤波通道
 * \param[in] pin    : 唤醒引脚
 * \param[in] arg    : 通道参数
 *
 * \retval AM_OK 构造成功
 */
am_static_inline
int am_kl26_pmu_llwu_src_filt_mkinfo (am_kl26_pmu_llwu_src_info_t   *p_info,
                                      amhw_fsl_llwu_filt_chan_t     chan,
                                      amhw_fsl_llwu_pin_t           pin,
                                      amhw_fsl_llwu_filt_chan_arg_t arg)
{
    p_info->src         = chan | AM_SBF(0x02, 24);
    p_info->extra_info  = pin;
    p_info->extra_info |= arg << 16;

    return AM_OK;
}

/**
 * \brief LLWU中断处理函数
 *
 * 获取唤醒源标志和清除唤醒源标志，该函数由中断调用
 *
 * \retval 无
 */
void am_kl26_pmu_llwu_irq_handle (void);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_KL26_PMU_H */

/*end of file */
