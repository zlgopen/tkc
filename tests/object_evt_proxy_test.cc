#include "gtest/gtest.h"
#include "tkc/object_evt_proxy.h"
#include "tkc/object_default.h"

static int s_callback_called = 0;
static event_t s_last_event;

// 测试订阅回调函数
static ret_t test_subscribe_callback(tk_object_t* subscriber, event_t* e, void* ctx) {
  s_callback_called++;
  s_last_event = *e;
  return RET_OK;
}

TEST(ObjectEvtProxy, basic) {
  tk_object_t* proxy = NULL;
  tk_object_t* publisher = NULL;
  object_default_t* publisher_obj = NULL;
  ret_t ret;

  // 1. 创建事件代理对象
  proxy = object_evt_proxy_create();
  ASSERT_TRUE(proxy != NULL) << "创建事件代理对象失败";

  // 2. 创建事件发布者（使用默认对象作为发布者）
  publisher = object_default_create();
  ASSERT_TRUE(publisher != NULL) << "创建发布者对象失败";
  publisher_obj = OBJECT_DEFAULT(publisher);

  // 3. 注册事件代理（主题："test_topic"，事件类型：EVT_PROP_CHANGED）
  ret = object_evt_proxy_register(proxy, "test_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_OK) << "注册事件代理失败";

  // 4. 订阅事件（订阅"test_topic"主题）
  ret = object_evt_proxy_subscribe(proxy, "test_topic", test_subscribe_callback, NULL);
  ASSERT_EQ(ret, RET_OK) << "订阅事件失败";

  // 5. 触发发布者事件（修改属性触发EVT_PROP_CHANGED）
  s_callback_called = 0;
  value_t v;
  value_set_int(&v, 123);
  tk_object_set_prop(publisher, "test_prop", &v);  // 触发属性变更事件
  ASSERT_EQ(s_callback_called, 1) << "事件回调未被调用";
  ASSERT_EQ(s_last_event.type, EVT_PROP_CHANGED) << "事件类型不匹配";
  ASSERT_EQ(s_last_event.target, publisher) << "事件源不匹配";

  // 6. 取消订阅
  ret = object_evt_proxy_unsubscribe(proxy, "test_topic", test_subscribe_callback, NULL);
  ASSERT_EQ(ret, RET_OK) << "取消订阅失败";

  // 7. 再次触发事件，验证回调不再被调用
  s_callback_called = 0;
  value_set_int(&v, 456);
  tk_object_set_prop(publisher, "test_prop", &v);
  ASSERT_EQ(s_callback_called, 0) << "取消订阅后回调仍被调用";

  // 8. 注销事件代理
  ret = object_evt_proxy_unregister(proxy, "test_topic");
  ASSERT_EQ(ret, RET_OK) << "注销事件代理失败";

  // 9. 释放资源
  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}

TEST(ObjectEvtProxy, register_duplicate_topic) {
  tk_object_t* proxy = object_evt_proxy_create();
  tk_object_t* publisher = object_default_create();
  ret_t ret;

  // 第一次注册成功
  ret = object_evt_proxy_register(proxy, "duplicate_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_OK);

  // 重复注册同一主题应失败
  ret = object_evt_proxy_register(proxy, "duplicate_topic", publisher, EVT_PROP_CHANGED, NULL);
  ASSERT_EQ(ret, RET_FAIL);

  TK_OBJECT_UNREF(proxy);
  TK_OBJECT_UNREF(publisher);
}
