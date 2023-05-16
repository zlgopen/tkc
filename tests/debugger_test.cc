#include "debugger/debugger_fscript.h"
#include "debugger/debugger_global.h"
#include "debugger/debugger_server.h"
#include "debugger/debugger_message.h"
#include "debugger/debugger_server_tcp.h"
#include "debugger/debugger_client_tcp.h"

#include "tkc/fscript.h"
#include "tkc/thread.h"
#include "tkc/async.h"
#include "tkc/object_default.h"
#include "gtest/gtest.h"

static void* fscript_thread_entry(void* args) {
  value_t v;
  fscript_t* fscript = (fscript_t*)args;
  fscript_exec(fscript, &v);
  fscript_destroy(fscript);

  return NULL;
}

static ret_t on_debugger_client_event(void* ctx, event_t* e) {
  str_t* str = (str_t*)ctx;
  switch (e->type) {
    case DEBUGGER_RESP_MSG_BREAKED: {
      debugger_breaked_event_t* event = debugger_breaked_event_cast(e);
      str_append(str, "breaked");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ")");
      break;
    }
    case DEBUGGER_RESP_MSG_LOG: {
      debugger_log_event_t* event = debugger_log_event_cast(e);
      if (event->line >= 0) {
        str_append(str, "log");
        str_append(str, "(");
        str_append_int(str, event->line);
        str_append(str, ",\"");
        str_append(str, event->message);
        str_append(str, "\")");
      }
      break;
    }
    case DEBUGGER_RESP_MSG_ERROR: {
      debugger_error_event_t* event = debugger_error_event_cast(e);
      str_append(str, "error");
      str_append(str, "(");
      str_append_int(str, event->line);
      str_append(str, ",\"");
      str_append(str, event->message);
      str_append(str, "\")");
      break;
    }
    case DEBUGGER_RESP_MSG_COMPLETED: {
      str_append(str, "completed()");
    }
    default:
      break;
  }

  return RET_OK;
}

TEST(Debugger, start_line1) {
  const char* code =
      "//hello\n"
      "function foo(a, b) {\n"
      " return a + b;\n"
      "}\n"
      "print(foo(1, 2))\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  ASSERT_EQ(debugger_fscript_get_start_line(fscript), 4);
  fscript_destroy(fscript);

  TK_OBJECT_UNREF(obj);
}

TEST(Debugger, start_line2) {
  const char* code =
      "\n"
      "function foo(a, b) {\n"
      " return a + b;\n"
      "}\n"
      "print(foo(1, 2))\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  ASSERT_EQ(debugger_fscript_get_start_line(fscript), 4);
  fscript_destroy(fscript);

  TK_OBJECT_UNREF(obj);
}

TEST(Debugger, launch) {
  const char* code =
      "var i = 0\n"
      "for(i = 0; i < 10; i=i+1) {\n"
      "  print(i)\n"
      "}\n"
      "print(i)\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();
  async_call_init();
  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  binary_data_t code_data;
  code_data.data = (uint8_t*)code;
  code_data.size = strlen(code) + 1;
  ASSERT_EQ(debugger_launch(client, DEBUGGER_LANG_FSCRIPT, &code_data), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  debugger_client_wait_for_completed(client);

  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str,
               "breaked(0)breaked(1)breaked(2)log(2,\"0\")breaked(1)breaked(2)log(2,\"1\")breaked("
               "1)log(2,\"2\")log(2,\"3\")log(2,\"4\")log(2,\"5\")log(2,\"6\")log(2,\"7\")log(2,"
               "\"8\")log(2,\"9\")log(4,\"10\")completed()");
  str_reset(&str);
  async_call_deinit();
}

#if 1
TEST(Debugger, step_over) {
  const char* code =
      "var i = 0\n"
      "for(i = 0; i < 10; i=i+1) {\n"
      "  print(i)\n"
      "}\n"
      "print(i)\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str,
               "breaked(0)breaked(1)breaked(2)log(2,\"0\")breaked(1)breaked(2)log(2,\"1\")breaked("
               "1)log(2,\"2\")log(2,\"3\")log(2,\"4\")log(2,\"5\")log(2,\"6\")log(2,\"7\")log(2,"
               "\"8\")log(2,\"9\")log(4,\"10\")completed()");
  str_reset(&str);
}

TEST(Debugger, get_debuggers) {
  const char* code =
      "function foo(a, b) {\nreturn a + b;\n}\nc=foo(1, 2)\nprint(c)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 1), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_debuggers(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data),
               "85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\n");
  ASSERT_EQ(debugger_get_break_points(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "1\n2\n3\n");

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);
  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  str_reset(&str);
}

TEST(Debugger, callstack1) {
  const char* code =
      "function foo(a, b) {\nreturn a + b;\n}\nc=foo(1, 2)\nprint(c)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 1), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "foo\n<root>\n");

  tk_object_t* local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 1);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 2);
  TK_OBJECT_UNREF(local);

  local = debugger_get_local(client, 1);
  ASSERT_EQ(local != NULL, true);
  TK_OBJECT_UNREF(local);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str, "breaked(1)log(4,\"3\")completed()");
  str_reset(&str);
}

TEST(Debugger, callstack2) {
  const char* code =
      "var c = 123\n"
      "function bar(a, b) {\n"
      "  var c = a + b\n"
      "  return c\n"
      "}\n"
      "function foo(a, b) {\n"
      "  return bar(2*a, 2*b)\n"
      "}\n"
      "c = foo(100, 200)\n"
      "print(c)\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "bar\nfoo\n<root>\n");

  tk_object_t* local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 200);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 400);
  TK_OBJECT_UNREF(local);

  local = debugger_get_local(client, 1);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 100);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 200);
  TK_OBJECT_UNREF(local);

  local = debugger_get_local(client, 2);
  //ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[2].value", 0), 123);
  TK_OBJECT_UNREF(local);

  ASSERT_EQ(debugger_continue(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str, "breaked(2)log(9,\"600.000000\")completed()");
  str_reset(&str);
}

TEST(Debugger, break_point) {
  const char* code =
      "function foo(a, b){\n"
      "define_param(\"a\")\n"
      "define_param(\"b\")\n"
      "return ((a + b))\n"
      "}\n"
      "foo(123, 456)\n"
      "foo(123, 456)\n"
      "foo(123, 456)\n"
      "//code_id(\"944676fc958f236e51adb1f1ae1b799350201b00b7e81219e7a41b5fae5f8aa8\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  sleep_ms(200);
  ASSERT_EQ(debugger_continue(client), RET_OK);
  sleep_ms(200);
  ASSERT_EQ(debugger_continue(client), RET_OK);
  sleep_ms(200);

  ASSERT_NE(debugger_continue(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str, "breaked(3)breaked(3)breaked(3)completed()");
  str_reset(&str);
}

TEST(Debugger, step_in) {
  const char* code =
      "var c = 123\n"
      "function bar(a, b) {\n"
      "  var c = a + b\n"
      "  return c\n"
      "}\n"
      "function foo(a, b) {\n"
      "  return bar(2*a, 2*b)\n"
      "}\n"
      "c = foo(100, 200)\n"
      "print(c)\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 8), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");
  tk_object_t* local = NULL;

  ASSERT_EQ(debugger_step_in(client), RET_OK);
  sleep_ms(500);
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "foo\n<root>\n");

  local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 100);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 200);
  TK_OBJECT_UNREF(local);

  ASSERT_EQ(debugger_step_in(client), RET_OK);
  sleep_ms(500);
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "bar\nfoo\n<root>\n");

  local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 200);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 400);
  TK_OBJECT_UNREF(local);

  ASSERT_EQ(debugger_step_out(client), RET_OK);
  sleep_ms(500);
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "foo\n<root>\n");

  ASSERT_EQ(debugger_step_out(client), RET_OK);
  sleep_ms(500);
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_continue(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(
      str.str,
      "breaked(8)breaked(6)breaked(2)breaked(6)breaked(8)log(9,\"600.000000\")completed()");
  str_reset(&str);
}

TEST(Debugger, step_loop_over) {
  const char* code =
      "var i = 0\n"
      "for(i = 0; i < 10; i=i+1) {\n"
      "  print(i)\n"
      "}\n"
      "print(i)\n"
      "//code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_step_loop_over(client), RET_OK);
  sleep_ms(200);

  ASSERT_EQ(debugger_step_loop_over(client), RET_OK);
  sleep_ms(200);

  for (uint32_t i = 0; i < 10; i++) {
    ASSERT_EQ(debugger_step_loop_over(client), RET_OK);
    sleep_ms(200);
  }

  ASSERT_EQ(debugger_step_loop_over(client), RET_OK);
  sleep_ms(200);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str,
               "breaked(0)breaked(1)breaked(2)log(2,\"0\")breaked(2)log(2,\"1\")breaked(2)log(2,"
               "\"2\")breaked(2)log(2,\"3\")breaked(2)log("
               "2,\"4\")breaked(2)log(2,\"5\")breaked(2)log(2,\"6\")breaked(2)log(2,\"7\")breaked("
               "2)log(2,\"8\")breaked(2)log(2,\"9\")breaked(4)log("
               "4,\"10\")completed()");
  str_reset(&str);
}

TEST(Debugger, event1) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);

  debugger_dispatch_messages(client);
  ASSERT_EQ(debugger_is_paused(client), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
  ASSERT_STREQ(str.str, "breaked(0)log(0,\"1\")log(1,\"2\")log(2,\"3\")completed()");
  str_reset(&str);
}

TEST(Debugger, event2) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";
  str_t str;
  str_init(&str, 100);
  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_BREAKED, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_LOG, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_ERROR, on_debugger_client_event, &str);
  emitter_on(EMITTER(client), DEBUGGER_RESP_MSG_COMPLETED, on_debugger_client_event, &str);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);

  debugger_client_wait_for_completed(client);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();

  ASSERT_STREQ(str.str, "log(0,\"1\")log(1,\"2\")breaked(2)log(2,\"3\")completed()");
  str_reset(&str);
}

TEST(Debugger, basic) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  debugger_t* debugger = debugger_server_find_debugger(fscript->code_id);
  ASSERT_EQ(debugger_is_paused(debugger), TRUE);

  ASSERT_EQ(debugger_continue(client), RET_OK);
  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, basic1) {
  const char* code =
      "print(1)\nprint(2)\nprint(3)\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 0), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 1), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 2), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  ASSERT_EQ(debugger_step_over(client), RET_OK);
  sleep_ms(200);
  ASSERT_EQ(debugger_step_over(client), RET_OK);
  ASSERT_EQ(debugger_remove_break_point(client, 2), RET_OK);
  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, local) {
  const char* code =
      "var aaa=111\nvar bbb =222;\nvar "
      "ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* local = debugger_get_local(client, 0);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[0].value", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(local, "body.variables.[1].value", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(local, "body.variables.[2].value"), "abc");
  TK_OBJECT_UNREF(local);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, self) {
  const char* code =
      "aaa=111\nbbb "
      "=222;\nccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* self = debugger_get_self(client);
  ASSERT_EQ(tk_object_get_prop_int(self, "aaa", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(self, "bbb", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(self, "ccc"), "abc");
  TK_OBJECT_UNREF(self);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, global) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  tk_object_t* global = debugger_get_global(client);
  ASSERT_EQ(tk_object_get_prop_int(global, "aaa", 0), 111);
  ASSERT_EQ(tk_object_get_prop_int(global, "bbb", 0), 222);
  ASSERT_STREQ(tk_object_get_prop_str(global, "ccc"), "abc");
  TK_OBJECT_UNREF(global);

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, callstack) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data = {0, NULL};
  ASSERT_EQ(debugger_get_callstack(client, &data), RET_OK);
  ASSERT_STREQ((char*)(data.data), "<root>\n");

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}

TEST(Debugger, code) {
  const char* code =
      "global.aaa=111\nglobal.bbb "
      "=222;\nglobal.ccc=\"abc\"\nprint(a);\n//"
      "code_id(\"85e86311e2d595c65b745d8143b6085efe819c354584742f72aeacd3336a0a5e\")";

  debugger_global_init();

  tk_object_t* obj = object_default_create();
  fscript_t* fscript = fscript_create(obj, code);
  tk_thread_t* thread = tk_thread_create(fscript_thread_entry, fscript);

  debugger_server_tcp_init(DEBUGGER_TCP_PORT);
  debugger_t* client = debugger_client_tcp_create("localhost", DEBUGGER_TCP_PORT);
  ASSERT_EQ(client != NULL, TRUE);

  debugger_server_tcp_start();
  ASSERT_EQ(debugger_attach(client, DEBUGGER_LANG_FSCRIPT, fscript->code_id), RET_OK);
  ASSERT_EQ(debugger_set_break_point(client, 3), RET_OK);

  tk_thread_start(thread);
  sleep_ms(500);

  binary_data_t data;
  data.data = (void*)"print(1)";
  data.size = strlen((char*)data.data);
  ASSERT_EQ(debugger_update_code(client, &data), RET_OK);

  data.data = NULL;
  ASSERT_EQ(debugger_get_code(client, &data), RET_OK);
  ASSERT_STREQ((char*)data.data, "print(1)");

  ASSERT_EQ(debugger_clear_break_points(client), RET_OK);

  tk_thread_join(thread);
  tk_thread_destroy(thread);

  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(client);
  debugger_server_tcp_deinit();
  debugger_global_deinit();
}
#endif
