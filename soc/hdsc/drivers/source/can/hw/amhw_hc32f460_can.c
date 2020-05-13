/*******************************************************************************
*                                 AMetal
*                       ---------------------------
*                       innovating embedded systems
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
 * \brief CAN
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-06  zcb, first implementation.
 * \endinternal
 */
#include "hw/amhw_hc32f460_can.h"

#if 0
/**
 * \brief can 模式设置
 */
void amhw_hc32f460_can_mode_set (amhw_hc32f460_can_t      *p_hw_can,
                                 amhw_hc32f460_can_type_t type,
                                 amhw_hc32f460_can_mode_t *p_now,
                                 amhw_hc32f460_can_mode_t update)
{
    if (NULL == p_hw_can || NULL == p_now) {
        return;
    }

    /* 更新模式和原模式相等则直接退出 */
    if (update == *p_now) {
        return;
    }

    /* 如果CAN处于睡眠模式，且将设置为其他模式，则先唤醒CAN，将参生唤醒中断 */
    if (*p_now == AMHW_HC32F460_CAN_MODE_SLEEP) {
        if (AMHW_HC32F460_CAN_BASIC_CAN == type) {
            amhw_hc32f460_basic_can_wake_up(p_hw_can);
        } else {
            amhw_hc32f460_peli_can_mod_clr(p_hw_can,
                                           AMHW_HC32F460_CAN_PELI_MODE_SM);
        }
    }

    /* basic CAN模式 */
    if (AMHW_HC32F460_CAN_BASIC_CAN == type) {

        /* 切换状态到正常模式 */
        if (AMHW_HC32F460_CAN_MODE_RESET == *p_now) {
            amhw_hc32f460_basic_can_reset_clr(p_hw_can);
        } else if (AMHW_HC32F460_CAN_MODE_SLEEP == *p_now) {
            amhw_hc32f460_basic_can_wake_up(p_hw_can);
        }

        /* 设置状态 */
        if (AMHW_HC32F460_CAN_MODE_RESET == update) {
            amhw_hc32f460_basic_can_reset_set(p_hw_can);
        } else if (AMHW_HC32F460_CAN_MODE_SLEEP == *p_now) {
            amhw_hc32f460_basic_can_go_to_sleep(p_hw_can);
        }

    } else {

        /* peli CAN,切换到正常模式 */
        amhw_hc32f460_peli_can_mod_clr(p_hw_can, *p_now);

        /* 只听模式只能在复位模式下进入 */
        if (AMHW_HC32F460_CAN_MODE_ONLY_LISTEN == update) {
            amhw_hc32f460_peli_can_mod_set(p_hw_can,
                                           AMHW_HC32F460_CAN_MODE_RESET);
        }

        /* 设置模式 */
        if (AMHW_HC32F460_CAN_MODE_RUN != update) {
            amhw_hc32f460_peli_can_mod_set(p_hw_can, update);
        }

    }

    *p_now = update;
}
#endif
/* end of file */
