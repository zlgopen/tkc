﻿/**
 * File:   debugger_server.h
 * Author: AWTK Develop Team
 * Brief:  debugger server
 *
 * Copyright (c) 2022 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DEBUGGER_SERVER_H
#define TK_DEBUGGER_SERVER_H

#include "tkc/iostream.h"
#include "debugger/debugger.h"

BEGIN_C_DECLS

/**
 * @class debugger_server_t
 * @annotaion ["fake"]
 * 调试器服务器。
 */

/**
 * @method debugger_server_start
 * 启用调试器服务。
 * @param {tk_iostream_t*} io IO对象，用于和客户端通信。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_start(tk_iostream_t* io);

/**
 * @method debugger_server_wait
 * 等待调试器服务退出。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_wait(void);

/**
 * @method debugger_server_stop
 * 停用调试器服务。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_stop(void);

/**
 * @method debugger_server_set_single_mode
 * 设置为单一debugger模式。
 * @param {bool_t} single_mode 单一debugger模式
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_set_single_mode(bool_t single_mode);

/**
 * @method debugger_server_set_single_mode_ex
 * 设置为单一debugger模式。
 *
 * @param {bool_t} single_mode 单一debugger模式
 * @param {const char*} lang 语言。
 * @param {const char*} code_id 代码ID。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_server_set_single_mode_ex(bool_t single_mode, const char* lang, const char* code_id);

/**
 * @method debugger_server_is_running
 * 判断服务是否在运行。
 *
 * @return {bool_t} 返回TRUE表示在运行，否则表示没有运行。
 */
bool_t debugger_server_is_running(void);

/**
 * @method debugger_server_find_debugger
 * 查找调试器对象。
 * @param {const char*} code_id 代码ID。
 *
 * @return {debugger_t*} 返回debugger对象或者NULL。
 */
debugger_t* debugger_server_find_debugger(const char* code_id);

END_C_DECLS

#endif /*TK_DEBUGGER_SERVER_H*/
