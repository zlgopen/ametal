/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2015 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/

/**
 * \file
 * \brief 系统控制接口
 *
 * 1. 时钟控制
 *    - 配置系统 PLL ；
 *    - 配置系统振荡器和看门狗振荡器；
 *    - 使能外设和内存时钟；
 *    - 配置时钟输出；
 *    - 配置时钟分频器，数字滤波器时钟和 USART 波特率时钟；
 * 2. 监视和复位外设；
 * 3. 选择引脚外部中断和模式匹配引擎；
 * 4. 配置低功耗模式；
 * 5. 唤醒控制；
 * 6. 掉电检测（BOD）配置；
 * 7. MTB 跟踪启动和停止；
 * 8. 中断延时控制；
 * 9. 选择 NMI 源；
 * 10. 校准系统时钟定时器。
 *
 * \internal
 * \par History
 * - 1.02 15-07-17  win, modify the file brief.
 * - 1.01 15-07-13  zxl, modified.
 * - 1.00 15-06-21  jon, first implementation.
 * \endinternal
 */

#ifndef __AMHW_LPC82X_SYSCON_H
#define __AMHW_LPC82X_SYSCON_H

#include "ametal.h"
#include "am_lpc82x.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup amhw_lpc82x_if_syscon
 * \copydoc amhw_lpc82x_syscon.h
 * @{
 */

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_lpc82x_syscon {
    __IO uint32_t sysmemremap;     /**< \brief 系统内存重定向寄存器          */
    __IO uint32_t presetctrl;      /**< \brief 外设复位控制                  */
    __IO uint32_t syspllctrl;      /**< \brief PLL控制寄存器                 */
    __IO uint32_t syspllstat;      /**< \brief PLL状态寄存器                 */
    __I  uint32_t reserved0[4];    /**< \brief 保留位                        */
    __IO uint32_t sysoscctrl;      /**< \brief 系统时钟源控制寄存器          */
    __IO uint32_t wdtoscctrl;      /**< \brief 看门狗时钟源控制寄存器        */
    __IO uint32_t ircctrl;         /**< \brief IRC控制寄存器(仅在LPC82X有效) */
    __I  uint32_t reserved1[1];    /**< \brief 保留位                        */
    __IO uint32_t sysrststat;      /**< \brief 系统复位状态寄存器            */
    __I  uint32_t reserved2[3];    /**< \brief 保留位                        */
    __IO uint32_t syspllclksel;    /**< \brief 系统时钟源选择寄存器          */
    __IO uint32_t syspllclkuen;    /**< \brief 系统PLL时钟更新使能寄存器     */
    __I  uint32_t reserved3[10];   /**< \brief 保留位                        */
    __IO uint32_t mainclksel;      /**< \brief 主时钟源选择                  */
    __IO uint32_t mainclkuen;      /**< \brief 主时钟更新使能                */
    __IO uint32_t sysahbclkdiv;    /**< \brief 系统AHB时钟分频               */
    __I  uint32_t reserved4[1];    /**< \brief 保留位                        */
    __IO uint32_t sysahbclkctrl;   /**< \brief 系统AHB时钟控制寄存器         */
    __I  uint32_t reserved5[4];    /**< \brief 保留位                        */
    __IO uint32_t uartclkdiv;      /**< \brief 串口时钟分频器                */
    __I  uint32_t reserved6[18];   /**< \brief 保留位                        */
    __IO uint32_t clkoutsel;       /**< \brief CLKOUT时钟源选择              */
    __IO uint32_t clkoutuen;       /**< \brief CLKOUT时钟更新使能            */
    __IO uint32_t clkoutdiv;       /**< \brief CLKOUT 时钟分频寄存器         */
    __I  uint32_t reserved7;       /**< \brief 保留位                        */
    __IO uint32_t uartfrgdiv;      /**< \brief 串口小数时钟分频器（SUB）     */
    __IO uint32_t uartfrgmult;     /**< \brief 串口小数时钟分频器（ADD）)    */
    __I  uint32_t reserved8[1];    /**< \brief 保留位                        */
    __IO uint32_t exttracecmd;     /**< \brief 外部追踪BUFFER命令寄存器      */
    __IO uint32_t pioporcap0;      /**< \brief 捕获ＩＯ状态                  */
    __I  uint32_t reserved9[12];   /**< \brief 保留位                        */
    __IO uint32_t ioconclkdiv[7];  /**< \brief ICON 时钟分频                 */
    __IO uint32_t bodctrl;         /**< \brief BOD控制寄存器                 */
    __IO uint32_t systckcal;       /**< \brief 系统滴答计数器刻度定义        */
    __I  uint32_t reserved10[6];   /**< \brief 保留位                        */
    __IO uint32_t irqlatency;      /**< \brief IRQ延时寄存器                 */
    __IO uint32_t nmisrc;          /**< \brief NMI控制寄存器                 */
    __IO uint32_t pintsel[8];      /**< \brief 引脚中断选择寄存器            */
    __I  uint32_t reserved11[27];  /**< \brief 保留位                        */
    __IO uint32_t starterp0;       /**< \brief 使能唤醒寄存器0               */
    __I  uint32_t reserved12[3];   /**< \brief 保留位                        */
    __IO uint32_t starterp1;       /**< \brief 使能唤醒寄存器01              */
    __I  uint32_t reserved13[6];   /**< \brief 保留位                        */
    __IO uint32_t pdsleepcfg;      /**< \brief 进入深度睡眠配置寄存器        */
    __IO uint32_t pdawakecfg;      /**< \brief 唤醒配置寄存器                */
    __IO uint32_t pdruncfg;        /**< \brief 掉电配置寄存器                */
    __I  uint32_t reserved14[111]; /**< \brief 保留位                        */
    __I  uint32_t deviceid;        /**< \brief 设备ID                        */
} amhw_lpc82x_syscon_t;

/**
 * \brief 系统内存重映射，用于重映射中断向量表
 */
typedef enum amhw_lpc82x_syscon_bootmode_remap {

    /** \brief bootlaoder Mode 中断向量表重映射至启动ROM中 */
    AM_SYSYCON_REMAP_BOOTROM_MODE = 0,

    /** \brief User RAM Mode 中断向量表重映射至SRAM中 */
    AM_SYSYCON_REMAP_SRAM_MODE,

    /** \brief User Flash Mode 中断向量不重映射，存在于FLASH中 */
    AM_SYSYCON_REMAP_FLASH_MODE
} amhw_lpc82x_syscon_bootmode_remap_t;

/**
 * \brief 重映射中断向量表
 *
 * \param[in] remap : 选择重映射至何种内存中
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_remap_set (amhw_lpc82x_syscon_bootmode_remap_t remap)
{
    LPC82X_SYSCON->sysmemremap = (uint32_t)remap;
}

/**
 * \brief 获取中断向量表的重映射值
 *
 * \return 中断向量表的重映射值
 */
am_static_inline
amhw_lpc82x_syscon_bootmode_remap_t amhw_lpc82x_syscon_remap_get (void)
{
    return (amhw_lpc82x_syscon_bootmode_remap_t)LPC82X_SYSCON->sysmemremap;
}

/**
 * \brief 设置Systick的校准值
 *
 * \param[in] systickcal_val : Systick的校准值
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_systickcal_set (uint32_t systickcal_val)
{
    LPC82X_SYSCON->systckcal = systickcal_val;
}


/**
 * \brief 得到Systick的校准值
 *
 * \return Systick的校准值
 */
am_static_inline
uint32_t amhw_lpc82x_syscon_systickcal_get (void)
{
    return LPC82X_SYSCON->systckcal;
}

/**
 * \name 系统复位状态值
 * \anchor grp_amhw_lpc82x_syscon_rststat
 * @{
 */

#define AMHW_LPC82X_SYSCON_RSTSTAT_POR    AM_BIT(0) /**< \brief 上电复位     */
#define AMHW_LPC82X_SYSCON_RSTSTAT_EXTRST AM_BIT(1) /**< \brief 外部管脚复位 */
#define AMHW_LPC82X_SYSCON_RSTSTAT_WDT    AM_BIT(2) /**< \brief 看门狗复位   */
#define AMHW_LPC82X_SYSCON_RSTSTAT_BOD    AM_BIT(3) /**< \brief 掉电检测复位 */
#define AMHW_LPC82X_SYSCON_RSTSTAT_SYSRST AM_BIT(4) /**< \brief 软件复位     */

/** @} */

/**
 * \brief 获取系统复位状态
 *
 * \return 多个AMHW_LPC82X_SYSCON_RSTSTAT_*宏的或（OR）值，
 *         参见 \ref grp_amhw_lpc82x_syscon_rststat
 */
am_static_inline
uint32_t amhw_lpc82x_syscon_rst_stat_get (void)
{
    return LPC82X_SYSCON->sysrststat;
}

/**
 * \brief 清除系统复位状态
 *
 * \param[in] reset : 多个AMHW_LPC82X_SYSCON_RSTSTAT_*宏的或（OR）值，
 *                    参见 \ref grp_amhw_lpc82x_syscon_rststat
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_rst_stat_clr (uint32_t reset)
{
    LPC82X_SYSCON->sysrststat = reset;
}

/**
 * \brief 用于外设复位的枚举值
 */
typedef enum amhw_lpc82x_syscon_periph_reset {
    AMHW_LPC82X_RESET_SPI0 = 0,           /**< \brief SPI0         */
    AMHW_LPC82X_RESET_SPI1,               /**< \brief SPI1         */
    AMHW_LPC82X_RESET_UARTFRG ,           /**< \brief UARTFRG      */
    AMHW_LPC82X_RESET_UART0,              /**< \brief UART0        */
    AMHW_LPC82X_RESET_UART1,              /**< \brief UART1        */
    AMHW_LPC82X_RESET_UART2,              /**< \brief UART2        */
    AMHW_LPC82X_RESET_I2C0,               /**< \brief I2C0         */
    AMHW_LPC82X_RESET_MRT,                /**< \brief MRT          */
    AMHW_LPC82X_RESET_SCT,                /**< \brief SCT          */
    AMHW_LPC82X_RESET_WKT,                /**< \brief WKT          */
    AMHW_LPC82X_RESET_GPIO,               /**< \brief GPIO         */
    AMHW_LPC82X_RESET_FLASH,              /**< \brief FLASH        */
    AMHW_LPC82X_RESET_ACMP,               /**< \brief ACMP         */
    AMHW_LPC82X_RESET_I2C1 = 14,          /**< \brief I2C1         */
    AMHW_LPC82X_RESET_I2C2,               /**< \brief I2C2         */
    AMHW_LPC82X_RESET_I2C3,               /**< \brief I2C3         */
    AMHW_LPC82X_RESET_ADC = 24,           /**< \brief ADC          */
    AMHW_LPC82X_RESET_DMA = 29            /**< \brief DMA          */
} amhw_lpc82x_syscon_periph_reset_t;

/**
 * \brief 复位一个外设
 *
 * \param[in] periph : 需要复位的外设
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_periph_reset (amhw_lpc82x_syscon_periph_reset_t periph)
{
    AM_BIT_CLR(LPC82X_SYSCON->presetctrl, periph);
    AM_BIT_SET(LPC82X_SYSCON->presetctrl, periph);
}

/**
 * \brief FLASH访问时间定义
 */
typedef enum amhw_lpc82x_syscon_flashtim {
    AMHW_LPC82X_SYSCON_FLASH_1CYCLE = 0, /**< \brief Flash 访问使用1个CPU时钟 */
    AMHW_LPC82X_SYSCON_FLASH_2CYCLE      /**< \brief Flash 访问使用2个CPU时钟 */
} amhw_lpc82x_syscon_flashtim_t;

/**
 * \brief 设置系统中断延时
 *
 * \param[in] latency : 延时时钟个数
 *
 * \return    无
 *
 * \note   延时时钟数在0-255个时钟之间，小的值允许更好的延迟
 *
 */
am_static_inline
void amhw_lpc82x_syscon_setirqlatency(uint32_t latency)
{
    LPC82X_SYSCON->irqlatency = latency;
}

/**
 * \brief 获取系统中断延时值
 *
 * \return 系统中断延时时钟个数
 */
am_static_inline
uint32_t amhw_lpc82x_syscon_getirqlatency(void)
{
    return (LPC82X_SYSCON->irqlatency & 0xFF);
}

/**
 * \brief 设置不可屏蔽中断源
 *
 * \param[in] intsrc : 用于NMI的中断号（IRQ number）
 *
 * \return 无
 *
 * \note NMI中断源此时还是处于禁止状态的，配置中断源后应使用
 *       amhw_lpc82x_syscon_nmisrc_enable()函数使能用于NMI中断源
 */
am_static_inline
void amhw_lpc82x_syscon_nmisrc_set(uint32_t intsrc)
{
    /* 先清除NMI使能位，后写入时钟源 */
    AM_BIT_CLR(LPC82X_SYSCON->nmisrc, 31);
    LPC82X_SYSCON->nmisrc  = intsrc;
}

/**
 * \brief 使能NMI中断源的中断
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_nmisrc_enable (void)
{
    AM_BIT_SET(LPC82X_SYSCON->nmisrc, 31);
}

/**
 * \brief 禁能NMI中断源的中断
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_nmisrc_disable (void)
{
    AM_BIT_CLR(LPC82X_SYSCON->nmisrc, 31);
}

/**
 * \brief 枚举值，用于使能或者禁能外设中断的唤醒源
 *  
 *  如果使能了某外设的唤醒源，则该中断能够将芯片从深度睡眠模式和掉电模式唤醒
 */
typedef enum amhw_lpc82x_syscon_wakeup {
    AMHW_LPC82X_SYSCON_STARTER_PINT0,           /**< \brief PINT0      */
    AMHW_LPC82X_SYSCON_STARTER_PINT1,           /**< \brief PINT1      */
    AMHW_LPC82X_SYSCON_STARTER_PINT2,           /**< \brief PINT2      */
    AMHW_LPC82X_SYSCON_STARTER_PINT3,           /**< \brief PINT3      */
    AMHW_LPC82X_SYSCON_STARTER_PINT4,           /**< \brief PINT4      */
    AMHW_LPC82X_SYSCON_STARTER_PINT5,           /**< \brief PINT5      */
    AMHW_LPC82X_SYSCON_STARTER_PINT6,           /**< \brief PINT6      */
    AMHW_LPC82X_SYSCON_STARTER_PINT7,           /**< \brief PINT7      */
    AMHW_LPC82X_SYSCON_STARTER_SPI0  = 32 + 0,  /**< \brief SPI0       */
    AMHW_LPC82X_SYSCON_STARTER_SPI1,            /**< \brief SPI1       */
    AMHW_LPC82X_SYSCON_STARTER_UART0 = 32 + 3,  /**< \brief UART0      */
    AMHW_LPC82X_SYSCON_STARTER_UART1,           /**< \brief UART1      */
    AMHW_LPC82X_SYSCON_STARTER_UART2,           /**< \brief UART2      */
    AMHW_LPC82X_SYSCON_STARTER_I2C1  = 32 + 7,  /**< \brief I2C1       */
    AMHW_LPC82X_SYSCON_STARTER_I2C0,            /**< \brief I2C0       */
    AMHW_LPC82X_SYSCON_STARTER_WWDT  = 32 + 12, /**< \brief WWDT       */
    AMHW_LPC82X_SYSCON_STARTER_BOD,             /**< \brief BOD        */
    AMHW_LPC82X_SYSCON_STARTER_WKT   = 32 + 15, /**< \brief WKT        */
    AMHW_LPC82X_SYSCON_STARTER_I2C2  = 32 + 21, /**< \brief I2C2       */
    AMHW_LPC82X_SYSCON_STARTER_I2C3             /**< \brief I2C3       */
} amhw_lpc82x_syscon_wakeup_t;

/**
 * \brief 使能外设的唤醒源
 *
 * \param[in] pid : 外设中断的唤醒源
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_wakeup_enable (amhw_lpc82x_syscon_wakeup_t pid)
{
    if (pid < 32) {
        AM_BIT_SET(LPC82X_SYSCON->starterp0, pid);
    } else {
        AM_BIT_SET(LPC82X_SYSCON->starterp1, pid - 32);
    }
}

/**
 * \brief 禁能外设的唤醒源
 *
 * \param[in] pid : 外设中断的唤醒源
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_wakeup_disable (amhw_lpc82x_syscon_wakeup_t pid)
{
    if (pid < 32) {
        AM_BIT_CLR(LPC82X_SYSCON->starterp0, pid);
    } else {
        AM_BIT_CLR(LPC82X_SYSCON->starterp1, pid - 32);
    }
}

/**
 * \name 电源控制位定义(0 = powered, 1 = powered down)
 * \anchor grp_amhw_lpc82x_syscon_pd
 * @{
 */

#define AMHW_LPC82X_SYSCON_PD_IRC_OUT AM_BIT(0)  /**< \brief IRC 振荡器输出   */
#define AMHW_LPC82X_SYSCON_PD_IRC     AM_BIT(1)  /**< \brief IRC 振荡器       */
#define AMHW_LPC82X_SYSCON_PD_FLASH   AM_BIT(2)  /**< \brief Flash            */
#define AMHW_LPC82X_SYSCON_PD_BOD     AM_BIT(3)  /**< \brief 掉电检测         */
#define AMHW_LPC82X_SYSCON_PD_ADC0    AM_BIT(4)  /**< \brief ADC              */
#define AMHW_LPC82X_SYSCON_PD_SYS_OSC AM_BIT(5)  /**< \brief 系统晶振电源     */
#define AMHW_LPC82X_SYSCON_PD_WDT_OSC AM_BIT(6)  /**< \brief 看门狗振荡器电源 */
#define AMHW_LPC82X_SYSCON_PD_SYS_PLL AM_BIT(7)  /**< \brief SYS PLL          */
#define AMHW_LPC82X_SYSCON_PD_ACMP    AM_BIT(15) /**< \brief 模拟比较器掉电   */

/** @} */

/**
 * \brief 在Deep_sleep模式下器电源使能
 *
 * \param[in] flags ：AMHW_LPC82X_SYSCON_PD_BOD 或 AMHW_LPC82X_SYSCON_PD_WDT_OSC
 *
 * \return 无
 *
 * \note Deep_sleep模式下，只允许BOD和WDT工作
 */
am_static_inline
void amhw_lpc82x_syscon_deepsleep_enable (uint32_t flags)
{
    AM_BIT_CLR_MASK(LPC82X_SYSCON->pdsleepcfg, flags);
}

/**
 * \brief 在Deep_sleep模式下器电源禁能
 *
 * \param[in] flags ：AMHW_LPC82X_SYSCON_PD_BOD 或 AMHW_LPC82X_SYSCON_PD_WDT_OSC
 *
 * \return 无
 *
 * \note Deep_sleep模式下，只允许BOD和WDT工作
 */
am_static_inline
void amhw_lpc82x_syscon_deepsleep_disable (uint32_t flags)
{
    AM_BIT_SET_MASK(LPC82X_SYSCON->pdsleepcfg, flags);
}

/**
 * \brief 唤醒时可控制器件的电源配置
 *
 * \param[in] flags : 可控制器件的电源配置情况 #amhw_lpc82x_syscon_powerstat_get()
 *
 * \return 无
 * 
 * \note 睡眠前先使用 amhw_lpc82x_syscon_powerstat_get()获取期间状态，
 *       唤醒时再使用改函数唤醒之前的状态
 */
am_static_inline
void amhw_lpc82x_syscon_deepwakeup_cfg (uint32_t flags)
{
    LPC82X_SYSCON->pdawakecfg = flags;
}

/**
 * \brief deep 唤醒时，外设上电使能
 *
 * \param[in] wakeupflags : AMHW_LPC82X_SYSCON_PD_* 宏值或多个 宏的 或（OR）值，
 *                          参见 \ref grp_amhw_lpc82x_syscon_pd
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_deepwakeup_enable (uint32_t wakeupflags)
{
    AM_BIT_CLR_MASK(LPC82X_SYSCON->pdawakecfg, wakeupflags);
}

/**
 * \brief deep 唤醒时，外设上电禁能
 *
 * \param[in] wakeupflags : AMHW_LPC82X_SYSCON_PD_* 宏值或多个宏的 或（OR）值，
 *                          参见 \ref grp_amhw_lpc82x_syscon_pd
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_deepwakeup_disable (uint32_t wakeupflags)
{
    AM_BIT_SET_MASK(LPC82X_SYSCON->pdawakecfg, wakeupflags);
}

/**
 * \brief 得到当前的电源控制寄存器状态
 *
 * \return  AMHW_LPC82X_SYSCON_PD_* 宏值或多个宏的 或（OR）值，
 *          参见 \ref grp_amhw_lpc82x_syscon_pd
 *
 * \note  相应位为高电平表明该外设未供电，低电平表明供电
 */
am_static_inline
uint32_t amhw_lpc82x_syscon_powerstat_get (void)
{
    return LPC82X_SYSCON->pdruncfg;
}

/**
 * \brief 关闭外设电源
 *
 * \param[in] powerdownmask ：AMHW_LPC82X_SYSCON_PD_* 宏值或多个宏的或（OR）值,
 *                            参见 \ref grp_amhw_lpc82x_syscon_pd
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_powerdown (uint32_t powerdownmask)
{
    AM_BIT_SET_MASK(LPC82X_SYSCON->pdruncfg, powerdownmask);
}

/**
 * \brief 打开外设电源    
 *
 * \param[in] powerupmask ：AMHW_LPC82X_SYSCON_PD_* 宏值或多个宏的或（OR）值,
 *                          参见 \ref grp_amhw_lpc82x_syscon_pd
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_powerup (uint32_t powerupmask)
{
    AM_BIT_CLR_MASK(LPC82X_SYSCON->pdruncfg, powerupmask);
}

/**
 * \brief 检测指定外设是否上电
 *
 * \param[in] powermask : AMHW_LPC82X_SYSCON_PD_* 宏值，
 *                        参见 \ref grp_amhw_lpc82x_syscon_pd
 *
 * \retval    AM_TRUE      : 外设已上电
 * \retval     AM_FALSE     : 外设未上电
 */
am_static_inline
am_bool_t amhw_lpc82x_syscon_power_check (uint32_t powermask)
{
    return (am_bool_t)((LPC82X_SYSCON->pdruncfg & powermask) == 0);
}

/**
 * \brief 获取设备ID 0
 *
 * \return 设备ID 0
 */
am_static_inline
uint32_t amhw_lpc82x_syscon_deviceid0_get (void)
{
    return LPC82X_SYSCON->deviceid;
}


/**
 * \brief 引脚中断通道选择引脚
 *
 * \param[in] pint_sel : 中断通道编号（#AMHW_LPC82X_PINT_CHAN_0）
 * \param[in] pin      : 引脚编号(#PIO0_0)
 *
 * \return  无
 */
am_static_inline
void amhw_lpc82x_syscon_pint_sel (int pint_sel, int pin)
{                         
    LPC82X_SYSCON->pintsel[pint_sel] = pin;
}



/**
 * \brief 掉电检测复位的电源电压等级
 */
typedef enum amhw_lpc82x_syscon_bod_rst_level {
    AMHW_LPC82X_SYSCON_BODRSTLVL_0,                /**< \brief 等级0，1.46V */
    AMHW_LPC82X_SYSCON_BODRSTLVL_1_46V = AMHW_LPC82X_SYSCON_BODRSTLVL_0,
    AMHW_LPC82X_SYSCON_BODRSTLVL_1,                /**< \brief 等级1，2.05V */
    AMHW_LPC82X_SYSCON_BODRSTLVL_2_05V = AMHW_LPC82X_SYSCON_BODRSTLVL_1,
    AMHW_LPC82X_SYSCON_BODRSTLVL_2,                /**< \brief 等级2，2.34V */
    AMHW_LPC82X_SYSCON_BODRSTLVL_2_34V = AMHW_LPC82X_SYSCON_BODRSTLVL_2,
    AMHW_LPC82X_SYSCON_BODRSTLVL_3,                /**< \brief 等级3，2.63V */
    AMHW_LPC82X_SYSCON_BODRSTLVL_2_63V = AMHW_LPC82X_SYSCON_BODRSTLVL_3
} amhw_lpc82x_syscon_bod_rst_level_t;


/**
 * \brief 掉电检测中断的电源电压等级
 */
typedef enum amHW_syscon_bod_int_level {
    AMHW_LPC82X_SYSCON_BODINTLVL_0,                /**< \brief 等级0，保留 */
    AMHW_LPC82X_SYSCON_BODINTLVL_1,                /**< \brief 等级1，2.25V */
    AMHW_LPC82X_SYSCON_BODINTLVL_2_25V = AMHW_LPC82X_SYSCON_BODINTLVL_1,
    AMHW_LPC82X_SYSCON_BODINTLVL_2,                /**< \brief 等级2，2.54V */
    AMHW_LPC82X_SYSCON_BODINTLVL_2_54V = AMHW_LPC82X_SYSCON_BODINTLVL_2,
    AMHW_LPC82X_SYSCON_BODINTLVL_3,                /**< \brief 等级3，2.85V */
    AMHW_LPC82X_SYSCON_BODINTLVL_2_85V = AMHW_LPC82X_SYSCON_BODINTLVL_3
} amhw_lpc82x_syscon_bod_int_level_t;


/**
 * \brief 设置掉电检测中断和掉电检测复位的电源电压等级
 *
 * \param[in] rstlvl : 掉电检测复位电压等级
 * \param[in] intlvl : 掉电检测中断电压等级
 *
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_bod_level_set (amhw_lpc82x_syscon_bod_rst_level_t rstlvl,
                                       amhw_lpc82x_syscon_bod_int_level_t intlvl)
{
    LPC82X_SYSCON->bodctrl = ((uint32_t)rstlvl) | AM_SBF(intlvl, 2);
}

/**
 * \brief 使能掉电检测复位
 * \param[in] : 无
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_bod_rst_enable (void)
{
    AM_BIT_SET(LPC82X_SYSCON->bodctrl, 4);
}

/**
 * \brief 禁能掉电检测复位
 * \param[in] : 无
 * \return 无
 */
am_static_inline
void amhw_lpc82x_syscon_bod_rst_disable (void)
{
    AM_BIT_CLR(LPC82X_SYSCON->bodctrl, 4);
}


/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_LPC82X_SYSCON_H */

/* end of file */
