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
 * \brief LPTMR实现 lowpower softimer 的接口实现
 *
 * \internal
 * \par Modification history
 * - 1.00 20-02-13  ebi, first implementation.
 * - 1.01 20-09-23  yg,  adaptation zsl42x.
 * \endinternal
 */

#ifndef __AM_ZSL42X_LPSOFTIMER_PORTING_H
#define __AM_ZSL42X_LPSOFTIMER_PORTING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_int.h"
#include "am_delay.h"

#include "am_zsl42x.h"
#include "hw/amhw_zsl42x_rcc.h"
#include "hw/amhw_zsl42x_lptim.h"

/**
 * @addtogroup am_if_lpsoftimer_if
 * @copydoc am_zsl42x_lpsoftimer_if.h
 * @{
 */

/**
 * \brief softimer 时间域定义
 */
typedef uint32_t am_lpsoftimer_time_if_t;

/**
 * \brief LPTMR定时功能相关的设备信息
 */
typedef struct am_zsl42x_lptmr_lpsoftimer_devinfo {
    amhw_zsl42x_lptim_t  *p_hw_lptmr0;       /**< \brief 指向LPTIMER寄存器块的指针 */
    amhw_zsl42x_lptim_t  *p_hw_lptmr1;       /**< \brief 指向LPTIMER寄存器的指针 */
    uint8_t             lptmr0_inum;       /**< \brief LPTMR0中断号  */
    uint32_t            xtl_clk;           /**< \brief 外部低速时钟(一般为32.768kHz)*/

    float               temp_coef;         /**< \brief 温度抛物线系数 */
    float               temp_coef_sub;     /**< \brief 温度抛物线系数误差范围 */
    float               temp_turnover;     /**< \brief 抛物线翻转温度点 */
    float               temp_turnover_sub; /**< \brief 抛物线翻转温度点误差范围 */

    double              timer_offset_coef; /**< \brief 定时器误差系数 */

    /** \brief 平台初始化函数，如打开时钟，配置引脚等工作 */
    void              (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void              (*pfn_plfm_deinit)(void);

} am_zsl42x_lptmr_lpsoftimer_devinfo_t;

/**
 * \brief lpsoftimer初始化
 * \param[in] p_devinfo : 指向lpsoftimer设备信息的指针
 * \retval AM_OK        : 操作成功
 * \retval -AM_EINVAL   : 参数有误
 */
int am_zsl42x_lptmr_lpsoftimer_init (
    const am_zsl42x_lptmr_lpsoftimer_devinfo_t *p_devinfo);

/**
 * \brief lpsoftimer去初始化
 * \param[in] 无
 * \return 无
 */
void am_zsl42x_lptmr_lpsoftimer_deinit (void);

/**
 * \brief lpsoftimer 温度补偿
 *
 * \param[in] period      : 时间周期
 * \param[in] temperature : 温度
 *
 * \return 补偿后的时间周期
 */
uint32_t am_lpsoftimer_temp_compensation (uint32_t period, float temperature);

/**
 * \brief softimer 设置下一次超时
 * \param[in] timeout : 从现在开始多久之后进入定时器中断
 * \return 无
 */
void am_lpsoftimer_timeout_set (am_lpsoftimer_time_if_t timeout);

/**
 * \brief softimer 获取当前毫秒计时值(自系统启动以来)
 * \param[in] 无
 * \return 计时值
 */
am_lpsoftimer_time_if_t am_lpsoftimer_current_get (void);

/**
 * @}
 */

void am_lora_if_lpsoftimer_init (void);

#ifdef __cplusplus
}
#endif

#endif /* __AM_ZSL42X_LPSOFTIMER_PORTING_H */

/* end of file */
