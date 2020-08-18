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
 * \brief ZLG118 ResetHandler code for GCC compiler
 *
 * \internal
 * \par Modification History
 * - 1.00 18-11-09  htf, first implementation
 * \endinternal
 */

  .word  _estack

.section  .text.ResetHandler
.type    ResetHandler, %function
.global  ResetHandler

ResetHandler:
    //关中断
    CPSID  I
    //初始化SP指针
    ldr    r0, = _estack
    msr    msp,r0
    //调用初始化函数
    bl     ResetHandler_function
    b      .

