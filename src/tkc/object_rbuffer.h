﻿/**
 * File:   object_rbuffer.h
 * Author: AWTK Develop Team
 * Brief:  wrap rbuffer to an object.
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OBJECT_RBUFFER_H
#define TK_OBJECT_RBUFFER_H

#include "tkc/object.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

/**
 * @class object_rbuffer_t
 * @parent tk_object_t
 *
 * 将rbuffer包装成object。
 *
 */
typedef struct _object_rbuffer_t {
  tk_object_t object;

  /**
   * @property {rbuffer_t*} rbuffer
   * @annotation ["readable"]
   * rbuffer对象。
   *
   */
  rbuffer_t* rbuffer;

  /*private*/
  rbuffer_t arbuffer;
} object_rbuffer_t;

/**
 * @method object_rbuffer_create
 *
 * 创建rbuffer对象。
 *
 * @annotation ["constructor"]
 * @param {const uint8_t*} data 缓冲区。
 * @param {uint32_t} capacity 缓冲区的容量。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_rbuffer_create(const uint8_t* data, uint32_t capacity);

/**
 * @method object_rbuffer_cast
 * 转换为object_rbuffer对象。
 * @annotation ["cast"]
 * @param {tk_object_t*} obj object_rbuffer对象。
 *
 * @return {object_rbuffer_t*} object_rbuffer对象。
 */
object_rbuffer_t* object_rbuffer_cast(tk_object_t* obj);
#define OBJECT_RBUFFER(obj) object_rbuffer_cast(obj)

END_C_DECLS

#endif /*TK_OBJECT_RBUFFER_H*/
