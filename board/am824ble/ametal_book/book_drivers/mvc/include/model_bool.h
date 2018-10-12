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
 * \brief book`s model_bool driver for AM824
 *
 * \internal
 * \par Modification history
 * - 1.00 18-09-26  ipk, first implementation
 * \endinternal
 */
#ifndef __MODEL_BOOL_H__
#define __MODEL_BOOL_H__

#include "ametal.h"
#include "mvc.h"

//派生model_bool_t模型类
typedef struct _model_bool{
    model_t         isa;
    am_bool_t       value;                    // bool模型的数据
}model_bool_t;


int model_bool_init (model_bool_t *p_this, am_bool_t init_value);  // 模型初始化
int model_bool_set (model_bool_t *p_this, am_bool_t value);        // 设置模型的value值
int model_bool_get (model_bool_t *p_this, am_bool_t *p_value);     // 获取模型的value值



#endif /* __MODEL_BOOL_H__ */
/* end of file */
