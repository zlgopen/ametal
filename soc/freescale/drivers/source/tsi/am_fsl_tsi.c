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
 * \brief TSI implementation
 *
 * 1. 电极震荡频率计算公式：
 *                        I(恒定电流)
 *      F(elec) = ---------------------------------
 *                  2 * C(电极电容) * V(电压增量)
 *
 * 2. 参考震荡频率计算公式：
 *                        I(参考恒定电流)
 *      F(ref)  = --------------------------------------
 *                  2 * C(内部参考电容) * V(电压增量)
 *
 * 3. 采样时间计算公式：
 *                 2 * PS(分频数) * NSCN(扫描次数) * C(电极电容) * V(电压增量)
 *      T(samp) = -----------------------------------------------------------------
 *                                  I(恒定电流)
 * \internal
 * \par Modification history
 * - 1.01 16-09-26  nwt, make some changes.
 * - 1.00 15-10-21  wxj, first implementation.
 * \endinternal
 */

#include "am_fsl_tsi.h"

/** \brief 定义指向TSI信息的指针 */
#define __TSI_DEVINFO_DECL(p_tsi_devinfo, p_dev)  \
        const am_fsl_tsi_devinfo_t *p_tsi_devinfo = p_dev->p_devinfo


/*******************************************************************************
  全局变量
*******************************************************************************/

/** \brief 指向TSI设备的指针 */
am_fsl_tsi_dev_t *__gp_tsi_dev;

/*******************************************************************************
  内部函数声明
*******************************************************************************/

static uint16_t __am_fsl_tsi_basevale_get (void);
static void __am_fsl_tsi_thresh_set (uint16_t val_h, uint16_t val_l);

/**
 * \brief TSI设置通道上、下限阀值
 */
void __am_fsl_tsi_thresh_set (uint16_t val_h, uint16_t val_l)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    amhw_fsl_tsi_wake_chan_thresholdh_write(p_tsi_devinfo->p_hw_tsi, val_h);
    amhw_fsl_tsi_wake_chan_thresholdl_write(p_tsi_devinfo->p_hw_tsi, val_l);
}

/**
 * \brief TSI 自动检测，获取当前环境下未触摸时的检测值
 */
uint16_t __am_fsl_tsi_basevale_get ()
{
    uint16_t trigger_backup;    /* 保存触发方式 */
    uint16_t calib_val[10];     /* 存放10次检测值 */
    uint16_t base_val = 0;      /* 存放当前环境下的基础值 */
    uint16_t i = 0;             /* 循环变量 */
    
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    /* 读取设置的触发方式 */
    if(amhw_fsl_tsi_scan_trig_get(p_tsi_devinfo->p_hw_tsi)) {
        trigger_backup = AM_TRUE;          /* 硬件触发 */
    } else {
        trigger_backup = AM_FALSE;         /* 软件触发 */
    }
    
    /* 清除扫描结束标志 */
    amhw_fsl_tsi_eosf_clr(p_tsi_devinfo->p_hw_tsi);

    /* 配置为软件触发扫描 */
    amhw_fsl_tsi_scan_trig_set(p_tsi_devinfo->p_hw_tsi,AMHW_FSL_TSI_TRIG_SOFTWARE);

    for (i = 0; i<10; i++) {

        /* 启动软件触发扫描 */
        amhw_fsl_tsi_soft_tirg_start(p_tsi_devinfo->p_hw_tsi);

        /* 获取扫描结束标志,等待扫描结束 */
        while ( !amhw_fsl_tsi_eosf_get(p_tsi_devinfo->p_hw_tsi));

        /* 读取没有触摸时的值 */
        calib_val[i] = amhw_fsl_tsi_convert_value_get(p_tsi_devinfo->p_hw_tsi);

        base_val = base_val + calib_val[i];

        /* 清扫描结束标志 */
        amhw_fsl_tsi_eosf_clr(p_tsi_devinfo->p_hw_tsi);
    }
      
    /* 恢复为设置的触发方式 */
    if (trigger_backup == AM_TRUE) {
        amhw_fsl_tsi_scan_trig_set(p_tsi_devinfo->p_hw_tsi, AMHW_FSL_TSI_TRIG_HAEDWARE);
    } 
    
    /* 返回基础平均值 */
    return (base_val / 10);
}

/**
 * \brief TSI 灵敏度设置
 */
void am_fsl_tsi_sens_set (uint16_t val)
{

    uint16_t base_val;                      /* 存放基础阀值 */
    uint16_t tshd_val, tsld_val;            /* 存放设置的上/下限阀值 */
    
    base_val = __am_fsl_tsi_basevale_get();   /* 读取基础转换值 */
    
    tshd_val = val + base_val;              /* 上限为设置值加基础值 */
    tsld_val = base_val - base_val / 2;     /* 下限为基础值的一半 */

    /* 写入上/下限值 */
    __am_fsl_tsi_thresh_set(tshd_val,tsld_val);
}

/**
 * \brief TSI中断触发模式
 */
void am_fsl_tsi_trigger_cfg (amhw_fsl_tsi_irq_mode_t flag)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    amhw_fsl_tsi_int_mode_set (p_tsi_devinfo->p_hw_tsi, flag);
}

/**
 * \brief TSI中断使能
 */
void am_fsl_tsi_int_enable (void)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    /* TSI中断使能 */
    amhw_fsl_tsi_int_enable(p_tsi_devinfo->p_hw_tsi);

    /* 系统中断使能 */
    am_int_enable(p_tsi_devinfo->inum);
}

/**
 * \brief TSI中禁能
 */
void am_fsl_tsi_int_disable (void)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    /* TSI 中断禁能 */
    amhw_fsl_tsi_int_disable(p_tsi_devinfo->p_hw_tsi);
    
    /* 系统中断禁能 */
    am_int_enable(p_tsi_devinfo->inum);
}

/**
 * \brief 连接中断回调函数
 */
int am_fsl_tsi_trigger_connect (am_pfnvoid_t pfn_callback)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);
    int key;

    /* 关闭CPU中断 */
    key = am_int_cpu_lock();

    /* 连接回调函数 */
    am_int_connect(p_tsi_devinfo->inum, pfn_callback, NULL);

    /* 打开中断 */
    am_int_cpu_unlock(key);

     return AM_OK;
}

/**
 * \brief TSI 功能配置
 */
int am_fsl_tsi_fun_cfg (amhw_fsl_tsi_channel_t    chan,
                         amhw_fsl_tsi_mode_t       work_mode,
                         amhw_fsl_tsi_trig_mode_t  trig_mode)
{
    __TSI_DEVINFO_DECL(p_tsi_devinfo, __gp_tsi_dev);

    /* 使能TSI通道 */
    amhw_fsl_tsi_chan_set(p_tsi_devinfo->p_hw_tsi, chan);

    /* 设置TSI工作模式 */
    amhw_fsl_tsi_mode_set(p_tsi_devinfo->p_hw_tsi, work_mode);

    /* 配置触发扫描模式 */
    amhw_fsl_tsi_scan_trig_set(p_tsi_devinfo->p_hw_tsi, trig_mode);

    /* 低功耗唤醒功能 */
    if(p_tsi_devinfo->lp_mode == AM_TRUE) {
        amhw_fsl_tsi_lowpower_enable(p_tsi_devinfo->p_hw_tsi);
    }

    /* 设置电压增量为600mV(Vp - Vm) */
    amhw_fsl_tsi_dvolt_set (p_tsi_devinfo->p_hw_tsi, AMHW_FSL_TSI_DV073);

    /* 扫描预分频设置为2分频 */
    amhw_fsl_tsi_oscfreq_div_set(p_tsi_devinfo->p_hw_tsi, AMHW_FSL_TSI_OSCFRQ_DIV_2);

    /* 设置电极扫描次数为16 */
    amhw_fsl_tsi_scan_num_set(p_tsi_devinfo->p_hw_tsi, 16);

    /* 设置外部振荡器恒定电流为16uA */
    amhw_fsl_tsi_extchrg_val_set(p_tsi_devinfo->p_hw_tsi, AMHW_FSL_TSI_EXTCHRG_16UA);

    /* 设置参考振荡器恒定电流源为12uA ,则参考时钟为10MHz*/
    amhw_fsl_tsi_refchrg_val_set(p_tsi_devinfo->p_hw_tsi, AMHW_FSL_TSI_REFCHRG_32UA);

    return AM_OK;
}

/**
 * \brief TSI初始化
 */
int am_fsl_tsi_init (am_fsl_tsi_dev_t *p_dev, const am_fsl_tsi_devinfo_t *p_devinfo)
{

    if (p_devinfo == NULL || p_devinfo->p_hw_tsi == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo = p_devinfo;

    __gp_tsi_dev = p_dev;

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    if (p_devinfo->lp_mode != AM_FALSE) {
        /* 添加低功耗模式下继续运行设置 */
        amhw_fsl_tsi_lowpower_enable(p_devinfo->p_hw_tsi);
    }

    /* 中断禁能 */
    amhw_fsl_tsi_int_disable(p_devinfo->p_hw_tsi);

    /* 使能TSI模块 */
    amhw_fsl_tsi_mode_enable(p_devinfo->p_hw_tsi);

    return AM_OK;
}

/**
 * \brief TSI 解初始化
 */
void am_fsl_tsi_deinit (am_fsl_tsi_dev_t *p_dev)
{
    amhw_fsl_tsi_t *p_hw_tsi = p_dev->p_devinfo->p_hw_tsi;

    const am_fsl_tsi_devinfo_t *p_devinfo = p_dev->p_devinfo;

    if (p_devinfo->pfn_plfm_deinit) {
        p_devinfo->pfn_plfm_deinit();
    }

    /* 中断禁能 */
    amhw_fsl_tsi_int_disable(p_hw_tsi);

    /* 禁能TSI模块 */
    amhw_fsl_tsi_mode_disable(p_hw_tsi);
}


/* end of file */
