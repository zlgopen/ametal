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
 * \brief 程序清单6.38
 *
 * \note 该历程需要用到第三方库，hash table, 若要使用该demo，需要将 am_prj_config.h
 *  中的宏 AM_CFG_NVRAM_ENABLE 设置为1（默认为0），否则该demo测试结果不正确。
 *  再测试其它demo，需要将宏还原为0.避免出现内存不足。 
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-25  adw, first implementation
 * \endinternal
 */
 #include <stdio.h>
#include <stdlib.h>
#include "hash_kv.h"
#include "am_vdebug.h"
typedef struct _student{
    char        name[10];                       // 姓名
    char        sex;                            // 性别
    int         height, weight;                 // 身高、体重
} student_t;

int db_id_to_idx (unsigned char id[6])          // 通过ID得到数组索引
{
    int i;
    int sum = 0;
    for (i = 0; i < 6; i++)
        sum += id[0];
    return sum % 250;
}

int student_info_generate (unsigned char *p_id, student_t *p_student) // 随机产生一条学生记录
{
    int i;
    for (i = 0; i < 6; i++)                            // 随机产生一个学号
        p_id[i] = rand();
    for (i = 0; i < 9; i++)                            // 随机名字，由 'a' ~ 'z' 组成
        p_student->name[i] = (rand() % ('z' - 'a')) + 'a';
    p_student->name[i]= '\0';                          // 字符串结束符
    p_student->sex    = (rand() & 0x01) ? 'F' : 'M';   // 随机性别
    p_student->height = (int)rand() / rand();
    p_student->weight = (int)rand() / rand();
    return 0;
}

int am_main ()
{
		
    student_t      stu;
    unsigned char  id[6];
    int            i;
    hash_kv_t      hash_students;
	
    hash_kv_init(&hash_students, 250, 6, sizeof(student_t), (hash_kv_func_t)db_id_to_idx, "hash_students");

    for (i = 0; i < 100; i++){                              // 添加100个学生的信息
        student_info_generate(id, &stu);                    // 设置学生信息，用随机数作为测试
        if (hash_kv_search(&hash_students, id, &stu) == 0){ // 查找到已经存在该ID的学生记录
            am_kprintf("该ID的记录已经存在！\n");
            continue;
        }
        am_kprintf("增加记录：ID : %02x%02x%02x%02x%02x%02x",id[0],id[1],id[2],id[3],id[4],id[5]);
        am_kprintf("信息： %s  %c %d %d\n", stu.name, stu.sex, stu.height, stu.weight);
        if (hash_kv_add(&hash_students, id, &stu) != 0){
            am_kprintf("添加失败\n");
        }
    }
    am_kprintf("查找ID为：%02x%02x%02x%02x%02x%02x的信息\n",id[0],id[1],id[2],id[3],id[4],id[5]);
    if (hash_kv_search(&hash_students, id, &stu) == 0)
        am_kprintf("学生信息： %s  %c %d %d\n", stu.name, stu.sex, stu.height, stu.weight);
    else
        am_kprintf("未找到该ID的记录！\r\n");

    hash_kv_deinit(&hash_students);
    return 0;
}



/* end of file */

