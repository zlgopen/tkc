﻿#include "tkc/wstr.h"
#include "tkc/mem.h"
#include "gtest/gtest.h"

static void testSetWStr(const char* utf8, const wchar_t* cstr) {
  wstr_t str;
  char sutf8[256];
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_get_utf8(&str, sutf8, sizeof(sutf8)), RET_OK);
  ASSERT_EQ(strcmp(utf8, sutf8), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, demo) {
  wstr_t s;
  wstr_init(&s, 0);

  wstr_append(&s, L"abc");
  wstr_append(&s, L"123");

  wstr_reset(&s);
}

TEST(WStr, basic) {
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中文" */
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};
  testSetWStr("Hello", L"Hello");
  testSetWStr(str, L"中文");
}

static void testSetUtf8(const char* utf8, const wchar_t* cstr) {
  wstr_t str;
  char sutf8[256];
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_set_utf8(&str, utf8), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_get_utf8(&str, sutf8, sizeof(sutf8)), RET_OK);
  ASSERT_EQ(strcmp(utf8, sutf8), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, utf8) {
  /* 兼容非utf8编码的编译器，采用utf8编码初始化str，编码内容："中文" */
  char str[7] = {(char)0xe4, (char)0xb8, (char)0xad, (char)0xe6, (char)0x96, (char)0x87, 0};
  testSetUtf8("Hello", L"Hello");
  testSetUtf8(str, L"中文");
}

static void testRemove(const wchar_t* cstr, uint16_t offset, uint16_t nr, ret_t ret,
                       const wchar_t* expected) {
  wstr_t str;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_remove(&str, offset, nr), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcs_cmp(str.str, expected), 0);
  }

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, remove) {
  testRemove(L"abc", 1, 2, RET_OK, L"a");
  testRemove(L"abc", 0, 3, RET_OK, L"");
  testRemove(L"abc", 1, 3, RET_OK, L"a");
  testRemove(L"abc", 1, 9, RET_OK, L"a");
  testRemove(L"abc", 1, 9, RET_OK, L"a");
  testRemove(L"abc", 2, 9, RET_OK, L"ab");

  testRemove(L"汉字测试", 1, 2, RET_OK, L"汉试");
  testRemove(L"汉字测试", 0, 3, RET_OK, L"试");
  testRemove(L"汉字测试", 1, 3, RET_OK, L"汉");
  testRemove(L"汉字测试", 1, 9, RET_OK, L"汉");
  testRemove(L"汉字测试", 1, 9, RET_OK, L"汉");
  testRemove(L"汉字测试", 2, 9, RET_OK, L"汉字");
}

static void testInsert(const wchar_t* cstr, const wchar_t* insert, uint16_t offset, ret_t ret,
                       const wchar_t* expected) {
  wstr_t str;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_insert(&str, offset, insert, wcslen(insert)), ret);
  if (ret == RET_OK) {
    ASSERT_EQ(wcs_cmp(str.str, expected), 0);
  }

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, insert) {
  testInsert(L"汉字测试", L"中国", 0, RET_OK, L"中国汉字测试");
  testInsert(L"汉字测试", L"中国", 1, RET_OK, L"汉中国字测试");
  testInsert(L"汉字测试", L"中国", 3, RET_OK, L"汉字测中国试");
  testInsert(L"汉字测试", L"中国", 4, RET_OK, L"汉字测试中国");
}

TEST(WStr, poppush) {
  wstr_t str;
  const wchar_t* cstr = L"汉字测试";
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_set(&str, cstr), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)wcslen(cstr));
  ASSERT_EQ(wcs_cmp(str.str, cstr), 0);

  ASSERT_EQ(wstr_push(&str, 'a'), RET_OK);
  ASSERT_EQ(str.size, (uint32_t)(wcslen(cstr) + 1));
  ASSERT_EQ(str.str[str.size - 1], (wchar_t)'a');

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, int) {
  wstr_t str;
  int32_t v1 = 123;
  int32_t v2 = 0;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_from_int(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, int64) {
  wstr_t str;
  int64_t v2 = 0;
  int64_t v1 = 0x0000000100000000;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_from_int64(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_int64(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, double) {
  wstr_t str;
  double v1 = 123;
  double v2 = 0;
  ASSERT_EQ(wstr_init(&str, 100), &str);
  ASSERT_EQ(str.capacity, 100u);

  ASSERT_EQ(wstr_from_float(&str, v1), RET_OK);
  ASSERT_EQ(wstr_to_float(&str, &v2), RET_OK);
  ASSERT_EQ(v1, v2);

  ASSERT_EQ(wstr_set(&str, L"1.01"), RET_OK);
  ASSERT_EQ(wstr_add_float(&str, 0.01), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.02"), 0);

  ASSERT_EQ(wstr_add_float(&str, 0.001), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.021"), 0);

  ASSERT_EQ(wstr_add_float(&str, 0.1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.121"), 0);

  ASSERT_EQ(wstr_add_float(&str, 1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"2.121"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.01"), RET_OK);
  ASSERT_EQ(wstr_add_float(&str, -0.01), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.00"), 0);

  ASSERT_EQ(wstr_add_float(&str, -0.001), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.999"), 0);

  ASSERT_EQ(wstr_add_float(&str, -0.1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, 1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, -1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"0.899"), 0);

  ASSERT_EQ(wstr_add_float(&str, 123), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"123.899"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.010"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1.01"), 0);

  ASSERT_EQ(wstr_set(&str, L"1.00"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"1"), 0);

  ASSERT_EQ(wstr_set(&str, L"100"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"100"), 0);

  ASSERT_EQ(wstr_set(&str, L"100.00000000"), RET_OK);
  ASSERT_EQ(wstr_trim_float_zero(&str), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"100"), 0);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, value) {
  wstr_t str;
  value_t v1;
  int32_t vi;
  double vf;
  ASSERT_EQ(wstr_init(&str, 0), &str);
  ASSERT_EQ(str.capacity, 0u);

  value_set_int(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  value_set_float(&v1, 1234);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_float(&str, &vf), RET_OK);
  ASSERT_EQ(value_float(&v1), vf);

  value_set_str(&v1, "a12345");
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"a12345"), 0);

  value_set_wstr(&v1, L"b12345");
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wcs_cmp(str.str, L"b12345"), 0);

  value_set_float32(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  value_set_double(&v1, 123);
  ASSERT_EQ(wstr_from_value(&str, &v1), RET_OK);
  ASSERT_EQ(wstr_to_int(&str, &vi), RET_OK);
  ASSERT_EQ(value_int(&v1), vi);

  ASSERT_EQ(wstr_reset(&str), RET_OK);
}

TEST(WStr, wcs_len) {
  ASSERT_EQ(wcs_len(L""), 0u);
  ASSERT_EQ(wcs_len(L"a"), 1u);
  ASSERT_EQ(wcs_len(L"abc中文测试123"), 10u);
}

TEST(WStr, wcs_cpy) {
  wchar_t str[100];

  ASSERT_EQ(wcs_len(wcs_cpy(str, L"")), 0u);
  ASSERT_EQ(wcs_len(wcs_cpy(str, L"a")), 1u);
  ASSERT_EQ(wcs_len(wcs_cpy(str, L"abc中文测试123")), 10u);
}

TEST(WStr, wcs_ncpy) {
  wchar_t str[100];

  ASSERT_EQ(wcs_len(wcs_ncpy(str, L"", 1)), 0u);
  ASSERT_EQ(wcs_len(wcs_ncpy(str, L"a", 1)), 1u);
  ASSERT_EQ(wcs_len(wcs_ncpy(str, L"abc", 1)), 1u);
  ASSERT_EQ(wcs_len(wcs_ncpy(str, L"abc中文测试123", 30)), 10u);
}

TEST(WStr, wcs_chr) {
  size_t i = 0;
  const wchar_t* str = L"abc中文测试123";
  size_t nr = wcslen(str);

  ASSERT_EQ(nr, wcs_len(str));
  for (i = 0; i < nr; i++) {
    ASSERT_EQ(wcs_chr(str, str[i]), wcschr(str, str[i]));
  }
}

TEST(WStr, wcs_dup) {
  wchar_t* str = NULL;

  str = wcs_dup(L"");
  ASSERT_EQ(wcs_cmp(str, L""), 0);
  TKMEM_FREE(str);

  str = wcs_dup(L"123");
  ASSERT_EQ(wcs_cmp(str, L"123"), 0);
  TKMEM_FREE(str);

  str = wcs_dup(L"abc中文测试123");
  ASSERT_EQ(wcs_cmp(str, L"abc中文测试123"), 0);
  TKMEM_FREE(str);
}

TEST(WStr, push) {
  wstr_t str;
  int32_t v = 0;
  wstr_t* s = &str;
  ASSERT_EQ(wstr_init(&str, 0), &str);

  ASSERT_EQ(wstr_push(s, (wchar_t)'1'), RET_OK);
  ASSERT_EQ(s->size, 1u);
  ASSERT_EQ(wstr_push(s, (wchar_t)'2'), RET_OK);
  ASSERT_EQ(s->size, 2u);

  ASSERT_EQ(wstr_append_with_len(s, L"345", 3), RET_OK);
  ASSERT_EQ(s->size, 5u);
  ASSERT_EQ(wstr_to_int(s, &v), RET_OK);
  ASSERT_EQ(v, 12345);
  wstr_reset(&str);
}

TEST(WStr, push_int) {
  wstr_t str;
  int32_t v = 0;
  wstr_t* s = &str;
  ASSERT_EQ(wstr_init(&str, 0), &str);

  ASSERT_EQ(wstr_push_int(s, "%d", 12345), RET_OK);
  ASSERT_EQ(s->size, 5u);
  ASSERT_EQ(wstr_to_int(s, &v), RET_OK);
  ASSERT_EQ(v, 12345);
  s->size = 0;
  wstr_reset(&str);
}

TEST(WStr, append) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_append(&str, L"123"), RET_OK);
  ASSERT_EQ(str.size, 3u);
  ASSERT_EQ(wstr_append(&str, L"abc"), RET_OK);
  ASSERT_EQ(str.size, 6u);
  ASSERT_EQ(wcscmp(str.str, L"123abc"), 0);

  wstr_reset(&str);
}

TEST(WStr, append_len) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_append_with_len(&str, L"123123", 3), RET_OK);
  ASSERT_EQ(str.size, 3u);
  ASSERT_EQ(wstr_append_with_len(&str, L"abcabc", 3), RET_OK);
  ASSERT_EQ(str.size, 6u);
  ASSERT_EQ(wcscmp(str.str, L"123abc"), 0);

  wstr_reset(&str);
}

TEST(WStr, count) {
  wstr_t str;
  wstr_init(&str, 0);
  ASSERT_EQ(wstr_set(&str, L"123@123"), RET_OK);
  ASSERT_EQ(wstr_count_char(&str, '#'), 0u);
  ASSERT_EQ(wstr_count_char(&str, '@'), 1u);
  ASSERT_EQ(wstr_count_char(&str, '1'), 2u);
  wstr_reset(&str);
}

TEST(WStr, set_utf8_with_len) {
  wstr_t str;

  wstr_init(&str, 0);
  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 0), RET_OK);
  ASSERT_EQ(str.size, 0u);
  ASSERT_EQ(wcscmp(str.str, L""), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 1), RET_OK);
  ASSERT_EQ(str.size, 1u);
  ASSERT_EQ(wcscmp(str.str, L"1"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 3), RET_OK);
  ASSERT_EQ(str.size, 3u);
  ASSERT_EQ(wcscmp(str.str, L"123"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 6), RET_OK);
  ASSERT_EQ(str.size, 6u);
  ASSERT_EQ(wcscmp(str.str, L"123123"), 0);

  ASSERT_EQ(wstr_set_utf8_with_len(&str, "123123", 6), RET_OK);
  ASSERT_EQ(str.size, 6u);
  ASSERT_EQ(wcscmp(str.str, L"123123"), 0);

  wstr_reset(&str);
}

TEST(WStr, set_with_len) {
  wstr_t s;
  uint32_t i = 0;
  const wchar_t* cstr = L"abc123";
  uint32_t n = wcslen(cstr);

  wstr_init(&s, 0);

  for (i = 0; i < n; i++) {
    ASSERT_EQ(wstr_set_with_len(&s, cstr, i), RET_OK);
    ASSERT_EQ(s.size, i);
    ASSERT_EQ(wcsncmp(s.str, cstr, i), 0);
  }

  wstr_reset(&s);
}

TEST(WStr, case_cmp) {
  wstr_t str1;
  wstr_t str2;
  ASSERT_EQ(wstr_init(&str1, 0), &str1);
  ASSERT_EQ(wstr_init(&str2, 0), &str2);

  ASSERT_EQ(wstr_set(&str1, L"Hello"), RET_OK);
  ASSERT_EQ(wcs_case_cmp(str1.str, str2.str) == -1, TRUE);

  ASSERT_EQ(wstr_set(&str2, L"abc"), RET_OK);
  ASSERT_EQ(wcs_case_cmp(str1.str, str2.str) > 0, TRUE);

  ASSERT_EQ(wstr_set(&str2, L"zlg"), RET_OK);
  ASSERT_EQ(wcs_case_cmp(str1.str, str2.str) < 0, TRUE);

  ASSERT_EQ(wstr_set(&str2, L"hello"), RET_OK);
  ASSERT_EQ(wcs_case_cmp(str1.str, str2.str) == 0, TRUE);

  ASSERT_EQ(wstr_reset(&str1), RET_OK);
  ASSERT_EQ(wstr_reset(&str2), RET_OK);
}

TEST(WStr, attach0) {
  wstr_t s;
  wchar_t buff[32];
  wstr_attach(&s, buff, ARRAY_SIZE(buff));
  wstr_set(&s, L"abc");
  wstr_append(&s, L"123");
}

TEST(WStr, attach1) {
  wstr_t str;
  wchar_t buff[64];
  wstr_t* s = wstr_attach(&str, buff, ARRAY_SIZE(buff));
  ASSERT_EQ(s != NULL, true);

  wstr_set(s, L"T#");
  wstr_append_int(s, 1);
  wstr_append(s, L"d");

  wstr_append_int(s, 2);
  wstr_append(s, L"h");

  wstr_append_int(s, 3);
  wstr_append(s, L"s");
  ASSERT_EQ(wcscmp(buff, L"T#1d2h3s"), 0);
}

TEST(WStr, attach2) {
  wstr_t str;
  wchar_t buff[64];
  wstr_t* s = wstr_attach(&str, buff, ARRAY_SIZE(buff));
  ASSERT_EQ(s != NULL, true);

  wstr_set(s, L"T#");
  wstr_append_int(s, 1);
  wstr_append(s, L"d");

  wstr_append_int(s, 2);
  wstr_append(s, L"h");

  wstr_append_int(s, 3);
  wstr_append(s, L"s");

  wstr_reset(s);
  ASSERT_EQ(wcscmp(buff, L"T#1d2h3s"), 0);
}
