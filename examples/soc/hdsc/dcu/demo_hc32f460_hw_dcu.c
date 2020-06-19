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
 * \brief DCU 例程，通过 HW 层接口实现
 *
 * - 实验现象：串口打印 "add operation test success!"
 *                 "sub operation test success!"
 *                 "cmp operation test success!"
 *
 * \note
 *    1. 如需观察串口打印的调试信息，需要将 PIOE_4 引脚连接 PC 串口的 TXD，
 *       PIOE_5 引脚连接 PC 串口的 RXD；
 *
 * \par 源代码
 * \snippet demo_hc32f460_hw_dcu.c src_hc32f460_hw_dcu
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 20-05-27  cds, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_hc32f460_hw_dcu
 * \copydoc demo_hc32f460_hw_dcu.c
 */

/** [src_hc32f460_hw_dcu] */
#include "ametal.h"
#include "am_hc32f460.h"
#include "am_board.h"
#include "hw/amhw_hc32f460_dcu.h"

static uint32_t data_buffer_add[5] = {1,2,3,4,5};
static uint32_t data_sub1 = 12345678;
static uint32_t data_sub2[5] = {11111,22222,33333,44444,55555};
static uint32_t data_cmp_data0 = 5000;
static uint32_t data_cmp_data1 = 1000;
static uint32_t data_cmp_data2 = 10000;


int __hc32f460_dcu_add_test(amhw_hc32f460_dcu_t *p_hw_dcu,
                            uint32_t            *p_data,
                            uint32_t             data_length)
{
    uint32_t i = 0;

    uint32_t data_result = 0;

    amhw_hc32f460_dcu_datasize_set (p_hw_dcu, AMHW_HC32F460_DCU_DATASIZE_32BIT);

    amhw_hc32f460_dcu_mode_set (p_hw_dcu, AMHW_HC32F460_DCU_OPERATION_MODE_ADD);

    amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP);

    amhw_hc32f460_dcu_data0_write_32bit (p_hw_dcu, p_data[0]);

    for (i = 1; i < data_length; i++) {
        amhw_hc32f460_dcu_data1_write_32bit (p_hw_dcu, p_data[i]);
    }

    data_result = amhw_hc32f460_dcu_data0_read_32bit (p_hw_dcu);

    if (amhw_hc32f460_dcu_result_flag_check(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP)) {
        amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP);
        return -AM_EINVAL;
    } else {
        return data_result;
    }

}

int __hc32f460_dcu_sub_test(amhw_hc32f460_dcu_t *p_hw_dcu,
                            uint32_t             data1,
                            uint32_t            *p_data2,
                            uint32_t             data2_length)
{
    uint32_t i = 0;

    uint32_t data_result = 0;

    amhw_hc32f460_dcu_datasize_set (p_hw_dcu, AMHW_HC32F460_DCU_DATASIZE_32BIT);

    amhw_hc32f460_dcu_mode_set (p_hw_dcu, AMHW_HC32F460_DCU_OPERATION_MODE_SUB);

    amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP);

    amhw_hc32f460_dcu_data0_write_32bit (p_hw_dcu, data1);

    for (i = 0; i < data2_length; i++) {
        amhw_hc32f460_dcu_data1_write_32bit (p_hw_dcu, p_data2[i]);
    }

    data_result = amhw_hc32f460_dcu_data0_read_32bit (p_hw_dcu);

    if (amhw_hc32f460_dcu_result_flag_check(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP)) {
        amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, AMHW_HC32F460_DCU_RESULT_FLAG_OP);
        return -AM_EINVAL;
    } else {
        return data_result;
    }

}

uint32_t __hc32f460_dcu_cmp_test(amhw_hc32f460_dcu_t *p_hw_dcu,
                                 uint32_t             data0,
                                 uint32_t             data1,
                                 uint32_t             data2)
{

    uint32_t flag;

    amhw_hc32f460_dcu_datasize_set (p_hw_dcu, AMHW_HC32F460_DCU_DATASIZE_32BIT);

    amhw_hc32f460_dcu_mode_set (p_hw_dcu, AMHW_HC32F460_DCU_OPERATION_MODE_COMP);

    amhw_hc32f460_dcu_comptrg_set (p_hw_dcu, AMHW_HC32F460_DCU_COMPTRG_BY_DATA0);

    amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, AMHW_HC32F460_DCU_OPERATION_MODE_COMP);

    amhw_hc32f460_dcu_data1_write_32bit (p_hw_dcu, data1);
    amhw_hc32f460_dcu_data2_write_32bit (p_hw_dcu, data2);
    amhw_hc32f460_dcu_data0_write_32bit (p_hw_dcu, data0);


    flag = amhw_hc32f460_dcu_result_flag_get (p_hw_dcu);

    amhw_hc32f460_dcu_result_flag_clr(p_hw_dcu, flag);

    return flag;
}


/**
 * \brief DCU 例程，通过 HW 层接口实现
 */
void demo_hc32f460_hw_dcu_entry (void *p_hw_dcu)
{

    if (p_hw_dcu == NULL)
    {
        return ;
    }

    uint32_t result = 0;
    uint32_t value_test = 0;

    p_hw_dcu  = (amhw_hc32f460_dcu_t *)p_hw_dcu;

    result = __hc32f460_dcu_add_test(p_hw_dcu, data_buffer_add, sizeof(data_buffer_add) / 4);

    if (result == (data_buffer_add[0] + data_buffer_add[1]+data_buffer_add[2]+data_buffer_add[3]+data_buffer_add[4])) {
        am_kprintf(" add operation test success! \r\n");
    } else {
        am_kprintf(" add operation test failed! \r\n");
    }

    result = __hc32f460_dcu_sub_test(p_hw_dcu, data_sub1, data_sub2, sizeof(data_sub2) / 4);
    value_test = data_sub1 - data_sub2[0] - data_sub2[1] - data_sub2[2] - data_sub2[3] - data_sub2[4];
    if (result == value_test) {
        am_kprintf(" sub operation test success! \r\n");
    } else {
        am_kprintf(" sub operation test failed! \r\n");
    }

    uint32_t flag = 0;
    flag = __hc32f460_dcu_cmp_test(p_hw_dcu, data_cmp_data0, data_cmp_data1, data_cmp_data2);
    if ((flag & AMHW_HC32F460_DCU_RESULT_FLAG_GT1) && (flag & AMHW_HC32F460_DCU_RESULT_FLAG_lS2)) {
        am_kprintf(" cmp operation test success! \r\n");
    } else {
        am_kprintf(" cmp operation test failed! \r\n");
    }


}

/* end of file */
