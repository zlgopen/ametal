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
 * \brief WDT驱动层接口
 * \internal
 * \par Modification history
 * - 1.00 20-05-13
 * \endinternal
 */

#include "ametal.h"
#include "am_clk.h"
#include "am_int.h"
#include "am_hc32f460_wdt.h"
#include "hc32f460_intctrl.h"
#include "hc32f460_inum.h"
#include "hc32f460_irq_handle.h"

/****************************************************************************
 * Functions Declaration
 ***************************************************************************/

static int __wdt_feed (void     *p_drv);
static int __wdt_enable (void   *p_drv, uint32_t timeout);
static int __wdt_info_get (void *p_drv, am_wdt_info_t *p_info);

/** \brief 看门狗标准服务函数结构体 */
static const struct am_wdt_drv_funcs __g_wdt_drv_funcs = {
    __wdt_info_get,
    __wdt_enable,
    __wdt_feed
};

#define AMHW_HC32F460_WDT_COUNTER_MAX_CLK_DIV  (8192ULL)  /**< \brief 看门狗计数时钟最大分频 */
#define AMHW_HC32F460_WDT_COUNTER_MIN_CLK_DIV  (4UL)      /**< \brief 看门狗计数时钟最小分频 */
#define AMHW_HC32F460_WDT_COUNTER_MAX_PERIOD   (65536ULL) /**< \brief 看门狗最大计数周期 */
#define AMHW_HC32F460_WDT_COUNTER_MIN_PERIOD   (256UL)    /**< \brief 看门狗最小计数周期 */

/** \brief 获取看门狗信息 */
static int __wdt_info_get (void *p_drv, am_wdt_info_t *p_info)
{
    am_hc32f460_wdt_dev_t *p_dev     = (am_hc32f460_wdt_dev_t *)p_drv;

    uint32_t clkrate = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    p_info->min_timeout_ms = AMHW_HC32F460_WDT_COUNTER_MIN_PERIOD *
                             (AMHW_HC32F460_WDT_COUNTER_MIN_CLK_DIV / (float)clkrate) * 1000;
    p_info->max_timeout_ms = AMHW_HC32F460_WDT_COUNTER_MAX_PERIOD *
                             (AMHW_HC32F460_WDT_COUNTER_MAX_CLK_DIV / (float)clkrate) * 1000;

    return AM_OK;
}

/** \brief 喂狗操作 */
static int __wdt_feed (void *p_drv)
{
    am_hc32f460_wdt_dev_t *p_dev     = (am_hc32f460_wdt_dev_t *)p_drv;
    amhw_hc32f460_wdt_t   *p_hw_wdt =
                           (amhw_hc32f460_wdt_t *)p_dev->p_devinfo->wdt_regbase;

    /* 刷新计数值 */
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_START);
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_END);

    return AM_OK;
}

void hc32f460_wdt_irq_handler(void *p_arg)
{
    uint32_t flag = 0;

    am_hc32f460_wdt_dev_t *p_dev = (am_hc32f460_wdt_dev_t *)p_arg;

    amhw_hc32f460_wdt_t *p_hw_wdt =
        (amhw_hc32f460_wdt_t *) p_dev->p_devinfo->wdt_regbase;

    if (amhw_hc32f460_wdt_status_get(p_hw_wdt, AMHW_HC32F460_WDT_STATUS_REF)) {
        flag = AM_HC32F460_WDT_STATUS_REFRESH_ERROR;
        amhw_hc32f460_wdt_status_clr(p_hw_wdt, AMHW_HC32F460_WDT_STATUS_REF);
    } else if (amhw_hc32f460_wdt_status_get(p_hw_wdt, AMHW_HC32F460_WDT_STATUS_UDF)) {
        flag = AM_HC32F460_WDT_STATUS_COUNT_UNDERFLOW;
        amhw_hc32f460_wdt_status_clr(p_hw_wdt, AMHW_HC32F460_WDT_STATUS_UDF);
    }

    if (p_dev->pfn_isr) {
        p_dev->pfn_isr(p_arg , flag);
    }
}


/* 连接DMA中断服务函数 */
int am_hc32f460_wdt_isr_connect (am_hc32f460_wdt_dev_t     *p_dev,
                                 am_hc32f460_pfn_wdt_isr_t  pfn_isr,
                                 void                      *p_arg)
{
    if (p_dev-> pfn_isr == NULL) {
        p_dev-> pfn_isr = pfn_isr;
        p_dev-> p_arg   = p_arg;
    } else {
        return -AM_EPERM;
    }

    return AM_OK;
}

/* 删除WDT中断服务函数连接 */
int am_hc32f460_wdt_isr_disconnect (am_hc32f460_wdt_dev_t    *p_dev,
                                    am_hc32f460_pfn_wdt_isr_t pfn_isr,
                                    void                     *p_arg)
{
    if (p_dev-> pfn_isr != NULL) {
        p_dev-> pfn_isr = NULL;
        p_dev-> p_arg   = NULL;
    } else {
        return -AM_EPERM;
    }

    return AM_OK;
}



/** \brief 配置超时时间并启动看门狗 */
static int __wdt_enable (void *p_drv, uint32_t timeout_ms)
{
    am_hc32f460_wdt_dev_t *p_dev     = (am_hc32f460_wdt_dev_t *)p_drv;
    amhw_hc32f460_wdt_t   *p_hw_wdt =
                           (amhw_hc32f460_wdt_t *)p_dev->p_devinfo->wdt_regbase;

    uint32_t clk_rate = am_clk_rate_get(p_dev->p_devinfo->clk_num);

    if ((timeout_ms > AMHW_HC32F460_WDT_COUNTER_MAX_PERIOD *
                     (AMHW_HC32F460_WDT_COUNTER_MAX_CLK_DIV / (float)clk_rate) * 1000) ||
        (timeout_ms < AMHW_HC32F460_WDT_COUNTER_MIN_PERIOD *
                     (AMHW_HC32F460_WDT_COUNTER_MIN_CLK_DIV / (float)clk_rate) * 1000)) {
        return AM_ERROR;
    }

    uint8_t i ,j = 0;
    float temp_time = 0;
    float err_currunt = 0; /* 当前误差 */
    float err_min = 0;     /* 最小误差 */
    uint32_t temp_clk_div = 0;
    uint32_t temp_preiod_count = 0;
    uint8_t temp_clk_div_regval = 0;
    uint8_t temp_period_regval = 0;
    uint8_t clk_div_regval = 0;
    uint8_t period_regval = 0;
    uint32_t reg_cr_val = 0;

    for (i = 0; i < 8; i++) {

        switch (i) {

        case 0: temp_clk_div = 4;    temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_4;    break;
        case 1: temp_clk_div = 64;   temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_64;   break;
        case 2: temp_clk_div = 128;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_128;  break;
        case 3: temp_clk_div = 256;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_256;  break;
        case 4: temp_clk_div = 512;  temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_512;  break;
        case 5: temp_clk_div = 1024; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_1024; break;
        case 6: temp_clk_div = 2048; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_2048; break;
        case 7: temp_clk_div = 8192; temp_clk_div_regval = AMHW_HC32F460_WDT_COUNT_CLK_DIV_8192; break;
        }

        for (j = 0; j < 4; j++) {
            switch (j) {

            case 0: temp_preiod_count = 256;   temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_256;   break;
            case 1: temp_preiod_count = 4096;  temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_4096;  break;
            case 2: temp_preiod_count = 16384; temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_16384; break;
            case 3: temp_preiod_count = 65536; temp_period_regval = AMHW_HC32F460_WDT_COUNT_PERIOD_65536; break;
            }

            temp_time = temp_preiod_count * (temp_clk_div / (float)clk_rate) * 1000;
            err_currunt = temp_time > timeout_ms ? (temp_time - timeout_ms) : (timeout_ms - temp_time);

            if (i == 0 && j == 0) {
                err_min = err_currunt;
                clk_div_regval = temp_clk_div_regval;
                period_regval = temp_period_regval;
            } else {
               if (err_currunt < err_min) {
                   err_min = err_currunt;
                   clk_div_regval = temp_clk_div_regval;
                   period_regval = temp_period_regval;
               }
            }
        }
    }


    if (p_dev->p_devinfo->EVENT_TYPE == AM_HC32F460_WDT_TRIGGER_EVENT_REQUEST_INT) {
        reg_cr_val |= AMHW_HC32F460_WDT_TRIGGER_INTERRUPT_REQUEST;
    } else {
        reg_cr_val |= AMHW_HC32F460_WDT_TRIGGER_RESET_REQUEST;
    }

    if (p_dev->p_devinfo->SLPOFF == AM_HC32F460_WDT_SLEEPOFF) {
        reg_cr_val |= AMHW_HC32F460_WDT_SLEEPOFF;
    } else {
        reg_cr_val |= AMHW_HC32F460_WDT_SLEEPON;
    }

    reg_cr_val |= p_dev->p_devinfo->REFRESH_REGION;
    reg_cr_val |= (clk_div_regval << 4);
    reg_cr_val |= period_regval;


    amhw_hc32f460_wdt_cr_set(p_hw_wdt, reg_cr_val);

    /* 启动看门狗 */
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_START);
    amhw_hc32f460_wdt_refresh_set(p_hw_wdt, AMHW_HC32F460_WDT_REFRESH_END);

    return AM_OK;
}


#define  AMHW_HC32F460_WDT_INT_VSSEL_BITS_MASK  (1 << 23)
/** \brief 初始化WDT设备指针 */
am_wdt_handle_t
am_hc32f460_wdt_init (am_hc32f460_wdt_dev_t           *p_dev,
                      const am_hc32f460_wdt_devinfo_t *p_devinfo)
{
    if (p_devinfo == NULL) {
        return NULL;
    }

    p_dev->p_devinfo = p_devinfo;
    p_dev->wdt_serv.p_funcs = (struct am_wdt_drv_funcs *)&__g_wdt_drv_funcs;
    p_dev->wdt_serv.p_drv   = p_dev;
    p_dev->trigger_type = AM_HC32F460_WDT_TRIGGER_EVENT_REQUEST_RESET;
    p_dev->sleep_count  = AM_TRUE;

    /* 使能共享中断DMA相关位 */
    amhw_hc32f460_intc_int_vssel_bits_set(p_dev->p_devinfo->inum,
                                          AMHW_HC32F460_WDT_INT_VSSEL_BITS_MASK);

    am_int_connect(p_devinfo->inum, IRQ141_Handler, (void *)0);
    am_int_enable(p_devinfo->inum);

    /* MPU平台初始化 */
    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    return &(p_dev->wdt_serv);
}

/** \brief MPU解初始化 */
void am_hc32f460_wdt_deinit (am_wdt_handle_t handle)
{
    am_hc32f460_wdt_dev_t *p_dev = (am_hc32f460_wdt_dev_t *)handle;
    am_hc32f460_wdt_devinfo_t *p_devinfo = NULL;

    if (NULL == p_dev) {
        return ;
    }

    p_dev->p_devinfo        = NULL;
    p_dev->wdt_serv.p_funcs = NULL;
    p_dev->wdt_serv.p_drv   = NULL;
    p_devinfo = (am_hc32f460_wdt_devinfo_t *)p_dev->p_devinfo;

    /* 禁能共享中断DMA相关位 */
    amhw_hc32f460_intc_int_vssel_bits_clr(p_dev->p_devinfo->inum,
                                          AMHW_HC32F460_WDT_INT_VSSEL_BITS_MASK);

    am_int_disconnect(p_devinfo->inum, IRQ141_Handler, (void *)0);

    /* 若该中断号相关的所有中断源的共享中断都被禁能，关闭中断 */
    if (amhw_hc32f460_intc_int_vssel_get(p_devinfo->inum) == 0) {
        am_int_disable(p_devinfo->inum);
    }

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}

/* end of file */
