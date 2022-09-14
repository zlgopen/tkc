﻿#include "gtest/gtest.h"
#include "tkc/event_source_idle.h"

static uint32_t s_idle_times = 0;
static ret_t idle_once(const idle_info_t* idle) {
  s_idle_times++;

  return RET_OK;
}

TEST(EventSourceIdle, basic) {
  idle_manager_t* tm = idle_manager_create();
  event_source_t* event_source = event_source_idle_create(tm);

  ASSERT_EQ(event_source_get_fd(event_source), -1);
  ASSERT_EQ(event_source_check(event_source), RET_OK);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0xffffu);
  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_idle_times, 0u);

  uint32_t id = idle_manager_get_next_idle_id(tm);
  ASSERT_EQ(id != TK_INVALID_ID, TRUE);
  ASSERT_EQ(idle_manager_add_with_id(tm, id, idle_once, NULL), id);
  ASSERT_EQ(idle_manager_add_with_id(tm, id, idle_once, NULL), TK_INVALID_ID);

  idle_manager_add(tm, idle_once, NULL);
  ASSERT_EQ(event_source_get_wakeup_time(event_source), 0u);

  ASSERT_EQ(event_source_dispatch(event_source), RET_OK);
  ASSERT_EQ(s_idle_times, 2u);

  tk_object_unref(TK_OBJECT(event_source));
  idle_manager_destroy(tm);
}
