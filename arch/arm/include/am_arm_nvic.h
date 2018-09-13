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
 * \brief  NVIC驱动，服务中断标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 17-04-10  sdy, modified.
 * - 1.00 15-01-29  hbt, first implementation.
 * \endinternal
 */

#ifndef __AM_ARM_NVIC_H
#define __AM_ARM_NVIC_H

#include "hw/amhw_arm_nvic.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_arm_if_nvic
 * \copydoc am_arm_nvic.h
 * @{
 */

/**
 * \brief 系统异常编号定义
 */
#define    INUM_NMI             -14    /**< \brief 不可屏蔽中断 */
#define    INUM_HARDFAULT       -13    /**< \brief 硬件错误 */
#define    INUM_MEMMANAGE       -12    /**< \brief 存储器管理异常（M0、M0+无此异常） */
#define    INUM_BUSFAULT        -11    /**< \brief 总线异常（M0、M0+无此异常） */
#define    INUM_USAGEFAULT      -10    /**< \brief 应用异常（M0、M0+无此异常） */
#define    INUM_SVCALL          -5     /**< \brief 系统调用异常 */
#define    INUM_PENDSV          -2     /**< \brief 可悬起系统调用异常 */
#define    INUM_SYSTICK         -1     /**< \brief 系统滴答定时器异常 */

enum {
    AM_ARM_NVIC_CORE_M0,      /**< \brief M0 内核 */
    AM_ARM_NVIC_CORE_M0PLUS,  /**< \brief M0+内核 */
    AM_ARM_NVIC_CORE_M3,      /**< \brief M3 内核 */
    AM_ARM_NVIC_CORE_M4,      /**< \brief M4 内核 */
};

/**
 * \brief 中断服务信息
 */
struct am_arm_nvic_servinfo {

    /** \brief 起始中断号 */
    int inum_start;

    /** \brief 末尾中断号 */
    int inum_end;
};

/** \brief 中断服务例行信息 */
struct am_arm_nvic_isr_info {

    /** \brief 触发回调函数 */
    am_pfnvoid_t  pfn_isr;

    /** \brief 回调函数的参数 */
    void         *p_arg;
};


/** \brief 中断设备信息 */
typedef struct am_arm_nvic_devinfo {
    
    /** \brief 中断服务信息实例 */
    const struct am_arm_nvic_servinfo int_servinfo;
    
    /** \brief 内核 */
    uint8_t core;

    /** \brief 优先级位数 */
    uint8_t prio_bits;

    /** \brief 设置优先级组 */
    int group;

    /** \brief 总中断数量 */
    int input_cnt;

    /** \brief ISR 信息映射(大小与 input_cnt 一致) */
    uint8_t *p_isrmap;

    /** \brief ISR 信息数量 */
    int isrinfo_cnt;

    /** \brief ISR 信息映射内存(大小与 isrinfo_cnt 一致) */
    struct am_arm_nvic_isr_info *p_isrinfo;
    
    /** \brief 平台初始化函数 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
    
} am_arm_nvic_devinfo_t;

/**
 * \brief 中断设备实例 
 */
typedef struct am_arm_nvic_dev {
    
    /** \brief 指向中断设备信息的指针 */
    const am_arm_nvic_devinfo_t *p_devinfo;
    
    /** \brief 参数有效标志 */
    am_bool_t                    valid_flg;
    
} am_arm_nvic_dev_t;


/**
 * \brief	设置中断优先级
 *
 * \param[in]	  inum             : 中断号
 * \param[in]	  preempt_priority : 主优先级
 * \param[in]     sub_priority     : 次优先级（针对m0内核此值无效）
 *
 * \retval  AM_OK       : 操作成功
 * \retval -AM_EINVAL   : 无效参数
 */
int am_arm_nvic_priority_set(int inum,
                             uint32_t preempt_priority,
                             uint32_t sub_priority);

/**
 * \brief 中断初始化 
 *
 * \param[in] p_dev     : 指向中断设备的指针 
 * \param[in] p_devinfo : 指向中断设备信息的指针 
 *
 * \retval  AM_OK       : 操作成功 
 * \retval -AM_EINVAL   : 无效参数 
 */
int am_arm_nvic_init (am_arm_nvic_dev_t           *p_dev,
                      const am_arm_nvic_devinfo_t *p_devinfo);

/**
 * \brief 中断去初始化
 * \return  无 
 */
void am_arm_nvic_deinit (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* __AM_ARM_NVIC_H */

/* end of file */
