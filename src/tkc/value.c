﻿/**
 * File:   value.h
 * Author: AWTK Develop Team
 * Brief:  generic value type
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object.h"

bool_t value_bool(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      return v->value.b;
    }
    case VALUE_TYPE_STRING: {
      return tk_atob(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return tk_watob(v->value.wstr);
    }
    default: {
      return value_int(v) ? TRUE : FALSE;
    }
  }
}

value_t* value_init(value_t* v, uint32_t type) {
  v->type = type;
  v->free_handle = FALSE;

  return v;
}

value_t* value_set_bool(value_t* v, bool_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.b = value;
  return value_init(v, VALUE_TYPE_BOOL);
}

value_t* value_set_int8(value_t* v, int8_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i8 = value;
  return value_init(v, VALUE_TYPE_INT8);
}

int8_t value_int8(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT8) {
    return v->value.i8;
  } else {
    return (int8_t)value_int(v);
  }
}

value_t* value_set_uint8(value_t* v, uint8_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u8 = value;

  return value_init(v, VALUE_TYPE_UINT8);
}

uint8_t value_uint8(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT8) {
    return v->value.u8;
  } else {
    return (uint8_t)value_int(v);
  }
}

value_t* value_set_int16(value_t* v, int16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i16 = value;

  return value_init(v, VALUE_TYPE_INT16);
}

int16_t value_int16(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT16) {
    return v->value.i16;
  } else {
    return (int16_t)value_int(v);
  }
}

value_t* value_set_uint16(value_t* v, uint16_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u16 = value;

  return value_init(v, VALUE_TYPE_UINT16);
}

uint16_t value_uint16(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT16) {
    return v->value.u16;
  } else {
    return (uint16_t)value_int(v);
  }
}

value_t* value_set_int32(value_t* v, int32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i32 = value;
  return value_init(v, VALUE_TYPE_INT32);
}

int32_t value_int32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT32) {
    return v->value.i32;
  } else {
    return (int32_t)value_int(v);
  }
}

value_t* value_set_uint32(value_t* v, uint32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u32 = value;
  return value_init(v, VALUE_TYPE_UINT32);
}

uint32_t value_uint32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT32) {
    return v->value.u32;
  } else {
    return (uint32_t)value_int(v);
  }
}

value_t* value_set_int64(value_t* v, int64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.i64 = value;
  return value_init(v, VALUE_TYPE_INT64);
}

int64_t value_int64(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_INT64) {
    return v->value.i64;
  } else if (v->type == VALUE_TYPE_UINT64) {
    return v->value.u64;
  } else if (v->type == VALUE_TYPE_STRING) {
    return tk_atol(v->value.str);
  } else {
    return (int64_t)value_int(v);
  }
}

value_t* value_set_uint64(value_t* v, uint64_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.u64 = value;

  return value_init(v, VALUE_TYPE_UINT64);
}

uint64_t value_uint64(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_UINT64) {
    return v->value.u64;
  } else if (v->type == VALUE_TYPE_INT64) {
    return v->value.i64;
  } else if (v->type == VALUE_TYPE_STRING) {
    return tk_atoul(v->value.str);
  } else {
    return (uint64_t)value_int(v);
  }
}

value_t* value_set_pointer(value_t* v, void* value) {
  return value_set_pointer_ex(v, value, NULL);
}

value_t* value_set_pointer_ex(value_t* v, void* value, tk_destroy_t destroy) {
  return_value_if_fail(v != NULL, NULL);

  if (destroy != NULL) {
    pointer_ref_t* ref = TKMEM_ZALLOC(pointer_ref_t);
    return_value_if_fail(ref != NULL, NULL);
    ref->refcount = 1;
    ref->destroy = destroy;
    ref->data = value;
    v->value.ptr_ref = ref;
    value_init(v, VALUE_TYPE_POINTER_REF);

    v->free_handle = TRUE;
  } else {
    v->value.ptr = value;
    value_init(v, VALUE_TYPE_POINTER);
  }

  return v;
}

void* value_pointer(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  switch (v->type) {
    case VALUE_TYPE_STRING: {
      return (void*)(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (void*)(v->value.wstr);
    }
    case VALUE_TYPE_OBJECT: {
      return (v->value.object);
    }
    case VALUE_TYPE_POINTER: {
      return (v->value.ptr);
    }
    case VALUE_TYPE_POINTER_REF: {
      return (v->value.ptr_ref->data);
    }
    default:
      break;
  }

  return NULL;
}

value_t* value_set_float(value_t* v, float_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f = value;

  return value_init(v, VALUE_TYPE_FLOAT);
}

float_t value_float(const value_t* v) {
  return_value_if_fail(v != NULL, 0);
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      return (float_t)v->value.b;
    }
    case VALUE_TYPE_INT8: {
      return (float_t)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (float_t)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (float_t)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (float_t)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (float_t)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (float_t)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (float_t)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (float_t)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (float_t)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (float_t)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return (float_t)v->value.f64;
    }
    case VALUE_TYPE_STRING: {
      return (float_t)tk_atof(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (float_t)tk_watof(v->value.wstr);
    }
    default:
      break;
  }

  return 0;
}

value_t* value_set_float32(value_t* v, float value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f32 = value;

  return value_init(v, VALUE_TYPE_FLOAT32);
}

float value_float32(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  if (v->type == VALUE_TYPE_FLOAT32) {
    return v->value.f32;
  } else {
    return (float)value_float(v);
  }
}

value_t* value_set_double(value_t* v, double value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.f64 = value;

  return value_init(v, VALUE_TYPE_DOUBLE);
}

double value_double(const value_t* v) {
  return_value_if_fail(v != NULL, 0);

  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return (double)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (double)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (double)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (double)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (double)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (double)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (double)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (double)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (double)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (double)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return v->value.f64;
    }
    case VALUE_TYPE_STRING: {
      return (double)tk_atof(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (double)tk_watof(v->value.wstr);
    }
    case VALUE_TYPE_BOOL: {
      return value_bool(v) ? 1 : 0;
    }
    default:
      break;
  }

  return 0;
}

value_t* value_set_str(value_t* v, const char* value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.str = value;
  return value_init(v, VALUE_TYPE_STRING);
}

value_t* value_dup_str(value_t* v, const char* value) {
  return_value_if_fail(v != NULL, NULL);

  value_init(v, VALUE_TYPE_STRING);
  v->value.str = tk_strdup(value);
  v->free_handle = TRUE;

  return v;
}

value_t* value_dup_str_with_len(value_t* v, const char* value, uint32_t len) {
  return_value_if_fail(v != NULL, NULL);

  value_init(v, VALUE_TYPE_STRING);
  v->value.str = tk_strndup(value, len);
  v->free_handle = TRUE;

  return v;
}

value_t* value_set_wstr(value_t* v, const wchar_t* value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.wstr = value;
  return value_init(v, VALUE_TYPE_WSTRING);
}

value_t* value_dup_wstr(value_t* v, const wchar_t* value) {
  return_value_if_fail(v != NULL, NULL);

  value_init(v, VALUE_TYPE_WSTRING);
  v->value.wstr = tk_wstrdup(value);
  v->free_handle = TRUE;

  return v;
}

const char* value_str(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_STRING, NULL);

  return v->value.str;
}

const wchar_t* value_wstr(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_WSTRING, NULL);

  return v->value.wstr;
}

ret_t value_copy(value_t* dst, const value_t* src) {
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  if (dst == src) {
    return RET_OK;
  }

  memcpy(dst, src, sizeof(value_t));
  dst->free_handle = FALSE;

  return RET_OK;
}

ret_t value_deep_copy(value_t* dst, const value_t* src) {
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  value_copy(dst, src);

  switch (dst->type) {
    case VALUE_TYPE_STRING: {
      dst->value.str = src->value.str != NULL ? tk_strdup(src->value.str) : NULL;
      dst->free_handle = dst->value.str != NULL;
      break;
    }
    case VALUE_TYPE_BINARY:
    case VALUE_TYPE_GRADIENT:
    case VALUE_TYPE_UBJSON: {
      if (src->value.binary_data.data != NULL) {
        uint32_t size = src->value.binary_data.size;
        void* data = tk_memdup(src->value.binary_data.data, size);
        return_value_if_fail(data != NULL, RET_OOM);

        dst->free_handle = TRUE;
        dst->value.binary_data.data = data;
        dst->value.binary_data.size = size;
      } else {
        dst->free_handle = FALSE;
      }
      break;
    }
    case VALUE_TYPE_WSTRING: {
      dst->value.wstr = tk_wstrdup(src->value.wstr);
      dst->free_handle = dst->value.wstr != NULL;
      break;
    }
    case VALUE_TYPE_OBJECT: {
      tk_object_ref(dst->value.object);
      dst->free_handle = dst->value.object != NULL;
      break;
    }
    case VALUE_TYPE_POINTER_REF: {
      dst->value.ptr_ref->refcount++;
      dst->free_handle = TRUE;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

bool_t value_is_null(value_t* v) {
  return v == NULL || v->type == VALUE_TYPE_INVALID;
}

int value_int(const value_t* v) {
  return_value_if_fail(v->type != VALUE_TYPE_INVALID, 0);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return (int)v->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return (int)v->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return (int)v->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return (int)v->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return (int)v->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return (int)v->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return (int)v->value.i64;
    }
    case VALUE_TYPE_UINT64: {
      return (int)v->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return (int)v->value.f;
    }
    case VALUE_TYPE_FLOAT32: {
      return (int)v->value.f32;
    }
    case VALUE_TYPE_DOUBLE: {
      return (int)v->value.f64;
    }
    case VALUE_TYPE_BOOL: {
      return (int)v->value.b;
    }
    case VALUE_TYPE_STRING: {
      return tk_atoi(v->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return tk_watoi(v->value.wstr);
    }
    default: {
      assert(!"not supported type");
    }
  }

  return 0;
}

bool_t value_equal(const value_t* v, const value_t* other) {
  return_value_if_fail(v != NULL && other != NULL, FALSE);
  if (v->type != other->type) {
    return FALSE;
  }

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      return v->value.i8 == other->value.i8;
    }
    case VALUE_TYPE_UINT8: {
      return v->value.u8 == other->value.u8;
    }
    case VALUE_TYPE_INT16: {
      return v->value.i16 == other->value.i16;
    }
    case VALUE_TYPE_UINT16: {
      return v->value.u16 == other->value.u16;
    }
    case VALUE_TYPE_INT32: {
      return v->value.i32 == other->value.i32;
    }
    case VALUE_TYPE_UINT32: {
      return v->value.u32 == other->value.u32;
    }
    case VALUE_TYPE_INT64: {
      return v->value.i64 == other->value.i64;
    }
    case VALUE_TYPE_BOOL: {
      return v->value.b == other->value.b;
    }
    case VALUE_TYPE_POINTER: {
      return v->value.ptr == other->value.ptr;
    }
    case VALUE_TYPE_POINTER_REF: {
      return v->value.ptr_ref == other->value.ptr_ref;
    }
    case VALUE_TYPE_UINT64: {
      return v->value.u64 == other->value.u64;
    }
    case VALUE_TYPE_FLOAT: {
      return tk_fequal(v->value.f, other->value.f32);
    }
    case VALUE_TYPE_FLOAT32: {
      return tk_fequal(v->value.f32, other->value.f32);
    }
    case VALUE_TYPE_DOUBLE: {
      return tk_lfequal(v->value.f64, other->value.f64);
    }
    case VALUE_TYPE_STRING: {
      return (v->value.str == other->value.str) || tk_str_eq(v->value.str, other->value.str);
    }
    case VALUE_TYPE_WSTRING: {
      return (v->value.wstr == other->value.wstr) || tk_wstr_eq(v->value.wstr, other->value.wstr);
    }
    case VALUE_TYPE_BINARY:
    case VALUE_TYPE_GRADIENT:
    case VALUE_TYPE_UBJSON: {
      return (v->value.binary_data.data == other->value.binary_data.data);
    }
    case VALUE_TYPE_OBJECT: {
      return tk_object_compare(v->value.object, other->value.object) == 0;
    }
    default:
      break;
  }

  return FALSE;
}

value_t* value_set_int(value_t* v, int32_t value) {
  return value_set_int32(v, value);
}

value_t* value_create() {
  return TKMEM_ZALLOC(value_t);
}

ret_t value_reset(value_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  if (v->free_handle) {
    switch (v->type) {
      case VALUE_TYPE_SIZED_STRING: {
        TKMEM_FREE(v->value.sized_str.str);
        break;
      }
      case VALUE_TYPE_BINARY:
      case VALUE_TYPE_UBJSON:
      case VALUE_TYPE_GRADIENT: {
        TKMEM_FREE(v->value.binary_data.data);
        break;
      }
      case VALUE_TYPE_STRING: {
        TKMEM_FREE(v->value.str);
        break;
      }
      case VALUE_TYPE_WSTRING: {
        TKMEM_FREE(v->value.wstr);
        break;
      }
      case VALUE_TYPE_OBJECT: {
        tk_object_t* obj = v->value.object;
        v->value.object = NULL;
        TK_OBJECT_UNREF(obj);
        break;
      }
      case VALUE_TYPE_ID: {
        TKMEM_FREE(v->value.id.id);
        break;
      }
      case VALUE_TYPE_FUNC: {
        TKMEM_FREE(v->value.func.func);
        break;
      }
      case VALUE_TYPE_POINTER_REF: {
        assert(v->value.ptr_ref->refcount >= 1);

        v->value.ptr_ref->refcount--;
        if (v->value.ptr_ref->refcount == 0) {
          v->value.ptr_ref->destroy(v->value.ptr_ref->data);
          TKMEM_FREE(v->value.ptr_ref);
          v->value.ptr_ref = NULL;
        }
        break;
      }
      default:
        break;
    }
  }

  memset(v, 0x00, sizeof(value_t));

  return RET_OK;
}

ret_t value_destroy(value_t* v) {
  return_value_if_fail(v != NULL, RET_BAD_PARAMS);

  value_reset(v);
  TKMEM_FREE(v);

  return RET_OK;
}

value_t* value_set_object(value_t* v, tk_object_t* value) {
  return_value_if_fail(v != NULL && value != NULL, NULL);

  v->value.object = value;
  return value_init(v, VALUE_TYPE_OBJECT);
}

tk_object_t* value_object(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_OBJECT, NULL);

  return v->value.object;
}

value_t* value_cast(value_t* value) {
  return_value_if_fail(value != NULL, NULL);

  return value;
}

value_t* value_set_token(value_t* v, uint32_t value) {
  return_value_if_fail(v != NULL, NULL);

  v->value.token = value;

  return value_init(v, VALUE_TYPE_TOKEN);
}

uint32_t value_token(const value_t* v) {
  return_value_if_fail(v != NULL, 0);
  return_value_if_fail(v->type == VALUE_TYPE_TOKEN, 0);

  return v->value.token;
}

value_t* value_set_sized_str(value_t* v, char* str, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.sized_str.str = str;
  v->value.sized_str.size = size;

  return value_init(v, VALUE_TYPE_SIZED_STRING);
}

sized_str_t* value_sized_str(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_SIZED_STRING, NULL);

  return (sized_str_t*)&(v->value.sized_str);
}

value_t* value_set_binary_data(value_t* v, void* data, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.binary_data.data = data;
  v->value.binary_data.size = size;

  return value_init(v, VALUE_TYPE_BINARY);
}

value_t* value_dup_binary_data(value_t* v, const void* data, uint32_t size) {
  void* new_data = NULL;
  return_value_if_fail(v != NULL && data != NULL, NULL);

  new_data = TKMEM_ALLOC(size);
  return_value_if_fail(new_data != NULL, NULL);
  memcpy(new_data, data, size);

  value_init(v, VALUE_TYPE_BINARY);
  v->value.binary_data.data = new_data;
  v->value.binary_data.size = size;
  v->free_handle = TRUE;

  return v;
}

binary_data_t* value_binary_data(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_BINARY || v->type == VALUE_TYPE_GRADIENT ||
                           v->type == VALUE_TYPE_UBJSON,
                       NULL);

  return (binary_data_t*)&(v->value.binary_data);
}

value_t* value_set_ubjson(value_t* v, void* data, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.binary_data.data = data;
  v->value.binary_data.size = size;

  return value_init(v, VALUE_TYPE_UBJSON);
}

binary_data_t* value_ubjson(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_UBJSON, NULL);

  return (binary_data_t*)&(v->value.binary_data);
}

value_t* value_set_gradient(value_t* v, void* data, uint32_t size) {
  return_value_if_fail(v != NULL, NULL);

  v->value.binary_data.data = data;
  v->value.binary_data.size = size;

  return value_init(v, VALUE_TYPE_GRADIENT);
}

binary_data_t* value_gradient(const value_t* v) {
  return_value_if_fail(v != NULL, NULL);
  return_value_if_fail(v->type == VALUE_TYPE_GRADIENT, NULL);

  return (binary_data_t*)&(v->value.binary_data);
}

const char* value_str_ex(const value_t* v, char* buff, uint32_t size) {
  return_value_if_fail(v != NULL && buff != NULL && size > 0, NULL);

  if (v->type == VALUE_TYPE_STRING) {
    return value_str(v);
  } else if (v->type == VALUE_TYPE_DOUBLE || v->type == VALUE_TYPE_FLOAT32 ||
             v->type == VALUE_TYPE_FLOAT) {
    tk_snprintf(buff, size, "%lf", value_double(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    tk_utf8_from_utf16(value_wstr(v), buff, size);
  } else if (v->type == VALUE_TYPE_BOOL) {
    tk_snprintf(buff, size, "%s", value_bool(v) ? "true" : "false");
  } else if (v->type == VALUE_TYPE_UINT8 || v->type == VALUE_TYPE_UINT16 ||
             v->type == VALUE_TYPE_UINT32) {
    tk_snprintf(buff, size, "%u", value_uint32(v));
  } else if (v->type == VALUE_TYPE_UINT64) {
    tk_snprintf(buff, size, "%" PRIu64, value_uint64(v));
  } else if (v->type == VALUE_TYPE_INT64) {
    tk_snprintf(buff, size, "%" PRId64, value_int64(v));
  } else if (v->type == VALUE_TYPE_ID) {
    assert(v->value.id.id != NULL);
    return v->value.id.id;
  } else if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    if (bin != NULL) {
      tk_snprintf(buff, size, "binary(%p:%u)", bin->data, bin->size);
    } else {
      tk_snprintf(buff, size, "(null)");
    }
  } else if (v->type == VALUE_TYPE_FUNC) {
    return "func";
  } else if (v->type == VALUE_TYPE_FUNC_DEF) {
    return "func_def";
  } else if (v->type == VALUE_TYPE_POINTER || v->type == VALUE_TYPE_POINTER_REF) {
    tk_snprintf(buff, size, "%p", value_pointer(v));
  } else if (v->type == VALUE_TYPE_OBJECT) {
    tk_object_t* obj = value_object(v);
    if (obj != NULL) {
      tk_snprintf(buff, size, "object(%p:%s)", obj, obj->vt->type);
    }
  } else if (v->type == VALUE_TYPE_INVALID) {
    *buff = '\0';
    return TK_VALUE_UNDEFINED;
  } else {
    tk_snprintf(buff, size, "%d", value_int(v));
  }

  return buff;
}

uint32_t value_type_size(value_type_t type) {
  switch (type) {
    case VALUE_TYPE_BOOL: {
      return sizeof(bool_t);
    }
    case VALUE_TYPE_INT8: {
      return sizeof(int8_t);
    }
    case VALUE_TYPE_UINT8: {
      return sizeof(uint8_t);
    }
    case VALUE_TYPE_INT16: {
      return sizeof(int16_t);
    }
    case VALUE_TYPE_UINT16: {
      return sizeof(uint16_t);
    }
    case VALUE_TYPE_INT32: {
      return sizeof(int32_t);
    }
    case VALUE_TYPE_UINT32: {
      return sizeof(uint32_t);
    }
    case VALUE_TYPE_INT64: {
      return sizeof(int64_t);
    }
    case VALUE_TYPE_UINT64: {
      return sizeof(uint64_t);
    }
    case VALUE_TYPE_FLOAT: {
      return sizeof(float_t);
    }
    case VALUE_TYPE_FLOAT32: {
      return sizeof(float);
    }
    case VALUE_TYPE_DOUBLE: {
      return sizeof(double);
    }
    case VALUE_TYPE_OBJECT:
    case VALUE_TYPE_STRING:
    case VALUE_TYPE_WSTRING:
    case VALUE_TYPE_POINTER:
    case VALUE_TYPE_POINTER_REF: {
      return sizeof(void*);
    }
    case VALUE_TYPE_BINARY: {
      return sizeof(binary_data_t);
    }
    case VALUE_TYPE_SIZED_STRING: {
      return sizeof(sized_str_t);
    }
    default: {
      return sizeof(void*);
    }
  }
}

const char* value_id(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_ID, NULL);

  return v->value.id.id;
}

value_t* value_set_id(value_t* v, const char* value, uint32_t len) {
  return_value_if_fail(v != NULL && value != NULL, NULL);
  v->value.id.id = tk_strndup(value, len);
  v->value.id.index = -1;
  v->value.id.suboffset = 0;
  value_init(v, VALUE_TYPE_ID);
  v->free_handle = TRUE;

  return v;
}

void* value_func(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_FUNC, NULL);

  return v->value.func.func;
}

value_t* value_set_func(value_t* v, void* value) {
  return_value_if_fail(v != NULL && value != NULL, NULL);

  v->value.func.func = value;
  v->value.func.memo = 0;

  return value_init(v, VALUE_TYPE_FUNC);
}

void* value_func_def(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_FUNC_DEF, NULL);

  return v->value.ptr;
}

value_t* value_set_func_def(value_t* v, void* value) {
  return_value_if_fail(v != NULL && value != NULL, NULL);

  v->value.ptr = value;

  return value_init(v, VALUE_TYPE_FUNC_DEF);
}

void* value_bitmap(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_BITMAP, NULL);

  return v->value.bitmap;
}

value_t* value_set_bitmap(value_t* v, void* bitmap) {
  return_value_if_fail(v != NULL && bitmap != NULL, NULL);

  v->value.bitmap = bitmap;

  return value_init(v, VALUE_TYPE_BITMAP);
}

rect_t* value_rect(const value_t* v) {
  return_value_if_fail(v != NULL && v->type == VALUE_TYPE_RECT, NULL);

  return (rect_t*)(&(v->value.rect));
}

value_t* value_set_rect(value_t* v, rect_t r) {
  return_value_if_fail(v != NULL, NULL);

  v->value.rect = r;

  return value_init(v, VALUE_TYPE_RECT);
}

/*operations*/

ret_t value_lshift(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      value_set_int8(result, value_int8(v) << n);
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(result, value_uint8(v) << n);
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(result, value_int16(v) << n);
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(result, value_uint16(v) << n);
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(result, value_int32(v) << n);
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(result, value_uint32(v) << n);
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(result, value_int64(v) << n);
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(result, value_uint64(v) << n);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_rshift(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      value_set_int8(result, value_int8(v) >> n);
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(result, value_uint8(v) >> n);
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(result, value_int16(v) >> n);
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(result, value_uint16(v) >> n);
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(result, value_int32(v) >> n);
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(result, value_uint32(v) >> n);
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(result, value_int64(v) >> n);
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(result, value_uint64(v) >> n);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_lshift_r(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      n = n % 8;
      value_set_int8(result, (vv >> (8 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      n = n % 8;
      value_set_uint8(result, (vv >> (8 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      n = n % 16;
      value_set_int16(result, (vv >> (16 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      n = n % 16;
      value_set_uint16(result, (vv >> (16 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      n = n % 32;
      value_set_int32(result, (vv >> (32 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      n = n % 32;
      value_set_uint32(result, (vv >> (32 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      n = n % 64;
      value_set_int64(result, (vv >> (64 - n)) | (vv << n));
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      n = n % 64;
      value_set_uint64(result, (vv >> (64 - n)) | (vv << n));
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_rshift_r(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      n = n % 8;
      value_set_int8(result, (vv << (8 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      n = n % 8;
      value_set_uint8(result, (vv << (8 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      n = n % 16;
      value_set_int16(result, (vv << (16 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      n = n % 16;
      value_set_uint16(result, (vv << (16 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      n = n % 32;
      value_set_int32(result, (vv << (32 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      n = n % 32;
      value_set_uint32(result, (vv << (32 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      n = n % 64;
      value_set_int64(result, (vv << (64 - n)) | (vv >> n));
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      n = n % 64;
      value_set_uint64(result, (vv << (64 - n)) | (vv >> n));
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_get_bit(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      bool_t bit = TK_TEST_BIT(vv, n);
      value_set_bool(result, bit);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

#define TK_SET_OR_CLEAR_BIT(v, n, b) \
  if (b) {                           \
    TK_SET_BIT(v, n);                \
  } else {                           \
    TK_CLEAR_BIT(v, n);              \
  }

ret_t value_set_bit(value_t* v, value_t* result, uint32_t n, bool_t bit) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      TK_SET_OR_CLEAR_BIT(vv, n, bit);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_toggle_bit(value_t* v, value_t* result, uint32_t n) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      TK_TOGGLE_BIT(vv, n);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_bit_not(value_t* v, value_t* result) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      bool_t vv = value_bool(v);
      value_set_bool(result, !vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      value_set_int8(result, ~vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      value_set_uint8(result, ~vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      value_set_int16(result, ~vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      value_set_uint16(result, ~vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      value_set_int32(result, ~vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      value_set_uint32(result, ~vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      value_set_int64(result, ~vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      value_set_uint64(result, ~vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_bit_or(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      bool_t vv = value_bool(v) | value_bool(other);
      value_set_bool(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v) | value_int8(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v) | value_uint8(other);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v) | value_int16(other);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v) | value_uint16(other);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) | value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) | value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) | value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) | value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_bit_and(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      bool_t vv = value_bool(v) & value_bool(other);
      value_set_bool(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v) & value_int8(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v) & value_uint8(other);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v) & value_int16(other);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v) & value_uint16(other);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) & value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) & value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) & value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) & value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_bit_xor(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      bool_t vv = value_bool(v) ^ value_bool(other);
      value_set_bool(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v) ^ value_int8(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v) ^ value_uint8(other);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v) ^ value_int16(other);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v) ^ value_uint16(other);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) ^ value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) ^ value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) ^ value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) ^ value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_abs(value_t* v, value_t* result) {
  ret_t ret = RET_OK;
  return_value_if_fail(v != NULL && result != NULL, RET_BAD_PARAMS);

  switch (v->type) {
    case VALUE_TYPE_BOOL: {
      bool_t vv = value_bool(v);
      value_set_bool(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v);
      value_set_int8(result, abs(vv));
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v);
      value_set_uint8(result, (vv));
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v);
      value_set_int16(result, abs(vv));
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v);
      value_set_uint16(result, (vv));
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v);
      value_set_int32(result, abs(vv));
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v);
      value_set_uint32(result, (vv));
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v);
      value_set_int64(result, vv > 0 ? vv : -vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v);
      value_set_uint64(result, (vv));
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      float vv = value_float32(v);
      value_set_float32(result, vv >= 0 ? vv : -vv);
      break;
    }
    case VALUE_TYPE_FLOAT: {
      float_t vv = value_float(v);
      value_set_float(result, vv >= 0 ? vv : -vv);
      break;
    }
    case VALUE_TYPE_DOUBLE: {
      double vv = value_double(v);
      value_set_double(result, vv >= 0 ? vv : -vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, v->type);
      break;
    }
  }

  return ret;
}

ret_t value_add(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      int8_t vv = value_bool(v) + value_bool(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int16_t vv = value_int8(v) + value_int8(other);
      if (vv >= INT8_MIN && vv <= INT8_MAX) {
        value_set_int8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT8: {
      int16_t vv = value_uint8(v) + value_uint8(other);
      if (vv >= 0 && vv <= UINT8_MAX) {
        value_set_uint8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT16: {
      int32_t vv = value_int16(v) + value_int16(other);
      if (vv >= INT16_MIN && vv <= INT16_MAX) {
        value_set_int16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT16: {
      int32_t vv = value_uint16(v) + value_uint16(other);
      if (vv >= 0 && vv <= UINT16_MAX) {
        value_set_uint16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) + value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) + value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) + value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) + value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      double vv = value_float32(v) + value_float32(other);
      value_set_float32(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      double vv = value_double(v) + value_double(other);
      value_set_double(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, type);
      break;
    }
  }

  return ret;
}

ret_t value_sub(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      int8_t vv = value_bool(v) - value_bool(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int16_t vv = value_int8(v) - value_int8(other);
      if (vv >= INT8_MIN && vv <= INT8_MAX) {
        value_set_int8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT8: {
      int16_t vv = value_uint8(v) - value_uint8(other);
      if (vv >= 0 && vv <= UINT8_MAX) {
        value_set_uint8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT16: {
      int32_t vv = value_int16(v) - value_int16(other);
      if (vv >= INT16_MIN && vv <= INT16_MAX) {
        value_set_int16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT16: {
      int32_t vv = value_uint16(v) - value_uint16(other);
      if (vv >= 0 && vv <= UINT16_MAX) {
        value_set_uint16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) - value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) - value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) - value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) - value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      double vv = value_float32(v) - value_float32(other);
      value_set_float32(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      double vv = value_double(v) - value_double(other);
      value_set_double(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, type);
      break;
    }
  }

  return ret;
}

ret_t value_mul(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      int8_t vv = value_bool(v) * value_bool(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int16_t vv = value_int8(v) * value_int8(other);
      if (vv >= INT8_MIN && vv <= INT8_MAX) {
        value_set_int8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT8: {
      int16_t vv = value_uint8(v) * value_uint8(other);
      if (vv >= 0 && vv <= UINT8_MAX) {
        value_set_uint8(result, vv);
      } else {
        value_set_int16(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT16: {
      int32_t vv = value_int16(v) * value_int16(other);
      if (vv >= INT16_MIN && vv <= INT16_MAX) {
        value_set_int16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_UINT16: {
      int32_t vv = value_uint16(v) * value_uint16(other);
      if (vv >= 0 && vv <= UINT16_MAX) {
        value_set_uint16(result, vv);
      } else {
        value_set_int32(result, vv);
      }
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) * value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) * value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) * value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) * value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      double vv = value_float32(v) * value_float32(other);
      value_set_float32(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      double vv = value_double(v) * value_double(other);
      value_set_double(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, type);
      break;
    }
  }

  return ret;
}

ret_t value_div(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      int8_t vv = value_bool(v) / value_bool(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v) / value_int8(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v) / value_uint8(other);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v) / value_int16(other);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v) / value_uint16(other);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) / value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) / value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) / value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) / value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT32: {
      double vv = value_float32(v) / value_float32(other);
      value_set_float32(result, vv);
      break;
    }
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_DOUBLE: {
      double vv = value_double(v) / value_double(other);
      value_set_double(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, type);
      break;
    }
  }

  return ret;
}

ret_t value_mod(value_t* v, value_t* other, value_t* result) {
  ret_t ret = RET_OK;
  uint32_t type = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  type = tk_max_int((int)(v->type), (int)(other->type));
  switch (type) {
    case VALUE_TYPE_BOOL: {
      int8_t vv = value_bool(v) % value_bool(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_INT8: {
      int8_t vv = value_int8(v) % value_int8(other);
      value_set_int8(result, vv);
      break;
    }
    case VALUE_TYPE_UINT8: {
      uint8_t vv = value_uint8(v) % value_uint8(other);
      value_set_uint8(result, vv);
      break;
    }
    case VALUE_TYPE_INT16: {
      int16_t vv = value_int16(v) % value_int16(other);
      value_set_int16(result, vv);
      break;
    }
    case VALUE_TYPE_UINT16: {
      uint16_t vv = value_uint16(v) % value_uint16(other);
      value_set_uint16(result, vv);
      break;
    }
    case VALUE_TYPE_INT32: {
      int32_t vv = value_int32(v) % value_int32(other);
      value_set_int32(result, vv);
      break;
    }
    case VALUE_TYPE_UINT32: {
      uint32_t vv = value_uint32(v) % value_uint32(other);
      value_set_uint32(result, vv);
      break;
    }
    case VALUE_TYPE_INT64: {
      int64_t vv = value_int64(v) % value_int64(other);
      value_set_int64(result, vv);
      break;
    }
    case VALUE_TYPE_UINT64: {
      uint64_t vv = value_uint64(v) % value_uint64(other);
      value_set_uint64(result, vv);
      break;
    }
    default: {
      ret = RET_BAD_PARAMS;
      log_debug("%s not supported type:%d\n", __FUNCTION__, type);
      break;
    }
  }

  return ret;
}

ret_t value_expt(value_t* v, value_t* other, value_t* result) {
  int32_t e = 0;
  uint32_t n = 0;
  double b = 0;
  double r = 0;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(v != NULL && other != NULL, RET_BAD_PARAMS);

  b = value_double(v);
  if (tk_fequal(b, 0)) {
    value_set_double(result, 0);
    return RET_OK;
  }

  e = value_int(other);
  if (e == 0) {
    value_set_double(result, 1);
    return RET_OK;
  } else if (e < 0) {
    n = -e;
  } else {
    n = e;
  }

  r = b;
  n--;
  /*TODO: optimize*/
  while (n > 0) {
    r = r * b;
    n--;
  }

  if (e < 0) {
    if (tk_fequal(r, 0)) {
      value_set_double(result, INFINITY);
    } else {
      value_set_double(result, 1 / r);
    }
  } else {
    value_set_double(result, r);
  }

  return RET_OK;
}

ret_t value_min(value_t* arr, uint32_t size, value_t* result) {
  uint32_t i = 0;
  uint32_t r = 0;
  uint32_t type = 0;
  return_value_if_fail(arr != NULL && size > 0 && result != NULL, RET_BAD_PARAMS);

  type = arr[0].type;

  if (type >= VALUE_TYPE_INT8 && type < VALUE_TYPE_UINT64) {
    for (i = 0; i < size; i++) {
      if (value_int64(arr + i) < value_int64(arr + r)) {
        r = i;
      }
    }

    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_UINT64) {
    for (i = 0; i < size; i++) {
      if (value_uint64(arr + i) < value_uint64(arr + r)) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_STRING) {
    for (i = 0; i < size; i++) {
      if (tk_strcmp(value_str(arr + i), value_str(arr + r)) < 0) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_WSTRING) {
    for (i = 0; i < size; i++) {
      if (tk_wstrcmp(value_wstr(arr + i), value_wstr(arr + r)) < 0) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type >= VALUE_TYPE_FLOAT && type <= VALUE_TYPE_DOUBLE) {
    for (i = 0; i < size; i++) {
      if (value_double(arr + i) < value_double(arr + r)) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else {
    log_warn("not impl for this type");
    return RET_NOT_IMPL;
  }

  return RET_OK;
}

ret_t value_max(value_t* arr, uint32_t size, value_t* result) {
  uint32_t i = 0;
  uint32_t r = 0;
  uint32_t type = 0;
  return_value_if_fail(arr != NULL && size > 0 && result != NULL, RET_BAD_PARAMS);

  type = arr[0].type;

  if (type >= VALUE_TYPE_INT8 && type < VALUE_TYPE_UINT64) {
    for (i = 0; i < size; i++) {
      if (value_int64(arr + i) > value_int64(arr + r)) {
        r = i;
      }
    }

    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_UINT64) {
    for (i = 0; i < size; i++) {
      if (value_uint64(arr + i) > value_uint64(arr + r)) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_STRING) {
    for (i = 0; i < size; i++) {
      if (tk_strcmp(value_str(arr + i), value_str(arr + r)) > 0) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type == VALUE_TYPE_WSTRING) {
    for (i = 0; i < size; i++) {
      if (tk_wstrcmp(value_wstr(arr + i), value_wstr(arr + r)) > 0) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else if (type >= VALUE_TYPE_FLOAT && type <= VALUE_TYPE_DOUBLE) {
    for (i = 0; i < size; i++) {
      if (value_double(arr + i) > value_double(arr + r)) {
        r = i;
      }
    }
    return value_copy(result, arr + r);
  } else {
    log_warn("not impl for this type");
    return RET_NOT_IMPL;
  }

  return RET_OK;
}
