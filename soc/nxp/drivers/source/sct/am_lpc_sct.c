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
 * \brief Timing driver implementation for SCT
 *
 * 1. SCT（状态可配置定时器）是一个以计数器为基础，以事件为中心，状态状态可配置的定时器
 * 2. SCT计数器
 *    - 一个最大32位计数的计数器；
 *    - 可实现单向递增计数或者双向计数；
 *    - 可被SCT事件硬件上自动限制（清零或反向计数）、停止、启动、停止；
 *    - 可被SCT事件硬件上自动跟踪设定的匹配值、判断是否匹配；
 *    - 可被SCT事件硬件上自动获取计数值，以便实现捕获逻辑。
 * 3. SCT事件
 *    - 事件相互独立，事件数目高达13个；
 *    - 可软件配置事件产生的条件，条件可能是IO条件、匹配条件、捕获条件或者IO跟匹配条件组合；
 *    - 可软件配置事件在哪些SCT状态下可以产生的；
 *    - 可软件配置事件产生时，对计数器的操作，如限制（清零或反向计数）、终止、启动、终止；
 *    - 可软件配置事件产生时，是否触发DMA传输请求，支持两个DMA触发通道；
 *    - 可软件配置事件产生时，对输出IO通道进行控制；
 *    - 可软件配置事件产生时，产生SCT事件中断；
 *    - 可软件配置事件产生后，对SCT状态的影响。
 * 4. SCT状态
 *    - SCT提供了13个状态
 *    - 可以配置SCT处于哪个状态；
 *    - 状态之间可通过SCT事件进行自动切换。
 * 5. SCT 输入IO通道
 *    - 提供了8个输入IO通道；
 *    - 每个通道均可用于作为SCT事件触发的IO条件或者捕获引脚状态的输入；
 *    - 4个通道可通过开关矩阵进行关联到任意引脚。
 * 6. SCT 输出IO通道
 *    - 提供了8个输入IO通道；
 *    - 每个通道均可用于作为SCT事件触发的IO条件或者事件产生时，被控制的输出IO通道；
 *    - 当多个事件，同时操作同一个输出通道时，且发生了电平冲突，则可以配置这种冲突的解决方法。
 *
 * \internal
 * \par Modification history
 * - 1.01 15-12-14  hgo, fix potential bug caused by incorrect p_arg on callback
 * - 1.00 15-12-01  hgo, first implementation.
 * \endinternal
 */
#include "am_lpc_sct.h"
#include "ametal.h"
#include "am_int.h"
#include "am_gpio.h"
#include "hw/amhw_lpc_sct.h"
#include "am_clk.h"
/******************************************************************************/
/** \brief 中断未连接标识 */
#define __INT_NOT_CONNECTED      0xFF


/* SCT中断处理函数 */
void __sct_int (void *handle)
{
    const am_lpc_sct_devinfo_t  *p_devinfo =
                                   ((am_lpc_sct_handle_t)handle)->p_devinfo;
    amhw_lpc_sct_t              *p_hw_sct  = NULL;

    int           slot;
    int           evt_num;
    am_pfnvoid_t  pfn_isr;
    void         *p_arg;
    uint32_t      int_flags;

    p_hw_sct = (amhw_lpc_sct_t *)(p_devinfo->sct_regbase);

    int_flags = amhw_lpc_sct_event_flag_get(p_hw_sct);
    for (evt_num = 0; evt_num < p_devinfo->evt_isr_cnt; evt_num++) {
        if (AM_BIT_ISSET(int_flags, evt_num)) {

            slot = p_devinfo->p_isrmap[evt_num];
            if (slot == __INT_NOT_CONNECTED) {
                continue;
            }

            pfn_isr = p_devinfo->p_isrinfo[slot].pfn_isr;
            p_arg   = p_devinfo->p_isrinfo[slot].p_arg;

            if (pfn_isr != NULL) {
                pfn_isr(p_arg);
            }
            /* 清除中断 */
            amhw_lpc_sct_event_flag_clr(p_hw_sct, evt_num);
        }
    }
}

/**
 * \brief SCT事件控制计时器配置
 */
static void __sct_evt_ctrl_cnt_cfgs(amhw_lpc_sct_t  *p_hw_sct,
                                    uint8_t             evt_num,
                                    uint8_t             out_cnt_flags)
{
    /* 限制计数器 */
    if(AM_BIT_ISSET(out_cnt_flags, AM_LPC_SCT_LIMIT_BITS_START)) {
        amhw_lpc_sct_event_limit_enable(p_hw_sct,
                                         AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    } else {
        amhw_lpc_sct_event_limit_disable(p_hw_sct,
                                         AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    }
    /* 停止计数器 */
    if(AM_BIT_ISSET(out_cnt_flags, AM_LPC_SCT_STOP_BITS_START)) {
        amhw_lpc_sct_event_stop_enable(p_hw_sct,
                                        AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    } else {
        amhw_lpc_sct_event_stop_disable(p_hw_sct,
                                         AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    }
    /* 启动计数器 */
    if(AM_BIT_ISSET(out_cnt_flags, AM_LPC_SCT_START_BITS_START)) {
        amhw_lpc_sct_event_start_enable(p_hw_sct,
                                         AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    } else {
        amhw_lpc_sct_event_start_disable(p_hw_sct,
                                          AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    }
    /* 终止计数器 */
    if(AM_BIT_ISSET(out_cnt_flags, AM_LPC_SCT_HALT_BITS_START)) {
        amhw_lpc_sct_event_halt_enable(p_hw_sct,
                                        AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    } else {
        amhw_lpc_sct_event_halt_disable(p_hw_sct,
                                         AMHW_LPC_SCT_MODE_UNIFY, evt_num);
    }
}

/**
 * \brief SCT事件控制输出IO配置
 */
static void __sct_evt_ctrl_io_cfgs(am_lpc_sct_handle_t sct_handle,
                                   uint8_t             evt_num,
                                   uint16_t            out_io_flags)
{
    uint8_t i  = 0;
    uint8_t ch ;

    amhw_lpc_sct_t              *p_hw_sct  = NULL;


    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    /* 设置输出通道置位 */
    ch = AM_BITS_GET(out_io_flags, AM_LPC_SCT_IOSET_CH_BITS_START,
                               sct_handle->p_devinfo->sct_out_num);

    for (i = 0; i < sct_handle->p_devinfo->sct_out_num; i++) {
        if (AM_BIT_ISSET(ch, i)) {
            amhw_lpc_sct_out_set_enable(p_hw_sct, i, evt_num);
        } else {
            amhw_lpc_sct_out_set_disable(p_hw_sct, i, evt_num);
        }
    }

    /* 设置输出通道清零 */
    ch = AM_BITS_GET(out_io_flags,
            AM_LPC_SCT_IOCLR_CH_BITS_START,
      sct_handle->p_devinfo->sct_out_num);

    for (i = 0; i < sct_handle->p_devinfo->sct_out_num; i++) {
        if (AM_BIT_ISSET(ch, i)) {
            amhw_lpc_sct_out_clr_enable(p_hw_sct, i, evt_num);
        } else {
            amhw_lpc_sct_out_clr_disable(p_hw_sct, i, evt_num);
        }
    }
}

/**
 * \brief SCT事件匹配或捕获来设置
 */
static void __sct_evt_matchcap_cfgs(amhw_lpc_sct_t  *p_hw_sct,
                                      uint8_t      evt_num,
                                      uint64_t     cond_flags)
{
    uint32_t tmp;
    uint8_t matcap_num;

    /* 获取通道 */
    matcap_num     = AM_BITS_GET(cond_flags,
                                 AM_LPC_SCT_MATCAP_CH_BITS_START,
                                 AM_LPC_SCT_MATCAP_CH_BITS_LEN);

    tmp = AM_BIT_ISSET(cond_flags,
                       AM_LPC_SCT_MATCAP_ISCAP_BITS_START) ?
                       AMHW_LPC_SCT_MATCAP_CAPTURE :
                       AMHW_LPC_SCT_MATCAP_MATCH;

    /* 配置匹配/捕获寄存器为匹配模式  */
    amhw_lpc_sct_regmode_config(p_hw_sct,
                                   AMHW_LPC_SCT_MODE_UNIFY,
                                   matcap_num,
                                   (amhw_lpc_sct_matcap_mode_t)tmp);

    /* 如果是匹配条件 */
    if (tmp == AMHW_LPC_SCT_MATCAP_MATCH) {
        tmp = (cond_flags >> AM_LPC_SCT_MAT_VAL_BITS_START) &
               0xFFFFFFFF;

        /* 匹配事件，设置匹配值 */
        if (amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY)) {

            amhw_lpc_sct_mat_val_set(p_hw_sct,
                                        AMHW_LPC_SCT_MODE_UNIFY,
                                        matcap_num,
                                        tmp - 1);
        }
        /* 设置匹配周期重载值  */
        amhw_lpc_sct_mat_reload_val_set(p_hw_sct,
                                           AMHW_LPC_SCT_MODE_UNIFY,
                                           matcap_num,
                                           tmp - 1);
    }

}


/**
 * \brief SCT事件中断配置
 */
static void __sct_evt_ctrl_int_cfg (am_lpc_sct_handle_t sct_handle,
                                    uint8_t                evt_num,
                                    am_pfnvoid_t           pfn_isr,
                                    void                  *p_arg,
                                    int                    slot)
{
    const am_lpc_sct_devinfo_t  *p_devinfo = sct_handle->p_devinfo;
    amhw_lpc_sct_t              *p_hw_sct  = NULL;

    p_hw_sct = (amhw_lpc_sct_t *)(p_devinfo->sct_regbase);

    if (NULL != pfn_isr) {
        p_devinfo->p_isrmap[evt_num]       = slot;
        p_devinfo->p_isrinfo[slot].p_arg   = p_arg ;
        p_devinfo->p_isrinfo[slot].pfn_isr = pfn_isr;

        if (p_hw_sct->even == 0) {
            am_int_connect(p_devinfo->inum, __sct_int, (void *)sct_handle);
            am_int_enable(p_devinfo->inum);       /* 使能中断 */
        }
        amhw_lpc_sct_event_int_enable(p_hw_sct, evt_num);
    } else {
        amhw_lpc_sct_event_int_disable(p_hw_sct, evt_num);
        if (p_hw_sct->even == 0) { /* 如果如果没有事件开启中断，关闭中断 */
            am_int_disable(p_devinfo->inum);
            am_int_disconnect(p_devinfo->inum, __sct_int, (void *)sct_handle);
        }
        if (slot != __INT_NOT_CONNECTED) {
            p_devinfo->p_isrinfo[slot].pfn_isr = NULL;
            p_devinfo->p_isrinfo[slot].p_arg   = NULL ;
            p_devinfo->p_isrmap[evt_num]       = __INT_NOT_CONNECTED;
        }

    }
}

/**
 * \brief SCT事件控制DMA配置
 */
static void __sct_evt_ctrl_dma_cfgs (am_lpc_sct_handle_t sct_handle,
                                     uint8_t            evt_num,
                                     uint8_t            out_dma_flags)
{
    uint8_t i  = 0;
    uint8_t ch ;

    const am_lpc_sct_devinfo_t  *p_devinfo = sct_handle->p_devinfo;
    amhw_lpc_sct_t              *p_hw_sct  = NULL;

    p_hw_sct = (amhw_lpc_sct_t *)(p_devinfo->sct_regbase);


    /* 设置输出通道清零 */
    ch = AM_BITS_GET(out_dma_flags, AM_LPC_SCT_DMA_REQ_CH_BITS_START,
                 p_devinfo->sct_dma_req_num);

    for (i = 0; i < p_devinfo->sct_dma_req_num; i++) {
        if (AM_BIT_ISSET(ch, i)) {
          amhw_lpc_sct_dma_request_event_enable(p_hw_sct, i, evt_num);
        } else {
          amhw_lpc_sct_dma_request_event_disable(p_hw_sct, i, evt_num);
        }
    }
}
/**
 * \brief 添加SCT事件
 */
int am_lpc_sct_evt_add (am_lpc_sct_handle_t sct_handle,
                           uint8_t                evt_num,
                           am_lpc_sct_evt_t   *p_evt)
{
  am_lpc_sct_dev_t *p_dev = (am_lpc_sct_dev_t *)sct_handle;

    amhw_lpc_sct_t *p_hw_sct        = NULL;
    uint32_t           evt_ctrl_cfgs   = 0;  /* 事件控制配置参数 */
    uint8_t            matcap_num      = 0;
    int                slot            = __INT_NOT_CONNECTED;
    int                i               = 0;
    uint32_t           tmp;

    if (NULL     == sct_handle                     ||
        NULL     == p_evt                          ||
        AM_FALSE == sct_handle->valid_flg          ||
        evt_num >= p_dev->p_devinfo->evt_isr_cnt   ||
        AM_BIT_ISSET(sct_handle->evt_stat, evt_num)) {

        return -AM_EINVAL;
    }

    /* 判断配置事件的产生条件有效性 */
    if (p_evt->cond_flags == 0) {
        return -AM_EINVAL;
    }

    /* 判断是否配置中断，是否映射内存是否足够 */
    tmp = p_evt->out_int_flag & 0xFFFFFFFF;
    if (0 != tmp) {
        if (NULL == sct_handle->p_devinfo->p_isrmap  ||
            NULL == sct_handle->p_devinfo->p_isrinfo ||
            evt_num >= sct_handle->p_devinfo->evt_isr_cnt ) {
            return -AM_EINVAL;
        }
        slot =  sct_handle->p_devinfo->p_isrmap[evt_num];
        if (slot == __INT_NOT_CONNECTED) {
            for (i = 0; i < sct_handle->p_devinfo->evt_isr_cnt; i++) {
                if (sct_handle->p_devinfo->p_isrinfo[i].pfn_isr == NULL) {
                    slot = i;
                    break;
                }
            }
        }
        if (slot == __INT_NOT_CONNECTED) {
            return -AM_EPERM;                           /* 没有空的内存映射 */
        }
    }

    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    /* 配置事件前，先关闭该事件 */
    amhw_lpc_sct_event_state_cfg(p_hw_sct, evt_num, 0);

    /* 事件对中断配置的控制 */
    __sct_evt_ctrl_int_cfg(sct_handle, evt_num,
                          (am_pfnvoid_t)tmp,
                          (void *)((uint32_t)(p_evt->out_int_flag >>
                                   AM_LPC_SCT_CBPARAM_BITS_START)),
                          slot);

    /* 从输入配置参数，提取出寄存器配置参数 */
    /*
     * 提取IO配置参数
     * 0x1FF为IO条件配置的位域长度
     * 5为IO条件配置在SCT事件控制寄存器（EV[0:7]_CTRL）中的位域偏移
     */
    evt_ctrl_cfgs |= AM_SBF(p_evt->cond_flags & 0x1FF, 5) ;

    /* 提取匹配或捕获通道 */
    matcap_num     = AM_BITS_GET(p_evt->cond_flags,
                                 AM_LPC_SCT_MATCAP_CH_BITS_START,
                                 AM_LPC_SCT_MATCAP_CH_BITS_LEN);
    evt_ctrl_cfgs |= matcap_num;

    /*
     * 提取事件控制SCT状态的变化
     * 0x3F为状态变化配置的位域长度
     * 14为状态变化配置在SCT事件控制寄存器（EV[0:7]_CTRL）中的位域偏移
     */
    evt_ctrl_cfgs |= AM_SBF(((uint32_t)p_evt->out_stat_flag & 0x3F), 14);

    /* 配置事件 */
    amhw_lpc_sct_event_ctrl(p_hw_sct, evt_num, evt_ctrl_cfgs);

    /* 是否设置了捕获或者匹配事件 */
    if(AM_BIT_ISSET(p_evt->cond_flags, AM_LPC_SCT_MATCAP_ISSET_BITS_START)) {
        __sct_evt_matchcap_cfgs(p_hw_sct, evt_num, p_evt->cond_flags);
    }

    /* 事件对计数器的控制 */
    __sct_evt_ctrl_cnt_cfgs(p_hw_sct, evt_num, p_evt->out_cnt_flags);

    /* 事件对IO输出的控制 */
    __sct_evt_ctrl_io_cfgs(sct_handle, evt_num, p_evt->out_io_flags);

    /* 事件对DMA触发的控制 */
    __sct_evt_ctrl_dma_cfgs(sct_handle, evt_num, p_evt->out_dma_flags);

    /* 使能一个事件在某些状态中发生 */
    amhw_lpc_sct_event_state_cfg(p_hw_sct, evt_num, p_evt->stat_en_flags);

    AM_BIT_SET(sct_handle->evt_stat, evt_num);        /* 记录该事件已被使用 */

    return AM_OK;
}

/**
 * \brief 删除SCT事件
 */
int am_lpc_sct_evt_del (am_lpc_sct_handle_t   sct_handle,
                             uint8_t                  evt_num)
{
    amhw_lpc_sct_t              *p_hw_sct  = NULL;



    if (NULL      == sct_handle              ||
        AM_FALSE  == sct_handle->valid_flg   ||
        evt_num   >= sct_handle->p_devinfo->evt_isr_cnt ||
        !AM_BIT_ISSET(sct_handle->evt_stat, evt_num)) {

        return -AM_EINVAL;
    }

    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    amhw_lpc_sct_event_state_cfg(p_hw_sct, evt_num, 0);

    /* 关闭该事件注册的中断 */
    __sct_evt_ctrl_int_cfg (sct_handle, evt_num,
                            NULL, NULL,
                            sct_handle->p_devinfo->p_isrmap[evt_num]);

    AM_BIT_CLR(sct_handle->evt_stat, evt_num);        /* 记录该事件未使用 */

    return AM_OK;
}


/**
 * \brief 终止SCT
 */
int am_lpc_sct_halt (am_lpc_sct_handle_t sct_handle)
{
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if (NULL == sct_handle || AM_FALSE == sct_handle->valid_flg ) {
        return -AM_EINVAL;
    }
    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    /* 设置终止位，终止 SCT */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_HALT_L);

    return AM_OK;
}

/**
 * \brief 停止SCT
 *
 */
int am_lpc_sct_stop (am_lpc_sct_handle_t sct_handle)
{
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if (NULL == sct_handle) {
        return -AM_EINVAL;
    }
    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    /* 设置停止位，暂停 SCT */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L);
    return AM_OK;
}

/**
 * \brief 启动SCT
 */
int am_lpc_sct_start (am_lpc_sct_handle_t sct_handle)
{
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if (NULL == sct_handle || AM_FALSE == sct_handle->valid_flg) {
        return -AM_EINVAL;
    }

    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    /* 清除终止和停止位，启动 SCT */
    amhw_lpc_sct_ctrl_clr(p_hw_sct,
                             AMHW_LPC_SCT_CTRL_STOP_L |
                             AMHW_LPC_SCT_CTRL_HALT_L);
    return AM_OK;
}



/**
 * \brief 解决输出引脚冲突
 */
int am_lpc_sct_conflict_res_config (am_lpc_sct_handle_t sct_handle,
                                       uint8_t                out_num,
                                       uint8_t                value)
{
    amhw_lpc_sct_t              *p_hw_sct  = NULL;

    if (NULL == sct_handle || AM_FALSE == sct_handle->valid_flg) {
        return -AM_EINVAL;
    }

    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    amhw_lpc_sct_conflict_res_config(p_hw_sct, out_num, value);

    return AM_OK;
}

int am_lpc_sct_state_set (am_lpc_sct_handle_t sct_handle,
                             uint16_t               state)
{
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if (NULL == sct_handle || AM_FALSE == sct_handle->valid_flg) {
        return -AM_EINVAL;
    }
    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    if (!amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY)) {
        return -AM_EPERM;
    }

    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, state);

    return AM_OK;
}

/**
 * \brief 通过时间，获取需要设定的匹配值
 */
uint32_t am_lpc_sct_time_to_match(am_lpc_sct_handle_t sct_handle,
                                  uint32_t               period_ns)
{
    uint32_t period_mat;
    uint32_t clkfreq;

    clkfreq = am_clk_rate_get(sct_handle->p_devinfo->clk_id);

    /* 将时间转变为周期 匹配值 */
    period_mat = (uint64_t)(period_ns) * (clkfreq) /
               (uint64_t)1000000000;

    /* 周期至少为1 */
    if (period_mat == 0) {
        period_mat = 1;
    } else if (period_mat > 4294967295UL) {
        period_mat = 4294967295UL;
    }

    return period_mat;
}

/**
 * \brief 设置SCT输出通道的状态
 */
int am_lpc_sct_out_state_set (am_lpc_sct_handle_t sct_handle,
                                 uint8_t                out_num,
                                 uint8_t                value)
{
    am_lpc_sct_dev_t *p_dev    = (am_lpc_sct_dev_t *)sct_handle;
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if (NULL     == sct_handle ||
        AM_FALSE == sct_handle->valid_flg ||
        out_num >=  p_dev->p_devinfo->evt_isr_cnt) {
        return -AM_EINVAL;
    }

    p_hw_sct = (amhw_lpc_sct_t *)(sct_handle->p_devinfo->sct_regbase);

    if (!amhw_lpc_sct_halt_check(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY)) {
        return -AM_EPERM;
    }
    if (value == AM_GPIO_LEVEL_LOW) {
      amhw_lpc_sct_output_state_set(p_hw_sct, out_num);
    } else {
      amhw_lpc_sct_output_state_clr(p_hw_sct, out_num);
    }

    return AM_OK;
}

am_lpc_sct_handle_t am_lpc_sct_init (am_lpc_sct_dev_t     *p_dev,
                                     const am_lpc_sct_devinfo_t *p_devinfo)
{
    uint8_t     i;
    amhw_lpc_sct_t *p_hw_sct  = NULL;

    if ((NULL == p_devinfo) || (NULL == p_devinfo) ) {
        return NULL;
    }

    if (p_devinfo->pfn_plfm_init) {
        p_devinfo->pfn_plfm_init();
    }

    p_hw_sct                  = (amhw_lpc_sct_t *)(p_devinfo->sct_regbase);
    p_dev->p_devinfo          = p_devinfo;
    p_dev->evt_stat           = 0;               /* 所有事件未被使用  */
    p_dev->valid_flg          = AM_TRUE;

    if ((p_devinfo->p_isrmap != NULL) && (p_devinfo->p_isrinfo != NULL)) {
        for (i = 0; i < p_devinfo->evt_isr_cnt; i++) {
            p_devinfo->p_isrmap[i] = __INT_NOT_CONNECTED;
        }

        for (i = 0; i < p_devinfo->isrinfo_cnt; i++) {
            p_devinfo->p_isrinfo[i].pfn_isr = NULL;
        }
    }

    /* 先终止SCT */
    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                       AMHW_LPC_SCT_CTRL_HALT_L);

    /* 配置 SCT 匹配寄存器0 为自动限制，SCT为32位定时器 */
    amhw_lpc_sct_config(p_hw_sct,
                AMHW_LPC_SCT_CONFIG_32BIT_COUNTER  | /* 32为定时器            */
          AMHW_LPC_SCT_CONFIG_CLKMODE_SYSCLK); /* 系统时钟              */

    /* 设置预分频为0.使用系统时钟，分频为0 */
    amhw_lpc_sct_prescale_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);

    /* SCT状态默认为状态0 */
    amhw_lpc_sct_state_set(p_hw_sct, AMHW_LPC_SCT_MODE_UNIFY, 0);

    return p_dev;
}

void am_lpc_sct_deinit (am_lpc_sct_handle_t handle)
{
    am_lpc_sct_dev_t *p_dev    = (am_lpc_sct_dev_t *)handle;
    amhw_lpc_sct_t   *p_hw_sct = NULL;
    uint8_t              evt_num;

    if (p_dev                      == NULL ||
        p_dev->p_devinfo           == NULL ||
        p_dev->valid_flg           == AM_FALSE ) {
        return ;
    }

    p_hw_sct = (amhw_lpc_sct_t *)(handle->p_devinfo->sct_regbase);

    amhw_lpc_sct_ctrl_set(p_hw_sct, AMHW_LPC_SCT_CTRL_STOP_L |
                                       AMHW_LPC_SCT_CTRL_HALT_L);

    am_int_disable(p_dev->p_devinfo->inum);
    am_int_disconnect(p_dev->p_devinfo->inum, __sct_int, (void *)p_dev);

    /* 关闭所有事件 */
    for (evt_num = 0; evt_num < p_dev->p_devinfo->evt_isr_cnt; evt_num++) {
        amhw_lpc_sct_event_state_cfg(p_hw_sct, evt_num, 0);
    }
    p_dev->evt_stat  = 0;               /* 所有事件未被使用  */
    p_dev->valid_flg = AM_FALSE;        /* 参数无效 */

    if (p_dev->p_devinfo->pfn_plfm_deinit) {
        p_dev->p_devinfo->pfn_plfm_deinit();
    }
}


/* end of file */
