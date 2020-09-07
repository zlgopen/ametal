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
 * \brief WDT操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 20-05-11
 * \endinternal
 */

#ifndef __AMHW_HC32F460_WDT_H
#define __AMHW_HC32F460_WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/*
 * \brief 匿名结构体段的开始
 */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/**
 * \addtogroup amhw_hc32f460_if_wdt
 * \copydoc amhw_hc32f460_wdt.h
 * @{
 */

/**
 * \brief WDT 控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t PERI                      : 2;  /**< \brief 计数周期*/
    uint32_t RESERVED2                      : 2;  /**< \brief 保留*/
    __IO uint32_t CKS                       : 4;  /**< \brief 计数时钟*/
    __IO uint32_t WDPT                      : 4;  /**< \brief 刷新允许区域计数值百分比*/
    uint32_t RESERVED12                     : 4;  /**< \brief 保留*/
    __IO uint32_t SLPOFF                    : 1;  /**< \brief WDT在sleep模式下计数许可*/
    uint32_t RESERVED17                     :14;  /**< \brief 保留*/
    __IO uint32_t ITS                       : 1;  /**< \brief 刷新错误/溢出中断/复位选择*/
} stc_wdt_cr_field_t;

/**
 * \brief WDT 状态寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CNT                       :16;  /**< \brief 计数值*/
    __IO uint32_t UDF                       : 1;  /**< \brief 计数下溢标志*/
    __IO uint32_t REF                       : 1;  /**< \brief 刷新错误标志*/
    uint32_t RESERVED18                     :14;  /**< \brief 保留*/
} stc_wdt_sr_field_t;

typedef struct
{
    __IO uint32_t RF                        :16;  /**< \brief 刷新值*/
    uint32_t RESERVED16                     :16;  /**< \brief 保留*/
} stc_wdt_rr_field_t;

/**
 * \brief WDT寄存器块结构体
 */
typedef struct amhw_hc32f460_wdt{
    union
    {
        __IO uint32_t CR;         /**< \brief 控制寄存器*/
        stc_wdt_cr_field_t CR_f;
    };
    union
    {
        __IO uint32_t SR;         /**< \brief 状态寄存器*/
        stc_wdt_sr_field_t SR_f;
    };
    union
    {
        __IO uint32_t RR;         /**< \brief 刷新寄存器*/
        stc_wdt_rr_field_t RR_f;
    };
} amhw_hc32f460_wdt_t;



#define    AMHW_HC32F460_WDT_TRIGGER_INTERRUPT_REQUEST  (0ul << 31)
#define    AMHW_HC32F460_WDT_TRIGGER_RESET_REQUEST      (1ul << 31)

typedef enum {
    AMHW_HC32F460_WDT_SLEEPOFF = (1 << 16),
    AMHW_HC32F460_WDT_SLEEPON  = (0 << 16),
} amhw_hc32f460_wdt_sleepoff_t;


typedef enum {
    AMHW_HC32F460_WDT_REFRESH_RANGE_100              = 0 << 8,  /**< \brief 100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_25             = 1 << 8,  /**< \brief 0%~25% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_25_50            = 2 << 8,  /**< \brief 25%~50% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_50             = 3 << 8,  /**< \brief 0%~50% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_50_75            = 4 << 8,  /**< \brief 50%~75% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_25_AND_50_75   = 5 << 8,  /**< \brief 0%~25% & 50%~75% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_25_75            = 6 << 8,  /**< \brief 25%~75% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_75             = 7 << 8,  /**< \brief 0%~75% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_75_100           = 8 << 8,  /**< \brief 75%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_25_AND_75_100  = 9 << 8,  /**< \brief 0%~25% & 75%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_25_50_AND_75_100 = 10 << 8, /**< \brief 25%~50% & 75%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_50_AND_75_100  = 11 << 8, /**< \brief 0%~50% & 75%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_50_100           = 12 << 8, /**< \brief 50%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_25_AND_50_100  = 13 << 8, /**< \brief 0%~25% & 50%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_25_100           = 14 << 8, /**< \brief 25%~100% */
    AMHW_HC32F460_WDT_REFRESH_RANGE_0_100            = 15 << 8, /**< \brief 0%~100% */
} amhw_hc32f460_wdt_refresh_range_t;


typedef enum {
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_4    = 2,   /**< \brief PCLK3/4 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_64   = 6,   /**< \brief PCLK3/64 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_128  = 7,   /**< \brief PCLK3/128 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_256  = 8,   /**< \brief PCLK3/256 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_512  = 9,   /**< \brief PCLK3/512 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_1024 = 0xA, /**< \brief PCLK3/1024 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_2048 = 0xB, /**< \brief PCLK3/2048 */
    AMHW_HC32F460_WDT_COUNT_CLK_DIV_8192 = 0xD, /**< \brief PCLK3/8192 */
} amhw_hc32f460_wdt_count_clk_div_t;


typedef enum {
    AMHW_HC32F460_WDT_COUNT_PERIOD_256    = 0,   /**< \brief 256 cycle */
    AMHW_HC32F460_WDT_COUNT_PERIOD_4096   = 1,   /**< \brief 4096 cycle */
    AMHW_HC32F460_WDT_COUNT_PERIOD_16384  = 2,   /**< \brief 16384 cycle */
    AMHW_HC32F460_WDT_COUNT_PERIOD_65536  = 3,   /**< \brief 65536 cycle */
} amhw_hc32f460_wdt_count_period_t;


/**
 * \brief 设置cr寄存器的值
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] reg_val  : 寄存器设置值
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_wdt_cr_set (amhw_hc32f460_wdt_t                *p_hw_wdt,
                               uint32_t                            reg_val)
{
    p_hw_wdt->CR = reg_val;
}

/**
 * \brief 获取计数器当前计数值
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 *
 * \return 无
 */
am_static_inline
uint16_t amhw_hc32f460_wdt_count_value_get (amhw_hc32f460_wdt_t  *p_hw_wdt)
{
    return p_hw_wdt->SR_f.CNT;
}

typedef enum {
    AMHW_HC32F460_WDT_STATUS_REF   = 1 << 17,   /**< \brief 刷新错误标志 */
    AMHW_HC32F460_WDT_STATUS_UDF   = 1 << 16,   /**< \brief 计数下溢标志 */
} amhw_hc32f460_wdt_status_flag_t;

/**
 * \brief 获取看门狗状态
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 状态标志
 *
 * \return 无
 */
am_static_inline
am_bool_t amhw_hc32f460_wdt_status_get (amhw_hc32f460_wdt_t             *p_hw_wdt,
                                        amhw_hc32f460_wdt_status_flag_t  flag)
{
    return (p_hw_wdt->SR & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief 清除看门狗状态
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 状态标志
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_wdt_status_clr (amhw_hc32f460_wdt_t             *p_hw_wdt,
                                   amhw_hc32f460_wdt_status_flag_t  flag)
{
    p_hw_wdt->SR &= ~flag;
}



typedef enum {
    AMHW_HC32F460_WDT_REFRESH_START = 0x0123, /**< \brief 刷新计数器要写入的第一个值 */
    AMHW_HC32F460_WDT_REFRESH_END   = 0x3210, /**< \brief 刷新计数器要写入的第二个值 */
} amhw_hc32f460_wdt_refresh_set_t;

/**
 * \brief 刷新看门狗计数器
 *
 * \param[in] p_hw_wdt : 指向WDT寄存器块的指针
 * \param[in] flag     : 刷新计数器要写入的特定值（amhw_hc32f460_wdt_refresh_set_t）
 *
 * \return 无
 */
am_static_inline
void amhw_hc32f460_wdt_refresh_set (amhw_hc32f460_wdt_t             *p_hw_wdt,
                                    amhw_hc32f460_wdt_refresh_set_t  flag)
{
    p_hw_wdt->RR_f.RF = flag;
}

/**
 * @}
 */
/*
 * \brief 匿名结构体段的结束
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#elif defined(__TMS470__)
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif

#ifdef __cplusplus
}
#endif /* __AMHW_HC32F460_WDT_H */

#endif

/* end of file */
