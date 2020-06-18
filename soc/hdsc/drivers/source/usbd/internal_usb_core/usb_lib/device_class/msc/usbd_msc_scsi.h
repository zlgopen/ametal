/*****************************************************************************
 * Copyright (C) 2016, Huada Semiconductor Co.,Ltd All rights reserved.
 *
 * This software is owned and published by:
 * Huada Semiconductor Co.,Ltd ("HDSC").
 *
 * BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
 * BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
 *
 * This software contains source code for use with HDSC
 * components. This software is licensed by HDSC to be adapted only
 * for use in systems utilizing HDSC components. HDSC shall not be
 * responsible for misuse or illegal use of this software for devices not
 * supported herein. HDSC is providing this software "AS IS" and will
 * not be responsible for issues arising from incorrect user implementation
 * of the software.
 *
 * Disclaimer:
 * HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
 * REGARDING THE SOFTWARE (INCLUDING ANY ACCOMPANYING WRITTEN MATERIALS),
 * ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
 * WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
 * WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
 * WARRANTY OF NONINFRINGEMENT.
 * HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
 * NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
 * LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
 * LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
 * INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
 * SAVINGS OR PROFITS,
 * EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
 * INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
 * FROM, THE SOFTWARE.
 *
 * This software may be replicated in part or whole for the licensed use,
 * with the restriction that this Disclaimer and Copyright notice must be
 * included with each copy of this software, whether used in part or whole,
 * at all times.
 */
/******************************************************************************/
/** \file usbd_msc_scsi.h
 **
 ** A detailed description is available at
 ** @link header file for the usbd_msc_scsi.c @endlink
 **
 **   - 2019-05-15  1.0  Zhangxl First version for USB MSC device demo.
 **
 ******************************************************************************/
#ifndef __USBD_MSC_SCSI_H__
#define __USBD_MSC_SCSI_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_def.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
typedef struct _SENSE_ITEM
{
    char Skey;
    union
    {
        struct _ASCs
        {
            char ASC;
            char ASCQ;
        }b;
        unsigned int ASC;
        char *       pData;
    } w;
} SCSI_Sense_TypeDef;

/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SENSE_LIST_DEEPTH                           (4u)

/* SCSI Commands */
#define SCSI_FORMAT_UNIT                            (0x04u)
#define SCSI_INQUIRY                                (0x12u)
#define SCSI_MODE_SELECT6                           (0x15u)
#define SCSI_MODE_SELECT10                          (0x55u)
#define SCSI_MODE_SENSE6                            (0x1Au)
#define SCSI_MODE_SENSE10                           (0x5Au)
#define SCSI_ALLOW_MEDIUM_REMOVAL                   (0x1Eu)
#define SCSI_READ6                                  (0x08u)
#define SCSI_READ10                                 (0x28u)
#define SCSI_READ12                                 (0xA8u)
#define SCSI_READ16                                 (0x88u)

#define SCSI_READ_CAPACITY10                        (0x25u)
#define SCSI_READ_CAPACITY16                        (0x9Eu)

#define SCSI_REQUEST_SENSE                          (0x03u)
#define SCSI_START_STOP_UNIT                        (0x1Bu)
#define SCSI_TEST_UNIT_READY                        (0x00u)
#define SCSI_WRITE6                                 (0x0Au)
#define SCSI_WRITE10                                (0x2Au)
#define SCSI_WRITE12                                (0xAAu)
#define SCSI_WRITE16                                (0x8Au)

#define SCSI_VERIFY10                               (0x2Fu)
#define SCSI_VERIFY12                               (0xAFu)
#define SCSI_VERIFY16                               (0x8Fu)

#define SCSI_SEND_DIAGNOSTIC                        (0x1Du)
#define SCSI_READ_FORMAT_CAPACITIES                 (0x23u)

#define NO_SENSE                                    (0u)
#define RECOVERED_ERROR                             (1u)
#define NOT_READY                                   (2u)
#define MEDIUM_ERROR                                (3u)
#define HARDWARE_ERROR                              (4u)
#define ILLEGAL_REQUEST                             (5u)
#define UNIT_ATTENTION                              (6u)
#define DATA_PROTECT                                (7u)
#define BLANK_CHECK                                 (8u)
#define VENDOR_SPECIFIC                             (9u)
#define COPY_ABORTED                                (10u)
#define ABORTED_COMMAND                             (11u)
#define VOLUME_OVERFLOW                             (13u)
#define MISCOMPARE                                  (14u)

#define INVALID_CDB                                 (0x20u)
#define INVALID_FIELED_IN_COMMAND                   (0x24u)
#define PARAMETER_LIST_LENGTH_ERROR                 (0x1Au)
#define INVALID_FIELD_IN_PARAMETER_LIST             (0x26u)
#define ADDRESS_OUT_OF_RANGE                        (0x21u)
#define MEDIUM_NOT_PRESENT                          (0x3Au)
#define MEDIUM_HAVE_CHANGED                         (0x28u)
#define WRITE_PROTECTED                             (0x27u)
#define UNRECOVERED_READ_ERROR                      (0x11u)
#define WRITE_FAULT                                 (0x03u)

#define READ_FORMAT_CAPACITY_DATA_LEN               (0x0Cu)
#define READ_CAPACITY10_DATA_LEN                    (0x08u)
#define MODE_SENSE10_DATA_LEN                       (0x08u)
#define MODE_SENSE6_DATA_LEN                        (0x04u)
#define REQUEST_SENSE_DATA_LEN                      (0x12u)
#define STANDARD_INQUIRY_DATA_LEN                   (0x24u)
#define BLKVFY                                      (0x04u)

/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
extern SCSI_Sense_TypeDef SCSI_Sense [SENSE_LIST_DEEPTH];
extern uint8_t SCSI_Sense_Head;
extern uint8_t SCSI_Sense_Tail;

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
int8_t SCSI_ProcessCmd(USB_OTG_CORE_HANDLE  *pdev,
                       uint8_t lun,
                       uint8_t *cmd);

void   SCSI_SenseCode(uint8_t lun,
                      uint8_t sKey,
                      uint8_t ASC);

#endif /* __USBD_MSC_SCSI_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
