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
 * \brief ADTIM操作接口
 *
 * \internal
 * \par Modification History
 * - 1.00 19-10-08  zp, first implementation
 * \endinternal
 */
#include "hw/amhw_hc32f460_adtim.h"

/**
 * \brief CHxX端口配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_hc32f460_adtim_chx_t通道枚举量
 * \param[in] p_cfg      : 指向TIMxX端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32f460_adtim_timxchxportcfg(
              amhw_hc32f460_adtim_t                   *p_hw_adtim,
              amhw_hc32f460_adtim_chx_t                port,
              amhw_hc32f460_adtim_timx_chx_port_cfg_t *p_cfg) {

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    switch(port) {
    case AMHW_HC32F460_ADTIM_CHX_A:
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 0))) |
                            (p_cfg->portmode << 0);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 1))) |
                            (p_cfg->staout << 1);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 2))) |
                            (p_cfg->stpout << 2);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 3))) |
                            (p_cfg->stastps << 3);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 4))) |
                            (p_cfg->cmpc << 4);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 6))) |
                            (p_cfg->perc << 6);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 8))) |
                            (p_cfg->outenable << 8);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 9))) |
                            (p_cfg->dissel << 9);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 11))) |
                            (p_cfg->disval << 11);
        p_hw_adtim->FCONR = (p_hw_adtim->FCONR & (~(0x1ul << 0))) |
                            (p_cfg->fltenable << 0);
        p_hw_adtim->FCONR = (p_hw_adtim->FCONR & (~(0x3ul << 1))) |
                            (p_cfg->fltclk << 1);
        break;
    case AMHW_HC32F460_ADTIM_CHX_B:
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 16))) |
                            (p_cfg->portmode << 16);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 17))) |
                            (p_cfg->staout << 17);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 18))) |
                            (p_cfg->stpout << 18);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 19))) |
                            (p_cfg->stastps << 19);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 20))) |
                            (p_cfg->cmpc << 20);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 22))) |
                            (p_cfg->perc << 22);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x1ul << 24))) |
                            (p_cfg->outenable << 24);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 25))) |
                            (p_cfg->dissel << 25);
        p_hw_adtim->PCONR = (p_hw_adtim->PCONR & (~(0x3ul << 27))) |
                            (p_cfg->disval << 27);
        p_hw_adtim->FCONR = (p_hw_adtim->FCONR & (~(0x1ul << 4))) |
                            (p_cfg->fltenable << 4);
        p_hw_adtim->FCONR = (p_hw_adtim->FCONR & (~(0x3ul << 5))) |
                            (p_cfg->fltclk << 5);
        break;
    default:
        return AM_FALSE;
    }
    return AM_TRUE;
}

/**
 * \brief ADTIM去初始化
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32f460_adtim_deinit(amhw_hc32f460_adtim_t *p_hw_adtim) {
    return AM_TRUE;
}

/* end of file */
