﻿#include "tkc/value.h"
#include "gtest/gtest.h"
#include "tkc/object_default.h"

TEST(value, i8) {
  value_t v;
  ASSERT_EQ(&v, value_set_int8(&v, 10));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, stri8) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, wstri8) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int8(&v), 10);
}

TEST(value, u8) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint8(&v, 10));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, stru8) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, wstru8) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint8(&v), 10);
}

TEST(value, i16) {
  value_t v;
  ASSERT_EQ(&v, value_set_int16(&v, 10));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, stri16) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, wstri16) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int16(&v), 10);
}

TEST(value, u16) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint16(&v, 10));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, stru16) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, wstru16) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint16(&v), 10);
}

TEST(value, i32) {
  value_t v;
  ASSERT_EQ(&v, value_set_int32(&v, 10));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, stri32) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, wstri32) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int32(&v), 10);
}

TEST(value, u32) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint32(&v, 10));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, stru32) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, wstru32) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint32(&v), 10u);
}

TEST(value, i64) {
  value_t v;
  ASSERT_EQ(&v, value_set_int64(&v, 10));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, stri64) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, wstri64) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_int64(&v), 10);
}

TEST(value, u64) {
  value_t v;
  ASSERT_EQ(&v, value_set_uint64(&v, 10));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, stru64) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, wstru64) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_uint64(&v), 10u);
}

TEST(value, float) {
  value_t v;
  ASSERT_EQ(&v, value_set_float(&v, 10));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, strfloat) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, wstrfloat) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_float(&v), 10.0f);
}

TEST(value, double) {
  value_t v;
  ASSERT_EQ(&v, value_set_double(&v, 10));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(value, bool_to_double) {
  value_t v;
  ASSERT_EQ(&v, value_set_bool(&v, true));
  ASSERT_EQ(value_double(&v), 1.0);
}

TEST(value, strdouble) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "10"));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(value, wstrdouble) {
  value_t v;
  ASSERT_EQ(&v, value_set_wstr(&v, L"10"));
  ASSERT_EQ(value_double(&v), 10.0);
}

TEST(ValueTest, str) {
  value_t v;
  const char* str = "str";

  ASSERT_EQ(&v, value_set_str(&v, str));
  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
}

TEST(ValueTest, sized_str) {
  value_t v;
  const char* str = "str";
  sized_str_t* sized_str = NULL;
  ASSERT_EQ(&v, value_set_sized_str(&v, (char*)str, 2));
  sized_str = value_sized_str(&v);
  ASSERT_EQ(sized_str != NULL, true);
  ASSERT_EQ(sized_str->size, 2u);
}

TEST(ValueTest, binary_data) {
  value_t v;
  const char* str = "str";
  binary_data_t* binary_data = NULL;
  ASSERT_EQ(&v, value_set_binary_data(&v, (void*)str, 2));
  binary_data = value_binary_data(&v);
  ASSERT_EQ(binary_data != NULL, true);
  ASSERT_EQ(binary_data->size, 2u);
}

TEST(ValueTest, token) {
  value_t v;

  ASSERT_EQ(&v, value_set_token(&v, 123));
  ASSERT_EQ(value_token(&v), 123u);
}

TEST(ValueTest, wstr) {
  value_t v;
  const wchar_t* str = L"str";

  ASSERT_EQ(&v, value_set_wstr(&v, str));
  ASSERT_EQ(wcscmp(value_wstr(&v), L"str"), 0);
}

TEST(ValueTest, object) {
  value_t v;
  tk_object_t* o = object_default_create();

  ASSERT_EQ(&v, value_set_object(&v, o));
  ASSERT_EQ(o, value_object(&v));

  tk_object_unref(o);
}

TEST(value, int) {
  value_t v;
  ASSERT_EQ(value_int(value_set_uint64(&v, 10)), 10);
}

TEST(value, id) {
  value_t v;
  ASSERT_STREQ(value_id(value_set_id(&v, "abc", 3)), "abc");
  value_reset(&v);
}

TEST(value, func) {
  value_t v;
  ASSERT_STREQ((char*)value_func(value_set_func(&v, (void*)"abc")), "abc");
  value_reset(&v);
}

TEST(ValueTest, copy) {
  value_t v;
  value_t other;
  const char* str = "str";

  ASSERT_EQ(&other, value_set_str(&other, str));
  ASSERT_EQ(value_copy(&v, &other), RET_OK);

  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
  ASSERT_EQ(v.value.str, other.value.str);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, deepcopy) {
  value_t v;
  value_t other;
  const char* str = "str";

  ASSERT_EQ(&other, value_set_str(&other, str));
  ASSERT_EQ(value_deep_copy(&v, &other), RET_OK);

  ASSERT_EQ(strcmp(value_str(&v), "str"), 0);
  ASSERT_NE(v.value.str, other.value.str);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, deepcopy_binary) {
  value_t v;
  value_t other;
  const char* str = "str";
  binary_data_t* bin1 = NULL;
  binary_data_t* bin2 = NULL;

  ASSERT_EQ(&other, value_set_binary_data(&other, (void*)str, 4));
  ASSERT_EQ(value_deep_copy(&v, &other), RET_OK);

  bin1 = value_binary_data(&v);
  bin2 = value_binary_data(&other);

  ASSERT_STREQ((const char*)(bin1->data), str);
  ASSERT_STREQ((const char*)(bin2->data), str);
  ASSERT_EQ(bin1->data != bin2->data, true);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, copy_str) {
  value_t v;
  value_t other;

  ASSERT_EQ(value_dup_str(&v, "hello"), &v);
  ASSERT_EQ(value_copy(&other, &v), RET_OK);

  value_reset(&v);
  value_reset(&other);
}

TEST(ValueTest, ubjson) {
  value_t v;
  const char* str = "str";
  binary_data_t* ubjson = NULL;
  ASSERT_EQ(&v, value_set_ubjson(&v, (void*)str, 2));
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_UBJSON);

  ubjson = value_ubjson(&v);
  ASSERT_EQ(ubjson != NULL, true);
  ASSERT_EQ(ubjson->size, 2u);
}

TEST(ValueTest, gradient) {
  value_t v;
  const char* str = "str";
  binary_data_t* gradient = NULL;
  ASSERT_EQ(&v, value_set_gradient(&v, (void*)str, 2));
  ASSERT_EQ(v.type, (uint32_t)VALUE_TYPE_GRADIENT);

  gradient = value_gradient(&v);
  ASSERT_EQ(gradient != NULL, true);
  ASSERT_EQ(gradient->size, 2u);
}

TEST(value, i64_from_str) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "12345678912345"));
  ASSERT_EQ(value_int64(&v), 12345678912345);

  ASSERT_EQ(&v, value_set_str(&v, "-12345678912345"));
  ASSERT_EQ(value_int64(&v), -12345678912345);
}

TEST(value, ui64_from_str) {
  value_t v;
  ASSERT_EQ(&v, value_set_str(&v, "12345678912345"));
  ASSERT_EQ(value_uint64(&v), 12345678912345);
}

#include "tkc/utils.h"

TEST(value, pointer) {
  value_t v;
  char* str = tk_strdup("abc");
  ASSERT_EQ(&v, value_set_pointer_ex(&v, str, default_destroy));
  ASSERT_STREQ((char*)value_pointer(&v), "abc");
  value_reset(&v);
}

TEST(value, bitmap) {
  value_t v;
  void* bitmap = &v;
  ASSERT_EQ(value_bitmap(value_set_bitmap(&v, bitmap)), bitmap);
}
