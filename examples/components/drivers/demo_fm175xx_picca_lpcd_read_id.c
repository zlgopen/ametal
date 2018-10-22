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
 * \brief FM175xx LPCD模式
 *
 * - 操作步骤：
 *   1. 正确连接并配置好串口。
 *   2. 正确连接好天线。
 *
 * - 实验现象：
 *   1. 在执行操作步骤1 2后， 随后卡进入低功耗自动卡片检测（LPCD）模式
 *   2. 将卡放置感应区，FM175xx设备将产生卡进场中断标志，执行相应设定操作。
 *   3. 移开卡片后，达到设定的AUTO_WUP_TIME的时间时，FM175xx自动退出LPCD模式，自动进行调校。
 *
 * \par 源代码
 * \snippet demo_fm175xx_picca_lpcd_read_id.c src_fm175xx_picca_lpcd_read_id
 *
 * \internal
 * \par Modification history
 * - 1.00 18-08-20  htf, first implementation.
 * \endinternal
 */

/**
 * \addtogroup demo_if_fm175xx_picca_lpcd_read_id
 * \copydoc demo_fm175xx_picca_lpcd_read_id.c
 */

/** [src_fm175xx_picca_lpcd_read_id] */
#include "am_fm175xx.h"
#include "am_fm175xx_reg.h"
#include "am_vdebug.h"
#include "am_delay.h"

/**
 * \brief 触发卡进场回调函数  用户可自己任意配置   本例程以读取ID为例
 */
int8_t  picca_active_info(am_fm175xx_handle_t handle)
{
    uint8_t      tag_type[2]  = { 0 };      /* ATQA */
    uint8_t      uid[10]      = { 0 };      /* UID */
    uint8_t      uid_real_len =   0;        /* 接收到的UID的长度 */
    uint8_t      sak[3]       = { 0 };      /* SAK */
    uint8_t      i;

    /* 卡激活操作  */
    if (AM_FM175XX_STATUS_SUCCESS == am_fm175xx_picca_active\
                                              (handle,
                                               AM_FM175XX_PICCA_REQ_ALL,
                                               tag_type,
                                               uid,
                                               &uid_real_len,
                                               sak)) {
        am_kprintf("ATQA :%02x %02x\n", tag_type[0], tag_type[1]);

        /* 打印UID */
        am_kprintf("UID  :");
        for (i = 0; i < uid_real_len; i++) {
            am_kprintf("%02x ", uid[i]);
        }
        am_kprintf("\n");

        /* 打印SAK */
        am_kprintf("SAK  :");
        for (i = 4; i <= uid_real_len; i+=3) {
            am_kprintf("%02x ", sak[(i - 4) / 3]);
        }
        am_kprintf("\n\n");
        return AM_OK;
    }else{
        return AM_ERROR;
    }
}

/**
 * \brief 用户回调函数
 *
 *  触发中断之后将会自动退出LPCD模式  再次进入用户需调用am_fm175xx_lpcd_mode_entry
 *  重新进入LPCD模式
 */
static void  __fm175xx_lpcd_cb(void *p_arg)
{
    int *p_flag = (int *)p_arg;
    *p_flag =1;
}

/**
 * \brief LPCD模式
 */
void demo_fm175xx_picca_lpcd_mode (am_fm175xx_handle_t handle)
{
    int int_flag = 0;

    /* 设置回调函数及函数变量*/
    am_fm175xx_lpcd_cb_set(handle, __fm175xx_lpcd_cb, &int_flag);

    /* 进入LPCD模式 */
    am_fm175xx_lpcd_mode_entry(handle);

    while(1){

        am_mdelay(10);

        if(int_flag == 1){
            int_flag = 0;

            /*可在此进行相应的处理函数   本例程以读取ID号为例*/
            picca_active_info(handle);

            /* 重新进入LPCD模式 */
            am_fm175xx_lpcd_mode_entry(handle);
        }
    }
}

/** [src_fm175xx_picca_lpcd_read_id] */

/* end of file */
