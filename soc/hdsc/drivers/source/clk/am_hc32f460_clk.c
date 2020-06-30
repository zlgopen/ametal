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
 * \brief CLK implementation
 *
 * \internal
 * \par Modification history
 * - 1.00 19-12-23  cds, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_hc32f460_clk.h"
#include "am_hc32f460_flash.h"
#include "hw/amhw_hc32f460_rcc.h"

am_hc32f460_clk_dev_t    *__gp_clk_dev;


/* TBDs 1us based 168M frequency. */
#define CLK_FCG_STABLE                      ((uint16_t)0xB0u)
#define CLK_SYSCLK_STABLE                   ((uint16_t)0xB0u)
#define CLK_USBCLK_STABLE                   ((uint16_t)0xB0u)

#define CLK_FREQ_MRC    (8000000ul)
#define CLK_FREQ_LRC    (32768)
#define CLK_FREQ_SWDT   (10000)

/*! Parameter validity check for system clock config \a cfg. */
#define IS_SYSCLK_CONFIG_VALID(cfg)                                            \
(   ((cfg)->hclk_div           <=  ((cfg)->pclk1_div))    &&                 \
    ((cfg)->hclk_div           <=  ((cfg)->pclk3_div))    &&                 \
    ((cfg)->hclk_div           <=  ((cfg)->pclk4_div))    &&                 \
    ((cfg)->pclk0_div          <=  ((cfg)->pclk1_div))    &&                 \
    ((cfg)->pclk0_div          <=  ((cfg)->pclk3_div))    &&                 \
    (((cfg)->pclk2_div-(cfg)->pclk4_div   ==  3)          ||                 \
    ((cfg)->pclk2_div-(cfg)->pclk4_div    ==  2)          ||                 \
    ((cfg)->pclk2_div-(cfg)->pclk4_div    ==  1)          ||                 \
    ((cfg)->pclk2_div-(cfg)->pclk4_div    ==  0)          ||                 \
    ((cfg)->pclk4_div-(cfg)->pclk2_div    ==  1)          ||                 \
    ((cfg)->pclk4_div-(cfg)->pclk2_div    ==  2)          ||                 \
    ((cfg)->pclk4_div-(cfg)->pclk2_div    ==  3)))

extern uint32_t SystemCoreClock;



/**
 * \brief CLK 初始化
 */
int am_hc32f460_clk_init (am_hc32f460_clk_dev_t           *p_dev,
                          const am_hc32f460_clk_devinfo_t *p_devinfo)

{

    uint32_t value_fcg[4] = {0};
    __IO uint32_t timeout = 0ul;
    amhw_hc32f460_cmu_xtalcfg_t xtal_cfg;
    amhw_hc32f460_clk_pll_cfg_t mpll_cfg;
    amhw_hc32f460_clk_pll_cfg_t upll_cfg;
    amhw_hc32f460_flash_read_waittime efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_0;

    if (p_dev == NULL || p_devinfo == NULL) {
        return -AM_EINVAL;
    }

    p_dev->p_devinfo  = p_devinfo;
    __gp_clk_dev      = p_dev;

    /* 读取当前FCG寄存器的值 */
    amhw_hc32f460_clk_get_mstb_fcg(value_fcg);

    if (amhw_hc32f460_clk_get_sysclk_src() == AMHW_HC32F460_SYSCLK_MPLL) {
        /* 若当前系统时钟源为MPLL，则在配置系统时钟分频前，先复位所有外设的功能时钟 */
        amhw_hc32f460_clk_reset_mstb_fcg();

        /* 复位功能时钟后，等待稳定 */
        do
        {
            timeout++;
        }while(timeout < CLK_FCG_STABLE);
    }


    /* Set bus clk div. */
    if (IS_SYSCLK_CONFIG_VALID(p_devinfo)){
        amhw_hc32f460_sysclk_cfg((p_devinfo->hclk_div  << 24) |
                                 (p_devinfo->exclk_div << 20) |
                                 (p_devinfo->pclk0_div << 0)  |
                                 (p_devinfo->pclk1_div << 4)  |
                                 (p_devinfo->pclk2_div << 8)  |
                                 (p_devinfo->pclk3_div << 12) |
                                 (p_devinfo->pclk4_div << 16));

    } else {
        return AM_ERROR;
    }

    timeout = 0ul;
    /* 复位功能时钟后，等待稳定 */
    do
    {
        timeout++;
    }while(timeout < CLK_FCG_STABLE);

    amhw_hc32f460_clk_set_mstb_fcg(value_fcg);

    timeout = 0ul;
    /* 复位功能时钟后，等待稳定 */
    do
    {
        timeout++;
    }while(timeout < CLK_FCG_STABLE);

    if ((p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_XTAL) ||
        (p_devinfo->sysclk_src == AMHW_HC32F460_SYSCLK_XTAL)){
        /*初始化XTAL*/
        xtal_cfg.supdrv = AMHW_HC32F460_CMU_XTAL_SUPDRV_DISABLE;
        xtal_cfg.mode   = AMHW_HC32F460_CMU_XTAL_MODE_OSC;
        xtal_cfg.drv    = AMHW_HC32F460_CMU_XTAL_DRV_LOW;
        amhw_hc32f460_clk_xtal_cfg(xtal_cfg);
        amhw_hc32f460_clk_xtal_cfg_enable();
    }

    if ((p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_HRC) ||
        (p_devinfo->sysclk_src == AMHW_HC32F460_SYSCLK_HRC)){
        /*打开HRC*/
        amhw_hc32f460_clk_hrc_cfg_enable();
    }

    /*初始化MPLL*/
    mpll_cfg.PllpDiv = p_devinfo->mpllp_div;
    mpll_cfg.PllqDiv = p_devinfo->mpllq_div;
    mpll_cfg.PllrDiv = p_devinfo->mpllr_div;
    mpll_cfg.plln    = p_devinfo->mpll_mul;
    mpll_cfg.pllmDiv = p_devinfo->mpll_div;
    if (p_dev->p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_XTAL) {
        p_dev->pllin_clk = p_devinfo->xth_osc;
    } else if (p_dev->p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_HRC){
        p_dev->pllin_clk = p_devinfo->hrc_osc;
    }

    amhw_hc32f460_clk_pll_src_set(p_dev->p_devinfo->pllin_src);
    amhw_hc32f460_clk_mpll_cfg(mpll_cfg);

    amhw_hc32f460_clk_mpll_enable();
    p_dev->mpllout_clk = p_dev->pllin_clk / mpll_cfg.pllmDiv * mpll_cfg.plln;
    p_dev->mpllpout_clk = p_dev->pllin_clk / mpll_cfg.pllmDiv * mpll_cfg.plln / mpll_cfg.PllpDiv;
    p_dev->mpllqout_clk = p_dev->pllin_clk / mpll_cfg.pllmDiv * mpll_cfg.plln / mpll_cfg.PllqDiv;
    p_dev->mpllrout_clk = p_dev->pllin_clk / mpll_cfg.pllmDiv * mpll_cfg.plln / mpll_cfg.PllrDiv;

    /*初始化UPLL*/
    upll_cfg.PllpDiv = p_devinfo->upllp_div;
    upll_cfg.PllqDiv = p_devinfo->upllq_div;
    upll_cfg.PllrDiv = p_devinfo->upllr_div;
    upll_cfg.plln    = p_devinfo->upll_mul;
    upll_cfg.pllmDiv = p_devinfo->upll_div;

    amhw_hc32f460_clk_upll_cfg(upll_cfg);
    amhw_hc32f460_clk_upll_enable();
    p_dev->upllout_clk = p_dev->pllin_clk / upll_cfg.pllmDiv * upll_cfg.plln;
    p_dev->upllpout_clk = p_dev->pllin_clk * upll_cfg.pllmDiv / upll_cfg.PllpDiv;
    p_dev->upllqout_clk = p_dev->pllin_clk * upll_cfg.pllmDiv / upll_cfg.PllqDiv;
    p_dev->upllrout_clk = p_dev->pllin_clk * upll_cfg.pllmDiv / upll_cfg.PllrDiv;

    /* 计算系统时钟频率 */
    switch (p_devinfo->sysclk_src) {

    case AMHW_HC32F460_SYSCLK_HRC:
        p_dev->sys_clk = p_dev->p_devinfo->hrc_osc;
        break;
    case AMHW_HC32F460_SYSCLK_MRC:
            p_dev->sys_clk = CLK_FREQ_MRC;
            break;
    case AMHW_HC32F460_SYSCLK_LRC:
            p_dev->sys_clk = CLK_FREQ_LRC;
            break;
    case AMHW_HC32F460_SYSCLK_XTAL:
            p_dev->sys_clk = p_dev->p_devinfo->xth_osc;
            break;
    case AMHW_HC32F460_SYSCLK_XTAL32:
            p_dev->sys_clk = p_dev->p_devinfo->xtl_osc;
            break;
    case AMHW_HC32F460_SYSCLK_MPLL:
        if (p_dev->p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_XTAL) {
            p_dev->sys_clk = p_dev->p_devinfo->xth_osc / mpll_cfg.pllmDiv * mpll_cfg.plln / mpll_cfg.PllpDiv;
        } else if (p_dev->p_devinfo->pllin_src == AMHW_HC32F460_CLK_PLLSRC_HRC) {
            p_dev->sys_clk = p_dev->p_devinfo->hrc_osc / mpll_cfg.pllmDiv * mpll_cfg.plln / mpll_cfg.PllpDiv;
        }
        break;
    }

    if (p_dev->sys_clk > 132000000 && p_dev->sys_clk <= 168000000) {
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_4;
    } else if (p_dev->sys_clk > 99000000 && p_dev->sys_clk <= 132000000) {
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_3;
    } else if (p_dev->sys_clk > 66000000 && p_dev->sys_clk <= 99000000) {
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_2;
    } else if (p_dev->sys_clk > 33000000 && p_dev->sys_clk <= 66000000){
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_1;
    } else if (p_dev->sys_clk > 2000000 && p_dev->sys_clk <= 33000000){
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_0;
    } else if (p_dev->sys_clk <= 2000000) {
        efm_latency = AMHW_HC32F460_FLASH_READ_WAITTIME_0;
    }

    /* flash read wait cycle setting */
    am_hc32f460_flash_unlock(HC32F460_EFM);
    am_hc32f460_flash_waitcycle(HC32F460_EFM, efm_latency);
    am_hc32f460_flash_lock(HC32F460_EFM);

    /* 设置系统时钟源 */
    amhw_hc32f460_clk_set_sysclk_src(p_devinfo->sysclk_src);

    /* 设置USBCK时钟源 */
    amhw_hc32f460_clk_cmu_usbcks_set(p_devinfo->usbck_src);

    __gp_clk_dev->sys_type = p_devinfo->sysclk_src;
    __gp_clk_dev->hclk = p_dev->sys_clk / p_devinfo->hclk_div;
    __gp_clk_dev->pclk0 = p_dev->sys_clk / p_devinfo->pclk0_div;
    __gp_clk_dev->pclk1 = p_dev->sys_clk / p_devinfo->pclk1_div;
    __gp_clk_dev->pclk2 = p_dev->sys_clk / p_devinfo->pclk2_div;
    __gp_clk_dev->pclk3 = p_dev->sys_clk / p_devinfo->pclk3_div;
    __gp_clk_dev->pclk4 = p_dev->sys_clk / p_devinfo->pclk4_div;

    SystemCoreClock = p_dev->sys_clk;

    return AM_OK;
}


/**
 * \brief 获取时钟源频率
 */
uint32_t __hc32f460_clk_src_freq_get(am_clk_id_t clk_id)
{
    int clk = 0;
    /* 时钟源ID */
    switch (clk_id) {

    case CLK_XTAL:
        clk = __gp_clk_dev->p_devinfo->xth_osc;
        break;
    case CLK_XTAL32:
        clk = __gp_clk_dev->p_devinfo->xtl_osc;
        break;
    case CLK_HRC:
        clk = __gp_clk_dev->p_devinfo->hrc_osc;
        break;
    case CLK_MRC:
        clk = CLK_FREQ_MRC;
        break;
    case CLK_LRC:
        clk = CLK_FREQ_LRC;
        break;
    case CLK_SWDTRC:
        clk = CLK_FREQ_SWDT;
        break;
    case CLK_MPLL:
        clk = __gp_clk_dev->mpllout_clk;
        break;
    case CLK_MPLLP:
        clk = __gp_clk_dev->mpllpout_clk;
        break;
    case CLK_MPLLQ:
        clk = __gp_clk_dev->mpllqout_clk;
        break;
    case CLK_MPLLR:
        clk = __gp_clk_dev->mpllrout_clk;
        break;
    case CLK_UPLL:
        clk = __gp_clk_dev->upllout_clk;
        break;
    case CLK_UPLLP:
        clk = __gp_clk_dev->upllpout_clk;
        break;
    case CLK_UPLLQ:
        clk = __gp_clk_dev->upllqout_clk;
        break;
    case CLK_UPLLR:
        clk = __gp_clk_dev->upllrout_clk;
        break;
    case CLK_SYSCLK_DIV1:
        clk = __gp_clk_dev->sys_clk;
        break;
    case CLK_SYSCLK_DIV2:
        clk = __gp_clk_dev->sys_clk / 2;
        break;
    case CLK_SYSCLK_DIV3:
        clk = __gp_clk_dev->sys_clk / 3;
        break;
    case CLK_SYSCLK_DIV4:
        clk = __gp_clk_dev->sys_clk / 4;
        ;
        break;
    case CLK_SYSCLK_DIV8:
        clk = __gp_clk_dev->sys_clk / 8;
        break;
    case CLK_SYSCLK_DIV16:
        clk = __gp_clk_dev->sys_clk / 16;
        break;
    case CLK_SYSCLK_DIV32:
        clk = __gp_clk_dev->sys_clk / 32;
        break;
    case CLK_SYSCLK_DIV64:
        clk = __gp_clk_dev->sys_clk / 64;
        break;
    }
    return clk;

}

/**
 * \brief 获取工作时钟频率
 */
uint32_t __hc32f460_clk_work_freq_get(am_clk_id_t clk_id)
{
    int clk = 0;
    am_clk_id_t clk_src = 0;

    switch (clk_id) {

        case CLK_HCLK:
            clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_HCLK);
            break;
        case CLK_EXCLK:
            clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_EXCLK);
            break;
        case CLK_PCLK0:
            clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_PCLK0);
            break;
        case CLK_PCLK1:
             clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_PCLK1);
             break;
        case CLK_PCLK2:
             clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_PCLK2);
             break;
        case CLK_PCLK3:
             clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_PCLK3);
             break;
        case CLK_PCLK4:
             clk = __hc32f460_clk_src_freq_get(CLK_SYSCLK_DIV1) / amhw_hc32f460_clk_cmu_scfgr_div_get(CLK_PCLK4);
             break;
        case CLK_UCLK:
             clk_src = amhw_hc32f460_clk_cmu_usbcks_get();
             clk = __hc32f460_clk_src_freq_get(clk_src);
             break;
        case CLK_CANCLK:
            /*  ??? */
             break;
        case CLK_STICCLK:
             clk = __hc32f460_clk_src_freq_get(CLK_LRC);
             break;
        case CLK_SWDTCLK:
             clk = __hc32f460_clk_src_freq_get(CLK_SWDTRC);;
             break;
        case CLK_TCK:
             /* JTAG用时钟 */
             break;
        case CLK_TPIUCLK:
             /* 调试时钟 */
             clk_src = amhw_hc32f460_clk_cmu_tpiu_src_get();
             clk = __hc32f460_clk_src_freq_get(clk_src);
             break;
        case CLK_I2S1CLK:
            clk_src = amhw_hc32f460_clk_i2s_src_get(CLK_I2S1CLK);
            clk = __hc32f460_clk_src_freq_get(clk_src);
            break;
       case CLK_I2S2CLK:
            clk_src = amhw_hc32f460_clk_i2s_src_get(CLK_I2S2CLK);
            clk = __hc32f460_clk_src_freq_get(clk_src);
            break;
       case CLK_I2S3CLK:
            clk_src = amhw_hc32f460_clk_i2s_src_get(CLK_I2S3CLK);
            clk = __hc32f460_clk_src_freq_get(clk_src);
            break;
       case CLK_I2S4CLK:
            clk_src = amhw_hc32f460_clk_i2s_src_get(CLK_I2S4CLK);
            clk = __hc32f460_clk_src_freq_get(clk_src);
            break;
    }
    return clk;
}

/**
 * \brief 使能指定的系统或外设时钟
 */
int am_clk_enable (am_clk_id_t clk_id)
{
    int peri;

    peri = clk_id & 0xff;

    /* 判断外设时钟是否已经使能，否，使能时钟且复位时钟 */
    if(amhw_hc32f460_rcc_peripheral_enable_check((amhw_hc32f460_peripheral)peri)) {
        amhw_hc32f460_rcc_peripheral_enable((amhw_hc32f460_peripheral)peri);
    }

    return AM_OK;
}

/**
 * \brief 禁能指定的系统或外设时钟
 */
int am_clk_disable (am_clk_id_t clk_id)
{
    int peri;

    peri = clk_id & 0xff;

    /* 判断外设时钟是否已经使能，否，使能时钟且复位时钟 */
    if(amhw_hc32f460_rcc_peripheral_enable_check((amhw_hc32f460_peripheral)peri) == 0) {
        amhw_hc32f460_rcc_peripheral_disable((amhw_hc32f460_peripheral)peri);
    }

    return AM_OK;
}



/**
 * \brief 获取时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id)
{
    int clk = 0;

    if ((clk_id & 0xFFFFFFF0) == 0) {
        /* 时钟源ID */
        clk = __hc32f460_clk_src_freq_get(clk_id);

    } else if(clk_id & WORK_CLK_FLAG) {
        /* 工作时钟ID */
        clk = __hc32f460_clk_work_freq_get(clk_id);
    } else if (clk_id & 0x7FFFC000) {
        /*外设时钟*/
        if (clk_id & FROM_HCLK) {
            clk = __hc32f460_clk_work_freq_get(CLK_HCLK);
        } else if (clk_id & FROM_PCLK0){
            clk = __hc32f460_clk_work_freq_get(CLK_PCLK0);
        } else if (clk_id & FROM_PCLK1){
            clk = __hc32f460_clk_work_freq_get(CLK_PCLK1);
        } else if (clk_id & FROM_PCLK2){
            clk = __hc32f460_clk_work_freq_get(CLK_PCLK2);
        } else if (clk_id & FROM_PCLK3){
            clk = __hc32f460_clk_work_freq_get(CLK_PCLK3);
        } else if (clk_id & FROM_PCLK4){
            clk = __hc32f460_clk_work_freq_get(CLK_PCLK4);
        } else if (clk_id & FROM_EXCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_EXCLK);
        } else if (clk_id & FROM_UCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_UCLK);
        } else if (clk_id & FROM_CANCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_CANCLK);
        } else if (clk_id & FROM_STICCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_STICCLK);
        } else if (clk_id & FROM_SWDTCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_SWDTCLK);
        } else if (clk_id & FROM_TCK){
            clk = __hc32f460_clk_work_freq_get(CLK_TCK);
        } else if (clk_id & FROM_TPIUCLK){
            clk = __hc32f460_clk_work_freq_get(CLK_TPIUCLK);
        } else if (clk_id & FROM_I2S1CLK){
            clk = __hc32f460_clk_work_freq_get(CLK_I2S1CLK);
        } else if (clk_id & FROM_I2S2CLK){
            clk = __hc32f460_clk_work_freq_get(CLK_I2S2CLK);
        } else if (clk_id & FROM_I2S3CLK){
            clk = __hc32f460_clk_work_freq_get(CLK_I2S3CLK);
        } else if (clk_id & FROM_I2S4CLK){
            clk = __hc32f460_clk_work_freq_get(CLK_I2S4CLK);
        }
    }

    return clk;
}

/* end of file */
