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
/** \file usbd_usr.h
 **
 ** A detailed description is available at
 ** @link Header file for usbd_usr.c @endlink
 **
 **   - 2018-12-26  1.0  wangmin First version for USB demo.
 **
 ******************************************************************************/
#ifndef __USBD_USR_H__
#define __USBD_USR_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "usbd_core.h"

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
extern USBD_Usr_cb_TypeDef USR_cb;
extern USBD_Usr_cb_TypeDef USR_FS_cb;
extern USBD_Usr_cb_TypeDef USR_HS_cb;

/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
void USBD_USR_Init(void);
void USBD_USR_DeviceReset (uint8_t speed);
void USBD_USR_DeviceConfigured (void);
void USBD_USR_DeviceSuspended(void);
void USBD_USR_DeviceResumed(void);

void USBD_USR_DeviceConnected(void);
void USBD_USR_DeviceDisconnected(void);

void USBD_USR_FS_Init(void);
void USBD_USR_FS_DeviceReset (uint8_t speed);
void USBD_USR_FS_DeviceConfigured (void);
void USBD_USR_FS_DeviceSuspended(void);
void USBD_USR_FS_DeviceResumed(void);

void USBD_USR_FS_DeviceConnected(void);
void USBD_USR_FS_DeviceDisconnected(void);

void USBD_USR_HS_Init(void);
void USBD_USR_HS_DeviceReset (uint8_t speed);
void USBD_USR_HS_DeviceConfigured (void);
void USBD_USR_HS_DeviceSuspended(void);
void USBD_USR_HS_DeviceResumed(void);

void USBD_USR_HS_DeviceConnected(void);
void USBD_USR_HS_DeviceDisconnected(void);

#endif /*__USBD_USR_H__*/

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
