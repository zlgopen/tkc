﻿/**
 * File:   async.h
 * Author: AWTK Develop Team
 * Brief:  async
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ===============================================================
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ASYNC_H
#define TK_ASYNC_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef ret_t (*async_exec_t)(void* ctx);
typedef ret_t (*async_on_result_t)(void* ctx, ret_t result);

/**
 * @class async_t
 * 在后台线程执行指定函数，不会阻塞UI线程。
 *
 * @annotation ["fake"]
 *
 */

/**
 * @method async_call
 * 异步执行exec函数，执行完成后，在后台线程调用on_result函数。
 *
 * @annotation ["static"]
 *
 * @param {async_exec_t} exec 需要异步支持的函数。
 * @param {async_on_result_t} on_result 返回执行结果(可选)
 * @param {void*} ctx 两个回调函数的上下文。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t async_call(async_exec_t exec, async_on_result_t on_result, void* ctx);

/**
 * @method async_call_init_ex
 * 全局初始化。
 *
 * @annotation ["static"]
 * @param {uint32_t} max_threads 最大线程数。
 * @param {uint32_t} min_threads 最小线程数(必须大于0)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t async_call_init_ex(uint32_t max_threads, uint32_t min_threads);

/**
 * @method async_call_init_ex2
 * 全局初始化。
 *
 * @annotation ["static"]
 * @param {uint32_t} max_threads 最大线程数。
 * @param {uint32_t} min_threads 最小线程数(必须大于0)。
 * @param {uint32_t} stack_size 栈空间大小(字节)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t async_call_init_ex2(uint32_t max_threads, uint32_t min_threads, uint32_t stack_size);

#define async_call_init() async_call_init_ex(5, 1)

/**
 * @method async_call_deinit
 * 全局~初始化。
 *
 * @annotation ["static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t async_call_deinit(void);

END_C_DECLS

#endif /*TK_ASYNC_H*/
