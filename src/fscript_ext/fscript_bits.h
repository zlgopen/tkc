﻿/**
 * File:   fscript_bits.h
 * Author: AWTK Develop Team
 * Brief:  bits functions for fscript
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

#ifndef TK_FSCRIPT_BITS_H
#define TK_FSCRIPT_BITS_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_bits_register
 * 注册数学函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_bits_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_BITS_H*/
