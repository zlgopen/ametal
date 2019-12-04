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
 * - 1.00 19-07-31  zc, first implementation
 * \endinternal
 */
#include "hw/amhw_hk32f103rbt6_can.h"



/**
 * \brief CAN 运行模式设置
 *
 * \param[in] p_hw_can          : 指向CAN寄存器块的指针
 * \param[in] operating_mode    : 指向amhw_hk32f103rbt6_can_operating_type_t 类型
 *
 * \return  CAN的模式设置状态是否成功  0  1
 */
uint8_t amhw_hk32f103rbt6_can_operating_mode_request (amhw_hk32f103rbt6_can_t     *p_hw_can,
                                                amhw_hk32f103rbt6_can_mode_t mode)
{
    uint8_t  status  = CAN_DISABLE;
    uint32_t timeout = 0x0000ffff;

    if (mode == AMHW_HK32F103RBT6_CAN_OPERATING_MODE_INIT) {

        /* 请求初始化  */
        p_hw_can->mcr = (uint32_t)((p_hw_can->mcr &(uint32_t)
                                     (~(uint32_t)AMHW_HK32F103RBT6_CAN_MCR_SLEEP)) |
                        AMHW_HK32F103RBT6_CAN_MCR_INRQ);
        /* 等待应答信号 */
        while ( ((p_hw_can->msr & (uint32_t)0x3) != AMHW_HK32F103RBT6_CAN_MSR_INAK) &&
                (timeout != 0)) {
            timeout--;
        }

        /* 超时或ACK后确认状态 */
        if ((p_hw_can->msr & (uint32_t)0x3) != AMHW_HK32F103RBT6_CAN_MSR_INAK) {
            status = CAN_DISABLE;
        } else {
            status = CAN_ENABLE;
        }
    } else if (mode == AMHW_HK32F103RBT6_CAN_OPERATING_NORMAL) {

        /* 请求离开初始化模式或睡眠模式 进入正常模式 */
        p_hw_can->mcr &= (uint32_t) (~(AMHW_HK32F103RBT6_CAN_MCR_SLEEP |
                                       AMHW_HK32F103RBT6_CAN_MCR_INRQ));

        /* 等待应答 */
        while ( ((p_hw_can->msr & (uint32_t)0x3) != 0) && (timeout != 0)) {
            timeout--;
        }

        if ((p_hw_can->msr & (uint32_t)0x3) != 0) {
            status = CAN_DISABLE;
        } else {
            status = CAN_ENABLE;
        }
    } else if (mode == AMHW_HK32F103RBT6_CAN_OPERATING_SLEEP) {
        /* 请求睡眠模式  */
        p_hw_can->mcr = (uint32_t)((p_hw_can->mcr &(uint32_t)
                                     (~(uint32_t)AMHW_HK32F103RBT6_CAN_MCR_INRQ)) |
                        AMHW_HK32F103RBT6_CAN_MCR_SLEEP);
        /* 等待应答信号 */
        while ( ((p_hw_can->msr & (uint32_t)0x3) != AMHW_HK32F103RBT6_CAN_MSR_SLAK) &&
                (timeout != 0)) {
            timeout--;
        }

        /* 超时或ACK后确认状态 */
        if ((p_hw_can->msr & (uint32_t)0x3) != AMHW_HK32F103RBT6_CAN_MSR_SLAK) {
            status = CAN_DISABLE;
        } else {
            status = CAN_ENABLE;
        }
        /* 测试模式 */
    } else {
        status = CAN_DISABLE;
    }

    return (uint8_t) status;
}


/**
 * \brief CAN 检测相应标志位是否被设置
 *
 * \param[in] p_hw_can        : 指向CAN寄存器块的指针
 * \param[in] can_it          : 指向设置 amhw_hk32f103rbt6_can_flag_t
 * \param[in] state           : 状态   0  1
 * \return  无
 */
uint8_t amhw_hk32f103rbt6_can_get_flagstatus(amhw_hk32f103rbt6_can_t        *p_hw_can,
                                       amhw_hk32f103rbt6_can_flag_t    can_flag)
{
    uint8_t bitstatus = CAN_DISABLE;

    if ( (can_flag &  AMHW_HK32F103RBT6_CAN_FLAG_ESR) != (uint32_t)CAN_DISABLE) {
        if ((p_hw_can->esr & (can_flag & 0x000fffff)) !=
                (uint32_t)CAN_DISABLE ) {
            bitstatus = CAN_ENABLE;
        } else {
            bitstatus = CAN_DISABLE;
        }
    } else if ( (can_flag &  AMHW_HK32F103RBT6_CAN_FLAG_MSR) !=
            (uint32_t)CAN_DISABLE) {
        if ((p_hw_can->msr & (can_flag & 0x000fffff)) !=
                (uint32_t)CAN_DISABLE ) {
            bitstatus = CAN_ENABLE;
        } else {
            bitstatus = CAN_DISABLE;
        }
    } else if ( (can_flag &  AMHW_HK32F103RBT6_CAN_FLAG_TSR) !=
            (uint32_t)CAN_DISABLE) {
        if ((p_hw_can->tsr & (can_flag & 0x000fffff)) !=
                (uint32_t)CAN_DISABLE) {
            bitstatus = CAN_ENABLE;
        } else {
            bitstatus = CAN_DISABLE;
        }
    } else if ( (can_flag &  AMHW_HK32F103RBT6_CAN_FLAG_RF0R) !=
            (uint32_t)CAN_DISABLE) {
        if ((p_hw_can->rf_0r & (can_flag & 0x000fffff)) !=
                (uint32_t)CAN_DISABLE) {
            bitstatus = CAN_ENABLE;
        } else {
            bitstatus = CAN_DISABLE;
        }
    } else {
        if ((p_hw_can->rf_1r & (can_flag & 0x000fffff)) !=
                (uint32_t)CAN_DISABLE) {
            bitstatus = CAN_ENABLE;
        } else {
            bitstatus = CAN_DISABLE;
        }
    }

    return bitstatus ;
}


/**
 * \brief 获取指定中断的状态
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
uint8_t amhw_hk32f103rbt6_can_get_itstatus (amhw_hk32f103rbt6_can_t *p_hw_can,
                                      uint32_t           can_it)
{
    uint8_t it_status = CAN_DISABLE;

        switch (can_it) {

        case (AMHW_HK32F103RBT6_CAN_INT_TMEIE):
            if (p_hw_can->ier & 0x01) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->tsr,
                                AMHW_HK32F103RBT6_CAN_TSR_RQCP0 |
                                AMHW_HK32F103RBT6_CAN_TSR_RQCP1 |
                                AMHW_HK32F103RBT6_CAN_TSR_RQCP2 );
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_FMPIE0):
            if (p_hw_can->ier &0x02) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_0r, AMHW_HK32F103RBT6_CAN_RF0R_FMP0);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_FFIE0):
            if (p_hw_can->ier & 0x04) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_0r, AMHW_HK32F103RBT6_CAN_RF0R_FULL0);
            }

            break;

        case (  AMHW_HK32F103RBT6_CAN_INT_FOVIE0):
            if (p_hw_can->ier & 0x08) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_0r, AMHW_HK32F103RBT6_CAN_RF0R_FOVR0);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_FMPIE1):
            if (p_hw_can->ier & 0x10) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_1r, AMHW_HK32F103RBT6_CAN_RF0R_FMP1);
            }

            break;

        case (  AMHW_HK32F103RBT6_CAN_INT_FFIE1):
            if (p_hw_can->ier & 0x20) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_1r, AMHW_HK32F103RBT6_CAN_RF0R_FULL1);
            }

            break;

        case (  AMHW_HK32F103RBT6_CAN_INT_FOVIE1):
            if (p_hw_can->ier & 0x40) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->rf_1r, AMHW_HK32F103RBT6_CAN_RF0R_FOVR1);
            }

            break;

        case (  AMHW_HK32F103RBT6_CAN_INT_EWGIE):
            if (p_hw_can->ier & 0x100) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->esr, AMHW_HK32F103RBT6_CAN_ESR_EWGF);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_EPVIE):
            if (p_hw_can->ier & 0x200) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->esr, AMHW_HK32F103RBT6_CAN_ESR_EPVF);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_BOFIE):
             if (p_hw_can->ier & 0x400) {
                 it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                 p_hw_can->esr, AMHW_HK32F103RBT6_CAN_ESR_BOFF);
             }

           break;

        case (  AMHW_HK32F103RBT6_CAN_INT_LECIE):
            if (p_hw_can->ier & 0x800) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->esr, AMHW_HK32F103RBT6_CAN_ESR_LEC);
            }

            break;

        case (  AMHW_HK32F103RBT6_CAN_INT_ERRIE):
            if (p_hw_can->ier & 0x8000) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->msr, AMHW_HK32F103RBT6_CAN_MSR_ERRI);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_WKUIE):
            if (p_hw_can->ier & 0x10000) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->msr, AMHW_HK32F103RBT6_CAN_MSR_WKUI);
            }

            break;

        case ( AMHW_HK32F103RBT6_CAN_INT_SLKIE):
            if (p_hw_can->ier & 0x20000) {
                it_status = amhw_hk32f103rbt6_can_check_itstatus(
                                p_hw_can->msr, AMHW_HK32F103RBT6_CAN_MSR_SLAKI);
            }

            break;

        default:
            it_status = 0;
            break;

        }

    return (uint8_t)it_status;
}


/**
 * \brief  清除指定中断的标志位
 *
 * \param[in] p_hw_can 指向CAN寄存器块的指针
 *
 * \return 无
 */
void amhw_hk32f103rbt6_can_clear_itstatus (amhw_hk32f103rbt6_can_t *p_hw_can,
                                      uint32_t           can_it)
{
    switch (can_it ) {

    case (AMHW_HK32F103RBT6_CAN_INT_TMEIE):
        p_hw_can->tsr |= (AMHW_HK32F103RBT6_CAN_TSR_RQCP0 |
                          AMHW_HK32F103RBT6_CAN_TSR_RQCP1 |
                          AMHW_HK32F103RBT6_CAN_TSR_RQCP2  );
        break;

    case ( AMHW_HK32F103RBT6_CAN_INT_FMPIE0):

        /* 通过对RFOM 位 置1 来释放FIFO邮箱 */
        p_hw_can->rf_0r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF0R_RFOM0;
        break;

    case (  AMHW_HK32F103RBT6_CAN_INT_FFIE0):

        /* 通过写 1 来清除此位 */
        p_hw_can->rf_0r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF0R_FULL0;
        break;

    case (  AMHW_HK32F103RBT6_CAN_INT_FOVIE0):

        /* 通过写 1 来清除此位 */
        p_hw_can->rf_0r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF0R_FOVR0;
        break;

    case ( AMHW_HK32F103RBT6_CAN_INT_FMPIE1):

        /* 通过对RFOM 位 置1 来释放FIFO邮箱 */
        p_hw_can->rf_1r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF1R_RFOM1;
        break;

    case (  AMHW_HK32F103RBT6_CAN_INT_FFIE1):

        /* 通过写 1 来清除此位 */
        p_hw_can->rf_1r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF0R_FULL1;
        break;

    case ( AMHW_HK32F103RBT6_CAN_INT_FOVIE1):

        /* 通过写 1 来清除此位 */
        p_hw_can->rf_1r |= (uint32_t)AMHW_HK32F103RBT6_CAN_RF0R_FOVR1;
        break;

    case ( AMHW_HK32F103RBT6_CAN_INT_WKUIE):

        /* 通过写 1 来清除此位 */
        p_hw_can->msr |= (uint32_t)AMHW_HK32F103RBT6_CAN_MSR_WKUI;
        break;

    case (AMHW_HK32F103RBT6_CAN_INT_SLKIE):

        /* 通过写 1 来清除此位 */
        p_hw_can->msr |= (uint32_t)AMHW_HK32F103RBT6_CAN_MSR_SLAKI;
        break;

//    case ( 1ul << AMHW_HK32F103RBT6_CAN_INT_EWGIE):
//        p_hw_can->esr &= ~(uint32_t)AMHW_HK32F103RBT6_CAN_ESR_EWGF;
//        break;
//
//    case ( 1ul << AMHW_HK32F103RBT6_CAN_INT_EPVIE):
//        p_hw_can->esr &= ~(uint32_t)AMHW_HK32F103RBT6_CAN_ESR_EPVF;
//        break;
//
//    case ( 1ul << AMHW_HK32F103RBT6_CAN_INT_BOFIE):
//        p_hw_can->esr &= ~(uint32_t)AMHW_HK32F103RBT6_CAN_ESR_BOFF;
//           break;

    case (  AMHW_HK32F103RBT6_CAN_INT_LECIE):
       p_hw_can->esr &= ~(uint32_t)AMHW_HK32F103RBT6_CAN_ESR_LEC;
       break;

    case ( AMHW_HK32F103RBT6_CAN_INT_ERRIE):

       /* 通过写 1 来清除此位 */
       p_hw_can->msr |= (uint32_t)AMHW_HK32F103RBT6_CAN_MSR_ERRI;
       break;

    default:
        break;

    }
}


/* end of file */
