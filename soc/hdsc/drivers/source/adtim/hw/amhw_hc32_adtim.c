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
#include "hw/amhw_hc32_adtim.h"

/**
 * \brief 中断触发使能配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] p_cfg      : 指向中断使能配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32_adtim_irqtrigcfg(
              amhw_hc32_adtim_t              *p_hw_adtim,
              amhw_hc32_adtim_irq_trig_cfg_t *p_cfg) {

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    p_hw_adtim->cr |= (p_cfg->cntmatch_A_trig << 0);
    p_hw_adtim->cr |= (p_cfg->cntmatch_B_trig << 1);
    p_hw_adtim->cr |= (p_cfg->cntmatch_C_trig << 2);
    p_hw_adtim->cr |= (p_cfg->cntmatch_D_trig << 3);
    p_hw_adtim->cr |= (p_cfg->overflow_trig << 6);
    p_hw_adtim->cr |= (p_cfg->underflow_trig << 7);
    p_hw_adtim->cr |= (p_cfg->specilmatch_A_trig << 11);
    p_hw_adtim->cr |= (p_cfg->specilmatch_B_trig << 12);
    p_hw_adtim->cr |= (p_cfg->cntmatch_A_trigdma << 13);
    p_hw_adtim->cr |= (p_cfg->cntmatch_B_trigdma << 14);
    p_hw_adtim->cr |= (p_cfg->cntmatch_C_trigdma << 15);
    p_hw_adtim->cr |= (p_cfg->cntmatch_D_trigdma << 16);
    p_hw_adtim->cr |= (p_cfg->overflow_trigdma << 19);
    p_hw_adtim->cr |= (p_cfg->underflow_trigdma << 20);
    p_hw_adtim->cr |= (p_cfg->specilmatch_A_trigdma << 21);
    p_hw_adtim->cr |= (p_cfg->specilmatch_B_trigdma << 22);

    return AM_TRUE;
}

/**
 * \brief 端口触发配置
 *
 * \param[in] trigport : amhw_hc32_adtim_trig_port_t端口枚举量
 * \param[in] p_cfg    : 指向端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32_adtim_porttrigcfg(
              amhw_hc32_adtim_trig_port_t      trigport,
              amhw_hc32_adtim_port_trig_cfg_t *p_cfg) {

    amhw_hc32_adtim_t *p_hw_adtim4 = (amhw_hc32_adtim_t *)HC32_TIM4_BASE;

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    switch(trigport) {
    case AMHW_HC32_ADTIM_TRIG_PORT_A:
        p_hw_adtim4->ttrig = (p_hw_adtim4->ttrig & (~(0xful << 0))) |
                            (p_cfg->trigsrc << 0);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(1ul << 16))) |
                            (p_cfg->flt_enable << 16);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(3ul << 17))) |
                            (p_cfg->flt_enable << 17);
        break;
    case AMHW_HC32_ADTIM_TRIG_PORT_B:
        p_hw_adtim4->ttrig = (p_hw_adtim4->ttrig & (~(0xful << 4))) |
                            (p_cfg->trigsrc << 4);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(1ul << 20))) |
                            (p_cfg->flt_enable << 20);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(3ul << 21))) |
                            (p_cfg->flt_enable << 21);
        break;
    case AMHW_HC32_ADTIM_TRIG_PORT_C:
        p_hw_adtim4->ttrig = (p_hw_adtim4->ttrig & (~(0xful << 8))) |
                            (p_cfg->trigsrc << 8);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(1ul << 24))) |
                            (p_cfg->flt_enable << 24);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(3ul << 25))) |
                            (p_cfg->flt_enable << 25);
        break;
    case AMHW_HC32_ADTIM_TRIG_PORT_D:
        p_hw_adtim4->ttrig = (p_hw_adtim4->ttrig & (~(0xful << 12))) |
                            (p_cfg->trigsrc << 12);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(1ul << 28))) |
                            (p_cfg->flt_enable << 28);
        p_hw_adtim4->fconr = (p_hw_adtim4->fconr & (~(3ul << 29))) |
                            (p_cfg->flt_enable << 29);
        break;
    default:
        return AM_FALSE;
    }

    return AM_TRUE;
}

/**
 * \brief CHxX端口配置
 *
 * \param[in] p_hw_adtim : 指向ADTIM定时器寄存器块的指针
 * \param[in] port       : amhw_hc32_adtim_chx_t通道枚举量
 * \param[in] p_cfg      : 指向TIMxX端口配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32_adtim_timxchxportcfg(
              amhw_hc32_adtim_t                   *p_hw_adtim,
              amhw_hc32_adtim_chx_t                port,
              amhw_hc32_adtim_timx_chx_port_cfg_t *p_cfg) {

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    switch(port) {
    case AMHW_HC32_ADTIM_CHX_A:
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 0))) |
                            (p_cfg->portmode << 0);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 1))) |
                            (p_cfg->staout << 1);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 2))) |
                            (p_cfg->stpout << 2);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 3))) |
                            (p_cfg->stastps << 3);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 4))) |
                            (p_cfg->cmpc << 4);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 6))) |
                            (p_cfg->perc << 6);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 8))) |
                            (p_cfg->outenable << 8);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 9))) |
                            (p_cfg->dissel << 9);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 11))) |
                            (p_cfg->disval << 11);
        p_hw_adtim->fconr = (p_hw_adtim->fconr & (~(0x1ul << 0))) |
                            (p_cfg->fltenable << 0);
        p_hw_adtim->fconr = (p_hw_adtim->fconr & (~(0x3ul << 1))) |
                            (p_cfg->fltclk << 1);
        break;
    case AMHW_HC32_ADTIM_CHX_B:
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 16))) |
                            (p_cfg->portmode << 16);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 17))) |
                            (p_cfg->staout << 17);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 18))) |
                            (p_cfg->stpout << 18);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 19))) |
                            (p_cfg->stastps << 19);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 20))) |
                            (p_cfg->cmpc << 20);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 22))) |
                            (p_cfg->perc << 22);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x1ul << 24))) |
                            (p_cfg->outenable << 24);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 25))) |
                            (p_cfg->dissel << 25);
        p_hw_adtim->pconr = (p_hw_adtim->pconr & (~(0x3ul << 27))) |
                            (p_cfg->disval << 27);
        p_hw_adtim->fconr = (p_hw_adtim->fconr & (~(0x1ul << 4))) |
                            (p_cfg->fltenable << 4);
        p_hw_adtim->fconr = (p_hw_adtim->fconr & (~(0x3ul << 5))) |
                            (p_cfg->fltclk << 5);
        break;
    default:
        return AM_FALSE;
    }
    return AM_TRUE;
}

/**
 * \brief 无效条件3配置(端口刹车)
 *
 * \param[in] p_cfg      : 指向TADT无效条件3配置结构体的指针
 *
 * \return AM_FALSE ：设置失败
 *         AM_TRUE  ：设置成功
 */
am_bool_t amhw_hc32_adtim_disable3cfg(
              amhw_hc32_adtim_disable_3_cfg_t *p_cfg) {

    amhw_hc32_adtim_t *p_hw_adtim4 = (amhw_hc32_adtim_t *)HC32_TIM4_BASE;

    uint8_t i = 0;

    if(p_cfg == NULL) {
        return AM_FALSE;
    }

    amhw_hc32_adtim_clearbrakeport();

    for(i = 0; i <= AMHW_HC32_ADTIM_TRIG_PD5; i++) {

        if(AM_TRUE == p_cfg->stcbrkptcfg[i].portenable) {
            amhw_hc32_adtim_enablebrakeport(i, &(p_cfg->stcbrkptcfg[i]));
        }
    }

    p_hw_adtim4->aossr &= ~(1ul << 4);
    p_hw_adtim4->aossr &= ~(3ul << 2);

    p_hw_adtim4->aossr |= (p_cfg->fltenable << 4);
    p_hw_adtim4->aossr |= (p_cfg->fltclk << 2);

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
am_bool_t amhw_hc32_adtim_deinit(amhw_hc32_adtim_t *p_hw_adtim) {

    p_hw_adtim->gconr   &= ~(1ul << 0);
    p_hw_adtim->cnter    = 0;
    p_hw_adtim->pconr    = 0;
    p_hw_adtim->gconr    = 0x00000100;
    p_hw_adtim->dconr    = 0;
    p_hw_adtim->iconr    = 0;
    p_hw_adtim->bconr    = 0;
    p_hw_adtim->fconr    = 0;
    p_hw_adtim->vperr    = 0;
    p_hw_adtim->perar    = 0xFFFF;
    p_hw_adtim->perbr    = 0xFFFF;
    p_hw_adtim->gcmxr[0] = 0xFFFF;
    p_hw_adtim->gcmxr[1] = 0xFFFF;
    p_hw_adtim->gcmxr[2] = 0xFFFF;
    p_hw_adtim->gcmxr[3] = 0xFFFF;
    p_hw_adtim->dtdar    = 0xFFFF;
    p_hw_adtim->dtuar    = 0xFFFF;
    p_hw_adtim->hstar    = 0;
    p_hw_adtim->hstpr    = 0;
    p_hw_adtim->hcelr    = 0;
    p_hw_adtim->hcpar    = 0;
    p_hw_adtim->hcpbr    = 0;
    p_hw_adtim->hcupr    = 0;
    p_hw_adtim->hcdor    = 0;
    p_hw_adtim->sstar    = 0;
    p_hw_adtim->sstpr    = 0;
    p_hw_adtim->sclrr    = 0;
    p_hw_adtim->ifr      = 0;
    p_hw_adtim->cr       = 0x00000300;
    p_hw_adtim->aossr    = 0;
    p_hw_adtim->aoscl    = 0;
    p_hw_adtim->ptbks    = 0;
    p_hw_adtim->ptbkp    = 0;
    p_hw_adtim->ttrig    = 0;
    p_hw_adtim->itrig    = 0;

    return AM_TRUE;
}

/* end of file */
