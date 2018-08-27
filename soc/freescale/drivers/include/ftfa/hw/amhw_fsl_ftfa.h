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
 * \brief FLASH 内存模块硬件层接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-09-20  sdy, first implementation.
 * \endinternal
 */

#ifndef __AMHW_FSL_FTFA_H
#define __AMHW_FSL_FTFA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"
#include "am_bitops.h"
#include "am_common.h"

/**
 * \addtogroup amhw_fsl_if_ftfa
 * \copydoc amhw_fsl_ftfa.h
 * @{
 */

/******************************************************************************
    状态寄存器操作相关宏定义
******************************************************************************/

/** \brief 命令完成中断标志位 */
#define AMHW_FSL_FTFA_CCIF     (1UL << 7)

/** \brief FLASH读取冲突错误标志位 */
#define AMHW_FSL_FTFA_RDCOLERR (1UL << 6)

/** \brief FLASH操作权限错误标志位 */
#define AMHW_FSL_FTFA_ACCERR   (1UL << 5)

/** \brief 发现入侵被保护的FLASH标志位 */
#define AMHW_FSL_FTFA_FPVIOL   (1UL << 4)

/** \brief FLASH命令完成状态标志位 */
#define AMHW_FSL_FTFA_MGSTAT0  (1UL << 0)


/******************************************************************************
    配置寄存器操作相关宏
******************************************************************************/

/** \brief 命令完成中断使能位 */
#define AMHW_FSL_FTFA_CCIE       (1UL << 7)

/** \brief 读取冲突错误中断使能位 */
#define AMHW_FSL_FTFA_RDCOLLIE   (1UL << 6)

/** \brief 擦除全部扇区请求位 */
#define AMHW_FSL_FTFA_ERSAREQ    (1UL << 5)

/** \brief 当前擦除操作挂起位 */
#define AMHW_FSL_FTFA_ERSSUSP    (1UL << 4)

/**
 *  \brief FLASH命令
 */
typedef enum amhw_fsl_ftfa_cmd {
    amhw_fsl_ftfa_rd1sec  = 0x01,    /**< \brief 检测扇区是否为全1        */
    amhw_fsl_ftfa_pgmchk  = 0x02,    /**< \brief 编程结构检验             */
    amhw_fsl_ftfa_rdrsrc  = 0x03,    /**< \brief 读取特殊区域的FLASH内容  */
    amhw_fsl_ftfa_pgm4    = 0x06,    /**< \brief 编程一个字(4字节)的FLASH */
    amhw_fsl_ftfa_ersscr  = 0x09,    /**< \brief 擦除FLASH扇区            */
    amhw_fsl_ftfa_rd1all  = 0x40,    /**< \brief 检测所有扇区是否为全1    */
    amhw_fsl_ftfa_rdonce  = 0x41,    /**< \brief 读取一次编程区的FLASH    */
    amhw_fsl_ftfa_pgmonce = 0x43,    /**< \brief 编程一次编程区           */
    amhw_fsl_ftfa_ersall  = 0x44,    /**< \brief 擦除所有扇区             */
    amhw_fsl_ftfa_vfykey  = 0x45,    /**< \brief 验证后门密钥             */
}
amhw_fsl_ftfa_cmd_t;

/**
 *  \brief FTFA 寄存器结构体定义
 */
typedef struct amhw_fsl_ftfa {
    __IO uint8_t fstat;  /**< \brief FLASH状态寄存器      */
    __IO uint8_t fcnfg;  /**< \brief FLASH配置寄存器      */
    __I  uint8_t fsec;   /**< \brief FLASH安全寄存器      */
    __I  uint8_t fopt;   /**< \brief FLASH选项寄存器      */
    __IO uint8_t fccob3; /**< \brief FLASH命令对象寄存器3 */
    __IO uint8_t fccob2; /**< \brief FLASH命令对象寄存器2 */
    __IO uint8_t fccob1; /**< \brief FLASH命令对象寄存器1 */
    __IO uint8_t fccob0; /**< \brief FLASH命令对象寄存器0 */
    __IO uint8_t fccob7; /**< \brief FLASH命令对象寄存器7 */
    __IO uint8_t fccob6; /**< \brief FLASH命令对象寄存器6 */
    __IO uint8_t fccob5; /**< \brief FLASH命令对象寄存器5 */
    __IO uint8_t fccob4; /**< \brief FLASH命令对象寄存器4 */
    __IO uint8_t fccobb; /**< \brief FLASH命令对象寄存器b */
    __IO uint8_t fccoba; /**< \brief FLASH命令对象寄存器a */
    __IO uint8_t fccob9; /**< \brief FLASH命令对象寄存器9 */
    __IO uint8_t fccob8; /**< \brief FLASH命令对象寄存器8 */
    __IO uint8_t fprot3; /**< \brief FLASH编程保护寄存器3 */
    __IO uint8_t fprot2; /**< \brief FLASH编程保护寄存器2 */
    __IO uint8_t fprot1; /**< \brief FLASH编程保护寄存器1 */
    __IO uint8_t fprot0; /**< \brief FLASH编程保护寄存器0 */
} amhw_fsl_ftfa_t;

/**
 *  \brief 获取命令完成情况
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \retval 0 命令正在执行
 *  \retval AMHW_FSL_FTFA_CCIF 命令执行完成
 */
am_static_inline
uint8_t amhw_fsl_ftfa_cmd_is_complete (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fstat & AMHW_FSL_FTFA_CCIF;
}

/**
 *  \brief 清除CCIF位，执行FLASH命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \return 无
 *
 *  \details Details
 */
am_static_inline
void amhw_fsl_ftfa_cmd_lanch (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_CCIF;
}

/**
 *  \brief 获取读取冲突错误状态
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \retval 0 没有读取冲突错误
 *  \retval AMHW_FSL_FTFA_RDCOLERR 检测到读取冲突错误
 */
am_static_inline
uint8_t amhw_fsl_ftfa_read_collision_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fstat & AMHW_FSL_FTFA_RDCOLERR;
}

/**
 *  \brief 清除读取冲突错误标志位
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_read_collision_clr (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_RDCOLERR;
}

/**
 *  \brief 非法权限操作状态获取
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \retval 0 没有发生非法权限操作
 *  \retval AMHW_FSL_FTFA_ACCERR 发生了非法权限操作
 */
am_static_inline
uint8_t amhw_fsl_ftfa_access_err_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fstat & AMHW_FSL_FTFA_ACCERR;
}

/**
 *  \brief 清除非法权限操作状态标志
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_access_err_clr (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_ACCERR;
}

/**
 *  \brief 保护区域被侵入状态获取
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \retval 0 没有保护区被侵入
 *  \retval AMHW_FSL_FTFA_FPVIOL 保护区被侵入过
 */
am_static_inline
uint8_t amhw_fsl_ftfa_protection_violation_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fstat & AMHW_FSL_FTFA_FPVIOL;
}

/**
 *  \brief 保护区域被侵入状态标志清除
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_protection_violation_clr (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_FPVIOL;
}

/**
 *  \brief 命令完成状态获取
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \retval 0 命令顺利完成
 *  \retval AMHW_FSL_FTFA_MGSTAT0 命令发生错误
 */
am_static_inline
uint8_t amhw_fsl_ftfa_cmd_completion_status_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fstat & AMHW_FSL_FTFA_MGSTAT0;
}

/**
 *  \brief 命令完成状态标志清除
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  return 无
 */
am_static_inline
void amhw_fsl_ftfa_cmd_completion_status_clr (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_MGSTAT0;
}

/**
 *  \brief 命令完成中断使能
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_cmd_complete_int_enable (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg |= AMHW_FSL_FTFA_CCIE;
}

/**
 *  \brief 命令完成中断除能
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_cmd_complete_int_disable (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg &= ~AMHW_FSL_FTFA_CCIE;
}

/**
 *  \brief 读取冲突中断使能
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_read_collision_int_enable (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg |= AMHW_FSL_FTFA_RDCOLLIE;
}

/**
 *  \brief 读取冲突中断除能
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_read_collision_int_disable (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg &= ~AMHW_FSL_FTFA_RDCOLLIE;
}

/**
 *  \brief 挂起当前FLASH扇区擦除命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_flash_erase_suspend (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg |= AMHW_FSL_FTFA_ERSSUSP;
}

/**
 *  \brief 继续FLASH扇区擦除命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_flash_erase_resume (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    if ((p_hw_ftfa->fcnfg & AMHW_FSL_FTFA_ERSSUSP)) {
        p_hw_ftfa->fstat = AMHW_FSL_FTFA_CCIF;
    }
}

/**
 *  \brief 中止FLASH扇区擦除命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 无
 */
am_static_inline
void amhw_fsl_ftfa_flash_erase_abort (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fcnfg &= AMHW_FSL_FTFA_ERSSUSP;
}

/**
 *  \brief FLASH安全状态获取
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 安全设置寄存器
 */
am_static_inline
uint8_t amhw_fsl_ftfa_flash_security_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fsec;
}

/**
 *  \brief FLASH选项寄存器获取
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 选项寄存器
 */
am_static_inline
uint8_t amhw_fsl_ftfa_flash_option_get (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    return p_hw_ftfa->fopt;
}

/**
 *  \brief 将FLASH的第n/32区域添加为编程保护
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param n 第n/32块区域,  n = 0~31
 *
 *  \retval AM_ERROR FLASH命令正在执行，不允许写入FPROT寄存器
 *  \retval AM_OK    设置成功
 */
am_static_inline
int amhw_fsl_ftfa_program_protection_add (amhw_fsl_ftfa_t *p_hw_ftfa, uint8_t n)
{
    uint32_t field = 0;
    field |= p_hw_ftfa->fprot0 << 24;
    field |= p_hw_ftfa->fprot1 << 16;
    field |= p_hw_ftfa->fprot2 << 8;
    field |= p_hw_ftfa->fprot3 << 0;

    field &= ~(1ul << n);
    if(0 == (p_hw_ftfa->fstat | AMHW_FSL_FTFA_CCIF)) {
        p_hw_ftfa->fprot0 = (field >> 24) & (0xff);
        p_hw_ftfa->fprot1 = (field >> 16) & (0xff);
        p_hw_ftfa->fprot2 = (field >> 8 ) & (0xff);
        p_hw_ftfa->fprot3 = (field >> 0 ) & (0xff);
        return AM_OK;
    } else {
        return AM_ERROR;
    }

}

/**
 *  \brief 将FLASH的第n/32区域从编程保护中取消
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param n 第n/32块区域
 *
 *  return 无
 */
am_static_inline
void amhw_fsl_ftfa_program_protection_sub (amhw_fsl_ftfa_t *p_hw_ftfa, uint8_t n)
{
    uint32_t field = 0;
    field |= p_hw_ftfa->fprot0 << 24;
    field |= p_hw_ftfa->fprot1 << 16;
    field |= p_hw_ftfa->fprot2 << 8;
    field |= p_hw_ftfa->fprot3 << 0;

    field |= (1ul << n);

    p_hw_ftfa->fprot0 = (field >> 24) & (0xff);
    p_hw_ftfa->fprot1 = (field >> 16) & (0xff);
    p_hw_ftfa->fprot2 = (field >> 8 ) & (0xff);
    p_hw_ftfa->fprot3 = (field >> 0 ) & (0xff);
}
/**
 *  \brief 将amhw_fsl_ftfa_cmd_launch 函数拷贝到RAM中
 *
 *  \return 无
 *
 *  \note FLASH控制器在修改flash时，代码要在RAM中运行
 */
void amhw_fsl_ftfa_func_copy (void);

/**
 *  \brief 发送判断扇区是否为全1命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param start_addr 扇区起始地址
 *  \param n_words 字(32bit)的个数
 *  \param margin 附加选项
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_1s_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                            uint32_t     start_addr,
                            uint16_t     n_words,
                            uint8_t      margin);

/**
 *  \brief 字(32bit)编程结果正确与否的判断
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr 要判断的地址
 *  \param context 期望的数据内容
 *  \param margin 附加选项
 *          - 可选 0x01, 0x02
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_program_check (amhw_fsl_ftfa_t *p_hw_ftfa,
                                 uint32_t     addr,
                                 uint32_t     context,
                                 uint8_t      margin);

/**
 *  \brief 读取特殊功能区FLASH
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr flash地址
 *  \param p_context 指向缓存读取内容的缓存区的指针
 *  \param resrc_select 资源选择
 *          - 0x00 Program Flash 0 IFR
 *          - 0x01 Version ID
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_resrc_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                               uint32_t     addr,
                               uint32_t    *p_context,
                               uint8_t      resrc_select);

/**
 *  \brief 对一个字编程
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr FLASH字地址
 *  \param word 写入字的内容
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_word_program (amhw_fsl_ftfa_t *p_hw_ftfa,
                                 uint32_t     addr,
                                 uint32_t     word);

/**
 *  \brief 擦除扇区
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr 扇区起始地址
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_sector_erase (amhw_fsl_ftfa_t *p_hw_ftfa,
                                 uint32_t     addr);

/**
 *  \brief 判断所有扇区是否为全1
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param margin 附加选项
 *          可选参数 0x0 - 0x2
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_all_blocks_1s_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                                       uint8_t      margin);

/**
 *  \brief 读取64字节保留区中的一个字
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param index 索引编号 0x00-0x0f
 *  \param p_value 指向读取结果缓冲的指针
 *
 *  \return 0: 表示命令成功执行，其他: 表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_once_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                              uint8_t      index,
                              uint32_t    *p_value);

/**
 *  \brief 对保留区的一个字进行一次性编程
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param index 索引编号 0x00-0x0f
 *  \param value 希望写入的值
 *
 *  \return 0: 表示命令成功执行，其他: 表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_once_program (amhw_fsl_ftfa_t *p_hw_ftfa,
                                 uint8_t      index,
                                 uint32_t     value);

/**
 *  \brief 擦除所有扇区
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 0: 表示命令成功执行，其他:表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_all_blocks_erase (amhw_fsl_ftfa_t *p_hw_ftfa);

/**
 *  \brief 验证后门密钥权限
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param key 密钥数组
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_backdoor_access_key_verify (amhw_fsl_ftfa_t *p_hw_ftfa,
        uint8_t      key[8]);
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /** __AMHW_FSL_FTFA_H */

