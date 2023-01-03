﻿/**
 * File:   fscript_object.h
 * Author: AWTK Develop Team
 * Brief:  object functions for fscript
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_OBJECT_H
#define TK_FSCRIPT_OBJECT_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_object_register
 * 注册object函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_object_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_OBJECT_H*/
