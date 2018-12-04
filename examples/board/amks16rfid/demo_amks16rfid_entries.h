/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Guangzhou ZHIYUAN Electronics Stock Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
* e-mail:      ametal.support@zlg.cn
*******************************************************************************/
/**
 * \file
 * \brief am824rfid 所有例程函数入口声明，本文只包含fm175xx相关demo，
 *  其他demo请参照am116_core中的demo
 * \sa demo_am824zb_all_entries.h
 *
 * \internal
 * \par Modification history
 * - 1.00 18-12-03  htf, first implementation.
 * \endinternal
 */

/**
 * \brief A类LPCD模式 卡读卡类型和卡号例程
 */
void demo_amks16rfid_dr_fm175xx_lpcd_read_id (void);

/**
 * \brief A类卡读休眠卡例程
 */
void demo_amks16rfid_dr_fm175xx_picca_halt (void);

/**
 * \brief A类卡读块例程
 */
void demo_amks16rfid_dr_fm175xx_picca_read_block (void);

/**
 * \brief A类卡读卡类型和卡号例程
 */
void demo_amks16rfid_dr_fm175xx_picca_read_id (void);

/**
 * \brief A类卡值操作例程
 */
void demo_amks16rfid_dr_fm175xx_picca_val_operate (void);

/**
 * \brief A类卡写块例程
 */
void demo_amks16rfid_dr_fm175xx_picca_write_block (void);

/**
 * \brief B类卡读卡号例程
 */
void demo_amks16rfid_dr_fm175xx_piccb_read_id (void);

/* end of file */
