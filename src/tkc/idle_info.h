﻿/**
 * File:   idle_info.h
 * Author: AWTK Develop Team
 * Brief:  idle info
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IDLE_INFO_H
#define TK_IDLE_INFO_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _idle_info_t;
typedef struct _idle_info_t idle_info_t;

struct _idle_manager_t;
typedef struct _idle_manager_t idle_manager_t;

typedef ret_t (*idle_func_t)(const idle_info_t* idle);

typedef enum _idle_info_type_t {
  IDLE_INFO_NORMAL = 0,
  IDLE_INFO_WIDGET_ADD,
} idle_info_type_t;

/**
 * @class idle_info_t
 * @parent tk_object_t
 * @annotation ["scriptable"]
 *
 * 单个idle的信息。
 */
struct _idle_info_t {
  tk_object_t object;

  /**
   * @property {idle_func_t} on_idle
   * @annotation ["readable"]
   * idle回调函数。
   */
  idle_func_t on_idle;

  /**
   * @property {void*} ctx
   * @annotation ["readable", "scriptable"]
   * idle回调函数上下文。
   */
  void* ctx;

  /**
   * @property {void*} extra_ctx
   * @annotation ["readable", "scriptable"]
   * idle回调函数上下文。
   */
  void* extra_ctx;

  /**
   * @property {uint32_t} id
   * @annotation ["readable", "scriptable"]
   * idle的ID
   *
   * > 为TK\_INVALID\_ID时表示无效idle。
   */
  uint32_t id;

  /**
   * @property {void*} on_destroy_ctx
   * @annotation ["readable"]
   * idle销毁时的回调函数的上下文。
   */
  void* on_destroy_ctx;

  /**
   * @property {tk_destroy_t} on_destroy
   * @annotation ["readable"]
   * idle销毁时的回调函数。
   */
  tk_destroy_t on_destroy;

  /*private*/
  uint32_t dispatch_id;
  bool_t busy;
  uint16_t idle_info_type;
};

/**
 * @method idle_info_cast
 * 转换为idle_info对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {idle_info_t*} idle idle_info对象。
 *
 * @return {idle_info_t*} idle_info对象。
 */
idle_info_t* idle_info_cast(idle_info_t* idle);

/*internal use*/
int idle_info_compare_by_id(const void* a, const void* b);
int idle_info_compare_by_ctx(const void* a, const void* b);
int idle_info_compare_by_ctx_and_type(const void* a, const void* b);
idle_info_t* idle_info_init_dummy(idle_info_t* idle, uint32_t id);
idle_info_t* idle_info_init_dummy_with_ctx_and_type(idle_info_t* idle, uint16_t idle_info_type,
                                                    void* ctx);
ret_t idle_info_on_idle(idle_info_t* idle, uint32_t dispatch_id);
bool_t idle_info_is_available(idle_info_t* idle, uint32_t dispatch_id);
idle_info_t* idle_info_create(idle_manager_t* idle_manager, uint32_t id, idle_func_t on_idle,
                              void* ctx, uint16_t idle_info_type);

#define IDLE_INFO(o) ((idle_info_t*)(o))

END_C_DECLS

#endif /*TK_IDLE_INFO_H*/
