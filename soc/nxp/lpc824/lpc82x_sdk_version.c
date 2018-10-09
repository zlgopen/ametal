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
 * \brief  SDK Version source
 *
 * \internal
 * \par modification history:
 * - 1.00 17-07-26  nwt, copy from AWorks
 * \endinternal
 */

#include "ametal.h"

/*******************************************************************************
  locals
*******************************************************************************/
#ifndef SDK_VERSION

#define SDK_VERSION      "1.0.4<2017-07-26>"

#endif

/*  sdk version */
static const char *p_version = SDK_VERSION;

/*******************************************************************************
  implementation
*******************************************************************************/

/**
 * \brief get version information
 */
const char * am_sdk_version_get (void)
{
    return p_version;
}

/* end of file */

