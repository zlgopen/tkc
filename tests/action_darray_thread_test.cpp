#define WIN32_LEAN_AND_MEAN 1
#include "assert.h"
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/str.h"
#include "tkc/platform.h"
#include "tkc/action_darray_thread.h"
#include "tkc/waitable_action_darray.h"

#define NR 10000

static uint32_t exec_times = 0;
static str_t s_str;
static str_t* s_pstr = NULL;

static ret_t qaction_dummy_on_event(qaction_t* action, event_t* e) {
  if (e->type == EVT_DONE) {
    log_debug("done\n");
    qaction_destroy(action);
  }

  return RET_OK;
}

static ret_t qaction_dummy_exec_nodelay(qaction_t* action) {
  int i = ((int*)action->args)[0];
  exec_times++;
  log_debug("exec: exec_times=%u\n", exec_times);
  str_append_format(s_pstr, 1024, "%s(%d);", __func__, i);
  return RET_OK;
}

static ret_t qaction_dummy_exec(qaction_t* action) {
  int i = ((int*)action->args)[0];
  exec_times++;
  sleep_ms(1000);
  log_debug("exec: exec_times=%u\n", exec_times);
  str_append_format(s_pstr, 1024, "%s(%d);", __func__, i);
  return RET_OK;
}
static ret_t qaction_dummy_exec1(qaction_t* action) {
  int i = ((int*)action->args)[0];
  sleep_ms(1000);
  str_append_format(s_pstr, 1024, "%s(%d);", __func__, i);
  return RET_OK;
}
static ret_t qaction_dummy_exec2(qaction_t* action) {
  int i = ((int*)action->args)[0];
  sleep_ms(1000);
  str_append_format(s_pstr, 1024, "%s(%d);", __func__, i);
  return RET_OK;
}
static ret_t qaction_sleep(qaction_t* action) {
  int i = ((int*)action->args)[0];
  sleep_ms(1000);
  str_append_format(s_pstr, 1024, "%s(%d);", __func__, i);
  log_info("%s: done\n", __func__);
  return RET_OK;
}

static ret_t on_idle(void* ctx, action_darray_thread_t* thread) {
  log_debug("on_idle\n");

  return RET_QUIT;
}

static ret_t on_quit(void* ctx, action_darray_thread_t* thread) {
  log_debug("on_quit\n");

  return RET_OK;
}

void test() {
  uint32_t i = 0;
  action_darray_thread_t* thread1 = action_darray_thread_create();
  action_darray_thread_set_on_idle(thread1, on_idle, NULL);
  action_darray_thread_set_on_quit(thread1, on_quit, NULL);

  for (i = 0; i < NR; i++) {
    qaction_t* a = qaction_create(qaction_dummy_exec_nodelay, NULL, 0);
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_darray_thread_exec(thread1, a);
  }

  sleep_ms(2000);

  action_darray_thread_destroy(thread1);
  log_debug("exec_times=%u\n", exec_times);
}

void test_flush() {
  int i = 0;
  qaction_t* a;
  waitable_action_darray_t* darray = waitable_action_darray_create(61);
  action_darray_thread_t* thread1 = action_darray_thread_create_with_darray(darray);
  action_darray_thread_set_strategy(thread1, ACTION_DARRAY_THREAD_STRATEGY_FLUSH);
  action_darray_thread_set_on_idle(thread1, on_idle, NULL);
  action_darray_thread_set_on_quit(thread1, on_quit, NULL);

  if (!s_pstr) {
    s_pstr = str_init(&s_str, 64);
  } else {
    str_reset(s_pstr);
  }

  i = -1;
  a = qaction_create(qaction_sleep, &i, sizeof(i));
  qaction_set_on_event(a, qaction_dummy_on_event);
  action_darray_thread_exec(thread1, a);

  for (i = 0; i < 20; i++) {
    a = qaction_create(qaction_dummy_exec, &i, sizeof(i));
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_darray_thread_exec(thread1, a);

    a = qaction_create(qaction_dummy_exec1, &i, sizeof(i));
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_darray_thread_exec(thread1, a);

    if (i == 0) {
      a = qaction_create(qaction_dummy_exec2, &i, sizeof(i));
      qaction_set_on_event(a, qaction_dummy_on_event);
      action_darray_thread_exec(thread1, a);
    }
  }

  sleep_ms(3000);

  action_darray_thread_destroy(thread1);
  waitable_action_darray_destroy(darray);
  log_info("%s: %s\n", __func__, s_pstr->str);
  ENSURE(tk_str_eq(
      s_pstr->str,
      "qaction_sleep(-1);qaction_dummy_exec2(0);qaction_dummy_exec(19);qaction_dummy_exec1(19);"));
}
void test_replace() {
  int i = 0;
  qaction_t* a;
  waitable_action_darray_t* darray = waitable_action_darray_create(61);
  action_darray_thread_t* thread1 = action_darray_thread_create_with_darray(darray);
  action_darray_thread_set_strategy(thread1, ACTION_DARRAY_THREAD_STRATEGY_REPLACE);
  action_darray_thread_set_on_idle(thread1, on_idle, NULL);
  action_darray_thread_set_on_quit(thread1, on_quit, NULL);
  ENSURE(darray && thread1);

  if (!s_pstr) {
    s_pstr = str_init(&s_str, 64);
  } else {
    str_reset(s_pstr);
  }

  i = -1;
  a = qaction_create(qaction_sleep, &i, sizeof(i));
  qaction_set_on_event(a, qaction_dummy_on_event);
  action_darray_thread_exec(thread1, a);

  for (i = 0; i < 20; i++) {
    a = qaction_create(qaction_dummy_exec, &i, sizeof(i));
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_darray_thread_exec(thread1, a);

    a = qaction_create(qaction_dummy_exec1, &i, sizeof(i));
    qaction_set_on_event(a, qaction_dummy_on_event);
    action_darray_thread_exec(thread1, a);

    if (i == 0) {
      a = qaction_create(qaction_dummy_exec2, &i, sizeof(i));
      qaction_set_on_event(a, qaction_dummy_on_event);
      action_darray_thread_exec(thread1, a);
    }
  }

  sleep_ms(3000);

  action_darray_thread_destroy(thread1);
  waitable_action_darray_destroy(darray);
  log_info("%s: %s\n", __func__, s_pstr->str);
  ENSURE(tk_str_eq(
      s_pstr->str,
      "qaction_sleep(-1);qaction_dummy_exec(19);qaction_dummy_exec1(19);qaction_dummy_exec2(0);"));
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();
  test_flush();
  test_replace();

  return 0;
}
