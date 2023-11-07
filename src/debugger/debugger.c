﻿/**
 * File:   debugger.c
 * Author: AWTK Develop Team
 * Brief:  debugger
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
 * 2022-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "debugger/debugger.h"

bool_t debugger_is_paused(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL, FALSE);
  return debugger_get_state(debugger) == DEBUGGER_PROGRAM_STATE_PAUSED;
}

bool_t debugger_is_running(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL, FALSE);
  return debugger_get_state(debugger) == DEBUGGER_PROGRAM_STATE_RUNNING;
}

bool_t debugger_is_paused_or_running(debugger_t* debugger) {
  debugger_program_state_t state;
  return_value_if_fail(debugger != NULL, FALSE);
  state = debugger_get_state(debugger);
  return state == DEBUGGER_PROGRAM_STATE_RUNNING || state == DEBUGGER_PROGRAM_STATE_PAUSED;
}

debugger_program_state_t debugger_get_state(debugger_t* debugger) {
  debugger_program_state_t state;
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, DEBUGGER_PROGRAM_STATE_NONE);
  if (debugger->vt->get_state != NULL) {
    state = debugger->vt->get_state(debugger);
  } else {
    state = debugger->state;
  }
  return state;
}

ret_t debugger_set_state(debugger_t* debugger, debugger_program_state_t state) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  if (debugger->vt->set_state != NULL) {
    return debugger->vt->set_state(debugger, state);
  } else {
    debugger->state = state;
  }
  return RET_OK;
}

ret_t debugger_lock(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->lock != NULL, RET_BAD_PARAMS);

  return debugger->vt->lock(debugger);
}

ret_t debugger_unlock(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->unlock != NULL, RET_BAD_PARAMS);

  return debugger->vt->unlock(debugger);
}

ret_t debugger_stop(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->stop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger_is_paused_or_running(debugger), RET_BAD_PARAMS);

  return debugger->vt->stop(debugger);
}

ret_t debugger_restart(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->restart != NULL, RET_BAD_PARAMS);

  return debugger->vt->restart(debugger);
}

ret_t debugger_pause(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->pause != NULL, RET_BAD_PARAMS);

  return debugger->vt->pause(debugger);
}

bool_t debugger_match(debugger_t* debugger, const char* code_id) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, FALSE);
  return_value_if_fail(debugger->vt->match != NULL && code_id != NULL, FALSE);

  return debugger->vt->match(debugger, code_id);
}

ret_t debugger_step_in(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_in != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_in(debugger);
}

ret_t debugger_step_out(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_out != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_out(debugger);
}

ret_t debugger_step_over(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_over != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_over(debugger);
}

ret_t debugger_step_loop_over(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->step_loop_over != NULL, RET_BAD_PARAMS);

  return debugger->vt->step_loop_over(debugger);
}

ret_t debugger_continue(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->continve != NULL, RET_BAD_PARAMS);

  return debugger->vt->continve(debugger);
}

tk_object_t* debugger_get_local(debugger_t* debugger, uint32_t frame_index) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_local != NULL, NULL);

  return debugger->vt->get_local(debugger, frame_index);
}

tk_object_t* debugger_get_self(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_self != NULL, NULL);

  return debugger->vt->get_self(debugger);
}

tk_object_t* debugger_get_global(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_global != NULL, NULL);

  return debugger->vt->get_global(debugger);
}

tk_object_t* debugger_get_callstack(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_callstack != NULL, NULL);

  return debugger->vt->get_callstack(debugger);
}

ret_t debugger_clear_break_points(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->clear_break_points != NULL, RET_BAD_PARAMS);

  return debugger->vt->clear_break_points(debugger);
}

ret_t debugger_set_break_point(debugger_t* debugger, uint32_t line) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->set_break_point != NULL, RET_BAD_PARAMS);

  return debugger->vt->set_break_point(debugger, line);
}

ret_t debugger_remove_break_point(debugger_t* debugger, uint32_t line) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->remove_break_point != NULL, RET_BAD_PARAMS);

  return debugger->vt->remove_break_point(debugger, line);
}

ret_t debugger_attach(debugger_t* debugger, const char* lang, const char* code_id) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->attach != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lang != NULL && code_id != NULL, RET_BAD_PARAMS);
  return_value_if_fail(!debugger_is_paused_or_running(debugger), RET_BAD_PARAMS);

  return debugger->vt->attach(debugger, lang, code_id);
}

ret_t debugger_get_code(debugger_t* debugger, binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_code != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL, RET_BAD_PARAMS);

  if (!debugger_is_paused_or_running(debugger)) {
    return RET_FAIL;
  }

  return debugger->vt->get_code(debugger, code);
}

ret_t debugger_get_debuggers(debugger_t* debugger, binary_data_t* debuggers) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_debuggers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debuggers != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_debuggers(debugger, debuggers);
}

ret_t debugger_get_break_points(debugger_t* debugger, binary_data_t* break_points) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->get_break_points != NULL, RET_BAD_PARAMS);
  return_value_if_fail(break_points != NULL, RET_BAD_PARAMS);

  return debugger->vt->get_break_points(debugger, break_points);
}

ret_t debugger_update_code(debugger_t* debugger, const binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->update_code != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL && code->data != NULL, RET_BAD_PARAMS);

  return debugger->vt->update_code(debugger, code);
}

ret_t debugger_launch(debugger_t* debugger, const char* lang, const binary_data_t* code) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->launch != NULL, RET_BAD_PARAMS);
  return_value_if_fail(code != NULL && code->data != NULL && lang != NULL, RET_BAD_PARAMS);
  return_value_if_fail(!debugger_is_paused_or_running(debugger), RET_BAD_PARAMS);

  return debugger->vt->launch(debugger, lang, code);
}

ret_t debugger_deinit(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->deinit != NULL, RET_BAD_PARAMS);
  debugger_set_state(debugger, DEBUGGER_PROGRAM_STATE_NONE);

  return debugger->vt->deinit(debugger);
}

tk_object_t* debugger_get_threads(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_threads != NULL, NULL);

  return debugger->vt->get_threads(debugger);
}

ret_t debugger_launch_app(debugger_t* debugger, const char* program, const char* work_dir, int argc,
                          char* argv[]) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->launch_app != NULL, RET_BAD_PARAMS);
  return_value_if_fail(program != NULL, RET_BAD_PARAMS);
  return_value_if_fail(!debugger_is_paused_or_running(debugger), RET_BAD_PARAMS);

  return debugger->vt->launch_app(debugger, program, work_dir, argc, argv);
}

ret_t debugger_set_break_point_ex(debugger_t* debugger, const char* position) {
  return_value_if_fail(position != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);

  if (debugger->vt->set_break_point_ex != NULL) {
    return debugger->vt->set_break_point_ex(debugger, position);
  } else if (debugger->vt->set_break_point != NULL) {
    const char* p = strchr(position, ':');
    if (p != NULL) {
      return debugger->vt->set_break_point(debugger, tk_atoi(p + 1));
    }
    return RET_NOT_IMPL;
  } else {
    return RET_NOT_IMPL;
  }
}

ret_t debugger_remove_break_point_ex(debugger_t* debugger, const char* position) {
  return_value_if_fail(position != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);

  if (debugger->vt->remove_break_point_ex != NULL) {
    return debugger->vt->remove_break_point_ex(debugger, position);
  } else if (debugger->vt->remove_break_point != NULL) {
    const char* p = strchr(position, ':');
    if (p != NULL) {
      return debugger->vt->remove_break_point(debugger, tk_atoi(p + 1));
    } else {
      return debugger->vt->remove_break_point(debugger, tk_atoi(position));
    }
    return RET_NOT_IMPL;
  } else {
    return RET_NOT_IMPL;
  }
}

tk_object_t* debugger_get_var(debugger_t* debugger, const char* path) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, NULL);
  return_value_if_fail(debugger->vt->get_var != NULL, NULL);

  return debugger->vt->get_var(debugger, path);
}

int32_t debugger_get_current_frame(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, -1);
  return debugger->current_frame_index;
}

ret_t debugger_set_current_frame(debugger_t* debugger, uint32_t frame_index) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);

  debugger->current_frame_index = frame_index;
  if (debugger->vt->set_current_frame != NULL) {
    return debugger->vt->set_current_frame(debugger, frame_index);
  } else {
    return RET_OK;
  }
}

uint64_t debugger_get_current_thread_id(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, 0);
  if (debugger->vt->get_current_thread_id != NULL) {
    return debugger->vt->get_current_thread_id(debugger);
  } else {
    return 0;
  }
}

ret_t debugger_set_current_thread_id(debugger_t* debugger, uint64_t thread_id) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);

  if (debugger->vt->set_current_thread_id != NULL) {
    return debugger->vt->set_current_thread_id(debugger, thread_id);
  } else {
    return RET_NOT_IMPL;
  }
}

ret_t debugger_dispatch_messages(debugger_t* debugger) {
  return_value_if_fail(debugger != NULL && debugger->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(debugger->vt->dispatch_messages != NULL, RET_BAD_PARAMS);

  return debugger->vt->dispatch_messages(debugger);
}
