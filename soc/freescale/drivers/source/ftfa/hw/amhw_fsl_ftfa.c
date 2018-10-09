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

#include "hw/amhw_fsl_ftfa.h"
#include "ametal.h"

/**
 *  brief amhw_fsl_ftfa_cmd_launch函数编译后数据拷贝
 *  到RAM需要的空间大小 （以半字 16bit 计）
 */
#define CMD_LAUNCH_FUNC_SIZE    (40)

static uint16_t amhw_fsl_ftfa_cmd_launch_ram_context[CMD_LAUNCH_FUNC_SIZE];

static uint32_t (* amhw_fsl_ftfa_cmd_launch_ram) (amhw_fsl_ftfa_t *);

/**
 *  \brief 执行flash命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
static uint32_t amhw_fsl_ftfa_cmd_launch (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    /** 等待之前的操作完成或者超时 */
    while (!(p_hw_ftfa->fstat & AMHW_FSL_FTFA_CCIF)) {
        ;
    }

    /** 清除错误标志 */
    p_hw_ftfa->fstat = AMHW_FSL_FTFA_FPVIOL  |
                       AMHW_FSL_FTFA_ACCERR  |
                       AMHW_FSL_FTFA_RDCOLERR;

    p_hw_ftfa->fstat = AMHW_FSL_FTFA_CCIF;

    /** 命令完成 */
    while (!(p_hw_ftfa->fstat & AMHW_FSL_FTFA_CCIF)) {
        ;
    }

    return p_hw_ftfa->fstat & (~AMHW_FSL_FTFA_CCIF);
}

/**
 *  \brief 将amhw_fsl_ftfa_cmd_launch 函数拷贝到RAM中
 *
 *  \return 无
 *
 *  \note FLASH控制器在修改flash时，代码要在RAM中运行
 */
void amhw_fsl_ftfa_func_copy (void)
{
    uint16_t *p;
    uint32_t i;

    /** 将函数地址调整为2字节对齐，并强制转化为 (uint16_t *) */
    p = (uint16_t *)(((uint32_t)&amhw_fsl_ftfa_cmd_launch) & (~0x1));

    /** 将函数的指令拷贝到RAM中 */
    for (i = 0; i < CMD_LAUNCH_FUNC_SIZE; i++) {
        amhw_fsl_ftfa_cmd_launch_ram_context[i] = *p;
        p++;
    }

    /** 将RAM中函数的起始地址给 amhw_fsl_ftfa_cmd_launch_ram
     *  由于Cortex M0+ 只支持 thumb指令
     *  所以 amhw_fsl_ftfa_cmd_launch_ram 的最低位为 1
     */
    amhw_fsl_ftfa_cmd_launch_ram
        = (uint32_t (*) (amhw_fsl_ftfa_t *))       /** 强制转化为函数指针 */
          (
           (uint32_t)                          /** 强制转化为整型便于运算 */
           amhw_fsl_ftfa_cmd_launch_ram_context | 0x01
          );
}
/**
 *  \brief 发送判断扇区是否为全1命令
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param start_addr 扇区起始地址
 *  \param n_words 字(32bit)的个数
 *  \param margin 附加选项
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_1s_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                                 uint32_t          start_addr,
                                 uint16_t          n_words,
                                 uint8_t           margin)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_rd1sec;
    p_hw_ftfa->fccob1 = (start_addr >> 16) & 0xff;
    p_hw_ftfa->fccob2 = (start_addr >> 8 ) & 0xff;
    p_hw_ftfa->fccob3 = (start_addr >> 0 ) & 0xff;
    p_hw_ftfa->fccob4 = (n_words    >> 8 ) & 0xff;
    p_hw_ftfa->fccob5 = (n_words    >> 0 ) & 0xff;
    p_hw_ftfa->fccob6 = margin;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 字(32bit)编程结果正确与否的判断
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr 要判断的地址
 *  \param context 期望的数据内容
 *  \param margin 附加选项
 *          #可选 0x01, 0x02
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_program_check (amhw_fsl_ftfa_t *p_hw_ftfa,
                                       uint32_t          addr,
                                       uint32_t          context,
                                       uint8_t           margin)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_pgmchk;
    p_hw_ftfa->fccob1 = (addr >> 16) & 0xff;
    p_hw_ftfa->fccob2 = (addr >> 8 ) & 0xff;
    p_hw_ftfa->fccob3 = (addr >> 0 ) & 0xff;
    p_hw_ftfa->fccob4 = margin;
    p_hw_ftfa->fccob8 = (context >> 24) & 0xff;
    p_hw_ftfa->fccob9 = (context >> 16) & 0xff;
    p_hw_ftfa->fccoba = (context >> 8 ) & 0xff;
    p_hw_ftfa->fccobb = (context >> 0 ) & 0xff;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 读取特殊功能区FLASH
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \parma addr flash地址
 *  \param p_context 指向缓存读取内容的缓存区的指针
 *  \param resrc_select 资源选择
 *          - 0x00 Program Flash 0 IFR
 *          - 0x01 Version ID
 *
 *  \return 0: 表示命令成功执行，其他: 表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_resrc_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                                    uint32_t          addr,
                                    uint32_t         *p_context,
                                    uint8_t           resrc_select)
{
    uint8_t status;
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_rdrsrc;
    p_hw_ftfa->fccob1 = (addr >> 16) & 0xff;
    p_hw_ftfa->fccob2 = (addr >> 8 ) & 0xff;
    p_hw_ftfa->fccob3 = (addr >> 0 ) & 0xff;
    p_hw_ftfa->fccob8 = resrc_select;

    status = amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);

    if (status != 0) {
        return status;
    }

    *p_context = 0;
    *p_context |= p_hw_ftfa->fccob4 << 24;
    *p_context |= p_hw_ftfa->fccob5 << 16;
    *p_context |= p_hw_ftfa->fccob6 << 8;
    *p_context |= p_hw_ftfa->fccob7 << 0;
    return status;
}

/**
 *  \brief 对一个字编程
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr FLASH字地址
 *  \param word 写入字的内容
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_word_program (amhw_fsl_ftfa_t *p_hw_ftfa,
                                      uint32_t          addr,
                                      uint32_t          word)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_pgm4;
    p_hw_ftfa->fccob1 = (addr >> 16) & 0xff;
    p_hw_ftfa->fccob2 = (addr >> 8 ) & 0xff;
    p_hw_ftfa->fccob3 = (addr >> 0 ) & 0xff;

    p_hw_ftfa->fccob4 = (word >> 24) & 0xff;
    p_hw_ftfa->fccob5 = (word >> 16) & 0xff;
    p_hw_ftfa->fccob6 = (word >> 8 ) & 0xff;
    p_hw_ftfa->fccob7 = (word >> 0 ) & 0xff;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 擦除扇区
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param addr 扇区起始地址
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_sector_erase (amhw_fsl_ftfa_t *p_hw_ftfa,
                                      uint32_t          addr)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_ersscr;
    p_hw_ftfa->fccob1 = (addr >> 16) & 0xff;
    p_hw_ftfa->fccob2 = (addr >> 8 ) & 0xff;
    p_hw_ftfa->fccob3 = (addr >> 0 ) & 0xff;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 判断所有扇区是否为全1
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param margin 附加选项
 *          可选参数 0x0 - 0x2
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_all_blocks_1s_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                                            uint8_t           margin)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_rd1all;
    p_hw_ftfa->fccob1 = margin;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 读取64字节保留区中的一个字
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param index 索引编号 0x00-0x0f
 *  \param p_value 指向读取结果缓冲的指针
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_once_read (amhw_fsl_ftfa_t *p_hw_ftfa,
                                   uint8_t           index,
                                   uint32_t         *p_value)
{
    uint8_t status;
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_rdonce;
    p_hw_ftfa->fccob1 = index;

    status = amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);

    if (0 != status) {
        return status;
    }

    *p_value = 0;
    *p_value |= p_hw_ftfa->fccob4 >> 24;
    *p_value |= p_hw_ftfa->fccob5 >> 16;
    *p_value |= p_hw_ftfa->fccob6 >> 8;
    *p_value |= p_hw_ftfa->fccob7 >> 0;

    return status;
}

/**
 *  \brief 对保留区一个字进行一次性编程
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param index 索引编号 0x00-0x0f
 *  \param value 希望写入的值
 *
 *  \return 0 表示命令成功执行，其他 表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_once_program (amhw_fsl_ftfa_t *p_hw_ftfa,
                                      uint8_t           index,
                                      uint32_t          value)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_pgmonce;
    p_hw_ftfa->fccob1 = index;

    p_hw_ftfa->fccob4 = (value >> 24) & 0xff;
    p_hw_ftfa->fccob5 = (value >> 16) & 0xff;
    p_hw_ftfa->fccob6 = (value >> 8 ) & 0xff;
    p_hw_ftfa->fccob7 = (value >> 0 ) & 0xff;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 擦除所有扇区
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t amhw_fsl_ftfa_all_blocks_erase (amhw_fsl_ftfa_t *p_hw_ftfa)
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_ersall;

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}

/**
 *  \brief 验证后门密钥权限
 *
 *  \param p_hw_ftfa 指向寄存器结构体的指针
 *  \param key 密钥数组
 *
 *  \return 0 表示命令成功执行，其他表示命令没有成功执行
 */
uint32_t
amhw_fsl_ftfa_backdoor_access_key_verify (amhw_fsl_ftfa_t *p_hw_ftfa,
                                           uint8_t           key[8])
{
    p_hw_ftfa->fccob0 = amhw_fsl_ftfa_vfykey;

    p_hw_ftfa->fccob4 = key[0];
    p_hw_ftfa->fccob5 = key[1];
    p_hw_ftfa->fccob6 = key[2];
    p_hw_ftfa->fccob7 = key[3];
    p_hw_ftfa->fccob8 = key[4];
    p_hw_ftfa->fccob9 = key[5];
    p_hw_ftfa->fccoba = key[6];
    p_hw_ftfa->fccobb = key[7];

    return amhw_fsl_ftfa_cmd_launch_ram(p_hw_ftfa);
}


