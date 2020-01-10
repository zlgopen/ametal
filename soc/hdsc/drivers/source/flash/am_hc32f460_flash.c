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
 * \brief Flash模块的驱动层实现
 *
 * \internal
 * \par Modification History
 * - 1.00 19-09-04  zp, first implementation
 * \endinternal
 */

#include "am_hc32f460_flash.h"
#include "am_types.h"
#include "am_bitops.h"
#include "am_int.h"
#include "ametal.h"


/*******************************************************************************
  defines
*******************************************************************************/


 void am_hc32f460_flash_lock(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_lock(p_hw_flash);
}

 void am_hc32f460_flash_unlock(amhw_hc32f460_flash_t *p_hw_flash)
{
    amhw_hc32f460_flash_unlock(p_hw_flash);
}



void am_hc32f460_flash_waitcycle(amhw_hc32f460_flash_t *p_hw_flash,
                                 amhw_hc32f460_flash_read_waittime waitcycle)
{
    amhw_hc32f460_flash_waitcycle(p_hw_flash, waitcycle);
}



/* end of file */
