﻿/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex base on rtthread
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
 * 2019-11-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "rtthread.h"
#include "tkc/mem.h"
#include "tkc/mutex.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"

struct _tk_mutex_t {
  rt_mutex_t mutex;
  char name[TK_NAME_LEN + 1];
};

tk_mutex_t* tk_mutex_create() {
  tk_mutex_t* mutex = TKMEM_ZALLOC(tk_mutex_t);
  return_value_if_fail(mutex != NULL, NULL);
  tk_snprintf(mutex->name, TK_NAME_LEN, "mutex%d", (int)time_now_ms());

  mutex->mutex = rt_mutex_create(mutex->name, 0);
  if (mutex->mutex == NULL) {
    TKMEM_FREE(mutex);
  }

  return mutex;
}

ret_t tk_mutex_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(rt_mutex_take(mutex->mutex, 0xffffff) == RT_EOK, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_try_lock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(rt_mutex_take(mutex->mutex, RT_WAITING_NO) == RT_EOK, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_unlock(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  return_value_if_fail(rt_mutex_release(mutex->mutex) == RT_EOK, RET_FAIL);

  return RET_OK;
}

ret_t tk_mutex_destroy(tk_mutex_t* mutex) {
  return_value_if_fail(mutex != NULL, RET_BAD_PARAMS);

  rt_mutex_delete(mutex->mutex);
  memset(mutex, 0x00, sizeof(tk_mutex_t));
  TKMEM_FREE(mutex);

  return RET_OK;
}
