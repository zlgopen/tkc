﻿/**
 * File:   fscript_typed_array.h
 * Author: AWTK Develop Team
 * Brief:  typed_array functions for fscript
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

#ifndef TK_FSCRIPT_TYPED_ARRAY_H
#define TK_FSCRIPT_TYPED_ARRAY_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_typed_array_register
 * 注册typed array函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_typed_array_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_TYPED_ARRAY_H*/
