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
 * \brief 引脚中断硬件接口实现。
 * 
 * \internal
 * \par Modification History
 * - 1.01 15-11-24  mem, modified.
 * - 1.00 15-07-10  zxl, first implementation.
 * \endinternal
 */

#include "ametal.h"
#include "hw/amhw_lpc82x_pint.h"

/******************************************************************************
* 公共函数
*******************************************************************************/

/**
 * \brief 触发形式选择
 */
void amhw_lpc82x_pint_trigger_set (amhw_lpc82x_pint_t          *p_hw_pint,
                                   uint8_t                      chan,
                                   amhw_lpc82x_pint_trigger_t   mode)
{

    switch (mode) {
        
    case AMHW_LPC82X_PINT_TRIGGER_HIGH:      /* 高电平触发 */
        AM_BIT_SET(p_hw_pint->isel, chan);        
        p_hw_pint->sienr = AM_BIT(chan);          
        p_hw_pint->sienf = AM_BIT(chan);          
        break;                                    
                                                  
    case AMHW_LPC82X_PINT_TRIGGER_LOW:        /* 低电平触发 */
        AM_BIT_SET(p_hw_pint->isel, chan);        
        p_hw_pint->sienr = AM_BIT(chan);          
        p_hw_pint->cienf = AM_BIT(chan);          
        break;                                    
                                                  
    case AMHW_LPC82X_PINT_TRIGGER_RISE:       /* 上升沿触发 */
        AM_BIT_CLR(p_hw_pint->isel, chan);        
        p_hw_pint->sienr = AM_BIT(chan);          
        p_hw_pint->cienf = AM_BIT(chan);          
        break;                                    
                                                  
    case AMHW_LPC82X_PINT_TRIGGER_FALL:       /* 下降沿触发 */
        AM_BIT_CLR(p_hw_pint->isel,  chan);       
        p_hw_pint->sienf = AM_BIT(chan);          
        p_hw_pint->cienr = AM_BIT(chan);          
        break;                                    
                                                  
    case AMHW_LPC82X_PINT_TRIGGER_BOTH_EDGES: /* 双边沿触发 */
        AM_BIT_CLR(p_hw_pint->isel, chan);        
        p_hw_pint->sienr = AM_BIT(chan);          
        p_hw_pint->sienf = AM_BIT(chan);          
        break;                                    
                                                  
    case AMHW_LPC82X_PINT_TRIGGER_OFF:       /* 关闭触发    */
        p_hw_pint->cienr = AM_BIT(chan);
        p_hw_pint->cienf = AM_BIT(chan);
        break;
    
    default:
        break;
    }
}

/**
 * \brief 获取通道的当前触发模式
 */
amhw_lpc82x_pint_trigger_t 
amhw_lpc82x_pint_trigger_get (amhw_lpc82x_pint_t *p_hw_pint, 
                              uint8_t             chan)
{
    uint8_t is_level = AM_BIT_GET(p_hw_pint->isel, chan);
    
    /* 电平中断 */
    if(is_level) {
        
        if (AM_BIT_GET(p_hw_pint->ienr, chan) == 0) {
            return AMHW_LPC82X_PINT_TRIGGER_OFF;
            
        } else if (AM_BIT_GET(p_hw_pint->ienf, chan) == 0) {
            return AMHW_LPC82X_PINT_TRIGGER_LOW;
            
        } else {
            return  AMHW_LPC82X_PINT_TRIGGER_HIGH;
        }
        
    /* 边沿中断 */
    } else {
        if (AM_BIT_GET(p_hw_pint->ienr, chan) &&
            AM_BIT_GET(p_hw_pint->ienf, chan)) {
                
            return AMHW_LPC82X_PINT_TRIGGER_BOTH_EDGES;
            
        } else if (AM_BIT_GET(p_hw_pint->ienr, chan)) {
            return AMHW_LPC82X_PINT_TRIGGER_RISE;
            
        } else if (AM_BIT_GET(p_hw_pint->ienf, chan)){
            return  AMHW_LPC82X_PINT_TRIGGER_FALL;
            
        } else {
            return  AMHW_LPC82X_PINT_TRIGGER_OFF;
        }
    }
}

/**
 * \brief 配置模式匹配引擎的位片参数
 */
void amhw_lpc82x_pint_pmatch_slice_cfg (amhw_lpc82x_pint_t          *p_hw_pint,
                                        amhw_lpc82x_pint_bitslice_t  slice,
                                        uint8_t                      chan,
                                        amhw_lpc82x_pint_slice_arg_t arg,
                    am_bool_t                    is_end)
{
    uint32_t reg;
    uint8_t pos = (slice * 3) + 8;
    
    reg = p_hw_pint->pmsrc;
    AM_BIT_CLR_MASK(reg, AM_SBF(0x07, pos));
    AM_BIT_SET_MASK(reg, AM_SBF(chan, pos));
    p_hw_pint->pmsrc = reg;
    
    reg = p_hw_pint->pmcfg;
    AM_BIT_CLR_MASK(reg, AM_SBF(0x07, pos));
    AM_BIT_SET_MASK(reg, AM_SBF(arg, pos));
    
    if (is_end) {
        AM_BIT_SET(reg, slice);
    } else {
        AM_BIT_CLR(reg, slice);
    }
    
    p_hw_pint->pmcfg = reg;
}

/* end of file */
