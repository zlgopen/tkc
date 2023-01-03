﻿/**
 * File:   fscript_rbuffer.h
 * Author: AWTK Develop Team
 * Brief:  rbuffer functions for fscript
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_RBUFFER_H
#define TK_FSCRIPT_RBUFFER_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_rbuffer_register
 * 注册rbuffer相关函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_rbuffer_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_RBUFFER_H*/
