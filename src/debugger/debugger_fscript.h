﻿/**
 * File:   debugger_fscript.h
 * Author: AWTK Develop Team
 * Brief:  debugger_fscript for fscript
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_DEBUGGER_FSCRIPT_H
#define TK_DEBUGGER_FSCRIPT_H

#include "tkc/fscript.h"
#include "tkc/cond_var.h"
#include "tkc/mutex_nest.h"
#include "debugger/debugger.h"

BEGIN_C_DECLS

typedef enum _debugger_fscript_break_type_t {
  DEBUGGER_FSCRIPT_BREAK_NONE = 0,
  DEBUGGER_FSCRIPT_BREAK_STEP_IN,
  DEBUGGER_FSCRIPT_BREAK_STEP_OUT,
  DEBUGGER_FSCRIPT_BREAK_STEP_OVER,
  DEBUGGER_FSCRIPT_BREAK_STEP_LOOP_OVER
} debugger_fscript_break_type_t;

/**
 * @class debugger_fscript_t
 * fscript调试器
 *
 */
typedef struct _debugger_fscript_t {
  debugger_t debugger;

  /*private*/
  char* code_id;
  fscript_t* fscript;

  /*前一次执行代码的行号*/
  uint32_t prev_executed_line;
  /*前一次中断执行代码的行号*/
  int32_t prev_breaked_line;
  /*总共执行的行数(包括重复执行)*/
  uint32_t executed_lines;

  /*下一次停止的物理行号(用以实现step over)*/
  int32_t next_stop_line;
  /*下一次停止的行号(用以实现step in/next)*/
  int32_t next_stop_executed_line;
  /*下一次停止的callstack序数(用以实现step over/next)*/
  int32_t next_stop_call_frame_index;

  str_t code;
  bool_t paused;
  str_t temp_str;
  tk_mutex_nest_t* mutex;
  tk_mutex_nest_t* mutex_for_destroy;
  tk_cond_var_t* cond_var;
  darray_t break_points;
  darray_t call_stack_frames;
  bool_t code_changed;
  bool_t stop_at_start_line;
  int32_t stop_at_next_line;
  debugger_fscript_break_type_t break_type;
} debugger_fscript_t;

/**
 * @method debugger_fscript_create
 * 创建调试器对象。
 *
 * @return {debugger_t*} 返回debugger对象。
 */
debugger_t* debugger_fscript_create(void);

/**
 * @method debugger_fscript_set_fscript
 * 设置fscript对象。
 * @param {debugger_t*} debugger debugger对象。
 * @param {fscript_t*} fscript 脚本对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_fscript_set_fscript(debugger_t* debugger, fscript_t* fscript);

/**
 * @method debugger_fscript_cast
 * 类型转换。
 * @param {debugger_t*} debugger debugger对象。
 *
 * @return {debugger_fscript_t*} 返回debugger对象。
 */
debugger_fscript_t* debugger_fscript_cast(debugger_t* debugger);

/**
 * @method debugger_fscript_set_code
 * 设置代码。
 * @param {debugger_t*} debugger debugger对象。
 * @param {const binary_data_t*} code 代码。
 * @param {bool_t} changed 是否重新加载。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t debugger_fscript_set_code(debugger_t* debugger, const binary_data_t* code, bool_t changed);

#define DEBUGGER_FSCRIPT(debugger) debugger_fscript_cast((debugger_t*)debugger)

#define DEBUGGER_LANG_FSCRIPT "fscript"

/*fscript hooks*/
ret_t debugger_fscript_set_var(fscript_t* fscript, const char* name, const value_t* v);
ret_t debugger_fscript_exec_func(fscript_t* fscript, const char* name, fscript_func_call_t* iter,
                                 value_t* result);

/*public for tests*/
/**
 * @method debugger_fscript_get_start_line
 * 获取第一行被执行的代码的行号。
 * @param {fscript_t*} fscript fscript对象。
 *
 * @return {int32_t} 返回行号。
 */
int32_t debugger_fscript_get_start_line(fscript_t* fscript);

END_C_DECLS

#endif /*TK_DEBUGGER_FSCRIPT_H*/
