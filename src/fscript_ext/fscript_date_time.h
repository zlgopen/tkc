﻿/**
 * File:   fscript_date_time.h
 * Author: AWTK Develop Team
 * Brief:  date_time functions for fscript
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-08 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_DATE_TIME_H
#define TK_FSCRIPT_DATE_TIME_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @method fscript_date_time_register
 * 注册date time函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_date_time_register(void);

END_C_DECLS

#endif /*TK_FSCRIPT_DATE_TIME_H*/
