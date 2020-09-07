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
 * \brief DCU 底层操作接口
 * \internal
 * \par Modification History
 * - 1.00 20-05-26
 * \endinternal
 */

#ifndef __AMHW_HC32F460_DCU_H
#define __AMHW_HC32F460_DCU_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "am_types.h"

/**
 * \addtogroup amhw_hc32f460_if_dcu
 * \copydoc amhw_hc32f460_dcu.h
 * @{
 */

/**
 * \brief 使用匿名联合体段开始
 * @{
 */

#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)

/* 默认使能匿名联合体 */
#elif defined(__TMS470__)

/* 默认使能匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler t
#endif

/** @} */

/**
 * \brief DCU控制寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t MODE                      : 3;  /** <brief 动作模式 */
    __IO uint32_t DATASIZE                  : 2;  /** <brief 数据大小 */
    uint32_t RESERVED5                      : 3;  /** <brief 保留 */
    __IO uint32_t COMP_TRG                  : 1;  /** <brief 比较模式触发比较的时机 */
    uint32_t RESERVED9                      :22;  /** <brief 保留 */
    __IO uint32_t INTEN                     : 1;  /** <brief 中断使能 */
} stc_dcu_ctl_field_t;

/**
 * \brief DCU标志寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t FLAG_OP                   : 1;  /** <brief 运算标志位 */
    __IO uint32_t FLAG_LS2                  : 1;  /** <brief 小于标志位2 */
    __IO uint32_t FLAG_EQ2                  : 1;  /** <brief 等于标志位2 */
    __IO uint32_t FLAG_GT2                  : 1;  /** <brief 大于标志位2 */
    __IO uint32_t FLAG_LS1                  : 1;  /** <brief 小于标志位1 */
    __IO uint32_t FLAG_EQ1                  : 1;  /** <brief 等于标志位1 */
    __IO uint32_t FLAG_GT1                  : 1;  /** <brief 大于标志位1 */
    uint32_t RESERVED7                      :25;  /** <brief 保留 */
} stc_dcu_flag_field_t;

/**
 * \brief DCU标志复位寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t CLR_OP                    : 1;  /** <brief 清除运算标志位 */
    __IO uint32_t CLR_LS2                   : 1;  /** <brief 清除小于标志位2 */
    __IO uint32_t CLR_EQ2                   : 1;  /** <brief 清除等于标志位2 */
    __IO uint32_t CLR_GT2                   : 1;  /** <brief 清除大于标志位2 */
    __IO uint32_t CLR_LS1                   : 1;  /** <brief 清除小于标志位1 */
    __IO uint32_t CLR_EQ1                   : 1;  /** <brief 清除等于标志位1 */
    __IO uint32_t CLR_GT1                   : 1;  /** <brief 清除大于标志位1 */
    uint32_t RESERVED7                      :25;  /** <brief 保留 */
} stc_dcu_flagclr_field_t;

/**
 * \brief DCU标志复位寄存器位域结构体
 */
typedef struct
{
    __IO uint32_t INT_OP                    : 1;  /** <brief 运算标志位 */
    __IO uint32_t INT_LS2                   : 1;  /** <brief 小于中断条件选择2 */
    __IO uint32_t INT_EQ2                   : 1;  /** <brief 等于中断条件选择2 */
    __IO uint32_t INT_GT2                   : 1;  /** <brief 大于中断条件选择2 */
    __IO uint32_t INT_LS1                   : 1;  /** <brief 小于中断条件选择1 */
    __IO uint32_t INT_EQ1                   : 1;  /** <brief 等于中断条件选择1 */
    __IO uint32_t INT_GT1                   : 1;  /** <brief 大于中断条件选择1 */
    __IO uint32_t INT_WIN                   : 2;  /** <brief 窗口比较中断条件选择 */
    uint32_t RESERVED9                      :23;  /** <brief 保留 */
} stc_dcu_intsel_field_t;

/**
 * \brief DCU - 寄存器组
 */
typedef struct amhw_hc32f460_dcu {
    union
    {
        __IO uint32_t CTL;          /**< \brief DCU0控制寄存器 */
        stc_dcu_ctl_field_t CTL_f;  /**< \brief DCU0标志寄存器 */
    };
    union
    {
        __IO uint32_t FLAG;         /**< \brief DCU0标志寄存器 */
        stc_dcu_flag_field_t FLAG_f;
    };
    __IO uint32_t DATA0;            /**< \brief DCU0数据寄存器0 */
    __IO uint32_t DATA1;            /**< \brief DCU0数据寄存器1 */
    __IO uint32_t DATA2;            /**< \brief DCU0数据寄存器2 */
    union
    {
        __IO uint32_t FLAGCLR;      /**< \brief DCU0标志复位寄存器 */
        stc_dcu_flagclr_field_t FLAGCLR_f;
    };
    union
    {
        __IO uint32_t INTSEL;       /**< \brief DCU0中断条件选择寄存器 */
        stc_dcu_intsel_field_t INTSEL_f;
    };
} amhw_hc32f460_dcu_t;


/**
 * \brief DCU 使能中断
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_int_enable (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    p_hw_dcu->CTL_f.INTEN = 1;
}

/**
 * \brief DCU 禁能中断
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_int_disable (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    p_hw_dcu->CTL_f.INTEN = 0;
}

/**
 * \brief DCU 比较模式触发比较的时机
 */
typedef enum {
    AMHW_HC32F460_DCU_COMPTRG_BY_DATA0   = 0,  /**< \brief 写入data0后触发比较 */
    AMHW_HC32F460_DCU_COMPTRG_BY_DATA012 = 1   /**< \brief 写入data0/data1/data2后触发比较 */
} amhw_hc32f460_dcu_comptrg_t;

/**
 * \brief DCU 设置比较模式触发的时机
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] flag     : 比较模式触发比较的时机（amhw_hc32f460_dcu_comptrg_t）
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_comptrg_set (amhw_hc32f460_dcu_t *p_hw_dcu, amhw_hc32f460_dcu_comptrg_t flag)
{
    p_hw_dcu->CTL_f.COMP_TRG = flag;
}


/**
 * \brief DCU 数据大小
 */
typedef enum {
    AMHW_HC32F460_DCU_DATASIZE_8BIT  = 0,  /**< \brief 8bit */
    AMHW_HC32F460_DCU_DATASIZE_16BIT = 1,  /**< \brief 16bit */
    AMHW_HC32F460_DCU_DATASIZE_32BIT = 2   /**< \brief 32bit */
} amhw_hc32f460_dcu_datasize_t;

/**
 * \brief DCU 设置数据大小
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] datasize : 数据大小标志（amhw_hc32f460_dcu_datasize_t）
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_datasize_set (amhw_hc32f460_dcu_t         *p_hw_dcu,
                                     amhw_hc32f460_dcu_datasize_t datasize)
{
    p_hw_dcu->CTL_f.DATASIZE = datasize;
}


/**
 * \brief DCU 动作模式
 */
typedef enum {
    AMHW_HC32F460_DCU_OPERATION_MODE_INVALID   = 0,  /**< \brief DCU无效 */
    AMHW_HC32F460_DCU_OPERATION_MODE_ADD       = 1,  /**< \brief 加法模式 */
    AMHW_HC32F460_DCU_OPERATION_MODE_SUB       = 2,  /**< \brief 减法模式 */
    AMHW_HC32F460_DCU_OPERATION_MODE_HWTRG_ADD = 3,  /**< \brief 硬件触发加法模式 */
    AMHW_HC32F460_DCU_OPERATION_MODE_HWTRG_SUB = 4,  /**< \brief 硬件触发减法模式 */
    AMHW_HC32F460_DCU_OPERATION_MODE_COMP      = 5   /**< \brief 比较模式 */
} amhw_hc32f460_dcu_operation_mode_t;


/**
 * \brief DCU 动作模式设置
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] mode     : DCU动作模式（amhw_hc32f460_dcu_operation_mode_t）
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_mode_set (amhw_hc32f460_dcu_t *p_hw_dcu, amhw_hc32f460_dcu_operation_mode_t mode)
{
    p_hw_dcu->CTL_f.MODE = mode;
}

/**
 * \brief DCU 结果标志
 */
typedef enum {
    AMHW_HC32F460_DCU_RESULT_FLAG_OP   = 1 << 0,  /**< \brief 运算溢出 */
    AMHW_HC32F460_DCU_RESULT_FLAG_lS2  = 1 << 1,  /**< \brief DATA0 < DATA2 */
    AMHW_HC32F460_DCU_RESULT_FLAG_EQ2  = 1 << 2,  /**< \brief DATA0 = DATA2 */
    AMHW_HC32F460_DCU_RESULT_FLAG_GT2  = 1 << 3,  /**< \brief DATA0 > DATA2 */
    AMHW_HC32F460_DCU_RESULT_FLAG_LS1  = 1 << 4,  /**< \brief DATA0 < DATA1 */
    AMHW_HC32F460_DCU_RESULT_FLAG_EQ1  = 1 << 5,  /**< \brief DATA0 = DATA1 */
    AMHW_HC32F460_DCU_RESULT_FLAG_GT1  = 1 << 6,  /**< \brief DATA0 > DATA1 */
} amhw_hc32f460_dcu_result_flag_t;

/**
 * \brief DCU 标志位检查
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] flag     : DCU标志（amhw_hc32f460_dcu_result_flag_t）
 *
 * \retval : 无
 */
am_static_inline
am_bool_t amhw_hc32f460_dcu_result_flag_check (amhw_hc32f460_dcu_t            *p_hw_dcu,
                                               amhw_hc32f460_dcu_result_flag_t flag)
{
    return (p_hw_dcu->FLAG & flag) ? AM_TRUE : AM_FALSE;
}

/**
 * \brief DCU 标志读取
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 标志寄存器值
 */
am_static_inline
uint32_t amhw_hc32f460_dcu_result_flag_get (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->FLAG;
}

/**
 * \brief DCU 标志位清除
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] flag     : DCU标志（amhw_hc32f460_dcu_result_flag_t）
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_result_flag_clr (amhw_hc32f460_dcu_t *p_hw_dcu, uint32_t flag)
{
    p_hw_dcu->FLAGCLR = flag;
}

/**
 * \brief DCU 写数据寄存器data0（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data0_write_8bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint8_t data)
{
    p_hw_dcu->DATA0 = data;
}

/**
 * \brief DCU 写数据寄存器data0（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data0_write_16bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint16_t data)
{
    p_hw_dcu->DATA0 = data;
}

/**
 * \brief DCU 写数据寄存器data0（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data0_write_32bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint32_t data)
{
    p_hw_dcu->DATA0 = data;
}

/**
 * \brief DCU 写数据寄存器data1（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data1_write_8bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint8_t data)
{
    p_hw_dcu->DATA1 = data;
}

/**
 * \brief DCU 写数据寄存器data1（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data1_write_16bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint16_t data)
{
    p_hw_dcu->DATA1 = data;
}

/**
 * \brief DCU 写数据寄存器data1（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data1_write_32bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint32_t data)
{
    p_hw_dcu->DATA1 = data;
}

/**
 * \brief DCU 写数据寄存器data2（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data2_write_8bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint8_t data)
{
    p_hw_dcu->DATA2 = data;
}

/**
 * \brief DCU 写数据寄存器data2（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data2_write_16bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint16_t data)
{
    p_hw_dcu->DATA2 = data;
}

/**
 * \brief DCU 写数据寄存器data2（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 * \param[in] data     : 要写入的数据
 *
 * \retval : 无
 */
am_static_inline
void amhw_hc32f460_dcu_data2_write_32bit (amhw_hc32f460_dcu_t *p_hw_dcu, uint32_t data)
{
    p_hw_dcu->DATA2 = data;
}


/**
 * \brief DCU 读取数据寄存器data0（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint8_t amhw_hc32f460_dcu_data0_read_8bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA0;
}

/**
 * \brief DCU 写数据寄存器data0（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint16_t amhw_hc32f460_dcu_data0_read_16bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA0;
}

/**
 * \brief DCU 写数据寄存器data0（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint32_t amhw_hc32f460_dcu_data0_read_32bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA0;
}

/**
 * \brief DCU 读取数据寄存器data1（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint8_t amhw_hc32f460_dcu_data1_read_8bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA1;
}

/**
 * \brief DCU 写数据寄存器data1（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint16_t amhw_hc32f460_dcu_data1_read_16bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA1;
}

/**
 * \brief DCU 写数据寄存器data1（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint32_t amhw_hc32f460_dcu_data1_read_32bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA1;
}

/**
 * \brief DCU 读取数据寄存器data2（8bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint8_t amhw_hc32f460_dcu_data2_read_8bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA2;
}

/**
 * \brief DCU 写数据寄存器data2（16bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint16_t amhw_hc32f460_dcu_data2_read_16bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA2;
}

/**
 * \brief DCU 写数据寄存器data2（32bit）
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
uint32_t amhw_hc32f460_dcu_data2_read_32bit (amhw_hc32f460_dcu_t *p_hw_dcu)
{
    return p_hw_dcu->DATA2;
}

/**
 * \brief DCU 中断条件
 */
typedef enum {
    AMHW_HC32F460_DCU_INT_OP          = 1 << 0,  /**< \brief 运算溢出 */
    AMHW_HC32F460_DCU_INT_lS2         = 1 << 1,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 < DATA2 */
    AMHW_HC32F460_DCU_INT_EQ2         = 1 << 2,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 = DATA2 */
    AMHW_HC32F460_DCU_INT_GT2         = 1 << 3,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 > DATA2 */
    AMHW_HC32F460_DCU_INT_LS1         = 1 << 4,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 < DATA1 */
    AMHW_HC32F460_DCU_INT_EQ1         = 1 << 5,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 = DATA1 */
    AMHW_HC32F460_DCU_INT_GT1         = 1 << 6,  /**< \brief 比较模式下，且窗口中断禁能时，DATA0 > DATA1 */
	AMHW_HC32F460_DCU_INT_WIN_INSIDE  = 1 << 7,  /**< \brief DATA2 <= DATA0 <= DATA1 */
	AMHW_HC32F460_DCU_INT_WIN_OUTSIDE = 2 << 7,  /**< \brief DATA0 > DATA1 或  DATA0 < DATA2*/
	AMHW_HC32F460_DCU_INT_CMP_INVALID = 3 << 7,  /**< \brief DATA0 > DATA1 */
} amhw_hc32f460_dcu_int_condition_t;

/**
 * \brief DCU 中断条件选择
 *
 * \param[in] p_hw_dcu : 指向DCU寄存器组指针
 *
 * \retval : 读取出的数据
 */
am_static_inline
void amhw_hc32f460_dcu_int_condition_set (amhw_hc32f460_dcu_t              *p_hw_dcu,
                                          amhw_hc32f460_dcu_int_condition_t flag)
{
    p_hw_dcu->INTSEL = flag;
}


#define HC32F460_DCU0_TRIGGER_SEL_REG_ADDR  (0x40010844)
#define HC32F460_DCU1_TRIGGER_SEL_REG_ADDR  (0x40010848)
#define HC32F460_DCU2_TRIGGER_SEL_REG_ADDR  (0x4001084c)
#define HC32F460_DCU3_TRIGGER_SEL_REG_ADDR  (0x40010850)

/**
 * \brief DCU 触发源选择
 *
 * \param[in] p_hw_dcu    : 指向DCU寄存器组指针
 * \param[in] trigger_src : 触发源事件编号
 *
 */
am_static_inline
void amhw_hc32f460_dcu_trigger_src_set (uint8_t dev_id, int trigger_src)
{
    switch (dev_id) {

    case 0: *(uint32_t *)HC32F460_DCU0_TRIGGER_SEL_REG_ADDR = trigger_src; break;
    case 1: *(uint32_t *)HC32F460_DCU1_TRIGGER_SEL_REG_ADDR = trigger_src; break;
    case 2: *(uint32_t *)HC32F460_DCU2_TRIGGER_SEL_REG_ADDR = trigger_src; break;
    case 3: *(uint32_t *)HC32F460_DCU3_TRIGGER_SEL_REG_ADDR = trigger_src; break;
    }
}

/**
 * \brief 使用匿名联合体段结束
 * @{
 */

#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)

/* 允许匿名联合体使能 */
#elif defined(__GNUC__)

/* 默认使用匿名联合体 */
#elif defined(__TMS470__)

/* 默认使用匿名联合体 */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler t
#endif
/** @} */

/**
 * @} amhw_if_hc32f460_dcu
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_HC32F460_DCU_H */

/* end of file */
