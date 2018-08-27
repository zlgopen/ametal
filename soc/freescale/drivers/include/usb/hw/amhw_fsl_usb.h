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
 * \brief USB 硬件操作接口
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-21  bob, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_USB_H
#define __AMHW_FSL_USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
/**
 * @addtogroup amhw_fsl_if_usb
 * @copydoc amhw_fsl_usb.h
 * @{
 */


/*!
 * @addtogroup USB_Peripheral_Access_Layer USB Peripheral Access Layer
 * @{
 */

/** USB - Register Layout Typedef */
typedef struct {
  __I  uint8_t perid;                              /**< Peripheral ID register, offset: 0x0 */
       uint8_t reserved_0[3];
  __I  uint8_t idcomp;                             /**< Peripheral ID Complement register, offset: 0x4 */
       uint8_t reserved_1[3];
  __I  uint8_t rev;                                /**< Peripheral Revision register, offset: 0x8 */
       uint8_t reserved_2[3];
  __I  uint8_t addinfo;                            /**< Peripheral Additional Info register, offset: 0xC */
       uint8_t reserved_3[3];
  __IO uint8_t otgistat;                           /**< OTG Interrupt Status register, offset: 0x10 */
       uint8_t reserved_4[3];
  __IO uint8_t otgicr;                             /**< OTG Interrupt Control register, offset: 0x14 */
       uint8_t reserved_5[3];
  __IO uint8_t otgstat;                            /**< OTG Status register, offset: 0x18 */
       uint8_t reserved_6[3];
  __IO uint8_t otgctl;                             /**< OTG Control register, offset: 0x1C */
       uint8_t reserved_7[99];
  __IO uint8_t istat;                              /**< Interrupt Status register, offset: 0x80 */
       uint8_t reserved_8[3];
  __IO uint8_t inten;                              /**< Interrupt Enable register, offset: 0x84 */
       uint8_t reserved_9[3];
  __IO uint8_t errstat;                            /**< Error Interrupt Status register, offset: 0x88 */
       uint8_t reserved_10[3];
  __IO uint8_t erren;                              /**< Error Interrupt Enable register, offset: 0x8C */
       uint8_t reserved_11[3];
  __I  uint8_t stat;                               /**< Status register, offset: 0x90 */
       uint8_t reserved_12[3];
  __IO uint8_t ctl;                                /**< Control register, offset: 0x94 */
       uint8_t reserved_13[3];
  __IO uint8_t addr;                               /**< Address register, offset: 0x98 */
       uint8_t reserved_14[3];
  __IO uint8_t bdtpage1;                           /**< BDT Page register 1, offset: 0x9C */
       uint8_t reserved_15[3];
  __IO uint8_t frmnuml;                            /**< Frame Number register Low, offset: 0xA0 */
       uint8_t reserved_16[3];
  __IO uint8_t frmnumh;                            /**< Frame Number register High, offset: 0xA4 */
       uint8_t reserved_17[3];
  __IO uint8_t token;                              /**< Token register, offset: 0xA8 */
       uint8_t reserved_18[3];
  __IO uint8_t softhld;                            /**< SOF Threshold register, offset: 0xAC */
       uint8_t reserved_19[3];
  __IO uint8_t bdtpage2;                           /**< BDT Page Register 2, offset: 0xB0 */
       uint8_t reserved_20[3];
  __IO uint8_t bdtpage3;                           /**< BDT Page Register 3, offset: 0xB4 */
       uint8_t reserved_21[11];
  struct {                                         /* offset: 0xC0, array step: 0x4 */
    __IO uint8_t endpt;                              /**< Endpoint Control register, array offset: 0xC0, array step: 0x4 */
         uint8_t reserved_0[3];
  } endpoint[16];
  __IO uint8_t usbctrl;                            /**< USB Control register, offset: 0x100 */
       uint8_t reserved_22[3];
  __I  uint8_t observe;                            /**< USB OTG Observe register, offset: 0x104 */
       uint8_t reserved_23[3];
  __IO uint8_t control;                            /**< USB OTG Control register, offset: 0x108 */
       uint8_t reserved_24[3];
  __IO uint8_t usbtrc0;                            /**< USB Transceiver Control register 0, offset: 0x10C */
       uint8_t reserved_25[7];
  __IO uint8_t usbfrmadjust;                       /**< Frame Adjust Register, offset: 0x114 */
} amhw_fsl_usbotg_t;
/* ----------------------------------------------------------------------------
   -- USB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup USB_Register_Masks USB Register Masks
 * @{
 */

/*! @name PERID - Peripheral ID register */
#define AMHW_FSL_USB_PERID_ID_MASK                        (0x3FU)
#define AMHW_FSL_USB_PERID_ID_SHIFT                       (0U)
#define AMHW_FSL_USB_PERID_ID(x)                          (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_PERID_ID_SHIFT)) & AMHW_FSL_USB_PERID_ID_MASK)

/*! @name IDCOMP - Peripheral ID Complement register */
#define AMHW_FSL_USB_IDCOMP_NID_MASK                      (0x3FU)
#define AMHW_FSL_USB_IDCOMP_NID_SHIFT                     (0U)
#define AMHW_FSL_USB_IDCOMP_NID(x)                        (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_IDCOMP_NID_SHIFT)) & AMHW_FSL_USB_IDCOMP_NID_MASK)

/*! @name REV - Peripheral Revision register */
#define AMHW_FSL_USB_REV_REV_MASK                         (0xFFU)
#define AMHW_FSL_USB_REV_REV_SHIFT                        (0U)
#define AMHW_FSL_USB_REV_REV(x)                           (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_REV_REV_SHIFT)) & AMHW_FSL_USB_REV_REV_MASK)

/*! @name ADDINFO - Peripheral Additional Info register */
#define AMHW_FSL_USB_ADDINFO_IEHOST_MASK                  (0x1U)
#define AMHW_FSL_USB_ADDINFO_IEHOST_SHIFT                 (0U)
#define AMHW_FSL_USB_ADDINFO_IEHOST(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ADDINFO_IEHOST_SHIFT)) & AMHW_FSL_USB_ADDINFO_IEHOST_MASK)
#define AMHW_FSL_USB_ADDINFO_IRQNUM_MASK                  (0xF8U)
#define AMHW_FSL_USB_ADDINFO_IRQNUM_SHIFT                 (3U)
#define AMHW_FSL_USB_ADDINFO_IRQNUM(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ADDINFO_IRQNUM_SHIFT)) & AMHW_FSL_USB_ADDINFO_IRQNUM_MASK)

/*! @name OTGISTAT - OTG Interrupt Status register */
#define AMHW_FSL_USB_OTGISTAT_AVBUSCHG_MASK               (0x1U)
#define AMHW_FSL_USB_OTGISTAT_AVBUSCHG_SHIFT              (0U)
#define AMHW_FSL_AMHW_FSL_(x)                 (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_AVBUSCHG_SHIFT)) & AMHW_FSL_USB_OTGISTAT_AVBUSCHG_MASK)
#define AMHW_FSL_USB_OTGISTAT_B_SESS_CHG_MASK             (0x4U)
#define AMHW_FSL_USB_OTGISTAT_B_SESS_CHG_SHIFT            (2U)
#define AMHW_FSL_USB_OTGISTAT_B_SESS_CHG(x)               (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_B_SESS_CHG_SHIFT)) & AMHW_FSL_USB_OTGISTAT_B_SESS_CHG_MASK)
#define AMHW_FSL_USB_OTGISTAT_SESSVLDCHG_MASK             (0x8U)
#define AMHW_FSL_USB_OTGISTAT_SESSVLDCHG_SHIFT            (3U)
#define AMHW_FSL_USB_OTGISTAT_SESSVLDCHG(x)               (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_SESSVLDCHG_SHIFT)) & AMHW_FSL_USB_OTGISTAT_SESSVLDCHG_MASK)
#define AMHW_FSL_USB_OTGISTAT_LINE_STATE_CHG_MASK         (0x20U)
#define AMHW_FSL_USB_OTGISTAT_LINE_STATE_CHG_SHIFT        (5U)
#define AMHW_FSL_USB_OTGISTAT_LINE_STATE_CHG(x)           (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_LINE_STATE_CHG_SHIFT)) & AMHW_FSL_USB_OTGISTAT_LINE_STATE_CHG_MASK)
#define AMHW_FSL_USB_OTGISTAT_ONEMSEC_MASK                (0x40U)
#define AMHW_FSL_USB_OTGISTAT_ONEMSEC_SHIFT               (6U)
#define AMHW_FSL_USB_OTGISTAT_ONEMSEC(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_ONEMSEC_SHIFT)) & AMHW_FSL_USB_OTGISTAT_ONEMSEC_MASK)
#define AMHW_FSL_USB_OTGISTAT_IDCHG_MASK                  (0x80U)
#define AMHW_FSL_USB_OTGISTAT_IDCHG_SHIFT                 (7U)
#define AMHW_FSL_USB_OTGISTAT_IDCHG(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGISTAT_IDCHG_SHIFT)) & AMHW_FSL_USB_OTGISTAT_IDCHG_MASK)

/*! @name OTGICR - OTG Interrupt Control register */
#define AMHW_FSL_USB_OTGICR_AVBUSEN_MASK                  (0x1U)
#define AMHW_FSL_USB_OTGICR_AVBUSEN_SHIFT                 (0U)
#define AMHW_FSL_USB_OTGICR_AVBUSEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_AVBUSEN_SHIFT)) & AMHW_FSL_USB_OTGICR_AVBUSEN_MASK)
#define AMHW_FSL_USB_OTGICR_BSESSEN_MASK                  (0x4U)
#define AMHW_FSL_USB_OTGICR_BSESSEN_SHIFT                 (2U)
#define AMHW_FSL_USB_OTGICR_BSESSEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_BSESSEN_SHIFT)) & AMHW_FSL_USB_OTGICR_BSESSEN_MASK)
#define AMHW_FSL_USB_OTGICR_SESSVLDEN_MASK                (0x8U)
#define AMHW_FSL_USB_OTGICR_SESSVLDEN_SHIFT               (3U)
#define AMHW_FSL_USB_OTGICR_SESSVLDEN(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_SESSVLDEN_SHIFT)) & AMHW_FSL_USB_OTGICR_SESSVLDEN_MASK)
#define AMHW_FSL_USB_OTGICR_LINESTATEEN_MASK              (0x20U)
#define AMHW_FSL_USB_OTGICR_LINESTATEEN_SHIFT             (5U)
#define AMHW_FSL_USB_OTGICR_LINESTATEEN(x)                (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_LINESTATEEN_SHIFT)) & AMHW_FSL_USB_OTGICR_LINESTATEEN_MASK)
#define AMHW_FSL_USB_OTGICR_ONEMSECEN_MASK                (0x40U)
#define AMHW_FSL_USB_OTGICR_ONEMSECEN_SHIFT               (6U)
#define AMHW_FSL_USB_OTGICR_ONEMSECEN(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_ONEMSECEN_SHIFT)) & AMHW_FSL_USB_OTGICR_ONEMSECEN_MASK)
#define AMHW_FSL_USB_OTGICR_IDEN_MASK                     (0x80U)
#define AMHW_FSL_USB_OTGICR_IDEN_SHIFT                    (7U)
#define AMHW_FSL_USB_OTGICR_IDEN(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGICR_IDEN_SHIFT)) & AMHW_FSL_USB_OTGICR_IDEN_MASK)

/*! @name OTGSTAT - OTG Status register */
#define AMHW_FSL_USB_OTGSTAT_AVBUSVLD_MASK                (0x1U)
#define AMHW_FSL_USB_OTGSTAT_AVBUSVLD_SHIFT               (0U)
#define AMHW_FSL_USB_OTGSTAT_AVBUSVLD(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_AVBUSVLD_SHIFT)) & AMHW_FSL_USB_OTGSTAT_AVBUSVLD_MASK)
#define AMHW_FSL_USB_OTGSTAT_BSESSEND_MASK                (0x4U)
#define AMHW_FSL_USB_OTGSTAT_BSESSEND_SHIFT               (2U)
#define AMHW_FSL_USB_OTGSTAT_BSESSEND(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_BSESSEND_SHIFT)) & AMHW_FSL_USB_OTGSTAT_BSESSEND_MASK)
#define AMHW_FSL_USB_OTGSTAT_SESS_VLD_MASK                (0x8U)
#define AMHW_FSL_USB_OTGSTAT_SESS_VLD_SHIFT               (3U)
#define AMHW_FSL_USB_OTGSTAT_SESS_VLD(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_SESS_VLD_SHIFT)) & AMHW_FSL_USB_OTGSTAT_SESS_VLD_MASK)
#define AMHW_FSL_USB_OTGSTAT_LINESTATESTABLE_MASK         (0x20U)
#define AMHW_FSL_USB_OTGSTAT_LINESTATESTABLE_SHIFT        (5U)
#define AMHW_FSL_USB_OTGSTAT_LINESTATESTABLE(x)           (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_LINESTATESTABLE_SHIFT)) & AMHW_FSL_USB_OTGSTAT_LINESTATESTABLE_MASK)
#define AMHW_FSL_USB_OTGSTAT_ONEMSECEN_MASK               (0x40U)
#define AMHW_FSL_USB_OTGSTAT_ONEMSECEN_SHIFT              (6U)
#define AMHW_FSL_USB_OTGSTAT_ONEMSECEN(x)                 (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_ONEMSECEN_SHIFT)) & AMHW_FSL_USB_OTGSTAT_ONEMSECEN_MASK)
#define AMHW_FSL_USB_OTGSTAT_ID_MASK                      (0x80U)
#define AMHW_FSL_USB_OTGSTAT_ID_SHIFT                     (7U)
#define AMHW_FSL_USB_OTGSTAT_ID(x)                        (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGSTAT_ID_SHIFT)) & AMHW_FSL_USB_OTGSTAT_ID_MASK)

/*! @name OTGCTL - OTG Control register */
#define AMHW_FSL_USB_OTGCTL_OTGEN_MASK                    (0x4U)
#define AMHW_FSL_USB_OTGCTL_OTGEN_SHIFT                   (2U)
#define AMHW_FSL_USB_OTGCTL_OTGEN(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGCTL_OTGEN_SHIFT)) & AMHW_FSL_USB_OTGCTL_OTGEN_MASK)
#define AMHW_FSL_USB_OTGCTL_DMLOW_MASK                    (0x10U)
#define AMHW_FSL_USB_OTGCTL_DMLOW_SHIFT                   (4U)
#define AMHW_FSL_USB_OTGCTL_DMLOW(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGCTL_DMLOW_SHIFT)) & AMHW_FSL_USB_OTGCTL_DMLOW_MASK)
#define AMHW_FSL_USB_OTGCTL_DPLOW_MASK                    (0x20U)
#define AMHW_FSL_USB_OTGCTL_DPLOW_SHIFT                   (5U)
#define AMHW_FSL_USB_OTGCTL_DPLOW(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGCTL_DPLOW_SHIFT)) & AMHW_FSL_USB_OTGCTL_DPLOW_MASK)
#define AMHW_FSL_USB_OTGCTL_DPHIGH_MASK                   (0x80U)
#define AMHW_FSL_USB_OTGCTL_DPHIGH_SHIFT                  (7U)
#define AMHW_FSL_USB_OTGCTL_DPHIGH(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OTGCTL_DPHIGH_SHIFT)) & AMHW_FSL_USB_OTGCTL_DPHIGH_MASK)

/*! @name ISTAT - Interrupt Status register */
#define AMHW_FSL_USB_ISTAT_USBRST_MASK                    (0x1U)
#define AMHW_FSL_USB_ISTAT_USBRST_SHIFT                   (0U)
#define AMHW_FSL_USB_ISTAT_USBRST(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_USBRST_SHIFT)) & AMHW_FSL_USB_ISTAT_USBRST_MASK)
#define AMHW_FSL_USB_ISTAT_ERROR_MASK                     (0x2U)
#define AMHW_FSL_USB_ISTAT_ERROR_SHIFT                    (1U)
#define AMHW_FSL_USB_ISTAT_ERROR(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_ERROR_SHIFT)) & AMHW_FSL_USB_ISTAT_ERROR_MASK)
#define AMHW_FSL_USB_ISTAT_SOFTOK_MASK                    (0x4U)
#define AMHW_FSL_USB_ISTAT_SOFTOK_SHIFT                   (2U)
#define AMHW_FSL_USB_ISTAT_SOFTOK(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_SOFTOK_SHIFT)) & AMHW_FSL_USB_ISTAT_SOFTOK_MASK)
#define AMHW_FSL_USB_ISTAT_TOKDNE_MASK                    (0x8U)
#define AMHW_FSL_USB_ISTAT_TOKDNE_SHIFT                   (3U)
#define AMHW_FSL_USB_ISTAT_TOKDNE(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_TOKDNE_SHIFT)) & AMHW_FSL_USB_ISTAT_TOKDNE_MASK)
#define AMHW_FSL_USB_ISTAT_SLEEP_MASK                     (0x10U)
#define AMHW_FSL_USB_ISTAT_SLEEP_SHIFT                    (4U)
#define AMHW_FSL_USB_ISTAT_SLEEP(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_SLEEP_SHIFT)) & AMHW_FSL_USB_ISTAT_SLEEP_MASK)
#define AMHW_FSL_USB_ISTAT_RESUME_MASK                    (0x20U)
#define AMHW_FSL_USB_ISTAT_RESUME_SHIFT                   (5U)
#define AMHW_FSL_USB_ISTAT_RESUME(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_RESUME_SHIFT)) & AMHW_FSL_USB_ISTAT_RESUME_MASK)
#define AMHW_FSL_USB_ISTAT_ATTACH_MASK                    (0x40U)
#define AMHW_FSL_USB_ISTAT_ATTACH_SHIFT                   (6U)
#define AMHW_FSL_USB_ISTAT_ATTACH(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_ATTACH_SHIFT)) & AMHW_FSL_USB_ISTAT_ATTACH_MASK)
#define AMHW_FSL_USB_ISTAT_STALL_MASK                     (0x80U)
#define AMHW_FSL_USB_ISTAT_STALL_SHIFT                    (7U)
#define AMHW_FSL_USB_ISTAT_STALL(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ISTAT_STALL_SHIFT)) & AMHW_FSL_USB_ISTAT_STALL_MASK)

/*! @name INTEN - Interrupt Enable register */
#define AMHW_FSL_USB_INTEN_USBRSTEN_MASK                  (0x1U)
#define AMHW_FSL_USB_INTEN_USBRSTEN_SHIFT                 (0U)
#define AMHW_FSL_USB_INTEN_USBRSTEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_USBRSTEN_SHIFT)) & AMHW_FSL_USB_INTEN_USBRSTEN_MASK)
#define AMHW_FSL_USB_INTEN_ERROREN_MASK                   (0x2U)
#define AMHW_FSL_USB_INTEN_ERROREN_SHIFT                  (1U)
#define AMHW_FSL_USB_INTEN_ERROREN(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_ERROREN_SHIFT)) & AMHW_FSL_USB_INTEN_ERROREN_MASK)
#define AMHW_FSL_USB_INTEN_SOFTOKEN_MASK                  (0x4U)
#define AMHW_FSL_USB_INTEN_SOFTOKEN_SHIFT                 (2U)
#define AMHW_FSL_USB_INTEN_SOFTOKEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_SOFTOKEN_SHIFT)) & AMHW_FSL_USB_INTEN_SOFTOKEN_MASK)
#define AMHW_FSL_USB_INTEN_TOKDNEEN_MASK                  (0x8U)
#define AMHW_FSL_USB_INTEN_TOKDNEEN_SHIFT                 (3U)
#define AMHW_FSL_USB_INTEN_TOKDNEEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_TOKDNEEN_SHIFT)) & AMHW_FSL_USB_INTEN_TOKDNEEN_MASK)
#define AMHW_FSL_USB_INTEN_SLEEPEN_MASK                   (0x10U)
#define AMHW_FSL_USB_INTEN_SLEEPEN_SHIFT                  (4U)
#define AMHW_FSL_USB_INTEN_SLEEPEN(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_SLEEPEN_SHIFT)) & AMHW_FSL_USB_INTEN_SLEEPEN_MASK)
#define AMHW_FSL_USB_INTEN_RESUMEEN_MASK                  (0x20U)
#define AMHW_FSL_USB_INTEN_RESUMEEN_SHIFT                 (5U)
#define AMHW_FSL_USB_INTEN_RESUMEEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_RESUMEEN_SHIFT)) & AMHW_FSL_USB_INTEN_RESUMEEN_MASK)
#define AMHW_FSL_USB_INTEN_ATTACHEN_MASK                  (0x40U)
#define AMHW_FSL_USB_INTEN_ATTACHEN_SHIFT                 (6U)
#define AMHW_FSL_USB_INTEN_ATTACHEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_ATTACHEN_SHIFT)) & AMHW_FSL_USB_INTEN_ATTACHEN_MASK)
#define AMHW_FSL_USB_INTEN_STALLEN_MASK                   (0x80U)
#define AMHW_FSL_USB_INTEN_STALLEN_SHIFT                  (7U)
#define AMHW_FSL_USB_INTEN_STALLEN(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_INTEN_STALLEN_SHIFT)) & AMHW_FSL_USB_INTEN_STALLEN_MASK)

/*! @name ERRSTAT - Error Interrupt Status register */
#define AMHW_FSL_USB_ERRSTAT_PIDERR_MASK                  (0x1U)
#define AMHW_FSL_USB_ERRSTAT_PIDERR_SHIFT                 (0U)
#define AMHW_FSL_USB_ERRSTAT_PIDERR(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_PIDERR_SHIFT)) & AMHW_FSL_USB_ERRSTAT_PIDERR_MASK)
#define AMHW_FSL_USB_ERRSTAT_CRC5EOF_MASK                 (0x2U)
#define AMHW_FSL_USB_ERRSTAT_CRC5EOF_SHIFT                (1U)
#define AMHW_FSL_USB_ERRSTAT_CRC5EOF(x)                   (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_CRC5EOF_SHIFT)) & AMHW_FSL_USB_ERRSTAT_CRC5EOF_MASK)
#define AMHW_FSL_USB_ERRSTAT_CRC16_MASK                   (0x4U)
#define AMHW_FSL_USB_ERRSTAT_CRC16_SHIFT                  (2U)
#define AMHW_FSL_USB_ERRSTAT_CRC16(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_CRC16_SHIFT)) & AMHW_FSL_USB_ERRSTAT_CRC16_MASK)
#define AMHW_FSL_USB_ERRSTAT_DFN8_MASK                    (0x8U)
#define AMHW_FSL_USB_ERRSTAT_DFN8_SHIFT                   (3U)
#define AMHW_FSL_USB_ERRSTAT_DFN8(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_DFN8_SHIFT)) & AMHW_FSL_USB_ERRSTAT_DFN8_MASK)
#define AMHW_FSL_USB_ERRSTAT_BTOERR_MASK                  (0x10U)
#define AMHW_FSL_USB_ERRSTAT_BTOERR_SHIFT                 (4U)
#define AMHW_FSL_USB_ERRSTAT_BTOERR(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_BTOERR_SHIFT)) & AMHW_FSL_USB_ERRSTAT_BTOERR_MASK)
#define AMHW_FSL_USB_ERRSTAT_DMAERR_MASK                  (0x20U)
#define AMHW_FSL_USB_ERRSTAT_DMAERR_SHIFT                 (5U)
#define AMHW_FSL_USB_ERRSTAT_DMAERR(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_DMAERR_SHIFT)) & AMHW_FSL_USB_ERRSTAT_DMAERR_MASK)
#define AMHW_FSL_USB_ERRSTAT_BTSERR_MASK                  (0x80U)
#define AMHW_FSL_USB_ERRSTAT_BTSERR_SHIFT                 (7U)
#define AMHW_FSL_USB_ERRSTAT_BTSERR(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERRSTAT_BTSERR_SHIFT)) & AMHW_FSL_USB_ERRSTAT_BTSERR_MASK)

/*! @name ERREN - Error Interrupt Enable register */
#define AMHW_FSL_USB_ERREN_PIDERREN_MASK                  (0x1U)
#define AMHW_FSL_USB_ERREN_PIDERREN_SHIFT                 (0U)
#define AMHW_FSL_USB_ERREN_PIDERREN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_PIDERREN_SHIFT)) & AMHW_FSL_USB_ERREN_PIDERREN_MASK)
#define AMHW_FSL_USB_ERREN_CRC5EOFEN_MASK                 (0x2U)
#define AMHW_FSL_USB_ERREN_CRC5EOFEN_SHIFT                (1U)
#define AMHW_FSL_USB_ERREN_CRC5EOFEN(x)                   (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_CRC5EOFEN_SHIFT)) & AMHW_FSL_USB_ERREN_CRC5EOFEN_MASK)
#define AMHW_FSL_USB_ERREN_CRC16EN_MASK                   (0x4U)
#define AMHW_FSL_USB_ERREN_CRC16EN_SHIFT                  (2U)
#define AMHW_FSL_USB_ERREN_CRC16EN(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_CRC16EN_SHIFT)) & AMHW_FSL_USB_ERREN_CRC16EN_MASK)
#define AMHW_FSL_USB_ERREN_DFN8EN_MASK                    (0x8U)
#define AMHW_FSL_USB_ERREN_DFN8EN_SHIFT                   (3U)
#define AMHW_FSL_USB_ERREN_DFN8EN(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_DFN8EN_SHIFT)) & AMHW_FSL_USB_ERREN_DFN8EN_MASK)
#define AMHW_FSL_USB_ERREN_BTOERREN_MASK                  (0x10U)
#define AMHW_FSL_USB_ERREN_BTOERREN_SHIFT                 (4U)
#define AMHW_FSL_USB_ERREN_BTOERREN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_BTOERREN_SHIFT)) & AMHW_FSL_USB_ERREN_BTOERREN_MASK)
#define AMHW_FSL_USB_ERREN_DMAERREN_MASK                  (0x20U)
#define AMHW_FSL_USB_ERREN_DMAERREN_SHIFT                 (5U)
#define AMHW_FSL_USB_ERREN_DMAERREN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_DMAERREN_SHIFT)) & AMHW_FSL_USB_ERREN_DMAERREN_MASK)
#define AMHW_FSL_USB_ERREN_BTSERREN_MASK                  (0x80U)
#define AMHW_FSL_USB_ERREN_BTSERREN_SHIFT                 (7U)
#define AMHW_FSL_USB_ERREN_BTSERREN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ERREN_BTSERREN_SHIFT)) & AMHW_FSL_USB_ERREN_BTSERREN_MASK)

/*! @name STAT - Status register */
#define AMHW_FSL_USB_STAT_ODD_MASK                        (0x4U)
#define AMHW_FSL_USB_STAT_ODD_SHIFT                       (2U)
#define AMHW_FSL_USB_STAT_ODD(x)                          (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_STAT_ODD_SHIFT)) & AMHW_FSL_USB_STAT_ODD_MASK)
#define AMHW_FSL_USB_STAT_TX_MASK                         (0x8U)
#define AMHW_FSL_USB_STAT_TX_SHIFT                        (3U)
#define AMHW_FSL_USB_STAT_TX(x)                           (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_STAT_TX_SHIFT)) & AMHW_FSL_USB_STAT_TX_MASK)
#define AMHW_FSL_USB_STAT_ENDP_MASK                       (0xF0U)
#define AMHW_FSL_USB_STAT_ENDP_SHIFT                      (4U)
#define AMHW_FSL_USB_STAT_ENDP(x)                         (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_STAT_ENDP_SHIFT)) & AMHW_FSL_USB_STAT_ENDP_MASK)

/*! @name CTL - Control register */
#define AMHW_FSL_USB_CTL_USBENSOFEN_MASK                  (0x1U)
#define AMHW_FSL_USB_CTL_USBENSOFEN_SHIFT                 (0U)
#define AMHW_FSL_USB_CTL_USBENSOFEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_USBENSOFEN_SHIFT)) & AMHW_FSL_USB_CTL_USBENSOFEN_MASK)
#define AMHW_FSL_USB_CTL_ODDRST_MASK                      (0x2U)
#define AMHW_FSL_USB_CTL_ODDRST_SHIFT                     (1U)
#define AMHW_FSL_USB_CTL_ODDRST(x)                        (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_ODDRST_SHIFT)) & AMHW_FSL_USB_CTL_ODDRST_MASK)
#define AMHW_FSL_USB_CTL_RESUME_MASK                      (0x4U)
#define AMHW_FSL_USB_CTL_RESUME_SHIFT                     (2U)
#define AMHW_FSL_USB_CTL_RESUME(x)                        (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_RESUME_SHIFT)) & AMHW_FSL_USB_CTL_RESUME_MASK)
#define AMHW_FSL_USB_CTL_HOSTMODEEN_MASK                  (0x8U)
#define AMHW_FSL_USB_CTL_HOSTMODEEN_SHIFT                 (3U)
#define AMHW_FSL_USB_CTL_HOSTMODEEN(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_HOSTMODEEN_SHIFT)) & AMHW_FSL_USB_CTL_HOSTMODEEN_MASK)
#define AMHW_FSL_USB_CTL_RESET_MASK                       (0x10U)
#define AMHW_FSL_USB_CTL_RESET_SHIFT                      (4U)
#define AMHW_FSL_USB_CTL_RESET(x)                         (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_RESET_SHIFT)) & AMHW_FSL_USB_CTL_RESET_MASK)
#define AMHW_FSL_USB_CTL_TXSUSPENDTOKENBUSY_MASK          (0x20U)
#define AMHW_FSL_USB_CTL_TXSUSPENDTOKENBUSY_SHIFT         (5U)
#define AMHW_FSL_USB_CTL_TXSUSPENDTOKENBUSY(x)            (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_TXSUSPENDTOKENBUSY_SHIFT)) & AMHW_FSL_USB_CTL_TXSUSPENDTOKENBUSY_MASK)
#define AMHW_FSL_USB_CTL_SE0_MASK                         (0x40U)
#define AMHW_FSL_USB_CTL_SE0_SHIFT                        (6U)
#define AMHW_FSL_USB_CTL_SE0(x)                           (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_SE0_SHIFT)) & AMHW_FSL_USB_CTL_SE0_MASK)
#define AMHW_FSL_USB_CTL_JSTATE_MASK                      (0x80U)
#define AMHW_FSL_USB_CTL_JSTATE_SHIFT                     (7U)
#define AMHW_FSL_USB_CTL_JSTATE(x)                        (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CTL_JSTATE_SHIFT)) & AMHW_FSL_USB_CTL_JSTATE_MASK)

/*! @name ADDR - Address register */
#define AMHW_FSL_USB_ADDR_ADDR_MASK                       (0x7FU)
#define AMHW_FSL_USB_ADDR_ADDR_SHIFT                      (0U)
#define AMHW_FSL_USB_ADDR_ADDR(x)                         (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ADDR_ADDR_SHIFT)) & AMHW_FSL_USB_ADDR_ADDR_MASK)
#define AMHW_FSL_USB_ADDR_LSEN_MASK                       (0x80U)
#define AMHW_FSL_USB_ADDR_LSEN_SHIFT                      (7U)
#define AMHW_FSL_USB_ADDR_LSEN(x)                         (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ADDR_LSEN_SHIFT)) & AMHW_FSL_USB_ADDR_LSEN_MASK)

/*! @name BDTPAGE1 - BDT Page register 1 */
#define AMHW_FSL_USB_BDTPAGE1_BDTBA_MASK                  (0xFEU)
#define AMHW_FSL_USB_BDTPAGE1_BDTBA_SHIFT                 (1U)
#define AMHW_FSL_USB_BDTPAGE1_BDTBA(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_BDTPAGE1_BDTBA_SHIFT)) & AMHW_FSL_USB_BDTPAGE1_BDTBA_MASK)

/*! @name FRMNUML - Frame Number register Low */
#define AMHW_FSL_USB_FRMNUML_FRM_MASK                     (0xFFU)
#define AMHW_FSL_USB_FRMNUML_FRM_SHIFT                    (0U)
#define AMHW_FSL_USB_FRMNUML_FRM(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_FRMNUML_FRM_SHIFT)) & AMHW_FSL_USB_FRMNUML_FRM_MASK)

/*! @name FRMNUMH - Frame Number register High */
#define AMHW_FSL_USB_FRMNUMH_FRM_MASK                     (0x7U)
#define AMHW_FSL_USB_FRMNUMH_FRM_SHIFT                    (0U)
#define AMHW_FSL_USB_FRMNUMH_FRM(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_FRMNUMH_FRM_SHIFT)) & AMHW_FSL_USB_FRMNUMH_FRM_MASK)

/*! @name TOKEN - Token register */
#define AMHW_FSL_USB_TOKEN_TOKENENDPT_MASK                (0xFU)
#define AMHW_FSL_USB_TOKEN_TOKENENDPT_SHIFT               (0U)
#define AMHW_FSL_USB_TOKEN_TOKENENDPT(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_TOKEN_TOKENENDPT_SHIFT)) & AMHW_FSL_USB_TOKEN_TOKENENDPT_MASK)
#define AMHW_FSL_USB_TOKEN_TOKENPID_MASK                  (0xF0U)
#define AMHW_FSL_USB_TOKEN_TOKENPID_SHIFT                 (4U)
#define AMHW_FSL_USB_TOKEN_TOKENPID(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_TOKEN_TOKENPID_SHIFT)) & AMHW_FSL_USB_TOKEN_TOKENPID_MASK)

/*! @name SOFTHLD - SOF Threshold register */
#define AMHW_FSL_USB_SOFTHLD_CNT_MASK                     (0xFFU)
#define AMHW_FSL_USB_SOFTHLD_CNT_SHIFT                    (0U)
#define AMHW_FSL_USB_SOFTHLD_CNT(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_SOFTHLD_CNT_SHIFT)) & AMHW_FSL_USB_SOFTHLD_CNT_MASK)

/*! @name BDTPAGE2 - BDT Page Register 2 */
#define AMHW_FSL_USB_BDTPAGE2_BDTBA_MASK                  (0xFFU)
#define AMHW_FSL_USB_BDTPAGE2_BDTBA_SHIFT                 (0U)
#define AMHW_FSL_USB_BDTPAGE2_BDTBA(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_BDTPAGE2_BDTBA_SHIFT)) & AMHW_FSL_USB_BDTPAGE2_BDTBA_MASK)

/*! @name BDTPAGE3 - BDT Page Register 3 */
#define AMHW_FSL_USB_BDTPAGE3_BDTBA_MASK                  (0xFFU)
#define AMHW_FSL_USB_BDTPAGE3_BDTBA_SHIFT                 (0U)
#define AMHW_FSL_USB_BDTPAGE3_BDTBA(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_BDTPAGE3_BDTBA_SHIFT)) & AMHW_FSL_USB_BDTPAGE3_BDTBA_MASK)

/*! @name ENDPT - Endpoint Control register */
#define AMHW_FSL_USB_ENDPT_EPHSHK_MASK                    (0x1U)
#define AMHW_FSL_USB_ENDPT_EPHSHK_SHIFT                   (0U)
#define AMHW_FSL_USB_ENDPT_EPHSHK(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_EPHSHK_SHIFT)) & AMHW_FSL_USB_ENDPT_EPHSHK_MASK)
#define AMHW_FSL_USB_ENDPT_EPSTALL_MASK                   (0x2U)
#define AMHW_FSL_USB_ENDPT_EPSTALL_SHIFT                  (1U)
#define AMHW_FSL_USB_ENDPT_EPSTALL(x)                     (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_EPSTALL_SHIFT)) & AMHW_FSL_USB_ENDPT_EPSTALL_MASK)
#define AMHW_FSL_USB_ENDPT_EPTXEN_MASK                    (0x4U)
#define AMHW_FSL_USB_ENDPT_EPTXEN_SHIFT                   (2U)
#define AMHW_FSL_USB_ENDPT_EPTXEN(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_EPTXEN_SHIFT)) & AMHW_FSL_USB_ENDPT_EPTXEN_MASK)
#define AMHW_FSL_USB_ENDPT_EPRXEN_MASK                    (0x8U)
#define AMHW_FSL_USB_ENDPT_EPRXEN_SHIFT                   (3U)
#define AMHW_FSL_USB_ENDPT_EPRXEN(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_EPRXEN_SHIFT)) & AMHW_FSL_USB_ENDPT_EPRXEN_MASK)
#define AMHW_FSL_USB_ENDPT_EPCTLDIS_MASK                  (0x10U)
#define AMHW_FSL_USB_ENDPT_EPCTLDIS_SHIFT                 (4U)
#define AMHW_FSL_USB_ENDPT_EPCTLDIS(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_EPCTLDIS_SHIFT)) & AMHW_FSL_USB_ENDPT_EPCTLDIS_MASK)
#define AMHW_FSL_USB_ENDPT_RETRYDIS_MASK                  (0x40U)
#define AMHW_FSL_USB_ENDPT_RETRYDIS_SHIFT                 (6U)
#define AMHW_FSL_USB_ENDPT_RETRYDIS(x)                    (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_RETRYDIS_SHIFT)) & AMHW_FSL_USB_ENDPT_RETRYDIS_MASK)
#define AMHW_FSL_USB_ENDPT_HOSTWOHUB_MASK                 (0x80U)
#define AMHW_FSL_USB_ENDPT_HOSTWOHUB_SHIFT                (7U)
#define AMHW_FSL_USB_ENDPT_HOSTWOHUB(x)                   (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_ENDPT_HOSTWOHUB_SHIFT)) & AMHW_FSL_USB_ENDPT_HOSTWOHUB_MASK)

/* The count of USB_ENDPT */
#define AMHW_FSL_USB_ENDPT_COUNT                          (16U)

/*! @name USBCTRL - USB Control register */
#define AMHW_FSL_USB_USBCTRL_PDE_MASK                     (0x40U)
#define AMHW_FSL_USB_USBCTRL_PDE_SHIFT                    (6U)
#define AMHW_FSL_USB_USBCTRL_PDE(x)                       (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBCTRL_PDE_SHIFT)) & AMHW_FSL_USB_USBCTRL_PDE_MASK)
#define AMHW_FSL_USB_USBCTRL_SUSP_MASK                    (0x80U)
#define AMHW_FSL_USB_USBCTRL_SUSP_SHIFT                   (7U)
#define AMHW_FSL_USB_USBCTRL_SUSP(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBCTRL_SUSP_SHIFT)) & AMHW_FSL_USB_USBCTRL_SUSP_MASK)

/*! @name OBSERVE - USB OTG Observe register */
#define AMHW_FSL_USB_OBSERVE_DMPD_MASK                    (0x10U)
#define AMHW_FSL_USB_OBSERVE_DMPD_SHIFT                   (4U)
#define AMHW_FSL_USB_OBSERVE_DMPD(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OBSERVE_DMPD_SHIFT)) & AMHW_FSL_USB_OBSERVE_DMPD_MASK)
#define AMHW_FSL_USB_OBSERVE_DPPD_MASK                    (0x40U)
#define AMHW_FSL_USB_OBSERVE_DPPD_SHIFT                   (6U)
#define AMHW_FSL_USB_OBSERVE_DPPD(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OBSERVE_DPPD_SHIFT)) & AMHW_FSL_USB_OBSERVE_DPPD_MASK)
#define AMHW_FSL_USB_OBSERVE_DPPU_MASK                    (0x80U)
#define AMHW_FSL_USB_OBSERVE_DPPU_SHIFT                   (7U)
#define AMHW_FSL_USB_OBSERVE_DPPU(x)                      (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_OBSERVE_DPPU_SHIFT)) & AMHW_FSL_USB_OBSERVE_DPPU_MASK)

/*! @name CONTROL - USB OTG Control register */
#define AMHW_FSL_USB_CONTROL_DPPULLUPNONOTG_MASK          (0x10U)
#define AMHW_FSL_USB_CONTROL_DPPULLUPNONOTG_SHIFT         (4U)
#define AMHW_FSL_USB_CONTROL_DPPULLUPNONOTG(x)            (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_CONTROL_DPPULLUPNONOTG_SHIFT)) & AMHW_FSL_USB_CONTROL_DPPULLUPNONOTG_MASK)

/*! @name USBTRC0 - USB Transceiver Control register 0 */
#define AMHW_FSL_USB_USBTRC0_USB_RESUME_INT_MASK          (0x1U)
#define AMHW_FSL_USB_USBTRC0_USB_RESUME_INT_SHIFT         (0U)
#define AMHW_FSL_USB_USBTRC0_USB_RESUME_INT(x)            (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBTRC0_USB_RESUME_INT_SHIFT)) & AMHW_FSL_USB_USBTRC0_USB_RESUME_INT_MASK)
#define AMHW_FSL_USB_USBTRC0_SYNC_DET_MASK                (0x2U)
#define AMHW_FSL_USB_USBTRC0_SYNC_DET_SHIFT               (1U)
#define AMHW_FSL_USB_USBTRC0_SYNC_DET(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBTRC0_SYNC_DET_SHIFT)) & AMHW_FSL_USB_USBTRC0_SYNC_DET_MASK)
#define AMHW_FSL_USB_USBTRC0_USBRESMEN_MASK               (0x20U)
#define AMHW_FSL_USB_USBTRC0_USBRESMEN_SHIFT              (5U)
#define AMHW_FSL_USB_USBTRC0_USBRESMEN(x)                 (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBTRC0_USBRESMEN_SHIFT)) & AMHW_FSL_USB_USBTRC0_USBRESMEN_MASK)
#define AMHW_FSL_USB_USBTRC0_USBRESET_MASK                (0x80U)
#define AMHW_FSL_USB_USBTRC0_USBRESET_SHIFT               (7U)
#define AMHW_FSL_USB_USBTRC0_USBRESET(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBTRC0_USBRESET_SHIFT)) & AMHW_FSL_USB_USBTRC0_USBRESET_MASK)

/*! @name USBFRMADJUST - Frame Adjust Register */
#define AMHW_FSL_USB_USBFRMADJUST_ADJ_MASK                (0xFFU)
#define AMHW_FSL_USB_USBFRMADJUST_ADJ_SHIFT               (0U)
#define AMHW_FSL_USB_USBFRMADJUST_ADJ(x)                  (((uint8_t)(((uint8_t)(x)) << AMHW_FSL_USB_USBFRMADJUST_ADJ_SHIFT)) & AMHW_FSL_USB_USBFRMADJUST_ADJ_MASK)

/**
 * @} amhw_fsl_if_usb
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_FSL_USB_H */


/* end of file */
