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
 * \brief 电源管理接口
 *
 * \internal
 * \par History
 * - 1.00 19-07-18  zp, first implementation
 * \endinternal
 */

#ifndef __AMHW_HK32F103RBT6_PWR_H
#define __AMHW_HK32F103RBT6_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "ametal.h"

/**
 * \addtogroup amhw_hk32f103rbt6_if_pwr
 * \copydoc amhw_hk32f103rbt6_pwr.h
 * @{
 */

/**
  * \brief 系统控制寄存器块结构体
  */
typedef struct amhw_hk32f103rbt6_pwr {
    __IO  uint32_t cr;          /**< \brief 电源控制寄存器 */
    __IO  uint32_t csr;         /**< \brief 电源控制/状态寄存器 */
    __I   uint32_t reserve[2];  /**< \brief 保留 */
    __IO  uint32_t ldo;         /**< \brief 片内 LDO 工作模式输出电压控制寄存器 */
    __IO  uint32_t ldo_stop;    /**< \brief 片内 LDO STOP模式输出电压控制寄存器 */
} amhw_hk32f103rbt6_pwr_t;

/**
 * \name 掉电深度睡眠模式 （停机模式下，电压调压器才能协调工作，请查看原手册）
 * @{
 */
#define AM_HK32F103RBT6_PDDS_STOP_MODE_LDO_ON      0 /* 00 停机模式  电压调压器(LDO)开启*/
#define AM_HK32F103RBT6_PDDS_STOP_MODE_LDO_OFF     1 /* 01 停机模式  电压调压器(LDO)低功耗模式*/
#define AM_HK32F103RBT6_PDDS_STANDBY_MODE_LDO_ON   2 /* 10 待机模式 */
#define AM_HK32F103RBT6_PDDS_STANDBY_MODE_LDO_OFF  3 /* 11 待机模式 */
/** @} */

/**
 * \brief 深度掉电模式选择
 *
 * \param[in] mode      ： 深度掉电模式类型,值为  AM_HK32F103RBT6_PDDS_STOP_MODE_LDO_ON     或
 *                                       AM_HK32F103RBT6_PDDS_STOP_MODE_LDO_OFF    或
 *                                       AM_HK32F103RBT6_PDDS_STANDBY_MODE_LDO_ON  或
 *                                       AM_HK32F103RBT6_PDDS_STANDBY_MODE_LDO_OFF
 * \param[in] p_hw_pwr  ： 指向电源控制寄存器块的指针
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_pwr_pdds_lpds_mode_set (amhw_hk32f103rbt6_pwr_t *p_hw_pwr,
                                         uint8_t            mode)
{
    p_hw_pwr->cr = (p_hw_pwr->cr & (~(3ul << 0))) | (mode << 0) ;
}

/**
 * \name 清除待机位与唤醒位清除
 * @{
 */
#define AM_HK32F103RBT6_STANDBY_FLAG_CLEAR  (1UL << 3)
#define AM_HK32F103RBT6_WAKEUP_FLAG_CLEAR   (1UL << 2)
/** @} */
/**
 * \brief 清除待机标志位与唤醒位
 *
 * \param[in] p_hw_pwr    ：指向电源控制寄存器块的指针
 * \param[in] status_flg  ：标志位清除选择，值为  AM_HK32F103RBT6_*_FLAG_CLEAR 这一类宏
 *                           (# AM_HK32F103RBT6_STANDBY_FLAG_CLEAR)
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_pwr_stauts_flag_clear (amhw_hk32f103rbt6_pwr_t *p_hw_pwr,
                                        uint8_t            status_flg)
{
    p_hw_pwr->cr |= status_flg;
}


/**
 * \name 源电压监测器（PVD）使能
 * @{
 */
#define AM_HK32F103RBT6_PVDE_ENABLE        1
#define AM_HK32F103RBT6_PVDE_DISABLE       0
/** @} */
/**
 * \brief PVD电压监测使能
 *
 * \param[in] p_hw_pwr    ：指向电源控制寄存器块的指针
 * \param[in] pvd_enable  ：电压监测使能，值为  AM_HK32F103RBT6_PVDE_* 这一类宏
 *                           (# AM_HK32F103RBT6_PVDE_ENABLE)
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_pwr_pvd_detect_enable (amhw_hk32f103rbt6_pwr_t *p_hw_pwr,
                                        uint8_t            pvd_enable)
{
    p_hw_pwr->cr = (p_hw_pwr->cr & (~(1ul << 4))) | (pvd_enable << 4) ;
}

/**
 * \brief PVD电平选择
 */
typedef enum amhw_hk32f103rbt6_pvd_lever_set {
    AMHW_HK32F103RBT6_PVD_LEVER_2_2V  = 0,  /**< \brief 2.2V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_3V  = 1,  /**< \brief 2.3V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_4V  = 2,  /**< \brief 2.4V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_5V  = 3,  /**< \brief 2.5V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_6V  = 4,  /**< \brief 2.6V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_7V  = 5,  /**< \brief 2.7V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_8V  = 6,  /**< \brief 2.8V PVD监测电压 */
    AMHW_HK32F103RBT6_PVD_LEVER_2_9V  = 7,  /**< \brief 2.9V PVD监测电压 */
} amhw_hk32f103rbt6_pvd_lever_set_t;
/**
 * \brief PV监测电压选择
 *
 * \param[in] p_hw_pwr ：指向电源控制寄存器块的指针
 * \param[in] pvd_lever：电压监测范围选择, 值为 amhw_hk32f103rbt6_pvd_lever_set_t 这一枚举类型的值
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_pvd_lever_set (amhw_hk32f103rbt6_pwr_t           *p_hw_pwr,
                                amhw_hk32f103rbt6_pvd_lever_set_t  pvd_lever)
{
    p_hw_pwr->cr = (p_hw_pwr->cr & (~(7ul << 5))) | (pvd_lever << 5) ;
}

/**
 * \name BKP备份寄存器访问使能
 * @{
 */
#define AM_HK32F103RBT6_BKP_ENABLE         1
#define AM_HK32F103RBT6_BKP_DISABLE        0
/** @} */
/**
 * \brief 访问备份寄存器使能
 *
 * \param[in] p_hw_pwr    ：指向电源控制寄存器块的指针
 * \param[in] bkp_enable  ：电压监测使能，值为  AM_HK32F103RBT6_BKP_* 这一类宏
 *                           (# AM_HK32F103RBT6_BKP_ENABLE)
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_pwr_bkp_access_enable (amhw_hk32f103rbt6_pwr_t *p_hw_pwr,
                                        uint8_t            bkp_enable)
{
    p_hw_pwr->cr = (p_hw_pwr->cr & (~(1ul << 8))) | (bkp_enable << 8) ;
}

/**
 * \name WAKE_UP唤醒引脚使能
 * @{
 */
#define AM_HK32F103RBT6_WAKEUP_ENABLE      1
#define AM_HK32F103RBT6_WAKEUP_DISABLE     0
/** @} */
/**
 * \brief 使能WAKE_UP引脚
 *
 * \param[in] p_hw_pwr ：指向电源控制寄存器块的指针
 * \param[in] wakeup_en：唤醒引脚使能与否，值为  AM_HK32F103RBT6_WAKEUP_ENABLE  或
 *                                      AM_HK32F103RBT6_WAKEUP_DISABLE
 *
 * \return 无
 */
am_static_inline
void amhw_hk32f103rbt6_wake_up_enable (amhw_hk32f103rbt6_pwr_t *p_hw_pwr, uint8_t wakeup_en)
{
    p_hw_pwr->csr = (p_hw_pwr->csr & (~(1ul << 8))) | (wakeup_en << 8) ;
}


/**
 * \brief 电源控制状态标志
 */
typedef enum amhw_hk32f103rbt6_pwr_status_flag {
    AMHW_HK32F103RBT6_PVD_OUTPUT_FLAG  = (1UL << 2),  /**< \brief PVD输出标志位 */
    AMHW_HK32F103RBT6_PVD_SBF_FLAG     = (1UL << 1),  /**< \brief 待机标志位 */
    AMHW_HK32F103RBT6_PVD_WUF_FLAG     = (1UL << 0),  /**< \brief 唤醒标志位 */
} amhw_hk32f103rbt6_pwr_status_flag_t;
/**
 * \brief 获取电源控制相关状态位
 *
 * \param[in] p_hw_pwr   ：指向电源控制寄存器块的指针
 * \param[in] status_flag：电源控制状态位，值为 amhw_hk32f103rbt6_pwr_status_flag_t 这一枚举类型
 *
 * \return 当返回值不为0时，代表该标志位置1
 */
am_static_inline
uint32_t amhw_hk32f103rbt6_pwr_csr_status_get (
    amhw_hk32f103rbt6_pwr_t *p_hw_pwr, amhw_hk32f103rbt6_pwr_status_flag_t status_flag)
{
    return p_hw_pwr->csr & status_flag;
}

/**
 * \brief MCU在RUN模式下，片内LDO的输出电压
 */
typedef enum amhw_hk32f103rbt6_pwr_ldo_run_set {
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_20V  = 0,     /**< \brief 1.20V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_24V  = 1,     /**< \brief 1.24V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_28V  = 2,     /**< \brief 1.28V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_32V  = 3,     /**< \brief 1.32V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_36V  = 4,     /**< \brief 1.36V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_40V  = 5,     /**< \brief 1.40V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_44V  = 6,     /**< \brief 1.44V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_52V  = 7,     /**< \brief 1.52V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_56V  = 8,     /**< \brief 1.56V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_60V  = 9,     /**< \brief 1.60V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_64V  = 10,    /**< \brief 1.64V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_LDO_RUN_1_68V  = 11,    /**< \brief 1.68V 片内 LDO输出电压 */
} amhw_hk32f103rbt6_pwr_ldo_run_t;
/**
 * \brief 片内LDO 工作模式输出电压控制寄存器
 *
 * \param[in] p_hw_pwr   ：指向电源控制寄存器块的指针
 * \param[in] status_flag：电源控制状态位，值为 amhw_hk32f103rbt6_pwr_ldo_run_t 这一枚举类型
 *
 * \note ：本寄存器保留位含有芯片内部测试寄存器，禁止改变其初始值，否则芯片功能可能异常。
 *
 * \return   AM_OK   :设置成功。
 *           AM_ERROR：设置失败，参数错误。
 */
am_static_inline
int amhw_hk32f103rbt6_pwr_ldo_run_set (
    amhw_hk32f103rbt6_pwr_t *p_hw_pwr, amhw_hk32f103rbt6_pwr_ldo_run_t ldo_run_vol)
{
    if((ldo_run_vol & 0xful) <= AMHW_HK32F103RBT6_PWR_LDO_RUN_1_68V) {

        /* 传递参数必须为枚举类型变量，才能进行对应设置 */
        p_hw_pwr->ldo = (p_hw_pwr->ldo & (~(0xful << 0))) |
                        ((ldo_run_vol & 0xful) << 0) ;

        return AM_OK;
    } else {

        return AM_ERROR;
    }
}

/**
 * \brief MCU在STOP模式下，片内LDO的输出电压
 */
typedef enum amhw_hk32f103rbt6_pwr_ldo_stop_set {
    AMHW_HK32F103RBT6_PWR_STOP_1_20V  = 0,     /**< \brief 1.20V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_24V  = 1,     /**< \brief 1.24V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_28V  = 2,     /**< \brief 1.28V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_32V  = 3,     /**< \brief 1.32V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_36V  = 4,     /**< \brief 1.36V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_40V  = 5,     /**< \brief 1.40V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_44V  = 6,     /**< \brief 1.44V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_52V  = 7,     /**< \brief 1.52V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_56V  = 8,     /**< \brief 1.56V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_60V  = 9,     /**< \brief 1.60V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_64V  = 10,    /**< \brief 1.64V 片内 LDO输出电压 */
    AMHW_HK32F103RBT6_PWR_STOP_1_68V  = 11,    /**< \brief 1.68V 片内 LDO输出电压 */
} amhw_hk32f103rbt6_pwr_ldo_stop_set_t;
/**
 * \brief 片内LDO STOP模式模式输出电压控制寄存器
 *
 * \param[in] p_hw_pwr   ：指向电源控制寄存器块的指针
 * \param[in] status_flag：电源控制状态位，
 *                         值为 amhw_hk32f103rbt6_pwr_ldo_stop_set_t 这一枚举类型
 *
 * \note ：本寄存器保留位含有芯片内部测试寄存器，禁止改变其初始值，否则芯片功能可能异常。
 *
 * \return   AM_OK   :设置成功。
 *           AM_ERROR：设置失败，参数错误。
 */
am_static_inline
int amhw_hk32f103rbt6_pwr_ldo_stop_set (
    amhw_hk32f103rbt6_pwr_t *p_hw_pwr, amhw_hk32f103rbt6_pwr_ldo_stop_set_t ldo_stop_vol)
{
    if((ldo_stop_vol & 0xful) <= AMHW_HK32F103RBT6_PWR_STOP_1_68V) {

        /* 传递参数必须为枚举类型变量，才能进行对应设置 */
        p_hw_pwr->ldo_stop = (p_hw_pwr->ldo_stop & (~(0xful << 0))) |
                             ((ldo_stop_vol & 0xful) << 0) ;

        return AM_OK;
    }
    return AM_ERROR;
}

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HK32F103RBT6_PWR_H */

/* end of file */
