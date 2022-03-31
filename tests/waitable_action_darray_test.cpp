#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/waitable_action_darray.h"

#define NR 100000

static uint32_t exec_times = 0;
static waitable_action_darray_t* q;

static ret_t qaction_exec_dummy(qaction_t* req) {
  exec_times++;
  return RET_OK;
}

static ret_t qaction_exec_dummy1(qaction_t* req) {
  exec_times++;
  return RET_OK;
}
static ret_t qaction_exec_dummy2(qaction_t* req) {
  exec_times++;
  return RET_OK;
}
static ret_t qaction_exec_dummy3(qaction_t* req) {
  exec_times++;
  return RET_OK;
}

static void* consumer(void* args) {
  uint32_t n = 0;
  qaction_t* action = NULL;

  log_debug("consumer start\n");
  while (waitable_action_darray_recv(q, &action, 3000) == RET_OK) {
    n++;
    qaction_exec(action);
    qaction_destroy(action);
  }
  log_debug("consumer done\n");

  return NULL;
}

static void* producer(void* args) {
  uint32_t i = 0;
  uint32_t id = tk_pointer_to_int(args);
  log_debug("p=%u start\n", id);
  for (i = 0; i < NR; i++) {
    qaction_t* a = qaction_create(qaction_exec_dummy, NULL, 0);
    if (waitable_action_darray_send(q, a, 3000) != RET_OK) {
      log_debug("send timeout\n");
      break;
    }
  }
  log_debug("p=%u done\n", id);

  return NULL;
}

void test() {
  tk_thread_t* c = tk_thread_create(consumer, NULL);
  tk_thread_t* p1 = tk_thread_create(producer, tk_pointer_from_int(1));
  tk_thread_t* p2 = tk_thread_create(producer, tk_pointer_from_int(2));
  tk_thread_t* p3 = tk_thread_create(producer, tk_pointer_from_int(3));
  tk_thread_t* p4 = tk_thread_create(producer, tk_pointer_from_int(4));

  q = waitable_action_darray_create(9);

  tk_thread_start(c);
  tk_thread_start(p1);
  tk_thread_start(p2);
  tk_thread_start(p3);
  tk_thread_start(p4);

  tk_thread_join(c);
  tk_thread_join(p1);
  tk_thread_join(p2);
  tk_thread_join(p3);
  tk_thread_join(p4);

  tk_thread_destroy(c);
  tk_thread_destroy(p1);
  tk_thread_destroy(p2);
  tk_thread_destroy(p3);
  tk_thread_destroy(p4);
  waitable_action_darray_destroy(q);

  log_debug("exec_times=%u \n", exec_times);
  ENSURE(exec_times == 4*NR);
}
void test_basic() {
  qaction_t* a = qaction_create(qaction_exec_dummy, NULL, 0);
  qaction_t* a1 = qaction_create(qaction_exec_dummy1, NULL, 0);
  qaction_t* a2 = qaction_create(qaction_exec_dummy2, NULL, 0);
  qaction_t* a3 = qaction_create(qaction_exec_dummy3, NULL, 0);
  qaction_t* ra;

  q = waitable_action_darray_create(9);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy) == NULL);
  waitable_action_darray_send(q, a, 3000);
  waitable_action_darray_send(q, a1, 3000);
  waitable_action_darray_send(q, a2, 3000);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy) == a);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy1) == a1);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy2) == a2);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy3) == NULL);

  ENSURE(waitable_action_darray_remove(q, qaction_exec_dummy) == RET_OK);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy) == NULL);

  a = qaction_create(qaction_exec_dummy, NULL, 0);
  waitable_action_darray_send(q, a, 3000);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy) == a);

  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy3) == NULL);
  ENSURE(waitable_action_darray_replace_ex(q, qaction_exec_dummy2, a3) == RET_OK);
  ENSURE(waitable_action_darray_find(q, qaction_exec_dummy3) == a3);

  ENSURE(waitable_action_darray_recv(q, &ra, 0) == RET_OK);
  ENSURE(a1 == ra);
  ENSURE(waitable_action_darray_recv(q, &ra, 0) == RET_OK);
  ENSURE(a3 == ra);
  ENSURE(waitable_action_darray_recv(q, &ra, 0) == RET_OK);
  ENSURE(a == ra);

  ENSURE(waitable_action_darray_recv(q, &ra, 0) != RET_OK);

  ENSURE(waitable_action_darray_destroy(q) == RET_OK);
  log_info("%s: ok\n", __func__);
}

#include "tkc/platform.h"

int main(int argc, char* argv[]) {
  platform_prepare();

  test();
  test_basic();

  return 0;
}
